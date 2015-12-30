#ifndef _SAPPHIRE_I_PARTICLE_SPHRERE_EMITTER_
#define _SAPPHIRE_I_PARTICLE_SPHRERE_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! һ��������ռ䷢�����ӵ����ӷ�����
	class IParticleSphereEmitter : public IParticleEmitter
	{
	public:

		//! �������ӷ����������
		virtual void setCenter(const Vector3& center) = 0;

		//! �������ӷ�����İ뾶
		virtual void setRadius(Real radius) = 0;

		//! ��ȡ���ӷ����������
		virtual const Vector3& getCenter() const = 0;

		//! ��ȡ���ӷ�����İ뾶
		virtual Real getRadius() const = 0;

		//! ��ȡ���ӷ���������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_SPHERE; }
	};
}

#endif