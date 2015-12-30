#ifndef _SAPPHIRE_I_PARTICLE_ATTRACTION_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_ATTRACTION_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! һ��������ת�����ӵ�����Ӱ����
	class IParticleAttractionAffector : public IParticleAffector
	{
	public:

		//! ���ý��������ӵĵ�
		virtual void setPoint(const Vector3& point) = 0;

		//! ������������������ת��
		virtual void setAttract(bool attract) = 0;

		//! �����Ƿ���x����Ӱ������
		virtual void setAffectX(bool affect) = 0;

		//! �����Ƿ���Y����Ӱ������
		virtual void setAffectY(bool affect) = 0;

		//! �����Ƿ���Z����Ӱ������
		virtual void setAffectZ(bool affect) = 0;

		//! ��ȡ���ӵ�������
		virtual const Vector3& getPoint() const = 0;

		//!��ȡ�Ƿ�������������ת��
		virtual bool getAttract() const = 0;

		//! ��ȡ����xλ���Ƿ�Ӱ��
		virtual bool getAffectX() const = 0;

		//! ��ȡ����yλ���Ƿ�Ӱ��
		virtual bool getAffectY() const = 0;

		//! ��ȡ����zλ���Ƿ�Ӱ��
		virtual bool getAffectZ() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_ATTRACT; }
	};
}

#endif