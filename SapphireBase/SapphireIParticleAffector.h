#ifndef _SAPPHIRE_IPARTICLE_AFFECTOR_
#define _SAPPHIRE_IPARTICLE_AFFECTOR_

#include "SapphirePrerequisites.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireSParticle.h"


namespace Sapphire
{
	//! ���õ�����Ӱ��������
	enum E_PARTICLE_AFFECTOR_TYPE
	{
		EPAT_NONE = 0,
		EPAT_ATTRACT,
		EPAT_FADE_OUT,
		EPAT_GRAVITY,
		EPAT_ROTATE,
		EPAT_SCALE,
		EPAT_COUNT
	};

	//! ��������Ӱ����������
	const c8* const ParticleAffectorTypeNames[] =
	{
		"None",
		"Attract",
		"FadeOut",
		"Gravity",
		"Rotate",
		"Scale",
		0
	};

	//! һ������Ч�����޸�����
	class IParticleAffector : public virtual  IAttributeExchangingObject
	{
	public:

		 
		IParticleAffector() : Enabled(true) {}

		//! Ӱ��һ����������
		/** \param now ��ǰʱ��(�� ITimer::getTime()���ص�һ��)
		\param particlearray ��������.
		\param count �������������*/
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count) = 0;

		//! �������Ӱ������ǰ�Ƿ��
		virtual void setEnabled(bool enabled) { Enabled = enabled; }

		//! ��ȡ���Ӱ��ȥ��ǰ�Ĵ�״̬
		virtual bool getEnabled() const { return Enabled; }

		//! ��ȡ����������
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const = 0;

	protected:
		bool Enabled;
	};
}

#endif