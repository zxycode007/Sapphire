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

	//! EMT_ONETEXTURE_BLEND 的标志( 混合因子) BlendFunc = source * sourceFactor + dest * destFactor
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

	//! 当混合开启时定义混合操作的值
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

	//!材质类型参数: e.g. DirectX: D3DTOP_MODULATE, D3DTOP_MODULATE2X, D3DTOP_MODULATE4X
	enum E_MODULATE_FUNC
	{
		EMFN_MODULATE_1X = 1,
		EMFN_MODULATE_2X = 2,
		EMFN_MODULATE_4X = 4
	};

	//! 比较函数, e.g. 用于深度测试
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


	//!渲染中开启/关闭颜色平面的枚举值
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


	//! 采取alpha值的来源
	/** 这个只支持于 EMT_ONETEXTURE_BLEND.
	你可以使用|来组合， alpha值可以被调制（多个）
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


	//! EMT_ONETEXTURE_BLEND: 包装srcFact, dstFact, Modulate 和 alpha source 到材质类型参数 MaterialTypeParam
	/** alpha 源可以由E_ALPHA_SOURCE的值和进行或或组合 */
	inline Real pack_textureBlendFunc(const E_BLEND_FACTOR srcFact, const E_BLEND_FACTOR dstFact, const E_MODULATE_FUNC modulate = EMFN_MODULATE_1X, const UINT32 alphaSource = EAS_TEXTURE)
	{
		const UINT32 tmp = (alphaSource << 12) | (modulate << 8) | (srcFact << 4) | dstFact;
		return FR(tmp);
	}


	//! EMT_ONETEXTURE_BLEND: 解包 srcFact & dstFact and Modulo 到材质类型参数 MaterialTypeParam
	inline void unpack_textureBlendFunc(E_BLEND_FACTOR &srcFact, E_BLEND_FACTOR &dstFact,
		E_MODULATE_FUNC &modulo, UINT32& alphaSource, const Real param)
	{
		const UINT32 state = IR(param);
		alphaSource = (state & 0x0000F000) >> 12;
		modulo = E_MODULATE_FUNC((state & 0x00000F00) >> 8);
		srcFact = E_BLEND_FACTOR((state & 0x000000F0) >> 4);
		dstFact = E_BLEND_FACTOR((state & 0x0000000F));
	}

	//! EMT_ONETEXTURE_BLEND: alpha混合是否有混合因子
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


	//! 这个标志用于指定抗锯齿和平滑模式
	/** 支持多重采样，几何平滑，和alpha覆盖
	有些取得不支持每一个材质的抗锯齿设置，这里情况中，FSAA/多重采样是由设备模式SapphireCreationParameters创建来定义的
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


	//! 这个标志允许定义当光照打开时顶点颜色的插值
	/** 光照没有打开，顶点颜色只有Fragment 颜色值定义。
	一旦光照打开， 会由漫反射，环境光，放射光和高光的四个值会替代。
	 默认使用的顶点颜色是漫反射颜色， 其它漂亮的通用值是用漫反射和环境光两个因子作为顶点颜色
	*/
	enum E_COLOR_MATERIAL
	{
		//! 不对顶点颜色进行光照计算
		ECM_NONE = 0,
		//! 用漫反射光的顶点颜色, 默认
		ECM_DIFFUSE,
		//! 环境光
		ECM_AMBIENT,
		//! 发射光
		ECM_EMISSIVE,
		//! 高光
		ECM_SPECULAR,
		//! 同时使用漫反射和环境光
		ECM_DIFFUSE_AND_AMBIENT
	};


	//! 定义多边形偏移特征的标志
	/** 这些标志定义偏移是朝向屏幕还是朝向眼睛*/
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

	//! 多边形偏移方向名字  
	const c8* const PolygonOffsetDirectionNames[] =
	{
		"Back",
		"Front",
		0
	};



	//! 材质所能支持的最大纹理数
	const UINT32 MATERIAL_MAX_TEXTURES = SAPPHIRE_MAX_TEXTURE_LAYERS;

	class SMaterial
	{
	public:
		//! 默认的构造函数， 创建固体，白色明亮材质
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
			// 清空每一层材质的纹理矩阵
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

		//! 本材质的纹理层数组
		SMaterialLayer TextureLayer[MATERIAL_MAX_TEXTURES];

		//! 材质类型，指定所有东西如何进行混合
		E_MATERIAL_TYPE MaterialType;

		//! 这个材质反射的环境光量
		/** 默认全白，意味着对象是完全照明的。如果你想看漫反射或高光效果，降低这个*/
		ColourValue AmbientColor;

		//! 这个材质反射的漫反射光量
		/** 默认全白. */
		ColourValue DiffuseColor;

		//! 材质发射光，默认没有发射光
		ColourValue EmissiveColor;

		//! 这个材质反射的镜面反射光量
		/** 这个材质反射白色镜面光。看SMaterial::Shininess如何开启镜面反射光源
		 */
		ColourValue SpecularColor;

		//! 镜面高光影响的大小值
		/** 值一般为20，如果设为0，没有高光。
		要激活，只需设置一个材质的shininess的值到范围[0.5,128]
		
		sceneNode->getMaterial(0).Shininess = 20.0f;
		
		你可以用下面代码，改变高光颜色
		sceneNode->getMaterial(0).SpecularColor.set(255,255,255,255);
		

		这个动态光源的高光颜色(SLight::SpecularColor)会很大程度影响高光颜色，
		但是当创建光源场景节点时，默认情况下设置它们到一个有效值。
		
		// 加载和显示网格
		scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
		smgr->getMesh("data/faerie.md2"));
		node->setMaterialTexture(0, driver->getTexture("data/Faerie2.pcx")); // 设置漫反射纹理
		node->setMaterialFlag(video::EMF_LIGHTING, true); // 打开动态光照
		node->getMaterial(0).Shininess = 20.0f; // 设置高光大小

		// 添加白色光源
		scene::ILightSceneNode* light = smgr->addLightSceneNode(0,
		core::vector3df(5,5,5), video::SColorf(1.0f, 1.0f, 1.0f));*/
		Real Shininess;

		//! 自由参数，依赖于材质类型
		/** 多数情况无效，Mostly ignored, 用在EMT_PARALLAX_MAP_SOLID  EMT_TRANSPARENT_ALPHA_CHANNEL. */
		Real MaterialTypeParam;

		//! 第二个自由参数，依赖材质类型
		/** 多数情况. */
		Real MaterialTypeParam2;

		//! 非3维元素如一个点和一条线的厚度
		Real Thickness;

		//! 是否开启Z缓冲区，默认ECFN_LESSEQUAL
		/** 值来自于 E_COMPARISON_FUNC. */
		UINT8 ZBuffer;

		//! 设置抗锯齿模式
		/** 值选择于E_ANTI_ALIASING_MODE. 默认是
		EAAM_SIMPLE|EAAM_LINE_SMOOTH, i.e. 简单msaa和线段平滑开启*/
		UINT8 AntiAliasing;

		//! 定义打开颜色平面
		/** 值定义作为E_COLOR_PLANE枚举类型可以'|'链接
		只在打开颜色平面渲染到当前渲染目标。通常用于当渲染到深度或模板缓冲区时关闭所有颜色
		，或对立体渲染用红和绿
		  */
		UINT8 ColorMask : 4;

		//! 定义在光照方程的顶点插值颜色
		/** 值选择于E_COLOR_MATERIAL.
		当光照开启，顶点颜色用材质值和光照调制替代。这使改变每个面的漫反射光的行为很简单。
	    默认ECM_DIFFUSE */
		UINT8 ColorMaterial : 3;

		//! 保存混合操作的旋转
		/** 值选择来自于E_BLEND_OPERATION. 使用此值的实际方法尚不确定，所以它现在忽略。 */
		E_BLEND_OPERATION BlendOperation : 4;

		//! 指定多边形偏移多远的因子
		/** 指定0关闭多边形偏移，这个方向的指定是分开的，这个因子从0到7
		The factor can be from 0 to 7.*/
		UINT8 PolygonOffsetFactor : 3;

		//! 定义多边形偏移方向的标志
		/** 可以前或后，指定值可以用E_POLYGON_OFFSET. */
		E_POLYGON_OFFSET PolygonOffsetDirection : 1;

		//! 绘制一个线框或填充的三角形，默认false
		/** 用户可以访问一个材质标志
		material.Wireframe=true
		或
		material.setFlag(EMF_WIREFRAME, true); 
		*/
		bool Wireframe : 1;

		//! 绘制点云或填充的三角形  默认false
		bool PointCloud : 1;

		//! Flat或Gouraud 高络德着色? 默认: true
		bool GouraudShading : 1;

		//! 材质是否接受光照计算 默认: true
		bool Lighting : 1;

		//! z缓冲是否可写或者只读 默认true.
		/** 如果材质类型MaterialType是一个透明类型或者场景参数没设置ALLOW_ZWRITE_ON_TRANSPARENT
		  */
		bool ZWriteEnable : 1;

		//! 是否背面剔除打开 默认 true
		bool BackfaceCulling : 1;

		//! 是否前面剔除  默认 false
		bool FrontfaceCulling : 1;

		//! 是否开启雾化  默认 false
		bool FogEnable : 1;

		//! 是否标准化法线 
		/** 默认false */
		bool NormalizeNormals : 1;

		//! 如果mipmap可用，是否使用mipmap
		/** 有时,关闭mipmap  默认true*/
		bool UseMipMaps : 1;

		//! 获取第i层纹理变换矩阵
		/** \param i  不能大于MATERIAL_MAX_TEXTURES
		\return 第i层纹理的纹理矩阵*/
		 Matrix4& getTextureMatrix(UINT32 i)
		{
			return TextureLayer[i].getTextureMatrix();
		}

		//! 获取一个不可修改的第i层纹理变换矩阵 
		/** \param i  
		\return 第i层纹理的纹理矩阵,或者i大于MATERIAL_MAX_TEXTURES则是单位矩阵*/
		 const Matrix4& getTextureMatrix(UINT32 i) const
		{
			if (i<MATERIAL_MAX_TEXTURES)
				return TextureLayer[i].getTextureMatrix();
			else
				return Matrix4::IDENTITY;
		}

		//! 设置第i纹理变换矩阵
		/** \param i 
		\param mat 第i层纹理的纹理矩阵. */
		 void setTextureMatrix(UINT32 i, const Matrix4& mat)
		{
			if (i >= MATERIAL_MAX_TEXTURES)
				return;
			TextureLayer[i].setTextureMatrix(mat);
		}

		//! 获取第i层纹理
		/** \param i 
		\return 如果定义了，返回第i层纹理，否则返回0 */
		ITexture* getTexture(UINT32 i) const
		{
			return i < MATERIAL_MAX_TEXTURES ? TextureLayer[i].Texture : 0;
		}

		//! 设置第i层纹理
		/** 如果i>=MATERIAL_MAX_TEXTURES 这个设置效
		\param i  
		\param tex 纹理指针 */
		void setTexture(UINT32 i, ITexture* tex)
		{
			if (i >= MATERIAL_MAX_TEXTURES)
				return;
			TextureLayer[i].Texture = tex;
		}

		//! 用给定的值设置材质标志
		/** \param flag 要设置的标志
		\param value 这个标志的值 */
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

		//! 获取材质的标志
		/** \param flag 要查询的材质标志
		\return 当前标志的值 */
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

	//! 全局常一致性材质
	_SapphireExport extern SMaterial IdentityMaterial;

}


#endif 