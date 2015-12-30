#ifndef _SAPPHIRE_IPARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_
#define _SAPPHIRE_IPARTICLE_ANIMATED_MESH_SCENE_NODE_EMITTER_

#include "SapphirePrerequisites.h"
#include "SapphireIParticleEmitter.h"
#include "SapphireIAnimatedMeshSceneNode.h"


namespace Sapphire
{
	//! һ���������񶥵�����ӵ����ӷ�����
	class IParticleAnimatedMeshSceneNodeEmitter : public IParticleEmitter
	{
	public:

		//! ���÷������ӵ�����
		virtual void setAnimatedMeshSceneNode(IAnimatedMeshSceneNode* node) = 0;

		//! �����Ƿ�Ҫ�Է���ʹ�ö��㷨�ߣ����߷���ָ��
		virtual void setUseNormalDirection(bool useNormalDirection = true) = 0;

		//! ���ö���һ�����������ӷ��������ķ�������
		virtual void setNormalDirectionModifier(Real normalDirectionModifier) = 0;

		//! �����Ƿ��ÿһ����������ʰȡ��min��max�Ķ��㷢���min��max������
		virtual void setEveryMeshVertex(bool everyMeshVertex = true) = 0;

		//! ��ȡ��������ӵ�����
		virtual const IAnimatedMeshSceneNode* getAnimatedMeshSceneNode() const = 0;

		//! �Ƿ�ʹ�÷��߷���
		virtual bool isUsingNormalDirection() const = 0;

		//! ��ȡ����һ�����������ӷ��������ķ�������
		virtual Real getNormalDirectionModifier() const = 0;

		//! ��ȡ�Ƿ��ÿһ����������ʰȡ��min��max�Ķ��㷢���min��max������
		virtual bool getEveryMeshVertex() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_ANIMATED_MESH; }
	};
}

#endif