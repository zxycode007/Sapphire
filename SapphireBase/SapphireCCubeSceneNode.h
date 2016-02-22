#ifndef _SAPPHIRE_C_CUBE_SCENE_NODE_
#define _SAPPHIRE_C_CUBE_SCENE_NODE_

#include "SapphireIMeshSceneNode.h"
#include "SapphireSMesh.h"

namespace Sapphire
{
	class CCubeSceneNode : public IMeshSceneNode
	{
	public:

		//! constructor
		CCubeSceneNode(FLOAT32 size, ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		virtual ~CCubeSceneNode();

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

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_CUBE; }

		//! Creates shadow volume scene node as child of this node
		//! and returns a pointer to it.
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
			SINT32 id, bool zfailmethod = true, FLOAT32 infinity = 10000.0f);

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

		//! Sets a new mesh to display
		virtual void setMesh(IMesh* mesh) {}

		//! Returns the current mesh
		virtual IMesh* getMesh(void) { return Mesh; }

		//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
		/* In this way it is possible to change the materials a mesh causing all mesh scene nodes
		referencing this mesh to change too. */
		virtual void setReadOnlyMaterials(bool readonly) {}

		//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
		virtual bool isReadOnlyMaterials() const { return false; }

		//! Removes a child from this scene node.
		//! Implemented here, to be able to remove the shadow properly, if there is one,
		//! or to remove attached childs.
		virtual bool removeChild(ISceneNode* child);

	private:
		void setSize();

		IMesh* Mesh;
		IShadowVolumeSceneNode* Shadow;
		FLOAT32 Size;
	};
}

#endif
