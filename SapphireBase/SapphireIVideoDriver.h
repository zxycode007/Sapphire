#ifndef _SAPPHIRE_VIDEO_DRVIER_
#define _SAPPHIRE_VIDEO_DRVIER_


#include "SapphireExposedVideoData.h"
#include "SapphireEMaterialFlags.h"
#include "SapphireEDriverFeatures.h"
#include "SapphireEPrimitiveTypes.h"
#include "SapphirePrerequisites.h"
#include "SapphireSLight.h"
#include "SapphireSMaterial.h"
#include "SapphireIMaterialRenderer.h"
#include "SapphireRectangle.h"
#include "SapphireIAttributes.h"
#include "SapphireIImageLoader.h"
#include "SapphireIImageWriter.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireIMesh.h"
#include "SapphireIGPUProgramServices.h"
#include "SapphireIMeshManipulator.h"
#include "SapphireISceneNode.h"



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

	//���ǲ�������
	struct SOverrideMaterial
	{
		//! ����
		SMaterial Material;
		//! ���ڸ��ǵ�E_MATERIAL_FLAGSֵ��������'|'���Ӷ��
		UINT32 EnableFlags;
		//! ������Ⱦͨ���Ĳ��ʸ��Ǵ��ڼ���״̬����Դ��E_SCENE_NODE_RENDER_PASS��ֵ��������'|'���Ӷ��
		UINT32 EnablePasses;
		//! ȫ�ֿ�����־, ͨ��SceneManager��д��ÿһ����Ⱦͨ��
		//������ǲ����ڵ�ǰͨ���򿪣�������������EnablePass���鲢������EnabledΪtrue
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
	//��ȾĿ��
	//��һ����Ⱦ�����Ϊһ���������ô������ٽ�����Ⱦ
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
		//��Ⱦ����
		ITexture* RenderTexture;
		E_RENDER_TARGET TargetType : 8;
		E_COLOR_PLANE ColorMask : 8;
		E_BLEND_FACTOR BlendFuncSrc : 4;
		E_BLEND_FACTOR BlendFuncDst : 4;
		E_BLEND_OPERATION BlendOp : 4;
	};


	class IVideoDriver //: public virtual IReferenceCounted
	{
	public:

		//! Ӧ�ó����ڽ����κ���Ⱦǰ��������������
		/** ��������ܹ���պ󱸺�z������
		\param backBuffer ָ���󱸻������Ƿ�Ҫ�����������ζ����ָ������ɫ�����Ļ��
		���������false������󱸻����������ᱻ���������ɫ������Ч��
		\param zBuffer ָ���Ƿ������Ȼ�������z���壩.��������2d��Ⱦʱ������Ҫ��ô��
		\param color �����ɫ���ں󱸻�����������
		\param videoData �������ڵľ�����������Ҫ���λͼ��ʾ���������ڵĻ���
		�������һ����Ԫ�أ�ÿ������������ʾ��Ĭ�ϵĴ����С�ע�⣺��������������е�Ӳ��֧�֡�
		\param sourceRect ָ��һ����ΪҪ��ʾ��Դ���������ָ�롣����Ϊ��չʾ���ж�����ע��:���������豸��֧��
		\return ���ʧ�ܷ���false */
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0) = 0;

		//! ����Ⱦ���ͼ����ʾ����Ļ
		/** Ӧ�ó������κ���Ⱦ�������������������
		\return ����ɹ�����true�����򷵻�false */
		virtual bool endScene() = 0;

		//! ��ѯ��������
		/** ���һ��������Ч������true
		\param feature Ҫ��ѯ������
		\return ��������Ч����true�����򷵻�false. */
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const = 0;

		//! �ر�һ������������
		/** Ҳ�ܹ��ôκ�����һ�δ������������ܴ򿪲�֧�ֵ����ԡ�
		\param feature �رյ�����
		\param flag �������־Ϊtue�ǹرգ�false�Ǵ�*/
		virtual void disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag = true) = 0;

		//! ��ȡʵ����Ƶ����������
		/** �����ǿ��Բ�ѯ�ĸ�������
		MaxTextures (int) ������ͬʱ֧�ֵ�������������� SAPPHIRE_MAX_TEXTURE_LAYERS ��ƥ�������ֵ
		MaxSupportedTextures (int) Ӳ�������Ĺ̶���Ⱦ�����ܹ�֧�����������������ʵ��֧�ֵ������������͡�
		MaxLights (int) �����Ĺ̶���ϵ����֧�ֵ�����Դ����ͨ��6-8�����ù�Դ���������߸�����shader������
		MaxAnisotropy (int) ���������˲�����֧����������� ����λ1�����ͨ����16��32
		MaxUserClipPlanes (int) �������ƽ������ �����û�ͨ��ָ����������������
		MaxAuxBuffers (int) �ض�����Ⱦ���壬���ڵ�ǰ���沢�����á�ֻ֧��OpenGL
		MaxMultipleRenderTargets (int) ����ͬʱ���ڵ���ȾĿ������ͨ��shader��Ⱦ��MRT
		MaxIndices (int) ������һ����Ⱦ�����е�������(i.e. һ�����񻺳���).
		MaxTextureSize (int) һ���������ߴ�
		MaxGeometryVerticesOut (int) geometry shader ��һ��ͨ���Ķ�����  ֻ֧��OpenGL
		MaxTextureLODBias (float) LOD ƫ������ֵ  ͨ����16���ң�ĳЩϵͳ���Ը���
		Version (int) �����汾. Ӧ����Major*100+Minor
		ShaderLanguageVersion (int) shader����İ汾  Major*100+Minor.
		AntiAlias (int) ������ÿ�����صĲ��������� 0��1��ζ�ſ���ݹرգ�ͨ��ֵ��2,4,8,16,32
		*/
		virtual const  IAttributes& getDriverAttributes() const = 0;

		//! �����������Ƿ��������
		/** ����D3D�豸������������ã��㽫��Ҫ���´���RTT�� Ӧ����beginScene()���ѯ��
		*/
		virtual bool checkDriverReset() = 0;

		//! ���ñ任����
		/** \param state Ҫ���б任������ �۲�/����/ͶӰ
		\param mat �����任�ľ��� */
		virtual void setTransform(E_TRANSFORMATION_STATE state, const  Matrix4& mat) = 0;

		//! ����setTransform���õı任����
		/** \param state ��ѯ�任������
		\return �����任�ľ��� */
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const = 0;

		//! ����ͼ�����������
		/** \return ͼ�������������*/
		virtual UINT32 getImageLoaderCount() const = 0;

		//! ���ظ�����ͼ�������
		/** \param n ���ؼ����������������������һ������0����������
		\return ָ�������������ָ�룬 ���Ϊ0��������*/
		virtual IImageLoader* getImageLoader(UINT32 n) = 0;

		//! ����ͼ��д����
		/** \return ͼ��д���������� */
		virtual UINT32 getImageWriterCount() const = 0;

		//! ���ظ���ͼ���д����
		/** \param n Ҫ����д���������������������һ������0����������
		\return ָ�����д������ָ�룬 ���Ϊ0�������� */
		virtual IImageWriter* getImageWriter(UINT32 n) = 0;

		//! ����һ������
		/** ����3d���ƺ�������������ʻ��Ƽ�����
		\param material: ��������Ҫ�õĲ��� */
		virtual void setMaterial(const SMaterial& material) = 0;

		//! ��ȡһ������������
		/** �������û�б����غʹ���mipmap�Ļ�����Ҫʱ�Ӵ����ϼ�������
		ʹ��setTextureCreationFlag()������Ӱ�쵽����ļ��ء������������ж��ָ�ʽ��
		��BMP, JPG, TGA, PCX, PNG, ��PSD.
		\param filename Ҫ���ص������ļ���
		\return �����ָ�룬 ���Ϊ0����������ᱻ���ء�  */
		virtual ITexture* getTexture(const  path& filename) = 0;

		//! �һ������������Get access to a named texture.
		/** �������û�б����غʹ���mipmap�Ļ�����Ҫʱ�Ӵ����ϼ�������
		ʹ��setTextureCreationFlag()������Ӱ�쵽����ļ��ء������������ж��ָ�ʽ��
		��BMP, JPG, TGA, PCX, PNG, ��PSD.
		\param file �Ѿ��򿪵��ļ�ָ��
		\return �����ָ�룬 ���Ϊ0����������ᱻ���ء� */
		virtual ITexture* getTexture( IReadFile* file) = 0;

		//! ͨ����������һ������
		/** \param index: ���������������С��getTextureCount()��
		��ע�⣺������Կ�������ӻ��ƶ�����ʱ�ı�
		\return �����ָ�룬��������������û�����û�������������ΧΪ0�� */
		virtual ITexture* getTextureByIndex(UINT32 index) = 0;

		//! ���ص�ǰ���ص���������
		/** \return ��ǰ���ص��������� */
		virtual UINT32 getTextureCount() const = 0;

		//! ������һ������
		/** \param texture ָ��Ҫ�����������ָ��
		\param newName ����������֡���Ӧ����Ψһ�� */
		virtual void renameTexture(ITexture* texture, const  path& newName) = 0;

		//! ����һ��ָ����С�Ŀ�����
		/** \param size: ����Ĵ�С
		\param name һ����������֡� ֮�����getTexture()ͨ��������ַ����������
		\param format ��Ҫ��������ɫ��ʽ����ע�⣺ȡ�ÿ���ѡ�񴴽������������ɫ��ʽ��
		\return ָ���´��������ָ�� */
		virtual ITexture* addTexture(const  dimension2d<UINT32>& size,
			const  path& name, ECOLOR_FORMAT format = ECF_A8R8G8B8) = 0;

		//! ��һ��IImage����һ������
		/** \param name һ����������֡� ֮�����getTexture()ͨ��������ַ����������
		\param image �������������ͼ��
		\param mipmapData ��ѡ�����ù���mipmap���ϵ�ָ�롣�����ͼ����ͬ������ɫ���͡�����������û�и�����
		���mipmap��ͼ�����ɡ�
		\return ָ���´��������ָ�� */
		virtual ITexture* addTexture(const  path& name, IImage* image, void* mipmapData = 0) = 0;

		//! ���һ���µ���ȾĿ�������������� 
		/** \param size ����Ĵ�С����λ���ء���Ⱥ͸߶�Ӧ��Ϊ2��ָ���ݣ�64��128��256��512...��
		���������ܹ��Ⱥ󱸻�����������Ϊ��������Ȼ���������Ļ������
		\param name һ����ѡ��RTT��
		\param format ��ȾĿ�����ɫ��ʽ��֧�ָ����ʽ
		\return ���ش��������ָ�룬�������û�б�����������0�� */
		virtual ITexture* addRenderTargetTexture(const  dimension2d<UINT32>& size,
			const  path& name = "rt", const ECOLOR_FORMAT format = ECF_UNKNOWN) = 0;

		//! �����������Ƴ�һ��������ɾ������
		/** ������������ͷŵ������ڴ档��ע�������������Ժ�ITexture��ָ�벻����Ч��
		\param texture ������ɾ��������*/
		virtual void removeTexture(ITexture* texture) = 0;

		//! �����������Ƴ����е�������ɾ������
		/** ������������ͷŵ������ڴ档��ע�������������Ժ�ITexture��ָ�벻����Ч��  */
		virtual void removeAllTextures() = 0;

		//! �Ƴ�Ӳ�����񻺳���
		virtual void removeHardwareBuffer(const  IMeshBuffer* mb) = 0;

		//! �Ƴ����е�Ӳ��������
		virtual void removeAllHardwareBuffers() = 0;

		//! ����occlusion query�ڱβ�ѯ
		/** �ýڵ�ı�ʶ�������ڱβ��� */
		virtual void addOcclusionQuery( ISceneNode* node,
			const  IMesh* mesh = 0) = 0;

		//! �Ƴ��ڱβ�ѯ
		virtual void removeOcclusionQuery(ISceneNode* node) = 0;

		//! �Ƴ����е��ڱβ�ѯ
		virtual void removeAllOcclusionQueries() = 0;

		//! �����ڱβ�ѯ�� �����������ڲ�ѯ��
		/** ����������Ӧ�ñ���Ⱦ�ɼ����ø��ǲ��ʹر���ɫ����Ȼ�����*/
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false) = 0;

		//! �������е��ڱβ�ѯ�������������ڲ�ѯ��
		/** ����������Ӧ�ñ���Ⱦ�ɼ����ø��ǲ��ʹر���ɫ����Ȼ����� */
		virtual void runAllOcclusionQueries(bool visible = false) = 0;

		//! �����ڱβ�ѯ�� ��GPU���ز�ѯ���
		/** �����ѯ���赲�� ���ñ�־Ϊfalse
		��������¸��¿��ܲ��ᷢ��*/
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true) = 0;

		//! �������нڵ��ڱβ�ѯ�� ��GPU���ز�ѯ���
		/** �����ѯ���赲�� ���ñ�־Ϊfalse
		��������¸��¿��ܲ��ᷢ�� */
		virtual void updateAllOcclusionQueries(bool block = true) = 0;

		//! ���ز�ѯ���
		/** ����ֵ�ǿɼ�������/Ƭ�ε����������ֵ��һ����ȫ�Ľ��Ʊƽ����ܹ�����ʵ�ʵ�����ֵ
		  */
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const = 0;

		//! ����������һ������ɫ���Ĳ���alphaͨ��
		/** ��ʹ������ɫ������ɫ������������ȫ͸����������draw2DImageʹ��useAlphachannel==true��
		\param texture Ҫ�޸�alphaͨ��������
		\param color ɫ����ɫ��ÿ���������ɫ������Ԫ�ض�������ȫ͸��.��ע�⣺����������ɫ
		�ڼ�������ʱ����ܱ����ǣ�������ɫֵ�������ͼƬ�༭�������в�һ����ȷ����ͬ��Ϊ����������⣬��
		����ʹ��makeColorKeyTexture��������ʹ��һ�����ص�λ�����һ����ɫ��ֵ
		\param zeroTexels �������Ϊtrue����������ô�κ��������ؽ������ǵ���ɫƥ�����ɫ���������ǵ�alphaһ����
		����Ϊ0��ɫ��  */
		virtual void makeColorKeyTexture(ITexture* texture,
			ColourValue color,
			bool zeroTexels = false) const = 0;

		//! �������ϻ���һ����ɫ��λ��������һ������alphaͨ��
		/**��ʹ������ɫ������ɫ������������ȫ͸����������draw2DImageʹ��useAlphachannel==true��
		��������Ԫ�ص�alpha���ᱻ�޸�
		\param texture Ҫ�޸�alphaͨ��������
		\param colorKeyPixelPos Ҫʹ��ɫ����ɫ������λ��. ÿ���������ص������ɫ�������ȫ͸����
		\param zeroTexels \�������Ϊtrue����������ô�κ��������ؽ������ǵ���ɫƥ�����ɫ���������ǵ�alphaһ����
		����Ϊ0��ɫ�� */
		virtual void makeColorKeyTexture(ITexture* texture,
			Vector2 colorKeyPixelPos,
			bool zeroTexels = false) const = 0;

		//! ��һ���߳�ͼ������һ��������ͼ
		/** ���Ŀ������32λ�� �߶�ֵ��Ϊ���ӱ����������alpha���������ֵ����EMT_PARALLAX_MAP_SOLID���ʺ����Ƶ�����
		\param texture Ҫ�޸�alphaͨ��������
		\param amplitude ���Ը߶���Ϣ�ĳ���*/
		virtual void makeNormalMapTexture(ITexture* texture, Real amplitude = 1.0f) const = 0;

		//! ����һ������ȾĿ��
		/** 
		ֻ��������֧��EVDF_RENDER_TO_TARGET�������ܹ����� ��������queryFeature()��ѯ��
		ͨ������Ⱦ��������ͨ������ķ�����
		// ������ȾĿ��
		ITexture* target = driver->addRenderTargetTexture(core::dimension2d<UINT32>(128,128), "rtt1");

		// ...

		driver->setRenderTarget(target); // ������ȾĿ��
		// .. �������������
		driver->setRenderTarget(0); // ���û�֮ǰ����ȾĿ��

		��ע�⣺�㲻��ͬʱ����Ⱦ�����������ȾĿ���ͬʱ������ȾĿ����Ⱦ3D��2D��������Ϊ���������ȾĿ����Ϊ����
		��ͨ��ֻ������IVideoDriver::beginScene()��endScene()�������ù�������Ⱦһ�������������Ϊ0�������õ����
		һ��setRenderTarget()����ǰ��һ����ȾĿ�ꡣ
		\param clearBackBuffer �������ɫ���������������ȾĿ��ĺ󱸻�����
		\param clearZBuffer ���������ȾĿ���Z������
		ע�⣺��Ϊ֡�������������ȾĿ�깲��Z�������� ����z���������ܻ���Ϊ�������������
		\param color �����ȾĿ��ĺ󱸻�����
		\return ����ɹ�����true������false*/
		virtual bool setRenderTarget( ITexture* texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! ���û�λ�ض�����ȾĿ��
		/** ��������򿪷����ض�����ɫ�������������建��������������
		\param target �����������ö��ֵ
		\param clearTarget �Ƿ���ָ������ɫ�������Ŀ�껺����
		\param clearZBuffer ���������ȾĿ���Z������
		ע�⣺��Ϊ֡�������������ȾĿ�깲��Z�������� ����z���������ܻ���Ϊ�������������
		\param color ��ȾĿ��ı���ɫ
		\return ����ɹ�����true������false */
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget = true,
			bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! �����µĶ���ȾĿ��
		virtual bool setRenderTarget(const vector<IRenderTarget>& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! ����һ���µ��ӿ�
		/** ÿһ����Ⱦ����������һ���µ������ڽ���
		\param area: ����Ϊ��Ⱦ������������ľ���*/
		virtual void setViewPort(const rect<SINT32>& area) = 0;

		//! ��ȡ��ǰ�ӿ�����
		/** \return ��ǰ�ӿ����� */
		virtual const rect<SINT32>& getViewPort() const = 0;

		//! ����һ������ͼԪ�б�
		/** ע��, �����������ͣ�һЩ������ܲ���ͨ�������б���ʵ�������Ϊ16λ����������65535������
		��ע�⣺��ǰ�������е�ͼԪ�������е���������Ч�� ��Ч���ܿ���ֻ��ͨ����������Ⱦ��ģ�⡣
		\param vertices ���������ָ��
		\param vertexCount ������Ķ��������
		\param indexList �������������ָ�롣������ÿ��ͼԪ���õ���һЩ����.
		����pTypeͼԪ���ͣ��������͵������󣨵㣩��һ�ԣ��߶Σ��������������Σ������ı���
		\param primCount ͼԪ����
		\param vType ��������   ����S3DVertex EVT_STANDARD
		\param pType ͼԪ����,  �������������δ� EPT_TRIANGLE_FAN
		\param iType ��������,  ����16λ���� EIT_16BIT */
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			E_PRIMITIVE_TYPE pType = EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! ����һ��2d�Ķ���ͼԪ�б�
		/** �����������ͨ��3d�汾��Ƚϣ� ��һ������Ϊ2d��Ⱦ����ģʽ��������ֻ��������x��y���֡�
		�����������ͣ�һЩ������ܲ���ͨ�������б���ʵ�������Ϊ16λ����������65535������
		��ע�⣺��ǰ�������е�ͼԪ�������е���������Ч�� ��Ч���ܿ���ֻ��ͨ����������Ⱦ��ģ�⡣
		\param vertices ���������ָ��
		\param vertexCount ������Ķ��������
		\param indexList �������������ָ�롣������ÿ��ͼԪ���õ���һЩ����.
		����pTypeͼԪ���ͣ��������͵������󣨵㣩��һ�ԣ��߶Σ��������������Σ������ı���
		\param primCount ͼԪ����
		\param vType ��������   ����S3DVertex EVT_STANDARD
		\param pType ͼԪ����,  �������������δ� EPT_TRIANGLE_FAN
		\param iType ��������,  ����16λ���� EIT_16BIT*/
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			E_PRIMITIVE_TYPE pType = EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! ����һ�����������������б�
		/** ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount ��������������������/3*/
		void drawIndexedTriangleList(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
		}

		//! ����һ�����������������б�
		/** ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount ��������������������/3*/
		void drawIndexedTriangleList(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, EPT_TRIANGLES, EIT_16BIT);
		}

		//!����һ�����������������б�
		/** ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount ��������������������/3 */
		void drawIndexedTriangleList(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS, EPT_TRIANGLES, EIT_16BIT);
		}

		//! ����һ�������������������ʹ�
		/**  ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount �������������������� - 2 */
		void drawIndexedTriangleFan(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! ����һ�������������������ʹ�
		/**  ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount �������������������� - 2 */
		void drawIndexedTriangleFan(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! ����һ�������������������ʹ�
		/**  ע�⣺�����֧��65536�����㣬��Ϊ�����б���һ��16λ����ÿ�����ֵ��65535.
		�������65535���������б����²���δ���塣
		\param vertices ��������ָ��
		\param vertexCount ��������
		\param indexList ������������ָ��
		\param triangleCount �������������������� - 2 */
		void drawIndexedTriangleFan(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS,  EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! ����һ��3d�߶�
		/** ����ĳЩʵ�֣������������һЩ�����μ򵥵���drawVertexPrimitiveList
        ע�⣺�����߶��õ�ǰ�任����Ͳ��ʻ��ơ������������Ҫ���Ʋ�������ǰ�任��3d�߶�
		�����д��룺
		 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		 
		�ڻ��������߶�ǰ���������ò��ʣ�ĳЩȡ��֧����������������߶κ��
		\param start 3d�߶ε���ʼ
		\param end 3d�߶ν���
		\param color 3d�߶ε���ɫ */
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end, ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//!����һ��3d������
		/** �����������ĳЩ�����ε���drawVertexPrimitiveList
		��������ܱ���������֧�֣���Ϊ���򵥵���drawVertexPrimitiveList, 
		�����������Ǻܿ졣
    
		ע�⣺����������õ�ǰ�任����Ͳ��ʻ��ơ������������Ҫ���Ʋ�������ǰ�任��3d������
		�����д��룺
		 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		�ڻ������������ǰ���������ò���
		\param triangle Ҫ���Ƶ�������.
		\param color �����ε���ɫ */
		virtual void draw3DTriangle(const triangle3df& triangle,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! ����һ��3D AABB����
		/** ����������ں��ӵı߼򵥵���draw3DLine 
		ע�⣺��������õ�ǰ�任����Ͳ��ʻ��ơ������������Ҫ���Ʋ�������ǰ�任�ĺ���
		�����д��룺 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		 �ڻ����������ǰ���������ò���
		 
		\param box Ҫ���Ƶ�AABB����
		\param color AABB���ӵ���ɫ */
		virtual void draw3DBox(const AxisAlignedBox& box,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! ����һ�������κ�����Ч����2dͼ��
		/** \param texture ʹ�õ�����ָ��
		\param destPos Ҫ���Ƶ�ͼ�����Ͻ�������Ŀ��2dλ��. */
		virtual void draw2DImage(const  ITexture* texture,
			const  Vector2& destPos) = 0;

		//! ��һ����ɫ����һ��2Dͼ��
		/** 
		�����ɫ����ColourValue(255,255,255,255)������alphaͨ������
		\param texture ʹ�õ�����ָ��
		\param destPos Ҫ���Ƶ�ͼ�����Ͻ�������Ŀ��2dλ��
		\param sourceRect ͼ���Դ����
		\param clipRect ָ����Ҫ���õ���Ļ�ϵľ���
		���ָ���ǿգ����ͼ�񲻱�����
		\param color ���ͼ��Ҫ���Ƶ���ɫ����������ɫ����ColourValue(255,255,255,255)������Ч��
		ע�⣺���alpha������ʹ�ã����alpha����255�����ͼ���͸��
		\param useAlphaChannelOfTexture: ���Ϊtrue��ʹ����������alphaͨ�������������*/
		virtual void draw2DImage(const  ITexture* texture, const Vector2& destPos,
			const  rect<SINT32>& sourceRect, const  rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false) = 0;

		//! ����һ��2dͼ��ļ��ϣ�����������һ����ɫ��alphaͨ��
		/** ���ͼ�������pos��ʼ�����ҽ�������һ��ֱ���ϡ�
		���л��ƶ��ᱻclipRect���ã������Ϊ0�Ļ���������������sourceRects�����ͨ������������ѡ��
		\param texture ʹ�õ�����ָ��
		\param pos Ҫ���Ƶ�ͼ�����Ͻ�������Ŀ��2dλ��
		\param sourceRects ͼ���Դ����
		\param indices �����б���ѡ��ÿ��ʵ��ʹ�õľ���
		\param kerningWidth λ��X��ƫ����
		\param clipRect ָ����Ҫ���õ���Ļ�ϵľ���
		���ָ���ǿգ����ͼ�񲻱�����
		\param color ���ͼ��Ҫ���Ƶ���ɫ����������ɫ����ColourValue(255,255,255,255)������Ч��
		ע�⣺���alpha������ʹ�ã����alpha����255�����ͼ���͸��
		\param useAlphaChannelOfTexture: ���Ϊtrue��ʹ����������alphaͨ������������� */
		virtual void draw2DImageBatch(const ITexture* texture,
			const Vector2& pos,
			const vector<rect<SINT32> >& sourceRects,
			const vector<SINT32>& indices,
			SINT32 kerningWidth = 0,
			const  rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! ����һ��2dͼ��ļ��ϣ�����������һ����ɫ��alphaͨ��
		/** ȫ�������ƶ������õ�һ����������clipRect�������Ϊ0�Ļ���
		�����������sourceRects�������ʹ�����λ�������λ�ö���
		\param texture ʹ�õ�����ָ��
		\param positions Ҫ���Ƶ�ͼ�����Ͻ�������Ŀ��2dλ��
		\param sourceRects ͼ���Դ����
		\param clipRect ָ��Ҫ��ʾ����Ļ�ϵļ�������ͼ��ᱻ�˼���
		���Ϊ0����ͼ�񲻻ᱻ����
		\param color ���ͼ��Ҫ���Ƶ���ɫ����������ɫ����ColourValue(255,255,255,255)������Ч��
		ע�⣺���alpha������ʹ�ã����alpha����255�����ͼ���͸��
		\param useAlphaChannelOfTexture: ���Ϊtrue��ʹ����������alphaͨ������������� */
		virtual void draw2DImageBatch(const ITexture* texture,
			const vector<Vector2 >& positions,
			const vector<rect<SINT32> >& sourceRects,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! ��������������һ���ֵ���������С�ע�����Ҫʹ����ɫ�Ļ���������һ��4����ɫ������
		/** 
		\param texture Ҫ���Ƶ�����
		\param destRect ͼ��Ҫ������ľ���
		\param sourceRect ������α�ʾΪ�����һ����
		\param clipRect ����Ŀ�����(������0) 
		\param colors 4����ɫ�������ʾĿ�������ĸ��ǵ���ɫֵ
		\param useAlphaChannelOfTexture ���Ϊtrue��alphaͨ��������� */
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue * const colors = 0, bool useAlphaChannelOfTexture = false) = 0;

		//! ����һ��2D ����
		/** \param color Ҫ���Ƶľ��ε���ɫ�����alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��
		\param pos ���ε�λ��
		\param clip ָ���������������εľ��Σ����ΪNULL�����������
		 */
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos,
			const rect<SINT32>* clip = 0) = 0;

		//! ����һ�������2d����
		/** \param colorLeftUp ���Ͻǵ���ɫ
		���alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��
		\param colorRightUp ���Ͻǵ���ɫ
		���alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��
		\param colorLeftDown ���½ǵ���ɫ
		���alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��
		\param colorRightDown ���½ǵ���ɫ
		���alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��
		\param pos ���ε�λ��
		\param clip ָ���������������εľ��Σ����ΪNULL����������� */
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp,
			ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0) = 0;

		//! ����һ�����ߵ�2d����
		/** \param pos ���ε�λ��
		\param color ���ε���ɫ
		���alpha�������ᱻ��������ָ����������Ƿ�Ӧ��͸��*/
		virtual void draw2DRectangleOutline(const recti& pos,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! ����һ��2d�߶�
		/** \param start ָ������Ļ����ʼ����
		\param end ֱ������Ļ�Ľ�������
		\param color Ҫ���Ƶ���ɫ */
		virtual void draw2DLine(const Vector2& start,
			const Vector2& end,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! ����һ������
		/** \param x ������ص�Xλ��
		\param y ������ص�Yλ��
		\param color ������ص���ɫ */
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue& color) = 0;

		//! ����һ�������Ĺ�Բ�����2d�����
		/** ����������ڻ���Բ��������Ҳ�ܹ����������Σ��Ľ��ͣ������
		,6���Σ�7����.... 
		\param center Բ��������λ�ã����أ�
		\param radius Բ�����ذ뾶
		\param color Բ������ɫ
		\param vertexCount �������εĶ�����Ŀ����������Բ���̶�
		*/
		virtual void draw2DPolygon(Vector2 center,
			Real radius,
			ColourValue color = ColourValue(100, 255, 255, 255),
			SINT32 vertexCount = 10) = 0;

		//! ����һ����Ӱ�ݻ���ģ�建����
		/** ����һ��ģ����Ӱ�����ȣ��������м����壬Ȼ��ʹ���������������Ӱ�ݻ���
		��IVideoDriver::drawStencilShadow()ȥ���ӻ���Ӱ��
		��ע�⣺��ֻ����openGL�汾��
		
		\param triangles 3d�������飬ָ����Ӱ�ݻ�
		\param zfail�������Ϊtrue��zfail�ķ����ᱻʹ�ã�����ʹ��zpass
		\param debugDataVisible debug���ݶ��������Ӱ�ڵ�Ŀ��� 
		*/
		virtual void drawStencilShadowVolume(const vector<Vector3>& triangles, bool zfail = true, UINT32 debugDataVisible = 0) = 0;

		//! ����ɫ���ģ����Ӱ
		/** IVideoDriver::drawStencilShadowVolume()��Ӱ�ݻ��Ѿ�����ӵ�ģ�建������
		���������������Ӱ����ɫ��
		��ע�⣺��ֻ����openGL�汾��
		\param clearStencilBuffer ����Ϊflase�������ֻ��ͬһ����ɫ����ÿһ����Ӱ��
		��ôֻ����drawStencilShadow()�����е���Ӱ�ݻ��Ķ������ơ��������ÿ����Ӱ
		��Ҫ���Լ�����ɫ���������λtrue��
		\param leftUpEdge ��Ӱ���Ͻǵ���ɫ
		\param rightUpEdge ��Ӱ���Ͻǵ���ɫ
		\param leftDownEdge ��Ӱ���½ǵ���ɫ
		\param rightDownEdge ��Ӱ���½ǵ���ɫ
		 */
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(255, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(255, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(255, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(255, 0, 0, 0)) = 0;

		//! ����һ�����񻺳���
		/** \param mb Ҫ�ػ��Ƶ����񻺳��� */
		virtual void drawMeshBuffer(const IMeshBuffer* mb) = 0;

		//! ����һ������ķ��߻�����
		/** \param mb ���ߵĻ�����
		\param length ���ߵĳ�����������
		\param color ������Ⱦ�ķ�����ɫ
		*/
		virtual void drawMeshBufferNormals(const IMeshBuffer* mb, Real length = 10.f, ColourValue color = ColourValue(1.0,1.0,1.0,1.0)) = 0;

		//! ��������ģʽ
		/** ��Щ����ӵ�ÿһ��3d������Ⱦȫ��ֵ���������Ĳ����������־
		\param color ����ɫ
		\param fogType ��������
		\param start ֻ������������ģʽ (linearFog=true).ָ������ʼ	
		\param end ֻ������������ģʽ(linearFog=true).ָ�����Ľ���
		\param density ֻ������ָ����ģʽ��������0��1֮��
		\param pixelFog ����vertex fog����������Ϊfalse, ���������������Ϊtrue
		\param rangeFog �������Ϊtrue�򿪻��ڷ�Χ��������������Ǵӹ۲���������������Z���ꡣ�������ã����ǻ������
		����ܲ�������������֧�����������
		 */
		virtual void setFog(ColourValue color = ColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false) = 0;

		//! ��ȡ��ģʽ
		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog) = 0;

		//! ��ȡ��ǰ��ɫ����������ɫģʽ
		/** \return ��ɫ����������ɫģʽ */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! ��ȡ��Ⱦ���ڵ����
		/** \return ������Ⱦ���ڵ���� */
		virtual const dimension2d<UINT32>& getScreenSize() const = 0;

		//! ��ȡ��ǰ��ȾĿ��Ĵ�С
		/** ���������֧����Ⱦ���������ߵ�ǰ��ȾĿ�������Ļ�Ļ�����������ᷭ����Ļ��С��
		\return ��ȾĿ��/��Ļ/���ڵĴ�С */
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const = 0;

		//! ��ȡ��ǰÿ��֡��ֵ
		/** ���ֵ����ÿ1.5�����һ�εĽ���ֵ����ֻ���ṩһ�����Ե�ƽ��֡���ο���
		����������������ʱ����������֡���ʵ��ƶ�
		\return Approximate ���Ƶ�ÿ��֡�� */
		virtual SINT32 getFPS() const = 0;

		//! �������һ֡���Ƶ�ͼԪ������һ���������Σ�
		/** ��getFPS����һ���ǳ����õľ�̬����
		\param mode ����ͼԪ�����Ƿ񱻼������ÿһ֡�м���
		\return ����һ֡�Ļ��Ƶ�ͼԪ�� */
		virtual UINT32 getPrimitiveCountDrawn(UINT32 mode = 0) const = 0;

		//! ɾ������֮ǰ��addDynamicLight������ӵĵĶ�̬��Դ 
		virtual void deleteAllDynamicLights() = 0;

		//! ���һ����̬��Դ�����������Դ������
		//! \param light: �������Դ�����ݴ��������Դ
		//! \return һ����Դ����������-1 ��������
		virtual SINT32 addDynamicLight(const SLight& light) = 0;

		//! �����豸��֧�ֵ����Ķ�̬��Դ����
		/** \return ���̬��Դ��Ŀ */
		virtual UINT32 getMaximalDynamicLightAmount() const = 0;

		//! ���ص�ǰ���õĶ�̬��Դ����
		/** \return ��ǰ���õĶ�̬��Դ���� */
		virtual UINT32 getDynamicLightCount() const = 0;

		//! ����֮ǰIVideoDriver::addDynamicLight()���õĹ�Դ����
		/** \param idx ��Դ����������0��������0��С��IVideoDriver::getDynamicLightCount.
		\return ��Դ������. */
		virtual const SLight& getDynamicLight(UINT32 idx) const = 0;

		//! ��һ����̬��Դ���߹ر�
		//! \param lightIndex: addDynamicLight��ӵĹ�Դ����
		//! \param turnOn: ���Ϊtrue����Դ�򿪣���֮�ر�
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn) = 0;

		//! ��ȡ��Ƶ������
		/** \return ������Ƶ������������Direct3D8��"Direct3D 8.1" */
		virtual const wchar_t* getName() const = 0;

		//! ���һ���ⲿ��ͼ����������������
		/** 
		������������ص�ǰ��֧�ֵ������ļ���ʽ��ֻ��ʵ�ֶ�Ӧ�ļ���IImageLoader
		һ��ָ��ָ���������ʽ��IImageLoaderʵ�֣����ݸ����档
		\param loader ָ���ⲿ��������ָ�� */
		virtual void addExternalImageLoader(IImageLoader* loader) = 0;

		//! ���һ���ⲿͼ��д�������������
		/** ����������д�뵱ǰ��֧�ֵ������ļ���ʽ��ֻ��ʵ�ֶ�Ӧ�ļ���IImageWriter
		һ��ָ��ָ���������ʽ��IImageWriterʵ�֣����ݸ����档
		\param writer: ָ���ⲿд������ָ��. */
		virtual void addExternalImageWriter(IImageWriter* writer) = 0;

		//! ����ͼԪ���������
		/** (���������)����豸����ͨ������һ��drawVertexPrimitiveList������Ⱦ
		\return ͼԪ��������� */
		virtual UINT32 getMaximalPrimitiveCount() const = 0;

		//! �򿪺͹ر�һ����������־
		/** �����־�����������δ�����ͨ���ı����ֵ������Ӱ��������Ⱦ�ٶȡ�
		����Ҫע�⣺��Ƶ���������ֵֻ���Ƽ����������ETCF_ALWAYS_16_BIT��������
		��Ȼ����32λ����
		\param flag ��������־
		\param enabled ָ�������ı�־�Ƿ�Ӧ�ô򿪻�ر� */
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled = true) = 0;

		//! ����һ�������־�Ĵ򿪻�ر�״̬
		/** ����ͨ�� setTextureCreationFlag().����
		\param flag ��������־
		\return ��ǰ�����־�Ĵ�״̬ */
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const = 0;

		//! ��һ���ļ�����һ�����ͼ��
		/** û��Ӳ��������Ҫ�����ͼ�񴴽�������������������ȡ�ر���Ⱦ���ĸ߳�ͼ
		\param filename �������ͼ����ļ���
		\return ������ͼ��
		 */
		virtual IImage* createImageFromFile(const path& filename) = 0;

		//! ��һ���ļ�����һ�����ͼ��
		/** û��Ӳ��������Ҫ�����ͼ�񴴽�������������������ȡ�ر���Ⱦ���ĸ߳�ͼ
		\param file �������ͼ����ļ���
		\return ������ͼ��
		 */
		virtual IImage* createImageFromFile(IReadFile* file) = 0;

		//! д��ͼ���ļ�
		/** ��Ҫע��һ�����ʵ�ͼ��д����
		\param image д���ͼ��
		\param filename д����ļ���
		\param param ��˵Ŀ��Ʋ�����ѹ������
		\return ���д��ɹ�����true */
		virtual bool writeImageToFile(IImage* image, const path& filename, UINT32 param = 0) = 0;

		//! д��ͼ���ļ�
		/** ��Ҫע��һ�����ʵ�ͼ��д����
		\param image д���ͼ��
		\param file  һ���Ѿ��򿪵�io::IWriteFile����. �������Ҫ����ʹ�õ�ͼ��д����
		\param param ��˵Ŀ��Ʋ�����ѹ������
		\return ���д��ɹ�����true. */
		virtual bool writeImageToFile(IImage* image, IWriteFile* file, UINT32 param = 0) = 0;

		//! ���ֽ����鴴��һ�����ͼ��
		/** û��Ӳ��������Ҫ�����ͼ�񴴽�������������������ȡ�ر���Ⱦ���ĸ߳�ͼ
		\param format �����������ɫ��ʽ
		\param size ����ͼ��Ĵ�С
		\param data һ��������ɫ��Ϣ���ֽ�����
		\param ownForeignMemory ���Ϊtrue�� ���ͼ��֮��ֱ�����������ָ�룬�����Ϊfalse��
		�ڴ潫���ڲ�����һ��
		\param deleteMemory �Ƿ��ڴ����������ͷ�
		\return ����ͼ��
		 */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = false,
			bool deleteMemory = true) = 0;

		//! ����һ�������ͼ��
		/**
		\param format ������ɫͼ��ĸ�ʽ
		\param size ����ͼ��Ĵ�С 
		\return ������ͼ��
		  */
		virtual IImage* createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size) = 0;



		//! ��һ�������һ���ִ���һ�����ͼ��
		/**
		\param texture Ҫ����һ���ֵ���ͼ�������
		\param pos Ҫ���ƾ���λ��
		\param size Ҫ���Ƶľ�����չ
		\return ������ͼ��
		  */
		virtual IImage* createImage(ITexture* texture,
			const  Vector2& pos,
			const  dimension2d<UINT32>& size) = 0;

		//! ���¸ı��С�¼��ľ���� ֻ���������ڲ���
		/** �����������������ڸı�
		ͨ�����ⲻ��Ҫ����������� */
		virtual void OnResize(const  dimension2d<UINT32>& size) = 0;

		//! ���һ���µĲ�����Ⱦ�����������
		/** ʹ������������²���������չ��Ƶ�����������������չ���水������������
		��IMaterialRenderer����һ���ಢ����д������Ҫ�ķ�����Ϊ��������ȷ����Ⱦ״̬��
		�������IVideoDriver::getExposedVideoData()��ȡһ��ָ��ʵ����Ⱦ�豸��ָ�룬
		��IVideoDriver::addMaterialRenderer()�������ࡣҪʹһ��������ʾ����²��ʣ�
		������������ķ���ֵ��SMaterial�ṹ�ڵĲ������ͳ�Ա��ֵ�������ֻ�������´���
		��������Vertex Shader��Fragment Shader��ʹ��IGPUProgrammingServices�ӿ��õ���
		getGPUProgrammingServices()�����������ס�
		\param renderer һ������Ⱦ����ָ��
		\param name ������Ⱦ����ڵĿ�ѡ��
		\return �������ͺţ������Ա�����ΪSMaterial::MaterialType������Ⱦ������-1����
		������������Ҫ�������һ��������Ⱦ���������Ⱦ�����߿��豸�����ǲ������
		 */
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer, const c8* name = 0) = 0;

		//! ͨ��������ȡһ��������Ⱦ��
		/** \param idx ������Ⱦ����ID��������E_MATERIAL_TYPEö��ֵ����addMaterialRenderer()�ķ���ֵ
	 
		\return ������Ⱦ��ָ����ǿ�*/
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx) = 0;

		//! ��ȡ��ǰ���õĲ�����Ⱦ������
		/** \return ��ǰ���õĲ�����Ⱦ������*/
		virtual UINT32 getMaterialRendererCount() const = 0;

		//! ��ȡ������Ⱦ����
		/**
		����ַ����ܹ����ڲ���һ���ض�����Ⱦ���Ƿ��Ѿ���ע��򴴽���
		����ʹ������ַ���������صĲ��ʵ����ݣ�������ص����ֽ��ڵ�
		���л�����ʱʹ��
		\param idx ������Ⱦ����ID�� ������E_MATERIAL_TYPEö��ֵ����addMaterialRenderer()�ķ���ֵ
		\return �����Ⱦ�������֣������0�򲻴��� */
		virtual const c8* getMaterialRendererName(UINT32 idx) const = 0;

		//! ����һ��������Ⱦ��������
		/** �������ڽ��Ĳ�������û��Ӱ��
		\param idx: ������Ⱦ����ID�� ������E_MATERIAL_TYPEö��ֵ����addMaterialRenderer()�ķ���ֵ.
		\param name: ���������Ⱦ���������� */
		virtual void setMaterialRendererName(SINT32 idx, const c8* name) = 0;

		//! ��һ�����ʴ������������б�
		/** ��������������л�������
		��ע�⣺��Ƶ������ͨ��getMaterialRendererName()��ȡ������Ⱦ�������������������������
		Ӧ�������ú�����
		\param options ��Ӱ�����л��ĸ���ѡ��
		\return �������ѡ���IAttributes����
	    */
		virtual IAttributes* createAttributesFromMaterial(const SMaterial& material,
			SAttributeReadWriteOptions* options = 0) = 0;

		//! ��һ���������һ��һ��SMaterial�ṹ
		/** ��ע�⣺����������ʵĲ������͵����ã������Ƶ������Ϊ����������ѯ������Ⱦ����
		���ԣ����Է����ò��ʱ����������������ǰ������
		\param outMaterial Ҫ�������ԵĲ���
		\param attributes Ҫ��ȡ������ */
		virtual void fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attributes) = 0;

		//! �������IVideoDriver�������Ͳ���ϵͳָ������ 
		/** ����������չ���޸�����ѡ���룬Ӧ��ʹ���������
		\return �豸����ָ��ļ��� */
		virtual const SExposedVideoData& getExposedVideoData() = 0;

		//! ��ȡ��Ƶ����������
		/** \return ���������� */
		virtual E_DRIVER_TYPE getDriverType() const = 0;

		//! ��ȡIGPUProgrammingServices�Ľӿ�
		/** \return ���� IGPUProgrammingServices��ָ�룬�����Ƶ������֧�ַ���0 */
		virtual IGPUProgrammingServices* getGPUProgrammingServices() = 0;

		//! ����һ��ָ�����������mesh manipulator��ָ��
		virtual IMeshManipulator* getMeshManipulator() = 0;

		//! ���ZBuffer
		/** ע�⣺��ͨ������Ҫ�������������������Z���壬
		����IVideoDriver::beginScene()��IVideoDriver::setRenderTarget()
		���Զ����á��������Ҫ��ȾһЩ�ر�Ķ��������������Ⱦ�����е��κ�
		ʱ���������������Z����
		*/
		virtual void clearZBuffer() = 0;

		//! ����һ��֡����Ľ�ͼ
		/** \return ����һ����һ��֡�����ͼ��*/
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER) = 0;

		//! ���ͼ���Ѿ��Ƿ񱻼���
		/** ͬgetTexture()���ͣ������������û���صĻ�������������������
		\param filename ������ļ���
		\return ָ�������ָ�룬0������ */
		virtual ITexture* findTexture(const path& filename) = 0;

		//! ����һ��δ���õ�һ������ƽ��
		/** ���û���������Ҫ����6����Ч�ļ���ƽ��
		\param index ���ƽ�������������Ϊ0��MaxUserClipPlanes.
		\param plane �������ƽ������
		\param enable ���Ϊtrue���������ƽ�潫���ر�
		\return ����������ƽ�汻�رշ���true */
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false) = 0;

		//! �򿪻�رռ���ƽ��
		/**���û���������Ҫ����6����Ч�ļ���ƽ��
		\param index ���ƽ�������������Ϊ0��MaxUserClipPlanes.
		\param enable ���Ϊtrue���������ƽ�潫���ر� */
		virtual void enableClipPlane(UINT32 index, bool enable) = 0;

		//! Ϊ��Ҫ������Ӳ��������������С������
		/** \param count ������������Сָ�� */
		virtual void setMinHardwareBufferVertexCount(UINT32 count) = 0;

		//! ��ȡȫ�ֲ��ʣ������Ը��Ǿֲ�����
		/** �����򿪱�־��������ʵ�ֵ���ڸ���ĳЩ������Ⱦ�����񻺳����ľֲ�����
		\return ���ǲ��ʵ�Ӧ�� */
		virtual SOverrideMaterial& getOverrideMaterial() = 0;

		//! Ϊ���޸�ֵ����ȡ2d���ǲ���
		/** 
		���2d���ǲ��������޸�2d������������Ⱦ״̬�����������е�SMaterial��Ա�����ʡ�
		�ر��ǲ������ͺ��������⣬zbufferһֱ��Ч�����ҹ���һֱ���ڹرա�����������
		��־����ı䣬��Ȼ�ڴ�����������ЩӰ�졣
		��ע�⣺����ͨ��enableInitMaterial2D()��/�ر����Ч�������Ч�����ıȽϴ�
		�������˿��ǵ���״̬�ı��������ÿ����ô����ʱ��Ҫ�����������ֵ��
		\return ��ӳ�޸ĵ������õĲ������� 
		*/
		virtual SMaterial& getMaterial2D() = 0;

		//! ��2d���ǲ���
		/** \param enable ��־������������Ƿ�Ӧ�ô򿪻�ر� */
		virtual void enableMaterial2D(bool enable = true) = 0;

		//! ��ȡ�Կ���Ӧ������
		virtual String getVendorInfo() = 0;

		//! ֻ���������ڲ�
		/** �ڳ��������������û�������ɫ,��ISceneManager::setAmbientLight()
		\param color �µĻ�������ɫ*/
		virtual void setAmbientLight(const ColourValue& color) = 0;

		//! ֻ���������ڲ�
		/** ����ȫ�ֲ��ʱ�־AllowZWriteOnTransparent.
		�����Ӧ�����ó����������������������ֵ
		\param flag Ĭ����Ϊ�ر�ZWrite */
		virtual void setAllowZWriteOnTransparent(bool flag) = 0;

		//! ��ȡ֧�ֵ���������С
		virtual dimension2du getMaxTextureSize() const = 0;

		//! ��ɫת������ 
		/** ת��һ��ͼ��ģ��������飩��Դ��Ŀ�ģ��Ӷ�ת����ɫ��ʽ��������ش�Сͨ����ɫ��ʽ����
		\param sP Դָ��
		\param sF Դ��ɫ��ʽ
		\param sN ת������������������������㹻��
		\param dP Ŀ��ָ��
		\param dF Ŀ����ɫ��ʽ
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const = 0;
	};

}



#endif
