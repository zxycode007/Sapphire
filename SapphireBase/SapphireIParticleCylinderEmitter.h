#ifndef _SAPPHIRE_I_PARTICLE_CYLINDER_EMITTER_
#define _SAPPHIRE_I_PARTICLE_CYLINDER_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! һ��Բ���οռ�ĵ����ӷ�����
	class IParticleCylinderEmitter : public IParticleEmitter
	{
	public:

		//! ����Բ��һ�˵����İ뾶
		virtual void setCenter(const Vector3& center) = 0;

		//! ����Բ���ķ���
		virtual void setNormal(const Vector3& normal) = 0;

		//! ����Բ���İ뾶
		virtual void setRadius(Real radius) = 0;

		//! ����Բ���ĳ���
		virtual void setLength(Real length) = 0;

		//! �����Ƿ���Բ���ڲ����Ƶ�
		virtual void setOutlineOnly(bool outlineOnly = true) = 0;

		//! ��ȡ��ǰԲ��������
		virtual const Vector3& getCenter() const = 0;

		//! ��ȡԲ���ķ���
		virtual const Vector3& getNormal() const = 0;

		//! ��ȡԲ���İ뾶
		virtual FLOAT32 getRadius() const = 0;

		//! ��ȡԲ��������
		virtual FLOAT32 getLength() const = 0;

		//! ��ȡ�Ƿ���Բ���ڲ����Ƶ�
		virtual bool getOutlineOnly() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_CYLINDER; }
	};
}

#endif