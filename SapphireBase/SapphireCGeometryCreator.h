#ifndef _SAPPHIRE_C_GEOMETRY_CREATOR_
#define _SAPPHIRE_C_GEOMETRY_CREATOR_

#include "SapphireIGeometryCreator.h"
#include "SapphireSMeshBuffer.h"

namespace Sapphire
{
	//! class for creating geometry on the fly
	class CGeometryCreator : public IGeometryCreator
	{
		void addToBuffer(const S3DVertex& v, SMeshBuffer* Buffer) const;
	public:
		IMesh* createCubeMesh(const Vector3& size) const;

		IMesh* createHillPlaneMesh(
			const dimension2d<FLOAT32>& tileSize, const dimension2d<UINT32>& tileCount,
			SMaterial* material, FLOAT32 hillHeight, const dimension2d<FLOAT32>& countHills,
			const dimension2d<FLOAT32>& textureRepeatCount) const;

		IMesh* createTerrainMesh(IImage* texture,
			IImage* heightmap, const dimension2d<FLOAT32>& stretchSize,
			FLOAT32 maxHeight, IVideoDriver* driver,
			const dimension2d<UINT32>& defaultVertexBlockSize,
			bool debugBorders = false) const;

		IMesh* createArrowMesh(const UINT32 tesselationCylinder,
			const UINT32 tesselationCone, const FLOAT32 height,
			const FLOAT32 cylinderHeight, const FLOAT32 width0,
			const FLOAT32 width1, const ColourValue vtxColor0,
			const ColourValue vtxColor1) const;

		IMesh* createSphereMesh(FLOAT32 radius, UINT32 polyCountX, UINT32 polyCountY) const;

		IMesh* createCylinderMesh(FLOAT32 radius, FLOAT32 length, UINT32 tesselation,
			const ColourValue& color = 0xffffffff,
			bool closeTop = true, FLOAT32 oblique = 0.f) const;

		IMesh* createConeMesh(FLOAT32 radius, FLOAT32 length, UINT32 tesselation,
			const ColourValue& colorTop = 0xffffffff,
			const ColourValue& colorBottom = 0xffffffff,
			FLOAT32 oblique = 0.f) const;

		IMesh* createVolumeLightMesh(
			const UINT32 subdivideU = 32, const UINT32 subdivideV = 32,
			const ColourValue footColor = 0xffffffff,
			const ColourValue tailColor = 0xffffffff,
			const FLOAT32 lpDistance = 8.f,
			const Vector3& lightDim = Vector3(1.f, 1.2f, 1.f)) const;
	};
}

#endif