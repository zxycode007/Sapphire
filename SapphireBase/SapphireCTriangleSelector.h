#ifndef _SAPPHIRE_C_TRIANGLE_SELECTOR_
#define _SAPPHIRE_C_TRIANGLE_SELECTOR_

#include "SapphireITriangleSelector.h"
#include "SapphireIMesh.h"
#include  "SapphireAxisAlignedBox.h"

namespace Sapphire
{
	class ISceneNode;
	class IAnimatedMeshSceneNode;


	//! Stupid triangle selector without optimization
	class CTriangleSelector : public  ITriangleSelector
	{
	public:

		//! Constructs a selector based on a mesh
		CTriangleSelector(ISceneNode* node);

		//! Constructs a selector based on a mesh
		CTriangleSelector(const IMesh* mesh, ISceneNode* node);

		//! Constructs a selector based on an animated mesh scene node
		//!\param node An animated mesh scene node, which must have a valid mesh
		CTriangleSelector(IAnimatedMeshSceneNode* node);

		//! Constructs a selector based on a bounding box
		CTriangleSelector(const AxisAlignedBox& box, ISceneNode* node);

		//! Gets all triangles.
		void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const Matrix4* transform = 0) const;

		//! Gets all triangles which lie within a specific bounding box.
		void getTriangles(triangle3df* triangles, SINT32 arraySize, SINT32& outTriangleCount,
			const AxisAlignedBox& box, const Matrix4* transform = 0) const;

		//! Gets all triangles which have or may have contact with a 3d line.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3df& line,
			const Matrix4* transform = 0) const;

		//! Returns amount of all available triangles in this selector
		virtual SINT32 getTriangleCount() const;

		//! Return the scene node associated with a given triangle.
		virtual ISceneNode* getSceneNodeForTriangle(UINT32 triangleIndex) const { return SceneNode; }

		// Get the number of TriangleSelectors that are part of this one
		virtual UINT32 getSelectorCount() const;

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual ITriangleSelector* getSelector(UINT32 index);

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual const ITriangleSelector* getSelector(UINT32 index) const;

	protected:
		//! Create from a mesh
		virtual void createFromMesh(const IMesh* mesh);

		//! Update when the mesh has changed
		virtual void updateFromMesh(const IMesh* mesh) const;

		//! Update the triangle selector, which will only have an effect if it
		//! was built from an animated mesh and that mesh's frame has changed
		//! since the last time it was updated.
		virtual void update(void) const;

		ISceneNode* SceneNode;
		mutable vector<triangle3df>::type Triangles; // (mutable for CTriangleBBSelector)
		mutable AxisAlignedBox BoundingBox; // Allows for trivial rejection

		IAnimatedMeshSceneNode* AnimatedNode;
		mutable UINT32 LastMeshFrame;
	};

}

#endif