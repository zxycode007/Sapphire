#ifndef _SAPPHIRE_IGPU_PROGRAM_SERVICES_
#define _SAPPHIRE_IGPU_PROGRAM_SERVICES_

#include "SapphirePrerequisites.h"
#include "SapphireEShaderTypes.h"
#include "SapphireEPrimitiveTypes.h"
#include "SapphireSMaterial.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	class IReadFile;
	class IVideoDriver;
	class IShaderConstantSetCallBack;

	//! ö�ٲ�ͬshader����
	enum E_GPU_SHADING_LANGUAGE
	{
		//! Ĭ�ϵ����壬 ����DX��HLSL��OGL��GLSL
		EGSL_DEFAULT = 0,

		//! CG����.*/
		EGSL_CG
	};

	//! ���Դ�����ʹ��GPU����Ľӿ�
	class IGPUProgrammingServices
	{
	public:

		 
		virtual ~IGPUProgrammingServices() {}

		//! ���һ���µĸ߼���ɫ������Ⱦ������Ƶ����
		/** ��ǰֻ֧��HLSL��GLSL
		\param vertexShaderProgram ����vertex shader �����Դ����ַ����������ʹ�ö���������Ϊ0��
		\param vertexShaderEntryPointName vertexShaderProgram����ں�����
		\param vsCompileTarget Vertex shader Ӧ�ñ�����İ汾
		\param pixelShaderProgram ����Fragment shader �����Դ����ַ����������ʹ�ö���������Ϊ0�� 
		\param pixelShaderEntryPointName FragmentShaderProgram����ں�����
		\param psCompileTarget  FragmentShader Ӧ�ñ�����İ汾
		\param geometryShaderProgram ����geometry shader�����Դ����ַ����������ʹ�ö���������Ϊ0��
		\param geometryShaderEntryPointName geometry shaderProgram����ں�����
		\param gsCompileTarget  geometry shader Ӧ�ñ�����İ汾
		\param inType ���ݸ�geometry shader�Ķ������� 
		\param outType ͨ��geometry shader����Ķ�������
		\param verticesOut geometry shader��������󶥵���,���Ϊ0���Լ��������Ŀ
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\param shaderLang һ����ǰshader����ɫ���Ե�����
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼�����������
		  */
		virtual SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget,
			const c8* pixelShaderProgram,
			const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget,
			const c8* geometryShaderProgram,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT) = 0;

		//! ����geometry shader�ķ�������
		SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* vertexShaderEntryPointName = "main",
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			const c8* pixelShaderProgram = 0,
			const c8* pixelShaderEntryPointName = "main",
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT)
		{
			return addHighLevelShaderMaterial(
				vertexShaderProgram, vertexShaderEntryPointName,
				vsCompileTarget, pixelShaderProgram,
				pixelShaderEntryPointName, psCompileTarget,
				0, "main", EGST_GS_4_0,
				EPT_TRIANGLES, EPT_TRIANGLE_STRIP, 0,
				callback, baseMaterial, userData, shadingLang);
		}

		//! ʹ�úܶ�Ĭ��ֵ�ķ��㺯��������geometry shader
		/** ����shader��������Ϊmain���ұ���Ŀ�굽shader type 1.1
		*/
		SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0)
		{
			return addHighLevelShaderMaterial(
				vertexShaderProgram, "main",
				EVST_VS_1_1, pixelShaderProgram,
				"main", EPST_PS_1_1,
				0, "main", EGST_GS_4_0,
				EPT_TRIANGLES, EPT_TRIANGLE_STRIP, 0,
				callback, baseMaterial, userData);
		}

		//! ʹ�úܶ�Ĭ��ֵ�ķ��㺯����ʹ��geometry shader 
		/** 
		����shader��������Ϊmain���ұ���Ŀ�굽shader type 1.1��geometry shader4.0
		*/
		SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* pixelShaderProgram = 0,
			const c8* geometryShaderProgram = 0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0)
		{
			return addHighLevelShaderMaterial(
				vertexShaderProgram, "main",
				EVST_VS_1_1, pixelShaderProgram,
				"main", EPST_PS_1_1,
				geometryShaderProgram, "main", EGST_GS_4_0,
				inType, outType, verticesOut,
				callback, baseMaterial, userData);
		}

		//! ������IGPUProgrammingServices::addShaderMaterial(), ���ǲ������ļ�
		/** ��ǰֻ֧��HLSL��GLSL
		\param vertexShaderProgram ����vertex shader �����Դ����ļ����������ʹ�ö���������Ϊ0��
		\param vertexShaderEntryPointName vertexShaderProgram����ں�����
		\param vsCompileTarget Vertex shader Ӧ�ñ�����İ汾
		\param pixelShaderProgram ����Fragment shader �����Դ����ļ����������ʹ�ö���������Ϊ0��
		\param pixelShaderEntryPointName FragmentShaderProgram����ں�����
		\param psCompileTarget  FragmentShader Ӧ�ñ�����İ汾
		\param geometryShaderProgram ����geometry shader�����Դ����ļ����������ʹ�ö���������Ϊ0��
		\param geometryShaderEntryPointName geometry shaderProgram����ں�����
		\param gsCompileTarget  geometry shader Ӧ�ñ�����İ汾
		\param inType ���ݸ�geometry shader�Ķ�������
		\param outType ͨ��geometry shader����Ķ�������
		\param verticesOut geometry shader��������󶥵���,���Ϊ0���Լ��������Ŀ
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\param shaderLang һ����ǰshader����ɫ���Ե�����
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼�����������
		*/
		virtual SINT32 addHighLevelShaderMaterialFromFiles(
			const path& vertexShaderProgramFileName,
			const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget,
			const path& pixelShaderProgramFileName,
			const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget,
			const path& geometryShaderProgramFileName,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT) = 0;

		//! ����geometry shaders�ķ��㺯��
		SINT32 addHighLevelShaderMaterialFromFiles(
			const path& vertexShaderProgramFileName,
			const c8* vertexShaderEntryPointName = "main",
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			const path& pixelShaderProgramFileName = "",
			const c8* pixelShaderEntryPointName = "main",
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT)
		{
			return addHighLevelShaderMaterialFromFiles(
				vertexShaderProgramFileName, vertexShaderEntryPointName,
				vsCompileTarget, pixelShaderProgramFileName,
				pixelShaderEntryPointName, psCompileTarget,
				"", "main", EGST_GS_4_0,
				EPT_TRIANGLES, EPT_TRIANGLE_STRIP, 0,
				callback, baseMaterial, userData, shadingLang);
		}

		//! ��ʹ��geometry shaders�� ʹ�úܶ�Ĭ��ֵ������ʹ�õĺ���
		/** ����shader��������Ϊmain���ұ���Ŀ�굽shader type 1.1
		*/
		SINT32 addHighLevelShaderMaterialFromFiles(
			const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName = "",
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0)
		{
			return addHighLevelShaderMaterialFromFiles(
				vertexShaderProgramFileName, "main",
				EVST_VS_1_1, pixelShaderProgramFileName,
				"main", EPST_PS_1_1,
				"", "main", EGST_GS_4_0,
				EPT_TRIANGLES, EPT_TRIANGLE_STRIP, 0,
				callback, baseMaterial, userData);
		}

		///! ʹ�úܶ�Ĭ��ֵ�ķ��㺯����ʹ��geometry shader 
		/**
		����shader��������Ϊmain���ұ���Ŀ�굽shader type 1.1��geometry shader4.0
		*/
		
		SINT32 addHighLevelShaderMaterialFromFiles(
			const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName = "",
			const path& geometryShaderProgramFileName = "",
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0)
		{
			return addHighLevelShaderMaterialFromFiles(
				vertexShaderProgramFileName, "main",
				EVST_VS_1_1, pixelShaderProgramFileName,
				"main", EPST_PS_1_1,
				geometryShaderProgramFileName, "main", EGST_GS_4_0,
				inType, outType, verticesOut,
				callback, baseMaterial, userData);
		}

		//! ���� IGPUProgrammingServices::addShaderMaterial(), ���Ǽ����ļ�
		/** 
		\param vertexShaderProgram ����vertex shader �����Դ����Ѽ����ļ����������ʹ�ö���������Ϊ0��
		\param vertexShaderEntryPointName vertexShaderProgram����ں�����
		\param vsCompileTarget Vertex shader Ӧ�ñ�����İ汾
		\param pixelShaderProgram ����Fragment shader �����Դ����Ѽ����ļ����������ʹ�ö���������Ϊ0�� 
		\param pixelShaderEntryPointName FragmentShaderProgram����ں�����
		\param psCompileTarget  FragmentShader Ӧ�ñ�����İ汾
		\param geometryShaderProgram ����geometry shader�����Դ����Ѽ����ļ����������ʹ�ö���������Ϊ0��
		\param geometryShaderEntryPointName geometry shaderProgram����ں�����
		\param gsCompileTarget  geometry shader Ӧ�ñ�����İ汾
		\param inType ���ݸ�geometry shader�Ķ������� 
		\param outType ͨ��geometry shader����Ķ�������
		\param verticesOut geometry shader��������󶥵���,���Ϊ0���Լ��������Ŀ
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\param shaderLang һ����ǰshader����ɫ���Ե�����
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼�����������
		*/
		virtual SINT32 addHighLevelShaderMaterialFromFiles(
			IReadFile* vertexShaderProgram,
			const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget,
			IReadFile* pixelShaderProgram,
			const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget,
			IReadFile* geometryShaderProgram,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT) = 0;

		//! ����geometry shaders�ķ��㺯��
		SINT32 addHighLevelShaderMaterialFromFiles(
			IReadFile* vertexShaderProgram,
			const c8* vertexShaderEntryPointName = "main",
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			IReadFile* pixelShaderProgram = 0,
			const c8* pixelShaderEntryPointName = "main",
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT)
		{
			return addHighLevelShaderMaterialFromFiles(
				vertexShaderProgram, vertexShaderEntryPointName,
				vsCompileTarget, pixelShaderProgram,
				pixelShaderEntryPointName, psCompileTarget,
				0, "main", EGST_GS_4_0,
				EPT_TRIANGLES, EPT_TRIANGLE_STRIP, 0,
				callback, baseMaterial, userData, shadingLang);
		}

		//! ���һ���µ�ASM Shader������Ⱦ������Ƶ����
		/** 
		ע�⣺�Ƽ�����IVideoDriver::queryFeature()�����IVideoDriver�ܷ�֧�����õ�Shader
	    �����������IVideoDriver::addMaterialRenderer()��ӵ���Ƶ����
		\param vertexShaderProgram ����vertex shader ������ַ������Ѽ����ļ����������ʹ�ö���������Ϊ0��

		����DX8���������Ч�Ļ�����������Ĵ�����������v0:
		position, v1: normal, v2: color, v3: texture cooridnates, v4: texture
		coordinates 2 
		 
		����DX9���� �������dcl_statements.�ֶ�������Щ�Ĵ���
		\param pixelShaderProgram ����Fragment shader �����Դ����Ѽ����ļ����������ʹ�ö���������Ϊ0��
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼����������� */
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram = 0,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;

		//! ���� IGPUProgrammingServices::addShaderMaterial(), ���Ǵ��ļ�����
		/** \param vertexShaderProgram����vertex shader ������ı��ļ����������ʹ�ö���������Ϊ0��
		\param pixelShaderProgram  ����Fragment shader ������ı��ļ����������ʹ�ö���������Ϊ0��
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼����������� */
		virtual SINT32 addShaderMaterialFromFiles(IReadFile* vertexShaderProgram,
			IReadFile* pixelShaderProgram,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;

		//! ���� IGPUProgrammingServices::addShaderMaterial(), ���Ǵ��ļ�����
		/** \param vertexShaderProgram����vertex shader ������ı��ļ��������ʹ�ö���������Ϊ0��
		\param pixelShaderProgram  ����Fragment shader ������ı��ļ��������ʹ�ö���������Ϊ0��
		\param callback ָ��һ��IShaderConstantSetCallBack��ʵ�֣������������������Ҫ�Ķ��㣬���أ��ͼ�����ɫ������
		����Ϊ0��ʾ�㲻��Ҫ���
		\param baseMaterial �������ʣ�������Ⱦ״̬������ɫ����
		\param userData һ���û���int���ݣ����int�ܹ���������ֵ���ڵ�����OnSetConstants()ʱ���ûص�������Ĳ�����
		����������ڶ���ʺ��ڵ���ʱ���������ܹ�������׵�ʹ����ͬ�Ļص�������
		\return ������������������������Ա��������ô�SMaterial::MaterialType������Ⱦ��������-1��ʾ����
		���һ��shader����û�б�������Ŀ�겻�ɵ����������ַ����ᱻ�����error.log�����ܹ����Զ���
		���¼����������� */
		virtual SINT32 addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;
	};


}  

#endif