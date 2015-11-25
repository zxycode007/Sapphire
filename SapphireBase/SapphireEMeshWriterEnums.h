#ifndef _SAPPHIRE_EMESH_WRITER_ENUMS
#define _SAPPHIRE_EMESH_WRITER_ENUMS

#include "SapphireDefines.h"

namespace Sapphire
{
	//! 一个内置网格类型写入器所能支持的所有类型枚举
	/** 一个场景网格写入器由四个字符构成,想'irrm',或者'coll',带来简单数字，避免与外部的网格写入器名字冲突*/
	enum EMESH_WRITER_TYPE
	{
		//! Irrlicht native mesh writer, 静态的 .irrmesh
		EMWT_IRR_MESH = MAKE_IRR_ID('i', 'r', 'r', 'm'),

		//! COLLADA mesh writer .dae 和 .xml文件
		EMWT_COLLADA = MAKE_IRR_ID('c', 'o', 'l', 'l'),

		//! STL mesh writer   .stl 文件
		EMWT_STL = MAKE_IRR_ID('s', 't', 'l', 0),

		//! OBJ mesh writer   .obj 文件
		EMWT_OBJ = MAKE_IRR_ID('o', 'b', 'j', 0),

		//! PLY mesh writer   .ply  文件
		EMWT_PLY = MAKE_IRR_ID('p', 'l', 'y', 0)
	};


	//! 网格写入器标志配置 
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