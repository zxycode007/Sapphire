#ifndef _SAPPHIRE_C_TERRIAN_TRIANGLE_SELECTOR_
#define _SAPPHIRE_C_TERRIAN_TRIANGLE_SELECTOR_

#include "SapphireITriangleSelector.h"

namespace Sapphire
{

	class ITerrainSceneNode;

	//! Triangle Selector for the TerrainSceneNode
	/** The code for the TerrainTriangleSelector is based on the GeoMipMapSelector
	developed by Spintz. He made it available for Irrlicht and allowed it to be
	distributed under this licence. I only modified some parts. A lot of thanks go
	to him.
	*/
	class CTerrainTriangleSelector : public ITriangleSelector
	{
	public:

		//! Constructs a selector based on an ITerrainSceneNode
		CTerrainTriangleSelector(ITerrainSceneNode* node, SINT32 LOD);

		//! Destructor
		virtual ~CTerrainTriangleSelector();

		//! Clears and sets triangle data
		virtual void setTriangleData(ITerrainSceneNode* node, SINT32 LOD);

		//! Gets all triangles.
		void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const Matrix4* transform = 0) const;

		//! Gets all triangles which lie within a specific bounding box.
		void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const AxisAlignedBox& box, const Matrix4* transform = 0) const;

		//! Gets all triangles which have or may have contact with a 3d line.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3d<FLOAT32>& line,
			const Matrix4* transform = 0) const;

		//! Returns amount of all available triangles in this selector
		virtual SINT32 getTriangleCount() const;

		//! Return the scene node associated with a given triangle.
		virtual ISceneNode* getSceneNodeForTriangle(UINT32 triangleIndex) const;

		// Get the number of TriangleSelectors that are part of this one
		virtual UINT32 getSelectorCount() const;

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual ITriangleSelector* getSelector(UINT32 index);

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual const ITriangleSelector* getSelector(UINT32 index) const;

	private:

		friend class CTerrainSceneNode;

		struct SGeoMipMapTrianglePatch
		{
			vector<triangle3df>::type Triangles;
			SINT32 NumTriangles;
			AxisAlignedBox Box;
		};

		struct SGeoMipMapTrianglePatches
		{
			SGeoMipMapTrianglePatches() :
				NumPatches(0), TotalTriangles(0)
			{
			}

			vector<SGeoMipMapTrianglePatch>::type TrianglePatchArray;
			SINT32 NumPatches;
			UINT32 TotalTriangles;
		};

		ITerrainSceneNode* SceneNode;
		SGeoMipMapTrianglePatches TrianglePatches;
	};
}

#endif