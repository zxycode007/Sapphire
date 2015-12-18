#ifndef _SAPPHIRE_ISCENE_NODE_FACTORY_
#define _SAPPHIRE_ISCENE_NODE_FACTORY_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	class ISceneNode;
	class ISceneNodeAnimator;

	//! ��̬����SceneNodeAnimator�Ľӿ�
	/** Ҫ����Զ����SceneNodeAnimator�����沢ʹ�����Ա���������������ͼ��أ� �򵥵�ʵ������ӿڲ�
	ͨ��ISceneManager::registerSceneNodeAnimatorFactoryע�������㳡������ȥ��
	ע�⣺����ʵ�����Լ��ĳ����ڵ㹤��ʱ����Ҫ����ISceneNodeManager::grab()���ӳ����ڵ�����������ü�����
	��Ϊ��������ڵ������������ζ���grab()������������ѭ�����ý��ᵼ��c���������������Ľڵ��޷����ͷš�
	*/
	class ISceneNodeAnimatorFactory : public virtual IReferenceCounter
	{
	public:

		//! ������������ID����һ��SceneNodeAnimator
		/** \param type: SceneNodeAnimator������ID 
		\param target: ��animator��Ŀ�곡���ڵ�
		\return Returns ָ�������SceneNodeAnimator��ָ����߲��ɹ�ΪNULL */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target) = 0;

		//! ����һ��������������SceneNodeAnimator
		/** \param typeName:  SceneNodeAnimator��������
		\param target: ��animator��Ŀ�곡���ڵ�
		\return ָ�������SceneNodeAnimator��ָ����߲��ɹ�ΪNULL */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const c8* typeName, ISceneNode* target) = 0;

		//! ��������������Դ�����SceneNodeAnimator����������
		virtual UINT32 getCreatableSceneNodeAnimatorTypeCount() const = 0;

		//! ����һ���ɴ�����SceneNodeAnimator���͵�����
		/** \param idx: SceneNodeAnimator�������������������������0��getCreatableSceneNodeTypeCount()*/
		virtual ESCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(UINT32 idx) const = 0;

		//! ����һ���ɴ�����SceneNodeAnimator���͵�������
		/** \param idx: SceneNodeAnimator�������������������������0��getCreatableSceneNodeTypeCount() */
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(UINT32 idx) const = 0;

		//! ����һ���ɴ�����SceneNodeAnimator���͵�������
		/** \param type: SceneNodeAnimator������
		\return: �������SceneNodeAnimator�����������������������Դ����������͵Ļ������򷵻�0*/
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const = 0;
	};
}

#endif