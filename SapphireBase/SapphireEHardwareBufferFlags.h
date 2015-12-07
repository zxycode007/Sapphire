#ifndef _SAPPHIRE_EHARDWAARE_BUFFER_FLAGS_
#define _SAPPHIRE_EHARDWAARE_BUFFER_FLAGS_

namespace Sapphire
{
	enum E_HARDWARE_MAPPING
	{
		//! 不保存在硬件中
		EHM_NEVER = 0,

		//! 很少改变，经常完整的保存在硬件中
		EHM_STATIC,

		//! 有时改变，驱动优化配置
		EHM_DYNAMIC,

		//! 经常改变，在GPU中优化缓冲
		EHM_STREAM
	};

	enum E_BUFFER_TYPE
	{
		//!不改变任何东西
		EBT_NONE = 0,
		//! 改变顶点映射
		EBT_VERTEX,
		//! 改变所有映射
		EBT_INDEX,
		//! 同时改变顶点和索引映射到同一个值中
		EBT_VERTEX_AND_INDEX
	};
}

#endif 