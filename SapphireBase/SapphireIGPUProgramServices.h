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

	//! 枚举不同shader类型
	enum E_GPU_SHADING_LANGUAGE
	{
		//! 默认的语义， 对于DX是HLSL，OGL是GLSL
		EGSL_DEFAULT = 0,

		//! CG语言.*/
		EGSL_CG
	};

	//! 可以创建和使用GPU程序的接口
	class IGPUProgrammingServices
	{
	public:

		 
		virtual ~IGPUProgrammingServices() {}

		//! 添加一个新的高级着色材质渲染器给视频驱动
		/** 当前只支持HLSL和GLSL
		\param vertexShaderProgram 包含vertex shader 程序的源码的字符串。如果不使用顶点程序可以为0。
		\param vertexShaderEntryPointName vertexShaderProgram的入口函数名
		\param vsCompileTarget Vertex shader 应该被编译的版本
		\param pixelShaderProgram 包含Fragment shader 程序的源码的字符串。如果不使用顶点程序可以为0。 
		\param pixelShaderEntryPointName FragmentShaderProgram的入口函数名
		\param psCompileTarget  FragmentShader 应该被编译的版本
		\param geometryShaderProgram 包含geometry shader程序的源码的字符串。如果不使用顶点程序可以为0。
		\param geometryShaderEntryPointName geometry shaderProgram的入口函数名
		\param gsCompileTarget  geometry shader 应该被编译的版本
		\param inType 传递给geometry shader的顶点类型 
		\param outType 通过geometry shader输出的顶点类型
		\param verticesOut geometry shader创建的最大顶点数,如果为0，以假设最大数目
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\param shaderLang 一个当前shader的着色语言的类型
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。
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

		//! 不用geometry shader的方波函数
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

		//! 使用很多默认值的方便函数，不用geometry shader
		/** 所有shader名都设置为main并且编译目标到shader type 1.1
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

		//! 使用很多默认值的方便函数，使用geometry shader 
		/** 
		所有shader名都设置为main并且编译目标到shader type 1.1和geometry shader4.0
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

		//! 类似于IGPUProgrammingServices::addShaderMaterial(), 但是不加载文件
		/** 当前只支持HLSL和GLSL
		\param vertexShaderProgram 包含vertex shader 程序的源码的文件名。如果不使用顶点程序可以为0。
		\param vertexShaderEntryPointName vertexShaderProgram的入口函数名
		\param vsCompileTarget Vertex shader 应该被编译的版本
		\param pixelShaderProgram 包含Fragment shader 程序的源码的文件名。如果不使用顶点程序可以为0。
		\param pixelShaderEntryPointName FragmentShaderProgram的入口函数名
		\param psCompileTarget  FragmentShader 应该被编译的版本
		\param geometryShaderProgram 包含geometry shader程序的源码的文件名。如果不使用顶点程序可以为0。
		\param geometryShaderEntryPointName geometry shaderProgram的入口函数名
		\param gsCompileTarget  geometry shader 应该被编译的版本
		\param inType 传递给geometry shader的顶点类型
		\param outType 通过geometry shader输出的顶点类型
		\param verticesOut geometry shader创建的最大顶点数,如果为0，以假设最大数目
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\param shaderLang 一个当前shader的着色语言的类型
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。
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

		//! 不用geometry shaders的方便函数
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

		//! 不使用geometry shaders， 使用很多默认值，方便使用的函数
		/** 所有shader名都设置为main并且编译目标到shader type 1.1
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

		///! 使用很多默认值的方便函数，使用geometry shader 
		/**
		所有shader名都设置为main并且编译目标到shader type 1.1和geometry shader4.0
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

		//! 类似 IGPUProgrammingServices::addShaderMaterial(), 但是加载文件
		/** 
		\param vertexShaderProgram 包含vertex shader 程序的源码的已加载文件对象。如果不使用顶点程序可以为0。
		\param vertexShaderEntryPointName vertexShaderProgram的入口函数名
		\param vsCompileTarget Vertex shader 应该被编译的版本
		\param pixelShaderProgram 包含Fragment shader 程序的源码的已加载文件对象。如果不使用顶点程序可以为0。 
		\param pixelShaderEntryPointName FragmentShaderProgram的入口函数名
		\param psCompileTarget  FragmentShader 应该被编译的版本
		\param geometryShaderProgram 包含geometry shader程序的源码的已加载文件对象。如果不使用顶点程序可以为0。
		\param geometryShaderEntryPointName geometry shaderProgram的入口函数名
		\param gsCompileTarget  geometry shader 应该被编译的版本
		\param inType 传递给geometry shader的顶点类型 
		\param outType 通过geometry shader输出的顶点类型
		\param verticesOut geometry shader创建的最大顶点数,如果为0，以假设最大数目
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\param shaderLang 一个当前shader的着色语言的类型
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。
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

		//! 不用geometry shaders的方便函数
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

		//! 添加一个新的ASM Shader材质渲染器给视频驱动
		/** 
		注意：推荐调用IVideoDriver::queryFeature()来检测IVideoDriver能否支持你用的Shader
	    这个材质是用IVideoDriver::addMaterialRenderer()添加到视频驱动
		\param vertexShaderProgram 包含vertex shader 程序的字符串的已加载文件对象。如果不使用顶点程序可以为0。

		对于DX8程序，如果有效的话，所有输入寄存器像这样：v0:
		position, v1: normal, v2: color, v3: texture cooridnates, v4: texture
		coordinates 2 
		 
		对于DX9程序， 你可以用dcl_statements.手动设置这些寄存器
		\param pixelShaderProgram 包含Fragment shader 程序的源码的已加载文件对象。如果不使用顶点程序可以为0。
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。 */
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram = 0,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;

		//! 类似 IGPUProgrammingServices::addShaderMaterial(), 但是从文件加载
		/** \param vertexShaderProgram包含vertex shader 程序的文本文件对象。如果不使用顶点程序可以为0。
		\param pixelShaderProgram  包含Fragment shader 程序的文本文件对象。如果不使用顶点程序可以为0。
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。 */
		virtual SINT32 addShaderMaterialFromFiles(IReadFile* vertexShaderProgram,
			IReadFile* pixelShaderProgram,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;

		//! 类似 IGPUProgrammingServices::addShaderMaterial(), 但是从文件加载
		/** \param vertexShaderProgram包含vertex shader 程序的文本文件。如果不使用顶点程序可以为0。
		\param pixelShaderProgram  包含Fragment shader 程序的文本文件。如果不使用顶点程序可以为0。
		\param callback 指向一个IShaderConstantSetCallBack的实现，这里面你可以设置需要的顶点，像素，和几何着色器程序。
		设置为0表示你不需要这个
		\param baseMaterial 基础材质，它的渲染状态用于着色材质
		\param userData 一个用户的int数据，这个int能够设置任意值和在当调用OnSetConstants()时设置回调函数里的参数。
		这个方法对于多材质和在调用时区分它们能够相对容易的使用相同的回调函数。
		\return 返回这个材质类型数，它可以被用做设置带SMaterial::MaterialType用于渲染器。返回-1表示错误。
		如果一个shader程序没有编译或编译目标不可到达。这个错误字符串会被输出的error.log并且能够被自定义
		的事件接收器捕获。 */
		virtual SINT32 addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0) = 0;
	};


}  

#endif