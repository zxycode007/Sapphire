#ifndef _SAPPHIRE_IPARTICLE_AFFECTOR_
#define _SAPPHIRE_IPARTICLE_AFFECTOR_

#include "SapphirePrerequisites.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireSParticle.h"


namespace Sapphire
{
	//! 内置的粒子影响器类型
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

	//! 内置粒子影响器的名字
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

	//! 一个粒子效果器修改粒子
	class IParticleAffector : public virtual  IAttributeExchangingObject
	{
	public:

		 
		IParticleAffector() : Enabled(true) {}

		//! 影响一个粒子数组
		/** \param now 当前时间(个 ITimer::getTime()返回的一样)
		\param particlearray 粒子数组.
		\param count 粒子数组的数量*/
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count) = 0;

		//! 设置这个影响器当前是否打开
		virtual void setEnabled(bool enabled) { Enabled = enabled; }

		//! 获取这个影响去当前的打开状态
		virtual bool getEnabled() const { return Enabled; }

		//! 获取发射器类型
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const = 0;

	protected:
		bool Enabled;
	};
}

#endif