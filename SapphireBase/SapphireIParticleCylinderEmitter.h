#ifndef _SAPPHIRE_I_PARTICLE_CYLINDER_EMITTER_
#define _SAPPHIRE_I_PARTICLE_CYLINDER_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! 一个圆柱形空间的的粒子发射器
	class IParticleCylinderEmitter : public IParticleEmitter
	{
	public:

		//! 设置圆柱一端的中心半径
		virtual void setCenter(const Vector3& center) = 0;

		//! 设置圆柱的法线
		virtual void setNormal(const Vector3& normal) = 0;

		//! 设置圆柱的半径
		virtual void setRadius(Real radius) = 0;

		//! 设置圆柱的长度
		virtual void setLength(Real length) = 0;

		//! 是在是否在圆柱内部绘制点
		virtual void setOutlineOnly(bool outlineOnly = true) = 0;

		//! 获取当前圆柱的中心
		virtual const Vector3& getCenter() const = 0;

		//! 获取圆柱的法线
		virtual const Vector3& getNormal() const = 0;

		//! 获取圆柱的半径
		virtual FLOAT32 getRadius() const = 0;

		//! 获取圆柱的中心
		virtual FLOAT32 getLength() const = 0;

		//! 获取是否在圆柱内部绘制点
		virtual bool getOutlineOnly() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_CYLINDER; }
	};
}

#endif