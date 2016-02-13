#include "SapphireCGeometryCreator.h"
#include "SapphireSAnimatedMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireSMesh.h"
#include "SapphireIMesh.h"
#include "SapphireIVideoDriver.h"
#include "SapphireOS.h"


namespace Sapphire
{

	IMesh* CGeometryCreator::createCubeMesh(const Vector3& size) const
	{
		SMeshBuffer* buffer = new SMeshBuffer();

		// Create indices
		const UINT16 u[36] = { 0, 2, 1, 0, 3, 2, 1, 5, 4, 1, 2, 5, 4, 6, 7, 4, 5, 6,
			7, 3, 0, 7, 6, 3, 9, 5, 2, 9, 8, 5, 0, 11, 10, 0, 10, 7 };

		//buffer->Indices.set_used(36);
		buffer->Indices.reserve(36);

		for (UINT32 i = 0; i<36; ++i)
			buffer->Indices[i] = u[i];


		// Create vertices
		ColourValue clr = ColourValue::getColourValue(255, 255, 255, 255);

		//buffer->Vertices.reallocate(12);

		buffer->Vertices.push_back(S3DVertex(0, 0, 0, -1, -1, -1, clr, 0, 1));
		buffer->Vertices.push_back(S3DVertex(1, 0, 0, 1, -1, -1, clr, 1, 1));
		buffer->Vertices.push_back(S3DVertex(1, 1, 0, 1, 1, -1, clr, 1, 0));
		buffer->Vertices.push_back(S3DVertex(0, 1, 0, -1, 1, -1, clr, 0, 0));
		buffer->Vertices.push_back(S3DVertex(1, 0, 1, 1, -1, 1, clr, 0, 1));
		buffer->Vertices.push_back(S3DVertex(1, 1, 1, 1, 1, 1, clr, 0, 0));
		buffer->Vertices.push_back(S3DVertex(0, 1, 1, -1, 1, 1, clr, 1, 0));
		buffer->Vertices.push_back(S3DVertex(0, 0, 1, -1, -1, 1, clr, 1, 1));
		buffer->Vertices.push_back(S3DVertex(0, 1, 1, -1, 1, 1, clr, 0, 1));
		buffer->Vertices.push_back(S3DVertex(0, 1, 0, -1, 1, -1, clr, 1, 1));
		buffer->Vertices.push_back(S3DVertex(1, 0, 1, 1, -1, 1, clr, 1, 0));
		buffer->Vertices.push_back(S3DVertex(1, 0, 0, 1, -1, -1, clr, 0, 0));

		// Recalculate bounding box
		buffer->BoundingBox.reset(Vector3(0, 0, 0));

		for (UINT32 i = 0; i<12; ++i)
		{
			buffer->Vertices[i].Pos -= Vector3(0.5f, 0.5f, 0.5f);
			buffer->Vertices[i].Pos *= size;
			buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
		}

		SMesh* mesh = new SMesh;
		mesh->addMeshBuffer(buffer);
		buffer->drop();

		mesh->recalculateBoundingBox();
		return mesh;
	}


