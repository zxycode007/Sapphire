#include "SapphireCShadowVolumeSceneNode.h"
#include "SapphireIScenesManager.h"
#include "SapphireIMesh.h"
#include "SapphireIVideoDriver.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireSViewFrustum.h"
#include "SapphireSLight.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CShadowVolumeSceneNode::CShadowVolumeSceneNode(const IMesh* shadowMesh, ISceneNode* parent,
		ISceneManager* mgr, SINT32 id, bool zfailmethod, FLOAT32 infinity)
		: IShadowVolumeSceneNode(parent, mgr, id),
		ShadowMesh(0), IndexCount(0), VertexCount(0), ShadowVolumesUsed(0),
		Infinity(infinity), UseZFailMethod(zfailmethod)
	{
#ifdef _DEBUG
		setDebugName("CShadowVolumeSceneNode");
#endif
		setShadowMesh(shadowMesh);
		setAutomaticCulling(EAC_OFF);
	}


	//! destructor
	CShadowVolumeSceneNode::~CShadowVolumeSceneNode()
	{
		if (ShadowMesh)
			ShadowMesh->drop();
	}


	void CShadowVolumeSceneNode::createShadowVolume(const Vector3& light, bool isDirectional)
	{
		SShadowVolume svp;
		AxisAlignedBox* bb = 0;

		// builds the shadow volume and adds it to the shadow volume list.

		if (ShadowVolumes.size() > ShadowVolumesUsed)
		{
			// get the next unused buffer

			svp = ShadowVolumes[ShadowVolumesUsed];
			//svp->set_used(0);
			svp.reserve(0);

			bb = &ShadowBBox[ShadowVolumesUsed];
		}
		else
		{
			ShadowVolumes.push_back(SShadowVolume());
			svp = ShadowVolumes.back();

			ShadowBBox.push_back(AxisAlignedBox());
			bb = &ShadowBBox.back();
		}
		//svp->reallocate(IndexCount * 5);
		svp.reserve(IndexCount * 5);
		++ShadowVolumesUsed;

		// We use triangle lists
		//Edges.set_used(IndexCount * 2);
		Edges.reserve(IndexCount * 2);
		UINT32 numEdges = 0;

		numEdges = createEdgesAndCaps(light, svp, bb);

		// for all edges add the near->far quads
		for (UINT32 i = 0; i<numEdges; ++i)
		{
			const Vector3 &v1 = Vertices[Edges[2 * i + 0]];
			const Vector3 &v2 = Vertices[Edges[2 * i + 1]];
			const Vector3 v3(v1 + (v1 - light).normalize()*Infinity);
			const Vector3 v4(v2 + (v2 - light).normalize()*Infinity);

			// Add a quad (two triangles) to the vertex list
#ifdef _DEBUG
			if (svp.size() >= svp.capacity() - 5)
				Printer::log("Allocation too small.", LML_DEBUG);
#endif
			svp.push_back(v1);
			svp.push_back(v2);
			svp.push_back(v3);

			svp.push_back(v2);
			svp.push_back(v4);
			svp.push_back(v3);
		}
	}


