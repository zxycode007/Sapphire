#include "SapphireCompileConifg.h"
#ifdef  _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_

#include "SapphireCBoneSceneNode.h"

namespace Sapphire
{

	//! constructor
	CBoneSceneNode::CBoneSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		UINT32 boneIndex, const c8* boneName)
		: IBoneSceneNode(parent, mgr, id), BoneIndex(boneIndex),
		AnimationMode(EBAM_AUTOMATIC), SkinningSpace(EBSS_LOCAL)
	{
#ifdef _DEBUG
		setDebugName("CBoneSceneNode");
#endif
		setName(boneName);
	}


	//! Returns the index of the bone
	UINT32 CBoneSceneNode::getBoneIndex() const
	{
		return BoneIndex;
	}


	//! Sets the animation mode of the bone. Returns true if successful.
	bool CBoneSceneNode::setAnimationMode(E_BONE_ANIMATION_MODE mode)
	{
		AnimationMode = mode;
		return true;
	}


	//! Gets the current animation mode of the bone
	E_BONE_ANIMATION_MODE CBoneSceneNode::getAnimationMode() const
	{
		return AnimationMode;
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CBoneSceneNode::getBoundingBox() const
	{
		return Box;
	}


	/*
	//! Returns the relative transformation of the scene node.
	core::matrix4 CBoneSceneNode::getRelativeTransformation() const
	{
	return core::matrix4(); // RelativeTransformation;
	}
	*/


	void CBoneSceneNode::OnAnimate(UINT32 timeMs)
	{
		if (IsVisible)
		{
			// animate this node with all animators

			ISceneNodeAnimatorList::iterator ait = Animators.begin();
			for (; ait != Animators.end(); ++ait)
				(*ait)->animateNode(this, timeMs);

			// update absolute position
			//updateAbsolutePosition();

			// perform the post render process on all children
			ISceneNodeList::iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				(*it)->OnAnimate(timeMs);
		}
	}


	void CBoneSceneNode::helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node)
	{
		Node->updateAbsolutePosition();

		ISceneNodeList::const_iterator it = Node->getChildren().begin();
		for (; it != Node->getChildren().end(); ++it)
		{
			helper_updateAbsolutePositionOfAllChildren((*it));
		}
	}


	void CBoneSceneNode::updateAbsolutePositionOfAllChildren()
	{
		helper_updateAbsolutePositionOfAllChildren(this);
	}


	void CBoneSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		IBoneSceneNode::serializeAttributes(out, options);
		out->addInt("BoneIndex", BoneIndex);
		out->addEnum("AnimationMode", AnimationMode, BoneAnimationModeNames);
	}


	void CBoneSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		BoneIndex = in->getAttributeAsInt("BoneIndex");
		AnimationMode = (E_BONE_ANIMATION_MODE)in->getAttributeAsEnumeration("AnimationMode", BoneAnimationModeNames);
		// for legacy files (before 1.5)
		const String boneName = in->getAttributeAsString("BoneName");
		setName(boneName);
		IBoneSceneNode::deserializeAttributes(in, options);
		// TODO: add/replace bone in parent with bone from mesh
	}
}


#endif