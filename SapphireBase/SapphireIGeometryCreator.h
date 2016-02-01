#ifndef _SAPPHIRE_I_GEOMETRY_CREATOR_
#define _SAPPHIRE_I_GEOMETRY_CREATOR_

#include "SapphireIReferenceCounter.h"
#include "SapphireIMesh.h"
#include "SapphireIImage.h"

namespace Sapphire
{
	class IVideoDriver;
	class SMaterial;


	//! Helper class for creating geometry on the fly.
	/** You can get an instance of this class through ISceneManager::getGeometryCreator() */
	class IGeometryCreator : public IReferenceCounter
	{
	public:

		//! Creates a simple cube mesh.
		/**
		\param size Dimensions of the cube.
		\return Generated mesh.
		*/
		virtual IMesh* createCubeMesh(const Vector3& size = Vector3(5.f, 5.f, 5.f)) const = 0;

		//! Create a pseudo-random mesh representing a hilly terrain.
		/**
		\param tileSize The size of each tile.
		\param tileCount The number of tiles in each dimension.
		\param material The material to apply to the mesh.
		\param hillHeight The maximum height of the hills.
		\param countHills The number of hills along each dimension.
		\param textureRepeatCount The number of times to repeat the material texture along each dimension.
		\return Generated mesh.
		*/
		virtual IMesh* createHillPlaneMesh(
			const dimension2d<FLOAT32>& tileSize,
			const dimension2d<UINT32>& tileCount,
			SMaterial* material, FLOAT32 hillHeight,
			const dimension2d<FLOAT32>& countHills,
			const dimension2d<FLOAT32>& textureRepeatCount) const = 0;

		//! Create a simple rectangular textured plane mesh.
		/**
		\param tileSize The size of each tile.
		\param tileCount The number of tiles in each dimension.
		\param material The material to apply to the mesh.
		\param textureRepeatCount The number of times to repeat the material texture along each dimension.
		\return Generated mesh.
		*/
		IMesh* createPlaneMesh(
			const dimension2d<FLOAT32>& tileSize,
			const dimension2d<UINT32>& tileCount = dimension2du(1, 1),
			SMaterial* material = 0,
			const dimension2d<FLOAT32>& textureRepeatCount = dimension2d<FLOAT32>(1.f, 1.f)) const
		{
			return createHillPlaneMesh(tileSize, tileCount, material, 0.f, dimension2d<FLOAT32>(), textureRepeatCount);
		}

		//! Create a terrain mesh from an image representing a heightfield.
		/**
		\param texture The texture to apply to the terrain.
		\param heightmap An image that will be interpreted as a heightmap. The
		brightness (average color) of each pixel is interpreted as a height,
		with a 255 brightness pixel producing the maximum height.
		\param stretchSize The size that each pixel will produce, i.e. a
		512x512 heightmap
		and a stretchSize of (10.f, 20.f) will produce a mesh of size
		5120.f x 10240.f
		\param maxHeight The maximum height of the terrain.
		\param driver The current video driver.
		\param defaultVertexBlockSize (to be documented)
		\param debugBorders (to be documented)
		\return Generated mesh.
		*/
		virtual IMesh* createTerrainMesh(IImage* texture,
			IImage* heightmap,
			const dimension2d<FLOAT32>& stretchSize,
			FLOAT32 maxHeight, IVideoDriver* driver,
			const dimension2d<UINT32>& defaultVertexBlockSize,
			bool debugBorders = false) const = 0;

		//! Create an arrow mesh, composed of a cylinder and a cone.
		/**
		\param tesselationCylinder Number of quads composing the cylinder.
		\param tesselationCone Number of triangles composing the cone's roof.
		\param height Total height of the arrow
		\param cylinderHeight Total height of the cylinder, should be lesser
		than total height
		\param widthCylinder Diameter of the cylinder
		\param widthCone Diameter of the cone's base, should be not smaller
		than the cylinder's diameter
		\param colorCylinder color of the cylinder
		\param colorCone color of the cone
		\return Generated mesh.
		*/
		virtual IMesh* createArrowMesh(const UINT32 tesselationCylinder = 4,
			const UINT32 tesselationCone = 8, const FLOAT32 height = 1.f,
			const FLOAT32 cylinderHeight = 0.6f, const FLOAT32 widthCylinder = 0.05f,
			const FLOAT32 widthCone = 0.3f, const ColourValue colorCylinder = 0xFFFFFFFF,
			const ColourValue colorCone = 0xFFFFFFFF) const = 0;


		//! Create a sphere mesh.
		/**
		\param radius Radius of the sphere
		\param polyCountX Number of quads used for the horizontal tiling
		\param polyCountY Number of quads used for the vertical tiling
		\return Generated mesh.
		*/
		virtual IMesh* createSphereMesh(FLOAT32 radius = 5.f,
			UINT32 polyCountX = 16, UINT32 polyCountY = 16) const = 0;

		//! Create a cylinder mesh.
		/**
		\param radius Radius of the cylinder.
		\param length Length of the cylinder.
		\param tesselation Number of quads around the circumference of the cylinder.
		\param color The color of the cylinder.
		\param closeTop If true, close the ends of the cylinder, otherwise leave them open.
		\param oblique (to be documented)
		\return Generated mesh.
		*/
		virtual IMesh* createCylinderMesh(FLOAT32 radius, FLOAT32 length,
			UINT32 tesselation,
			const ColourValue& color = ColourValue(0xffffffff),
			bool closeTop = true, FLOAT32 oblique = 0.f) const = 0;

		//! Create a cone mesh.
		/**
		\param radius Radius of the cone.
		\param length Length of the cone.
		\param tesselation Number of quads around the circumference of the cone.
		\param colorTop The color of the top of the cone.
		\param colorBottom The color of the bottom of the cone.
		\param oblique (to be documented)
		\return Generated mesh.
		*/
		virtual IMesh* createConeMesh(FLOAT32 radius, FLOAT32 length, UINT32 tesselation,
			const ColourValue& colorTop = ColourValue(0xffffffff),
			const ColourValue& colorBottom = ColourValue(0xffffffff),
			FLOAT32 oblique = 0.f) const = 0;

		//! Create a volume light mesh.
		/**
		\param subdivideU Horizontal patch count.
		\param subdivideV Vertical patch count.
		\param footColor Color at the bottom of the light.
		\param tailColor Color at the mid of the light.
		\param lpDistance Virtual distance of the light point for normals.
		\param lightDim Dimensions of the light.
		\return Generated mesh.
		*/
		virtual IMesh* createVolumeLightMesh(
			const UINT32 subdivideU = 32, const UINT32 subdivideV = 32,
			const ColourValue footColor = 0xffffffff,
			const ColourValue tailColor = 0xffffffff,
			const FLOAT32 lpDistance = 8.f,
			const Vector3& lightDim = Vector3(1.f, 1.2f, 1.f)) const = 0;
	};
}

#endif