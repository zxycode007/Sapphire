#include "SapphireCTerrianTriangleSelector.h"
#include "SapphireCTerrainSceneNode.h"
#include "SapphireOS.h"


namespace Sapphire
{

	//! constructor
	CTerrainTriangleSelector::CTerrainTriangleSelector(ITerrainSceneNode* node, SINT32 LOD)
		: SceneNode(node)
	{
#ifdef _DEBUG
		setDebugName("CTerrainTriangleSelector");
#endif

		setTriangleData(node, LOD);
	}


	//! destructor
	CTerrainTriangleSelector::~CTerrainTriangleSelector()
	{
		TrianglePatches.TrianglePatchArray.clear();
	}


	//! Clears and sets triangle data
	void CTerrainTriangleSelector::setTriangleData(ITerrainSceneNode* node, SINT32 LOD)
	{
		// Get pointer to the GeoMipMaps vertices
		const S3DVertex2TCoords* vertices = static_cast<const S3DVertex2TCoords*>(node->getRenderBuffer()->getVertices());

		// Clear current data
		const SINT32 count = (static_cast<CTerrainSceneNode*>(node))->TerrainData.PatchCount;
		TrianglePatches.TotalTriangles = 0;
		TrianglePatches.NumPatches = count*count;

		//TrianglePatches.TrianglePatchArray.reallocate(TrianglePatches.NumPatches);
		TrianglePatches.TrianglePatchArray.reserve(TrianglePatches.NumPatches);
		for (SINT32 o = 0; o<TrianglePatches.NumPatches; ++o)
			TrianglePatches.TrianglePatchArray.push_back(SGeoMipMapTrianglePatch());

		triangle3df tri;
		vector<UINT32>::type indices;
		SINT32 tIndex = 0;
		for (SINT32 x = 0; x < count; ++x)
		{
			for (SINT32 z = 0; z < count; ++z)
			{
				TrianglePatches.TrianglePatchArray[tIndex].NumTriangles = 0;
				TrianglePatches.TrianglePatchArray[tIndex].Box = node->getBoundingBox(x, z);
				UINT32 indexCount = node->getIndicesForPatch(indices, x, z, LOD);

				//TrianglePatches.TrianglePatchArray[tIndex].Triangles.reallocate(indexCount / 3);
				TrianglePatches.TrianglePatchArray[tIndex].Triangles.reserve(indexCount / 3);
				for (UINT32 i = 0; i < indexCount; i += 3)
				{
					tri.pointA = vertices[indices[i + 0]].Pos;
					tri.pointB = vertices[indices[i + 1]].Pos;
					tri.pointC = vertices[indices[i + 2]].Pos;
					TrianglePatches.TrianglePatchArray[tIndex].Triangles.push_back(tri);
					++TrianglePatches.TrianglePatchArray[tIndex].NumTriangles;
				}

				TrianglePatches.TotalTriangles += TrianglePatches.TrianglePatchArray[tIndex].NumTriangles;
				++tIndex;
			}
		}
	}


