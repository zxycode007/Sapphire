#include "SapphireCMeshManipulator.h"
#include "SapphireSMesh.h"
#include "SapphireCMeshBuffer.h"
#include "SapphireSAnimatedMesh.h"
#include "SapphireOS.h"

namespace Sapphire
{

	static inline Vector3 getAngleWeight(const Vector3& v1,
		const Vector3& v2,
		const Vector3& v3)
	{
		// Calculate this triangle's weight for each of its three vertices
		// start by calculating the lengths of its sides
		const FLOAT32 a = v2.squaredDistance(v3);
		const FLOAT32 asqrt = sqrtf(a);
		const FLOAT32 b = v1.squaredDistance(v3);
		const FLOAT32 bsqrt = sqrtf(b);
		const FLOAT32 c = v1.squaredDistance(v2);
		const FLOAT32 csqrt = sqrtf(c);

		// use them to find the angle at each vertex
		return Vector3(
			acosf((b + c - a) / (2.f * bsqrt * csqrt)),
			acosf((-b + c + a) / (2.f * asqrt * csqrt)),
			acosf((b - c + a) / (2.f * bsqrt * asqrt)));
	}


	//! Flips the direction of surfaces. Changes backfacing triangles to frontfacing
	//! triangles and vice versa.
	//! \param mesh: Mesh on which the operation is performed.
	void CMeshManipulator::flipSurfaces(IMesh* mesh) const
	{
		if (!mesh)
			return;

		const UINT32 bcount = mesh->getMeshBufferCount();
		for (UINT32 b = 0; b<bcount; ++b)
		{
			IMeshBuffer* buffer = mesh->getMeshBuffer(b);
			const UINT32 idxcnt = buffer->getIndexCount();
			if (buffer->getIndexType() == EIT_16BIT)
			{
				UINT16* idx = buffer->getIndices();
				for (UINT32 i = 0; i<idxcnt; i += 3)
				{
					const UINT16 tmp = idx[i + 1];
					idx[i + 1] = idx[i + 2];
					idx[i + 2] = tmp;
				}
			}
			else
			{
				UINT32* idx = reinterpret_cast<UINT32*>(buffer->getIndices());
				for (UINT32 i = 0; i<idxcnt; i += 3)
				{
					const UINT32 tmp = idx[i + 1];
					idx[i + 1] = idx[i + 2];
					idx[i + 2] = tmp;
				}
			}
		}
	}


	namespace
	{
		template <typename T>
		void recalculateNormalsT(IMeshBuffer* buffer, bool smooth, bool angleWeighted)
		{
			const UINT32 vtxcnt = buffer->getVertexCount();
			const UINT32 idxcnt = buffer->getIndexCount();
			const T* idx = reinterpret_cast<T*>(buffer->getIndices());

			if (!smooth)
			{
				for (UINT32 i = 0; i<idxcnt; i += 3)
				{
					const Vector3& v1 = buffer->getPosition(idx[i + 0]);
					const Vector3& v2 = buffer->getPosition(idx[i + 1]);
					const Vector3& v3 = buffer->getPosition(idx[i + 2]);
					const Vector3 normal = plane3d<FLOAT32>(v1, v2, v3).Normal;
					buffer->getNormal(idx[i + 0]) = normal;
					buffer->getNormal(idx[i + 1]) = normal;
					buffer->getNormal(idx[i + 2]) = normal;
				}
			}
			else
			{
				UINT32 i;

				for (i = 0; i != vtxcnt; ++i)
					buffer->getNormal(i).set(0.f, 0.f, 0.f);

				for (i = 0; i<idxcnt; i += 3)
				{
					const Vector3& v1 = buffer->getPosition(idx[i + 0]);
					const Vector3& v2 = buffer->getPosition(idx[i + 1]);
					const Vector3& v3 = buffer->getPosition(idx[i + 2]);
					const Vector3 normal = plane3d<FLOAT32>(v1, v2, v3).Normal;

					Vector3 weight(1.f, 1.f, 1.f);
					if (angleWeighted)
						weight = irr::getAngleWeight(v1, v2, v3); // writing irr:: necessary for borland

					buffer->getNormal(idx[i + 0]) += weight.X*normal;
					buffer->getNormal(idx[i + 1]) += weight.Y*normal;
					buffer->getNormal(idx[i + 2]) += weight.Z*normal;
				}

				for (i = 0; i != vtxcnt; ++i)
					buffer->getNormal(i).normalise();
			}
		}
	}


	//! Recalculates all normals of the mesh buffer.
	/** \param buffer: Mesh buffer on which the operation is performed. */
	void CMeshManipulator::recalculateNormals(IMeshBuffer* buffer, bool smooth, bool angleWeighted) const
	{
		if (!buffer)
			return;

		if (buffer->getIndexType() == EIT_16BIT)
			recalculateNormalsT<UINT16>(buffer, smooth, angleWeighted);
		else
			recalculateNormalsT<UINT32>(buffer, smooth, angleWeighted);
	}


	//! Recalculates all normals of the mesh.
	//! \param mesh: Mesh on which the operation is performed.
	void CMeshManipulator::recalculateNormals(IMesh* mesh, bool smooth, bool angleWeighted) const
	{
		if (!mesh)
			return;

		const UINT32 bcount = mesh->getMeshBufferCount();
		for (UINT32 b = 0; b<bcount; ++b)
			recalculateNormals(mesh->getMeshBuffer(b), smooth, angleWeighted);
	}


