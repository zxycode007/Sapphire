#ifndef _SAPPHIRE_I_PARTICLE_SPHRERE_EMITTER_
#define _SAPPHIRE_I_PARTICLE_SPHRERE_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! 一个从球体空间发射粒子的粒子发射器
	class IParticleSphereEmitter : public IParticleEmitter
	{
	public:

		//! 设置粒子发射球的中心
		virtual void setCenter(const Vector3& center) = 0;

		//! 设置粒子反射球的半径
		virtual void setRadius(Real radius) = 0;

		//! 获取粒子发射求的中心
		virtual const Vector3& getCenter() const = 0;

		//! 获取粒子发射球的半径
		virtual Real getRadius() const = 0;

		//! 获取粒子发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_SPHERE; }
	};
}

#endif