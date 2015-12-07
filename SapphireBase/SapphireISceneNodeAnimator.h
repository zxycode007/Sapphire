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
	//! ����һ�������ڵ㡣 �ܹ��λ�ã���ת�����ʣ�������
	/** һ�������ڵ�animator�ܹ�ʹһ�������ڵ�ǳ��򵥵ķ�ʽ����������ܹ��ı�����λ�ã���ת�����źͲ��ʡ��кܶ�animator�ɹ�ѡ��
	�������ISceneManager�ӿڴ��������ڵ�animator
	*/
	class ISceneNodeAnimator : public SceneAlloc, public IAttributeExchangingObject, public IEventReceiver, public IReferenceCounter
	{
	public:
		//! ����һ�������ڵ�
		/** \param node Ҫ����Ľڵ�
		\param timeMs ��ǰʱ��ĺ����� */
		virtual void animateNode(ISceneNode* node, UINT32 timeMs) = 0;

		//! ������ǰanimator��һ����¡
		/** ��ע������Ҫdrop����(IReferenceCounter::drop()) */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node,
			ISceneManager* newManager = 0) = 0;

		//! ������animator�����¼�������true
		/** ��������һ���������������animator��������Ӧ�������ͼ��̵��¼�*/
		virtual bool isEventReceiverEnabled() const
		{
			return false;
		}

		//! ÿһ����������Ϊ������Ƶ�animator�����������
		virtual bool OnEvent(const SEvent& event)
		{
			return false;
		}

		//! ������������ڵ�animator������
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_UNKNOWN;
		}

		//! �������animator�Ƿ����н���
		/** ��ֻ����һ���������״̬�ķ�ѭ��ʽanimator��Ч
		\return true ���animator����������true�����false������Ȼ����*/
		virtual bool hasFinished(void) const
		{
			return false;
		}
	};

	typedef SharedPtr<ISceneNodeAnimator> ISceneNodeAnimatorPtr;
}

#endif