#ifndef _SAPPHIRE_SMATERIAL_
#define _SAPPHIRE_SMATERIAL_

#include "SapphireEMaterialTypes.h"
#include "SapphireEMaterialFlags.h"
#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"
#include "SapphireColorValue.h"
#include "SapphireMaterialLayer.h"

namespace Sapphire
{
	class ITexture;

	//! EMT_ONETEXTURE_BLEND �ı�־( �������) BlendFunc = source * sourceFactor + dest * destFactor
	enum E_BLEND_FACTOR
	{
		EBF_ZERO = 0,			//!< src & dest	(0, 0, 0, 0)
		EBF_ONE,					//!< src & dest	(1, 1, 1, 1)
		EBF_DST_COLOR, 				//!< src	(destR, destG, destB, destA)
		EBF_ONE_MINUS_DST_COLOR,	//!< src	(1-destR, 1-destG, 1-destB, 1-destA)
		EBF_SRC_COLOR,				//!< dest	(srcR, srcG, srcB, srcA)
		EBF_ONE_MINUS_SRC_COLOR, 	//!< dest	(1-srcR, 1-srcG, 1-srcB, 1-srcA)
		EBF_SRC_ALPHA,				//!< src & dest	(srcA, srcA, srcA, srcA)
		EBF_ONE_MINUS_SRC_ALPHA,	//!< src & dest	(1-srcA, 1-srcA, 1-srcA, 1-srcA)
		EBF_DST_ALPHA,				//!< src & dest	(destA, destA, destA, destA)
		EBF_ONE_MINUS_DST_ALPHA,	//!< src & dest	(1-destA, 1-destA, 1-destA, 1-destA)
		EBF_SRC_ALPHA_SATURATE		//!< src	(min(srcA, 1-destA), idem, ...)
	};

	//! ����Ͽ���ʱ�����ϲ�����ֵ
	enum E_BLEND_OPERATION
	{
		EBO_NONE = 0,	//!< No blending happens
		EBO_ADD,		//!< Default blending adds the color values
		EBO_SUBTRACT,	//!< This mode subtracts the color values
		EBO_REVSUBTRACT,//!< This modes subtracts destination from source
		EBO_MIN,		//!< Choose minimum value of each color channel
		EBO_MAX,		//!< Choose maximum value of each color channel
		EBO_MIN_FACTOR,	//!< Choose minimum value of each color channel after applying blend factors, not widely supported
		EBO_MAX_FACTOR,	//!< Choose maximum value of each color channel after applying blend factors, not widely supported
		EBO_MIN_ALPHA,	//!< Choose minimum value of each color channel based on alpha value, not widely supported
		EBO_MAX_ALPHA	//!< Choose maximum value of each color channel based on alpha value, not widely supported
	};

	//!�������Ͳ���: e.g. DirectX: D3DTOP_MODULATE, D3DTOP_MODULATE2X, D3DTOP_MODULATE4X
	enum E_MODULATE_FUNC
	{
		EMFN_MODULATE_1X = 1,
		EMFN_MODULATE_2X = 2,
		EMFN_MODULATE_4X = 4
	};

	//! �ȽϺ���, e.g. ������Ȳ���
	enum E_COMPARISON_FUNC
	{
		//! Test never succeeds, this equals disable
		ECFN_NEVER = 0,
		//! <= test, default for e.g. depth test
		ECFN_LESSEQUAL = 1,
		//! Exact equality
		ECFN_EQUAL = 2,
		//! exclusive less comparison, i.e. <
		ECFN_LESS,
		//! Succeeds almost always, except for exact equality
		ECFN_NOTEQUAL,
		//! >= test
		ECFN_GREATEREQUAL,
		//! inverse of <=
		ECFN_GREATER,
		//! test succeeds always
		ECFN_ALWAYS
	};


