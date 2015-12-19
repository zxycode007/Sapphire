#ifndef _SAPPHIRE_I_BONE_SCENE_NODE_
#define _SAPPHIRE_I_BONE_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! 不同骨骼动画模式的枚举
	enum E_BONE_ANIMATION_MODE
	{
		//! 这个骨骼通常被激活，除法它父节点不被激活
		EBAM_AUTOMATIC = 0,

		//! 这个骨骼通过皮肤激活， 如果它的父节点不被激活，那么动画仍然继续 
		EBAM_ANIMATED,

		//! 这个骨骼不是通过皮肤激活
		EBAM_UNANIMATED,

		//! 骨骼动画模式数量
		EBAM_COUNT

	};

	enum E_BONE_SKINNING_SPACE
	{
		//! 本地蒙皮，标志
		EBSS_LOCAL = 0,

		//! 全局蒙皮
		EBSS_GLOBAL,

		EBSS_COUNT
	};

	//! 骨骼动画模式名
	const c8* const BoneAnimationModeNames[] =
	{
		"automatic",
		"animated",
		"unanimated",
		0,
	};


	//! 用于骨骼动画的骨骼接口
	/** 用于ISkinnedMesh 和 IAnimatedMeshSceneNode. */
	class IBoneSceneNode : public ISceneNode
	{
	public:

		IBoneSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id = -1) :
			ISceneNode(parent, mgr, id), positionHint(-1), scaleHint(-1), rotationHint(-1) { }

		//! 获取骨骼的索引
		virtual UINT32 getBoneIndex() const = 0;

		//! 设置骨骼动画模式
		/** \return 如果成功返回true （不常使用）*/
		virtual bool setAnimationMode(E_BONE_ANIMATION_MODE mode) = 0;

		//! 获取这骨骼当前的动画模式
		virtual E_BONE_ANIMATION_MODE getAnimationMode() const = 0;

		//! 获取这个节点的AABB盒子
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! 返回这个场景节点的相关变换
		//virtual Matrix getRelativeTransformation() const = 0;

		//! 动画的方法
		virtual void OnAnimate(UINT32 timeMs) = 0;

		//! 渲染方法
		/** 作为不可见的骨骼什么都不用做 */
		virtual void render() { }

		//! 设置蒙皮空间
		//骨骼的相关变换如何使用
		virtual void setSkinningSpace(E_BONE_SKINNING_SPACE space) = 0;

		//! 获取蒙皮空间
		//! 获取骨骼的相关变换如何使用
		virtual E_BONE_SKINNING_SPACE getSkinningSpace() const = 0;

		//! 通过基于相对于父节点的位置更新绝对位置
		virtual void updateAbsolutePositionOfAllChildren() = 0;

		SINT32 positionHint;
		SINT32 scaleHint;
		SINT32 rotationHint;
	};
}

#endif