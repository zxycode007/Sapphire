#ifndef _SAPPHIRE_I_DUMMY_TRANSFORMATION_SCENE_NODE_
#define _SAPPHIRE_I_DUMMY_TRANSFORMATION_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! 为增加附加变换到场景图而导出场景节点
	/** 这个场景节点不会被渲染，并且不会想要set/getPosition(),set/getRotation 和 set/getScale。
	它只是一个带一个相关变换矩阵的简单的场景节点，
	 这个场景节点用于例如：IAnimatedMeshSceneNode在播放骨骼动画时模拟joint场景节点
	*/
	class IDummyTransformationSceneNode : public ISceneNode
	{
	public:

		//! 构造器
		IDummyTransformationSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id)
			: ISceneNode(parent, mgr, id) {}

		//! 返回一个对当前相关矩阵的引用
		/** 这是一个矩阵，这个场景节点用于代替缩放，变换和旋转 */
		virtual Matrix4& getRelativeTransformationMatrix() = 0;
	};
}

#endif