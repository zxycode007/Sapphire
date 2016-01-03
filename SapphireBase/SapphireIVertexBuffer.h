#ifndef _SAPPHIRE_I_VERTEX_BUFFER_
#define _SAPPHIRE_I_VERTEX_BUFFER_

#include "SapphireEHardwareBufferFlags.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireSVertex.h"


namespace Sapphire
{
	class IVertexBuffer : public virtual IReferenceCounter
	{
	public:
		virtual void* getData() = 0;
		virtual E_VERTEX_TYPE getType() const = 0;
		virtual void setType(E_VERTEX_TYPE vertexType) = 0;
		virtual UINT32 stride() const = 0;
		virtual UINT32 size() const = 0;
		virtual void push_back(const S3DVertex &element) = 0;
		virtual S3DVertex& operator [](const UINT32 index) const = 0;
		virtual S3DVertex& getLast() = 0;
		virtual void set_used(UINT32 usedNow) = 0;
		virtual void reallocate(UINT32 new_size) = 0;
		virtual UINT32 allocated_size() const = 0;
		virtual S3DVertex* pointer() = 0;

		//! ��ȡ��ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint() const = 0;

		//! Ϊ�������õ�ǰӲ��ӳ����ʾ
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint) = 0;

		//! ���������ı��־������Ӳ��������
		virtual void setDirty() = 0;

		//! ��ȡ�ı��ʶ��ID
		/** VideoDriver�ڲ�ʹ��*/
		virtual UINT32 getChangedID() const = 0;
	};
}

#endif