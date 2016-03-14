#ifndef _SAPPHIRE_C_OPENGL_GLSL_MATERIAL_RENDERER_
#define _SAPPHIRE_C_OPENGL_GLSL_MATERIAL_RENDERER_

#include "SapphirePrerequisites.h"

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_

#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
// include windows headers for HWND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"

#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
#pragma comment(lib, "OpenGL32.lib")
#endif
#elif SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include <OpenGL/gl.h>
#elif defined(_SAPPHIRE_COMPILE_WITH_SDL_DEVICE_)
#define NO_SDL_GLEXT
#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>
#else
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

#include "SapphireIMaterialRenderer.h"
#include "SapphireIMaterialRendererServices.h"
#include "SapphireIGPUProgramServices.h"

namespace Sapphire
{
	class COpenGLDriver;
	class IShaderConstantSetCallBack;

	//! Class for using GLSL shaders with OpenGL
	//! Please note: This renderer implements its own IMaterialRendererServices
	// 使用GLSL 着色器的OpenGL类
	class COpenGLSLMaterialRenderer : public IMaterialRenderer, public IMaterialRendererServices
	{
	public:

		//! Constructor
		COpenGLSLMaterialRenderer(
			COpenGLDriver* driver,          //opengl 驱动
			SINT32& outMaterialTypeNr,               //输出材质类型数
			const c8* vertexShaderProgram = 0,       //vertex shader名
			const c8* vertexShaderEntryPointName = 0,     //vertex shader 入口名
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,   //vertex shader 编译版本
			const c8* pixelShaderProgram = 0,                 //pixel shader 名
			const c8* pixelShaderEntryPointName = 0,            //pixel shader 入口名
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,      //pixel shader 编译版本
			const c8* geometryShaderProgram = 0,                      //geometry shader  名
			const c8* geometryShaderEntryPointName = "main",         // geometry shader 入口名
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,    //geometry shader  编译版本
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,                  //输入图元类型
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,            // 输出图元类型
			UINT32 verticesOut = 0,                          //输出顶点数
			IShaderConstantSetCallBack* callback = 0,            //Constant设置回调指针
			IMaterialRenderer* baseMaterial = 0,                 //材质渲染器
			SINT32 userData = 0);                                 //用户数据

		//! Destructor
		virtual ~COpenGLSLMaterialRenderer();

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services);

		virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);

		virtual void OnUnsetMaterial();

		//! Returns if the material is transparent.
		virtual bool isTransparent() const;

		// implementations for the render services
		// 渲染器服务的实现
		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastMaterial, bool resetAllRenderstates);
		virtual bool setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count);
		virtual bool setVertexShaderConstant(const c8* name, const bool* bools, int count);
		virtual bool setVertexShaderConstant(const c8* name, const SINT32* ints, int count);
		virtual void setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);
		virtual bool setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count);
		virtual bool setPixelShaderConstant(const c8* name, const bool* bools, int count);
		virtual bool setPixelShaderConstant(const c8* name, const SINT32* ints, int count);
		virtual void setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);
		virtual IVideoDriver* getVideoDriver();

	protected:

		//! constructor only for use by derived classes who want to
		//! create a fall back material for example.
		COpenGLSLMaterialRenderer(COpenGLDriver* driver,
			IShaderConstantSetCallBack* callback,
			IMaterialRenderer* baseMaterial,
			SINT32 userData = 0);

		void init(SINT32& outMaterialTypeNr,
			const c8* vertexShaderProgram,
			const c8* pixelShaderProgram,
			const c8* geometryShaderProgram,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0);

		bool createProgram();
		bool createShader(GLenum shaderType, const char* shader);
		bool linkProgram();

		COpenGLDriver* Driver;
		IShaderConstantSetCallBack* CallBack;
		IMaterialRenderer* BaseMaterial;

		struct SUniformInfo
		{
			String name;
			GLenum type;
		};

		GLhandleARB Program;
		GLuint Program2;
		vector<SUniformInfo>::type UniformInfo;
		SINT32 UserData;
	};

}

#endif
#endif