	// creates a hill plane
	IMesh* CGeometryCreator::createHillPlaneMesh(
		const dimension2d<FLOAT32>& tileSize,
		const dimension2d<UINT32>& tc, SMaterial* material,
		FLOAT32 hillHeight, const dimension2d<FLOAT32>& ch,
		const dimension2d<FLOAT32>& textureRepeatCount) const
	{
		dimension2d<UINT32> tileCount = tc;
		dimension2d<FLOAT32> countHills = ch;

		if (countHills.Width < 0.01f)
			countHills.Width = 1.f;
		if (countHills.Height < 0.01f)
			countHills.Height = 1.f;

		// center
		const Position2d center((tileSize.Width * tileCount.Width) * 0.5f, (tileSize.Height * tileCount.Height) * 0.5f);

		// texture coord step
		const dimension2d<FLOAT32> tx(
			textureRepeatCount.Width / tileCount.Width,
			textureRepeatCount.Height / tileCount.Height);

		// add one more point in each direction for proper tile count
		++tileCount.Height;
		++tileCount.Width;

		SMeshBuffer* buffer = new SMeshBuffer();
		S3DVertex vtx;
		vtx.Color = ColourValue::getColourValue(255, 255, 255, 255);

		// create vertices from left-front to right-back
		UINT32 x;

		FLOAT32 sx = 0.f, tsx = 0.f;
		for (x = 0; x<tileCount.Width; ++x)
		{
			FLOAT32 sy = 0.f, tsy = 0.f;
			for (UINT32 y = 0; y<tileCount.Height; ++y)
			{
				vtx.Pos.set(sx - center.x, 0, sy - center.y);
				//vtx.TCoords.set(tsx, 1.0f - tsy);
				vtx.TCoords = Vector2(tsx, 1.0f - tsy);

				//if (isnotzero(hillHeight))
				if (!Math::iszero(hillHeight))
					vtx.Pos.y = sinf(vtx.Pos.x * countHills.Width * PI / center.x) *
					cosf(vtx.Pos.z * countHills.Height * PI / center.y) *
					hillHeight;

				buffer->Vertices.push_back(vtx);
				sy += tileSize.Height;
				tsy += tx.Height;
			}
			sx += tileSize.Width;
			tsx += tx.Width;
		}

		// create indices

		for (x = 0; x<tileCount.Width - 1; ++x)
		{
			for (UINT32 y = 0; y<tileCount.Height - 1; ++y)
			{
				const SINT32 current = x*tileCount.Height + y;

				buffer->Indices.push_back(current);
				buffer->Indices.push_back(current + 1);
				buffer->Indices.push_back(current + tileCount.Height);

				buffer->Indices.push_back(current + 1);
				buffer->Indices.push_back(current + 1 + tileCount.Height);
				buffer->Indices.push_back(current + tileCount.Height);
			}
		}

		// recalculate normals
		for (UINT32 i = 0; i<buffer->Indices.size(); i += 3)
		{
			const Vector3 normal = Plane(
				buffer->Vertices[buffer->Indices[i + 0]].Pos,
				buffer->Vertices[buffer->Indices[i + 1]].Pos,
				buffer->Vertices[buffer->Indices[i + 2]].Pos).normal;

			buffer->Vertices[buffer->Indices[i + 0]].Normal = normal;
			buffer->Vertices[buffer->Indices[i + 1]].Normal = normal;
			buffer->Vertices[buffer->Indices[i + 2]].Normal = normal;
		}

		if (material)
			buffer->Material = *material;

		buffer->recalculateBoundingBox();
		buffer->setHardwareMappingHint(EHM_STATIC);

		SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(buffer);
		mesh->recalculateBoundingBox();
		buffer->drop();
		return mesh;
	}


