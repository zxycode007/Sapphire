#ifndef _SAPPHIRE_INODE_FACTORY_
#define _SAPPHIRE_INODE_FACTORY_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireESceneNodeTypes.h"


namespace Sapphire
{
	class ISceneNode;

	//! ��̬���������ڵ�Ľӿ�
	/** 
	Ҫ����Զ����SceneNode�����沢ʹ�����Ա���������������ͼ��أ� �򵥵�ʵ������ӿڲ�
	ͨ��ISceneManager::registerSceneNodeFactoryע�������㳡������ȥ��
	ע�⣺����ʵ�����Լ��ĳ����ڵ㹤��ʱ����Ҫ����ISceneNodeManager::grab()���ӳ����ڵ�����������ü�����
	��Ϊ��������ڵ������������ζ���grab()������������ѭ�����ý��ᵼ��c���������������Ľڵ��޷����ͷš�
	*/
	class ISceneNodeFactory : public virtual IReferenceCounter
	{
	public:
		//!������������ID����һ��SceneNode 
		/** \param type: Ҫ��ӵĳ����ڵ������ 
		\param parent: �½ڵ�ĸ��ڵ㣬 ����Ϊ�գ�Ϊ�յĻ��͸��ڵ���Ϊ���ڵ�
		\return ��������´����ڵ��ָ�룬���򷵻ؿ�*/
		virtual ISceneNode* addSceneNode(ESCENE_NODE_TYPE type, ISceneNode* parent = 0) = 0;

		//! ���ݳ����ڵ�������������һ�������ڵ㵽����ͼ��
		/** \param typeName: Ҫ��ӳ����ڵ��������
		\param parent: �½ڵ�ĸ��ڵ㣬 ����Ϊ�գ�Ϊ�յĻ��͸��ڵ���Ϊ���ڵ�
		\return Returns ��������´����ڵ��ָ�룬���򷵻ؿ� */
		virtual ISceneNode* addSceneNode(const c8* typeName, ISceneNode* parent = 0) = 0;

		//! ��������������Դ��������ڵ����͵�����
		virtual UINT32 getCreatableSceneNodeTypeCount() const = 0;

		//! ����һ���ɴ�����SceneNode���͵�����
		/** \param idx: SceneNode�������������������������0��getCreatableSceneNodeTypeCount()*/
		virtual ESCENE_NODE_TYPE getCreateableSceneNodeType(UINT32 idx) const = 0;

		//! ����һ���ɴ�����SceneNode���͵�������
		/** \param idx: SceneNode�������������������������0��getCreatableSceneNodeTypeCount() */
		virtual const c8* getCreateableSceneNodeTypeName(UINT32 idx) const = 0;

		//! ����һ���ɴ�����SceneNode���͵�������
		/** \param type: SceneNode������
		\return: �������SceneNode�����������������������Դ����������͵Ļ������򷵻�0*/
		virtual const c8* getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const = 0;
	};
}

#endif