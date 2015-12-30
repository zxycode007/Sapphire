#ifndef _SAPPHIRE_I_PARTICLE_ATTRACTION_AFFECTOR_
#define _SAPPHIRE_I_PARTICLE_ATTRACTION_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	//! 一个吸引或转移粒子的粒子影响器
	class IParticleAttractionAffector : public IParticleAffector
	{
	public:

		//! 设置将吸附粒子的点
		virtual void setPoint(const Vector3& point) = 0;

		//! 设置粒子是吸附还是转移
		virtual void setAttract(bool attract) = 0;

		//! 设置是否在x方向影响例子
		virtual void setAffectX(bool affect) = 0;

		//! 设置是否在Y方向影响例子
		virtual void setAffectY(bool affect) = 0;

		//! 设置是否在Z方向影响例子
		virtual void setAffectZ(bool affect) = 0;

		//! 获取粒子的吸附点
		virtual const Vector3& getPoint() const = 0;

		//!获取是否粒子吸附还是转移
		virtual bool getAttract() const = 0;

		//! 获取粒子x位置是否被影响
		virtual bool getAffectX() const = 0;

		//! 获取粒子y位置是否被影响
		virtual bool getAffectY() const = 0;

		//! 获取粒子z位置是否被影响
		virtual bool getAffectZ() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const { return EPAT_ATTRACT; }
	};
}

#endif