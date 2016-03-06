#ifndef _SAPPHIRE_I_PARTICLE_MESH_EMITTER_
#define _SAPPHIRE_I_PARTICLE_MESH_EMITTER_

#include "SapphireIParticleEmitter.h"
#include "SapphireIMesh.h"

namespace Sapphire
{
	//! 一个从一个网格顶点发射的例子发射器
	class IParticleMeshEmitter : public IParticleEmitter
	{
	public:

		//! 设置发射粒子的网格
		virtual void setMesh(IMesh* mesh) = 0;

		//! 设置是否使用顶点法线做方向，或指定方向
		virtual void setUseNormalDirection(bool useNormalDirection = true) = 0;

		//! 设置对粒子方向派生的法线方向数量
		virtual void setNormalDirectionModifier(Real normalDirectionModifier) = 0;

		//! 设置是否对每个网格顶点发射粒子
		virtual void setEveryMeshVertex(bool everyMeshVertex = true) = 0;

		//! 获取是否从网格发射粒子
		virtual const IMesh* getMesh() const = 0;

		//!获取是否使用顶点法线做方向，或指定方向
		virtual bool isUsingNormalDirection() const = 0;

		//! 获取对粒子方向派生的法线方向数量
		virtual FLOAT32 getNormalDirectionModifier() const = 0;

		//! 获取是否对每个网格顶点发射粒子
		virtual bool getEveryMeshVertex() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_MESH; }
	};
}

#endif