	namespace
	{
		void calculateTangents(
			Vector3& normal,
			Vector3& tangent,
			Vector3& binormal,
			const Vector3& vt1, const Vector3& vt2, const Vector3& vt3, // vertices
			const Vector2& tc1, const Vector2& tc2, const Vector2& tc3) // texture coords
		{
			// choose one of them:
			//#define USE_NVIDIA_GLH_VERSION // use version used by nvidia in glh headers
#define USE_IRR_VERSION

#ifdef USE_IRR_VERSION

			Vector3 v1 = vt1 - vt2;
			Vector3 v2 = vt3 - vt1;
			normal = v2.crossProduct(v1);
			normal.normalise();

			// binormal

			FLOAT32 deltaX1 = tc1.x - tc2.x;
			FLOAT32 deltaX2 = tc3.x - tc1.x;
			binormal = (v1 * deltaX2) - (v2 * deltaX1);
			binormal.normalise();

			// tangent

			FLOAT32 deltaY1 = tc1.y - tc2.y;
			FLOAT32 deltaY2 = tc3.y - tc1.y;
			tangent = (v1 * deltaY2) - (v2 * deltaY1);
			tangent.normalise();

			// adjust

			Vector3 txb = tangent.crossProduct(binormal);
			if (txb.dotProduct(normal) < 0.0f)
			{
				tangent *= -1.0f;
				binormal *= -1.0f;
			}

#endif // USE_IRR_VERSION

#ifdef USE_NVIDIA_GLH_VERSION

			tangent.set(0, 0, 0);
			binormal.set(0, 0, 0);

			Vector3 v1(vt2.X - vt1.X, tc2.x - tc1.x, tc2.Y - tc1.Y);
			Vector3 v2(vt3.X - vt1.X, tc3.X - tc1.x, tc3.Y - tc1.Y);

			Vector3 txb = v1.crossProduct(v2);
			if (!iszero(txb.X))
			{
				tangent.X = -txb.Y / txb.X;
				binormal.X = -txb.Z / txb.X;
			}

			v1.X = vt2.Y - vt1.Y;
			v2.X = vt3.Y - vt1.Y;
			txb = v1.crossProduct(v2);

			if (!iszero(txb.X))
			{
				tangent.Y = -txb.Y / txb.X;
				binormal.Y = -txb.Z / txb.X;
			}

			v1.X = vt2.Z - vt1.Z;
			v2.X = vt3.Z - vt1.Z;
			txb = v1.crossProduct(v2);

			if (!iszero(txb.X))
			{
				tangent.Z = -txb.Y / txb.X;
				binormal.Z = -txb.Z / txb.X;
			}

			tangent.normalise();
			binormal.normalise();

			normal = tangent.crossProduct(binormal);
			normal.normalise();

			binormal = tangent.crossProduct(normal);
			binormal.normalise();

			plane3d<FLOAT32> pl(vt1, vt2, vt3);

			if (normal.dotProduct(pl.Normal) < 0.0f)
				normal *= -1.0f;

#endif // USE_NVIDIA_GLH_VERSION
		}


		//! Recalculates tangents for a tangent mesh buffer
		template <typename T>
		void recalculateTangentsT(IMeshBuffer* buffer, bool recalculateNormals, bool smooth, bool angleWeighted)
		{
			if (!buffer || (buffer->getVertexType() != EVT_TANGENTS))
				return;

			const UINT32 vtxCnt = buffer->getVertexCount();
			const UINT32 idxCnt = buffer->getIndexCount();

			T* idx = reinterpret_cast<T*>(buffer->getIndices());
			S3DVertexTangents* v =
				(S3DVertexTangents*)buffer->getVertices();

			if (smooth)
			{
				UINT32 i;

				for (i = 0; i != vtxCnt; ++i)
				{
					if (recalculateNormals)
						v[i].Normal.set(0.f, 0.f, 0.f);
					v[i].Tangent.set(0.f, 0.f, 0.f);
					v[i].Binormal.set(0.f, 0.f, 0.f);
				}

				//Each vertex gets the sum of the tangents and binormals from the faces around it
				for (i = 0; i<idxCnt; i += 3)
				{
					// if this triangle is degenerate, skip it!
					if (v[idx[i + 0]].Pos == v[idx[i + 1]].Pos ||
						v[idx[i + 0]].Pos == v[idx[i + 2]].Pos ||
						v[idx[i + 1]].Pos == v[idx[i + 2]].Pos
						/*||
						v[idx[i+0]].TCoords == v[idx[i+1]].TCoords ||
						v[idx[i+0]].TCoords == v[idx[i+2]].TCoords ||
						v[idx[i+1]].TCoords == v[idx[i+2]].TCoords */
						)
						continue;

					//Angle-weighted normals look better, but are slightly more CPU intensive to calculate
					Vector3 weight(1.f, 1.f, 1.f);
					if (angleWeighted)
						weight = irr::getAngleWeight(v[i + 0].Pos, v[i + 1].Pos, v[i + 2].Pos);	// writing irr:: necessary for borland
					Vector3 localNormal;
					Vector3 localTangent;
					Vector3 localBinormal;

					calculateTangents(
						localNormal,
						localTangent,
						localBinormal,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords);

					if (recalculateNormals)
						v[idx[i + 0]].Normal += localNormal * weight.X;
					v[idx[i + 0]].Tangent += localTangent * weight.X;
					v[idx[i + 0]].Binormal += localBinormal * weight.X;

					calculateTangents(
						localNormal,
						localTangent,
						localBinormal,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords);

					if (recalculateNormals)
						v[idx[i + 1]].Normal += localNormal * weight.Y;
					v[idx[i + 1]].Tangent += localTangent * weight.Y;
					v[idx[i + 1]].Binormal += localBinormal * weight.Y;

					calculateTangents(
						localNormal,
						localTangent,
						localBinormal,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords);

					if (recalculateNormals)
						v[idx[i + 2]].Normal += localNormal * weight.Z;
					v[idx[i + 2]].Tangent += localTangent * weight.Z;
					v[idx[i + 2]].Binormal += localBinormal * weight.Z;
				}

				// Normalize the tangents and binormals
				if (recalculateNormals)
				{
					for (i = 0; i != vtxCnt; ++i)
						v[i].Normal.normalise();
				}
				for (i = 0; i != vtxCnt; ++i)
				{
					v[i].Tangent.normalise();
					v[i].Binormal.normalise();
				}
			}
			else
			{
				Vector3 localNormal;
				for (UINT32 i = 0; i<idxCnt; i += 3)
				{
					calculateTangents(
						localNormal,
						v[idx[i + 0]].Tangent,
						v[idx[i + 0]].Binormal,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords);
					if (recalculateNormals)
						v[idx[i + 0]].Normal = localNormal;

					calculateTangents(
						localNormal,
						v[idx[i + 1]].Tangent,
						v[idx[i + 1]].Binormal,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords);
					if (recalculateNormals)
						v[idx[i + 1]].Normal = localNormal;

					calculateTangents(
						localNormal,
						v[idx[i + 2]].Tangent,
						v[idx[i + 2]].Binormal,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords);
					if (recalculateNormals)
						v[idx[i + 2]].Normal = localNormal;
				}
			}
		}
	}


