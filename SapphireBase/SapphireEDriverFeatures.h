#ifndef _SAPPHIRE_EDRIVER_FEATURES_
#define _SAPPHIRE_EDRIVER_FEATURES_

namespace Sapphire
{
	//! 显示驱动特效的支持查询枚举
	enum E_VIDEO_DRIVER_FEATURE
	{
		//! 驱动能够渲染到一个表面
		EVDF_RENDER_TO_TARGET = 0,

		//! 硬件级T&L
		EVDF_HARDWARE_TL,

		//! 每个材质有多重纹理
		EVDF_MULTITEXTURE,

		//! 双线性滤波
		EVDF_BILINEAR_FILTER,

		//! MIPMAP
		EVDF_MIP_MAP,

		//! 自动更新mipmap
		EVDF_MIP_MAP_AUTO_UPDATE,

		//! 支持模板缓冲区
		EVDF_STENCIL_BUFFER,

		//! Is Vertex Shader 1.1 supported?
		EVDF_VERTEX_SHADER_1_1,

		//! Is Vertex Shader 2.0 supported?
		EVDF_VERTEX_SHADER_2_0,

		//! Is Vertex Shader 3.0 supported?
		EVDF_VERTEX_SHADER_3_0,

		//! Is Pixel Shader 1.1 supported?
		EVDF_PIXEL_SHADER_1_1,

		//! Is Pixel Shader 1.2 supported?
		EVDF_PIXEL_SHADER_1_2,

		//! Is Pixel Shader 1.3 supported?
		EVDF_PIXEL_SHADER_1_3,

		//! Is Pixel Shader 1.4 supported?
		EVDF_PIXEL_SHADER_1_4,

		//! Is Pixel Shader 2.0 supported?
		EVDF_PIXEL_SHADER_2_0,

		//! Is Pixel Shader 3.0 supported?
		EVDF_PIXEL_SHADER_3_0,

		//!ARB顶点编程
		EVDF_ARB_VERTEX_PROGRAM_1,

		//! ARB片段编程
		EVDF_ARB_FRAGMENT_PROGRAM_1,

		//!glsl
		EVDF_ARB_GLSL,

		//!hlsl
		EVDF_HLSL,

		//! 非正方形纹理支持
		EVDF_TEXTURE_NSQUARE,

		//!非2指数幂纹理支持
		EVDF_TEXTURE_NPOT,

		//! 帧缓冲区FBO
		EVDF_FRAMEBUFFER_OBJECT,

		// 顶点缓冲区VBO
		EVDF_VERTEX_BUFFER_OBJECT,

		//! 支持Alpha覆盖
		EVDF_ALPHA_TO_COVERAGE,

		//! 支持颜色掩码 (在输出中关闭颜色平面)
		EVDF_COLOR_MASK,

		//! 支持多渲染目标MRT
		EVDF_MULTIPLE_RENDER_TARGETS,

		//! 支持多渲染目标混合
		EVDF_MRT_BLEND,

		//! 支持多渲染目标分离颜色掩码
		EVDF_MRT_COLOR_MASK,

		//! 支持多渲染目标分离混合函数
		EVDF_MRT_BLEND_FUNC,

		//! 支持几何shader
		EVDF_GEOMETRY_SHADER,

		//! 支持遮蔽查询
		EVDF_OCCLUSION_QUERY,

		//! 支持多边形/深度基线偏移
		EVDF_POLYGON_OFFSET,

		//! 支持不同的混合操作
		EVDF_BLEND_OPERATIONS,

		//! 支持通过纹理矩阵变换纹理坐标
		EVDF_TEXTURE_MATRIX,

		//! 支持cg shader语言
		EVDF_CG,

		//! 统计特性数量
		EVDF_COUNT
	};
}

#endif