	IMesh* CGeometryCreator::createTerrainMesh(IImage* texture,
		IImage* heightmap, const dimension2d<FLOAT32>& stretchSize,
		FLOAT32 maxHeight, IVideoDriver* driver,
		const dimension2d<UINT32>& maxVtxBlockSize,
		bool debugBorders) const
	{
		if (!texture || !heightmap)
			return 0;

		// debug border
		const SINT32 borderSkip = debugBorders ? 0 : 1;

		S3DVertex vtx;
		//vtx.Color.set(255, 255, 255, 255);
		vtx.Color = ColourValue::getColourValue(255, 255, 255, 255);

		SMesh* mesh = new SMesh();

		const UINT32 tm = Timer::getRealTime() / 1000;
		const dimension2d<UINT32> hMapSize = heightmap->getDimension();
		const dimension2d<UINT32> tMapSize = texture->getDimension();
		const Position2d thRel(static_cast<FLOAT32>(tMapSize.Width) / hMapSize.Width, static_cast<FLOAT32>(tMapSize.Height) / hMapSize.Height);
		maxHeight /= 255.0f; // height step per color value

		Position2d processed(0, 0);
		while (processed.y<hMapSize.Height)
		{
			while (processed.x<hMapSize.Width)
			{
				dimension2d<UINT32> blockSize = maxVtxBlockSize;
				if (processed.x + blockSize.Width > hMapSize.Width)
					blockSize.Width = hMapSize.Width - processed.x;
				if (processed.y + blockSize.Height > hMapSize.Height)
					blockSize.Height = hMapSize.Height - processed.y;

				SMeshBuffer* buffer = new SMeshBuffer();
				buffer->setHardwareMappingHint(EHM_STATIC);
				//buffer->Vertices.reallocate(blockSize.getArea());
				// add vertices of vertex block
				UINT32 y;
				Vector2 pos(0.f, processed.y*stretchSize.Height);
				const Vector2 bs(1.f / blockSize.Width, 1.f / blockSize.Height);
				Vector2 tc(0.f, 0.5f*bs.y);
				for (y = 0; y<blockSize.Height; ++y)
				{
					pos.x = processed.x*stretchSize.Width;
					tc.x = 0.5f*bs.x;
					for (UINT32 x = 0; x<blockSize.Width; ++x)
					{
						const FLOAT32 height = heightmap->getPixel(x + processed.x, y + processed.y).getAverage() * maxHeight;

						vtx.Pos.set(pos.x, height, pos.y);
						//vtx.TCoords.set(tc);
						vtx.TCoords = tc;
						buffer->Vertices.push_back(vtx);
						pos.x += stretchSize.Width;
						tc.x += bs.x;
					}
					pos.y += stretchSize.Height;
					tc.y += bs.y;
				}

				//buffer->Indices.reallocate((blockSize.Height - 1)*(blockSize.Width - 1) * 6);
				buffer->Indices.reserve((blockSize.Height - 1)*(blockSize.Width - 1) * 6);

				// add indices of vertex block
				SINT32 c1 = 0;
				for (y = 0; y<blockSize.Height - 1; ++y)
				{
					for (UINT32 x = 0; x<blockSize.Width - 1; ++x)
					{
						const SINT32 c = c1 + x;

						buffer->Indices.push_back(c);
						buffer->Indices.push_back(c + blockSize.Width);
						buffer->Indices.push_back(c + 1);

						buffer->Indices.push_back(c + 1);
						buffer->Indices.push_back(c + blockSize.Width);
						buffer->Indices.push_back(c + 1 + blockSize.Width);
					}
					c1 += blockSize.Width;
				}

				// recalculate normals
				for (UINT32 i = 0; i<buffer->Indices.size(); i += 3)
				{
					const Vector3 normal = Plane(
						buffer->Vertices[buffer->Indices[i + 0]].Pos,
						buffer->Vertices[buffer->Indices[i + 1]].Pos,
						buffer->Vertices[buffer->Indices[i + 2]].Pos).normal;

					buffer->Vertices[buffer->Indices[i + 0]].Normal = normal;
					buffer->Vertices[buffer->Indices[i + 1]].Normal = normal;
					buffer->Vertices[buffer->Indices[i + 2]].Normal = normal;
				}

				if (buffer->Vertices.size())
				{
					c8 textureName[64];
					// create texture for this block
					IImage* img = driver->createImage(texture->getColorFormat(), dimension2d<UINT32>(floor32(blockSize.Width*thRel.x), floor32(blockSize.Height*thRel.y)));
					texture->copyTo(img, Position2d(0, 0), recti(
						Position2d(floor32(processed.x*thRel.x), floor32(processed.y*thRel.y)),
						dimension2d<UINT32>(floor32(blockSize.Width*thRel.x), floor32(blockSize.Height*thRel.y))), 0);

					sprintf(textureName, "terrain%u_%u", tm, mesh->getMeshBufferCount());

					buffer->Material.setTexture(0, driver->addTexture(textureName, img));

					if (buffer->Material.getTexture(0))
					{
						c8 tmp[255];
						sprintf(tmp, "Generated terrain texture (%dx%d): %s",
							buffer->Material.getTexture(0)->getSize().Width,
							buffer->Material.getTexture(0)->getSize().Height,
							textureName);
						Printer::log(tmp);
					}
					else
						Printer::log("Could not create terrain texture.", textureName, LML_ERROR);

					img->drop();
				}

				buffer->recalculateBoundingBox();
				mesh->addMeshBuffer(buffer);
				buffer->drop();

				// keep on processing
				processed.x += maxVtxBlockSize.Width - borderSkip;
			}

			// keep on processing
			processed.x = 0;
			processed.y += maxVtxBlockSize.Height - borderSkip;
		}

		mesh->recalculateBoundingBox();
		return mesh;
	}


	/*
	a cylinder, a cone and a cross
	point up on (0,1.f, 0.f )
	*/
	IMesh* CGeometryCreator::createArrowMesh(const UINT32 tesselationCylinder,
		const UINT32 tesselationCone,
		const FLOAT32 height,
		const FLOAT32 cylinderHeight,
		const FLOAT32 width0,
		const FLOAT32 width1,
		const ColourValue vtxColor0,
		const ColourValue vtxColor1) const
	{
		SMesh* mesh = (SMesh*)createCylinderMesh(width0, cylinderHeight, tesselationCylinder, vtxColor0, false);

		IMesh* mesh2 = createConeMesh(width1, height - cylinderHeight, tesselationCone, vtxColor1, vtxColor0);
		for (UINT32 i = 0; i<mesh2->getMeshBufferCount(); ++i)
		{
			IMeshBuffer* buffer = mesh2->getMeshBuffer(i);
			for (UINT32 j = 0; j<buffer->getVertexCount(); ++j)
				buffer->getPosition(j).y += cylinderHeight;
			buffer->setDirty(EBT_VERTEX);
			buffer->recalculateBoundingBox();
			mesh->addMeshBuffer(buffer);
		}
		mesh2->drop();
		mesh->setHardwareMappingHint(EHM_STATIC);

		mesh->recalculateBoundingBox();
		return mesh;
	}


