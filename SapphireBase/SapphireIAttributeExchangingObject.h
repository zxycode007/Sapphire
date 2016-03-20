#ifndef _SAPPHIRE_IATTRIBUTE_EXCHANGING_OBJECT_
#define _SAPPHIRE_IATTRIBUTE_EXCHANGING_OBJECT_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IAttributes;

	//! ͨ��SAttributeReadWriteOptions���ݸ�IAttributeExchangingObject�����ö�ٱ�־
	enum E_ATTRIBUTE_READ_WRITE_FLAGS
	{
		//! ��XML�ļ����л�/�����л�
		EARWF_FOR_FILE = 0x00000001,

		//! ��һ���༭�����Ժ����ļ����л�/�����л� 
		EARWF_FOR_EDITOR = 0x00000002,

		//! ��д���ļ���ʹ����ʵ·��
		EARWF_USE_RELATIVE_PATHS = 0x00000004
	};

	//! ��������������Ľ��
	struct SAttributeReadWriteOptions
	{
		 
		SAttributeReadWriteOptions()
			: Flags(0), Filename(0)
		{
		}

		//! E_ATTRIBUTE_READ_WRITE_FLAGS����������ϱ�־���Զ���һ��
		SINT32 Flags;

		//! ��ѡ�ļ���
		 String Filename;
	};

	//! һ��Ҫ���л��ͷ����л��������Ե�һ�����Զ���Ķ���
	class IAttributeExchangingObject : virtual public IReferenceCounter
	{
	public:

		//! д����������
		/** ʵ�������չʾ��ĳ����ڵ㶯�������ڽű����ԣ��༭����debugger��xml�����Ե����л�*/
		virtual void serializeAttributes( IAttributes* out, SAttributeReadWriteOptions* options = 0) const {}

		//! ��ȡ���������
		/** ʵ������������㳡���ڵ㶯�������ڽű����ԣ��༭����debugger��xml�����Է����л� */
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0) {}

	};
}

#endif
