#ifndef _SAPPHIRE_I_INDEX_BUFFER_
#define _SAPPHIRE_I_INDEX_BUFFER_

#include "SapphireEHardwareBufferFlags.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireSVertexIndex.h"

namespace Sapphire
{
	class IIndexBuffer : public virtual IReferenceCounter
	{
	public:

		virtual void* getData() = 0;

		virtual E_INDEX_TYPE getType() const = 0;
		virtual void setType(E_INDEX_TYPE IndexType) = 0;

		virtual UINT32 stride() const = 0;

		virtual UINT32 size() const = 0;
		virtual void push_back(const UINT32 &element) = 0;
		virtual UINT32 operator [](UINT32 index) const = 0;
		virtual UINT32 getLast() = 0;
		virtual void setValue(UINT32 index, UINT32 value) = 0;
		virtual void set_used(UINT32 usedNow) = 0;
		virtual void reallocate(UINT32 new_size) = 0;
		virtual UINT32 allocated_size() const = 0;

		virtual void* pointer() = 0;

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