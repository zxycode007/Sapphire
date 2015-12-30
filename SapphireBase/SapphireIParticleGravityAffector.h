#ifndef _SAPPHIRE_I_PARTICLE_GRAVITY_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_GRAVITY_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! һ��������Ӧ������������Ӱ����
	class IParticleGravityAffector : public IParticleAffector
	{
	public:

		//! ����ʧȥ������ʱ��
		/** ���Ǹ�ʱ�����Ӳ��ܽ����κ��ƶ� */
		virtual void setTimeForceLost(Real timeForceLost) = 0;

		//! ����ά�ռ�ʩ����������
		virtual void setGravity(const Vector3& gravity) = 0;

		//! ��ȡ������������ȫ��ʧ��ʱ��
		virtual Real getTimeForceLost() const = 0;

		//! ��ȡ�����ķ�������
		virtual const Vector3& getGravity() const = 0;

		//!��ȡ����������
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_GRAVITY; }
	};
}

#endif