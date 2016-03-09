#ifndef _SAPPHIRE_I_PARTICLE_RING_EMITTER_
#define _SAPPHIRE_I_PARTICLE_RING_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! һ�����Ż������������ӵ����ӷ�����
	class IParticleRingEmitter : public IParticleEmitter
	{
	public:

		//! ���û�������
		virtual void setCenter(const Vector3& center) = 0;

		//! ���û��ΰ뾶
		virtual void setRadius(Real radius) = 0;

		//! ���û��κ��
		virtual void setRingThickness(Real ringThickness) = 0;

		//! ���û�������
		virtual const Vector3& getCenter() const = 0;

		//! ��ȡ���ΰ뾶
		virtual FLOAT32 getRadius() const = 0;

		//! ��ȡ���κ��
		virtual FLOAT32 getRingThickness() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_RING; }
	};
}

#endif