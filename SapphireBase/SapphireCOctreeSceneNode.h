#ifndef _SAPPHIRE_C_OCTREE_SCENE_NODE_
#define _SAPPHIRE_C_OCTREE_SCENE_NODE_

#include "SapphireIMeshSceneNode.h"
#include "SapphireOctree.h"

namespace Sapphire
{
	//! implementation of the IBspTreeSceneNode
	class COctreeSceneNode : public IMeshSceneNode
	{
	public:

		//! constructor
		COctreeSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			SINT32 minimalPolysPerNode = 512);

		//! destructor
		virtual ~COctreeSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! creates the tree
		bool createTree(IMesh* mesh);

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual SMaterial& getMaterial(UINT32 i);

		//! returns amount of materials used by this scene node.
		virtual UINT32 getMaterialCount() const;

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_OCTREE; }

		//! Sets a new mesh to display
		virtual void setMesh(IMesh* mesh);

		//! Get the currently defined mesh for display.
		virtual IMesh* getMesh(void);

		//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
		virtual void setReadOnlyMaterials(bool readonly);

		//! Check if the scene node should not copy the materials of the mesh but use them in a read only style
		virtual bool isReadOnlyMaterials() const;

		//! Creates shadow volume scene node as child of this node
		//! and returns a pointer to it.
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
			SINT32 id, bool zfailmethod = true, FLOAT32 infinity = 10000.0f);

		//! Removes a child from this scene node.
		//! Implemented here, to be able to remove the shadow properly, if there is one,
		//! or to remove attached childs.
		virtual bool removeChild(ISceneNode* child);

	private:

		void deleteTree();

		AxisAlignedBox Box;

		Octree<S3DVertex>* StdOctree;
		vector< Octree<S3DVertex>::SMeshChunk >::type StdMeshes;

		Octree<S3DVertex2TCoords>* LightMapOctree;
		vector< Octree<S3DVertex2TCoords>::SMeshChunk >::type LightMapMeshes;

		Octree<S3DVertexTangents>* TangentsOctree;
		vector< Octree<S3DVertexTangents>::SMeshChunk >::type TangentsMeshes;

		E_VERTEX_TYPE VertexType;
		vector< SMaterial >::type Materials;

		String MeshName;
		SINT32 MinimalPolysPerNode;
		SINT32 PassCount;

		IMesh * Mesh;
		IShadowVolumeSceneNode* Shadow;
		//! use VBOs for rendering where possible
		bool UseVBOs;
		//! use visibility information together with VBOs
		bool UseVisibilityAndVBOs;
		//! use bounding box or frustum for calculate polys
		bool BoxBased;
	};
}

#endif