	//! Recalculates tangents for a tangent mesh buffer
	void CMeshManipulator::recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals, bool smooth, bool angleWeighted) const
	{
		if (buffer && (buffer->getVertexType() == EVT_TANGENTS))
		{
			if (buffer->getIndexType() == EIT_16BIT)
				recalculateTangentsT<UINT16>(buffer, recalculateNormals, smooth, angleWeighted);
			else
				recalculateTangentsT<UINT32>(buffer, recalculateNormals, smooth, angleWeighted);
		}
	}


	//! Recalculates tangents for all tangent mesh buffers
	void CMeshManipulator::recalculateTangents(IMesh* mesh, bool recalculateNormals, bool smooth, bool angleWeighted) const
	{
		if (!mesh)
			return;

		const UINT32 meshBufferCount = mesh->getMeshBufferCount();
		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			recalculateTangents(mesh->getMeshBuffer(b), recalculateNormals, smooth, angleWeighted);
		}
	}


	namespace
	{
		//! Creates a planar texture mapping on the meshbuffer
		template<typename T>
		void makePlanarTextureMappingT(IMeshBuffer* buffer, FLOAT32 resolution)
		{
			UINT32 idxcnt = buffer->getIndexCount();
			T* idx = reinterpret_cast<T*>(buffer->getIndices());

			for (UINT32 i = 0; i<idxcnt; i += 3)
			{
				plane3df p(buffer->getPosition(idx[i + 0]), buffer->getPosition(idx[i + 1]), buffer->getPosition(idx[i + 2]));
				p.Normal.X = fabsf(p.Normal.X);
				p.Normal.Y = fabsf(p.Normal.Y);
				p.Normal.Z = fabsf(p.Normal.Z);
				// calculate planar mapping worldspace coordinates

				if (p.Normal.X > p.Normal.Y && p.Normal.X > p.Normal.Z)
				{
					for (UINT32 o = 0; o != 3; ++o)
					{
						buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).Y * resolution;
						buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Z * resolution;
					}
				}
				else
					if (p.Normal.Y > p.Normal.X && p.Normal.Y > p.Normal.Z)
					{
						for (UINT32 o = 0; o != 3; ++o)
						{
							buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).X * resolution;
							buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Z * resolution;
						}
					}
					else
					{
						for (UINT32 o = 0; o != 3; ++o)
						{
							buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).X * resolution;
							buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Y * resolution;
						}
					}
			}
		}
	}


	//! Creates a planar texture mapping on the meshbuffer
	void CMeshManipulator::makePlanarTextureMapping(IMeshBuffer* buffer, FLOAT32 resolution) const
	{
		if (!buffer)
			return;

		if (buffer->getIndexType() == EIT_16BIT)
			makePlanarTextureMappingT<UINT16>(buffer, resolution);
		else
			makePlanarTextureMappingT<UINT32>(buffer, resolution);
	}


	//! Creates a planar texture mapping on the mesh
	void CMeshManipulator::makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolution) const
	{
		if (!mesh)
			return;

		const UINT32 bcount = mesh->getMeshBufferCount();
		for (UINT32 b = 0; b<bcount; ++b)
		{
			makePlanarTextureMapping(mesh->getMeshBuffer(b), resolution);
		}
	}


	namespace
	{
		//! Creates a planar texture mapping on the meshbuffer
		template <typename T>
		void makePlanarTextureMappingT(IMeshBuffer* buffer, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT8 axis, const Vector3& offset)
		{
			UINT32 idxcnt = buffer->getIndexCount();
			T* idx = reinterpret_cast<T*>(buffer->getIndices());

			for (UINT32 i = 0; i<idxcnt; i += 3)
			{
				// calculate planar mapping worldspace coordinates
				if (axis == 0)
				{
					for (UINT32 o = 0; o != 3; ++o)
					{
						buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).Z + offset.Z) * resolutionS;
						buffer->getTCoords(idx[i + o]).Y = 0.5f - (buffer->getPosition(idx[i + o]).Y + offset.Y) * resolutionT;
					}
				}
				else if (axis == 1)
				{
					for (UINT32 o = 0; o != 3; ++o)
					{
						buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).X + offset.X) * resolutionS;
						buffer->getTCoords(idx[i + o]).Y = 1.f - (buffer->getPosition(idx[i + o]).Z + offset.Z) * resolutionT;
					}
				}
				else if (axis == 2)
				{
					for (UINT32 o = 0; o != 3; ++o)
					{
						buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).X + offset.X) * resolutionS;
						buffer->getTCoords(idx[i + o]).Y = 0.5f - (buffer->getPosition(idx[i + o]).Y + offset.Y) * resolutionT;
					}
				}
			}
		}
	}


	//! Creates a planar texture mapping on the meshbuffer
	void CMeshManipulator::makePlanarTextureMapping(IMeshBuffer* buffer, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const
	{
		if (!buffer)
			return;

		if (buffer->getIndexType() == EIT_16BIT)
			makePlanarTextureMappingT<UINT16>(buffer, resolutionS, resolutionT, axis, offset);
		else
			makePlanarTextureMappingT<UINT32>(buffer, resolutionS, resolutionT, axis, offset);
	}


	//! Creates a planar texture mapping on the mesh
	void CMeshManipulator::makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const
	{
		if (!mesh)
			return;

		const UINT32 bcount = mesh->getMeshBufferCount();
		for (UINT32 b = 0; b<bcount; ++b)
		{
			makePlanarTextureMapping(mesh->getMeshBuffer(b), resolutionS, resolutionT, axis, offset);
		}
	}


	//! Clones a static IMesh into a modifyable SMesh.
	// not yet 32bit
	SMesh* CMeshManipulator::createMeshCopy(IMesh* mesh) const
	{
		if (!mesh)
			return 0;

		SMesh* clone = new SMesh();

		const UINT32 meshBufferCount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
			switch (mb->getVertexType())
			{
			case EVT_STANDARD:
			{
				SMeshBuffer* buffer = new SMeshBuffer();
				buffer->Material = mb->getMaterial();
				const UINT32 vcount = mb->getVertexCount();
				//buffer->Vertices.reallocate(vcount);
				S3DVertex* vertices = (S3DVertex*)mb->getVertices();
				for (UINT32 i = 0; i < vcount; ++i)
					buffer->Vertices.push_back(vertices[i]);
				const UINT32 icount = mb->getIndexCount();
				//buffer->Indices.reallocate(icount);
				const UINT16* indices = mb->getIndices();
				for (UINT32 i = 0; i < icount; ++i)
					buffer->Indices.push_back(indices[i]);
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			case EVT_2TCOORDS:
			{
				SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
				buffer->Material = mb->getMaterial();
				const UINT32 vcount = mb->getVertexCount();
				//buffer->Vertices.reallocate(vcount);
				S3DVertex2TCoords* vertices = (S3DVertex2TCoords*)mb->getVertices();
				for (UINT32 i = 0; i < vcount; ++i)
					buffer->Vertices.push_back(vertices[i]);
				const UINT32 icount = mb->getIndexCount();
				//buffer->Indices.reallocate(icount);
				const UINT16* indices = mb->getIndices();
				for (UINT32 i = 0; i < icount; ++i)
					buffer->Indices.push_back(indices[i]);
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			case EVT_TANGENTS:
			{
				SMeshBufferTangents* buffer = new SMeshBufferTangents();
				buffer->Material = mb->getMaterial();
				const UINT32 vcount = mb->getVertexCount();
				//buffer->Vertices.reallocate(vcount);
				S3DVertexTangents* vertices = (S3DVertexTangents*)mb->getVertices();
				for (UINT32 i = 0; i < vcount; ++i)
					buffer->Vertices.push_back(vertices[i]);
				const UINT32 icount = mb->getIndexCount();
				//buffer->Indices.reallocate(icount);
				const UINT16* indices = mb->getIndices();
				for (UINT32 i = 0; i < icount; ++i)
					buffer->Indices.push_back(indices[i]);
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			}// end switch

		}// end for all mesh buffers

		clone->BoundingBox = mesh->getBoundingBox();
		return clone;
	}


	//! Creates a copy of the mesh, which will only consist of unique primitives
	// not yet 32bit
	IMesh* CMeshManipulator::createMeshUniquePrimitives(IMesh* mesh) const
	{
		if (!mesh)
			return 0;

		SMesh* clone = new SMesh();

		const UINT32 meshBufferCount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
			const SINT32 idxCnt = mb->getIndexCount();
			const UINT16* idx = mb->getIndices();

			switch (mb->getVertexType())
			{
			case EVT_STANDARD:
			{
				SMeshBuffer* buffer = new SMeshBuffer();
				buffer->Material = mb->getMaterial();

				S3DVertex* v =
					(S3DVertex*)mb->getVertices();

				//buffer->Vertices.reallocate(idxCnt);
			//	buffer->Indices.reallocate(idxCnt);
				for (SINT32 i = 0; i<idxCnt; i += 3)
				{
					buffer->Vertices.push_back(v[idx[i + 0]]);
					buffer->Vertices.push_back(v[idx[i + 1]]);
					buffer->Vertices.push_back(v[idx[i + 2]]);

					buffer->Indices.push_back(i + 0);
					buffer->Indices.push_back(i + 1);
					buffer->Indices.push_back(i + 2);
				}

				buffer->setBoundingBox(mb->getBoundingBox());
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			case EVT_2TCOORDS:
			{
				SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
				buffer->Material = mb->getMaterial();

				S3DVertex2TCoords* v =
					(S3DVertex2TCoords*)mb->getVertices();

				//buffer->Vertices.reallocate(idxCnt);
				//buffer->Indices.reallocate(idxCnt);
				for (SINT32 i = 0; i<idxCnt; i += 3)
				{
					buffer->Vertices.push_back(v[idx[i + 0]]);
					buffer->Vertices.push_back(v[idx[i + 1]]);
					buffer->Vertices.push_back(v[idx[i + 2]]);

					buffer->Indices.push_back(i + 0);
					buffer->Indices.push_back(i + 1);
					buffer->Indices.push_back(i + 2);
				}
				buffer->setBoundingBox(mb->getBoundingBox());
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			case EVT_TANGENTS:
			{
				SMeshBufferTangents* buffer = new SMeshBufferTangents();
				buffer->Material = mb->getMaterial();

				S3DVertexTangents* v =
					(S3DVertexTangents*)mb->getVertices();

				//buffer->Vertices.reallocate(idxCnt);
				//buffer->Indices.reallocate(idxCnt);
				for (SINT32 i = 0; i<idxCnt; i += 3)
				{
					buffer->Vertices.push_back(v[idx[i + 0]]);
					buffer->Vertices.push_back(v[idx[i + 1]]);
					buffer->Vertices.push_back(v[idx[i + 2]]);

					buffer->Indices.push_back(i + 0);
					buffer->Indices.push_back(i + 1);
					buffer->Indices.push_back(i + 2);
				}

				buffer->setBoundingBox(mb->getBoundingBox());
				clone->addMeshBuffer(buffer);
				buffer->drop();
			}
			break;
			}// end switch

		}// end for all mesh buffers

		clone->BoundingBox = mesh->getBoundingBox();
		return clone;
	}


	//! Creates a copy of a mesh, which will have identical vertices welded together
	// not yet 32bit
	IMesh* CMeshManipulator::createMeshWelded(IMesh *mesh, FLOAT32 tolerance) const
	{
		SMesh* clone = new SMesh();
		clone->BoundingBox = mesh->getBoundingBox();

		vector<UINT16>::type redirects;

		for (UINT32 b = 0; b<mesh->getMeshBufferCount(); ++b)
		{
			const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
			// reset redirect list
			//redirects.set_used(mb->getVertexCount());

			const UINT16* indices = 0;
			UINT32 indexCount = 0;
			vector<UINT16>::type* outIdx = 0;

			switch (mb->getVertexType())
			{
			case EVT_STANDARD:
			{
				SMeshBuffer* buffer = new SMeshBuffer();
				buffer->BoundingBox = mb->getBoundingBox();
				buffer->Material = mb->getMaterial();
				clone->addMeshBuffer(buffer);
				buffer->drop();

				S3DVertex* v =
					(S3DVertex*)mb->getVertices();

				UINT32 vertexCount = mb->getVertexCount();

				indices = mb->getIndices();
				indexCount = mb->getIndexCount();
				outIdx = &buffer->Indices;

				//buffer->Vertices.reallocate(vertexCount);

				for (UINT32 i = 0; i < vertexCount; ++i)
				{
					bool found = false;
					for (UINT32 j = 0; j < i; ++j)
					{
						//if (v[i].Pos.equals(v[j].Pos, tolerance) &&
						//	v[i].Normal.equals(v[j].Normal, tolerance) &&
						//	v[i].TCoords.equals(v[j].TCoords) &&
						if (v[i].Pos == v[j].Pos &&
							v[i].Normal == v[j].Normal &&
							v[i].TCoords == v[j].TCoords &&
							(v[i].Color == v[j].Color))
						{
							redirects[i] = redirects[j];
							found = true;
							break;
						}
					}
					if (!found)
					{
						redirects[i] = buffer->Vertices.size();
						buffer->Vertices.push_back(v[i]);
					}
				}

				break;
			}
			case EVT_2TCOORDS:
			{
				SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
				buffer->BoundingBox = mb->getBoundingBox();
				buffer->Material = mb->getMaterial();
				clone->addMeshBuffer(buffer);
				buffer->drop();

				S3DVertex2TCoords* v =
					(S3DVertex2TCoords*)mb->getVertices();

				UINT32 vertexCount = mb->getVertexCount();

				indices = mb->getIndices();
				indexCount = mb->getIndexCount();
				outIdx = &buffer->Indices;

				//buffer->Vertices.reallocate(vertexCount);

				for (UINT32 i = 0; i < vertexCount; ++i)
				{
					bool found = false;
					for (UINT32 j = 0; j < i; ++j)
					{
						//if (v[i].Pos.equals(v[j].Pos, tolerance) &&
						//	v[i].Normal.equals(v[j].Normal, tolerance) &&
						//	v[i].TCoords.equals(v[j].TCoords) &&
						//	v[i].TCoords2.equals(v[j].TCoords2) &&
						if (v[i].Pos == v[j].Pos &&
							v[i].Normal == v[j].Normal  &&
							v[i].TCoords == v[j].TCoords &&
							v[i].TCoords2 == v[j].TCoords2 &&
							(v[i].Color == v[j].Color))
						{
							redirects[i] = redirects[j];
							found = true;
							break;
						}
					}
					if (!found)
					{
						redirects[i] = buffer->Vertices.size();
						buffer->Vertices.push_back(v[i]);
					}
				}
				break;
			}
			case EVT_TANGENTS:
			{
				SMeshBufferTangents* buffer = new SMeshBufferTangents();
				buffer->BoundingBox = mb->getBoundingBox();
				buffer->Material = mb->getMaterial();
				clone->addMeshBuffer(buffer);
				buffer->drop();

				S3DVertexTangents* v =
					(S3DVertexTangents*)mb->getVertices();

				UINT32 vertexCount = mb->getVertexCount();

				indices = mb->getIndices();
				indexCount = mb->getIndexCount();
				outIdx = &buffer->Indices;

				//buffer->Vertices.reallocate(vertexCount);

				for (UINT32 i = 0; i < vertexCount; ++i)
				{
					bool found = false;
					for (UINT32 j = 0; j < i; ++j)
					{
						//if (v[i].Pos.equals(v[j].Pos, tolerance) &&
						//	v[i].Normal.equals(v[j].Normal, tolerance) &&
						//	v[i].TCoords.equals(v[j].TCoords) &&
						//	v[i].Tangent.equals(v[j].Tangent, tolerance) &&
						//	v[i].Binormal.equals(v[j].Binormal, tolerance) &&
						if (v[i].Pos == v[j].Pos  &&
							v[i].Normal == v[j].Normal  &&
							v[i].TCoords == v[j].TCoords &&
							v[i].Tangent == v[j].Tangent  &&
							v[i].Binormal == v[j].Binormal  &&
							(v[i].Color == v[j].Color))
						{
							redirects[i] = redirects[j];
							found = true;
							break;
						}
					}
					if (!found)
					{
						redirects[i] = buffer->Vertices.size();
						buffer->Vertices.push_back(v[i]);
					}
				}
				break;
			}
			default:
				Printer::log("Cannot create welded mesh, vertex type unsupported", LML_ERROR);
				break;
			}

			// write the buffer's index list
			vector<UINT16>::type &Indices = *outIdx;

			//Indices.set_used(indexCount);
			for (UINT32 i = 0; i<indexCount; ++i)
			{
				Indices[i] = redirects[indices[i]];
			}
		}
		return clone;
	}


	//! Creates a copy of the mesh, which will only consist of S3DVertexTangents vertices.
	// not yet 32bit
	IMesh* CMeshManipulator::createMeshWithTangents(IMesh* mesh, bool recalculateNormals, bool smooth, bool angleWeighted, bool calculateTangents) const
	{
		if (!mesh)
			return 0;

		// copy mesh and fill data into SMeshBufferTangents

		SMesh* clone = new SMesh();
		const UINT32 meshBufferCount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			const IMeshBuffer* const original = mesh->getMeshBuffer(b);
			const UINT32 idxCnt = original->getIndexCount();
			const UINT16* idx = original->getIndices();

			SMeshBufferTangents* buffer = new SMeshBufferTangents();

			buffer->Material = original->getMaterial();
			//buffer->Vertices.reallocate(idxCnt);
		//	buffer->Indices.reallocate(idxCnt);

			map<S3DVertexTangents, int>::type vertMap;
			int vertLocation;

			// copy vertices

			const E_VERTEX_TYPE vType = original->getVertexType();
			S3DVertexTangents vNew;
			for (UINT32 i = 0; i<idxCnt; ++i)
			{
				switch (vType)
				{
				case EVT_STANDARD:
				{
					const S3DVertex* v =
						(const S3DVertex*)original->getVertices();
					vNew = S3DVertexTangents(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
				}
				break;
				case EVT_2TCOORDS:
				{
					const S3DVertex2TCoords* v =
						(const S3DVertex2TCoords*)original->getVertices();
					vNew = S3DVertexTangents(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
				}
				break;
				case EVT_TANGENTS:
				{
					const S3DVertexTangents* v =
						(const S3DVertexTangents*)original->getVertices();
					vNew = v[idx[i]];
				}
				break;
				}
				map<S3DVertexTangents, int>::iterator n = std::find(vertMap.begin(), vertMap.end(), vNew);//vertMap.find(vNew);
				if (n != vertMap.end())
				{
					//vertLocation = n->getValue();
					vertLocation = n->second;
				}
				else
				{
					vertLocation = buffer->Vertices.size();
					buffer->Vertices.push_back(vNew);
					//vertMap.insert(vNew, vertLocation);
					vertMap.insert(pair<S3DVertexTangents, int>(vNew, vertLocation));
				}

				// create new indices
				buffer->Indices.push_back(vertLocation);
			}
			buffer->recalculateBoundingBox();

			// add new buffer
			clone->addMeshBuffer(buffer);
			buffer->drop();
		}

		clone->recalculateBoundingBox();
		if (calculateTangents)
			recalculateTangents(clone, recalculateNormals, smooth, angleWeighted);

		return clone;
	}


	//! Creates a copy of the mesh, which will only consist of S3DVertex2TCoords vertices.
	// not yet 32bit
	IMesh* CMeshManipulator::createMeshWith2TCoords(IMesh* mesh) const
	{
		if (!mesh)
			return 0;

		// copy mesh and fill data into SMeshBufferLightMap

		SMesh* clone = new SMesh();
		const UINT32 meshBufferCount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			const IMeshBuffer* const original = mesh->getMeshBuffer(b);
			const UINT32 idxCnt = original->getIndexCount();
			const UINT16* idx = original->getIndices();

			SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
			buffer->Material = original->getMaterial();
			//buffer->Vertices.reallocate(idxCnt);
			//buffer->Indices.reallocate(idxCnt);

			map<S3DVertex2TCoords, int>::type vertMap;
			int vertLocation;

			// copy vertices

			const E_VERTEX_TYPE vType = original->getVertexType();
			S3DVertex2TCoords vNew;
			for (UINT32 i = 0; i<idxCnt; ++i)
			{
				switch (vType)
				{
				case EVT_STANDARD:
				{
					const S3DVertex* v =
						(const S3DVertex*)original->getVertices();
					vNew = S3DVertex2TCoords(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords, v[idx[i]].TCoords);
				}
				break;
				case EVT_2TCOORDS:
				{
					const S3DVertex2TCoords* v =
						(const S3DVertex2TCoords*)original->getVertices();
					vNew = v[idx[i]];
				}
				break;
				case EVT_TANGENTS:
				{
					const S3DVertexTangents* v =
						(const S3DVertexTangents*)original->getVertices();
					vNew = S3DVertex2TCoords(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords, v[idx[i]].TCoords);
				}
				break;
				}
				map<S3DVertex2TCoords, int>::iterator n = std::find(vertMap.begin(), vertMap.end(), vNew); //vertMap.find(vNew);
				if (n != vertMap.end())
				{
					vertLocation = n->second; //n->getValue();
				}
				else
				{
					vertLocation = buffer->Vertices.size();
					buffer->Vertices.push_back(vNew);
					//vertMap.insert(vNew, vertLocation);
					vertMap.insert(pair<S3DVertex2TCoords, int>(vNew, vertLocation));
				}

				// create new indices
				buffer->Indices.push_back(vertLocation);
			}
			buffer->recalculateBoundingBox();

			// add new buffer
			clone->addMeshBuffer(buffer);
			buffer->drop();
		}

		clone->recalculateBoundingBox();
		return clone;
	}


	//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
	// not yet 32bit
	IMesh* CMeshManipulator::createMeshWith1TCoords(IMesh* mesh) const
	{
		if (!mesh)
			return 0;

		// copy mesh and fill data into SMeshBuffer
		SMesh* clone = new SMesh();
		const UINT32 meshBufferCount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b<meshBufferCount; ++b)
		{
			IMeshBuffer* original = mesh->getMeshBuffer(b);
			const UINT32 idxCnt = original->getIndexCount();
			const UINT16* idx = original->getIndices();

			SMeshBuffer* buffer = new SMeshBuffer();
			buffer->Material = original->getMaterial();
			//buffer->Vertices.reallocate(idxCnt);
			//buffer->Indices.reallocate(idxCnt);

			map<S3DVertex, int>::type vertMap;
			int vertLocation;

			// copy vertices
			const E_VERTEX_TYPE vType = original->getVertexType();
			S3DVertex vNew;
			for (UINT32 i = 0; i<idxCnt; ++i)
			{
				switch (vType)
				{
				case EVT_STANDARD:
				{
					S3DVertex* v =
						(S3DVertex*)original->getVertices();
					vNew = v[idx[i]];
				}
				break;
				case EVT_2TCOORDS:
				{
					S3DVertex2TCoords* v =
						(S3DVertex2TCoords*)original->getVertices();
					vNew = S3DVertex(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
				}
				break;
				case EVT_TANGENTS:
				{
					S3DVertexTangents* v =
						(S3DVertexTangents*)original->getVertices();
					vNew = S3DVertex(
						v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
				}
				break;
				}
				//map<S3DVertex, int>::Node* n = vertMap.find(vNew);
				map<S3DVertex, int>::iterator n = std::find(vertMap.begin(), vertMap.end(), vNew);
				if (n != vertMap.end())
				{
					//vertLocation = n->getValue();
					vertLocation = n->second;
				}
				else
				{
					vertLocation = buffer->Vertices.size();
					buffer->Vertices.push_back(vNew);
					//vertMap.insert(vNew, vertLocation);
					vertMap.insert(pair<S3DVertex, int>(vNew, vertLocation));
				}

				// create new indices
				buffer->Indices.push_back(vertLocation);
			}
			buffer->recalculateBoundingBox();
			// add new buffer
			clone->addMeshBuffer(buffer);
			buffer->drop();
		}

		clone->recalculateBoundingBox();
		return clone;
	}


	//! Returns amount of polygons in mesh.
	SINT32 CMeshManipulator::getPolyCount(IMesh* mesh) const
	{
		if (!mesh)
			return 0;

		SINT32 trianglecount = 0;

		for (UINT32 g = 0; g<mesh->getMeshBufferCount(); ++g)
			trianglecount += mesh->getMeshBuffer(g)->getIndexCount() / 3;

		return trianglecount;
	}


	//! Returns amount of polygons in mesh.
	SINT32 CMeshManipulator::getPolyCount(IAnimatedMesh* mesh) const
	{
		if (mesh && mesh->getFrameCount() != 0)
			return getPolyCount(mesh->getMesh(0));

		return 0;
	}


	//! create a new AnimatedMesh and adds the mesh to it
	IAnimatedMesh * CMeshManipulator::createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const
	{
		return new SAnimatedMesh(mesh, type);
	}

	namespace
	{

		struct vcache
		{
			vector<UINT32>::type tris;
			float score;
			SINT16 cachepos;
			UINT16 NumActiveTris;
		};

		struct tcache
		{
			UINT16 ind[3];
			float score;
			bool drawn;
		};

		const UINT16 cachesize = 32;

		float FindVertexScore(vcache *v)
		{
			const float CacheDecayPower = 1.5f;
			const float LastTriScore = 0.75f;
			const float ValenceBoostScale = 2.0f;
			const float ValenceBoostPower = 0.5f;
			const float MaxSizeVertexCache = 32.0f;

			if (v->NumActiveTris == 0)
			{
				// No tri needs this vertex!
				return -1.0f;
			}

			float Score = 0.0f;
			int CachePosition = v->cachepos;
			if (CachePosition < 0)
			{
				// Vertex is not in FIFO cache - no score.
			}
			else
			{
				if (CachePosition < 3)
				{
					// This vertex was used in the last triangle,
					// so it has a fixed score.
					Score = LastTriScore;
				}
				else
				{
					// Points for being high in the cache.
					const float Scaler = 1.0f / (MaxSizeVertexCache - 3);
					Score = 1.0f - (CachePosition - 3) * Scaler;
					Score = powf(Score, CacheDecayPower);
				}
			}

			// Bonus points for having a low number of tris still to
			// use the vert, so we get rid of lone verts quickly.
			float ValenceBoost = powf(v->NumActiveTris,
				-ValenceBoostPower);
			Score += ValenceBoostScale * ValenceBoost;

			return Score;
		}

		/*
		A specialized LRU cache for the Forsyth algorithm.
		*/

		class f_lru
		{

		public:
			f_lru(vcache *v, tcache *t) : vc(v), tc(t)
			{
				for (UINT16 i = 0; i < cachesize; i++)
				{
					cache[i] = -1;
				}
			}

			// Adds this vertex index and returns the highest-scoring triangle index
			UINT32 add(UINT16 vert, bool updatetris = false)
			{
				bool found = false;

				// Mark existing pos as empty
				for (UINT16 i = 0; i < cachesize; i++)
				{
					if (cache[i] == vert)
					{
						// Move everything down
						for (UINT16 j = i; j; j--)
						{
							cache[j] = cache[j - 1];
						}

						found = true;
						break;
					}
				}

				if (!found)
				{
					if (cache[cachesize - 1] != -1)
						vc[cache[cachesize - 1]].cachepos = -1;

					// Move everything down
					for (UINT16 i = cachesize - 1; i; i--)
					{
						cache[i] = cache[i - 1];
					}
				}

				cache[0] = vert;

				UINT32 highest = 0;
				float hiscore = 0;

				if (updatetris)
				{
					// Update cache positions
					for (UINT16 i = 0; i < cachesize; i++)
					{
						if (cache[i] == -1)
							break;

						vc[cache[i]].cachepos = i;
						vc[cache[i]].score = FindVertexScore(&vc[cache[i]]);
					}

					// Update triangle scores
					for (UINT16 i = 0; i < cachesize; i++)
					{
						if (cache[i] == -1)
							break;

						const UINT16 trisize = vc[cache[i]].tris.size();
						for (UINT16 t = 0; t < trisize; t++)
						{
							tcache *tri = &tc[vc[cache[i]].tris[t]];

							tri->score =
								vc[tri->ind[0]].score +
								vc[tri->ind[1]].score +
								vc[tri->ind[2]].score;

							if (tri->score > hiscore)
							{
								hiscore = tri->score;
								highest = vc[cache[i]].tris[t];
							}
						}
					}
				}

				return highest;
			}

		private:
			SINT32 cache[cachesize];
			vcache *vc;
			tcache *tc;
		};

	} // end anonymous namespace

	/**
	Vertex cache optimization according to the Forsyth paper:
	http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

	The function is thread-safe (read: you can optimize several meshes in different threads)

	\param mesh Source mesh for the operation.  */
	IMesh* CMeshManipulator::createForsythOptimizedMesh(const IMesh *mesh) const
	{
		if (!mesh)
			return 0;

		SMesh *newmesh = new SMesh();
		newmesh->BoundingBox = mesh->getBoundingBox();

		const UINT32 mbcount = mesh->getMeshBufferCount();

		for (UINT32 b = 0; b < mbcount; ++b)
		{
			const IMeshBuffer *mb = mesh->getMeshBuffer(b);

			if (mb->getIndexType() != EIT_16BIT)
			{
				Printer::log("Cannot optimize a mesh with 32bit indices", LML_ERROR);
				newmesh->drop();
				return 0;
			}

			const UINT32 icount = mb->getIndexCount();
			const UINT32 tcount = icount / 3;
			const UINT32 vcount = mb->getVertexCount();
			const UINT16 *ind = mb->getIndices();

			vcache *vc = new vcache[vcount];
			tcache *tc = new tcache[tcount];

			f_lru lru(vc, tc);

			// init
			for (UINT16 i = 0; i < vcount; i++)
			{
				vc[i].score = 0;
				vc[i].cachepos = -1;
				vc[i].NumActiveTris = 0;
			}

			// First pass: count how many times a vert is used
			for (UINT32 i = 0; i < icount; i += 3)
			{
				vc[ind[i]].NumActiveTris++;
				vc[ind[i + 1]].NumActiveTris++;
				vc[ind[i + 2]].NumActiveTris++;

				const UINT32 tri_ind = i / 3;
				tc[tri_ind].ind[0] = ind[i];
				tc[tri_ind].ind[1] = ind[i + 1];
				tc[tri_ind].ind[2] = ind[i + 2];
			}

			// Second pass: list of each triangle
			for (UINT32 i = 0; i < tcount; i++)
			{
				vc[tc[i].ind[0]].tris.push_back(i);
				vc[tc[i].ind[1]].tris.push_back(i);
				vc[tc[i].ind[2]].tris.push_back(i);

				tc[i].drawn = false;
			}

			// Give initial scores
			for (UINT16 i = 0; i < vcount; i++)
			{
				vc[i].score = FindVertexScore(&vc[i]);
			}
			for (UINT32 i = 0; i < tcount; i++)
			{
				tc[i].score =
					vc[tc[i].ind[0]].score +
					vc[tc[i].ind[1]].score +
					vc[tc[i].ind[2]].score;
			}

			switch (mb->getVertexType())
			{
			case EVT_STANDARD:
			{
				S3DVertex *v = (S3DVertex *) mb->getVertices();

				SMeshBuffer *buf = new SMeshBuffer();
				buf->Material = mb->getMaterial();

				//buf->Vertices.reallocate(vcount);
				//buf->Indices.reallocate(icount);

				map<const S3DVertex, const UINT16>::type sind; // search index for fast operation
				//typedef map<const S3DVertex, const UINT16>::Node snode;
				typedef map<const S3DVertex, const UINT16>::iterator snode;

				// Main algorithm
				UINT32 highest = 0;
				UINT32 drawcalls = 0;
				for (;;)
				{
					if (tc[highest].drawn)
					{
						bool found = false;
						float hiscore = 0;
						for (UINT32 t = 0; t < tcount; t++)
						{
							if (!tc[t].drawn)
							{
								if (tc[t].score > hiscore)
								{
									highest = t;
									hiscore = tc[t].score;
									found = true;
								}
							}
						}
						if (!found)
							break;
					}

					// Output the best triangle
					UINT16 newind = buf->Vertices.size();

					//snode *s = sind.find(v[tc[highest].ind[0]]);
					snode s = std::find(sind.begin(), sind.end(), v[tc[highest].ind[0]]);

					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[0]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[0]], newind);
						sind.insert(pair<const S3DVertex, const UINT16>(v[tc[highest].ind[0]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[1]]);

					//if (!s)
					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[1]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[1]], newind);
						sind.insert(pair<const S3DVertex, const UINT16>(v[tc[highest].ind[1]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[2]]);

					if (s != sind.end())//if (!s)
					{
						buf->Vertices.push_back(v[tc[highest].ind[2]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[2]], newind);
						sind.insert(pair<const S3DVertex, const UINT16>(v[tc[highest].ind[2]], newind));
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					vc[tc[highest].ind[0]].NumActiveTris--;
					vc[tc[highest].ind[1]].NumActiveTris--;
					vc[tc[highest].ind[2]].NumActiveTris--;

					tc[highest].drawn = true;

					for (UINT16 j = 0; j < 3; j++)
					{
						vcache *vert = &vc[tc[highest].ind[j]];
						for (UINT16 t = 0; t < vert->tris.size(); t++)
						{
							if (highest == vert->tris[t])
							{
								//vert->tris.erase(t);
								vert->tris.erase(vert->tris.begin() + t);
								break;
							}
						}
					}

					lru.add(tc[highest].ind[0]);
					lru.add(tc[highest].ind[1]);
					highest = lru.add(tc[highest].ind[2], true);
					drawcalls++;
				}

				buf->setBoundingBox(mb->getBoundingBox());
				newmesh->addMeshBuffer(buf);
				buf->drop();
			}
			break;
			case EVT_2TCOORDS:
			{
				S3DVertex2TCoords *v = (S3DVertex2TCoords *) mb->getVertices();

				SMeshBufferLightMap *buf = new SMeshBufferLightMap();
				buf->Material = mb->getMaterial();

				//buf->Vertices.reallocate(vcount);
				//buf->Indices.reallocate(icount);

				map<const S3DVertex2TCoords, const UINT16>::type sind; // search index for fast operation
				//typedef map<const S3DVertex2TCoords, const UINT16>::Node snode;
				typedef map<const S3DVertex2TCoords, const UINT16>::iterator snode;

				// Main algorithm
				UINT32 highest = 0;
				UINT32 drawcalls = 0;
				for (;;)
				{
					if (tc[highest].drawn)
					{
						bool found = false;
						float hiscore = 0;
						for (UINT32 t = 0; t < tcount; t++)
						{
							if (!tc[t].drawn)
							{
								if (tc[t].score > hiscore)
								{
									highest = t;
									hiscore = tc[t].score;
									found = true;
								}
							}
						}
						if (!found)
							break;
					}

					// Output the best triangle
					UINT16 newind = buf->Vertices.size();

					//snode *s = sind.find(v[tc[highest].ind[0]]);
					snode s = sind.find(v[tc[highest].ind[0]]);

					if(s != sind.end())//if (!s)
					{
						buf->Vertices.push_back(v[tc[highest].ind[0]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[0]], newind);
						sind.insert(pair<const S3DVertex2TCoords, const UINT16>(v[tc[highest].ind[0]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[1]]);

					//if (!s)
					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[1]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[1]], newind);
						sind.insert(pair<const S3DVertex2TCoords, const UINT16>(v[tc[highest].ind[1]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[2]]);

					//if (!s)
					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[2]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[2]], newind);
						sind.insert(pair<const S3DVertex2TCoords, const UINT16>(v[tc[highest].ind[2]], newind));
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					vc[tc[highest].ind[0]].NumActiveTris--;
					vc[tc[highest].ind[1]].NumActiveTris--;
					vc[tc[highest].ind[2]].NumActiveTris--;

					tc[highest].drawn = true;

					for (UINT16 j = 0; j < 3; j++)
					{
						vcache *vert = &vc[tc[highest].ind[j]];
						for (UINT16 t = 0; t < vert->tris.size(); t++)
						{
							if (highest == vert->tris[t])
							{
								vert->tris.erase(vert->tris.begin()+t);
								break;
							}
						}
					}

					lru.add(tc[highest].ind[0]);
					lru.add(tc[highest].ind[1]);
					highest = lru.add(tc[highest].ind[2]);
					drawcalls++;
				}

				buf->setBoundingBox(mb->getBoundingBox());
				newmesh->addMeshBuffer(buf);
				buf->drop();

			}
			break;
			case EVT_TANGENTS:
			{
				S3DVertexTangents *v = (S3DVertexTangents *) mb->getVertices();

				SMeshBufferTangents *buf = new SMeshBufferTangents();
				buf->Material = mb->getMaterial();

				//buf->Vertices.reallocate(vcount);
				//buf->Indices.reallocate(icount);

				map<const S3DVertexTangents, const UINT16>::type sind; // search index for fast operation
				//typedef map<const S3DVertexTangents, const UINT16>::Node snode;
				typedef map<const S3DVertexTangents, const UINT16>::iterator snode;

				// Main algorithm
				UINT32 highest = 0;
				UINT32 drawcalls = 0;
				for (;;)
				{
					if (tc[highest].drawn)
					{
						bool found = false;
						float hiscore = 0;
						for (UINT32 t = 0; t < tcount; t++)
						{
							if (!tc[t].drawn)
							{
								if (tc[t].score > hiscore)
								{
									highest = t;
									hiscore = tc[t].score;
									found = true;
								}
							}
						}
						if (!found)
							break;
					}

					// Output the best triangle
					UINT16 newind = buf->Vertices.size();

					//snode *s = sind.find(v[tc[highest].ind[0]]);
					snode s = sind.find(v[tc[highest].ind[0]]);

					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[0]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[0]], newind);
						sind.insert(pair<const S3DVertexTangents, const UINT16>(v[tc[highest].ind[0]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[1]]);

					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[1]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[1]], newind);
						sind.insert(pair<const S3DVertexTangents, const UINT16>(v[tc[highest].ind[1]], newind));
						newind++;
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					s = sind.find(v[tc[highest].ind[2]]);

					if (s != sind.end())
					{
						buf->Vertices.push_back(v[tc[highest].ind[2]]);
						buf->Indices.push_back(newind);
						//sind.insert(v[tc[highest].ind[2]], newind);
						sind.insert(pair<const S3DVertexTangents, const UINT16>(v[tc[highest].ind[2]], newind));
					}
					else
					{
						//buf->Indices.push_back(s->getValue());
						buf->Indices.push_back(s->second);
					}

					vc[tc[highest].ind[0]].NumActiveTris--;
					vc[tc[highest].ind[1]].NumActiveTris--;
					vc[tc[highest].ind[2]].NumActiveTris--;

					tc[highest].drawn = true;

					for (UINT16 j = 0; j < 3; j++)
					{
						vcache *vert = &vc[tc[highest].ind[j]];
						for (UINT16 t = 0; t < vert->tris.size(); t++)
						{
							if (highest == vert->tris[t])
							{
								//vert->tris.erase(t);
								vert->tris.erase(vert->tris.begin()+t);
								break;
							}
						}
					}

					lru.add(tc[highest].ind[0]);
					lru.add(tc[highest].ind[1]);
					highest = lru.add(tc[highest].ind[2]);
					drawcalls++;
				}

				buf->setBoundingBox(mb->getBoundingBox());
				newmesh->addMeshBuffer(buf);
				buf->drop();
			}
			break;
			}

			delete[] vc;
			delete[] tc;

		} // for each meshbuffer

		return newmesh;
	}
}