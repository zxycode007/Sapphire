#ifndef _SAPPHIRE_EMESH_WRITER_ENUMS
#define _SAPPHIRE_EMESH_WRITER_ENUMS

#include "SapphireDefines.h"

namespace Sapphire
{
	//! һ��������������д��������֧�ֵ���������ö��
	/** һ����������д�������ĸ��ַ�����,��'irrm',����'coll',���������֣��������ⲿ������д�������ֳ�ͻ*/
	enum EMESH_WRITER_TYPE
	{
		//! Irrlicht native mesh writer, ��̬�� .irrmesh
		EMWT_IRR_MESH = MAKE_IRR_ID('i', 'r', 'r', 'm'),

		//! COLLADA mesh writer .dae �� .xml�ļ�
		EMWT_COLLADA = MAKE_IRR_ID('c', 'o', 'l', 'l'),

		//! STL mesh writer   .stl �ļ�
		EMWT_STL = MAKE_IRR_ID('s', 't', 'l', 0),

		//! OBJ mesh writer   .obj �ļ�
		EMWT_OBJ = MAKE_IRR_ID('o', 'b', 'j', 0),

		//! PLY mesh writer   .ply  �ļ�
		EMWT_PLY = MAKE_IRR_ID('p', 'l', 'y', 0)
	};


	//! ����д������־���� 
	enum E_MESH_WRITER_FLAGS
	{
		//! no writer flags
		EMWF_NONE = 0,

		//! write lightmap textures out if possible
		EMWF_WRITE_LIGHTMAPS = 0x1,

		//! write in a way that consumes less disk space
		EMWF_WRITE_COMPRESSED = 0x2,

		//! write in binary format rather than text
		EMWF_WRITE_BINARY = 0x4
	};
}

#endif