	//!��Ⱦ�п���/�ر���ɫƽ���ö��ֵ
	enum E_COLOR_PLANE
	{
		//! No color enabled
		ECP_NONE = 0,
		//! Alpha enabled
		ECP_ALPHA = 1,
		//! Red enabled
		ECP_RED = 2,
		//! Green enabled
		ECP_GREEN = 4,
		//! Blue enabled
		ECP_BLUE = 8,
		//! All colors, no alpha
		ECP_RGB = 14,
		//! All planes enabled
		ECP_ALL = 15
	};


	//! ��ȡalphaֵ����Դ
	/** ���ֻ֧���� EMT_ONETEXTURE_BLEND.
	�����ʹ��|����ϣ� alphaֵ���Ա����ƣ������
	*/
	enum E_ALPHA_SOURCE
	{
		//! Use no alpha, somewhat redundant with other settings
		EAS_NONE = 0,
		//! Use vertex color alpha
		EAS_VERTEX_COLOR,
		//! Use texture alpha channel
		EAS_TEXTURE
	};


	//! EMT_ONETEXTURE_BLEND: ��װsrcFact, dstFact, Modulate �� alpha source ���������Ͳ��� MaterialTypeParam
	/** alpha Դ������E_ALPHA_SOURCE��ֵ�ͽ��л����� */
	inline Real pack_textureBlendFunc(const E_BLEND_FACTOR srcFact, const E_BLEND_FACTOR dstFact, const E_MODULATE_FUNC modulate = EMFN_MODULATE_1X, const UINT32 alphaSource = EAS_TEXTURE)
	{
		const UINT32 tmp = (alphaSource << 12) | (modulate << 8) | (srcFact << 4) | dstFact;
		return FR(tmp);
	}


	//! EMT_ONETEXTURE_BLEND: ��� srcFact & dstFact and Modulo ���������Ͳ��� MaterialTypeParam
	inline void unpack_textureBlendFunc(E_BLEND_FACTOR &srcFact, E_BLEND_FACTOR &dstFact,
		E_MODULATE_FUNC &modulo, UINT32& alphaSource, const Real param)
	{
		const UINT32 state = IR(param);
		alphaSource = (state & 0x0000F000) >> 12;
		modulo = E_MODULATE_FUNC((state & 0x00000F00) >> 8);
		srcFact = E_BLEND_FACTOR((state & 0x000000F0) >> 4);
		dstFact = E_BLEND_FACTOR((state & 0x0000000F));
	}

	//! EMT_ONETEXTURE_BLEND: alpha����Ƿ��л������
	inline bool textureBlendFunc_hasAlpha(const E_BLEND_FACTOR factor)
	{
		switch (factor)
		{
		case EBF_SRC_ALPHA:
		case EBF_ONE_MINUS_SRC_ALPHA:
		case EBF_DST_ALPHA:
		case EBF_ONE_MINUS_DST_ALPHA:
		case EBF_SRC_ALPHA_SATURATE:
			return true;
		default:
			return false;
		}
	}


	//! �����־����ָ������ݺ�ƽ��ģʽ
	/** ֧�ֶ��ز���������ƽ������alpha����
	��Щȡ�ò�֧��ÿһ�����ʵĿ�������ã���������У�FSAA/���ز��������豸ģʽSapphireCreationParameters�����������
	*/
	enum E_ANTI_ALIASING_MODE
	{
		//! Use to turn off anti-aliasing for this material
		EAAM_OFF = 0,
		//! Default anti-aliasing mode
		EAAM_SIMPLE = 1,
		//! High-quality anti-aliasing, not always supported, automatically enables SIMPLE mode
		EAAM_QUALITY = 3,
		//! Line smoothing
		EAAM_LINE_SMOOTH = 4,
		//! point smoothing, often in software and slow, only with OpenGL
		EAAM_POINT_SMOOTH = 8,
		//! All typical anti-alias and smooth modes
		EAAM_FULL_BASIC = 15,
		//! Enhanced anti-aliasing for transparent materials
		/** Usually used with EMT_TRANSPARENT_ALPHA_REF and multisampling. */
		EAAM_ALPHA_TO_COVERAGE = 16
	};


