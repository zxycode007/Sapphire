#ifndef _SAPPHIRE_C_OCTREE_TRIANGLE_SELECTOR_
#define _SAPPHIRE_C_OCTREE_TRIANGLE_SELECTOR_

#include "SapphireCTriangleSelector.h"

namespace Sapphire
{

	class ISceneNode;

	//! Stupid triangle selector without optimization
	class COctreeTriangleSelector : public CTriangleSelector
	{
	public:

		//! Constructs a selector based on a mesh
		COctreeTriangleSelector(const IMesh* mesh, ISceneNode* node, SINT32 minimalPolysPerNode);

		virtual ~COctreeTriangleSelector();

		//! Gets all triangles which lie within a specific bounding box.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const AxisAlignedBox& box, const Matrix4* transform = 0) const;

		//! Gets all triangles which have or may have contact with a 3d line.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3df& line,
			const Matrix4* transform = 0) const;

	private:

		struct SOctreeNode
		{
			SOctreeNode()
			{
				for (UINT32 i = 0; i != 8; ++i)
					Child[i] = 0;
			}

			~SOctreeNode()
			{
				for (UINT32 i = 0; i != 8; ++i)
					delete Child[i];
			}

			vector<triangle3df>::type Triangles;
			SOctreeNode* Child[8];
			AxisAlignedBox Box;
		};


		void constructOctree(SOctreeNode* node);
		void deleteEmptyNodes(SOctreeNode* node);
		void getTrianglesFromOctree(SOctreeNode* node, SINT32& trianglesWritten,
			SINT32 maximumSize, const AxisAlignedBox& box,
			const Matrix4* transform,
			triangle3df* triangles) const;

		void getTrianglesFromOctree(SOctreeNode* node, SINT32& trianglesWritten,
			SINT32 maximumSize, const line3d<FLOAT32>& line,
			const Matrix4* transform,
			triangle3df* triangles) const;

		SOctreeNode* Root;
		SINT32 NodeCount;
		SINT32 MinimalPolysPerNode;
	};
}

#endif