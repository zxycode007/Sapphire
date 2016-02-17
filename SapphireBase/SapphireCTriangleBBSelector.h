#ifndef _SAPPHIRE_C_TRIANGLE_BB_SELECTOR_
#define _SAPPHIRE_C_TRIANGLE_BB_SELECTOR_

#include "SapphireCTriangleSelector.h"

namespace Sapphire
{

	//! Stupid triangle selector without optimization
	class CTriangleBBSelector : public CTriangleSelector
	{
	public:

		//! Constructs a selector based on a mesh
		CTriangleBBSelector(ISceneNode* node);

		//! Gets all triangles.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const Matrix4* transform = 0) const;

		//! Gets all triangles which lie within a specific bounding box.
		void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const AxisAlignedBox& box, const Matrix4* transform = 0) const;

		//! Gets all triangles which have or may have contact with a 3d line.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3df& line,
			const Matrix4* transform = 0) const;

	};
}
#endif