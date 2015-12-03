#ifndef _SAPPHIRE_VIDEO_DRVIER_
#define _SAPPHIRE_VIDEO_DRVIER_

#include "SapphireExposedVideoData.h"
#include "SapphireEMaterialFlags.h"
#include "SapphireEDriverFeatures.h"
#include "SapphirePrerequisites.h"
#include "SapphireSMaterial.h"
#include "SapphireRectangle.h"
#include "SapphireIAttributes.h"
#include "SapphireIImageLoader.h"
#include "SapphireIImageWriter.h"
#include "SapphireIMeshBuffer.h"

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
			const scene::IMesh* mesh = 0) = 0;

		//! Remove occlusion query.
		virtual void removeOcclusionQuery(scene::ISceneNode* node) = 0;

		//! Remove all occlusion queries.
		virtual void removeAllOcclusionQueries() = 0;

		//! Run occlusion query. Draws mesh stored in query.
		/** If the mesh shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runOcclusionQuery(scene::ISceneNode* node, bool visible = false) = 0;

		//! Run all occlusion queries. Draws all meshes stored in queries.
		/** If the meshes shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runAllOcclusionQueries(bool visible = false) = 0;

		//! Update occlusion query. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateOcclusionQuery(scene::ISceneNode* node, bool block = true) = 0;

		//! Update all occlusion queries. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateAllOcclusionQueries(bool block = true) = 0;

		//! Return query result.
		/** Return value is the number of visible pixels/fragments.
		The value is a safe approximation, i.e. can be larger than the
		actual value of pixels. */
		virtual UINT32 getOcclusionQueryResult(scene::ISceneNode* node) const = 0;

		//! Sets a boolean alpha channel on the texture based on a color key.
		/** This makes the texture fully transparent at the texels where
		this color key can be found when using for example draw2DImage
		with useAlphachannel==true.  The alpha of other texels is not modified.
		\param texture Texture whose alpha channel is modified.
		\param color Color key color. Every texel with this color will
		become fully transparent as described above. Please note that the
		colors of a texture may be converted when loading it, so the
		color values may not be exactly the same in the engine and for
		example in picture edit programs. To avoid this problem, you
		could use the makeColorKeyTexture method, which takes the
		position of a pixel instead a color value.
		\param zeroTexels \deprecated If set to true, then any texels that match
		the color key will have their color, as well as their alpha, set to zero
		(i.e. black). This behavior matches the legacy (buggy) behavior prior
		to release 1.5 and is provided for backwards compatibility only.
		This parameter may be removed by Irrlicht 1.9. */
		virtual void makeColorKeyTexture(video::ITexture* texture,
			video::ColourValue color,
			bool zeroTexels = false) const = 0;

		//! Sets a boolean alpha channel on the texture based on the color at a position.
		/** This makes the texture fully transparent at the texels where
		the color key can be found when using for example draw2DImage
		with useAlphachannel==true.  The alpha of other texels is not modified.
		\param texture Texture whose alpha channel is modified.
		\param colorKeyPixelPos Position of a pixel with the color key
		color. Every texel with this color will become fully transparent as
		described above.
		\param zeroTexels \deprecated If set to true, then any texels that match
		the color key will have their color, as well as their alpha, set to zero
		(i.e. black). This behavior matches the legacy (buggy) behavior prior
		to release 1.5 and is provided for backwards compatibility only.
		This parameter may be removed by Irrlicht 1.9. */
		virtual void makeColorKeyTexture(video::ITexture* texture,
			core::position2d<SINT32> colorKeyPixelPos,
			bool zeroTexels = false) const = 0;

		//! Creates a normal map from a height map texture.
		/** If the target texture has 32 bit, the height value is
		stored in the alpha component of the texture as addition. This
		value is used by the video::EMT_PARALLAX_MAP_SOLID material and
		similar materials.
		\param texture Texture whose alpha channel is modified.
		\param amplitude Constant value by which the height
		information is multiplied.*/
		virtual void makeNormalMapTexture(video::ITexture* texture, Real amplitude = 1.0f) const = 0;

		//! Sets a new render target.
		/** This will only work if the driver supports the
		EVDF_RENDER_TO_TARGET feature, which can be queried with
		queryFeature(). Usually, rendering to textures is done in this
		way:
		\code
		// create render target
		ITexture* target = driver->addRenderTargetTexture(core::dimension2d<UINT32>(128,128), "rtt1");

		// ...

		driver->setRenderTarget(target); // set render target
		// .. draw stuff here
		driver->setRenderTarget(0); // set previous render target
		\endcode
		Please note that you cannot render 3D or 2D geometry with a
		render target as texture on it when you are rendering the scene
		into this render target at the same time. It is usually only
		possible to render into a texture between the
		IVideoDriver::beginScene() and endScene() method calls.
		\param texture New render target. Must be a texture created with
		IVideoDriver::addRenderTargetTexture(). If set to 0, it sets
		the previous render target which was set before the last
		setRenderTarget() call.
		\param clearBackBuffer Clears the backbuffer of the render
		target with the color parameter
		\param clearZBuffer Clears the zBuffer of the rendertarget.
		Note that because the frame buffer may share the zbuffer with
		the rendertarget, its zbuffer might be partially cleared too
		by this.
		\param color The background color for the render target.
		\return True if sucessful and false if not. */
		virtual bool setRenderTarget(video::ITexture* texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = video::ColourValue(0, 0, 0, 0)) = 0;

		//! set or reset special render targets
		/** This method enables access to special color buffers such as
		stereoscopic buffers or auxiliary buffers.
		\param target Enum value for the render target
		\param clearTarget Clears the target buffer with the color
		parameter
		\param clearZBuffer Clears the zBuffer of the rendertarget.
		Note that because the main frame buffer may share the zbuffer with
		the rendertarget, its zbuffer might be partially cleared too
		by this.
		\param color The background color for the render target.
		\return True if sucessful and false if not. */
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget = true,
			bool clearZBuffer = true,
			ColourValue color = video::ColourValue(0, 0, 0, 0)) = 0;

		//! Sets new multiple render targets.
		virtual bool setRenderTarget(const core::array<video::IRenderTarget>& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true,
			ColourValue color = video::ColourValue(0, 0, 0, 0)) = 0;

		//! Sets a new viewport.
		/** Every rendering operation is done into this new area.
		\param area: Rectangle defining the new area of rendering
		operations. */
		virtual void setViewPort(const core::rect<SINT32>& area) = 0;

		//! Gets the area of the current viewport.
		/** \return Rectangle of the current viewport. */
		virtual const rect<SINT32>& getViewPort() const = 0;

		//! Draws a vertex primitive list
		/** Note that, depending on the index type, some vertices might be not
		accessible through the index list. The limit is at 65535 vertices for 16bit
		indices. Please note that currently not all primitives are available for
		all drivers, and some might be emulated via triangle renders.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices. These define the vertices used
		for each primitive. Depending on the pType, indices are interpreted as single
		objects (for point like primitives), pairs (for lines), triplets (for
		triangles), or quads.
		\param primCount Amount of Primitives
		\param vType Vertex type, e.g. video::EVT_STANDARD for S3DVertex.
		\param pType Primitive type, e.g. scene::EPT_TRIANGLE_FAN for a triangle fan.
		\param iType Index type, e.g. video::EIT_16BIT for 16bit indices. */
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			scene::E_PRIMITIVE_TYPE pType = scene::EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! Draws a vertex primitive list in 2d
		/** Compared to the general (3d) version of this method, this
		one sets up a 2d render mode, and uses only x and y of vectors.
		Note that, depending on the index type, some vertices might be
		not accessible through the index list. The limit is at 65535
		vertices for 16bit indices. Please note that currently not all
		primitives are available for all drivers, and some might be
		emulated via triangle renders. This function is not available
		for the sw drivers.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices. These define the
		vertices used for each primitive. Depending on the pType,
		indices are interpreted as single objects (for point like
		primitives), pairs (for lines), triplets (for triangles), or
		quads.
		\param primCount Amount of Primitives
		\param vType Vertex type, e.g. video::EVT_STANDARD for S3DVertex.
		\param pType Primitive type, e.g. scene::EPT_TRIANGLE_FAN for a triangle fan.
		\param iType Index type, e.g. video::EIT_16BIT for 16bit indices. */
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primCount,
			E_VERTEX_TYPE vType = EVT_STANDARD,
			scene::E_PRIMITIVE_TYPE pType = scene::EPT_TRIANGLES,
			E_INDEX_TYPE iType = EIT_16BIT) = 0;

		//! Draws an indexed triangle list.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices / 3. */
		void drawIndexedTriangleList(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, scene::EPT_TRIANGLES, EIT_16BIT);
		}

		//! Draws an indexed triangle list.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices / 3. */
		void drawIndexedTriangleList(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, scene::EPT_TRIANGLES, EIT_16BIT);
		}

		//! Draws an indexed triangle list.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices / 3. */
		void drawIndexedTriangleList(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS, scene::EPT_TRIANGLES, EIT_16BIT);
		}

		//! Draws an indexed triangle fan.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices - 2. */
		void drawIndexedTriangleFan(const S3DVertex* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! Draws an indexed triangle fan.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices - 2. */
		void drawIndexedTriangleFan(const S3DVertex2TCoords* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_2TCOORDS, scene::EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! Draws an indexed triangle fan.
		/** Note that there may be at maximum 65536 vertices, because
		the index list is an array of 16 bit values each with a maximum
		value of 65536. If there are more than 65536 vertices in the
		list, results of this operation are not defined.
		\param vertices Pointer to array of vertices.
		\param vertexCount Amount of vertices in the array.
		\param indexList Pointer to array of indices.
		\param triangleCount Amount of Triangles. Usually amount of indices - 2. */
		void drawIndexedTriangleFan(const S3DVertexTangents* vertices,
			UINT32 vertexCount, const UINT16* indexList, UINT32 triangleCount)
		{
			drawVertexPrimitiveList(vertices, vertexCount, indexList, triangleCount, EVT_TANGENTS, scene::EPT_TRIANGLE_FAN, EIT_16BIT);
		}

		//! Draws a 3d line.
		/** For some implementations, this method simply calls
		drawVertexPrimitiveList for some triangles.
		Note that the line is drawn using the current transformation
		matrix and material. So if you need to draw the 3D line
		independently of the current transformation, use
		\code
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		\endcode
		for some properly set up material before drawing the line.
		Some drivers support line thickness set in the material.
		\param start Start of the 3d line.
		\param end End of the 3d line.
		\param color Color of the line. */
		virtual void draw3DLine(const core::vector3df& start,
			const core::vector3df& end, ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! Draws a 3d triangle.
		/** This method calls drawVertexPrimitiveList for some triangles.
		This method works with all drivers because it simply calls
		drawVertexPrimitiveList, but it is hence not very fast.
		Note that the triangle is drawn using the current
		transformation matrix and material. So if you need to draw it
		independently of the current transformation, use
		\code
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		\endcode
		for some properly set up material before drawing the triangle.
		\param triangle The triangle to draw.
		\param color Color of the line. */
		virtual void draw3DTriangle(const core::triangle3df& triangle,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! Draws a 3d axis aligned box.
		/** This method simply calls draw3DLine for the edges of the
		box. Note that the box is drawn using the current transformation
		matrix and material. So if you need to draw it independently of
		the current transformation, use
		\code
		driver->setMaterial(someMaterial);
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		\endcode
		for some properly set up material before drawing the box.
		\param box The axis aligned box to draw
		\param color Color to use while drawing the box. */
		virtual void draw3DBox(const core::aabbox3d<Real>& box,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! Draws a 2d image without any special effects
		/** \param texture Pointer to texture to use.
		\param destPos Upper left 2d destination position where the
		image will be drawn. */
		virtual void draw2DImage(const video::ITexture* texture,
			const core::position2d<SINT32>& destPos) = 0;

		//! Draws a 2d image using a color
		/** (if color is other than
		Color(255,255,255,255)) and the alpha channel of the texture.
		\param texture Texture to be drawn.
		\param destPos Upper left 2d destination position where the
		image will be drawn.
		\param sourceRect Source rectangle in the image.
		\param clipRect Pointer to rectangle on the screen where the
		image is clipped to.
		If this pointer is NULL the image is not clipped.
		\param color Color with which the image is drawn. If the color
		equals Color(255,255,255,255) it is ignored. Note that the
		alpha component is used: If alpha is other than 255, the image
		will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of
		the texture is used to draw the image.*/
		virtual void draw2DImage(const video::ITexture* texture, const core::position2d<SINT32>& destPos,
			const core::rect<SINT32>& sourceRect, const core::rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false) = 0;

		//! Draws a set of 2d images, using a color and the alpha channel of the texture.
		/** The images are drawn beginning at pos and concatenated in
		one line. All drawings are clipped against clipRect (if != 0).
		The subtextures are defined by the array of sourceRects and are
		chosen by the indices given.
		\param texture Texture to be drawn.
		\param pos Upper left 2d destination position where the image
		will be drawn.
		\param sourceRects Source rectangles of the image.
		\param indices List of indices which choose the actual
		rectangle used each time.
		\param kerningWidth Offset to Position on X
		\param clipRect Pointer to rectangle on the screen where the
		image is clipped to.
		If this pointer is 0 then the image is not clipped.
		\param color Color with which the image is drawn.
		Note that the alpha component is used. If alpha is other than
		255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of
		the texture is used to draw the image. */
		virtual void draw2DImageBatch(const video::ITexture* texture,
			const core::position2d<SINT32>& pos,
			const core::array<core::rect<SINT32> >& sourceRects,
			const core::array<SINT32>& indices,
			SINT32 kerningWidth = 0,
			const core::rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! Draws a set of 2d images, using a color and the alpha channel of the texture.
		/** All drawings are clipped against clipRect (if != 0).
		The subtextures are defined by the array of sourceRects and are
		positioned using the array of positions.
		\param texture Texture to be drawn.
		\param positions Array of upper left 2d destinations where the
		images will be drawn.
		\param sourceRects Source rectangles of the image.
		\param clipRect Pointer to rectangle on the screen where the
		images are clipped to.
		If this pointer is 0 then the image is not clipped.
		\param color Color with which the image is drawn.
		Note that the alpha component is used. If alpha is other than
		255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of
		the texture is used to draw the image. */
		virtual void draw2DImageBatch(const video::ITexture* texture,
			const core::array<core::position2d<SINT32> >& positions,
			const core::array<core::rect<SINT32> >& sourceRects,
			const core::rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false) = 0;

		//! Draws a part of the texture into the rectangle. Note that colors must be an array of 4 colors if used.
		/** Suggested and first implemented by zola.
		\param texture The texture to draw from
		\param destRect The rectangle to draw into
		\param sourceRect The rectangle denoting a part of the texture
		\param clipRect Clips the destination rectangle (may be 0)
		\param colors Array of 4 colors denoting the color values of
		the corners of the destRect
		\param useAlphaChannelOfTexture True if alpha channel will be
		blended. */
		virtual void draw2DImage(const video::ITexture* texture, const core::rect<SINT32>& destRect,
			const core::rect<SINT32>& sourceRect, const core::rect<SINT32>* clipRect = 0,
			const video::ColourValue * const colors = 0, bool useAlphaChannelOfTexture = false) = 0;

		//! Draws a 2d rectangle.
		/** \param color Color of the rectangle to draw. The alpha
		component will not be ignored and specifies how transparent the
		rectangle will be.
		\param pos Position of the rectangle.
		\param clip Pointer to rectangle against which the rectangle
		will be clipped. If the pointer is null, no clipping will be
		performed. */
		virtual void draw2DRectangle(ColourValue color, const core::rect<SINT32>& pos,
			const core::rect<SINT32>* clip = 0) = 0;

		//! Draws a 2d rectangle with a gradient.
		/** \param colorLeftUp Color of the upper left corner to draw.
		The alpha component will not be ignored and specifies how
		transparent the rectangle will be.
		\param colorRightUp Color of the upper right corner to draw.
		The alpha component will not be ignored and specifies how
		transparent the rectangle will be.
		\param colorLeftDown Color of the lower left corner to draw.
		The alpha component will not be ignored and specifies how
		transparent the rectangle will be.
		\param colorRightDown Color of the lower right corner to draw.
		The alpha component will not be ignored and specifies how
		transparent the rectangle will be.
		\param pos Position of the rectangle.
		\param clip Pointer to rectangle against which the rectangle
		will be clipped. If the pointer is null, no clipping will be
		performed. */
		virtual void draw2DRectangle(const core::rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp,
			ColourValue colorLeftDown, ColourValue colorRightDown,
			const core::rect<SINT32>* clip = 0) = 0;

		//! Draws the outline of a 2D rectangle.
		/** \param pos Position of the rectangle.
		\param color Color of the rectangle to draw. The alpha component
		specifies how transparent the rectangle outline will be. */
		virtual void draw2DRectangleOutline(const core::recti& pos,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! Draws a 2d line. Both start and end will be included in coloring.
		/** \param start Screen coordinates of the start of the line
		in pixels.
		\param end Screen coordinates of the start of the line in
		pixels.
		\param color Color of the line to draw. */
		virtual void draw2DLine(const core::position2d<SINT32>& start,
			const core::position2d<SINT32>& end,
			ColourValue color = ColourValue(255, 255, 255, 255)) = 0;

		//! Draws a pixel.
		/** \param x The x-position of the pixel.
		\param y The y-position of the pixel.
		\param color Color of the pixel to draw. */
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue& color) = 0;

		//! Draws a non filled concyclic regular 2d polyon.
		/** This method can be used to draw circles, but also
		triangles, tetragons, pentagons, hexagons, heptagons, octagons,
		enneagons, decagons, hendecagons, dodecagon, triskaidecagons,
		etc. I think you'll got it now. And all this by simply
		specifying the vertex count. Welcome to the wonders of
		geometry.
		\param center Position of center of circle (pixels).
		\param radius Radius of circle in pixels.
		\param color Color of the circle.
		\param vertexCount Amount of vertices of the polygon. Specify 2
		to draw a line, 3 to draw a triangle, 4 for tetragons and a lot
		(>10) for nearly a circle. */
		virtual void draw2DPolygon(core::position2d<SINT32> center,
			Real radius,
			video::ColourValue color = ColourValue(100, 255, 255, 255),
			SINT32 vertexCount = 10) = 0;

		//! Draws a shadow volume into the stencil buffer.
		/** To draw a stencil shadow, do this: First, draw all geometry.
		Then use this method, to draw the shadow volume. Then, use
		IVideoDriver::drawStencilShadow() to visualize the shadow.
		Please note that the code for the opengl version of the method
		is based on free code sent in by Philipp Dortmann, lots of
		thanks go to him!
		\param triangles Array of 3d vectors, specifying the shadow
		volume.
		\param zfail If set to true, zfail method is used, otherwise
		zpass.
		\param debugDataVisible The debug data that is enabled for this
		shadow node
		*/
		virtual void drawStencilShadowVolume(const core::array<core::vector3df>& triangles, bool zfail = true, UINT32 debugDataVisible = 0) = 0;

		//! Fills the stencil shadow with color.
		/** After the shadow volume has been drawn into the stencil
		buffer using IVideoDriver::drawStencilShadowVolume(), use this
		to draw the color of the shadow.
		Please note that the code for the opengl version of the method
		is based on free code sent in by Philipp Dortmann, lots of
		thanks go to him!
		\param clearStencilBuffer Set this to false, if you want to
		draw every shadow with the same color, and only want to call
		drawStencilShadow() once after all shadow volumes have been
		drawn. Set this to true, if you want to paint every shadow with
		its own color.
		\param leftUpEdge Color of the shadow in the upper left corner
		of screen.
		\param rightUpEdge Color of the shadow in the upper right
		corner of screen.
		\param leftDownEdge Color of the shadow in the lower left
		corner of screen.
		\param rightDownEdge Color of the shadow in the lower right
		corner of screen. */
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			video::ColourValue leftUpEdge = video::ColourValue(255, 0, 0, 0),
			video::ColourValue rightUpEdge = video::ColourValue(255, 0, 0, 0),
			video::ColourValue leftDownEdge = video::ColourValue(255, 0, 0, 0),
			video::ColourValue rightDownEdge = video::ColourValue(255, 0, 0, 0)) = 0;

		//! Draws a mesh buffer
		/** \param mb Buffer to draw */
		virtual void drawMeshBuffer(const scene::IMeshBuffer* mb) = 0;

		//! Draws normals of a mesh buffer
		/** \param mb Buffer to draw the normals of
		\param length length scale factor of the normals
		\param color Color the normals are rendered with
		*/
		virtual void drawMeshBufferNormals(const scene::IMeshBuffer* mb, Real length = 10.f, ColourValue color = 0xffffffff) = 0;

		//! Sets the fog mode.
		/** These are global values attached to each 3d object rendered,
		which has the fog flag enabled in its material.
		\param color Color of the fog
		\param fogType Type of fog used
		\param start Only used in linear fog mode (linearFog=true).
		Specifies where fog starts.
		\param end Only used in linear fog mode (linearFog=true).
		Specifies where fog ends.
		\param density Only used in exponential fog mode
		(linearFog=false). Must be a value between 0 and 1.
		\param pixelFog Set this to false for vertex fog, and true if
		you want per-pixel fog.
		\param rangeFog Set this to true to enable range-based vertex
		fog. The distance from the viewer is used to compute the fog,
		not the z-coordinate. This is better, but slower. This might not
		be available with all drivers and fog settings. */
		virtual void setFog(ColourValue color = ColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false) = 0;

		//! Gets the fog mode.
		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog) = 0;

		//! Get the current color format of the color buffer
		/** \return Color format of the color buffer. */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! Get the size of the screen or render window.
		/** \return Size of screen or render window. */
		virtual const core::dimension2d<UINT32>& getScreenSize() const = 0;

		//! Get the size of the current render target
		/** This method will return the screen size if the driver
		doesn't support render to texture, or if the current render
		target is the screen.
		\return Size of render target or screen/window */
		virtual const core::dimension2d<UINT32>& getCurrentRenderTargetSize() const = 0;

		//! Returns current frames per second value.
		/** This value is updated approximately every 1.5 seconds and
		is only intended to provide a rough guide to the average frame
		rate. It is not suitable for use in performing timing
		calculations or framerate independent movement.
		\return Approximate amount of frames per second drawn. */
		virtual SINT32 getFPS() const = 0;

		//! Returns amount of primitives (mostly triangles) which were drawn in the last frame.
		/** Together with getFPS() very useful method for statistics.
		\param mode Defines if the primitives drawn are accumulated or
		counted per frame.
		\return Amount of primitives drawn in the last frame. */
		virtual UINT32 getPrimitiveCountDrawn(UINT32 mode = 0) const = 0;

		//! Deletes all dynamic lights which were previously added with addDynamicLight().
		virtual void deleteAllDynamicLights() = 0;

		//! adds a dynamic light, returning an index to the light
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual SINT32 addDynamicLight(const SLight& light) = 0;

		//! Returns the maximal amount of dynamic lights the device can handle
		/** \return Maximal amount of dynamic lights. */
		virtual UINT32 getMaximalDynamicLightAmount() const = 0;

		//! Returns amount of dynamic lights currently set
		/** \return Amount of dynamic lights currently set */
		virtual UINT32 getDynamicLightCount() const = 0;

		//! Returns light data which was previously set by IVideoDriver::addDynamicLight().
		/** \param idx Zero based index of the light. Must be 0 or
		greater and smaller than IVideoDriver::getDynamicLightCount.
		\return Light data. */
		virtual const SLight& getDynamicLight(UINT32 idx) const = 0;

		//! Turns a dynamic light on or off
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn) = 0;

		//! Gets name of this video driver.
		/** \return Returns the name of the video driver, e.g. in case
		of the Direct3D8 driver, it would return "Direct3D 8.1". */
		virtual const wchar_t* getName() const = 0;

		//! Adds an external image loader to the engine.
		/** This is useful if the Irrlicht Engine should be able to load
		textures of currently unsupported file formats (e.g. gif). The
		IImageLoader only needs to be implemented for loading this file
		format. A pointer to the implementation can be passed to the
		engine using this method.
		\param loader Pointer to the external loader created. */
		virtual void addExternalImageLoader(IImageLoader* loader) = 0;

		//! Adds an external image writer to the engine.
		/** This is useful if the Irrlicht Engine should be able to
		write textures of currently unsupported file formats (e.g
		.gif). The IImageWriter only needs to be implemented for
		writing this file format. A pointer to the implementation can
		be passed to the engine using this method.
		\param writer: Pointer to the external writer created. */
		virtual void addExternalImageWriter(IImageWriter* writer) = 0;

		//! Returns the maximum amount of primitives
		/** (mostly vertices) which the device is able to render with
		one drawVertexPrimitiveList call.
		\return Maximum amount of primitives. */
		virtual UINT32 getMaximalPrimitiveCount() const = 0;

		//! Enables or disables a texture creation flag.
		/** These flags define how textures should be created. By
		changing this value, you can influence for example the speed of
		rendering a lot. But please note that the video drivers take
		this value only as recommendation. It could happen that you
		enable the ETCF_ALWAYS_16_BIT mode, but the driver still creates
		32 bit textures.
		\param flag Texture creation flag.
		\param enabled Specifies if the given flag should be enabled or
		disabled. */
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled = true) = 0;

		//! Returns if a texture creation flag is enabled or disabled.
		/** You can change this value using setTextureCreationFlag().
		\param flag Texture creation flag.
		\return The current texture creation flag enabled mode. */
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const = 0;

		//! Creates a software image from a file.
		/** No hardware texture will be created for this image. This
		method is useful for example if you want to read a heightmap
		for a terrain renderer.
		\param filename Name of the file from which the image is
		created.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IImage* createImageFromFile(const io::path& filename) = 0;

		//! Creates a software image from a file.
		/** No hardware texture will be created for this image. This
		method is useful for example if you want to read a heightmap
		for a terrain renderer.
		\param file File from which the image is created.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IImage* createImageFromFile(io::IReadFile* file) = 0;

		//! Writes the provided image to a file.
		/** Requires that there is a suitable image writer registered
		for writing the image.
		\param image Image to write.
		\param filename Name of the file to write.
		\param param Control parameter for the backend (e.g. compression
		level).
		\return True on successful write. */
		virtual bool writeImageToFile(IImage* image, const io::path& filename, UINT32 param = 0) = 0;

		//! Writes the provided image to a file.
		/** Requires that there is a suitable image writer registered
		for writing the image.
		\param image Image to write.
		\param file  An already open io::IWriteFile object. The name
		will be used to determine the appropriate image writer to use.
		\param param Control parameter for the backend (e.g. compression
		level).
		\return True on successful write. */
		virtual bool writeImageToFile(IImage* image, io::IWriteFile* file, UINT32 param = 0) = 0;

		//! Creates a software image from a byte array.
		/** No hardware texture will be created for this image. This
		method is useful for example if you want to read a heightmap
		for a terrain renderer.
		\param format Desired color format of the texture
		\param size Desired size of the image
		\param data A byte array with pixel color information
		\param ownForeignMemory If true, the image will use the data
		pointer directly and own it afterwards. If false, the memory
		will by copied internally.
		\param deleteMemory Whether the memory is deallocated upon
		destruction.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const core::dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = false,
			bool deleteMemory = true) = 0;

		//! Creates an empty software image.
		/**
		\param format Desired color format of the image.
		\param size Size of the image to create.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IImage* createImage(ECOLOR_FORMAT format, const core::dimension2d<UINT32>& size) = 0;

		//! Creates a software image by converting it to given format from another image.
		/** \deprecated Create an empty image and use copyTo(). This method may be removed by Irrlicht 1.9.
		\param format Desired color format of the image.
		\param imageToCopy Image to copy to the new image.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		_IRR_DEPRECATED_ virtual IImage* createImage(ECOLOR_FORMAT format, IImage *imageToCopy) = 0;

		//! Creates a software image from a part of another image.
		/** \deprecated Create an empty image and use copyTo(). This method may be removed by Irrlicht 1.9.
		\param imageToCopy Image to copy to the new image in part.
		\param pos Position of rectangle to copy.
		\param size Extents of rectangle to copy.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		_IRR_DEPRECATED_ virtual IImage* createImage(IImage* imageToCopy,
			const core::position2d<SINT32>& pos,
			const core::dimension2d<UINT32>& size) = 0;

		//! Creates a software image from a part of a texture.
		/**
		\param texture Texture to copy to the new image in part.
		\param pos Position of rectangle to copy.
		\param size Extents of rectangle to copy.
		\return The created image.
		If you no longer need the image, you should call IImage::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IImage* createImage(ITexture* texture,
			const core::position2d<SINT32>& pos,
			const core::dimension2d<UINT32>& size) = 0;

		//! Event handler for resize events. Only used by the engine internally.
		/** Used to notify the driver that the window was resized.
		Usually, there is no need to call this method. */
		virtual void OnResize(const core::dimension2d<UINT32>& size) = 0;

		//! Adds a new material renderer to the video device.
		/** Use this method to extend the VideoDriver with new material
		types. To extend the engine using this method do the following:
		Derive a class from IMaterialRenderer and override the methods
		you need. For setting the right renderstates, you can try to
		get a pointer to the real rendering device using
		IVideoDriver::getExposedVideoData(). Add your class with
		IVideoDriver::addMaterialRenderer(). To use an object being
		displayed with your new material, set the MaterialType member of
		the SMaterial struct to the value returned by this method.
		If you simply want to create a new material using vertex and/or
		pixel shaders it would be easier to use the
		video::IGPUProgrammingServices interface which you can get
		using the getGPUProgrammingServices() method.
		\param renderer A pointer to the new renderer.
		\param name Optional name for the material renderer entry.
		\return The number of the material type which can be set in
		SMaterial::MaterialType to use the renderer. -1 is returned if
		an error occured. For example if you tried to add an material
		renderer to the software renderer or the null device, which do
		not accept material renderers. */
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer, const c8* name = 0) = 0;

		//! Get access to a material renderer by index.
		/** \param idx Id of the material renderer. Can be a value of
		the E_MATERIAL_TYPE enum or a value which was returned by
		addMaterialRenderer().
		\return Pointer to material renderer or null if not existing. */
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx) = 0;

		//! Get amount of currently available material renderers.
		/** \return Amount of currently available material renderers. */
		virtual UINT32 getMaterialRendererCount() const = 0;

		//! Get name of a material renderer
		/** This string can, e.g., be used to test if a specific
		renderer already has been registered/created, or use this
		string to store data about materials: This returned name will
		be also used when serializing materials.
		\param idx Id of the material renderer. Can be a value of the
		E_MATERIAL_TYPE enum or a value which was returned by
		addMaterialRenderer().
		\return String with the name of the renderer, or 0 if not
		exisiting */
		virtual const c8* getMaterialRendererName(UINT32 idx) const = 0;

		//! Sets the name of a material renderer.
		/** Will have no effect on built-in material renderers.
		\param idx: Id of the material renderer. Can be a value of the
		E_MATERIAL_TYPE enum or a value which was returned by
		addMaterialRenderer().
		\param name: New name of the material renderer. */
		virtual void setMaterialRendererName(SINT32 idx, const c8* name) = 0;

		//! Creates material attributes list from a material
		/** This method is useful for serialization and more.
		Please note that the video driver will use the material
		renderer names from getMaterialRendererName() to write out the
		material type name, so they should be set before.
		\param material The material to serialize.
		\param options Additional options which might influence the
		serialization.
		\return The io::IAttributes container holding the material
		properties. */
		virtual io::IAttributes* createAttributesFromMaterial(const video::SMaterial& material,
			io::SAttributeReadWriteOptions* options = 0) = 0;

		//! Fills an SMaterial structure from attributes.
		/** Please note that for setting material types of the
		material, the video driver will need to query the material
		renderers for their names, so all non built-in materials must
		have been created before calling this method.
		\param outMaterial The material to set the properties for.
		\param attributes The attributes to read from. */
		virtual void fillMaterialStructureFromAttributes(video::SMaterial& outMaterial, io::IAttributes* attributes) = 0;

		//! Returns driver and operating system specific data about the IVideoDriver.
		/** This method should only be used if the engine should be
		extended without having to modify the source of the engine.
		\return Collection of device dependent pointers. */
		virtual const SExposedVideoData& getExposedVideoData() = 0;

		//! Get type of video driver
		/** \return Type of driver. */
		virtual E_DRIVER_TYPE getDriverType() const = 0;

		//! Gets the IGPUProgrammingServices interface.
		/** \return Pointer to the IGPUProgrammingServices. Returns 0
		if the video driver does not support this. For example the
		Software driver and the Null driver will always return 0. */
		virtual IGPUProgrammingServices* getGPUProgrammingServices() = 0;

		//! Returns a pointer to the mesh manipulator.
		virtual scene::IMeshManipulator* getMeshManipulator() = 0;

		//! Clears the ZBuffer.
		/** Note that you usually need not to call this method, as it
		is automatically done in IVideoDriver::beginScene() or
		IVideoDriver::setRenderTarget() if you enable zBuffer. But if
		you have to render some special things, you can clear the
		zbuffer during the rendering process with this method any time.
		*/
		virtual void clearZBuffer() = 0;

		//! Make a screenshot of the last rendered frame.
		/** \return An image created from the last rendered frame. */
		virtual IImage* createScreenShot(video::ECOLOR_FORMAT format = video::ECF_UNKNOWN, video::E_RENDER_TARGET target = video::ERT_FRAME_BUFFER) = 0;

		//! Check if the image is already loaded.
		/** Works similar to getTexture(), but does not load the texture
		if it is not currently loaded.
		\param filename Name of the texture.
		\return Pointer to loaded texture, or 0 if not found. */
		virtual video::ITexture* findTexture(const io::path& filename) = 0;

		//! Set or unset a clipping plane.
		/** There are at least 6 clipping planes available for the user
		to set at will.
		\param index The plane index. Must be between 0 and
		MaxUserClipPlanes.
		\param plane The plane itself.
		\param enable If true, enable the clipping plane else disable
		it.
		\return True if the clipping plane is usable. */
		virtual bool setClipPlane(UINT32 index, const core::plane3df& plane, bool enable = false) = 0;

		//! Enable or disable a clipping plane.
		/** There are at least 6 clipping planes available for the user
		to set at will.
		\param index The plane index. Must be between 0 and
		MaxUserClipPlanes.
		\param enable If true, enable the clipping plane else disable
		it. */
		virtual void enableClipPlane(UINT32 index, bool enable) = 0;

		//! Set the minimum number of vertices for which a hw buffer will be created
		/** \param count Number of vertices to set as minimum. */
		virtual void setMinHardwareBufferVertexCount(UINT32 count) = 0;

		//! Get the global Material, which might override local materials.
		/** Depending on the enable flags, values from this Material
		are used to override those of local materials of some
		meshbuffer being rendered.
		\return Reference to the Override Material. */
		virtual SOverrideMaterial& getOverrideMaterial() = 0;

		//! Get the 2d override material for altering its values
		/** The 2d override materual allows to alter certain render
		states of the 2d methods. Not all members of SMaterial are
		honored, especially not MaterialType and Textures. Moreover,
		the zbuffer is always ignored, and lighting is always off. All
		other flags can be changed, though some might have to effect
		in most cases.
		Please note that you have to enable/disable this effect with
		enableInitMaterial2D(). This effect is costly, as it increases
		the number of state changes considerably. Always reset the
		values when done.
		\return Material reference which should be altered to reflect
		the new settings.
		*/
		virtual SMaterial& getMaterial2D() = 0;

		//! Enable the 2d override material
		/** \param enable Flag which tells whether the material shall be
		enabled or disabled. */
		virtual void enableMaterial2D(bool enable = true) = 0;

		//! Get the graphics card vendor name.
		virtual core::stringc getVendorInfo() = 0;

		//! Only used by the engine internally.
		/** The ambient color is set in the scene manager, see
		scene::ISceneManager::setAmbientLight().
		\param color New color of the ambient light. */
		virtual void setAmbientLight(const ColourValuef& color) = 0;

		//! Only used by the engine internally.
		/** Passes the global material flag AllowZWriteOnTransparent.
		Use the SceneManager attribute to set this value from your app.
		\param flag Default behavior is to disable ZWrite, i.e. false. */
		virtual void setAllowZWriteOnTransparent(bool flag) = 0;

		//! Get the maximum texture size supported.
		virtual core::dimension2du getMaxTextureSize() const = 0;

		//! Color conversion convenience function
		/** Convert an image (as array of pixels) from source to destination
		array, thereby converting the color format. The pixel size is
		determined by the color formats.
		\param sP Pointer to source
		\param sF Color format of source
		\param sN Number of pixels to convert, both array must be large enough
		\param dP Pointer to destination
		\param dF Color format of destination
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const = 0;
	};

}



#endif
