#ifndef _SAPPHIRE_I_PARTICLE_GRAVITY_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_GRAVITY_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! 一个对粒子应用重力的粒子影响器
	class IParticleGravityAffector : public IParticleAffector
	{
	public:

		//! 设置失去重力的时间
		/** 在那个时候粒子不能进行任何移动 */
		virtual void setTimeForceLost(Real timeForceLost) = 0;

		//! 在三维空间施加重力向量
		virtual void setGravity(const Vector3& gravity) = 0;

		//! 获取发射器力量完全消失的时间
		virtual Real getTimeForceLost() const = 0;

		//! 获取重力的方向向量
		virtual const Vector3& getGravity() const = 0;

		//!获取发射器类型
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_GRAVITY; }
	};
}

#endif