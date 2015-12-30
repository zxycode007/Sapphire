#ifndef _SAPPHIRE_I_PARTICLE_FADEOUT_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_FADEOUT_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! 一个粒子逐渐消失的效果器
	class IParticleFadeOutAffector : public IParticleAffector
	{
	public:

		//! 设置目标颜色，这个粒子颜色随着时间进行插值
		virtual void setTargetColor(const ColourValue& targetColor) = 0;

		//! 设置对于每个粒子消失的时间的毫秒数
		virtual void setFadeOutTime(UINT32 fadeOutTime) = 0;

		//! 获取目标颜色，这个粒子颜色随世界进行插值
		virtual const ColourValue& getTargetColor() const = 0;

		//! 设置对于每个粒子消失的时间的毫秒数
		virtual UINT32 getFadeOutTime() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_FADE_OUT; }
	};
}

#endif