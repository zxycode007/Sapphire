#ifndef _SAPPHIRE_C_OPENGL_MATERIAL_RENDERER_
#define _SAPPHIRE_C_OPENGL_MATERIAL_RENDERER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_

#include "SapphireCOpenGLDriver.h"
#include "SapphireIMaterialRenderer.h"



namespace Sapphire
{
	//! Base class for all internal OpenGL material renderers
	// 所有OpenGL材质渲染器的内部基类
	class COpenGLMaterialRenderer : public IMaterialRenderer
	{
	public:

		//! Constructor
		COpenGLMaterialRenderer(COpenGLDriver* driver) : Driver(driver)
		{
		}

	protected:

		COpenGLDriver* Driver;
	};


	//! Solid material renderer
	// 固体材质渲染器
	class COpenGLMaterialRenderer_SOLID : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_SOLID(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}
		//重载OnSetMaterial()函数  
		//在调用IVideoDriver::setMaterial()时设置渲染状态
		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (resetAllRenderstates || (material.MaterialType != lastMaterial.MaterialType))
			{
				// thanks to Murphy, the following line removed some
				// bugs with several OpenGL implementations.
				//设置OpenGL纹理环境
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
	};


	//! Generic Texture Blend
	//普通纹理混合渲染器
	class COpenGLMaterialRenderer_ONETEXTURE_BLEND : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_ONETEXTURE_BLEND(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			//		if (material.MaterialType != lastMaterial.MaterialType ||
			//			material.MaterialTypeParam != lastMaterial.MaterialTypeParam ||
			//			resetAllRenderstates)
			{
				E_BLEND_FACTOR srcFact, dstFact;
				E_MODULATE_FUNC modulate;
				UINT32 alphaSource;
				unpack_textureBlendFunc(srcFact, dstFact, modulate, alphaSource, material.MaterialTypeParam);

#ifdef GL_ARB_texture_env_combine
				//设置纹理环境模式:   GL_COMBINE
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				//设置纹理RGB组合：  GL_MODULATE   颜色调制
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
				//设置纹理RGB源1：  GL_TEXTURE  当前激活的纹理单元
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
				//设置纹理RGB源2：  GL_PREVIOUS   前一个纹理单元的结果值
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
				glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, (FLOAT32)modulate);
#else
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);  
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, (FLOAT32)modulate);
#endif
				//设置源和目的的混合因子函数
				glBlendFunc(Driver->getGLBlend(srcFact), Driver->getGLBlend(dstFact));
				//启用alpha测试
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.f);
				//打开混合
				glEnable(GL_BLEND);
				//判断是否有alpha混合因子
				if (textureBlendFunc_hasAlpha(srcFact) || textureBlendFunc_hasAlpha(dstFact))
				{
					if (alphaSource == EAS_VERTEX_COLOR)
					{
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
#endif
					}
					else if (alphaSource == EAS_TEXTURE)
					{
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
					}
					else
					{
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_TEXTURE);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
						glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_TEXTURE);
#endif
					}
				}
			}
		}
		//取消材质
		virtual void OnUnsetMaterial()
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.f);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}

		
		//Returns if the material is transparent.
		//这个材质是否透明
		/** Is not always transparent, but mostly. */
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Solid 2 layer material renderer
	// 2层材质固体渲染器
	class COpenGLMaterialRenderer_SOLID_2_LAYER : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_SOLID_2_LAYER(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(2);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				//是否有多重纹理
				if (Driver->queryFeature(EVDF_MULTITEXTURE))
				{
					//激活纹理单元1
					Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
					//设置纹理环境模式:   GL_COMBINE
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					//设置Alpha组合属性 GL_REPLACE
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
					//设置alpha源1   GL_PRIMARY_COLOR_ARB  几何图元颜色
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
					//设置RGB组合属性  GL_INTERPOLATE   插值
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE);
					//设置RGB源1  GL_TEXTURE  当前激活的纹理单元
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					//设置RGB源2  GL_PREVIOUS 前一个纹理单元的结果
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
					//设置RGB源3  GL_PRIMARY_COLOR  几何图元自身颜色
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR);
					//设定操作数Arg2 为GL_SRC_ALPHA  使用alpha值  （几何图元自身颜色只使用alpha值）
					glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);
#else
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_INTERPOLATE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, GL_PRIMARY_COLOR);
					glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_SRC_ALPHA);
#endif
				}
			}
		}

		virtual void OnUnsetMaterial()
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
				glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);
#else
				glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_SRC_COLOR);
