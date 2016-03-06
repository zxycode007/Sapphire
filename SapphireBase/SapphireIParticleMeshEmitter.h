#ifndef _SAPPHIRE_I_PARTICLE_MESH_EMITTER_
#define _SAPPHIRE_I_PARTICLE_MESH_EMITTER_

#include "SapphireIParticleEmitter.h"
#include "SapphireIMesh.h"

namespace Sapphire
{
	//! һ����һ�����񶥵㷢������ӷ�����
	class IParticleMeshEmitter : public IParticleEmitter
	{
	public:

		//! ���÷������ӵ�����
		virtual void setMesh(IMesh* mesh) = 0;

		//! �����Ƿ�ʹ�ö��㷨�������򣬻�ָ������
		virtual void setUseNormalDirection(bool useNormalDirection = true) = 0;

		//! ���ö����ӷ��������ķ��߷�������
		virtual void setNormalDirectionModifier(Real normalDirectionModifier) = 0;

		//! �����Ƿ��ÿ�����񶥵㷢������
		virtual void setEveryMeshVertex(bool everyMeshVertex = true) = 0;

		//! ��ȡ�Ƿ������������
		virtual const IMesh* getMesh() const = 0;

		//!��ȡ�Ƿ�ʹ�ö��㷨�������򣬻�ָ������
		virtual bool isUsingNormalDirection() const = 0;

		//! ��ȡ�����ӷ��������ķ��߷�������
		virtual FLOAT32 getNormalDirectionModifier() const = 0;

		//! ��ȡ�Ƿ��ÿ�����񶥵㷢������
		virtual bool getEveryMeshVertex() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_MESH; }
	};
}

#endif