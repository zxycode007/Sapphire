#ifndef _SAPPHIRE_EHARDWAARE_BUFFER_FLAGS_
#define _SAPPHIRE_EHARDWAARE_BUFFER_FLAGS_

namespace Sapphire
{
	enum E_HARDWARE_MAPPING
	{
		//! ��������Ӳ����
		EHM_NEVER = 0,

		//! ���ٸı䣬���������ı�����Ӳ����
		EHM_STATIC,

		//! ��ʱ�ı䣬�����Ż�����
		EHM_DYNAMIC,

		//! �����ı䣬��GPU���Ż�����
		EHM_STREAM
	};

	enum E_BUFFER_TYPE
	{
		//!���ı��κζ���
		EBT_NONE = 0,
		//! �ı䶥��ӳ��
		EBT_VERTEX,
		//! �ı�����ӳ��
		EBT_INDEX,
		//! ͬʱ�ı䶥�������ӳ�䵽ͬһ��ֵ��
		EBT_VERTEX_AND_INDEX
	};
}

#endif 