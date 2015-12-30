#ifndef _SAPPHIRE_I_PARTICLE_BOX_EMITTER_
#define _SAPPHIRE_I_PARTICLE_BOX_EMITTER_

#include "SapphireIParticleEmitter.h"
#include "SapphireAxisAlignedBox.h"

namespace Sapphire
{
	//! 一个从一个盒子形状空间发射粒子的粒子发射器
	class IParticleBoxEmitter : public IParticleEmitter
	{
	public:

		//! 设置盒子形状
		virtual void setBox(const AxisAlignedBox& box) = 0;

		//! 获取盒子形状设置
		virtual const AxisAlignedBox& getBox() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_BOX; }
	};
}

#endif