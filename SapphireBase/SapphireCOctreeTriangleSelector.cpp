#include "SapphireCOctreeTriangleSelector.h"
#include "SapphireISceneNode.h"

#include "SapphireOS.h"


namespace Sapphire
{

	//! constructor
	COctreeTriangleSelector::COctreeTriangleSelector(const IMesh* mesh,
		ISceneNode* node, SINT32 minimalPolysPerNode)
		: CTriangleSelector(mesh, node), Root(0), NodeCount(0),
		MinimalPolysPerNode(minimalPolysPerNode)
	{
#ifdef _DEBUG
		setDebugName("COctreeTriangleSelector");
#endif

		if (!Triangles.empty())
		{
			const UINT32 start = Timer::getRealTime();

			// create the triangle octree
			Root = new SOctreeNode();
			Root->Triangles = Triangles;
			constructOctree(Root);

			c8 tmp[256];
			sprintf(tmp, "Needed %ums to create OctreeTriangleSelector.(%d nodes, %u polys)",
				Timer::getRealTime() - start, NodeCount, Triangles.size());
			Printer::log(tmp, LML_NORMAL);
		}
	}


	//! destructor
	COctreeTriangleSelector::~COctreeTriangleSelector()
	{
		delete Root;
	}


	void COctreeTriangleSelector::constructOctree(SOctreeNode* node)
	{
		++NodeCount;

		node->Box.reset(node->Triangles[0].pointA);

		// get bounding box
		const UINT32 cnt = node->Triangles.size();
		for (UINT32 i = 0; i<cnt; ++i)
		{
			node->Box.addInternalPoint(node->Triangles[i].pointA);
			node->Box.addInternalPoint(node->Triangles[i].pointB);
			node->Box.addInternalPoint(node->Triangles[i].pointC);
		}

		const Vector3& middle = node->Box.getCenter();
		//Vector3 edges[8];
		const Vector3* edges;
		//node->Box.getEdges(edges);
		//注意环绕顺序，Sapphire右手坐标系
		edges = node->Box.getAllCorners();

		AxisAlignedBox box;
		vector<triangle3df>::type keepTriangles;

		// 计算子节点

		//if (!node->Box.isEmpty() && (SINT32)node->Triangles.size() > MinimalPolysPerNode)
		if (!node->Box.isNull() && !node->Box.isInfinite() && (SINT32)node->Triangles.size() > MinimalPolysPerNode)
			for (SINT32 ch = 0; ch<8; ++ch)
			{
				box.reset(middle);
				box.addInternalPoint(edges[ch]);
				node->Child[ch] = new SOctreeNode();

				for (SINT32 i = 0; i<(SINT32)node->Triangles.size(); ++i)
				{
					if (node->Triangles[i].isTotalInsideBox(box))
					{
						node->Child[ch]->Triangles.push_back(node->Triangles[i]);
						//node->Triangles.erase(i);
						//--i;
					}
					else
					{
						keepTriangles.push_back(node->Triangles[i]);
					}
				}
				//memcpy(node->Triangles.pointer(), keepTriangles.pointer(),
					//sizeof(triangle3df)*keepTriangles.size());
				std::copy(node->Triangles.begin(), node->Triangles.begin() + node->Triangles.size(), keepTriangles.begin());
				//memcpy(node->Triangles.begin()._Ptr, keepTriangles.begin()._Ptr,
				//sizeof(triangle3df)*keepTriangles.size());

				//node->Triangles.set_used(keepTriangles.size());
				//keepTriangles.set_used(0);

				if (node->Child[ch]->Triangles.empty())
				{
					delete node->Child[ch];
					node->Child[ch] = 0;
				}
				else
					constructOctree(node->Child[ch]);
			}
	}


