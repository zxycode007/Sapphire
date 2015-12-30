#ifndef _SAPPHIRE_I_PARTICLE_FADEOUT_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_FADEOUT_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! һ����������ʧ��Ч����
	class IParticleFadeOutAffector : public IParticleAffector
	{
	public:

		//! ����Ŀ����ɫ�����������ɫ����ʱ����в�ֵ
		virtual void setTargetColor(const ColourValue& targetColor) = 0;

		//! ���ö���ÿ��������ʧ��ʱ��ĺ�����
		virtual void setFadeOutTime(UINT32 fadeOutTime) = 0;

		//! ��ȡĿ����ɫ�����������ɫ��������в�ֵ
		virtual const ColourValue& getTargetColor() const = 0;

		//! ���ö���ÿ��������ʧ��ʱ��ĺ�����
		virtual UINT32 getFadeOutTime() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_FADE_OUT; }
	};
}

#endif