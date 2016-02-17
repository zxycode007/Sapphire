#include "SapphireCTriangleSelector.h"
#include "SapphireISceneNode.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireIAnimatedMeshSceneNode.h"


namespace Sapphire
{

	//! constructor
	CTriangleSelector::CTriangleSelector(ISceneNode* node)
		: SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
	{
#ifdef _DEBUG
		setDebugName("CTriangleSelector");
#endif

		BoundingBox.reset(Vector3(0.f, 0.f, 0.f));
	}


	//! constructor
	CTriangleSelector::CTriangleSelector(const AxisAlignedBox& box, ISceneNode* node)
		: SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
	{
#ifdef _DEBUG
		setDebugName("CTriangleSelector");
#endif

		BoundingBox = box;
		// TODO
	}


	//! constructor
	CTriangleSelector::CTriangleSelector(const IMesh* mesh, ISceneNode* node)
		: SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
	{
#ifdef _DEBUG
		setDebugName("CTriangleSelector");
#endif

		createFromMesh(mesh);
	}


	CTriangleSelector::CTriangleSelector(IAnimatedMeshSceneNode* node)
		: SceneNode(node), AnimatedNode(node), LastMeshFrame(0)
	{
#ifdef _DEBUG
		setDebugName("CTriangleSelector");
#endif

		if (!AnimatedNode)
			return;

		IAnimatedMesh* animatedMesh = AnimatedNode->getMesh();
		if (!animatedMesh)
			return;

		LastMeshFrame = (UINT32)AnimatedNode->getFrameNr();
		IMesh* mesh = animatedMesh->getMesh(LastMeshFrame);

		if (mesh)
			createFromMesh(mesh);
	}


	void CTriangleSelector::createFromMesh(const IMesh* mesh)
	{
		const UINT32 cnt = mesh->getMeshBufferCount();
		UINT32 totalFaceCount = 0;
		for (UINT32 j = 0; j<cnt; ++j)
			totalFaceCount += mesh->getMeshBuffer(j)->getIndexCount();
		totalFaceCount /= 3;
		//Triangles.reallocate(totalFaceCount);
		Triangles.reserve(totalFaceCount);
		BoundingBox.reset(Vector3(0.f, 0.f, 0.f));

		for (UINT32 i = 0; i<cnt; ++i)
		{
			const IMeshBuffer* buf = mesh->getMeshBuffer(i);

			const UINT32 idxCnt = buf->getIndexCount();
			const UINT16* const indices = buf->getIndices();

			for (UINT32 j = 0; j<idxCnt; j += 3)
			{
				Triangles.push_back(triangle3df(
					buf->getPosition(indices[j + 0]),
					buf->getPosition(indices[j + 1]),
					buf->getPosition(indices[j + 2])));
				//const triangle3df& tri = Triangles.getLast();
				const triangle3df& tri = Triangles.back();
				BoundingBox.addInternalPoint(tri.pointA);
				BoundingBox.addInternalPoint(tri.pointB);
				BoundingBox.addInternalPoint(tri.pointC);
			}
		}
	}


	void CTriangleSelector::updateFromMesh(const IMesh* mesh) const
	{
		if (!mesh)
			return;

		UINT32 meshBuffers = mesh->getMeshBufferCount();
		UINT32 triangleCount = 0;

		BoundingBox.reset(Vector3(0.f, 0.f, 0.f));
		for (UINT32 i = 0; i < meshBuffers; ++i)
		{
			IMeshBuffer* buf = mesh->getMeshBuffer(i);
			UINT32 idxCnt = buf->getIndexCount();
			const UINT16* indices = buf->getIndices();

			for (UINT32 index = 0; index < idxCnt; index += 3)
			{
				triangle3df& tri = Triangles[triangleCount++];
				tri.pointA = buf->getPosition(indices[index + 0]);
				tri.pointB = buf->getPosition(indices[index + 1]);
				tri.pointC = buf->getPosition(indices[index + 2]);
				BoundingBox.addInternalPoint(tri.pointA);
				BoundingBox.addInternalPoint(tri.pointB);
				BoundingBox.addInternalPoint(tri.pointC);
			}
		}
	}


	void CTriangleSelector::update(void) const
	{
		if (!AnimatedNode)
			return; //< harmless no-op

		const UINT32 currentFrame = (UINT32)AnimatedNode->getFrameNr();
		if (currentFrame == LastMeshFrame)
			return; //< Nothing to do

		LastMeshFrame = currentFrame;
		IAnimatedMesh * animatedMesh = AnimatedNode->getMesh();

		if (animatedMesh)
		{
			IMesh * mesh = animatedMesh->getMesh(LastMeshFrame);

			if (mesh)
				updateFromMesh(mesh);
		}
	}


