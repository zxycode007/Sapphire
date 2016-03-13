#ifndef _SAPPHIRE_C_OPENGL_MATERIAL_RENDERER_
#define  _SAPPHIRE_C_OPENGL_MATERIAL_RENDERER_

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

namespace Sapphire
{
	class COpenGLDriver;
	class IShaderConstantSetCallBack;
	class IMaterialRenderer;

	//! Class for using vertex and pixel shaders with OpenGL
	// OpenGL使用顶点和像素着色器的渲染器类
	class COpenGLShaderMaterialRenderer : public IMaterialRenderer
	{
	public:

		//! Constructor
		COpenGLShaderMaterialRenderer(COpenGLDriver* driver,
			SINT32& outMaterialTypeNr, const c8* vertexShaderProgram, const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback, IMaterialRenderer* baseMaterial, SINT32 userData);

		//! Destructor
		virtual ~COpenGLShaderMaterialRenderer();

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services);

		virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);

		virtual void OnUnsetMaterial();

		//! Returns if the material is transparent.
		virtual bool isTransparent() const;

	protected:

		//! constructor only for use by derived classes who want to
		//! create a fall back material for example.
		COpenGLShaderMaterialRenderer(COpenGLDriver* driver,
			IShaderConstantSetCallBack* callback,
			IMaterialRenderer* baseMaterial, SINT32 userData = 0);

		// must not be called more than once!
		void init(SINT32& outMaterialTypeNr, const c8* vertexShaderProgram,
			const c8* pixelShaderProgram, E_VERTEX_TYPE type);

		bool createPixelShader(const c8* pxsh);
		bool createVertexShader(const c8* vtxsh);
		bool checkError(const c8* type);

		COpenGLDriver* Driver;
		IShaderConstantSetCallBack* CallBack;
		IMaterialRenderer* BaseMaterial;

		GLuint VertexShader;
		// We have 4 values here, [0] is the non-fog version, the other three are
		// ARB_fog_linear, ARB_fog_exp, and ARB_fog_exp2 in that order
		vector<GLuint>::type PixelShader;
		SINT32 UserData;
	};
}
#endif
#endif