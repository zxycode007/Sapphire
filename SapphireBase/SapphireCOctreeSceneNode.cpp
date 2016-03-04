#include "SapphireCOctreeSceneNode.h"
#include "SapphireOctree.h"
#include "SapphireIScenesManager.h"
#include "SapphireIVideoDriver.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireIMeshCache.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireIMaterialRenderer.h"
#include "SapphireOS.h"
#include "SapphireCShadowVolumeSceneNode.h"


namespace Sapphire
{


	//! constructor
	COctreeSceneNode::COctreeSceneNode(ISceneNode* parent, ISceneManager* mgr,
		SINT32 id, SINT32 minimalPolysPerNode)
		: IMeshSceneNode(parent, mgr, id), StdOctree(0), LightMapOctree(0),
		TangentsOctree(0), VertexType((E_VERTEX_TYPE) - 1),
		MinimalPolysPerNode(minimalPolysPerNode), Mesh(0), Shadow(0),
		UseVBOs(OCTREE_USE_HARDWARE), UseVisibilityAndVBOs(OCTREE_USE_VISIBILITY),
		BoxBased(OCTREE_BOX_BASED)
	{
#ifdef _DEBUG
		setDebugName("COctreeSceneNode");
#endif
	}


	//! destructor
	COctreeSceneNode::~COctreeSceneNode()
	{
		if (Shadow)
			Shadow->drop();
		deleteTree();
	}


