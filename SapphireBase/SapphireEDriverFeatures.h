#ifndef _SAPPHIRE_EDRIVER_FEATURES_
#define _SAPPHIRE_EDRIVER_FEATURES_

namespace Sapphire
{
	//! ��ʾ������Ч��֧�ֲ�ѯö��
	enum E_VIDEO_DRIVER_FEATURE
	{
		//! �����ܹ���Ⱦ��һ������
		EVDF_RENDER_TO_TARGET = 0,

		//! Ӳ����T&L
		EVDF_HARDWARE_TL,

		//! ÿ�������ж�������
		EVDF_MULTITEXTURE,

		//! ˫�����˲�
		EVDF_BILINEAR_FILTER,

		//! MIPMAP
		EVDF_MIP_MAP,

		//! �Զ�����mipmap
		EVDF_MIP_MAP_AUTO_UPDATE,

		//! ֧��ģ�建����
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

		//!ARB������
		EVDF_ARB_VERTEX_PROGRAM_1,

		//! ARBƬ�α��
		EVDF_ARB_FRAGMENT_PROGRAM_1,

		//!glsl
		EVDF_ARB_GLSL,

		//!hlsl
		EVDF_HLSL,

		//! ������������֧��
		EVDF_TEXTURE_NSQUARE,

		//!��2ָ��������֧��
		EVDF_TEXTURE_NPOT,

		//! ֡������FBO
		EVDF_FRAMEBUFFER_OBJECT,

		// ���㻺����VBO
		EVDF_VERTEX_BUFFER_OBJECT,

		//! ֧��Alpha����
		EVDF_ALPHA_TO_COVERAGE,

		//! ֧����ɫ���� (������йر���ɫƽ��)
		EVDF_COLOR_MASK,

		//! ֧�ֶ���ȾĿ��MRT
		EVDF_MULTIPLE_RENDER_TARGETS,

		//! ֧�ֶ���ȾĿ����
		EVDF_MRT_BLEND,

		//! ֧�ֶ���ȾĿ�������ɫ����
		EVDF_MRT_COLOR_MASK,

		//! ֧�ֶ���ȾĿ������Ϻ���
		EVDF_MRT_BLEND_FUNC,

		//! ֧�ּ���shader
		EVDF_GEOMETRY_SHADER,

		//! ֧���ڱβ�ѯ
		EVDF_OCCLUSION_QUERY,

		//! ֧�ֶ����/��Ȼ���ƫ��
		EVDF_POLYGON_OFFSET,

		//! ֧�ֲ�ͬ�Ļ�ϲ���
		EVDF_BLEND_OPERATIONS,

		//! ֧��ͨ���������任��������
		EVDF_TEXTURE_MATRIX,

		//! ֧��cg shader����
		EVDF_CG,

		//! ͳ����������
		EVDF_COUNT
	};
}

#endif