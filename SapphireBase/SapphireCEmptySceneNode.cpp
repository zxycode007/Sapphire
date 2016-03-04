#include "SapphireCEmptySceneNode.h"
#include "SapphireIScenesManager.h"



namespace Sapphire
{

	//! constructor
	CEmptySceneNode::CEmptySceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id)
		: ISceneNode(parent, mgr, id)
	{
#ifdef _DEBUG
		setDebugName("CEmptySceneNode");
#endif

		setAutomaticCulling(EAC_OFF);
	}


	//! pre render event
	void CEmptySceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}


	//! render
	void CEmptySceneNode::render()
	{
		// do nothing
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CEmptySceneNode::getBoundingBox() const
	{
		return Box;
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CEmptySceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CEmptySceneNode* nb = new CEmptySceneNode(newParent,
			newManager, ID);

		nb->cloneMembers(this, newManager);
		nb->Box = Box;

		if (newParent)
			nb->drop();
		return nb;
	}
}