#ifndef _SAPPHIRE_C_NULL_DRIVER_
#define _SAPPHIRE_C_NULL_DRIVER_

#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIFileSystem.h"
#include "SapphireCFPSCounter.h"

#ifdef _MSC_VER
#pragma warning( disable: 4996)
#endif

namespace Sapphire
{
	class IWriteFile;
	class IReadFile;


	class IImageLoader;
	class IImageWriter;

	class CNullDriver : public IVideoDriver, public IGPUProgrammingServices
	{
	public:

		 
		CNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize);

		 
		virtual ~CNullDriver();

		//开始渲染场景
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue::getColourValue(255, 0, 0, 0),
			//ColourValue color = ColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0);
		//结束渲染场景
		virtual bool endScene();

		//! 关闭驱动的一个特征
		virtual void disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag = true);

		//! 查询驱动特征，如果特征有效，返回true
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

		//! 获取实际视频驱动的属性
		const IAttributes& getDriverAttributes() const;

		//! 设置变换
		virtual void setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat);

		//! 返回图像加载器的数量
		virtual UINT32 getImageLoaderCount() const;

		//! 返回给定的图像加载器
		virtual IImageLoader* getImageLoader(UINT32 n);

		//! 返回图像写入器的数量
		virtual UINT32 getImageWriterCount() const;

		//! 返回给定图像写入器
		virtual IImageWriter* getImageWriter(UINT32 n);

		//! 设置一个材质
		virtual void setMaterial(const SMaterial& material);

		//! 加载一个材质
		virtual ITexture* getTexture(const path& filename);

		//! 加载一个纹理
		virtual ITexture* getTexture(IReadFile* file);

		//! 从一个索引返回一个纹理
		virtual ITexture* getTextureByIndex(UINT32 index);

		//! 返回当前加载纹理的数量
		virtual UINT32 getTextureCount() const;

		//! 重命名一个纹理
		virtual void renameTexture(ITexture* texture, const path& newName);

		//! 创建一个纹理
		virtual ITexture* addTexture(const dimension2d<UINT32>& size, const path& name, ECOLOR_FORMAT format = ECF_A8R8G8B8);

		//! 设置一个渲染目标
		virtual bool setRenderTarget(ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, ColourValue color);

		//! 设置或者重设指定的渲染目标
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
			bool clearZBuffer, ColourValue color);

		//! 设置多个渲染目标
		virtual bool setRenderTarget(const vector<IRenderTarget>::type& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true, ColourValue color = ColourValue(0, 0, 0, 0));

		//! 设置一个视口
		virtual void setViewPort(const rect<SINT32>& area);

		//! 获取当前视口区域
		virtual const rect<SINT32>& getViewPort() const;

		//! 绘制一个顶点图元列表
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType = EVT_STANDARD, E_PRIMITIVE_TYPE pType = EPT_TRIANGLES, E_INDEX_TYPE iType = EIT_16BIT);

		//! 绘制2d 图元列表
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType = EVT_STANDARD, E_PRIMITIVE_TYPE pType = EPT_TRIANGLES, E_INDEX_TYPE iType = EIT_16BIT);

		//! 绘制一个3d线段
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制一个3d 三角形
		virtual void draw3DTriangle(const triangle3df& triangle,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制一个3d AABB盒子 
		virtual void draw3DBox(const AxisAlignedBox& box,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制2d图像
		virtual void draw2DImage(const ITexture* texture, const Vector2& destPos);

		//! 绘制一个2d图像的集合， 使用一个颜色和一个alpha
		/** 如果需要一个纹理通道。这个图像是被绘制在pos位置，并连续绘制在一条直线上.
		所有绘制都要通过clipRect进行剪裁。子纹理由sourceRects数组定义并通过索引进行
		选择
		\param texture: 要绘制的纹理
		\param pos: 要绘制图像的左上角2d目标位置
		\param sourceRects: 图元源矩形区域
		\param indices: 用于每次选择实际矩形的索引列表
		\param kerningWidth: 在位置的偏移量
		\param clipRect: 指向屏幕矩形区域的指针，图像会被剪裁到里面去，如果pointer=0，那么图像不会被剪裁
		\param color: 在这个图像中的的这个颜色。注意：这是由alpha分量使用的；如果alpha值不少255，那么这个图像会被透明化。
		\param useAlphaChannelOfTexture: 如果为true，这个纹理的alpha通道会被使用来绘制这个图像*/
		virtual void draw2DImageBatch(const ITexture* texture,
			const Vector2& pos,
			const vector<rect<SINT32> >::type& sourceRects,
			const vector<SINT32>::type& indices,
			SINT32 kerningWidth = 0,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! 绘制一个2d图像的集合，使用一个颜色和一个纹理的alpha通道
		/** 如果需要一个纹理通道。这个图像是被绘制在pos位置，并连续绘制在一条直线上.
		所有绘制都要通过clipRect进行剪裁。子纹理由sourceRects数组定义并通过索引进行
		选择
		\param texture: 要绘制的纹理
		\param pos: 要绘制图像的左上角2d目标位置
		\param sourceRects 图元源矩形区域
		\param clipRect 指向屏幕矩形区域的指针，图像会被剪裁到里面去，如果pointer=0，那么图像不会被剪裁
		\param color 在这个图像中的的这个颜色。注意：这是由alpha分量使用的；如果alpha值不少255，那么这个图像会被透明化。
		\param useAlphaChannelOfTexture: 如果为true，这个纹理的alpha通道会被使用来绘制这个图像 */
		virtual void draw2DImageBatch(const ITexture* texture,
			const vector<Vector2>::type& positions,
			const vector<rect<SINT32> >::type& sourceRects,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! 用一个颜色和纹理的alphat通道（如果需要）绘制一个2d图像
		virtual void draw2DImage(const ITexture* texture, const Vector2& destPos,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);

		//! 在一个矩形中绘制纹理的一部分
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue* const colors = 0, bool useAlphaChannelOfTexture = false);

		//! 绘制一个2d矩形
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip = 0);

		//! 绘制一个渐变的矩形
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0);

		//! 绘制一个离线的2d矩形
		virtual void draw2DRectangleOutline(const recti& pos, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制一个2的线段
		virtual void draw2DLine(const Vector2& start,
			const Vector2& end,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制一个像素
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue & color);

		//! 绘制一个非填充的共圆的正规2d多边形
		virtual void draw2DPolygon(Vector2 center,
			Real radius, ColourValue Color, SINT32 vertexCount);

		virtual void setFog(ColourValue color = ColourValue::getColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false);

		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog);

		//! 获取当前颜色缓冲区中的颜色格式
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! 获取屏幕大小
		virtual const dimension2d<UINT32>& getScreenSize() const;

		//! 获取渲染目标大小
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const;

		// 获取当前FPS数
		virtual SINT32 getFPS() const;

		//! 返回在上一帧中绘制的图元数量（大多是三角形）
		virtual UINT32 getPrimitiveCountDrawn(UINT32 param = 0) const;

		//! 删除所有动态光源
		virtual void deleteAllDynamicLights();

		//! 添加一个动态光源，返回这个光源的索引
		//! \param light: 要创建光源的光源数据
		//! \return 这个光源的索引，-1是发生错误
		virtual SINT32 addDynamicLight(const SLight& light);

		//! 打开或关闭一个动态光源
		//! \param lightIndex: addDynamicLight所返回的索引
		//! \param turnOn: true为打开，false为关闭
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn);

		//! 返回这个设备能够支持的动态光源的最大数量
		virtual UINT32 getMaximalDynamicLightAmount() const;

		//! \return 返回视频驱动名: 例如DIRECT3D8驱动，返回“Direct3D8.1”
		virtual const wchar_t* getName() const;

		//! 设置动态环境光颜色。这个默认值是（0，0，0，0）黑色
		//! \param color: 新设置的环境光颜色
		virtual void setAmbientLight(const ColourValue& color);

		//! 添加一个额外的图像加载器给引擎
		virtual void addExternalImageLoader(IImageLoader* loader);

		//! 添加一个额外的图像写入器给引擎
		virtual void addExternalImageWriter(IImageWriter* writer);

		//! 绘制一个阴影容积到模板缓冲区中。要绘制一个模板阴影：首先,会所有几何体。再用这个方法，去绘制阴影容积。
		//然后，用IVideoDriver::drawStencilShadow()去可视化阴影
		virtual void drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail = true, UINT32 debugDataVisible = 0);

		//! 在用IVideoDriver::drawStencilShadowVolume()阴影容积绘制到模板缓冲区后，用颜色填充模板阴影。
		//! 这个方法绘制阴影的颜色
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(0, 0, 0, 0));

		//! 返回当前设置的动态光源数量
		//! \return 当前设置的动态光源数量
		virtual UINT32 getDynamicLightCount() const;

		//! 返回之前通过IVideDriver::addDynamicLight()设置的光源数据
		//! \param idx: 索引从0开始，小于IVideoDriver()::getDynamicLightCount
		//! \return 光源数据SLight
		virtual const SLight& getDynamicLight(UINT32 idx) const;

		//! 从纹理缓冲区移除纹理并且删除它，释放大量内存
		virtual void removeTexture(ITexture* texture);

		//!  从纹理缓冲区移除所有纹理并且删除它们，释放大量内存
		virtual void removeAllTextures();

		//! 创建一个渲染目标纹理
		virtual ITexture* addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);

		//! 创建一个1位的基于一个色键的纹理alpha通道
		virtual void makeColorKeyTexture(ITexture* texture, ColourValue color, bool zeroTexels) const;

		//! 创建一个1位的基于一个色键位置的纹理alpha通道
		virtual void makeColorKeyTexture(ITexture* texture, Vector2 colorKeyPixelPos, bool zeroTexels) const;

		//! 高程纹理图创建一个法线贴图
		//! \param amplitude: 高度信息的增幅常量值
		virtual void makeNormalMapTexture(ITexture* texture, FLOAT32 amplitude = 1.0f) const;

		//! 返回设备所能够用drawIndexedTriangleList渲染的图元的最大数量 
		virtual UINT32 getMaximalPrimitiveCount() const;

		//! 打开或关闭一个纹理创建标准
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled);

		//! 返回一个纹理创建标志是否打开或关闭
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const;

		//! 从一个文件创建一个软图像
		virtual IImage* createImageFromFile(const path& filename);

		//! 从一个文件创建一个软图像
		virtual IImage* createImageFromFile(IReadFile* file);

		//! 从一个字节数组创建一个软图像
		/** \param useForeignMemory: 如果为true，这个图像将直接使用原始图像数据指针，这样可以通过delete[]来销毁图像数据。
		如果为false，图像将被复制一份。
		 */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = true, bool deleteForeignMemory = true);

		//! 创建一个空的软件图像
		virtual IImage* createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);


		//! 从另外图像创建一个软件图像
		virtual IImage* createImage(ECOLOR_FORMAT format, IImage *imageToCopy);

		//!从另外图像的一部分创建一个软件图像 Creates a software image from part of another image.
		virtual IImage* createImage(IImage* imageToCopy,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! 从另外图像创建一个软件图像
		virtual IImage* createImage(ITexture* texture,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! 绘制一个网格缓冲区
		virtual void drawMeshBuffer(const IMeshBuffer* mb);

		//! 绘制一个网格缓冲区的法线
		virtual void drawMeshBufferNormals(const IMeshBuffer* mb, Real length = 10.f, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

	protected:
		//硬件缓冲区链接
		struct SHWBufferLink
		{
			SHWBufferLink(const IMeshBuffer *_MeshBuffer)
				:MeshBuffer(_MeshBuffer),
				ChangedID_Vertex(0), ChangedID_Index(0), LastUsed(0),
				Mapped_Vertex(EHM_NEVER), Mapped_Index(EHM_NEVER)
			{
				if (MeshBuffer)
					MeshBuffer->grab();
			}

			virtual ~SHWBufferLink()
			{
				if (MeshBuffer)
					MeshBuffer->drop();
			}
			//网格缓冲区指针
			const IMeshBuffer *MeshBuffer;
			UINT32 ChangedID_Vertex;
			UINT32 ChangedID_Index;
			UINT32 LastUsed;
			E_HARDWARE_MAPPING Mapped_Vertex;
			E_HARDWARE_MAPPING Mapped_Index;
		};

		//! 从一个网格缓冲区获取硬件缓冲区链接(用于创建或更新缓冲区)
		virtual SHWBufferLink *getBufferLink(const IMeshBuffer* mb);

		//! 如果需要更新硬件缓冲区(只对某些驱动有效)
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer) { return false; }

		//! 绘制硬件缓冲区(只对某些驱动有效)
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer) {}

		//! 删除硬件缓冲区
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! 从网格创建硬件缓冲区(只对某些驱动有效)
		virtual SHWBufferLink *createHardwareBuffer(const IMeshBuffer* mb) { return 0; }

	public:
		//! 更新索引硬件缓冲区，移除不用的
		virtual void updateAllHardwareBuffers();

		//! 移除硬件缓冲区
		virtual void removeHardwareBuffer(const IMeshBuffer* mb);

		//! 移除所有硬件缓冲区
		virtual void removeAllHardwareBuffers();

		//! 这个网格是否推荐使用VBO
		virtual bool isHardwareBufferRecommend(const IMeshBuffer* mb);

		//! 创建添加遮蔽查询
		/**  用来确认和遮蔽测试的节点*/
		virtual void addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh = 0);

		//! 移除遮蔽查询
		virtual void removeOcclusionQuery(ISceneNode* node);

		//! 移除遮蔽查询
		virtual void removeAllOcclusionQueries();

		//! 运行遮蔽查询。绘制存放在查询中的网格
		/** 如果网格不被渲染为可见的，用overrideMaterial来关闭颜色和深度缓冲 */
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false);

		//! 允许所有的遮蔽查询。绘制所有存放在查询中的网格
		/** 如果网格不被渲染为可见的，用overrideMaterial来关闭颜色和深度缓冲 */
		virtual void runAllOcclusionQueries(bool visible = false);

		//! 更新遮蔽查询。从GPU返回结果
		/** 如果查询不被阻塞，设置标志为false 。更新在这里不会发生 */
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true);

		//! 更新所有的遮蔽查询。从GPU返回结果
		/**如果查询不被阻塞，设置标志为false 。更新在这里不会发生 */
		virtual void updateAllOcclusionQueries(bool block = true);

		//! 返回查询结果
		/** 返回值是可见的像素/片段的数量。这个值是一个安全的近似值，能够大于实际像素值
		*/
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const;

		//! 只背引擎内部使用
		/**用于提醒驱动：窗口大小已经改变 */
		virtual void OnResize(const dimension2d<UINT32>& size);

		//! 添加一个新材质渲染器到视频设备
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer,
			const char* name = 0);

		//! 返回驱动和操作系统关于IVideoDriver的指定数据
		virtual const SExposedVideoData& getExposedVideoData();

		//! 返回视频驱动类型
		virtual E_DRIVER_TYPE getDriverType() const;

		//! 返回通过setTransform设置的变换矩阵
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! 返回指向IGPUProgrammingServices接口的指针
		virtual IGPUProgrammingServices* getGPUProgrammingServices();

		//! 添加一个新的材质渲染器到视频驱动，用vertex shader或pixel shader来渲染几何体
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram = 0,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! 像IGPUProgrammingServices::addShaderMaterial(), 但是尝试从文件中加载程序
		virtual SINT32 addShaderMaterialFromFiles(IReadFile* vertexShaderProgram = 0,
			IReadFile* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! 像IGPUProgrammingServices::addShaderMaterial(), 但是尝试从文件中加载程序
		virtual SINT32 addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! 返回指向材质渲染器或NULL的指针
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx);

		//! 返回当前可用的材质渲染器的数量
		virtual UINT32 getMaterialRendererCount() const;

		//! 返回材质渲染器的名字
		virtual const char* getMaterialRendererName(UINT32 idx) const;

		//! 添加一个新的材质渲染器到视频驱动，基于HLSL，当前只在D3D9支持
		virtual SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* vertexShaderEntryPointName = 0,
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			const c8* pixelShaderProgram = 0,
			const c8* pixelShaderEntryPointName = 0,
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			const c8* geometryShaderProgram = 0,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0, E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT);

		//! 类似IGPUProgrammingServices::addShaderMaterial()  
		//! 但尝试从文件里加载程序
		virtual SINT32 addHighLevelShaderMaterialFromFiles(
			const path& vertexShaderProgramFile,
			const c8* vertexShaderEntryPointName = "main",
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			const path& pixelShaderProgramFile = "",
			const c8* pixelShaderEntryPointName = "main",
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			const path& geometryShaderProgramFileName = "",
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0, E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT);

		//! 类似IGPUProgrammingServices::addShaderMaterial()  
		//! 但尝试从文件里加载程序
		virtual SINT32 addHighLevelShaderMaterialFromFiles(
			IReadFile* vertexShaderProgram,
			const c8* vertexShaderEntryPointName = "main",
			E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
			IReadFile* pixelShaderProgram = 0,
			const c8* pixelShaderEntryPointName = "main",
			E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
			IReadFile* geometryShaderProgram = 0,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0, E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT);

		//! 返回一个指向mesh manipulator.的指针
		virtual IMeshManipulator* getMeshManipulator();

		//! 清空ZBuffer.
		virtual void clearZBuffer();

		//! 返回一个上一帧创建的图像
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);

		//! 写入提供的图像到磁盘文件
		virtual bool writeImageToFile(IImage* image, const path& filename, UINT32 param = 0);

		//! 写入提供的图像到磁盘文件
		virtual bool writeImageToFile(IImage* image, IWriteFile * file, UINT32 param = 0);

		//! 设置一个材质渲染器的名字
		virtual void setMaterialRendererName(SINT32 idx, const char* name);

		//! 从一个材质创建材质属性列表
		virtual IAttributes* createAttributesFromMaterial(const SMaterial& material,
			SAttributeReadWriteOptions* options = 0);

		//! 用attributes填充一个结构
		virtual void fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attributes);

		//! 看看图像是否已经被加载
		virtual ITexture* findTexture(const path& filename);

		//! 设置/取消设置 一个剪裁平面
		//! 有最少6个剪裁平面用户可以设置
		//! \param index: 平面的索引。必须在0到MaxUserClipPlanes.
		//! \param plane: 平面自身
		//! \param enable: 如果为true，打开剪裁平面，否则关闭它
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false);

		//! 打开/关闭一个剪裁平面
		//! 有最少6个剪裁平面用户可以设置
		//! \param plane: 平面自身
		//! \param enable: 如果为true，打开剪裁平面，否则关闭它
		virtual void enableClipPlane(UINT32 index, bool enable);

		//! 返回显卡供应商名称
		virtual String getVendorInfo() { return "Not available on this driver."; }

		//! 设置一个硬件缓冲区所创建的最小顶点数
		/** \param count 顶点数目的最小值 */
		virtual void setMinHardwareBufferVertexCount(UINT32 count);

		//! 获取全局材质，它可以覆盖本地材质
		/** 依赖enable标志， 这个材质的值用于覆盖某些在渲染的网格缓冲区的一些本地材质*/
		virtual SOverrideMaterial& getOverrideMaterial();

		//! 为了修改它的值获取2d覆盖材质
		virtual SMaterial& getMaterial2D();

		//! 打开2d覆盖材质
		virtual void enableMaterial2D(bool enable = true);

		//! 只用于引擎内部
		virtual void setAllowZWriteOnTransparent(bool flag)
		{
			AllowZWriteOnTransparent = flag;
		}

		//! 返回所支持的最大纹理大小
		virtual dimension2du getMaxTextureSize() const;

		//! 颜色转换函数
		/** 将一个图像（一个像素数组）转换到目的格式的图像（像素数组）.
		像素大小由颜色格式决定
		\param sP 源图像指针
		\param sF 源图像颜色格式
		\param sN 转换的像素格式，两个数组必须一样大小
		\param dP 目的图像制作
		\param dF 目的图像格式
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const;

		 
		virtual ITexture* createRenderTargetTexture(const dimension2d<UINT32>& size,
			const c8* name = 0);

		virtual bool checkDriverReset() { return false; }
	protected:

		//! 删除所有纹理
		void deleteAllTextures();

		//! 打开文件并且加载它到表面
		ITexture* loadTextureFromFile(IReadFile* file, const path& hashName = "");

		//! 添加一个表面，不被引擎加载或创建
		void addTexture(ITexture* surface);

		//! 从一个已加载图像创建纹理
		virtual ITexture* addTexture(const path& name, IImage* image, void* mipmapData = 0);

		//! 由一个软件表面（IImage）返回一个设备依赖的纹理
		//! 这个方法将由设备驱动使用自己的纹理所覆盖
		virtual ITexture* createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData = 0);

		//! 检测三角形数量并且如果错误输出警告
		bool checkPrimitiveCount(UINT32 prmcnt) const;

		// 添加一个材质渲染器并且之后丢弃它。用于内部操作
		SINT32 addAndDropMaterialRenderer(IMaterialRenderer* m);

		//! 删除所有材质渲染器
		void deleteMaterialRenders();

		// 输出渲染器版本
		void printVersion();

		//! 法线图查询32位版本
		inline Real nml32(int x, int y, int pitch, int height, SINT32 *p) const
		{
			if (x < 0) x = pitch - 1; if (x >= pitch) x = 0;
			if (y < 0) y = height - 1; if (y >= height) y = 0;
			return (Real)(((p[(y * pitch) + x]) >> 16) & 0xff);
		}

		//!  法线图查询16位版本
		inline Real nml16(int x, int y, int pitch, int height, SINT16 *p) const
		{
			if (x < 0) x = pitch - 1; if (x >= pitch) x = 0;
			if (y < 0) y = height - 1; if (y >= height) y = 0;

			return (Real)getAverage(p[(y * pitch) + x]);
		}

		struct SSurface
		{
			ITexture* Surface;

			bool operator < (const SSurface& other) const
			{
				return Surface->getName() < other.Surface->getName();
			}

			bool operator==(const SSurface& other) const
			{
				return other.Surface == Surface;
			}
		};

		bool ITextureCMP(const SSurface& a, const SSurface& b)
		{
			return (a.Surface->getSize().getArea() < b.Surface->getSize().getArea());
		}

		struct SMaterialRenderer
		{
			String Name;
			IMaterialRenderer* Renderer;
		};

		struct SDummyTexture : public ITexture
		{
			SDummyTexture(const path& name) : ITexture(name), size(0, 0) {};

			virtual void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, UINT32 mipmapLevel = 0) { return 0; };
			virtual void unlock(){}
			virtual const dimension2d<UINT32>& getOriginalSize() const { return size; }
			virtual const dimension2d<UINT32>& getSize() const { return size; }
			virtual E_DRIVER_TYPE getDriverType() const { return EDT_NULL; }
			virtual ECOLOR_FORMAT getColorFormat() const { return ECF_A1R5G5B5; };
			virtual UINT32 getPitch() const { return 0; }
			virtual void regenerateMipMapLevels(void* mipmapData = 0) {};
			dimension2d<UINT32> size;
		};
		vector<SSurface>::type Textures;

		struct SOccQuery
		{
			SOccQuery(ISceneNode* node, const IMesh* mesh = 0) : Node(node), Mesh(mesh), PID(0), Result(~0), Run(~0)
			{
				if (Node)
					Node->grab();
				if (Mesh)
					Mesh->grab();
			}

			SOccQuery(const SOccQuery& other) : Node(other.Node), Mesh(other.Mesh), PID(other.PID), Result(other.Result), Run(other.Run)
			{
				if (Node)
					Node->grab();
				if (Mesh)
					Mesh->grab();
			}

			~SOccQuery()
			{
				if (Node)
					Node->drop();
				if (Mesh)
					Mesh->drop();
			}

			SOccQuery& operator=(const SOccQuery& other)
			{
				Node = other.Node;
				Mesh = other.Mesh;
				PID = other.PID;
				Result = other.Result;
				Run = other.Run;
				if (Node)
					Node->grab();
				if (Mesh)
					Mesh->grab();
				return *this;
			}

			bool operator==(const SOccQuery& other) const
			{
				return other.Node == Node;
			}

			ISceneNode* Node;
			const IMesh* Mesh;
			union
			{
				void* PID;
				unsigned int UID;
			};
			UINT32 Result;
			UINT32 Run;
		};
		//遮蔽查询数组
		vector<SOccQuery>::type OcclusionQueries;
		//表面加载器数组
		vector<IImageLoader*>::type SurfaceLoader;
		//表面写入器数组
		vector<IImageWriter*>::type SurfaceWriter;
		//光源数组
		vector<SLight>::type Lights;
		vector<SMaterialRenderer>::type MaterialRenderers;

		//vector<SHWBufferLink*> HWBufferLinks;
		//!硬件缓冲区HashMap容器
		map< const IMeshBuffer*, SHWBufferLink* >::type HWBufferMap;

		IFileSystem* FileSystem;

		//! mesh manipulator
		IMeshManipulator* MeshManipulator;

		rect<SINT32> ViewPort;
		dimension2d<UINT32> ScreenSize;
		Matrix4 TransformationMatrix;

		CFPSCounter FPSCounter;

		UINT32 PrimitivesDrawn;
		UINT32 MinVertexCountForVBO;

		UINT32 TextureCreationFlags;

		Real FogStart;
		Real FogEnd;
		Real FogDensity;
		ColourValue FogColor;
		SExposedVideoData ExposedData;

		IAttributes* DriverAttributes;

		SOverrideMaterial OverrideMaterial;
		SMaterial OverrideMaterial2D;
		SMaterial InitMaterial2D;
		bool OverrideMaterial2DEnabled;

		E_FOG_TYPE FogType;
		bool PixelFog;
		bool RangeFog;
		bool AllowZWriteOnTransparent;

		bool FeatureEnabled[EVDF_COUNT];
	};
}

#endif