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


	class IVideoDriver //: public virtual IReferenceCounted
	{
	public:

		//! 应用程序在进行任何渲染前必须调用这个方法
		/** 这个方法能够清空后备和z缓冲区
		\param backBuffer 指定后备缓冲区是否要被清除，它意味着用指定的颜色填充屏幕。
		如果参数是false，这个后备缓冲区将不会被清除并且颜色参数无效。
		\param zBuffer 指定是否清除深度缓冲区（z缓冲）.它仅仅在2d渲染时，不需要这么做
		\param color 这个颜色用于后备缓冲区的清理
		\param videoData 其它窗口的句柄，如果你想要这个位图显示到其它窗口的话。
		如果这是一个空元素，每个东西都会显示到默认的窗口中。注意：这个特征不被所有的硬件支持。
		\param sourceRect 指向一个作为要显示的源矩形区域的指针。设置为空展示所有东西。注意:不是所有设备都支持
		\return 如果失败返回false */
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0) = 0;

		//! 把渲染后的图像显示到屏幕
		/** 应用程序在任何渲染结束后必须调用这个方法
		\return 如果成功返回true，否则返回false */
		virtual bool endScene() = 0;

		//! 查询驱动特征
		/** 如果一个特征有效，返回true
		\param feature 要查询的特征
		\return 如果如果有效返回true，否则返回false. */
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const = 0;

		//! 关闭一个驱动的特征
		/** 也能够用次函数再一次打开特征。它不能打开不支持的特性。
		\param feature 关闭的特性
		\param flag 当这个标志为tue是关闭，false是打开*/
		virtual void disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag = true) = 0;

		//! 获取实际视频驱动的属性
		/** 下面是可以查询的给定类型
		MaxTextures (int) 驱动能同时支持的最大纹理数。用 SAPPHIRE_MAX_TEXTURE_LAYERS 会匹配这个数值
		MaxSupportedTextures (int) 硬件驱动的固定渲染管线能够支持最大纹理数，引擎实际支持的引擎数量更低。
		MaxLights (int) 驱动的固定关系所能支持的最大光源数，通常6-8个，用光源管理器或者更多由shader决定。
		MaxAnisotropy (int) 各向异性滤波所能支持最大数量。 至少位1，最大通常是16或32
		MaxUserClipPlanes (int) 额外剪裁平面数， 它由用户通过指定的驱动方法设置
		MaxAuxBuffers (int) 特定的渲染缓冲，它在当前引擎并不可用。只支持OpenGL
		MaxMultipleRenderTargets (int) 可以同时存在的渲染目标数。通过shader渲染到MRT
		MaxIndices (int) 用于在一次渲染调用中的索引数(i.e. 一个网格缓冲区).
		MaxTextureSize (int) 一个纹理最大尺寸
		MaxGeometryVerticesOut (int) geometry shader 在一个通道的顶点数  只支持OpenGL
		MaxTextureLODBias (float) LOD 偏差的最大值  通常是16左右，某些系统可以更低
		Version (int) 驱动版本. 应该是Major*100+Minor
		ShaderLanguageVersion (int) shader语义的版本  Major*100+Minor.
		AntiAlias (int) 驱动对每个像素的采样次数， 0和1意味着抗锯齿关闭，通常值是2,4,8,16,32
		*/
		virtual const  IAttributes& getDriverAttributes() const = 0;

		//! 检查这个驱动是否最近重置
		/** 对于D3D设备，如果驱动重置，你将需要重新创建RTT。 应该在beginScene()后查询。
		*/
		virtual bool checkDriverReset() = 0;

		//! 设置变换矩阵
		/** \param state 要进行变换的类型 观察/世界/投影
		\param mat 描述变换的矩阵 */
		virtual void setTransform(E_TRANSFORMATION_STATE state, const  Matrix4& mat) = 0;

		//! 返回setTransform设置的变换矩阵
		/** \param state 查询变换的类型
		\return 描述变换的矩阵 */
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const = 0;

		//! 返回图像加载器数量
		/** \return 图像加载器的数量*/
		virtual UINT32 getImageLoaderCount() const = 0;

		//! 返回给定的图像加载器
		/** \param n 返回加载器的索引。这个参数是一个基于0的数组索引
		\return 指向这个加载器的指针， 如果为0索引错误*/
		virtual IImageLoader* getImageLoader(UINT32 n) = 0;

		//! 返回图像写入器
		/** \return 图像写入器的数量 */
		virtual UINT32 getImageWriterCount() const = 0;

		//! 返回给定图像的写入器
		/** \param n 要返回写入器的索引。这个参数是一个基于0的数组索引
		\return 指向这个写入器的指针， 如果为0索引错误 */
		virtual IImageWriter* getImageWriter(UINT32 n) = 0;

		//! 设置一个材质
		/** 所有3d绘制函数将用这个材质绘制几何体
		\param material: 从现在起要用的材质 */
		virtual void setMaterial(const SMaterial& material) = 0;

		//! 获取一个命名的纹理
		/** 如果它还没有被加载和创建mipmap的话，必要时从磁盘上加载纹理。
		使用setTextureCreationFlag()函数会影响到纹理的加载。这个纹理可以有多种格式，
		如BMP, JPG, TGA, PCX, PNG, 和PSD.
		\param filename 要加载的纹理文件名
		\return 纹理的指针， 如果为0，这个纹理不会被加载。  */
		virtual ITexture* getTexture(const  path& filename) = 0;

		//! 活动一个命名的纹理Get access to a named texture.
		/** 如果它还没有被加载和创建mipmap的话，必要时从磁盘上加载纹理。
		使用setTextureCreationFlag()函数会影响到纹理的加载。这个纹理可以有多种格式，
		如BMP, JPG, TGA, PCX, PNG, 和PSD.
		\param file 已经打开的文件指针
		\return 纹理的指针， 如果为0，这个纹理不会被加载。 */
		virtual ITexture* getTexture( IReadFile* file) = 0;

		//! 通过索引返回一个纹理
		/** \param index: 纹理的索引，必须小于getTextureCount()。
		请注意：这个所以可能在添加或移动纹理时改变
		\return 纹理的指针，或者如果这个纹理没有设置或者索引超出范围为0。 */
		virtual ITexture* getTextureByIndex(UINT32 index) = 0;

		//! 返回当前加载的纹理数量
		/** \return 当前加载的纹理数量 */
		virtual UINT32 getTextureCount() const = 0;

		//! 重命名一个纹理
		/** \param texture 指向要重命名纹理的指针
		\param newName 纹理的新名字。它应该是唯一的 */
		virtual void renameTexture(ITexture* texture, const  path& newName) = 0;

		//! 创建一个指定大小的空纹理
		/** \param size: 纹理的大小
		\param name 一个纹理的名字。 之后调用getTexture()通过这个名字返回这个纹理。
		\param format 想要的纹理颜色格式。请注意：取得可能选择创建纹理另外的颜色格式。
		\return 指针新创建纹理的指针 */
		virtual ITexture* addTexture(const  dimension2d<UINT32>& size,
			const  path& name, ECOLOR_FORMAT format = ECF_A8R8G8B8) = 0;

		//! 由一个IImage创建一个纹理
		/** \param name 一个纹理的名字。 之后调用getTexture()通过这个名字返回这个纹理。
		\param image 用来创建纹理的图像
		\param mipmapData 可选的设置构建mipmap集合的指针。必须和图像有同样的颜色类型。如果这个参数没有给出，
		这个mipmap由图像生成。
		\return 指向新创建纹理的指针 */
		virtual ITexture* addTexture(const  path& name, IImage* image, void* mipmapData = 0) = 0;

		//! 添加一个新的渲染目标纹理到纹理缓冲区 
		/** \param size 纹理的大小，单位像素。宽度和高度应该为2的指数幂（64，128，256，512...）
		并且它不能够比后备缓冲区更大，因为它分享深度缓冲区和屏幕缓冲区
		\param name 一个可选的RTT名
		\param format 渲染目标的颜色格式。支持浮点格式
		\return 返回创建纹理的指针，如果纹理没有被创建，返回0。 */
		virtual ITexture* addRenderTargetTexture(const  dimension2d<UINT32>& size,
			const  path& name = "rt", const ECOLOR_FORMAT format = ECF_UNKNOWN) = 0;

		//! 从纹理缓冲区移除一个纹理并且删除它。
		/** 这个方法可以释放掉大量内存。请注意调用这个方法以后，ITexture的指针不再有效。
		\param texture 从引擎删除的纹理*/
		virtual void removeTexture(ITexture* texture) = 0;

		//! 从纹理缓冲区移除所有的纹理并且删除它们
		/** 这个方法可以释放掉大量内存。请注意调用这个方法以后，ITexture的指针不再有效。  */
		virtual void removeAllTextures() = 0;

		//! 移除硬件网格缓冲区
		virtual void removeHardwareBuffer(const  IMeshBuffer* mb) = 0;

		//! 移除所有的硬件缓冲区
		virtual void removeAllHardwareBuffers() = 0;

		//! 创建occlusion query遮蔽查询
		/** 用节点的辨识和网格遮蔽测试 */
		virtual void addOcclusionQuery( ISceneNode* node,
			const  IMesh* mesh = 0) = 0;

		//! 移除遮蔽查询
		virtual void removeOcclusionQuery(ISceneNode* node) = 0;

		//! 移除所有的遮蔽查询
		virtual void removeAllOcclusionQueries() = 0;

		//! 允许遮蔽查询。 绘制网格存放在查询中
		/** 如果这个网格不应该被渲染可见，用覆盖材质关闭颜色和深度缓冲区*/
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false) = 0;

		//! 允许所有的遮蔽查询，绘制网格存放在查询中
		/** 如果这个网格不应该被渲染可见，用覆盖材质关闭颜色和深度缓冲区 */
		virtual void runAllOcclusionQueries(bool visible = false) = 0;

		//! 更新遮蔽查询， 从GPU返回查询结果
		/** 如果查询被阻挡， 设置标志为false
		这种情况下更新可能不会发生*/
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true) = 0;

		//! 更新所有节点遮蔽查询， 从GPU返回查询结果
		/** 如果查询被阻挡， 设置标志为false
		这种情况下更新可能不会发生 */
		virtual void updateAllOcclusionQueries(bool block = true) = 0;

		//! 返回查询结果
		/** 返回值是可见的像素/片段的数量。这个值是一个安全的近似逼近，能够大于实际的像素值
		  */
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const = 0;

		//! 在纹理设置一个基于色键的布尔alpha通道
		/** 这使纹理在色键的颜色的纹理像素完全透明，例如在draw2DImage使用useAlphachannel==true，
		\param texture 要修改alpha通道的纹理
		\param color 色键颜色，每个用这个颜色的纹理元素都会变得完全透明.请注意：这个纹理的颜色
		在加载它的时候可能被覆盖，所以颜色值在引擎和图片编辑器程序中不一定精确的相同。为避免这个问题，你
		可以使用makeColorKeyTexture方法，它使用一个像素的位置替代一个颜色的值
		\param zeroTexels 如果设置为true，丢弃，那么任何纹理像素姜勇它们的颜色匹配这个色键，和它们的alpha一样，
		这种为0黑色。  */
		virtual void makeColorKeyTexture(ITexture* texture,
			ColourValue color,
			bool zeroTexels = false) const = 0;

		//! 在纹理上基于一个颜色的位置来设置一个布尔alpha通道
		/**这使纹理在色键的颜色的纹理像素完全透明，例如在draw2DImage使用useAlphachannel==true，
		其它纹理元素的alpha不会被修改
		\param texture 要修改alpha通道的纹理
		\param colorKeyPixelPos 要使用色键颜色的像素位置. 每个纹理像素的这个颜色将变的完全透明。
		\param zeroTexels \如果设置为true，丢弃，那么任何纹理像素姜勇它们的颜色匹配这个色键，和它们的alpha一样，
		这种为0黑色。 */
		virtual void makeColorKeyTexture(ITexture* texture,
			Vector2 colorKeyPixelPos,
			bool zeroTexels = false) const = 0;

		//! 从一个高程图纹理创建一个法线贴图
		/** 如果目标纹理32位， 高度值作为附加保存在纹理的alpha分量。这个值用于EMT_PARALLAX_MAP_SOLID材质和相似的纹理
		\param texture 要修改alpha通道的纹理
		\param amplitude 乘以高度信息的常数*/
		virtual void makeNormalMapTexture(ITexture* texture, Real amplitude = 1.0f) const = 0;

		//! 设置一个新渲染目标
		/** 
		只有在驱动支持EVDF_RENDER_TO_TARGET特征才能工作， 它可以用queryFeature()查询。
		通常，渲染到纹理是通过下面的方法。
		// 创建渲染目标
		ITexture* target = driver->addRenderTargetTexture(core::dimension2d<UINT32>(128,128), "rtt1");

		// ...

		driver->setRenderTarget(target); // 设置渲染目标
		// .. 在这里绘制物体
		driver->setRenderTarget(0); // 设置回之前的渲染目标

		请注意：你不能同时在渲染场景到这个渲染目标的同时，再渲染目标渲染3D或2D几何体作为纹理到这个渲染目标作为纹理。
		它通常只可能在IVideoDriver::beginScene()和endScene()方法调用过程中渲染一个纹理。如果设置为0，它设置到最后
		一次setRenderTarget()调用前的一个渲染目标。
		\param clearBackBuffer 用这个颜色参数来清理这个渲染目标的后备缓冲区
		\param clearZBuffer 清零这个渲染目标的Z缓冲区
		注意：因为帧缓冲区和这个渲染目标共享Z缓冲区， 它的z缓冲区可能会因为这样被部分清除
		\param color 这个渲染目标的后备缓冲区
		\return 如果成功返回true，否则false*/
		virtual bool setRenderTarget( ITexture* texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! 设置或复位特定的渲染目标
		/** 这个方法打开访问特定的颜色缓冲区，如立体缓冲区或辅助缓冲区
		\param target 这个缓冲区的枚举值
		\param clearTarget 是否用指定的颜色参数清除目标缓冲区
		\param clearZBuffer 清零这个渲染目标的Z缓冲区
		注意：因为帧缓冲区和这个渲染目标共享Z缓冲区， 它的z缓冲区可能会因为这样被部分清除
		\param color 渲染目标的背景色
		\return 如果成功返回true，否则false */
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget = true,
			bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! 设置新的多渲染目标
		virtual bool setRenderTarget(const vector<IRenderTarget>& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = ColourValue(0, 0, 0, 0)) = 0;

		//! 设置一个新的视口
		/** 每一个渲染操作都会在一个新的区域内进行
		\param area: 定义为渲染操作的新区域的矩形*/
		virtual void setViewPort(const rect<SINT32>& area) = 0;

		//! 获取当前视口区域
		/** \return 当前视口区域 */
		virtual const rect<SINT32>& getViewPort() const = 0;

		//! 绘制一个顶点图元列表
		/** 注意, 依赖索引类型，一些顶点可能不能通过索引列表访问到。这因为16位索引的限制65535个顶点
		请注意：当前不是所有的图元对于所有的驱动都有效， 有效可能可能只能通过三角形渲染来模拟。
		\param vertices 顶点数组的指针
		\param vertexCount 数组里的顶点的数量
		\param indexList 顶点索引数组的指针。它定义每个图元所用的哪一些顶点.
		依赖pType图元类型，索引解释单个对象（点），一对（线段），三个（三角形），或四边形
		\param primCount 图元数量
		\param vType 顶点类型   对于S3DVertex EVT_STANDARD
		\param pType 图元类型,  对于扇形三角形带 EPT_TRIANGLE_FAN
		\param iType 索引类型,  对于16位索引 EIT_16BIT */
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			E_PRIMITIVE_TYPE pType = EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! 绘制一个2d的顶点图元列表
		/** 和这个方法的通用3d版本相比较， 这一个设置为2d渲染器的模式，并且用只有向量的x和y部分。
		依赖索引类型，一些顶点可能不能通过索引列表访问到。这因为16位索引的限制65535个顶点
		请注意：当前不是所有的图元对于所有的驱动都有效， 有效可能可能只能通过三角形渲染来模拟。
		\param vertices 顶点数组的指针
		\param vertexCount 数组里的顶点的数量
		\param indexList 顶点索引数组的指针。它定义每个图元所用的哪一些顶点.
		依赖pType图元类型，索引解释单个对象（点），一对（线段），三个（三角形），或四边形
		\param primCount 图元数量
		\param vType 顶点类型   对于S3DVertex EVT_STANDARD
		\param pType 图元类型,  对于扇形三角形带 EPT_TRIANGLE_FAN
		\param iType 索引类型,  对于16位索引 EIT_16BIT*/
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			E_PRIMITIVE_TYPE pType = EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! 绘制一个索引化的三角形列表
		/** 注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数/3*/
		void drawIndexedTriangleList(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
		}

		//! 绘制一个索引化的三角形列表
		/** 注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数/3*/
		void drawIndexedTriangleList(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, EPT_TRIANGLES, EIT_16BIT);
		}

		//!绘制一个索引化的三角形列表
		/** 注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数/3 */
		void drawIndexedTriangleList(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS, EPT_TRIANGLES, EIT_16BIT);
		}

		//! 绘制一个索引化的三角形扇型带
		/**  注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数 - 2 */
		void drawIndexedTriangleFan(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! 绘制一个索引化的三角形扇型带
		/**  注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数 - 2 */
		void drawIndexedTriangleFan(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! 绘制一个索引化的三角形扇型带
		/**  注意：这最大支持65536个顶点，因为索引列表是一个16位数组每个最大值是65535.
		如果大于65535个顶点在列表，导致操作未定义。
		\param vertices 顶点数组指针
		\param vertexCount 顶点数量
		\param indexList 顶点索引数组指针
		\param triangleCount 三角形数量，用索引数 - 2 */
		void drawIndexedTriangleFan(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS,  EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! 绘制一条3d线段
		/** 对于某些实现，这个方法对于一些三角形简单调用drawVertexPrimitiveList
        注意：这条线段用当前变换矩阵和材质绘制。所以如果你需要绘制不依赖当前变换的3d线段
		用下列代码：
		 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		 
		在绘制这条线段前正常的设置材质，某些取得支持在这材质中设置线段厚度
		\param start 3d线段的起始
		\param end 3d线段结束
		\param color 3d线段的颜色 */
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end, ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//!绘制一条3d三角形
		/** 这个方法对于某些三角形调用drawVertexPrimitiveList
		这个方法能被所有驱动支持，因为它简单调用drawVertexPrimitiveList, 
		但是它允许不是很快。
    
		注意：这个三角形用当前变换矩阵和材质绘制。所以如果你需要绘制不依赖当前变换的3d三角形
		用下列代码：
		 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		在绘制这个三角形前正常的设置材质
		\param triangle 要绘制的三角形.
		\param color 三角形的颜色 */
		virtual void draw3DTriangle(const triangle3df& triangle,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! 绘制一个3D AABB盒子
		/** 这个方法对于盒子的边简单调用draw3DLine 
		注意：这个盒子用当前变换矩阵和材质绘制。所以如果你需要绘制不依赖当前变换的盒子
		用下列代码： 
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		 在绘制这个盒子前正常的设置材质
		 
		\param box 要绘制的AABB盒子
		\param color AABB盒子的颜色 */
		virtual void draw3DBox(const AxisAlignedBox& box,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! 绘制一个不用任何特殊效果的2d图像
		/** \param texture 使用的纹理指针
		\param destPos 要绘制的图像左上角所处的目标2d位置. */
		virtual void draw2DImage(const  ITexture* texture,
			const  Vector2& destPos) = 0;

		//! 用一种颜色绘制一个2D图像
		/** 
		如果颜色不是ColourValue(255,255,255,255)和纹理alpha通道相与
		\param texture 使用的纹理指针
		\param destPos 要绘制的图像左上角所处的目标2d位置
		\param sourceRect 图像的源矩形
		\param clipRect 指向在要剪裁到屏幕上的矩形
		如果指针是空，这个图像不被剪裁
		\param color 这个图像要绘制的颜色。如果这个颜色等于ColourValue(255,255,255,255)，它无效。
		注意：这个alpha分量被使用；如果alpha不是255，这个图像会透明
		\param useAlphaChannelOfTexture: 如果为true，使用这个纹理的alpha通道绘制这个纹理。*/
		virtual void draw2DImage(const  ITexture* texture, const Vector2& destPos,
			const  rect<SINT32>& sourceRect, const  rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false) = 0;

		//! 绘制一个2d图像的集合，用这个纹理的一个颜色和alpha通道
		/** 这个图像绘制在pos开始，并且接连绘制一条直线上。
		所有绘制都会被clipRect剪裁（如果不为0的话）。这子纹理由sourceRects数组和通过给定的索引选择
		\param texture 使用的纹理指针
		\param pos 要绘制的图像左上角所处的目标2d位置
		\param sourceRects 图像的源矩形
		\param indices 索引列表，它选择每次实际使用的矩形
		\param kerningWidth 位置X的偏移量
		\param clipRect 指向在要剪裁到屏幕上的矩形
		如果指针是空，这个图像不被剪裁
		\param color 这个图像要绘制的颜色。如果这个颜色等于ColourValue(255,255,255,255)，它无效。
		注意：这个alpha分量被使用；如果alpha不是255，这个图像会透明
		\param useAlphaChannelOfTexture: 如果为true，使用这个纹理的alpha通道绘制这个纹理。 */
		virtual void draw2DImageBatch(const ITexture* texture,
			const Vector2& pos,
			const vector<rect<SINT32> >& sourceRects,
			const vector<SINT32>& indices,
			SINT32 kerningWidth = 0,
			const  rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! 绘制一个2d图像的集合，用这个纹理的一个颜色和alpha通道
		/** 全部都绘制都被剪裁到一个剪裁区域clipRect（如果不为0的话）
		这个子纹理由sourceRects的数组和使用这个位置数组的位置定义
		\param texture 使用的纹理指针
		\param positions 要绘制的图像左上角所处的目标2d位置
		\param sourceRects 图像的源矩形
		\param clipRect 指向要显示在屏幕上的剪裁区域，图像会被此剪裁
		如果为0，则图像不会被剪裁
		\param color 这个图像要绘制的颜色。如果这个颜色等于ColourValue(255,255,255,255)，它无效。
		注意：这个alpha分量被使用；如果alpha不是255，这个图像会透明
		\param useAlphaChannelOfTexture: 如果为true，使用这个纹理的alpha通道绘制这个纹理。 */
		virtual void draw2DImageBatch(const ITexture* texture,
			const vector<Vector2 >& positions,
			const vector<rect<SINT32> >& sourceRects,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! 绘制在这个纹理的一部分到这个矩形中。注意如果要使用颜色的话，必须是一个4个颜色的数组
		/** 
		\param texture 要绘制的纹理
		\param destRect 图像要绘制入的矩形
		\param sourceRect 这个矩形表示为纹理的一部分
		\param clipRect 剪裁目标矩形(可以是0) 
		\param colors 4个颜色的数组表示目标区域四个角的颜色值
		\param useAlphaChannelOfTexture 如果为true，alpha通道将被混合 */
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue * const colors = 0, bool useAlphaChannelOfTexture = false) = 0;

		//! 绘制一个2D 矩形
		/** \param color 要绘制的矩形的颜色。这个alpha分量不会被丢弃并且指定这个矩形是否应该透明
		\param pos 矩形的位置
		\param clip 指向这个剪裁这个矩形的矩形，如果为NULL，将不会剪裁
		 */
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos,
			const rect<SINT32>* clip = 0) = 0;

		//! 绘制一个渐变的2d矩形
		/** \param colorLeftUp 左上角的颜色
		这个alpha分量不会被丢弃并且指定这个矩形是否应该透明
		\param colorRightUp 右上角的颜色
		这个alpha分量不会被丢弃并且指定这个矩形是否应该透明
		\param colorLeftDown 左下角的颜色
		这个alpha分量不会被丢弃并且指定这个矩形是否应该透明
		\param colorRightDown 右下角的颜色
		这个alpha分量不会被丢弃并且指定这个矩形是否应该透明
		\param pos 矩形的位置
		\param clip 指向这个剪裁这个矩形的矩形，如果为NULL，将不会剪裁 */
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp,
			ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0) = 0;

		//! 绘制一个离线的2d矩形
		/** \param pos 矩形的位置
		\param color 矩形的颜色
		这个alpha分量不会被丢弃并且指定这个矩形是否应该透明*/
		virtual void draw2DRectangleOutline(const recti& pos,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! 绘制一条2d线段
		/** \param start 指向在屏幕的起始像素
		\param end 直线在屏幕的结束像素
		\param color 要绘制的颜色 */
		virtual void draw2DLine(const Vector2& start,
			const Vector2& end,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! 绘制一个像素
		/** \param x 这个像素的X位置
		\param y 这个像素的Y位置
		\param color 这个像素的颜色 */
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue& color) = 0;

		//! 绘制一个非填充的共圆规则的2d多边形
		/** 这个方法用于绘制圆环，但是也能够绘制三角形，四角型，五角型
		,6边形，7边形.... 
		\param center 圆环的中心位置（像素）
		\param radius 圆环像素半径
		\param color 圆环的颜色
		\param vertexCount 这个多边形的顶点数目，决定它的圆滑程度
		*/
		virtual void draw2DPolygon(Vector2 center,
			Real radius,
			ColourValue color = ColourValue(100, 255, 255, 255),
			SINT32 vertexCount = 10) = 0;

		//! 绘制一个阴影容积到模板缓冲区
		/** 绘制一个模板阴影，首先：绘制所有几何体，然后使用这个方法绘制阴影容积。
		用IVideoDriver::drawStencilShadow()去可视化阴影。
		请注意：这只对于openGL版本。
		
		\param triangles 3d向量数组，指定阴影容积
		\param zfail如果设置为true，zfail的方法会被使用，否则使用zpass
		\param debugDataVisible debug数据对于这个阴影节点的开启 
		*/
		virtual void drawStencilShadowVolume(const vector<Vector3>& triangles, bool zfail = true, UINT32 debugDataVisible = 0) = 0;

		//! 用颜色填充模板阴影
		/** IVideoDriver::drawStencilShadowVolume()阴影容积已经被添加到模板缓冲区后，
		用这个函数绘制阴影的颜色。
		请注意：这只对于openGL版本。
		\param clearStencilBuffer 设置为flase，如果你只用同一种颜色绘制每一种阴影，
		那么只调用drawStencilShadow()后所有的阴影容积的都被绘制。如果你想每个阴影
		都要有自己的颜色，设置这个位true，
		\param leftUpEdge 阴影左上角的颜色
		\param rightUpEdge 阴影右上角的颜色
		\param leftDownEdge 阴影左下角的颜色
		\param rightDownEdge 阴影右下角的颜色
		 */
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(255, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(255, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(255, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(255, 0, 0, 0)) = 0;

		//! 绘制一个网格缓冲区
		/** \param mb 要回绘制的网格缓冲区 */
		virtual void drawMeshBuffer(const IMeshBuffer* mb) = 0;

		//! 绘制一个网格的法线缓冲区
		/** \param mb 法线的缓冲区
		\param length 法线的长度缩放因子
		\param color 用于渲染的法线颜色
		*/
		virtual void drawMeshBufferNormals(const IMeshBuffer* mb, Real length = 10.f, ColourValue color = ColourValue(1.0,1.0,1.0,1.0)) = 0;

		//! 设置雾化的模式
		/** 这些是添加到每一个3d对象渲染全局值，将在它的材质里打开雾化标志
		\param color 雾化颜色
		\param fogType 雾化的类型
		\param start 只能用于线性雾化模式 (linearFog=true).指定雾化开始	
		\param end 只能用于线性雾化模式(linearFog=true).指定雾化的结束
		\param density 只能用于指数雾化模式，必须在0与1之间
		\param pixelFog 对于vertex fog顶点雾设置为false, 如果逐像素雾设置为true
		\param rangeFog 设置这个为true打开基于范围顶点雾。这个距离是从观察者来计算雾，不用Z坐标。这样更好，但是会更慢。
		这可能并非所有驱动都支持这个雾化设置
		 */
		virtual void setFog(ColourValue color = ColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false) = 0;

		//! 获取雾化模式
		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog) = 0;

		//! 获取当前颜色缓冲区的颜色模式
		/** \return 颜色缓冲区的颜色模式 */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! 获取渲染窗口的面积
		/** \return 返回渲染窗口的面积 */
		virtual const dimension2d<UINT32>& getScreenSize() const = 0;

		//! 获取当前渲染目标的大小
		/** 如果驱动不支持渲染到纹理，或者当前渲染目标就是屏幕的话，这个方法会翻盘屏幕大小。
		\return 渲染目标/屏幕/窗口的大小 */
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const = 0;

		//! 获取当前每秒帧数值
		/** 这个值是由每1.5秒更新一次的近似值并且只是提供一个粗略的平均帧数参考。
		它不适用用于性能时间计算或依靠帧数率的移动
		\return Approximate 近似的每秒帧数 */
		virtual SINT32 getFPS() const = 0;

		//! 返回最后一帧绘制的图元数量（一般是三角形）
		/** 和getFPS（）一样非常有用的静态方法
		\param mode 定义图元绘制是否被计算或在每一帧中计数
		\return 在上一帧的绘制的图元数 */
		virtual UINT32 getPrimitiveCountDrawn(UINT32 mode = 0) const = 0;

		//! 删除所有之前被addDynamicLight（）添加的的动态光源 
		virtual void deleteAllDynamicLights() = 0;

		//! 添加一个动态光源，返回这个光源的索引
		//! \param light: 用这个光源的数据创建这个光源
		//! \return 一个光源的索引，或-1 发生错误
		virtual SINT32 addDynamicLight(const SLight& light) = 0;

		//! 返回设备能支持的最大的动态光源数量
		/** \return 最大动态光源数目 */
		virtual UINT32 getMaximalDynamicLightAmount() const = 0;

		//! 返回当前设置的动态光源数量
		/** \return 当前设置的动态光源数量 */
		virtual UINT32 getDynamicLightCount() const = 0;

		//! 返回之前IVideoDriver::addDynamicLight()设置的光源数据
		/** \param idx 光源的索引基于0，必须在0或小于IVideoDriver::getDynamicLightCount.
		\return 光源的数据. */
		virtual const SLight& getDynamicLight(UINT32 idx) const = 0;

		//! 打开一个动态光源或者关闭
		//! \param lightIndex: addDynamicLight添加的光源索引
		//! \param turnOn: 如果为true，光源打开，反之关闭
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn) = 0;

		//! 获取视频驱动名
		/** \return 返回视频驱动名，例如Direct3D8，"Direct3D 8.1" */
		virtual const wchar_t* getName() const = 0;

		//! 添加一个外部的图像加载器到这个引擎
		/** 
		这用于引擎加载当前不支持的纹理文件格式。只需实现对应文件的IImageLoader
		一个指针指向新纹理格式的IImageLoader实现，传递给引擎。
		\param loader 指向外部加载器的指针 */
		virtual void addExternalImageLoader(IImageLoader* loader) = 0;

		//! 添加一个外部图像写入器到这个引擎
		/** 这用于引擎写入当前不支持的纹理文件格式。只需实现对应文件的IImageWriter
		一个指针指向新纹理格式的IImageWriter实现，传递给引擎。
		\param writer: 指向外部写入器的指针. */
		virtual void addExternalImageWriter(IImageWriter* writer) = 0;

		//! 返回图元的最大数量
		/** (大多数顶点)这个设备可以通过调用一次drawVertexPrimitiveList进行渲染
		\return 图元的最大数量 */
		virtual UINT32 getMaximalPrimitiveCount() const = 0;

		//! 打开和关闭一各纹理创建标志
		/** 这个标志定义纹理该如何创建，通过改变这个值，可以影响例如渲染速度。
		但是要注意：视频驱动对这个值只做推荐，可能你打开ETCF_ALWAYS_16_BIT，但驱动
		仍然创建32位纹理。
		\param flag 纹理创建标志
		\param enabled 指定给定的标志是否应该打开或关闭 */
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled = true) = 0;

		//! 驱动一个纹理标志的打开或关闭状态
		/** 可以通过 setTextureCreationFlag().设置
		\param flag 纹理创建标志
		\return 当前纹理标志的打开状态 */
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const = 0;

		//! 从一个文件创建一个软件图像
		/** 没有硬件纹理需要从这个图像创建。这个方法用于例如读取地表渲染器的高程图
		\param filename 创建这个图像的文件名
		\return 创建的图像
		 */
		virtual IImage* createImageFromFile(const path& filename) = 0;

		//! 从一个文件创建一个软件图像
		/** 没有硬件纹理需要从这个图像创建。这个方法用于例如读取地表渲染器的高程图
		\param file 创建这个图像的文件名
		\return 创建的图像
		 */
		virtual IImage* createImageFromFile(IReadFile* file) = 0;

		//! 写入图像到文件
		/** 需要注册一个合适的图像写入器
		\param image 写入的图像
		\param filename 写入的文件名
		\param param 后端的控制参数（压缩级别）
		\return 如果写入成功返回true */
		virtual bool writeImageToFile(IImage* image, const path& filename, UINT32 param = 0) = 0;

		//! 写入图像到文件
		/** 需要注册一个合适的图像写入器
		\param image 写入的图像
		\param file  一个已经打开的io::IWriteFile对象. 这个名字要合适使用的图像写入器
		\param param 后端的控制参数（压缩级别）
		\return 如果写入成功返回true. */
		virtual bool writeImageToFile(IImage* image, IWriteFile* file, UINT32 param = 0) = 0;

		//! 从字节数组创建一个软件图像
		/** 没有硬件纹理需要从这个图像创建。这个方法用于例如读取地表渲染器的高程图
		\param format 描述纹理的颜色格式
		\param size 描述图像的大小
		\param data 一个像素颜色信息的字节数组
		\param ownForeignMemory 如果为true， 这个图像之后将直接用这个数据指针，如果不为false，
		内存将在内部拷贝一份
		\param deleteMemory 是否内存由析构器释放
		\return 创建图像
		 */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = false,
			bool deleteMemory = true) = 0;

		//! 创建一个空软件图像
		/**
		\param format 描述颜色图像的格式
		\param size 创建图像的大小 
		\return 创建的图像
		  */
		virtual IImage* createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size) = 0;



		//! 从一个纹理的一部分创建一个软件图像
		/**
		\param texture 要拷贝一部分到新图像的纹理
		\param pos 要复制矩形位置
		\param size 要复制的矩形扩展
		\return 创建的图像
		  */
		virtual IImage* createImage(ITexture* texture,
			const  Vector2& pos,
			const  dimension2d<UINT32>& size) = 0;

		//! 重新改变大小事件的句柄。 只用于引擎内部。
		/** 用于提醒驱动：窗口改变
		通常，这不需要调用这个方法 */
		virtual void OnResize(const  dimension2d<UINT32>& size) = 0;

		//! 添加一个新的材质渲染器到这个驱动
		/** 使用这个方法用新材质类型扩展视频驱动。用这个方法扩展引擎按下面这样做：
		从IMaterialRenderer派生一个类并且重写它你需要的方法。为了设置正确的渲染状态，
		你可以用IVideoDriver::getExposedVideoData()获取一个指向实际渲染设备的指针，
		用IVideoDriver::addMaterialRenderer()添加你的类。要使一个对象显示你的新材质，
		设置这个方法的返回值给SMaterial结构内的材质类型成员的值。如果你只是想让新创建
		材质用于Vertex Shader和Fragment Shader。使用IGPUProgrammingServices接口用调用
		getGPUProgrammingServices()方法更加容易。
		\param renderer 一个新渲染器的指针
		\param name 材质渲染器入口的可选名
		\return 材质类型号，它可以被设置为SMaterial::MaterialType用于渲染。返回-1则是
		发生错误。如你要尝试添加一个材质渲染器到软件渲染器或者空设备，它们不会接受
		 */
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer, const c8* name = 0) = 0;

		//! 通过索引获取一个材质渲染器
		/** \param idx 材质渲染器的ID。可以是E_MATERIAL_TYPE枚举值或者addMaterialRenderer()的返回值
	 
		\return 材质渲染器指针或是空*/
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx) = 0;

		//! 获取当前可用的材质渲染器数量
		/** \return 当前可用的材质渲染器数量*/
		virtual UINT32 getMaterialRendererCount() const = 0;

		//! 获取材质渲染器名
		/**
		这个字符串能够用于测试一个特定的渲染器是否已经被注册或创建，
		或者使用这个字符串保存相关的材质的数据：这个返回的名字将在当
		序列化材质时使用
		\param idx 材质渲染器的ID， 可以是E_MATERIAL_TYPE枚举值或者addMaterialRenderer()的返回值
		\return 这个渲染器的名字，如果是0则不存在 */
		virtual const c8* getMaterialRendererName(UINT32 idx) const = 0;

		//! 设置一个材质渲染器的名字
		/** 对引擎内建的材质类型没有影响
		\param idx: 材质渲染器的ID， 可以是E_MATERIAL_TYPE枚举值或者addMaterialRenderer()的返回值.
		\param name: 这个材质渲染器的新名字 */
		virtual void setMaterialRendererName(SINT32 idx, const c8* name) = 0;

		//! 由一个材质创建材质属性列表
		/** 这个方法用于序列化和其它
		请注意：视频驱动会通过getMaterialRendererName()获取材质渲染器名来输出材质类型名，所以
		应该先设置好它们
		\param options 将影响序列化的附加选项
		\return 保存材质选项的IAttributes容器
	    */
		virtual IAttributes* createAttributesFromMaterial(const SMaterial& material,
			SAttributeReadWriteOptions* options = 0) = 0;

		//! 由一个属性填充一个一个SMaterial结构
		/** 请注意：对于这个材质的材质类型的设置，这个视频驱动将为材质名而查询材质渲染器。
		所以，所以非内置材质必须在这个函数调用前被创建
		\param outMaterial 要设置属性的材质
		\param attributes 要读取的属性 */
		virtual void fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attributes) = 0;

		//! 返回相关IVideoDriver的驱动和操作系统指定数据 
		/** 如果引擎的扩展不修改引擎选代码，应该使用这个方法
		\return 设备依赖指针的集合 */
		virtual const SExposedVideoData& getExposedVideoData() = 0;

		//! 获取视频驱动的类型
		/** \return 驱动的类型 */
		virtual E_DRIVER_TYPE getDriverType() const = 0;

		//! 获取IGPUProgrammingServices的接口
		/** \return 返回 IGPUProgrammingServices的指针，如果视频驱动不支持返回0 */
		virtual IGPUProgrammingServices* getGPUProgrammingServices() = 0;

		//! 返回一个指向网格操作器mesh manipulator的指针
		virtual IMeshManipulator* getMeshManipulator() = 0;

		//! 清除ZBuffer
		/** 注意：你通常不需要调用这个方法，如果你打开Z缓冲，
		它在IVideoDriver::beginScene()或IVideoDriver::setRenderTarget()
		里自动调用。但是如果要渲染一些特别的东西，你可以在渲染过程中的任何
		时候调用这个方法情况Z缓冲
		*/
		virtual void clearZBuffer() = 0;

		//! 截上一个帧缓冲的截图
		/** \return 返回一个上一次帧缓冲的图像*/
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER) = 0;

		//! 检测图像已经是否被加载
		/** 同getTexture()类型，但是如果纹理没加载的话，不会主动加载纹理
		\param filename 纹理的文件名
		\return 指向纹理的指针，0不存在 */
		virtual ITexture* findTexture(const path& filename) = 0;

		//! 设置一个未设置的一个剪裁平面
		/** 对用户这里至少要设置6个有效的剪裁平面
		\param index 这个平面的索引，必须为0到MaxUserClipPlanes.
		\param plane 这个剪裁平面自身
		\param enable 如果为true，这个剪裁平面将被关闭
		\return 如果这个剪裁平面被关闭返回true */
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false) = 0;

		//! 打开或关闭剪裁平面
		/**对用户这里至少要设置6个有效的剪裁平面
		\param index 这个平面的索引，必须为0到MaxUserClipPlanes.
		\param enable 如果为true，这个剪裁平面将被关闭 */
		virtual void enableClipPlane(UINT32 index, bool enable) = 0;

		//! 为将要创建的硬件缓冲区设置最小顶点数
		/** \param count 顶点数量的最小指针 */
		virtual void setMinHardwareBufferVertexCount(UINT32 count) = 0;

		//! 获取全局材质，它可以覆盖局部材质
		/** 依赖打开标志，这个材质的值用于覆盖某些正在渲染的网格缓冲区的局部材质
		\return 覆盖材质的应用 */
		virtual SOverrideMaterial& getOverrideMaterial() = 0;

		//! 为了修改值，获取2d覆盖材质
		/** 
		这个2d覆盖材质允许修改2d方法的中心渲染状态。并不是所有的SMaterial成员都合适。
		特别是材质类型和纹理。此外，zbuffer一直无效，并且光照一直处于关闭。所有其它的
		标志都会改变，虽然在大多数情况下有些影响。
		请注意：可以通过enableInitMaterial2D()打开/关闭这个效果。这个效果消耗比较大
		它增加了考虑到的状态改变的数量。每次这么做的时候都要经常重置这个值。
		\return 反映修改的新设置的材质引用 
		*/
		virtual SMaterial& getMaterial2D() = 0;

		//! 打开2d覆盖材质
		/** \param enable 标志告诉这个材质是否应该打开或关闭 */
		virtual void enableMaterial2D(bool enable = true) = 0;

		//! 获取显卡供应商名字
		virtual String getVendorInfo() = 0;

		//! 只用于引擎内部
		/** 在场景管理器中设置环境光颜色,见ISceneManager::setAmbientLight()
		\param color 新的环境光颜色*/
		virtual void setAmbientLight(const ColourValue& color) = 0;

		//! 只用于引擎内部
		/** 传递全局材质标志AllowZWriteOnTransparent.
		在你的应用中用场景管理器属性来设置这个值
		\param flag 默认行为关闭ZWrite */
		virtual void setAllowZWriteOnTransparent(bool flag) = 0;

		//! 获取支持的最大纹理大小
		virtual dimension2du getMaxTextureSize() const = 0;

		//! 颜色转换函数 
		/** 转移一个图像的（像素数组）从源到目的，从而转换颜色格式。这个像素大小通过颜色格式决定
		\param sP 源指针
		\param sF 源颜色格式
		\param sN 转换的像素数，两个数组必须足够大
		\param dP 目的指针
		\param dF 目的颜色格式
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const = 0;
	};

}



#endif
