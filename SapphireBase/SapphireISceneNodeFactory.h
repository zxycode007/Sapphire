#ifndef _SAPPHIRE_INODE_FACTORY_
#define _SAPPHIRE_INODE_FACTORY_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireESceneNodeTypes.h"


namespace Sapphire
{
	class ISceneNode;

	//! 动态创建场景节点的接口
	/** 
	要添加自定义的SceneNode到引擎并使它可以被场景管理器保存和加载， 简单的实现这个接口并
	通过ISceneManager::registerSceneNodeFactory注册它到你场景管理去。
	注意：当你实现你自己的场景节点工厂时，不要调用ISceneNodeManager::grab()增加场景节点管理器的引用计数。
	因为这个场景节点管理器无论如何都会grab()工厂，否则是循环引用将会导致c场景管理器和它的节点无法被释放。
	*/
	class ISceneNodeFactory : public virtual IReferenceCounter
	{
	public:
		//!根据它的类型ID创建一个SceneNode 
		/** \param type: 要添加的场景节点的类型 
		\param parent: 新节点的父节点， 可以为空，为空的话就父节点作为根节点
		\return 返回这个新创建节点的指针，否则返回空*/
		virtual ISceneNode* addSceneNode(ESCENE_NODE_TYPE type, ISceneNode* parent = 0) = 0;

		//! 根据场景节点的类型名来添加一个场景节点到场景图中
		/** \param typeName: 要添加场景节点的类型名
		\param parent: 新节点的父节点， 可以为空，为空的话就父节点作为根节点
		\return Returns 返回这个新创建节点的指针，否则返回空 */
		virtual ISceneNode* addSceneNode(const c8* typeName, ISceneNode* parent = 0) = 0;

		//! 返回这个工厂可以创建场景节点类型的数量
		virtual UINT32 getCreatableSceneNodeTypeCount() const = 0;

		//! 返回一个可创建的SceneNode类型的类型
		/** \param idx: SceneNode类型在这个工厂的索引，基于0到getCreatableSceneNodeTypeCount()*/
		virtual ESCENE_NODE_TYPE getCreateableSceneNodeType(UINT32 idx) const = 0;

		//! 返回一个可创建的SceneNode类型的类型名
		/** \param idx: SceneNode类型在这个工厂的索引，基于0到getCreatableSceneNodeTypeCount() */
		virtual const c8* getCreateableSceneNodeTypeName(UINT32 idx) const = 0;

		//! 返回一个可创建的SceneNode类型的类型名
		/** \param type: SceneNode的类型
		\return: 返回这个SceneNode的类型名，如果这个工厂可以创建这种类型的话，否则返回0*/
		virtual const c8* getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const = 0;
	};
}

#endif