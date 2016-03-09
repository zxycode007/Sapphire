#ifndef _SAPPHIRE_I_PARTICLE_RING_EMITTER_
#define _SAPPHIRE_I_PARTICLE_RING_EMITTER_

#include "SapphireIParticleEmitter.h"

namespace Sapphire
{
	//! 一个沿着环形区域发射粒子的粒子发射器
	class IParticleRingEmitter : public IParticleEmitter
	{
	public:

		//! 设置环形中心
		virtual void setCenter(const Vector3& center) = 0;

		//! 设置环形半径
		virtual void setRadius(Real radius) = 0;

		//! 设置环形厚度
		virtual void setRingThickness(Real ringThickness) = 0;

		//! 设置环形中心
		virtual const Vector3& getCenter() const = 0;

		//! 获取环形半径
		virtual FLOAT32 getRadius() const = 0;

		//! 获取环形厚度
		virtual FLOAT32 getRingThickness() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_RING; }
	};
}

#endif