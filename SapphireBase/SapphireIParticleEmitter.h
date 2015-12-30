#ifndef _SAPPHIRE_IPARTICLE_EMITTER_
#define _SAPPHIRE_IPARTICLE_EMITTER_


#include "SapphirePrerequisites.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireSParticle.h"

namespace Sapphire
{
	//! �������ӷ���������
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

	//! �������ӷ�����������
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

	//! ʹ������ϵͳ�����ӷ�����
	/** һ�����ӷ��������������ӵ�һ������ϵͳ��
	*/
	class IParticleEmitter : public virtual IAttributeExchangingObject
	{
	public:

		//! Ԥ����һ�����䵽ϵͳ��������Ⱥ������
		/** \param now ��ǰʱ��
		\param timeSinceLastCall ���ϴε������ŵ�ʱ�䣬�������
		\param outArray ���䵽ϵͳ��������Ⱥ�������ָ��
		\return ���������������������Ϊ0 */
		virtual SINT32 emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray) = 0;

		//! �������ӷ������������ӵķ���
		virtual void setDirection(const Vector3& newDirection) = 0;

		//! �������ӷ�����ÿ�뷢�����ӵ���С����
		virtual void setMinParticlesPerSecond(UINT32 minPPS) = 0;

		//! �������ӷ�����ÿ�뷢�����ӵ�������� 
		virtual void setMaxParticlesPerSecond(UINT32 maxPPS) = 0;

		//! ����������С��ʼ��ɫ
		virtual void setMinStartColor(const ColourValue& color) = 0;

		//! �����������ʼ��ɫ
		virtual void setMaxStartColor(const ColourValue& color) = 0;

		//! �����������ʼ��С
		virtual void setMaxStartSize(const dimension2df& size) = 0;

		//! ����������С��ʼ��С
		virtual void setMinStartSize(const dimension2df& size) = 0;

		//! �������Ӱ����������С��������
		virtual void setMinLifeTime(UINT32 lifeTimeMin) = 0;

		//! �������Ӱ�������������������
		virtual void setMaxLifeTime(UINT32 lifeTimeMax) = 0;

		//!	�������������ɷ���
		virtual void setMaxAngleDegrees(SINT32 maxAngleDegrees) = 0;

		//! ��ȡ���ӷ������������ӵķ���
		virtual const Vector3& getDirection() const = 0;

		//! ��ȡ���ӷ�����ÿ�뷢�����С����
		virtual UINT32 getMinParticlesPerSecond() const = 0;

		//! ��ȡ���ӷ�����ÿ�뷢����������
		virtual UINT32 getMaxParticlesPerSecond() const = 0;

		//! ��ȡ������С��ʼ��ɫ
		virtual const ColourValue& getMinStartColor() const = 0;

		//!��ȡ�������ʼ��ɫ
		virtual const ColourValue& getMaxStartColor() const = 0;

		//! ��ȡ�������ʼ��С
		virtual const dimension2df& getMaxStartSize() const = 0;

		//! ��ȡ������С��ʼ��С
		virtual const dimension2df& getMinStartSize() const = 0;

		//! ��ȡ���Ӱ����������С�������� 
		virtual UINT32 getMinLifeTime() const = 0;

		//! ��ȡ���Ӱ������������������� 
		virtual UINT32 getMaxLifeTime() const = 0;

		//! ��ȡ���������ɷ���
		virtual SINT32 getMaxAngleDegrees() const = 0;


		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_POINT; }
	};

	typedef IParticleEmitter IParticlePointEmitter;
}

#endif