	/* A sphere with proper normals and texture coords */
	IMesh* CGeometryCreator::createSphereMesh(FLOAT32 radius, UINT32 polyCountX, UINT32 polyCountY) const
	{
		// thanks to Alfaz93 who made his code available for Irrlicht on which
		// this one is based!

		// we are creating the sphere mesh here.

		if (polyCountX < 2)
			polyCountX = 2;
		if (polyCountY < 2)
			polyCountY = 2;
		while (polyCountX * polyCountY > 32767) // prevent UINT16 overflow
		{
			polyCountX /= 2;
			polyCountY /= 2;
		}

		const UINT32 polyCountXPitch = polyCountX + 1; // get to same vertex on next level

		SMeshBuffer* buffer = new SMeshBuffer();

		//buffer->Indices.reallocate((polyCountX * polyCountY) * 6);
		buffer->Indices.reserve((polyCountX * polyCountY) * 6);

		const ColourValue clr(255, 255, 255, 255);

		UINT32 level = 0;

		for (UINT32 p1 = 0; p1 < polyCountY - 1; ++p1)
		{
			//main quads, top to bottom
			for (UINT32 p2 = 0; p2 < polyCountX - 1; ++p2)
			{
				const UINT32 curr = level + p2;
				buffer->Indices.push_back(curr + polyCountXPitch);
				buffer->Indices.push_back(curr);
				buffer->Indices.push_back(curr + 1);
				buffer->Indices.push_back(curr + polyCountXPitch);
				buffer->Indices.push_back(curr + 1);
				buffer->Indices.push_back(curr + 1 + polyCountXPitch);
			}

			// the connectors from front to end
			buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
			buffer->Indices.push_back(level + polyCountX - 1);
			buffer->Indices.push_back(level + polyCountX);

			buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
			buffer->Indices.push_back(level + polyCountX);
			buffer->Indices.push_back(level + polyCountX + polyCountXPitch);
			level += polyCountXPitch;
		}

		const UINT32 polyCountSq = polyCountXPitch * polyCountY; // top point
		const UINT32 polyCountSq1 = polyCountSq + 1; // bottom point
		const UINT32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

		for (UINT32 p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			// create triangles which are at the top of the sphere

			buffer->Indices.push_back(polyCountSq);
			buffer->Indices.push_back(p2 + 1);
			buffer->Indices.push_back(p2);

			// create triangles which are at the bottom of the sphere

			buffer->Indices.push_back(polyCountSqM1 + p2);
			buffer->Indices.push_back(polyCountSqM1 + p2 + 1);
			buffer->Indices.push_back(polyCountSq1);
		}

		// create final triangle which is at the top of the sphere

		buffer->Indices.push_back(polyCountSq);
		buffer->Indices.push_back(polyCountX);
		buffer->Indices.push_back(polyCountX - 1);

		// create final triangle which is at the bottom of the sphere

		buffer->Indices.push_back(polyCountSqM1 + polyCountX - 1);
		buffer->Indices.push_back(polyCountSqM1);
		buffer->Indices.push_back(polyCountSq1);

		// calculate the angle which separates all points in a circle
		const FLOAT64 AngleX = 2 * PI / polyCountX;
		const FLOAT64 AngleY = PI / polyCountY;

		UINT32 i = 0;
		FLOAT64 axz;

		// we don't start at 0.

		FLOAT64 ay = 0;//AngleY / 2;

		//buffer->Vertices.set_used((polyCountXPitch * polyCountY) + 2);
		for (UINT32 y = 0; y < polyCountY; ++y)
		{
			ay += AngleY;
			const FLOAT64 sinay = sin(ay);
			axz = 0;

			// calculate the necessary vertices without the doubled one
			for (UINT32 xz = 0; xz < polyCountX; ++xz)
			{
				// calculate points position

				const Vector3 pos(static_cast<FLOAT32>(radius * cos(axz) * sinay),
					static_cast<FLOAT32>(radius * cos(ay)),
					static_cast<FLOAT32>(radius * sin(axz) * sinay));
				// for spheres the normal is the position
				Vector3 normal(pos);
				normal.normalise();

				// calculate texture coordinates via sphere mapping
				// tu is the same on each level, so only calculate once
				FLOAT32 tu = 0.5f;
				if (y == 0)
				{
					if (normal.y != -1.0f && normal.y != 1.0f)
						tu = static_cast<FLOAT32>(acos(Math::Clamp(normal.x / sinay, -1.0, 1.0)) * 0.5 *RECIPROCAL_PI64);
					if (normal.z < 0.0f)
						tu = 1 - tu;
				}
				else
					tu = buffer->Vertices[i - polyCountXPitch].TCoords.x;
				buffer->Vertices[i] = S3DVertex(pos.x, pos.y, pos.z,
					normal.x, normal.y, normal.z,
					clr, tu,
					static_cast<FLOAT32>(ay*RECIPROCAL_PI64));
				++i;
				axz += AngleX;
			}
			// This is the doubled vertex on the initial position
			buffer->Vertices[i] = S3DVertex(buffer->Vertices[i - polyCountX]);
			buffer->Vertices[i].TCoords.x = 1.0f;
			++i;
		}

		// the vertex at the top of the sphere
		buffer->Vertices[i] = S3DVertex(0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f, clr, 0.5f, 0.0f);

		// the vertex at the bottom of the sphere
		++i;
		buffer->Vertices[i] = S3DVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, clr, 0.5f, 1.0f);

