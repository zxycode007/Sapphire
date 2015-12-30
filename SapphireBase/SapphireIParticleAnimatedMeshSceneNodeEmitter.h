#ifndef _SAPPHIRE_IPARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_
#define _SAPPHIRE_IPARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_

#include "SapphirePrerequisites.h"
#include "SapphireIParticleEmitter.h"
#include "SapphireIAnimatedMeshSceneNode.h"


namespace Sapphire
{
	//! 一个发射网格顶点的粒子的粒子发射器
	class IParticleAnimatedMeshSceneNodeEmitter : public IParticleEmitter
	{
	public:

		//! 设置发射粒子的网格
		virtual void setAnimatedMeshSceneNode(IAnimatedMeshSceneNode* node) = 0;

		//! 设置是否要对方向使用顶点法线，或者方向指的
		virtual void setUseNormalDirection(bool useNormalDirection = true) = 0;

		//! 设置对于一个给定的粒子方向分离出的法线数量
		virtual void setNormalDirectionModifier(Real normalDirectionModifier) = 0;

		//! 设置是否对每一个顶点或或者拾取从min到max的顶点发射从min到max的粒子
		virtual void setEveryMeshVertex(bool everyMeshVertex = true) = 0;

		//! 获取发射的粒子的网格
		virtual const IAnimatedMeshSceneNode* getAnimatedMeshSceneNode() const = 0;

		//! 是否使用法线方向
		virtual bool isUsingNormalDirection() const = 0;

		//! 获取对于一个给定的粒子方向分离出的法线数量
		virtual Real getNormalDirectionModifier() const = 0;

		//! 获取是否对每一个顶点或或者拾取从min到max的顶点发射从min到max的粒子
		virtual bool getEveryMeshVertex() const = 0;

		//! 获取发射器类型
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_ANIMATED_MESH; }
	};
}

#endif