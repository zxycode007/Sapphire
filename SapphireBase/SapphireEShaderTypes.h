#ifndef _SAPPHIRE_ESHADER_TYPES_
#define _SAPPHIRE_ESHADER_TYPES_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	//!  addHighLevelShaderMaterial()的编译目标枚举
	enum E_VERTEX_SHADER_TYPE
	{
		EVST_VS_1_1 = 0,
		EVST_VS_2_0,
		EVST_VS_2_a,
		EVST_VS_3_0,
		EVST_VS_4_0,
		EVST_VS_4_1,
		EVST_VS_5_0,

		 
		EVST_COUNT
	};

	//! 所有vertex shader types的名字,每个入口对应一个E_VERTEX_SHADER_TYPE的入口
	const c8* const VERTEX_SHADER_TYPE_NAMES[] = {
		"vs_1_1",
		"vs_2_0",
		"vs_2_a",
		"vs_3_0",
		"vs_4_0",
		"vs_4_1",
		"vs_5_0",
		0 };

	//! addHighLevelShaderMaterial()的编译目标枚举
	enum E_PIXEL_SHADER_TYPE
	{
		EPST_PS_1_1 = 0,
		EPST_PS_1_2,
		EPST_PS_1_3,
		EPST_PS_1_4,
		EPST_PS_2_0,
		EPST_PS_2_a,
		EPST_PS_2_b,
		EPST_PS_3_0,
		EPST_PS_4_0,
		EPST_PS_4_1,
		EPST_PS_5_0,

		 
		EPST_COUNT
	};

	//所有pixel shader types的名字, 每个入口对应一个E_PIXEL_SHADER_TYPE的入口
	const c8* const PIXEL_SHADER_TYPE_NAMES[] = {
		"ps_1_1",
		"ps_1_2",
		"ps_1_3",
		"ps_1_4",
		"ps_2_0",
		"ps_2_a",
		"ps_2_b",
		"ps_3_0",
		"ps_4_0",
		"ps_4_1",
		"ps_5_0",
		0 };

	//! 支持的eometry shader types类型枚举
	enum E_GEOMETRY_SHADER_TYPE
	{
		EGST_GS_4_0 = 0,

		 
		E_GST_COUNT
	};

	//! 支持的geometry shader types名
	const c8* const GEOMETRY_SHADER_TYPE_NAMES[] = {
		"gs_4_0",
		0 };
}

#endif