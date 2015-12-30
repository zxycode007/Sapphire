#ifndef _SAPPHIRE_I_PARTICLE_BOX_EMITTER_
#define _SAPPHIRE_I_PARTICLE_BOX_EMITTER_

#include "SapphireIParticleEmitter.h"
#include "SapphireAxisAlignedBox.h"

namespace Sapphire
{
	//! һ����һ��������״�ռ䷢�����ӵ����ӷ�����
	class IParticleBoxEmitter : public IParticleEmitter
	{
	public:

		//! ���ú�����״
		virtual void setBox(const AxisAlignedBox& box) = 0;

		//! ��ȡ������״����
		virtual const AxisAlignedBox& getBox() const = 0;

		//! ��ȡ����������
		virtual E_PARTICLE_EMITTER_TYPE getType() const { return EPET_BOX; }
	};
}

#endif