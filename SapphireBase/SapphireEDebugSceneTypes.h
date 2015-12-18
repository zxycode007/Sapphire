#ifndef _SAPPHIRE_EDEBUG_SCENE_TYPES_
#define _SAPPHIRE_EDEBUG_SCENE_TYPES_

namespace Sapphire
{
	//! 一个对于所有内置场景节点debug数据类型的枚举
	enum E_DEBUG_SCENE_TYPE
	{
		//! 没有debug数据 ( Default )
		EDS_OFF = 0,

		//! 为场景节点展示一个包围体盒子
		EDS_BBOX = 1,

		//!显示顶点法线
		EDS_NORMALS = 2,

		//! 显示骨骼标签
		EDS_SKELETON = 4,

		//! 覆盖网格线框
		EDS_MESH_WIRE_OVERLAY = 8,

		//! 临时使用透明材质
		EDS_HALF_TRANSPARENCY = 16,

		//! 展示所有网格缓冲区的包围盒子
		EDS_BBOX_BUFFERS = 32,

		//! EDS_BBOX | EDS_BBOX_BUFFERS
		EDS_BBOX_ALL = EDS_BBOX | EDS_BBOX_BUFFERS,

		//! 显示所有debug信息
		EDS_FULL = 0xffffffff
	};
}

#endif