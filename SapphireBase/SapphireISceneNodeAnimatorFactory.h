#ifndef _SAPPHIRE_ISCENE_NODE_FACTORY_
#define _SAPPHIRE_ISCENE_NODE_FACTORY_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	class ISceneNode;
	class ISceneNodeAnimator;

	//! 动态创建SceneNodeAnimator的接口
	/** 要添加自定义的SceneNodeAnimator到引擎并使它可以被场景管理器保存和加载， 简单的实现这个接口并
	通过ISceneManager::registerSceneNodeAnimatorFactory注册它到你场景管理去。
	注意：当你实现你自己的场景节点工厂时，不要调用ISceneNodeManager::grab()增加场景节点管理器的引用计数。
	因为这个场景节点管理器无论如何都会grab()工厂，否则是循环引用将会导致c场景管理器和它的节点无法被释放。
	*/
	class ISceneNodeAnimatorFactory : public virtual IReferenceCounter
	{
	public:

		//! 根据它的类型ID创建一个SceneNodeAnimator
		/** \param type: SceneNodeAnimator的类型ID 
		\param target: 新animator的目标场景节点
		\return Returns 指向这个新SceneNodeAnimator的指针或者不成功为NULL */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target) = 0;

		//! 创建一个基于类型名的SceneNodeAnimator
		/** \param typeName:  SceneNodeAnimator的类型名
		\param target: 新animator的目标场景节点
		\return 指向这个新SceneNodeAnimator的指针或者不成功为NULL */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const c8* typeName, ISceneNode* target) = 0;

		//! 返回这个工厂可以创建的SceneNodeAnimator的类型数量
		virtual UINT32 getCreatableSceneNodeAnimatorTypeCount() const = 0;

		//! 返回一个可创建的SceneNodeAnimator类型的类型
		/** \param idx: SceneNodeAnimator类型在这个工厂的索引，基于0到getCreatableSceneNodeTypeCount()*/
		virtual ESCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(UINT32 idx) const = 0;

		//! 返回一个可创建的SceneNodeAnimator类型的类型名
		/** \param idx: SceneNodeAnimator类型在这个工厂的索引，基于0到getCreatableSceneNodeTypeCount() */
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(UINT32 idx) const = 0;

		//! 返回一个可创建的SceneNodeAnimator类型的类型名
		/** \param type: SceneNodeAnimator的类型
		\return: 返回这个SceneNodeAnimator的类型名，如果这个工厂可以创建这种类型的话，否则返回0*/
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const = 0;
	};
}

#endif