	//! �����־�����嵱���մ�ʱ������ɫ�Ĳ�ֵ
	/** ����û�д򿪣�������ɫֻ��Fragment ��ɫֵ���塣
	һ�����մ򿪣� ���������䣬�����⣬�����͸߹���ĸ�ֵ�������
	 Ĭ��ʹ�õĶ�����ɫ����������ɫ�� ����Ư����ͨ��ֵ����������ͻ���������������Ϊ������ɫ
	*/
	enum E_COLOR_MATERIAL
	{
		//! ���Զ�����ɫ���й��ռ���
		ECM_NONE = 0,
		//! ���������Ķ�����ɫ, Ĭ��
		ECM_DIFFUSE,
		//! ������
		ECM_AMBIENT,
		//! �����
		ECM_EMISSIVE,
		//! �߹�
		ECM_SPECULAR,
		//! ͬʱʹ��������ͻ�����
		ECM_DIFFUSE_AND_AMBIENT
	};


	//! ��������ƫ�������ı�־
	/** ��Щ��־����ƫ���ǳ�����Ļ���ǳ����۾�*/
	enum E_POLYGON_OFFSET
	{
		//! Push pixel towards the far plane, away from the eye
		/** This is typically used for rendering inner areas. */
		EPO_BACK = 0,
		//! Pull pixels towards the camera.
		/** This is typically used for polygons which should appear on top
		of other elements, such as decals. */
		EPO_FRONT = 1
	};

	//! �����ƫ�Ʒ�������  
	const c8* const PolygonOffsetDirectionNames[] =
	{
		"Back",
		"Front",
		0
	};



	//! ��������֧�ֵ����������
	const UINT32 MATERIAL_MAX_TEXTURES = SAPPHIRE_MAX_TEXTURE_LAYERS;

	class SMaterial
	{
	public:
		//! Ĭ�ϵĹ��캯���� �������壬��ɫ��������
		SMaterial()
			: MaterialType(EMT_SOLID), AmbientColor(255, 255, 255, 255), DiffuseColor(255, 255, 255, 255),
			EmissiveColor(0, 0, 0, 0), SpecularColor(255, 255, 255, 255),
			Shininess(0.0f), MaterialTypeParam(0.0f), MaterialTypeParam2(0.0f), Thickness(1.0f),
			ZBuffer(ECFN_LESSEQUAL), AntiAliasing(EAAM_SIMPLE), ColorMask(ECP_ALL),
			ColorMaterial(ECM_DIFFUSE), BlendOperation(EBO_NONE),
			PolygonOffsetFactor(0), PolygonOffsetDirection(EPO_FRONT),
			Wireframe(false), PointCloud(false), GouraudShading(true),
			Lighting(true), ZWriteEnable(true), BackfaceCulling(true), FrontfaceCulling(false),
			FogEnable(false), NormalizeNormals(false), UseMipMaps(true)
		{ }

		 
		SMaterial(const SMaterial& other)
		{
			// ���ÿһ����ʵ��������
			for (UINT32 i = 0; i < MATERIAL_MAX_TEXTURES; ++i)
				TextureLayer[i].TextureMatrix->setNull();
			*this = other;
		}

		 
		SMaterial& operator=(const SMaterial& other)
		{
			 
			if (this == &other)
				return *this;

			MaterialType = other.MaterialType;

			AmbientColor = other.AmbientColor;
			DiffuseColor = other.DiffuseColor;
			EmissiveColor = other.EmissiveColor;
			SpecularColor = other.SpecularColor;
			Shininess = other.Shininess;
			MaterialTypeParam = other.MaterialTypeParam;
			MaterialTypeParam2 = other.MaterialTypeParam2;
			Thickness = other.Thickness;
			for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				TextureLayer[i] = other.TextureLayer[i];
			}