#define IRR_USE_ADJACENCY
#define IRR_USE_REVERSE_EXTRUDED

	UINT32 CShadowVolumeSceneNode::createEdgesAndCaps(const Vector3& light,
		SShadowVolume svp, AxisAlignedBox* bb)
	{
		UINT32 numEdges = 0;
		const UINT32 faceCount = IndexCount / 3;

		if (faceCount >= 1)
			bb->reset(Vertices[Indices[0]]);
		else
			bb->reset(Vector3(0, 0, 0));

		// Check every face if it is front or back facing the light.
		for (UINT32 i = 0; i<faceCount; ++i)
		{
			const Vector3 v0 = Vertices[Indices[3 * i + 0]];
			const Vector3 v1 = Vertices[Indices[3 * i + 1]];
			const Vector3 v2 = Vertices[Indices[3 * i + 2]];

#ifdef IRR_USE_REVERSE_EXTRUDED
			FaceData[i] = triangle3df(v0, v1, v2).isFrontFacing(light);
#else
			FaceData[i] = triangle3df(v2, v1, v0).isFrontFacing(light);
#endif

			if (UseZFailMethod && FaceData[i])
			{
#ifdef _DEBUG
				if (svp.size() >= svp.capacity() - 5)
					Printer::log("Allocation too small.", LML_DEBUG);
#endif
				// add front cap from light-facing faces
				svp.push_back(v2);
				svp.push_back(v1);
				svp.push_back(v0);

				// add back cap
				const Vector3 i0 = v0 + (v0 - light).normalize()*Infinity;
				const Vector3 i1 = v1 + (v1 - light).normalize()*Infinity;
				const Vector3 i2 = v2 + (v2 - light).normalize()*Infinity;

				svp.push_back(i0);
				svp.push_back(i1);
				svp.push_back(i2);

				bb->addInternalPoint(i0);
				bb->addInternalPoint(i1);
				bb->addInternalPoint(i2);
			}
		}

		// Create edges
		for (UINT32 i = 0; i<faceCount; ++i)
		{
			// check all front facing faces
			if (FaceData[i] == true)
			{
				const UINT16 wFace0 = Indices[3 * i + 0];
				const UINT16 wFace1 = Indices[3 * i + 1];
				const UINT16 wFace2 = Indices[3 * i + 2];

				const UINT16 adj0 = Adjacency[3 * i + 0];
				const UINT16 adj1 = Adjacency[3 * i + 1];
				const UINT16 adj2 = Adjacency[3 * i + 2];

				// add edges if face is adjacent to back-facing face
				// or if no adjacent face was found
#ifdef IRR_USE_ADJACENCY
				if (adj0 == i || FaceData[adj0] == false)
#endif
				{
					// add edge v0-v1
					Edges[2 * numEdges + 0] = wFace0;
					Edges[2 * numEdges + 1] = wFace1;
					++numEdges;
				}

#ifdef IRR_USE_ADJACENCY
				if (adj1 == i || FaceData[adj1] == false)
#endif
				{
					// add edge v1-v2
					Edges[2 * numEdges + 0] = wFace1;
					Edges[2 * numEdges + 1] = wFace2;
					++numEdges;
				}

#ifdef IRR_USE_ADJACENCY
				if (adj2 == i || FaceData[adj2] == false)
#endif
				{
					// add edge v2-v0
					Edges[2 * numEdges + 0] = wFace2;
					Edges[2 * numEdges + 1] = wFace0;
					++numEdges;
				}
			}
		}
		return numEdges;
	}


	void CShadowVolumeSceneNode::setShadowMesh(const IMesh* mesh)
	{
		if (ShadowMesh == mesh)
			return;
		if (ShadowMesh)
			ShadowMesh->drop();
		ShadowMesh = mesh;
		if (ShadowMesh)
		{
			ShadowMesh->grab();
			Box = ShadowMesh->getBoundingBox();
		}
	}


	void CShadowVolumeSceneNode::updateShadowVolumes()
	{
		const UINT32 oldIndexCount = IndexCount;
		const UINT32 oldVertexCount = VertexCount;

		const IMesh* const mesh = ShadowMesh;
		if (!mesh)
			return;

		// create as much shadow volumes as there are lights but
		// do not ignore the max light settings.
		const UINT32 lightCount = SceneManager->getVideoDriver()->getDynamicLightCount();
		if (!lightCount)
			return;

		// calculate total amount of vertices and indices

		VertexCount = 0;
		IndexCount = 0;
		ShadowVolumesUsed = 0;

		UINT32 i;
		UINT32 totalVertices = 0;
		UINT32 totalIndices = 0;
		const UINT32 bufcnt = mesh->getMeshBufferCount();

		for (i = 0; i<bufcnt; ++i)
		{
			const IMeshBuffer* buf = mesh->getMeshBuffer(i);
			totalIndices += buf->getIndexCount();
			totalVertices += buf->getVertexCount();
		}

		// allocate memory if necessary

		Vertices.reserve(totalVertices);
		Indices.reserve(totalIndices);
		FaceData.reserve(totalIndices / 3);

		// copy mesh
		for (i = 0; i<bufcnt; ++i)
		{
			const IMeshBuffer* buf = mesh->getMeshBuffer(i);

			const UINT16* idxp = buf->getIndices();
			const UINT16* idxpend = idxp + buf->getIndexCount();
			for (; idxp != idxpend; ++idxp)
				Indices[IndexCount++] = *idxp + VertexCount;

			const UINT32 vtxcnt = buf->getVertexCount();
			for (UINT32 j = 0; j<vtxcnt; ++j)
				Vertices[VertexCount++] = buf->getPosition(j);
		}

		// recalculate adjacency if necessary
		if (oldVertexCount != VertexCount || oldIndexCount != IndexCount)
			calculateAdjacency();

		Matrix4 mat = Parent->getAbsoluteTransformation();
		mat = mat.inverse();
		const Vector3 parentpos = Parent->getAbsolutePosition();

		// TODO: Only correct for point lights.
		for (i = 0; i<lightCount; ++i)
		{
			const SLight& dl = SceneManager->getVideoDriver()->getDynamicLight(i);
			Vector3 lpos = dl.Position;
			if (dl.CastShadows &&
				fabs((lpos - parentpos).squaredLength()) <= (dl.Radius*dl.Radius*4.0f))
			{
				lpos = mat.transformAffine(lpos);
				createShadowVolume(lpos);
			}
		}
	}


	//! pre render method
	void CShadowVolumeSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
		{
			SceneManager->registerNodeForRendering(this, ESNRP_SHADOW);
			ISceneNode::OnRegisterSceneNode();
		}
	}

	//! renders the node.
	void CShadowVolumeSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();

		if (!ShadowVolumesUsed || !driver)
			return;

		driver->setTransform(ETS_WORLD, Parent->getAbsoluteTransformation());

		for (UINT32 i = 0; i<ShadowVolumesUsed; ++i)
		{
			bool drawShadow = true;

			if (UseZFailMethod && SceneManager->getActiveCamera())
			{
				// Disable shadows drawing, when back cap is behind of ZFar plane.

				SViewFrustum frust = *SceneManager->getActiveCamera()->getViewFrustum();

				//Matrix4 invTrans(Parent->getAbsoluteTransformation(), Matrix4::EM4CONST_INVERSE);
				Matrix4 invTrans = Parent->getAbsoluteTransformation();
				invTrans = invTrans.inverse();
				frust.transform(invTrans);

				//Vector3 edges[8];
				const Vector3* edges = ShadowBBox[i].getAllCorners();
				//ShadowBBox[i].getEdges(edges);
				

				//Vector3 largestEdge = edges[0];
				Vector3 largestEdge = ShadowBBox[i].getMaximum();
				//FLOAT32 maxDistance = Vector3(SceneManager->getActiveCamera()->getPosition() - edges[0]).length();
				FLOAT32 maxDistance = Vector3(SceneManager->getActiveCamera()->getPosition() - ShadowBBox[i].getMaximum()).length();
				FLOAT32 curDistance = 0.f;

				for (int j = 1; j < 8; ++j)
				{
					curDistance = Vector3(SceneManager->getActiveCamera()->getPosition() - edges[j]).length();

					if (curDistance > maxDistance)
					{
						maxDistance = curDistance;
						largestEdge = edges[j];
					}
				}

				if (!(frust.planes[SViewFrustum::VF_FAR_PLANE].classifyPointRelation(largestEdge) != ISREL3D_FRONT))
					drawShadow = false;
			}

			if (drawShadow)
				driver->drawStencilShadowVolume(ShadowVolumes[i], UseZFailMethod, DebugDataVisible);
			else
			{
				vector<Vector3>::type triangles;
				driver->drawStencilShadowVolume(triangles, UseZFailMethod, DebugDataVisible);
			}
		}
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CShadowVolumeSceneNode::getBoundingBox() const
	{
		return Box;
	}


	//! Generates adjacency information based on mesh indices.
	void CShadowVolumeSceneNode::calculateAdjacency()
	{
		Adjacency.reserve(IndexCount);

		// go through all faces and fetch their three neighbours
		for (UINT32 f = 0; f<IndexCount; f += 3)
		{
			for (UINT32 edge = 0; edge<3; ++edge)
			{
				const Vector3& v1 = Vertices[Indices[f + edge]];
				const Vector3& v2 = Vertices[Indices[f + ((edge + 1) % 3)]];

				// now we search an_O_ther _F_ace with these two
				// vertices, which is not the current face.
				UINT32 of;

				for (of = 0; of<IndexCount; of += 3)
				{
					// only other faces
					if (of != f)
					{
						bool cnt1 = false;
						bool cnt2 = false;

						for (SINT32 e = 0; e<3; ++e)
						{
							if (v1.equals(Vertices[Indices[of + e]]))
								cnt1 = true;

							if (v2.equals(Vertices[Indices[of + e]]))
								cnt2 = true;
						}
						// one match for each vertex, i.e. edge is the same
						if (cnt1 && cnt2)
							break;
					}
				}

				// no adjacent edges -> store face number, else store adjacent face
				if (of >= IndexCount)
					Adjacency[f + edge] = f / 3;
				else
					Adjacency[f + edge] = of / 3;
			}
		}
	}
}