#endif
				Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
			}
		}
	};


	//! Transparent add color material renderer
	//  透明附加颜色材质渲染器
	class COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
			//如果材质类型发生改变或重置所有渲染状态
			if ((material.MaterialType != lastMaterial.MaterialType) || resetAllRenderstates)
			{
				//设置源和目的的混合因子函数
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glEnable(GL_BLEND);
			}
		}

		virtual void OnUnsetMaterial()
		{
			glDisable(GL_BLEND);
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Transparent vertex alpha material renderer
	//透明顶点alpha材质渲染器
	class COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
#ifdef GL_ARB_texture_env_combine
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
			}
		}

		virtual void OnUnsetMaterial()
		{
			// default values
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
			glDisable(GL_BLEND);
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//!Transparent alpha channel material renderer
	//alpha透明通道材质渲染器
	class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates
				|| material.MaterialTypeParam != lastMaterial.MaterialTypeParam)
			{
#ifdef GL_ARB_texture_env_combine
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST);

				glAlphaFunc(GL_GREATER, material.MaterialTypeParam);
			}
		}

		virtual void OnUnsetMaterial()
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
#else
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
#endif
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Transparent alpha channel material renderer
	//  透明alpha通道材质渲染器
	class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.5f);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}

		virtual void OnUnsetMaterial()
		{
			glDisable(GL_ALPHA_TEST);
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return false;  // this material is not really transparent because it does no blending.
		}
	};


	//! material renderer for all kinds of lightmaps
	//所有光照贴图的材质渲染器
	class COpenGLMaterialRenderer_LIGHTMAP : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_LIGHTMAP(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(2);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				// 漫反射贴图

				switch (material.MaterialType)
				{
				case EMT_LIGHTMAP_LIGHTING:
				case EMT_LIGHTMAP_LIGHTING_M2:
				case EMT_LIGHTMAP_LIGHTING_M4:
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					break;
				case EMT_LIGHTMAP_ADD:
				case EMT_LIGHTMAP:
				case EMT_LIGHTMAP_M2:
				case EMT_LIGHTMAP_M4:
				default:
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
					break;
				}
				//是否有多重纹理
				if (Driver->queryFeature(EVDF_MULTITEXTURE))
				{
					// lightmap

					Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);

					if (material.MaterialType == EMT_LIGHTMAP_ADD)
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD);
					else
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);

					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

					if (material.MaterialType == EMT_LIGHTMAP_ADD)
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
					else
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);

					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PREVIOUS_EXT);
#endif

					switch (material.MaterialType)
					{
					case EMT_LIGHTMAP_M4:
					case EMT_LIGHTMAP_LIGHTING_M4:
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 4.0f);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
#endif
						break;
					case EMT_LIGHTMAP_M2:
					case EMT_LIGHTMAP_LIGHTING_M2:
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2.0f);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
#endif
						break;
					default:
#ifdef GL_ARB_texture_env_combine
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.0f);
#else
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.0f);
#endif
					}
					Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
				}
			}
		}

		virtual void OnUnsetMaterial()
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
				glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.f);
#else
				glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f);
#endif
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
	};



	//! detail map  material renderer
	//细节贴图材质 渲染器
	class COpenGLMaterialRenderer_DETAIL_MAP : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_DETAIL_MAP(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(2);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				// diffuse map is default modulated

				// detail map on second layer
				//细节纹理在第二层
				if (Driver->queryFeature(EVDF_MULTITEXTURE))
				{
					Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_EXT);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
				}
			}
		}

		virtual void OnUnsetMaterial()
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
			}
		}
	};


	//! sphere map material renderer
	//球体贴图材质渲染器
	class COpenGLMaterialRenderer_SPHERE_MAP : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_SPHERE_MAP(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(1);
			// texture needs to be flipped for OpenGL
			// 纹理在OpenGL中需要被翻转
			Matrix4 tmp = Driver->getTransform(ETS_TEXTURE_0);
			//tmp[5] *= -1;
			tmp.getIndex(5) *= -1;
			Driver->setTransform(ETS_TEXTURE_0, tmp);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
			}
		}

		virtual void OnUnsetMaterial()
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}
	};


	//! reflection 2 layer material renderer
	// 反射2层材质渲染器
	class COpenGLMaterialRenderer_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_REFLECTION_2_LAYER(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(2);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
				if (Driver->queryFeature(EVDF_MULTITEXTURE))
				{
					Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
				}
				//自动生成纹理设置
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
			}
		}

		virtual void OnUnsetMaterial()
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
			}
		}
	};


	//! reflection 2 layer material renderer
	class COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
	{
	public:

		COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(COpenGLDriver* d)
			: COpenGLMaterialRenderer(d) {}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services)
		{
			Driver->disableTextures(2);
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

			if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
			{
#ifdef GL_ARB_texture_env_combine
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_ARB);
#endif
				if (Driver->queryFeature(EVDF_MULTITEXTURE))
				{
					Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_ARB);
#endif
				}
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
			}
		}

		virtual void OnUnsetMaterial()
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
			}
			glDisable(GL_BLEND);
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};
}
#endif
#endif