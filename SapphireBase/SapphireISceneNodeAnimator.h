#ifndef _SAPPHIRE_ISCENE_NODE_ANIMATOR_
#define _SAPPHIRE_ISCENE_NODE_ANIMATOR_

#include "SapphireIAttributeExchangingObject.h"
#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireIEventRecevier.h"
#include "SapphireESceneNodeAnimatorType.h"

namespace Sapphire
{
	class ISceneNode;
	class ISceneManager;
	class IAttributes;
	//! 激活一个场景节点。 能够活动位置，旋转，材质，等这下
	/** 一个场景节点animator能够使一个场景节点非常简单的方式活动起来。它能够改变它的位置，旋转，缩放和材质。有很多animator可供选择。
	你可以用ISceneManager接口创建创建节点animator
	*/
	class ISceneNodeAnimator : public SceneAlloc, public IAttributeExchangingObject, public IEventReceiver, public IReferenceCounter
	{
	public:
		//! 激活一个场景节点
		/** \param node 要激活的节点
		\param timeMs 当前时间的毫秒数 */
		virtual void animateNode(ISceneNode* node, UINT32 timeMs) = 0;

		//! 创建当前animator的一个克隆
		/** 请注意你需要drop（）(IReferenceCounter::drop()) */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node,
			ISceneManager* newManager = 0) = 0;

		//! 如果这个animator接受事件，返回true
		/** 当关联到一个激活的相机，这个animator将可以响应类似鼠标和键盘的事件*/
		virtual bool isEventReceiverEnabled() const
		{
			return false;
		}

		//! 每一个接收器，为相机控制的animator重载这个函数
		virtual bool OnEvent(const SEvent& event)
		{
			return false;
		}

		//! 返回这个场景节点animator的类型
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_UNKNOWN;
		}

		//! 返回这个animator是否运行结束
		/** 这只对用一个分离结束状态的非循环式animator有效
		\return true 如果animator结束，返回true，如果false，它依然运行*/
		virtual bool hasFinished(void) const
		{
			return false;
		}
	};

	typedef SharedPtr<ISceneNodeAnimator> ISceneNodeAnimatorPtr;
}

#endif