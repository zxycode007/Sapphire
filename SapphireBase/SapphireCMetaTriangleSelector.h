#ifndef _SAPPHIRE_C_META_TRIANGLE_SELECTOR_
#define _SAPPHIRE_C_META_TRIANGLE_SELECTOR_


#include "SapphireIMetaTriangleSelector.h"

namespace Sapphire
{

	//! Interface for making multiple triangle selectors work as one big selector.
	class CMetaTriangleSelector : public IMetaTriangleSelector
	{
	public:

		//! constructor
		CMetaTriangleSelector();

		//! destructor
		virtual ~CMetaTriangleSelector();

		//! Get amount of all available triangles in this selector
		virtual SINT32 getTriangleCount() const;

		//! Gets all triangles.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const Matrix4* transform = 0) const;

		//! Gets all triangles which lie within a specific bounding box.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const AxisAlignedBox& box,
			const Matrix4* transform = 0) const;

		//! Gets all triangles which have or may have contact with a 3d line.
		virtual void getTriangles(triangle3df* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3d<FLOAT32>& line,
			const Matrix4* transform = 0) const;

		//! Adds a triangle selector to the collection of triangle selectors
		//! in this metaTriangleSelector.
		virtual void addTriangleSelector(ITriangleSelector* toAdd);

		//! Removes a specific triangle selector which was added before	from the collection.
		virtual bool removeTriangleSelector(ITriangleSelector* toRemove);

		//! Removes all triangle selectors from the collection.
		virtual void removeAllTriangleSelectors();

		//! Get the scene node associated with a given triangle.
		virtual ISceneNode* getSceneNodeForTriangle(UINT32 triangleIndex) const;

		// Get the number of TriangleSelectors that are part of this one
		virtual UINT32 getSelectorCount() const;

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual ITriangleSelector* getSelector(UINT32 index);

		// Get the TriangleSelector based on index based on getSelectorCount
		virtual const ITriangleSelector* getSelector(UINT32 index) const;

	private:

		vector<ITriangleSelector*>::type TriangleSelectors;
	};
}

#endif