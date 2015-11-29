#ifndef _SAPPHIRE_VIDEO_DRVIER_
#define _SAPPHIRE_VIDEO_DRVIER_

#include "SapphireEMaterialFlags.h"
#include "SapphirePrerequisites.h"
#include "SapphireSMaterial.h"

namespace Sapphire
{
	//! 几何变换状态的枚举
	enum E_TRANSFORMATION_STATE
	{
		//! 观察空间变换
		ETS_VIEW = 0,
		//! 世界空间变换
		ETS_WORLD,
		//! 透视投影变换
		ETS_PROJECTION,
		//! 纹理变换0
		ETS_TEXTURE_0,
		//! 纹理变换1
		ETS_TEXTURE_1,
		//! 纹理变换2
		ETS_TEXTURE_2,
		//! 纹理变换3
		ETS_TEXTURE_3,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>4
		//! 纹理变换4
		ETS_TEXTURE_4,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>5
		//! 纹理变换5
		ETS_TEXTURE_5,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>6
		//!纹理变换6
		ETS_TEXTURE_6,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>7
		//! 纹理变换7
		ETS_TEXTURE_7,
#endif
#endif
#endif
#endif
		 
		ETS_COUNT
	};


	//! 枚举上个渲染循环后将有丢弃的资源信号
	/** 这个值通过驱动标记，告诉应用程序某些资源需要丢弃和重新创建。 引擎将有时会重新创建实际的对象，但是重新创建的内容由应用程序决定。
     */
	enum E_LOST_RESOURCE
	{
		//! 哪个设备/驱动要丢弃
		ELR_DEVICE = 1,
		//! 所有的纹理都要丢弃， 罕见问题
		ELR_TEXTURES = 2,
		//! 所有的渲染目标都要丢弃， d3d常见问题
		ELR_RTTS = 4,
		//! 硬件缓冲区丢弃，将会自动重新创建，但是在这一帧里会需要一点时间
		ELR_HW_BUFFERS = 8
	};


	//! 特别的渲染目标， 它通常映射到专门的硬件
	/** 这些渲染目标不需要支持GFX卡*/
	enum E_RENDER_TARGET
	{
		//! 这个渲染目标是主颜色缓冲区
		ERT_FRAME_BUFFER = 0,
		//! 这个渲染目标是一个渲染纹理
		ERT_RENDER_TEXTURE,
		//! 多渲染目标纹理
		ERT_MULTI_RENDER_TEXTURES,
		//! 这个渲染目标是主颜色帧缓冲区
		ERT_STEREO_LEFT_BUFFER,
		//! 渲染目标是右颜色缓冲区（左是主缓冲区）
		ERT_STEREO_RIGHT_BUFFER,
		//! 同时渲染两个stereo缓冲区
		ERT_STEREO_BOTH_BUFFERS,
		//! 辅助缓冲区 0
		ERT_AUX_BUFFER0,
		//! 辅助缓冲区 1
		ERT_AUX_BUFFER1,
		//! 辅助缓冲区 2
		ERT_AUX_BUFFER2,
		//! 辅助缓冲区 3
		ERT_AUX_BUFFER3,
		//! 辅助缓冲区 4
		ERT_AUX_BUFFER4
	};


	//雾的类型
	enum E_FOG_TYPE
	{
		EFT_FOG_EXP = 0,
		EFT_FOG_LINEAR,
		EFT_FOG_EXP2
	};

	const c8* const FogTypeNames[] =
	{
		"FogExp",
		"FogLinear",
		"FogExp2",
		0
	};

	//覆盖材质设置
	struct SOverrideMaterial
	{
		//! 材质
		SMaterial Material;
		//! 用于覆盖的E_MATERIAL_FLAGS值，可以用'|'链接多个
		UINT32 EnableFlags;
		//! 设置渲染通道的材质覆盖处于激活状态，来源于E_SCENE_NODE_RENDER_PASS的值，可以用'|'链接多个
		UINT32 EnablePasses;
		//! 全局开启标志, 通过SceneManager覆写到每一个渲染通道
		//如果覆盖材质在当前通道打开，场景管理器用EnablePass数组并且设置Enabled为true
		bool Enabled;

	 
		SOverrideMaterial() : EnableFlags(0), EnablePasses(0), Enabled(false) {}

