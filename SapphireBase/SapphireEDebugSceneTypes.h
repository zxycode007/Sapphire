#ifndef _SAPPHIRE_EDEBUG_SCENE_TYPES_
#define _SAPPHIRE_EDEBUG_SCENE_TYPES_

namespace Sapphire
{
	//! һ�������������ó����ڵ�debug�������͵�ö��
	enum E_DEBUG_SCENE_TYPE
	{
		//! û��debug���� ( Default )
		EDS_OFF = 0,

		//! Ϊ�����ڵ�չʾһ����Χ�����
		EDS_BBOX = 1,

		//!��ʾ���㷨��
		EDS_NORMALS = 2,

		//! ��ʾ������ǩ
		EDS_SKELETON = 4,

		//! ���������߿�
		EDS_MESH_WIRE_OVERLAY = 8,

		//! ��ʱʹ��͸������
		EDS_HALF_TRANSPARENCY = 16,

		//! չʾ�������񻺳����İ�Χ����
		EDS_BBOX_BUFFERS = 32,

		//! EDS_BBOX | EDS_BBOX_BUFFERS
		EDS_BBOX_ALL = EDS_BBOX | EDS_BBOX_BUFFERS,

		//! ��ʾ����debug��Ϣ
		EDS_FULL = 0xffffffff
	};
}

#endif