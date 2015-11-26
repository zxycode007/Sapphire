#ifndef _SAPPHIRE_VIDEO_DRVIER_
#define _SAPPHIRE_VIDEO_DRVIER_

#include "SapphireEMaterialFlags.h"
#include "SapphirePrerequisites.h"

namespace Sapphire
{
	//! ���α任״̬��ö��
	enum E_TRANSFORMATION_STATE
	{
		//! �۲�ռ�任
		ETS_VIEW = 0,
		//! ����ռ�任
		ETS_WORLD,
		//! ͸��ͶӰ�任
		ETS_PROJECTION,
		//! ����任0
		ETS_TEXTURE_0,
		//! ����任1
		ETS_TEXTURE_1,
		//! ����任2
		ETS_TEXTURE_2,
		//! ����任3
		ETS_TEXTURE_3,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>4
		//! ����任4
		ETS_TEXTURE_4,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>5
		//! ����任5
		ETS_TEXTURE_5,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>6
		//!����任6
		ETS_TEXTURE_6,
#if SAPPHIRE_MAX_TEXTURE_LAYERS>7
		//! ����任7
		ETS_TEXTURE_7,
#endif
#endif
#endif
#endif
		 
		ETS_COUNT
	};


	//! ö���ϸ���Ⱦѭ�����ж�������Դ�ź�
	/** ���ֵͨ��������ǣ�����Ӧ�ó���ĳЩ��Դ��Ҫ���������´����� ���潫��ʱ�����´���ʵ�ʵĶ��󣬵������´�����������Ӧ�ó��������
     */
	enum E_LOST_RESOURCE
	{
		//! �ĸ��豸/����Ҫ����
		ELR_DEVICE = 1,
		//! ���е�����Ҫ������ ��������
		ELR_TEXTURES = 2,
		//! ���е���ȾĿ�궼Ҫ������ d3d��������
		ELR_RTTS = 4,
		//! Ӳ�������������������Զ����´�������������һ֡�����Ҫһ��ʱ��
		ELR_HW_BUFFERS = 8
	};


	//! �ر����ȾĿ�꣬ ��ͨ��ӳ�䵽ר�ŵ�Ӳ��
	/** ��Щ��ȾĿ�겻��Ҫ֧��GFX��*/
	enum E_RENDER_TARGET
	{
		//! �����ȾĿ��������ɫ������
		ERT_FRAME_BUFFER = 0,
		//! �����ȾĿ����һ����Ⱦ����
		ERT_RENDER_TEXTURE,
		//! ����ȾĿ������
		ERT_MULTI_RENDER_TEXTURES,
		//! �����ȾĿ��������ɫ֡������
		ERT_STEREO_LEFT_BUFFER,
		//! ��ȾĿ��������ɫ����������������������
		ERT_STEREO_RIGHT_BUFFER,
		//! ͬʱ��Ⱦ����stereo������
		ERT_STEREO_BOTH_BUFFERS,
		//! ���������� 0
		ERT_AUX_BUFFER0,
		//! ���������� 1
		ERT_AUX_BUFFER1,
		//! ���������� 2
		ERT_AUX_BUFFER2,
		//! ���������� 3
		ERT_AUX_BUFFER3,
		//! ���������� 4
		ERT_AUX_BUFFER4
	};


	//�������
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


	struct SOverrideMaterial
	{
		//! The Material values
		SMaterial Material;
		//! Which values are taken for override
		/** OR'ed values from E_MATERIAL_FLAGS. */
		UINT32 EnableFlags;
		//! Set in which render passes the material override is active.
		/** OR'ed values from E_SCENE_NODE_RENDER_PASS. */
		UINT32 EnablePasses;
		//! Global enable flag, overwritten by the SceneManager in each pass
		/** The Scenemanager uses the EnablePass array and sets Enabled to
		true if the Override material is enabled in the current pass. */
		bool Enabled;

	 
		SOverrideMaterial() : EnableFlags(0), EnablePasses(0), Enabled(false) {}

		//!Ӧ�ô򿪸���
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

}



#endif