		//!应用打开覆盖
		void apply(SMaterial& material)
		{
			if (Enabled)
			{
				for (UINT32 i = 0; i<32; ++i)
				{
					const UINT32 num = (1 << i);
					if (EnableFlags & num)
					{
						switch (num)
						{
						case EMF_WIREFRAME: material.Wireframe = Material.Wireframe; break;
						case EMF_POINTCLOUD: material.PointCloud = Material.PointCloud; break;
						case EMF_GOURAUD_SHADING: material.GouraudShading = Material.GouraudShading; break;
						case EMF_LIGHTING: material.Lighting = Material.Lighting; break;
						case EMF_ZBUFFER: material.ZBuffer = Material.ZBuffer; break;
						case EMF_ZWRITE_ENABLE: material.ZWriteEnable = Material.ZWriteEnable; break;
						case EMF_BACK_FACE_CULLING: material.BackfaceCulling = Material.BackfaceCulling; break;
						case EMF_FRONT_FACE_CULLING: material.FrontfaceCulling = Material.FrontfaceCulling; break;
						case EMF_BILINEAR_FILTER: material.TextureLayer[0].BilinearFilter = Material.TextureLayer[0].BilinearFilter; break;
						case EMF_TRILINEAR_FILTER: material.TextureLayer[0].TrilinearFilter = Material.TextureLayer[0].TrilinearFilter; break;
						case EMF_ANISOTROPIC_FILTER: material.TextureLayer[0].AnisotropicFilter = Material.TextureLayer[0].AnisotropicFilter; break;
						case EMF_FOG_ENABLE: material.FogEnable = Material.FogEnable; break;
						case EMF_NORMALIZE_NORMALS: material.NormalizeNormals = Material.NormalizeNormals; break;
						case EMF_TEXTURE_WRAP:
							material.TextureLayer[0].TextureWrapU = Material.TextureLayer[0].TextureWrapU;
							material.TextureLayer[0].TextureWrapV = Material.TextureLayer[0].TextureWrapV;
							break;
						case EMF_ANTI_ALIASING: material.AntiAliasing = Material.AntiAliasing; break;
						case EMF_COLOR_MASK: material.ColorMask = Material.ColorMask; break;
						case EMF_COLOR_MATERIAL: material.ColorMaterial = Material.ColorMaterial; break;
						case EMF_USE_MIP_MAPS: material.UseMipMaps = Material.UseMipMaps; break;
						case EMF_BLEND_OPERATION: material.BlendOperation = Material.BlendOperation; break;
						case EMF_POLYGON_OFFSET:
							material.PolygonOffsetDirection = Material.PolygonOffsetDirection;
							material.PolygonOffsetFactor = Material.PolygonOffsetFactor; break;
						}
					}
				}
			}
		}

	};
	//渲染目标
	//将一个渲染结果作为一个纹理，再用此纹理再进行渲染
	struct IRenderTarget
	{
		IRenderTarget(ITexture* texture,
			E_COLOR_PLANE colorMask = ECP_ALL,
			E_BLEND_FACTOR blendFuncSrc = EBF_ONE,
			E_BLEND_FACTOR blendFuncDst = EBF_ONE_MINUS_SRC_ALPHA,
			E_BLEND_OPERATION blendOp = EBO_NONE) :
			RenderTexture(texture),
			TargetType(ERT_RENDER_TEXTURE), ColorMask(colorMask),
			BlendFuncSrc(blendFuncSrc), BlendFuncDst(blendFuncDst),
			BlendOp(blendOp) {}
		IRenderTarget(E_RENDER_TARGET target,
			E_COLOR_PLANE colorMask = ECP_ALL,
			E_BLEND_FACTOR blendFuncSrc = EBF_ONE,
			E_BLEND_FACTOR blendFuncDst = EBF_ONE_MINUS_SRC_ALPHA,
			E_BLEND_OPERATION blendOp = EBO_NONE) :
			RenderTexture(0),
			TargetType(target), ColorMask(colorMask),
			BlendFuncSrc(blendFuncSrc), BlendFuncDst(blendFuncDst),
			BlendOp(blendOp) {}
		bool operator!=(const IRenderTarget& other) const
		{
			return ((RenderTexture != other.RenderTexture) ||
				(TargetType != other.TargetType) ||
				(ColorMask != other.ColorMask) ||
				(BlendFuncSrc != other.BlendFuncSrc) ||
				(BlendFuncDst != other.BlendFuncDst) ||
				(BlendOp != other.BlendOp));
		}
		//渲染纹理
		ITexture* RenderTexture;
		E_RENDER_TARGET TargetType : 8;
		E_COLOR_PLANE ColorMask : 8;
		E_BLEND_FACTOR BlendFuncSrc : 4;
		E_BLEND_FACTOR BlendFuncDst : 4;
		E_BLEND_OPERATION BlendOp : 4;
	};

}



#endif
