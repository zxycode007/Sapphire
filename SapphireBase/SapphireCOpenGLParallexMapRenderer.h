#ifndef _SAPPHIRE_C_OPENGL_PARALLEX_MAP_RENDERER_
#define _SAPPHIRE_C_OPENGL_PARALLEX_MAP_RENDERER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_

#include "SapphireCOpenGLShaderMaterialRenderer.h"
#include "SapphireIShaderConstantSetCallBack.h"

namespace Sapphire
{
	//! Class for rendering normal maps with OpenGL
	class COpenGLParallaxMapRenderer : public COpenGLShaderMaterialRenderer, public IShaderConstantSetCallBack
	{
	public:

		//! Constructor
		COpenGLParallaxMapRenderer(COpenGLDriver* driver,
			SINT32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);

		//! Destructor
		~COpenGLParallaxMapRenderer();

		//! Called by the engine when the vertex and/or pixel shader constants for an
		//! material renderer should be set.
		virtual void OnSetConstants(IMaterialRendererServices* services, SINT32 userData);

		//! Returns the render capability of the material.
		virtual SINT32 getRenderCapability() const;

		virtual void OnSetMaterial(const SMaterial& material) { }
		virtual void OnSetMaterial(const SMaterial& material,
			const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services);

	protected:

		bool CompiledShaders;
		FLOAT32 CurrentScale;
	};
}

#endif
#endif