	//! Gets all triangles.
	void CTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const Matrix4* transform) const
	{
		// Update my triangles if necessary
		update();

		UINT32 cnt = Triangles.size();
		if (cnt > (UINT32)arraySize)
			cnt = (UINT32)arraySize;

		Matrix4 mat;
		if (transform)
			mat = *transform;
		if (SceneNode)
			//mat *= SceneNode->getAbsoluteTransformation();
			//ÁÐ¾ØÕó£¬´Ó×óµ½ÓÒ³Ë
			mat = SceneNode->getAbsoluteTransformation() * mat;

		for (UINT32 i = 0; i<cnt; ++i)
		{
			//mat.transformVect(triangles[i].pointA, Triangles[i].pointA);
			//mat.transformVect(triangles[i].pointB, Triangles[i].pointB);
			//mat.transformVect(triangles[i].pointC, Triangles[i].pointC);
			triangles[i].pointA = mat.transformAffine(triangles[i].pointA);
			triangles[i].pointB = mat.transformAffine(triangles[i].pointB);
			triangles[i].pointC = mat.transformAffine(triangles[i].pointC);
		}

		outTriangleCount = cnt;
	}


	//! Gets all triangles which lie within a specific bounding box.
	void CTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const AxisAlignedBox& box,
		const Matrix4* transform) const
	{
		// Update my triangles if necessary
		update();

		Matrix4 mat(Matrix4::ZERO);
		AxisAlignedBox tBox(box);

		if (SceneNode)
		{
			//SceneNode->getAbsoluteTransformation().getInverse(mat);
			mat = SceneNode->getAbsoluteTransformation().inverse();
			tBox.transform(mat);
			//mat.transformBoxEx(tBox);
		}
		if (transform)
			mat = *transform;
		else
			mat = Matrix4::IDENTITY; //	mat.makeIdentity();
		if (SceneNode)
			//mat *= SceneNode->getAbsoluteTransformation();
			mat = SceneNode->getAbsoluteTransformation() * mat;

		outTriangleCount = 0;

		//if (!tBox.intersectsWithBox(BoundingBox))
		if (!tBox.intersects(BoundingBox))
			return;

		SINT32 triangleCount = 0;
		const UINT32 cnt = Triangles.size();
		for (UINT32 i = 0; i<cnt; ++i)
		{
			// This isn't an accurate test, but it's fast, and the 
			// API contract doesn't guarantee complete accuracy.
			if (Triangles[i].isTotalOutsideBox(tBox))
				continue;

			triangles[triangleCount] = Triangles[i];
			//mat.transformVect(triangles[triangleCount].pointA);
			//mat.transformVect(triangles[triangleCount].pointB);
			//mat.transformVect(triangles[triangleCount].pointC);
			triangles[triangleCount].pointA = mat.transformAffine(triangles[triangleCount].pointA);
			triangles[triangleCount].pointA = mat.transformAffine(triangles[triangleCount].pointB);
			triangles[triangleCount].pointA = mat.transformAffine(triangles[triangleCount].pointC);

			++triangleCount;

			if (triangleCount == arraySize)
				break;
		}

		outTriangleCount = triangleCount;
	}


	//! Gets all triangles which have or may have contact with a 3d line.
	void CTriangleSelector::getTriangles(triangle3df* triangles,
		SINT32 arraySize, SINT32& outTriangleCount,
		const line3df& line,
		const Matrix4* transform) const
	{
		// Update my triangles if necessary
		update();

		AxisAlignedBox box(line.start,line.start);
		box.addInternalPoint(line.end);

		// TODO: Could be optimized for line a little bit more.
		getTriangles(triangles, arraySize, outTriangleCount,
			box, transform);
	}


	//! Returns amount of all available triangles in this selector
	SINT32 CTriangleSelector::getTriangleCount() const
	{
		return Triangles.size();
	}


	/* Get the number of TriangleSelectors that are part of this one.
	Only useful for MetaTriangleSelector others return 1
	*/
	UINT32 CTriangleSelector::getSelectorCount() const
	{
		return 1;
	}


	/* Get the TriangleSelector based on index based on getSelectorCount.
	Only useful for MetaTriangleSelector others return 'this' or 0
	*/
	ITriangleSelector* CTriangleSelector::getSelector(UINT32 index)
	{
		if (index)
			return 0;
		else
			return this;
	}


	/* Get the TriangleSelector based on index based on getSelectorCount.
	Only useful for MetaTriangleSelector others return 'this' or 0
	*/
	const ITriangleSelector* CTriangleSelector::getSelector(UINT32 index) const
	{
		if (index)
			return 0;
		else
			return this;
	}
}