	//! Gets all triangles which lie within a specific bounding box.
	void COctreeTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const AxisAlignedBox& box,
		const Matrix4* transform) const
	{
		Matrix4 mat(Matrix4::ZERO);
		AxisAlignedBox invbox = box;

		if (SceneNode)
		{
			//SceneNode->getAbsoluteTransformation().getInverse(mat);
			mat = SceneNode->getAbsoluteTransformation();
			mat.inverse();
			//mat.transformBoxEx(invbox);
			invbox.transform(mat);
		}

		if (transform)
			mat = *transform;
		else
			mat = Matrix4::IDENTITY;

		if (SceneNode)
			//mat *= SceneNode->getAbsoluteTransformation();
			mat = SceneNode->getAbsoluteTransformation() * mat;

		SINT32 trianglesWritten = 0;

		if (Root)
			getTrianglesFromOctree(Root, trianglesWritten,
			arraySize, invbox, &mat, triangles);

		outTriangleCount = trianglesWritten;
	}


	void COctreeTriangleSelector::getTrianglesFromOctree(
		SOctreeNode* node, SINT32& trianglesWritten,
		SINT32 maximumSize, const AxisAlignedBox& box,
		const Matrix4* mat, triangle3df* triangles) const
	{
		if (!box.intersects(node->Box))    //if (!box.intersectsWithBox(node->Box))
			return;

		const UINT32 cnt = node->Triangles.size();

		for (UINT32 i = 0; i<cnt; ++i)
		{
			const triangle3df& srcTri = node->Triangles[i];
			// This isn't an accurate test, but it's fast, and the 
			// API contract doesn't guarantee complete accuracy.
			if (srcTri.isTotalOutsideBox(box))
				continue;

			triangle3df& dstTri = triangles[trianglesWritten];
			//mat->transformVect(dstTri.pointA, srcTri.pointA
			//mat->transformVect(dstTri.pointB, srcTri.pointB);
			//mat->transformVect(dstTri.pointC, srcTri.pointC);
			dstTri.pointA = mat->transformAffine(srcTri.pointA);
			dstTri.pointB = mat->transformAffine(srcTri.pointB);
			dstTri.pointC = mat->transformAffine(srcTri.pointC);
			++trianglesWritten;

			// Halt when the out array is full.
			if (trianglesWritten == maximumSize)
				return;
		}

		for (UINT32 i = 0; i<8; ++i)
			if (node->Child[i])
				getTrianglesFromOctree(node->Child[i], trianglesWritten,
				maximumSize, box, mat, triangles);
	}


	//! Gets all triangles which have or may have contact with a 3d line.
	// new version: from user Piraaate
	void COctreeTriangleSelector::getTriangles(triangle3df* triangles, SINT32 arraySize,
		SINT32& outTriangleCount, const line3df& line,
		const Matrix4* transform) const
	{
#if 0
		AxisAlignedBox box(line.start);
		box.addInternalPoint(line.end);

		// TODO: Could be optimized for line a little bit more.
		COctreeTriangleSelector::getTriangles(triangles, arraySize, outTriangleCount,
			box, transform);
#else

		Matrix4 mat(Matrix4::ZERO);

		Vector3 vectStartInv(line.start), vectEndInv(line.end);
		if (SceneNode)
		{
			mat = SceneNode->getAbsoluteTransformation();
			//mat.makeInverse();
			mat = Matrix4::IDENTITY;
			//mat.transformVect(vectStartInv, line.start);
			//mat.transformVect(vectEndInv, line.end);
			vectStartInv = mat.transformAffine(line.start);
			vectEndInv = mat.transformAffine(line.end);
		}
		line3d<FLOAT32> invline(vectStartInv, vectEndInv);

		//mat.makeIdentity();
		mat = Matrix4::IDENTITY;

		if (transform)
			mat = (*transform);

		if (SceneNode)
			mat = SceneNode->getAbsoluteTransformation() * mat;//mat *= SceneNode->getAbsoluteTransformation();

		SINT32 trianglesWritten = 0;

		if (Root)
			getTrianglesFromOctree(Root, trianglesWritten, arraySize, invline, &mat, triangles);

		outTriangleCount = trianglesWritten;
#endif
	}

	void COctreeTriangleSelector::getTrianglesFromOctree(SOctreeNode* node,
		SINT32& trianglesWritten, SINT32 maximumSize, const line3d<FLOAT32>& line,
		const Matrix4* transform, triangle3df* triangles) const
	{
		if (!node->Box.intersectsWithLine(line))
			return;

		SINT32 cnt = node->Triangles.size();
		if (cnt + trianglesWritten > maximumSize)
			cnt -= cnt + trianglesWritten - maximumSize;

		SINT32 i;

		if (transform->isIdentify())
		{
			for (i = 0; i<cnt; ++i)
			{
				triangles[trianglesWritten] = node->Triangles[i];
				++trianglesWritten;
			}
		}
		else
		{
			for (i = 0; i<cnt; ++i)
			{
				triangles[trianglesWritten] = node->Triangles[i];
				//transform->transformVect(triangles[trianglesWritten].pointA);
				//transform->transformVect(triangles[trianglesWritten].pointB);
				//transform->transformVect(triangles[trianglesWritten].pointC);
				triangles[trianglesWritten].pointA = transform->transformAffine(triangles[trianglesWritten].pointA);
				triangles[trianglesWritten].pointB = transform->transformAffine(triangles[trianglesWritten].pointB);
				triangles[trianglesWritten].pointC = transform->transformAffine(triangles[trianglesWritten].pointC);
				++trianglesWritten;
			}
		}

		for (i = 0; i<8; ++i)
			if (node->Child[i])
				getTrianglesFromOctree(node->Child[i], trianglesWritten,
				maximumSize, line, transform, triangles);
	}
}