#ifndef _SAPPHIRE_C_EMPTY_SCENE_NODE_
#define _SAPPHIRE_C_EMPTY_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class CEmptySceneNode : public ISceneNode
	{
	public:

		//! constructor
		CEmptySceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id);

		//! returns the axis aligned bounding box of this node
		virtual const  AxisAlignedBox& getBoundingBox() const;

		//! This method is called just before the rendering process of the whole scene.
		virtual void OnRegisterSceneNode();

		//! does nothing.
		virtual void render();

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_EMPTY; }

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

	private:

		AxisAlignedBox Box;
	};

}

#endif