			Wireframe = other.Wireframe;
			PointCloud = other.PointCloud;
			GouraudShading = other.GouraudShading;
			Lighting = other.Lighting;
			ZWriteEnable = other.ZWriteEnable;
			BackfaceCulling = other.BackfaceCulling;
			FrontfaceCulling = other.FrontfaceCulling;
			FogEnable = other.FogEnable;
			NormalizeNormals = other.NormalizeNormals;
			ZBuffer = other.ZBuffer;
			AntiAliasing = other.AntiAliasing;
			ColorMask = other.ColorMask;
			ColorMaterial = other.ColorMaterial;
			BlendOperation = other.BlendOperation;
			PolygonOffsetFactor = other.PolygonOffsetFactor;
			PolygonOffsetDirection = other.PolygonOffsetDirection;
			UseMipMaps = other.UseMipMaps;

			return *this;
		}

		//! �����ʵ����������
		SMaterialLayer TextureLayer[MATERIAL_MAX_TEXTURES];

		//! �������ͣ�ָ�����ж�����ν��л��
		E_MATERIAL_TYPE MaterialType;

		//! ������ʷ���Ļ�������
		/** Ĭ��ȫ�ף���ζ�Ŷ�������ȫ�����ġ�������뿴�������߹�Ч�����������*/
		ColourValue AmbientColor;

		//! ������ʷ�������������
		/** Ĭ��ȫ��. */
		ColourValue DiffuseColor;

		//! ���ʷ���⣬Ĭ��û�з����
		ColourValue EmissiveColor;

		//! ������ʷ���ľ��淴�����
		/** ������ʷ����ɫ����⡣��SMaterial::Shininess��ο������淴���Դ
		 */
		ColourValue SpecularColor;

		//! ����߹�Ӱ��Ĵ�Сֵ
		/** ֵһ��Ϊ20�������Ϊ0��û�и߹⡣
		Ҫ���ֻ������һ�����ʵ�shininess��ֵ����Χ[0.5,128]
		
		sceneNode->getMaterial(0).Shininess = 20.0f;
		
		�������������룬�ı�߹���ɫ
		sceneNode->getMaterial(0).SpecularColor.set(255,255,255,255);
		

		�����̬��Դ�ĸ߹���ɫ(SLight::SpecularColor)��ܴ�̶�Ӱ��߹���ɫ��
		���ǵ�������Դ�����ڵ�ʱ��Ĭ��������������ǵ�һ����Чֵ��
		
		// ���غ���ʾ����
		scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
		smgr->getMesh("data/faerie.md2"));
		node->setMaterialTexture(0, driver->getTexture("data/Faerie2.pcx")); // ��������������
		node->setMaterialFlag(video::EMF_LIGHTING, true); // �򿪶�̬����
		node->getMaterial(0).Shininess = 20.0f; // ���ø߹��С

		// ��Ӱ�ɫ��Դ
		scene::ILightSceneNode* light = smgr->addLightSceneNode(0,
		core::vector3df(5,5,5), video::SColorf(1.0f, 1.0f, 1.0f));*/
		Real Shininess;

		//! ���ɲ����������ڲ�������
		/** ���������Ч��Mostly ignored, ����EMT_PARALLAX_MAP_SOLID  EMT_TRANSPARENT_ALPHA_CHANNEL. */
		Real MaterialTypeParam;

		//! �ڶ������ɲ�����������������
		/** �������. */
		Real MaterialTypeParam2;

		//! ��3άԪ����һ�����һ���ߵĺ��
		Real Thickness;

		//! �Ƿ���Z��������Ĭ��ECFN_LESSEQUAL
		/** ֵ������ E_COMPARISON_FUNC. */
		UINT8 ZBuffer;

		//! ���ÿ����ģʽ
		/** ֵѡ����E_ANTI_ALIASING_MODE. Ĭ����
		EAAM_SIMPLE|EAAM_LINE_SMOOTH, i.e. ��msaa���߶�ƽ������*/
		UINT8 AntiAliasing;

