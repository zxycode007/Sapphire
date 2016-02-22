#ifndef _SAPPHIRE_C_MESH_SCENE_NODE_
#define _SAPPHIRE_C_MESH_SCENE_NODE_

#include "SapphireIMeshSceneNode.h"
#include "SapphireIMesh.h"

namespace Sapphire
{
	class CMeshSceneNode : public IMeshSceneNode
	{
	public:

		//! constructor
		CMeshSceneNode(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CMeshSceneNode();

		//! frame
		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

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
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_MESH; }

		//! Sets a new mesh
		virtual void setMesh(IMesh* mesh);

		//! Returns the current mesh
		virtual IMesh* getMesh(void) { return Mesh; }

		//! Creates shadow volume scene node as child of this node
		//! and returns a pointer to it.
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
			SINT32 id, bool zfailmethod = true, FLOAT32 infinity = 10000.0f);

		//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
		/* In this way it is possible to change the materials a mesh causing all mesh scene nodes
		referencing this mesh to change too. */
		virtual void setReadOnlyMaterials(bool readonly);

		//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
		virtual bool isReadOnlyMaterials() const;

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

		//! Removes a child from this scene node.
		//! Implemented here, to be able to remove the shadow properly, if there is one,
		//! or to remove attached childs.
		virtual bool removeChild(ISceneNode* child);

	protected:

		void copyMaterials();

		vector<SMaterial>::type Materials;
		AxisAlignedBox Box;
		SMaterial ReadOnlyMaterial;

		IMesh* Mesh;
		IShadowVolumeSceneNode* Shadow;

		SINT32 PassCount;
		bool ReadOnlyMaterials;
	};
}

#endif