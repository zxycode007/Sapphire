#ifndef _SAPPHIRE_C_BONE_SCENE_NODE_
#define _SAPPHIRE_C_BONE_SCENE_NODE_

#include "SapphireIBoneSceneNode.h"

namespace Sapphire
{
	class CBoneSceneNode : public IBoneSceneNode
	{
	public:

		//! constructor
		CBoneSceneNode(ISceneNode* parent, ISceneManager* mgr,
			SINT32 id = -1, UINT32 boneIndex = 0, const c8* boneName = 0);

		//! Returns the index of the bone
		virtual UINT32 getBoneIndex() const;

		//! Sets the animation mode of the bone. Returns true if successful.
		virtual bool setAnimationMode(E_BONE_ANIMATION_MODE mode);

		//! Gets the current animation mode of the bone
		virtual E_BONE_ANIMATION_MODE getAnimationMode() const;

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		/*
		//! Returns the relative transformation of the scene node.
		//virtual matrix4 getRelativeTransformation() const;
		*/

		virtual void OnAnimate(UINT32 timeMs);

		virtual void updateAbsolutePositionOfAllChildren();

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! How the relative transformation of the bone is used
		virtual void setSkinningSpace(E_BONE_SKINNING_SPACE space)
		{
			SkinningSpace = space;
		}

		virtual E_BONE_SKINNING_SPACE getSkinningSpace() const
		{
			return SkinningSpace;
		}

	private:
		void helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node);

		UINT32 BoneIndex;

		AxisAlignedBox Box;

		E_BONE_ANIMATION_MODE AnimationMode;
		E_BONE_SKINNING_SPACE SkinningSpace;
	};
}

#endif