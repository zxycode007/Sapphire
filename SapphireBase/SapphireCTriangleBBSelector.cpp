#include "SapphireCTriangleBBSelector.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{

	//! constructor
	CTriangleBBSelector::CTriangleBBSelector(ISceneNode* node)
		: CTriangleSelector(node)
	{
#ifdef _DEBUG
		setDebugName("CTriangleBBSelector");
#endif

		Triangles.reserve(12); // a box has 12 triangles.
	}



	//! Gets all triangles.
	void CTriangleBBSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const Matrix4* transform) const
	{
		if (!SceneNode)
			return;

		// construct triangles
		const AxisAlignedBox& box = SceneNode->getBoundingBox();
		//Vector3 edges[8];
		const Vector3* edges;
		//box.getEdges(edges);
		edges = box.getAllCorners();
		/*
		1-----2
		/|    /|
		/ |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		正面顶点顺时针，反面逆时针
		*/
		Triangles[0].set(edges[1], edges[6], edges[0]);
		Triangles[1].set(edges[1], edges[5], edges[6]);

		Triangles[2].set(edges[1], edges[0], edges[2]);
		Triangles[3].set(edges[2], edges[0], edges[3]);

		Triangles[4].set(edges[2], edges[3], edges[7]);
		Triangles[5].set(edges[4], edges[2], edges[7]);

		Triangles[6].set(edges[4], edges[7], edges[6]);
		Triangles[7].set(edges[4], edges[6], edges[5]);

		Triangles[8].set(edges[5], edges[1], edges[2]);
		Triangles[9].set(edges[5], edges[2], edges[4]);

		Triangles[10].set(edges[6], edges[3], edges[0]);
		Triangles[11].set(edges[6], edges[7], edges[3]);
		//Triangles[0].set(edges[3], edges[0], edges[2]);
		//Triangles[1].set(edges[3], edges[1], edges[0]);

		//Triangles[2].set(edges[3], edges[2], edges[7]);
		//Triangles[3].set(edges[7], edges[2], edges[6]);

		//Triangles[4].set(edges[7], edges[6], edges[4]);
		//Triangles[5].set(edges[5], edges[7], edges[4]);

		//Triangles[6].set(edges[5], edges[4], edges[0]);
		//Triangles[7].set(edges[5], edges[0], edges[1]);

		//Triangles[8].set(edges[1], edges[3], edges[7]);
		//Triangles[9].set(edges[1], edges[7], edges[5]);

		//Triangles[10].set(edges[0], edges[6], edges[2]);
		//Triangles[11].set(edges[0], edges[4], edges[6]);


		// call parent
		CTriangleSelector::getTriangles(triangles, arraySize, outTriangleCount, transform);
	}

	void CTriangleBBSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const AxisAlignedBox& box,
		const Matrix4* transform) const
	{
		return getTriangles(triangles, arraySize, outTriangleCount, transform);
	}

	void CTriangleBBSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const line3df& line,
		const Matrix4* transform) const
	{
		return getTriangles(triangles, arraySize, outTriangleCount, transform);
	}

}