	void COctreeSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
		{
			// because this node supports rendering of mixed mode meshes consisting of
			// transparent and solid material at the same time, we need to go through all
			// materials, check of what type they are and register this node for the right
			// render pass according to that.

			IVideoDriver* driver = SceneManager->getVideoDriver();

			PassCount = 0;
			UINT32 transparentCount = 0;
			UINT32 solidCount = 0;

			// count transparent and solid materials in this scene node
			for (UINT32 i = 0; i<Materials.size(); ++i)
			{
				const IMaterialRenderer* const rnd =
					driver->getMaterialRenderer(Materials[i].MaterialType);

				if (rnd && rnd->isTransparent())
					++transparentCount;
				else
					++solidCount;

				if (solidCount && transparentCount)
					break;
			}

			// register according to material types counted

			if (solidCount)
				SceneManager->registerNodeForRendering(this, ESNRP_SOLID);

			if (transparentCount)
				SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

			ISceneNode::OnRegisterSceneNode();
		}
	}


	//! renders the node.
	void COctreeSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();

		if (VertexType == -1 || !driver)
			return;

		ICameraSceneNode* camera = SceneManager->getActiveCamera();
		if (!camera)
			return;

		bool isTransparentPass =
			SceneManager->getSceneNodeRenderPass() == ESNRP_TRANSPARENT;
		++PassCount;

		driver->setTransform(ETS_WORLD, AbsoluteTransformation);

		if (Shadow)
			Shadow->updateShadowVolumes();

		SViewFrustum frust = *camera->getViewFrustum();

		//transform the frustum to the current absolute transformation
		if (!AbsoluteTransformation.isIdentify())
		{
			//Matrix4 invTrans(AbsoluteTransformation, Matrix4::EM4CONST_INVERSE);
			Matrix4 invTrans = AbsoluteTransformation.inverse();
			frust.transform(invTrans);
		}

		const AxisAlignedBox &box = frust.getBoundingBox();

		switch (VertexType)
		{
		case EVT_STANDARD:
		{
			if (BoxBased)
				StdOctree->calculatePolys(box);
			else
				StdOctree->calculatePolys(frust);

			const Octree<S3DVertex>::SIndexData* d = StdOctree->getIndexData();

			for (UINT32 i = 0; i<Materials.size(); ++i)
			{
				if (0 == d[i].CurrentSize)
					continue;

				const IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					driver->drawIndexedTriangleList(
						&StdMeshes[i].Vertices[0], StdMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount == 1)
			{
				const AxisAlignedBox& box = frust.getBoundingBox();
				vector< const AxisAlignedBox* >::type boxes;
				SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if (DebugDataVisible & EDS_BBOX_BUFFERS)
				{
					StdOctree->getBoundingBoxes(box, boxes);
					for (UINT32 b = 0; b != boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if (DebugDataVisible & EDS_BBOX)
					driver->draw3DBox(Box, ColourValue::getColourValue(0, 255, 0, 0));
			}
		}
		break;
		case EVT_2TCOORDS:
		{
			if (BoxBased)
				LightMapOctree->calculatePolys(box);
			else
				LightMapOctree->calculatePolys(frust);

			const Octree<S3DVertex2TCoords>::SIndexData* d = LightMapOctree->getIndexData();

			for (UINT32 i = 0; i<Materials.size(); ++i)
			{
				if (0 == d[i].CurrentSize)
					continue;

				const IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					if (UseVBOs)
					{
						if (UseVisibilityAndVBOs)
						{
							//UINT16* oldPointer = LightMapMeshes[i].Indices.pointer();
							vector<UINT16>::type oldPointer = LightMapMeshes[i].Indices;
							//UINT16* oldPointer = LightMapMeshes[i].Indices.data();
							const UINT32 oldSize = LightMapMeshes[i].Indices.size();
							//LightMapMeshes[i].Indices.set_free_when_destroyed(false);
							//LightMapMeshes[i].Indices.set_pointer(d[i].Indices, d[i].CurrentSize, false, false);
							LightMapMeshes[i].Indices.resize(d[i].CurrentSize);
							std::copy(d[i].Indices, d[i].Indices + d[i].CurrentSize, LightMapMeshes[i].Indices.begin());
							LightMapMeshes[i].setDirty(EBT_INDEX);
							driver->drawMeshBuffer(&LightMapMeshes[i]);
							//LightMapMeshes[i].Indices.set_pointer(oldPointer, oldSize);
							LightMapMeshes[i].Indices = oldPointer;
							LightMapMeshes[i].setDirty(EBT_INDEX);
						}
						else
							driver->drawMeshBuffer(&LightMapMeshes[i]);
					}
					else
						driver->drawIndexedTriangleList(
						&LightMapMeshes[i].Vertices[0],
						LightMapMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount == 1)
			{
				const AxisAlignedBox &box = frust.getBoundingBox();
				vector< const AxisAlignedBox* >::type boxes;
				SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if (DebugDataVisible & EDS_BBOX_BUFFERS)
				{
					LightMapOctree->getBoundingBoxes(box, boxes);
					for (UINT32 b = 0; b<boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if (DebugDataVisible & EDS_BBOX)
					driver->draw3DBox(Box, ColourValue::getColourValue(0, 255, 0, 0));
			}
		}
		break;
		case EVT_TANGENTS:
		{
			if (BoxBased)
				TangentsOctree->calculatePolys(box);
			else
				TangentsOctree->calculatePolys(frust);

			const Octree<S3DVertexTangents>::SIndexData* d = TangentsOctree->getIndexData();

			for (UINT32 i = 0; i<Materials.size(); ++i)
			{
				if (0 == d[i].CurrentSize)
					continue;

				const IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					driver->drawIndexedTriangleList(
						&TangentsMeshes[i].Vertices[0], TangentsMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount == 1)
			{
				const AxisAlignedBox &box = frust.getBoundingBox();
				vector< const AxisAlignedBox* >::type boxes;
				SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if (DebugDataVisible & EDS_BBOX_BUFFERS)
				{
					TangentsOctree->getBoundingBoxes(box, boxes);
					for (UINT32 b = 0; b<boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if (DebugDataVisible & EDS_BBOX)
					driver->draw3DBox(Box, ColourValue::getColourValue(0, 255, 0, 0));
			}
		}
		break;
		}
	}


	//! Removes a child from this scene node.
	//! Implemented here, to be able to remove the shadow properly, if there is one,
	//! or to remove attached childs.
	bool COctreeSceneNode::removeChild(ISceneNode* child)
	{
		if (child && Shadow == child)
		{
			Shadow->drop();
			Shadow = 0;
		}

		return ISceneNode::removeChild(child);
	}


	//! Creates shadow volume scene node as child of this node
	//! and returns a pointer to it.
	IShadowVolumeSceneNode* COctreeSceneNode::addShadowVolumeSceneNode(
		const IMesh* shadowMesh, SINT32 id, bool zfailmethod, FLOAT32 infinity)
	{
		if (!SceneManager->getVideoDriver()->queryFeature(EVDF_STENCIL_BUFFER))
			return 0;

		if (!shadowMesh)
			shadowMesh = Mesh; // if null is given, use the mesh of node

		if (Shadow)
			Shadow->drop();

		Shadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id, zfailmethod, infinity);
		return Shadow;
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& COctreeSceneNode::getBoundingBox() const
	{
		return Box;
	}


	//! creates the tree
	/* This method has a lot of duplication and overhead. Moreover, the tangents mesh conversion does not really work. I think we need a a proper mesh implementation for octrees, which handle all vertex types internally. Converting all structures to just one vertex type is always problematic.
	Thanks to Auria for fixing major parts of this method. */
	bool COctreeSceneNode::createTree(IMesh* mesh)
	{
		if (!mesh)
			return false;

		//MeshName = SceneManager->getMeshCache()->getMeshName(mesh);
		MeshName = SceneManager->getMeshCache()->getMeshName(mesh).getPath();

		mesh->grab();
		deleteTree();

		Mesh = mesh;

		const UINT32 beginTime = Timer::getRealTime();

		UINT32 nodeCount = 0;
		UINT32 polyCount = 0;
		UINT32 i;

		Box = mesh->getBoundingBox();

		if (mesh->getMeshBufferCount())
		{
			// check for "larger" buffer types
			VertexType = EVT_STANDARD;
			UINT32 meshReserve = 0;
			for (i = 0; i<mesh->getMeshBufferCount(); ++i)
			{
				const IMeshBuffer* b = mesh->getMeshBuffer(i);
				if (b->getVertexCount() && b->getIndexCount())
				{
					++meshReserve;
					if (b->getVertexType() == EVT_2TCOORDS)
						VertexType = EVT_2TCOORDS;
					else if (b->getVertexType() == EVT_TANGENTS)
						VertexType = EVT_TANGENTS;
				}
			}
			//Materials.reallocate(Materials.size() + meshReserve);
			Materials.reserve(Materials.size() + meshReserve);

			switch (VertexType)
			{
			case EVT_STANDARD:
			{
				//StdMeshes.reallocate(StdMeshes.size() + meshReserve);
				StdMeshes.reserve(StdMeshes.size() + meshReserve);
				for (i = 0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());

						StdMeshes.push_back(Octree<S3DVertex>::SMeshChunk());
						Octree<S3DVertex>::SMeshChunk &nchunk = StdMeshes.back();
						nchunk.MaterialId = Materials.size() - 1;

						UINT32 v;
						//nchunk.Vertices.reallocate(b->getVertexCount());
						nchunk.Vertices.reserve(b->getVertexCount());
						switch (b->getVertexType())
						{
						case EVT_STANDARD:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertex*)b->getVertices())[v]);
							break;
						case EVT_2TCOORDS:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertex2TCoords*)b->getVertices())[v]);
							break;
						case EVT_TANGENTS:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();

						//nchunk.Indices.reallocate(b->getIndexCount());
						nchunk.Indices.reserve(b->getIndexCount());
						for (v = 0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				StdOctree = new Octree<S3DVertex>(StdMeshes, MinimalPolysPerNode);
				nodeCount = StdOctree->getNodeCount();
			}
			break;
			case EVT_2TCOORDS:
			{
				//LightMapMeshes.reallocate(LightMapMeshes.size() + meshReserve);
				LightMapMeshes.reserve(LightMapMeshes.size() + meshReserve);

				for (i = 0; i < mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());
						LightMapMeshes.push_back(Octree<S3DVertex2TCoords>::SMeshChunk());
						Octree<S3DVertex2TCoords>::SMeshChunk& nchunk = LightMapMeshes.back();
						nchunk.MaterialId = Materials.size() - 1;

						if (UseVisibilityAndVBOs)
						{
							nchunk.setHardwareMappingHint(EHM_STATIC, EBT_VERTEX);
							nchunk.setHardwareMappingHint(EHM_DYNAMIC, EBT_INDEX);
						}
						else
							nchunk.setHardwareMappingHint(EHM_STATIC);

						UINT32 v;
						//nchunk.Vertices.reallocate(b->getVertexCount());
						nchunk.Vertices.reserve(b->getVertexCount());
						switch (b->getVertexType())
						{
						case EVT_STANDARD:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertex*)b->getVertices())[v]);
							break;
						case EVT_2TCOORDS:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertex2TCoords*)b->getVertices())[v]);
							break;
						case EVT_TANGENTS:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();
						//nchunk.Indices.reallocate(b->getIndexCount());
						nchunk.Indices.reserve(b->getIndexCount());
						for (v = 0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				LightMapOctree = new Octree<S3DVertex2TCoords>(LightMapMeshes, MinimalPolysPerNode);
				nodeCount = LightMapOctree->getNodeCount();
			}
			break;
			case EVT_TANGENTS:
			{
				//TangentsMeshes.reallocate(TangentsMeshes.size() + meshReserve);
				TangentsMeshes.reserve(TangentsMeshes.size() + meshReserve);

				for (UINT32 i = 0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());
						TangentsMeshes.push_back(Octree<S3DVertexTangents>::SMeshChunk());
						Octree<S3DVertexTangents>::SMeshChunk& nchunk = TangentsMeshes.back();
						nchunk.MaterialId = Materials.size() - 1;

						UINT32 v;
						//nchunk.Vertices.reallocate(b->getVertexCount());
						nchunk.Vertices.reserve(b->getVertexCount());
						switch (b->getVertexType())
						{
						case EVT_STANDARD:
							for (v = 0; v<b->getVertexCount(); ++v)
							{
								const S3DVertex& tmpV = ((S3DVertex*)b->getVertices())[v];
								nchunk.Vertices.push_back(S3DVertexTangents(tmpV.Pos, tmpV.Color, tmpV.TCoords));
							}
							break;
						case EVT_2TCOORDS:
							for (v = 0; v<b->getVertexCount(); ++v)
							{
								const S3DVertex2TCoords& tmpV = ((S3DVertex2TCoords*)b->getVertices())[v];
								nchunk.Vertices.push_back(S3DVertexTangents(tmpV.Pos, tmpV.Color, tmpV.TCoords));
							}
							break;
						case EVT_TANGENTS:
							for (v = 0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();
						//nchunk.Indices.reallocate(b->getIndexCount());
						nchunk.Indices.reserve(b->getIndexCount());
						for (v = 0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				TangentsOctree = new Octree<S3DVertexTangents>(TangentsMeshes, MinimalPolysPerNode);
				nodeCount = TangentsOctree->getNodeCount();
			}
			break;
			}
		}

		const UINT32 endTime = Timer::getRealTime();
		c8 tmp[255];
		sprintf(tmp, "Needed %ums to create Octree SceneNode.(%u nodes, %u polys)",
			endTime - beginTime, nodeCount, polyCount / 3);
		Printer::log(tmp, LML_NORMAL);

		return true;
	}


	//! returns the material based on the zero based index i. To get the amount
	//! of materials used by this scene node, use getMaterialCount().
	//! This function is needed for inserting the node into the scene hirachy on a
	//! optimal position for minimizing renderstate changes, but can also be used
	//! to directly modify the material of a scene node.
	SMaterial& COctreeSceneNode::getMaterial(UINT32 i)
	{
		if (i >= Materials.size())
			return ISceneNode::getMaterial(i);

		return Materials[i];
	}


	//! returns amount of materials used by this scene node.
	UINT32 COctreeSceneNode::getMaterialCount() const
	{
		return Materials.size();
	}


	//! Writes attributes of the scene node.
	void COctreeSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		ISceneNode::serializeAttributes(out, options);

		out->addInt("MinimalPolysPerNode", MinimalPolysPerNode);
		out->addString("Mesh", MeshName.c_str());
	}


	//! Reads attributes of the scene node.
	void COctreeSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		const SINT32 oldMinimal = MinimalPolysPerNode;

		MinimalPolysPerNode = in->getAttributeAsInt("MinimalPolysPerNode");
		path newMeshStr = in->getAttributeAsString("Mesh");

		IMesh* newMesh = 0;

		if (newMeshStr == "")
			newMeshStr = MeshName;

		IAnimatedMesh* newAnimatedMesh = SceneManager->getMesh(newMeshStr.c_str());

		if (newAnimatedMesh)
			newMesh = newAnimatedMesh->getMesh(0);

		if (newMesh && ((MeshName != newMeshStr) || (MinimalPolysPerNode != oldMinimal)))
		{
			// recalculate tree
			createTree(newMesh);
		}

		ISceneNode::deserializeAttributes(in, options);
	}


	void COctreeSceneNode::deleteTree()
	{
		delete StdOctree;
		StdOctree = 0;
		StdMeshes.clear();

		delete LightMapOctree;
		LightMapOctree = 0;
		LightMapMeshes.clear();

		delete TangentsOctree;
		TangentsOctree = 0;
		TangentsMeshes.clear();

		Materials.clear();

		if (Mesh)
			Mesh->drop();
	}

	void COctreeSceneNode::setMesh(IMesh* mesh)
	{
		createTree(mesh);
	}

	IMesh* COctreeSceneNode::getMesh(void)
	{
		return Mesh;
	}

	void COctreeSceneNode::setReadOnlyMaterials(bool readonly)
	{
		// Do nothing
	}

	bool COctreeSceneNode::isReadOnlyMaterials() const
	{
		return false;
	}

}