	//! Gets all triangles.
	void CTerrainTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const Matrix4* transform) const
	{
		SINT32 count = TrianglePatches.TotalTriangles;

		if (count > arraySize)
			count = arraySize;

		Matrix4 mat;

		if (transform)
			mat = (*transform);

		SINT32 tIndex = 0;

		for (SINT32 i = 0; i<TrianglePatches.NumPatches; ++i)
		{
			if (tIndex + TrianglePatches.TrianglePatchArray[i].NumTriangles <= count)
				for (SINT32 j = 0; j<TrianglePatches.TrianglePatchArray[i].NumTriangles; ++j)
				{
					triangles[tIndex] = TrianglePatches.TrianglePatchArray[i].Triangles[j];

					//mat.transformVect(triangles[tIndex].pointA);
					//mat.transformVect(triangles[tIndex].pointB);
					//mat.transformVect(triangles[tIndex].pointC);
					triangles[tIndex].pointA = mat.transformAffine(triangles[tIndex].pointA);
					triangles[tIndex].pointB = mat.transformAffine(triangles[tIndex].pointB);
					triangles[tIndex].pointC = mat.transformAffine(triangles[tIndex].pointC);

					++tIndex;
				}
		}

		outTriangleCount = tIndex;
	}


	//! Gets all triangles which lie within a specific bounding box.
	void CTerrainTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const AxisAlignedBox& box, const Matrix4* transform) const
	{
		SINT32 count = TrianglePatches.TotalTriangles;

		if (count > arraySize)
			count = arraySize;

		Matrix4 mat;

		if (transform)
			mat = (*transform);

		SINT32 tIndex = 0;

		for (SINT32 i = 0; i<TrianglePatches.NumPatches; ++i)
		{
			if (tIndex + TrianglePatches.TrianglePatchArray[i].NumTriangles <= count &&
				TrianglePatches.TrianglePatchArray[i].Box.intersects(box))//TrianglePatches.TrianglePatchArray[i].Box.intersectsWithBox(box))
				for (SINT32 j = 0; j<TrianglePatches.TrianglePatchArray[i].NumTriangles; ++j)
				{
					triangles[tIndex] = TrianglePatches.TrianglePatchArray[i].Triangles[j];

					//mat.transformVect(triangles[tIndex].pointA);
					//mat.transformVect(triangles[tIndex].pointB);
					//mat.transformVect(triangles[tIndex].pointC);
					triangles[tIndex].pointA = mat.transformAffine(triangles[tIndex].pointA);
					triangles[tIndex].pointC = mat.transformAffine(triangles[tIndex].pointC);
					triangles[tIndex].pointB = mat.transformAffine(triangles[tIndex].pointB);

					++tIndex;
				}
		}

		outTriangleCount = tIndex;
	}


	//! Gets all triangles which have or may have contact with a 3d line.
	void CTerrainTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount, const line3d<FLOAT32>& line,
		const Matrix4* transform) const
	{
		const SINT32 count = Math::_min<SINT32>((SINT32)TrianglePatches.TotalTriangles, arraySize);

		Matrix4 mat;

		if (transform)
			mat = (*transform);

		SINT32 tIndex = 0;

		for (SINT32 i = 0; i<TrianglePatches.NumPatches; ++i)
		{
			if (tIndex + TrianglePatches.TrianglePatchArray[i].NumTriangles <= count
				//&& TrianglePatches.TrianglePatchArray[i].Box.intersectsWithLine(line))
				&& TrianglePatches.TrianglePatchArray[i].Box.intersectsWithLine<FLOAT32>(line))
			{
				for (SINT32 j = 0; j<TrianglePatches.TrianglePatchArray[i].NumTriangles; ++j)
				{
					triangles[tIndex] = TrianglePatches.TrianglePatchArray[i].Triangles[j];

					//mat.transformVect(triangles[tIndex].pointA);
					//mat.transformVect(triangles[tIndex].pointB);
					//mat.transformVect(triangles[tIndex].pointC);
					triangles[tIndex].pointA = mat.transformAffine(triangles[tIndex].pointA);
					triangles[tIndex].pointB = mat.transformAffine(triangles[tIndex].pointB);
					triangles[tIndex].pointC = mat.transformAffine(triangles[tIndex].pointC);

					++tIndex;
				}
			}
		}

		outTriangleCount = tIndex;
	}


	//! Returns amount of all available triangles in this selector
	SINT32 CTerrainTriangleSelector::getTriangleCount() const
	{
		return TrianglePatches.TotalTriangles;
	}


	ISceneNode* CTerrainTriangleSelector::getSceneNodeForTriangle(
		UINT32 triangleIndex) const
	{
		return SceneNode;
	}


	/* Get the number of TriangleSelectors that are part of this one.
	Only useful for MetaTriangleSelector others return 1
	*/
	UINT32 CTerrainTriangleSelector::getSelectorCount() const
	{
		return 1;
	}


	/* Get the TriangleSelector based on index based on getSelectorCount.
	Only useful for MetaTriangleSelector others return 'this' or 0
	*/
	ITriangleSelector* CTerrainTriangleSelector::getSelector(UINT32 index)
	{
		if (index)
			return 0;
		else
			return this;
	}


	/* Get the TriangleSelector based on index based on getSelectorCount.
	Only useful for MetaTriangleSelector others return 'this' or 0
	*/
	const ITriangleSelector* CTerrainTriangleSelector::getSelector(UINT32 index) const
	{
		if (index)
			return 0;
		else
			return this;
	}
}