		//! �������ɫƽ��
		/** ֵ������ΪE_COLOR_PLANEö�����Ϳ���'|'����
		ֻ�ڴ���ɫƽ����Ⱦ����ǰ��ȾĿ�ꡣͨ�����ڵ���Ⱦ����Ȼ�ģ�建����ʱ�ر�������ɫ
		�����������Ⱦ�ú����
		  */
		UINT8 ColorMask : 4;

		//! �����ڹ��շ��̵Ķ����ֵ��ɫ
		/** ֵѡ����E_COLOR_MATERIAL.
		�����տ�����������ɫ�ò���ֵ�͹��յ����������ʹ�ı�ÿ���������������Ϊ�ܼ򵥡�
	    Ĭ��ECM_DIFFUSE */
		UINT8 ColorMaterial : 3;

		//! �����ϲ�������ת
		/** ֵѡ��������E_BLEND_OPERATION. ʹ�ô�ֵ��ʵ�ʷ����в�ȷ�������������ں��ԡ� */
		E_BLEND_OPERATION BlendOperation : 4;

		//! ָ�������ƫ�ƶ�Զ������
		/** ָ��0�رն����ƫ�ƣ���������ָ���Ƿֿ��ģ�������Ӵ�0��7
		The factor can be from 0 to 7.*/
		UINT8 PolygonOffsetFactor : 3;

		//! ��������ƫ�Ʒ���ı�־
		/** ����ǰ���ָ��ֵ������E_POLYGON_OFFSET. */
		E_POLYGON_OFFSET PolygonOffsetDirection : 1;

		//! ����һ���߿�����������Σ�Ĭ��false
		/** �û����Է���һ�����ʱ�־
		material.Wireframe=true
		��
		material.setFlag(EMF_WIREFRAME, true); 
		*/
		bool Wireframe : 1;

		//! ���Ƶ��ƻ�����������  Ĭ��false
		bool PointCloud : 1;

		//! Flat��Gouraud �������ɫ? Ĭ��: true
		bool GouraudShading : 1;

		//! �����Ƿ���ܹ��ռ��� Ĭ��: true
		bool Lighting : 1;

		//! z�����Ƿ��д����ֻ�� Ĭ��true.
		/** �����������MaterialType��һ��͸�����ͻ��߳�������û����ALLOW_ZWRITE_ON_TRANSPARENT
		  */
		bool ZWriteEnable : 1;

		//! �Ƿ����޳��� Ĭ�� true
		bool BackfaceCulling : 1;

		//! �Ƿ�ǰ���޳�  Ĭ�� false
		bool FrontfaceCulling : 1;

		//! �Ƿ�����  Ĭ�� false
		bool FogEnable : 1;

		//! �Ƿ��׼������ 
		/** Ĭ��false */
		bool NormalizeNormals : 1;

		//! ���mipmap���ã��Ƿ�ʹ��mipmap
		/** ��ʱ,�ر�mipmap  Ĭ��true*/
		bool UseMipMaps : 1;

		//! ��ȡ��i������任����
		/** \param i  ���ܴ���MATERIAL_MAX_TEXTURES
		\return ��i��������������*/
		 Matrix4& getTextureMatrix(UINT32 i)
		{
			return TextureLayer[i].getTextureMatrix();
		}

		//! ��ȡһ�������޸ĵĵ�i������任���� 
		/** \param i  
		\return ��i��������������,����i����MATERIAL_MAX_TEXTURES���ǵ�λ����*/
		 const Matrix4& getTextureMatrix(UINT32 i) const
		{
			if (i<MATERIAL_MAX_TEXTURES)
				return TextureLayer[i].getTextureMatrix();
			else
				return Matrix4::IDENTITY;
		}

		//! ���õ�i����任����
		/** \param i 
		\param mat ��i��������������. */
		 void setTextureMatrix(UINT32 i, const Matrix4& mat)
		{
			if (i >= MATERIAL_MAX_TEXTURES)
				return;
			TextureLayer[i].setTextureMatrix(mat);
		}