		// recalculate bounding box

		buffer->BoundingBox.reset(buffer->Vertices[i].Pos);
		buffer->BoundingBox.addInternalPoint(buffer->Vertices[i - 1].Pos);
		buffer->BoundingBox.addInternalPoint(radius, 0.0f, 0.0f);
		buffer->BoundingBox.addInternalPoint(-radius, 0.0f, 0.0f);
		buffer->BoundingBox.addInternalPoint(0.0f, 0.0f, radius);
		buffer->BoundingBox.addInternalPoint(0.0f, 0.0f, -radius);

		SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(buffer);
		buffer->drop();

		mesh->setHardwareMappingHint(EHM_STATIC);
		mesh->recalculateBoundingBox();
		return mesh;
	}


	/* A cylinder with proper normals and texture coords */
	IMesh* CGeometryCreator::createCylinderMesh(FLOAT32 radius, FLOAT32 length,
		UINT32 tesselation, const ColourValue& color,
		bool closeTop, FLOAT32 oblique) const
	{
		SMeshBuffer* buffer = new SMeshBuffer();

		const FLOAT32 recTesselation = reciprocal((FLOAT32)tesselation);
		const FLOAT32 recTesselationHalf = recTesselation * 0.5f;
		const FLOAT32 angleStep = (PI * 2.f) * recTesselation;
		const FLOAT32 angleStepHalf = angleStep*0.5f;

		UINT32 i;
		S3DVertex v;
		v.Color = color;
		//buffer->Vertices.reallocate(tesselation * 4 + 4 + (closeTop ? 2 : 1));
		buffer->Vertices.reserve(tesselation * 4 + 4 + (closeTop ? 2 : 1));
		//buffer->Indices.reallocate((tesselation * 2 + 1)*(closeTop ? 12 : 9));
		buffer->Indices.reserve((tesselation * 2 + 1)*(closeTop ? 12 : 9));
		FLOAT32 tcx = 0.f;
		for (i = 0; i <= tesselation; ++i)
		{
			const FLOAT32 angle = angleStep * i;
			v.Pos.x = radius * cosf(angle);
			v.Pos.y = 0.f;
			v.Pos.z = radius * sinf(angle);
			v.Normal = v.Pos;
			v.Normal.normalise();
			v.TCoords.x = tcx;
			v.TCoords.y = 0.f;
			buffer->Vertices.push_back(v);

			v.Pos.x += oblique;
			v.Pos.y = length;
			v.Normal = v.Pos;
			v.Normal.normalise();
			v.TCoords.y = 1.f;
			buffer->Vertices.push_back(v);

			v.Pos.x = radius * cosf(angle + angleStepHalf);
			v.Pos.y = 0.f;
			v.Pos.z = radius * sinf(angle + angleStepHalf);
			v.Normal = v.Pos;
			v.Normal.normalise();
			v.TCoords.x = tcx + recTesselationHalf;
			v.TCoords.y = 0.f;
			buffer->Vertices.push_back(v);

			v.Pos.x += oblique;
			v.Pos.y = length;
			v.Normal = v.Pos;
			v.Normal.normalise();
			v.TCoords.y = 1.f;
			buffer->Vertices.push_back(v);
			tcx += recTesselation;
		}

		// indices for the main hull part
		const UINT32 nonWrappedSize = tesselation * 4;
		for (i = 0; i != nonWrappedSize; i += 2)
		{
			buffer->Indices.push_back(i + 2);
			buffer->Indices.push_back(i + 0);
			buffer->Indices.push_back(i + 1);

			buffer->Indices.push_back(i + 2);
			buffer->Indices.push_back(i + 1);
			buffer->Indices.push_back(i + 3);
		}

		// two closing quads between end and start
		buffer->Indices.push_back(0);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(i + 1);

		buffer->Indices.push_back(0);
		buffer->Indices.push_back(i + 1);
		buffer->Indices.push_back(1);

		// close down
		v.Pos.x = 0.f;
		v.Pos.y = 0.f;
		v.Pos.z = 0.f;
		v.Normal.x = 0.f;
		v.Normal.y = -1.f;
		v.Normal.z = 0.f;
		v.TCoords.x = 1.f;
		v.TCoords.y = 1.f;
		buffer->Vertices.push_back(v);

		UINT32 index = buffer->Vertices.size() - 1;

		for (i = 0; i != nonWrappedSize; i += 2)
		{
			buffer->Indices.push_back(index);
			buffer->Indices.push_back(i + 0);
			buffer->Indices.push_back(i + 2);
		}

		buffer->Indices.push_back(index);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(0);

		if (closeTop)
		{
			// close top
			v.Pos.x = oblique;
			v.Pos.y = length;
			v.Pos.z = 0.f;
			v.Normal.x = 0.f;
			v.Normal.y = 1.f;
			v.Normal.z = 0.f;
			v.TCoords.x = 0.f;
			v.TCoords.y = 0.f;
			buffer->Vertices.push_back(v);

			index = buffer->Vertices.size() - 1;

			for (i = 0; i != nonWrappedSize; i += 2)
			{
				buffer->Indices.push_back(i + 1);
				buffer->Indices.push_back(index);
				buffer->Indices.push_back(i + 3);
			}

			buffer->Indices.push_back(i + 1);
			buffer->Indices.push_back(index);
			buffer->Indices.push_back(1);
		}

		buffer->recalculateBoundingBox();
		SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(buffer);
		mesh->setHardwareMappingHint(EHM_STATIC);
		mesh->recalculateBoundingBox();
		buffer->drop();
		return mesh;
	}


	/* A cone with proper normals and texture coords */
	IMesh* CGeometryCreator::createConeMesh(FLOAT32 radius, FLOAT32 length, UINT32 tesselation,
		const ColourValue& colorTop,
		const ColourValue& colorBottom,
		FLOAT32 oblique) const
	{
		SMeshBuffer* buffer = new SMeshBuffer();

		const FLOAT32 angleStep = (PI * 2.f) / tesselation;
		const FLOAT32 angleStepHalf = angleStep*0.5f;

		S3DVertex v;
		UINT32 i;

		v.Color = colorTop;
		for (i = 0; i != tesselation; ++i)
		{
			FLOAT32 angle = angleStep * FLOAT32(i);

			v.Pos.x = radius * cosf(angle);
			v.Pos.y = 0.f;
			v.Pos.z = radius * sinf(angle);
			v.Normal = v.Pos;
			v.Normal.normalise();
			buffer->Vertices.push_back(v);

			angle += angleStepHalf;
			v.Pos.x = radius * cosf(angle);
			v.Pos.y = 0.f;
			v.Pos.z = radius * sinf(angle);
			v.Normal = v.Pos;
			v.Normal.normalise();
			buffer->Vertices.push_back(v);
		}
		const UINT32 nonWrappedSize = buffer->Vertices.size() - 1;

		// close top
		v.Pos.x = oblique;
		v.Pos.y = length;
		v.Pos.z = 0.f;
		v.Normal.x = 0.f;
		v.Normal.y = 1.f;
		v.Normal.z = 0.f;
		buffer->Vertices.push_back(v);

		UINT32 index = buffer->Vertices.size() - 1;

		for (i = 0; i != nonWrappedSize; i += 1)
		{
			buffer->Indices.push_back(i + 0);
			buffer->Indices.push_back(index);
			buffer->Indices.push_back(i + 1);
		}

		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(index);
		buffer->Indices.push_back(0);

		// close down
		v.Color = colorBottom;
		v.Pos.x = 0.f;
		v.Pos.y = 0.f;
		v.Pos.z = 0.f;
		v.Normal.x = 0.f;
		v.Normal.y = -1.f;
		v.Normal.z = 0.f;
		buffer->Vertices.push_back(v);

		index = buffer->Vertices.size() - 1;

		for (i = 0; i != nonWrappedSize; i += 1)
		{
			buffer->Indices.push_back(index);
			buffer->Indices.push_back(i + 0);
			buffer->Indices.push_back(i + 1);
		}

		buffer->Indices.push_back(index);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(0);

		buffer->recalculateBoundingBox();
		SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(buffer);
		buffer->drop();

		mesh->setHardwareMappingHint(EHM_STATIC);
		mesh->recalculateBoundingBox();
		return mesh;
	}


	void CGeometryCreator::addToBuffer(const S3DVertex& v, SMeshBuffer* Buffer) const
	{
		//const SINT32 tnidx = Buffer->Vertices.linear_reverse_search(v);
		const SINT32 tnidx = std::distance(Buffer->Vertices.begin(),std::find(Buffer->Vertices.begin(), Buffer->Vertices.end(), v));
		const bool alreadyIn = (tnidx != -1);
		UINT16 nidx = (UINT16)tnidx;
		if (!alreadyIn)
		{
			nidx = (UINT16)Buffer->Vertices.size();
			Buffer->Indices.push_back(nidx);
			Buffer->Vertices.push_back(v);
		}
		else
			Buffer->Indices.push_back(nidx);
	}


	IMesh* CGeometryCreator::createVolumeLightMesh(
		const UINT32 subdivideU, const UINT32 subdivideV,
		const ColourValue footColor, const ColourValue tailColor,
		const FLOAT32 lpDistance, const Vector3& lightDim) const
	{
		SMeshBuffer* Buffer = new SMeshBuffer();
		Buffer->setHardwareMappingHint(EHM_STATIC);

		const Vector3 lightPoint(0, -(lpDistance*lightDim.y), 0);
		const FLOAT32 ax = lightDim.x * 0.5f; // X Axis
		const FLOAT32 az = lightDim.z * 0.5f; // Z Axis

		Buffer->Vertices.clear();
		//Buffer->Vertices.reallocate(6 + 12 * (subdivideU + subdivideV));
		Buffer->Vertices.reserve(6 + 12 * (subdivideU + subdivideV));
		Buffer->Indices.clear();
		//Buffer->Indices.reallocate(6 + 12 * (subdivideU + subdivideV));
		Buffer->Indices.reserve(6 + 12 * (subdivideU + subdivideV));
		//draw the bottom foot.. the glowing region
		addToBuffer(S3DVertex(-ax, 0, az, 0, 0, 0, footColor, 0, 1), Buffer);
		addToBuffer(S3DVertex(ax, 0, az, 0, 0, 0, footColor, 1, 1), Buffer);
		addToBuffer(S3DVertex(ax, 0, -az, 0, 0, 0, footColor, 1, 0), Buffer);

		addToBuffer(S3DVertex(ax, 0, -az, 0, 0, 0, footColor, 1, 0), Buffer);
		addToBuffer(S3DVertex(-ax, 0, -az, 0, 0, 0, footColor, 0, 0), Buffer);
		addToBuffer(S3DVertex(-ax, 0, az, 0, 0, 0, footColor, 0, 1), Buffer);

		FLOAT32 tu = 0.f;
		const FLOAT32 tuStep = 1.f / subdivideU;
		FLOAT32 bx = -ax;
		const FLOAT32 bxStep = lightDim.x * tuStep;
		// Slices in X/U space
		for (UINT32 i = 0; i <= subdivideU; ++i)
		{
			// These are the two endpoints for a slice at the foot
			Vector3 end1(bx, 0.0f, -az);
			Vector3 end2(bx, 0.0f, az);

			end1 -= lightPoint;		// get a vector from point to lightsource
			end1.normalise();		// normalize vector
			end1 *= lightDim.y;	// multiply it out by shootlength

			end1.x += bx;			// Add the original point location to the vector
			end1.z -= az;

			// Do it again for the other point.
			end2 -= lightPoint;
			end2.normalise();
			end2 *= lightDim.y;

			end2.x += bx;
			end2.z += az;

			addToBuffer(S3DVertex(bx, 0, az, 0, 0, 0, footColor, tu, 1), Buffer);
			addToBuffer(S3DVertex(bx, 0, -az, 0, 0, 0, footColor, tu, 0), Buffer);
			addToBuffer(S3DVertex(end2.x, end2.y, end2.z, 0, 0, 0, tailColor, tu, 1), Buffer);

			addToBuffer(S3DVertex(bx, 0, -az, 0, 0, 0, footColor, tu, 0), Buffer);
			addToBuffer(S3DVertex(end1.x, end1.y, end1.z, 0, 0, 0, tailColor, tu, 0), Buffer);
			addToBuffer(S3DVertex(end2.x, end2.y, end2.z, 0, 0, 0, tailColor, tu, 1), Buffer);

			//back side
			addToBuffer(S3DVertex(-end2.x, end2.y, -end2.z, 0, 0, 0, tailColor, tu, 1), Buffer);
			addToBuffer(S3DVertex(-bx, 0, -az, 0, 0, 0, footColor, tu, 1), Buffer);
			addToBuffer(S3DVertex(-bx, 0, az, 0, 0, 0, footColor, tu, 0), Buffer);

			addToBuffer(S3DVertex(-bx, 0, az, 0, 0, 0, footColor, tu, 0), Buffer);
			addToBuffer(S3DVertex(-end1.x, end1.y, -end1.z, 0, 0, 0, tailColor, tu, 0), Buffer);
			addToBuffer(S3DVertex(-end2.x, end2.y, -end2.z, 0, 0, 0, tailColor, tu, 1), Buffer);
			tu += tuStep;
			bx += bxStep;
		}

		FLOAT32 tv = 0.f;
		const FLOAT32 tvStep = 1.f / subdivideV;
		FLOAT32 bz = -az;
		const FLOAT32 bzStep = lightDim.z * tvStep;
		// Slices in Z/V space
		for (UINT32 i = 0; i <= subdivideV; ++i)
		{
			// These are the two endpoints for a slice at the foot
			Vector3 end1(-ax, 0.0f, bz);
			Vector3 end2(ax, 0.0f, bz);

			end1 -= lightPoint;		// get a vector from point to lightsource
			end1.normalise();		// normalize vector
			end1 *= lightDim.y;	// multiply it out by shootlength

			end1.x -= ax;			// Add the original point location to the vector
			end1.z += bz;

			// Do it again for the other point.
			end2 -= lightPoint;
			end2.normalise();
			end2 *= lightDim.y;

			end2.x += ax;
			end2.z += bz;

			addToBuffer(S3DVertex(-ax, 0, bz, 0, 0, 0, footColor, 0, tv), Buffer);
			addToBuffer(S3DVertex(ax, 0, bz, 0, 0, 0, footColor, 1, tv), Buffer);
			addToBuffer(S3DVertex(end2.x, end2.y, end2.z, 0, 0, 0, tailColor, 1, tv), Buffer);

			addToBuffer(S3DVertex(end2.x, end2.y, end2.z, 0, 0, 0, tailColor, 1, tv), Buffer);
			addToBuffer(S3DVertex(end1.x, end1.y, end1.z, 0, 0, 0, tailColor, 0, tv), Buffer);
			addToBuffer(S3DVertex(-ax, 0, bz, 0, 0, 0, footColor, 0, tv), Buffer);

			//back side
			addToBuffer(S3DVertex(ax, 0, -bz, 0, 0, 0, footColor, 0, tv), Buffer);
			addToBuffer(S3DVertex(-ax, 0, -bz, 0, 0, 0, footColor, 1, tv), Buffer);
			addToBuffer(S3DVertex(-end2.x, end2.y, -end2.z, 0, 0, 0, tailColor, 1, tv), Buffer);

			addToBuffer(S3DVertex(-end2.x, end2.y, -end2.z, 0, 0, 0, tailColor, 1, tv), Buffer);
			addToBuffer(S3DVertex(-end1.x, end1.y, -end1.z, 0, 0, 0, tailColor, 0, tv), Buffer);
			addToBuffer(S3DVertex(ax, 0, -bz, 0, 0, 0, footColor, 0, tv), Buffer);
			tv += tvStep;
			bz += bzStep;
		}

		Buffer->recalculateBoundingBox();

		Buffer->Material.MaterialType = EMT_ONETEXTURE_BLEND;
		Buffer->Material.MaterialTypeParam = pack_textureBlendFunc(EBF_SRC_COLOR, EBF_SRC_ALPHA, EMFN_MODULATE_1X);

		Buffer->Material.Lighting = false;
		Buffer->Material.ZWriteEnable = false;	

		Buffer->setDirty(EBT_VERTEX_AND_INDEX);

		Buffer->recalculateBoundingBox();
		SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(Buffer);
		Buffer->drop();

		mesh->recalculateBoundingBox();
		return mesh;
	}

}