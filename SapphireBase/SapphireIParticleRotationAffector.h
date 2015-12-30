#ifndef _SAPPHIRE_I_PARTICLE_ROTATION_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_ROTATION_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! һ����ת����ϵͳ������Ӱ����
	class IParticleRotationAffector : public IParticleAffector
	{
	public:

		//! ���û�����ת���ӵĵ�
		virtual void setPivotPoint(const Vector3& point) = 0;

		//! ������������ÿ����ٶ�
		virtual void setSpeed(const Vector3& speed) = 0;

		//! ��ȡ����������
		virtual const Vector3& getPivotPoint() const = 0;

		//! ��ȡ��������ÿ����ٶ�
		virtual const Vector3& getSpeed() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_ROTATE; }
	};
}

#endif