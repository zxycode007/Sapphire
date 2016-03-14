#ifndef _SAPPHIRE_C_OPENGL_CG_RENDERER_
#define _SAPPHIRE_C_OPENGL_CG_RENDERER_

#include "SapphirePrerequisites.h"
#include "SapphireCompileConifg.h"
#if defined(_SAPPHIRE_COMPILE_WITH_OPENGL_) && defined(_SAPPHIRE_COMPILE_WITH_CG_)


#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#else
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#include "glext.h"
#endif
#endif

#include "CCgMaterialRenderer.h"
#include "Cg/cgGL.h"

#ifdef _MSC_VER
#pragma comment(lib, "cgGL.lib")
#endif

namespace Sapphire
{

	class COpenGLDriver;
	class IShaderConstantSetCallBack;

	class COpenGLCgUniformSampler2D : public CCgUniform
	{
	public:
		COpenGLCgUniformSampler2D(const CGparameter& parameter, bool global);

		void update(const void* data, const SMaterial& material) const;
	};

	class COpenGLCgMaterialRenderer : public CCgMaterialRenderer
	{
	public:
		COpenGLCgMaterialRenderer(COpenGLDriver* driver, SINT32& materialType,
			const c8* vertexProgram = 0, const c8* vertexEntry = "main",
			E_VERTEX_SHADER_TYPE vertexProfile = EVST_VS_1_1,
			const c8* fragmentProgram = 0, const c8* fragmentEntry = "main",
			E_PIXEL_SHADER_TYPE fragmentProfile = EPST_PS_1_1,
			const c8* geometryProgram = 0, const c8* geometryEntry = "main",
			E_GEOMETRY_SHADER_TYPE geometryProfile = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 vertices = 0, IShaderConstantSetCallBack* callback = 0,
			IMaterialRenderer* baseMaterial = 0, SINT32 userData = 0);

		virtual ~COpenGLCgMaterialRenderer();

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial, bool resetAllRenderstates, IMaterialRendererServices* services);
		virtual bool OnRender(IMaterialRendererServices* services, E_VERTEX_TYPE vtxtype);
		virtual void OnUnsetMaterial();

		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastMaterial, bool resetAllRenderstates);
		virtual IVideoDriver* getVideoDriver();

	protected:
		void init(SINT32& materialType,
			const c8* vertexProgram = 0, const c8* vertexEntry = "main",
			E_VERTEX_SHADER_TYPE vertexProfile = EVST_VS_1_1,
			const c8* fragmentProgram = 0, const c8* fragmentEntry = "main",
			E_PIXEL_SHADER_TYPE fragmentProfile = EPST_PS_1_1,
			const c8* geometryProgram = 0, const c8* geometryEntry = "main",
			E_GEOMETRY_SHADER_TYPE geometryProfile = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 vertices = 0);

		COpenGLDriver* Driver;
	};
}

#endif

#endif