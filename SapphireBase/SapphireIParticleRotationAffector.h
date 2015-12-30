#ifndef _SAPPHIRE_I_PARTICLE_ROTATION_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_ROTATION_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! 一个旋转粒子系统的粒子影响器
	class IParticleRotationAffector : public IParticleAffector
	{
	public:

		//! 设置环绕旋转粒子的点
		virtual void setPivotPoint(const Vector3& point) = 0;

		//! 设置在三个轴每秒角速度
		virtual void setSpeed(const Vector3& speed) = 0;

		//! 获取粒子吸附点
		virtual const Vector3& getPivotPoint() const = 0;

		//! 获取在三个轴每秒角速度
		virtual const Vector3& getSpeed() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_ROTATE; }
	};
}

#endif