		//! ��ȡ��i������
		/** \param i 
		\return ��������ˣ����ص�i���������򷵻�0 */
		ITexture* getTexture(UINT32 i) const
		{
			return i < MATERIAL_MAX_TEXTURES ? TextureLayer[i].Texture : 0;
		}

		//! ���õ�i������
		/** ���i>=MATERIAL_MAX_TEXTURES �������Ч
		\param i  
		\param tex ����ָ�� */
		void setTexture(UINT32 i, ITexture* tex)
		{
			if (i >= MATERIAL_MAX_TEXTURES)
				return;
			TextureLayer[i].Texture = tex;
		}

		//! �ø�����ֵ���ò��ʱ�־
		/** \param flag Ҫ���õı�־
		\param value �����־��ֵ */
		void setFlag(E_MATERIAL_FLAG flag, bool value)
		{
			switch (flag)
			{
			case EMF_WIREFRAME:
				Wireframe = value; break;
			case EMF_POINTCLOUD:
				PointCloud = value; break;
			case EMF_GOURAUD_SHADING:
				GouraudShading = value; break;
			case EMF_LIGHTING:
				Lighting = value; break;
			case EMF_ZBUFFER:
				ZBuffer = value; break;
			case EMF_ZWRITE_ENABLE:
				ZWriteEnable = value; break;
			case EMF_BACK_FACE_CULLING:
				BackfaceCulling = value; break;
			case EMF_FRONT_FACE_CULLING:
				FrontfaceCulling = value; break;
			case EMF_BILINEAR_FILTER:
			{
				for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
					TextureLayer[i].BilinearFilter = value;
			}
			break;
			case EMF_TRILINEAR_FILTER:
			{
				for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
					TextureLayer[i].TrilinearFilter = value;
			}
			break;
			case EMF_ANISOTROPIC_FILTER:
			{
				if (value)
					for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
						TextureLayer[i].AnisotropicFilter = 0xFF;
				else
					for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
						TextureLayer[i].AnisotropicFilter = 0;
			}
			break;
			case EMF_FOG_ENABLE:
				FogEnable = value; break;
			case EMF_NORMALIZE_NORMALS:
				NormalizeNormals = value; break;
			case EMF_TEXTURE_WRAP:
			{
				for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				{
					TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)value;
					TextureLayer[i].TextureWrapV = (E_TEXTURE_CLAMP)value;
				}
			}
			break;
			case EMF_ANTI_ALIASING:
				AntiAliasing = value ? EAAM_SIMPLE : EAAM_OFF; break;
			case EMF_COLOR_MASK:
				ColorMask = value ? ECP_ALL : ECP_NONE; break;
			case EMF_COLOR_MATERIAL:
				ColorMaterial = value ? ECM_DIFFUSE : ECM_NONE; break;
			case EMF_USE_MIP_MAPS:
				UseMipMaps = value; break;
			case EMF_BLEND_OPERATION:
				BlendOperation = value ? EBO_ADD : EBO_NONE; break;
			case EMF_POLYGON_OFFSET:
				PolygonOffsetFactor = value ? 1 : 0;
				PolygonOffsetDirection = EPO_BACK;
				break;
			default:
				break;
			}
		}

		//! ��ȡ���ʵı�־
		/** \param flag Ҫ��ѯ�Ĳ��ʱ�־
		\return ��ǰ��־��ֵ */
		bool getFlag(E_MATERIAL_FLAG flag) const
		{
			switch (flag)
			{
			case EMF_WIREFRAME:
				return Wireframe;
			case EMF_POINTCLOUD:
				return PointCloud;
			case EMF_GOURAUD_SHADING:
				return GouraudShading;
			case EMF_LIGHTING:
				return Lighting;
			case EMF_ZBUFFER:
				return ZBuffer != ECFN_NEVER;
			case EMF_ZWRITE_ENABLE:
				return ZWriteEnable;
			case EMF_BACK_FACE_CULLING:
				return BackfaceCulling;
			case EMF_FRONT_FACE_CULLING:
				return FrontfaceCulling;
			case EMF_BILINEAR_FILTER:
				return TextureLayer[0].BilinearFilter;
			case EMF_TRILINEAR_FILTER:
				return TextureLayer[0].TrilinearFilter;
			case EMF_ANISOTROPIC_FILTER:
				return TextureLayer[0].AnisotropicFilter != 0;
			case EMF_FOG_ENABLE:
				return FogEnable;
			case EMF_NORMALIZE_NORMALS:
				return NormalizeNormals;
			case EMF_TEXTURE_WRAP:
				return !(TextureLayer[0].TextureWrapU ||
					TextureLayer[0].TextureWrapV ||
					TextureLayer[1].TextureWrapU ||
					TextureLayer[1].TextureWrapV ||
					TextureLayer[2].TextureWrapU ||
					TextureLayer[2].TextureWrapV ||
					TextureLayer[3].TextureWrapU ||
					TextureLayer[3].TextureWrapV);
			case EMF_ANTI_ALIASING:
				return (AntiAliasing == 1);
			case EMF_COLOR_MASK:
				return (ColorMask != ECP_NONE);
			case EMF_COLOR_MATERIAL:
				return (ColorMaterial != ECM_NONE);
			case EMF_USE_MIP_MAPS:
				return UseMipMaps;
			case EMF_BLEND_OPERATION:
				return BlendOperation != EBO_NONE;
			case EMF_POLYGON_OFFSET:
				return PolygonOffsetFactor != 0;
			}

			return false;
		}

		 
		inline bool operator!=(const SMaterial& b) const
		{
			bool different =
				MaterialType != b.MaterialType ||
				AmbientColor != b.AmbientColor ||
				DiffuseColor != b.DiffuseColor ||
				EmissiveColor != b.EmissiveColor ||
				SpecularColor != b.SpecularColor ||
				Shininess != b.Shininess ||
				MaterialTypeParam != b.MaterialTypeParam ||
				MaterialTypeParam2 != b.MaterialTypeParam2 ||
				Thickness != b.Thickness ||
				Wireframe != b.Wireframe ||
				PointCloud != b.PointCloud ||
				GouraudShading != b.GouraudShading ||
				Lighting != b.Lighting ||
				ZBuffer != b.ZBuffer ||
				ZWriteEnable != b.ZWriteEnable ||
				BackfaceCulling != b.BackfaceCulling ||
				FrontfaceCulling != b.FrontfaceCulling ||
				FogEnable != b.FogEnable ||
				NormalizeNormals != b.NormalizeNormals ||
				AntiAliasing != b.AntiAliasing ||
				ColorMask != b.ColorMask ||
				ColorMaterial != b.ColorMaterial ||
				BlendOperation != b.BlendOperation ||
				PolygonOffsetFactor != b.PolygonOffsetFactor ||
				PolygonOffsetDirection != b.PolygonOffsetDirection ||
				UseMipMaps != b.UseMipMaps;
			for (UINT32 i = 0; (i<MATERIAL_MAX_TEXTURES) && !different; ++i)
			{
				different |= (TextureLayer[i] != b.TextureLayer[i]);
			}
			return different;
		}

		 
		inline bool operator==(const SMaterial& b) const
		{
			return !(b != *this);
		}

		bool isTransparent() const
		{
			return MaterialType == EMT_TRANSPARENT_ADD_COLOR ||
				MaterialType == EMT_TRANSPARENT_ALPHA_CHANNEL ||
				MaterialType == EMT_TRANSPARENT_VERTEX_ALPHA ||
				MaterialType == EMT_TRANSPARENT_REFLECTION_2_LAYER;
		}
	};

	//! ȫ�ֳ�һ���Բ���
	_SapphireExport extern SMaterial IdentityMaterial;

}


#endif 