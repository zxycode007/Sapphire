#ifndef _SAPPHIRE_C_OPENGL_NORMAL_MAP_RENDERER_
#define _SAPPHIRE_C_OPENGL_NORMAL_MAP_RENDERER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_

#include "SapphireCOpenGLShaderMaterialRenderer.h"
#include "SapphireIShaderConstantSetCallBack.h"


namespace Sapphire
{
	//! Class for rendering normal maps with OpenGL
	class COpenGLNormalMapRenderer : public COpenGLShaderMaterialRenderer, public IShaderConstantSetCallBack
	{
	public:

		//! Constructor
		COpenGLNormalMapRenderer(COpenGLDriver* driver,
			SINT32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);

		//! Destructor
		~COpenGLNormalMapRenderer();

		//! Called by the engine when the vertex and/or pixel shader constants for an
		//! material renderer should be set.
		virtual void OnSetConstants(IMaterialRendererServices* services, SINT32 userData);

		//! Returns the render capability of the material.
		virtual SINT32 getRenderCapability() const;

	protected:

		bool CompiledShaders;
	};
}


#endif
#endif