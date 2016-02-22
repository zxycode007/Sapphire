#ifndef _SAPPHIRE_C_SKYBOX_SCENE_NODE_
#define _SAPPHIRE_C_SKYBOX_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	// Skybox, rendered with zbuffer turned off, before all other nodes.
	class CSkyBoxSceneNode : public ISceneNode
	{
	public:

		//! constructor
		CSkyBoxSceneNode(ITexture* top, ITexture* bottom, ITexture* left,
			ITexture* right, ITexture* front, ITexture* back,
			ISceneNode* parent, ISceneManager* mgr, SINT32 id);

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
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SKY_BOX; }

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

	private:

		AxisAlignedBox Box;
		UINT16 Indices[4];
		S3DVertex Vertices[4 * 6];
		SMaterial Material[6];
	};
}

#endif