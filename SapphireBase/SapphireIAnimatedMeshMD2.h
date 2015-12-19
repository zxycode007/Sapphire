#ifndef _SAPPHIRE_I_ANIMATED_MESH_MD2_
#define _SAPPHIRE_I_ANIMATED_MESH_MD2_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireIAnimatedMesh.h"
#

namespace Sapphire
{
	//! MD2�Ķ�������
	enum EMD2_ANIMATION_TYPE
	{
		EMAT_STAND = 0,
		EMAT_RUN,
		EMAT_ATTACK,
		EMAT_PAIN_A,
		EMAT_PAIN_B,
		EMAT_PAIN_C,
		EMAT_JUMP,
		EMAT_FLIP,
		EMAT_SALUTE,
		EMAT_FALLBACK,
		EMAT_WAVE,
		EMAT_POINT,
		EMAT_CROUCH_STAND,
		EMAT_CROUCH_WALK,
		EMAT_CROUCH_ATTACK,
		EMAT_CROUCH_PAIN,
		EMAT_CROUCH_DEATH,
		EMAT_DEATH_FALLBACK,
		EMAT_DEATH_FALLFORWARD,
		EMAT_DEATH_FALLBACKSLOW,
		EMAT_BOOM,

		//! �ܵĶ�������
		EMAT_COUNT
	};

	//! ����ʹ��һЩMD2�������к����Ľӿ�
	class IAnimatedMeshMD2 : public IAnimatedMesh
	{
	public:

		//! ��ȡһ��Ĭ�ϵ�MD2�������͵�֡ѭ������
		/** \param l Ҫ��ȡ֡��������
		\param outBegin ��������ض��������͵���ʼ֡
		\param outEnd ��������ض��������͵Ľ���֡
		\param outFPS ÿ�⶯������ʱ��ÿ��֡��
		\return ����һ��Ĭ��MD2�������͵���ʼ֡������֡��ÿ��֡�� */
		virtual void getFrameLoop(EMD2_ANIMATION_TYPE l, SINT32& outBegin,
			SINT32& outEnd, SINT32& outFPS) const = 0;

		//! ͨ����������ȡһ���ض�MD2�������͵�֡ѭ������
		/** \param name ������
		\param outBegin ��������ض��������͵���ʼ֡
		\param outEnd ��������ض��������͵Ľ���֡
		\param outFPS ÿ�⶯������ʱ��ÿ��֡��
		\return ����һ��Ĭ��MD2�������͵���ʼ֡������֡��ÿ��֡�� */
		virtual bool getFrameLoop(const c8* name,
			SINT32& outBegin, SINT32& outEnd, SINT32& outFPS) const = 0;

		//! ��ȡ�ļ���MD2����������
		virtual SINT32 getAnimationCount() const = 0;

		//! ��ȡMD2����������
		/** \param nr: ����0�Ķ������� */
		virtual const c8* getAnimationName(SINT32 nr) const = 0;
	};
}



#endif 