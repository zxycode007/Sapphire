#ifndef _SAPPHIRE_IPARTICLE_EMITTER_
#define _SAPPHIRE_IPARTICLE_EMITTER_


#include "SapphirePrerequisites.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireSParticle.h"

namespace Sapphire
{
	//! 内置粒子发射器类型
	enum E_PARTICLE_EMITTER_TYPE
	{
		EPET_POINT = 0,
		EPET_ANIMATED_MESH,
		EPET_BOX,
		EPET_CYLINDER,
		EPET_MESH,
		EPET_RING,
		EPET_SPHERE,
		EPET_COUNT
	};

	//! 内置粒子发射器类型名
	const c8* const ParticleEmitterTypeNames[] =
	{
		"Point",
		"AnimatedMesh",
		"Box",
		"Cylinder",
		"Mesh",
		"Ring",
		"Sphere",
		0
	};

	//! 使用粒子系统的粒子发射器
	/** 一个粒子发射器发射新粒子到一个粒子系统中
	*/
	class IParticleEmitter : public virtual IAttributeExchangingObject
	{
	public:

		//! 预处理一个发射到系统中新粒子群的数组
		/** \param now 当前时间
		\param timeSinceLastCall 自上次调用流逝的时间，毫秒计算
		\param outArray 发射到系统中新粒子群的数组的指针
		\return 新粒子数组的数量，可以为0 */
		virtual SINT32 emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray) = 0;

		//! 设置粒子发射器发射粒子的方向
		virtual void setDirection(const Vector3& newDirection) = 0;

		//! 设置粒子发射器每秒发射粒子的最小数量
		virtual void setMinParticlesPerSecond(UINT32 minPPS) = 0;

		//! 设置粒子发射器每秒发射粒子的最大数量 
		virtual void setMaxParticlesPerSecond(UINT32 maxPPS) = 0;

		//! 设置粒子最小开始颜色
		virtual void setMinStartColor(const ColourValue& color) = 0;

		//! 设置粒子最大开始颜色
		virtual void setMaxStartColor(const ColourValue& color) = 0;

		//! 设置粒子最大开始大小
		virtual void setMaxStartSize(const dimension2df& size) = 0;

		//! 设置粒子最小开始大小
		virtual void setMinStartSize(const dimension2df& size) = 0;

		//! 设置粒子按毫秒算的最小生命周期
		virtual void setMinLifeTime(UINT32 lifeTimeMin) = 0;

		//! 设置粒子按毫秒算的最大生命周期
		virtual void setMaxLifeTime(UINT32 lifeTimeMax) = 0;

		//!	设置最大随机生成方向
		virtual void setMaxAngleDegrees(SINT32 maxAngleDegrees) = 0;

		//! 获取粒子发射器发射粒子的方向
		virtual const Vector3& getDirection() const = 0;

		//! 获取粒子发射器每秒发射的最小数量
		virtual UINT32 getMinParticlesPerSecond() const = 0;

		//! 获取粒子发射器每秒发射的最大数量
		virtual UINT32 getMaxParticlesPerSecond() const = 0;

		//! 获取粒子最小开始颜色
		virtual const ColourValue& getMinStartColor() const = 0;

		//!获取粒子最大开始颜色
		virtual const ColourValue& getMaxStartColor() const = 0;

		//! 获取粒子最大开始大小
		virtual const dimension2df& getMaxStartSize() const = 0;

		//! 获取粒子最小开始大小
		virtual const dimension2df& getMinStartSize() const = 0;

		//! 获取粒子按毫秒算的最小生命周期 
		virtual UINT32 getMinLifeTime() const = 0;

		//! 获取粒子按毫秒算的最大生命周期 
		virtual UINT32 getMaxLifeTime() const = 0;

		//! 获取最大随机生成方向
		virtual SINT32 getMaxAngleDegrees() const = 0;


		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_POINT; }
	};

	typedef IParticleEmitter IParticlePointEmitter;
}

#endif