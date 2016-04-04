#ifndef _SAPPHIRE_C_OPENGL_DRIVER_
#define _SAPPHIRE_C_OPENGL_DRIVER_

#include "SapphireCompileConifg.h"
#include "SapphireSCreationParameters.h"

namespace Sapphire
{
	class CSapphireDeviceWin32;
	class CSapphireDeviceLinux;
	class CSapphireDeviceSDL;
	class CSapphireDeviceMacOSX;
}

#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_

#include "SapphireCNullDriver.h"
#include "SapphireIMaterialRendererServices.h"
	// also includes the OpenGL stuff
#include "SapphireCOpenGLExtensionHandler.h"

#ifdef _Sapphire_COMPILE_WITH_CG_
#include "Cg/cg.h"
#endif

namespace Sapphire
{
	class COpenGLTexture;

	class COpenGLDriver : public CNullDriver, public IMaterialRendererServices, public COpenGLExtensionHandler
	{
		friend class COpenGLTexture;
	public:

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceWin32* device);
		//! 初始化windows的opengl驱动的特定部分
		bool initDriver(CSapphireDeviceWin32* device);
		bool changeRenderContext(const SExposedVideoData& videoData, CSapphireDeviceWin32* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_X11
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceLinux* device);
		//! 初始化GLX的opengl驱动的特定部分
		bool initDriver(CSapphireDeviceLinux* device);
		bool changeRenderContext(const SExposedVideoData& videoData, CSapphireDeviceLinux* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_SDL
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceSDL* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceMacOSX *device);
#endif

		//! 重载未实现的通用版本
		bool changeRenderContext(const SExposedVideoData& videoData, void* device) { return false; }

		//!析构函数
		virtual ~COpenGLDriver();

		
		//! 清空ZBUFFER
		//开始渲染场景
		//初始化所有渲染参数
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue::getColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0);

		//! 展示渲染后的场景到屏幕上，如果失败，返回false
		virtual bool endScene();

		//! 设置变换
		virtual void setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat);


		struct SHWBufferLink_opengl : public SHWBufferLink
		{
			SHWBufferLink_opengl(const IMeshBuffer *_MeshBuffer) : SHWBufferLink(_MeshBuffer), vbo_verticesID(0), vbo_indicesID(0){}

			GLuint vbo_verticesID; //tmp
			GLuint vbo_indicesID; //tmp

			GLuint vbo_verticesSize; //tmp
			GLuint vbo_indicesSize; //tmp
		};

		//! 如果需要更新硬件缓冲区
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer);

		//! 为一个网格创建硬件缓冲区
		virtual SHWBufferLink *createHardwareBuffer(const IMeshBuffer* mb);

		//! 删除硬件缓冲区（只有某些驱动可以）
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! 绘制硬件缓冲区
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer);

		//! 创建遮蔽查询 
		/** 对节点进行标识和对网格进行遮蔽测试*/
		virtual void addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh = 0);

		//! 移除遮蔽查询
		virtual void removeOcclusionQuery(ISceneNode* node);

		//! 运行遮蔽查询，绘制在在查询里的网格 
		/** 如果网格不可见，用overrideMaterial 关闭颜色和深度缓冲区*/
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false);

		//! 更新遮蔽查询。 从GPU返回结果
		/** 如果查询被阻挡，设置标志为false，这种情况下更新可能不会发生
		*/
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true);

		//! 返回查询结果
		/** 返回遮蔽测试后可见的像素/片段的数量， 这个值是一个安全的近似值。可能大于实际的
		像素值
		 */
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const;

		//! 绘制顶点图元列表
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		//! 绘制一个顶点图元列表（2D）
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		//! 插值驱动的特征，如果这个特征有效返回true
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
		{
			return FeatureEnabled[feature] && COpenGLExtensionHandler::queryFeature(feature);
		}

		//! 设置一个材质。所有3d绘制函数绘制几何体都用这个材质
		//! \param material: 当前要使用的材质
		virtual void setMaterial(const SMaterial& material);

		//! 绘制一个2d图像的集合，可以用一个带alpha通道的颜色的纹理
		void draw2DImageBatch(const ITexture* texture,
			const vector<Position2d >::type& positions,
			const vector<rect<SINT32> >::type& sourceRects,
			const rect<SINT32>* clipRect,
			ColourValue color,
			bool useAlphaChannelOfTexture);

		//! 使用一个颜色和alpha通道的纹理绘制一个2d图像
		virtual void draw2DImage(const ITexture* texture, const Position2d& destPos,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);

		//! 用一个颜色和alpha通道的纹理绘制一个2d图像集合。
		/** 
		这个图像全部绘制从同一个位置开始和连接到一条直线上，所有的绘制都被clipRect剪裁（如果不为0）
		子纹理由sourceRects数组和给定的indices定义
		\param texture: Texture to be drawn.
		\param pos: Upper left 2d destination position where the image will be drawn.
		\param sourceRects: Source rectangles of the image.
		\param indices: List of indices which choose the actual rectangle used each time.
		\param clipRect: Pointer to rectangle on the screen where the image is clipped to.
		This pointer can be 0. Then the image is not clipped.
		\param color: Color with which the image is colored.
		Note that the alpha component is used: If alpha is other than 255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of the texture is
		used to draw the image. */
		virtual void draw2DImage(const ITexture* texture,
			const Position2d& pos,
			const vector<rect<SINT32> >::type& sourceRects,
			const vector<SINT32>::type& indices,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! 绘制纹理的一部分到矩形中区域中
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue* const colors = 0, bool useAlphaChannelOfTexture = false);

		//! 绘制一个2d矩形
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos,
			const rect<SINT32>* clip = 0);

		//!用一个梯度绘制一个2d矩形
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0);

		//! 绘制一个2d 线段
		virtual void draw2DLine(const Position2d& start,
			const Position2d& end,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! 绘制一个像素
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue & color);

		//! 绘制一个3d线段
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end,
			ColourValue color = ColourValue(255, 255, 255, 255));

		//! \return 返回视频驱动名。 
		virtual const wchar_t* getName() const;

		//! 删除所有动态光源
		virtual void deleteAllDynamicLights();

		//! 添加一个动态光源，返回这个光源的一个索引
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual SINT32 addDynamicLight(const SLight& light);

		//! 打开或关闭一个动态光源
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn);

		//! 获取设备可以支持的最大动态光源的数量
		virtual UINT32 getMaximalDynamicLightAmount() const;

		//! 设置动态环境光颜色。默认颜色为(0,0,0,0) 意味它颜色是黑色
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const ColourValue& color);

		//! 绘制阴影容积到模板缓冲区中。要绘制一个模板阴影，
		//首先,绘制所有几何体。然后使用这个方法，来绘制阴影容积，然后再使用IVideoDriver::drawStencilShadow()来可视化阴影
		virtual void drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible = 0);

		//! 用颜色来填充模板阴影。在用IVideoDriver::drawStencilShadowVolume()阴影容积被绘制到模板缓冲区中后，用
		// 用这个来绘制阴影的颜色
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(0, 0, 0, 0));

		//! 设置一个视口
		virtual void setViewPort(const rect<SINT32>& area);

		//! 设置雾的模式
		virtual void setFog(ColourValue color, E_FOG_TYPE fogType, FLOAT32 start,
			FLOAT32 end, FLOAT32 density, bool pixelFog, bool rangeFog);

		//! 只用于引擎内部调用。用于提醒驱动，窗口发生了改变
		virtual void OnResize(const dimension2d<UINT32>& size);

		//! 返回视频驱动的类型
		virtual E_DRIVER_TYPE getDriverType() const;

		//! 获取当前颜色缓冲区的颜色格式
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! 返回setTransform设置的变换
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! 通过调用一个IMaterialRenderer来使工作容易些
		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
			bool resetAllRenderstates);

		//! 设置一个顶点shader 常量
		virtual void setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);

		//! 设置一个像素shader 常量
		virtual void setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);

		//! 通过一个名字设置一个vertex shader常量
		virtual bool setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count);

		//! Bool interface for the above.
		virtual bool setVertexShaderConstant(const c8* name, const bool* bools, int count);

		//! Int interface for the above.
		virtual bool setVertexShaderConstant(const c8* name, const SINT32* ints, int count);

		//! Sets a constant for the pixel shader based on a name.
		virtual bool setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count);

		//! Bool interface for the above.
		virtual bool setPixelShaderConstant(const c8* name, const bool* bools, int count);

		//! Int interface for the above.
		virtual bool setPixelShaderConstant(const c8* name, const SINT32* ints, int count);

		//! 设置当前纹理
		//! Returns whether setting was a success or not.
		bool setActiveTexture(UINT32 stage, const ITexture* texture);

		//! 关闭从fromStage参数开始的所有的纹理。 
		//! Returns whether disabling was successful or not.
		bool disableTextures(UINT32 fromStage = 0);

		//! 添加一个新的材质渲染器到这个视频驱动，用extGLGetObjectParameteriv(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status)
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, SINT32 userData);

		//! 添加一个新的材质渲染器到这个视频驱动，用GLSL来渲染几何体
		virtual SINT32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget,
			const c8* pixelShaderProgram,
			const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget,
			const c8* geometryShaderProgram,
			const c8* geometryShaderEntryPointName = "main",
			E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
			E_PRIMITIVE_TYPE inType = EPT_TRIANGLES,
			E_PRIMITIVE_TYPE outType = EPT_TRIANGLE_STRIP,
			UINT32 verticesOut = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0,
			E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT);

		//! 返回一个指向视频驱动的指针(对IMaterialRendererServices的实现) 
		virtual IVideoDriver* getVideoDriver();

		//!返回这个设备支持能用drawIndexedTriangleList绘制的图元的最大数量
		virtual UINT32 getMaximalPrimitiveCount() const;

		virtual ITexture* addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);

		//! 设置或重置渲染目标
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
			bool clearZBuffer, ColourValue color);

		//! set or reset render target texture
		virtual bool setRenderTarget(ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, ColourValue color);

		//! 设置多个渲染目标
		virtual bool setRenderTarget(const vector<IRenderTarget>::type& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true, ColourValue color = ColourValue(0, 0, 0, 0));

		//! Clears the ZBuffer.
		virtual void clearZBuffer();

		//! 返回最后渲染帧的图像
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);

		//! 检测OpenGL的错误,并且打印输出它,只在debug模式下有效
		//checks if an OpenGL error has happend and prints it
		//! for performance reasons only available in debug mode
		bool testGLError();

		//! 设置/取消设置一个剪裁平面
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param plane: The plane itself.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false);

		//! 打开/关闭一个剪裁平面
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual void enableClipPlane(UINT32 index, bool enable);

		//! 打开2d覆盖材质
		virtual void enableMaterial2D(bool enable = true);

		//! 返回显卡供应商名
		virtual String getVendorInfo() { return VendorName; }

		//! 返回支持的最大纹理大小
		virtual dimension2du getMaxTextureSize() const;

		ITexture* createDepthTexture(ITexture* texture, bool shared = true);
		void removeDepthTexture(ITexture* texture);

		//! 从一个纹理缓存中移除一个纹理并且删除它，释放大量内存
		void removeTexture(ITexture* texture);

		//! 转换E_PRIMITIVE_TYPE到与OpenGL等价的类型
		GLenum primitiveTypeToGL(E_PRIMITIVE_TYPE type) const;

		//! 转换E_BLEND_FACTOR到OpenGL等价的类型
		GLenum getGLBlend(E_BLEND_FACTOR factor) const;

		//! 获取ZBuffer位深
		GLenum getZBufferBits() const;

		//! Get Cg context
#ifdef _SAPPHIRE_COMPILE_WITH_CG_
		const CGcontext& getCgContext();
#endif

	private:

		//! 同时清除zbuffer和color buffer
		void clearBuffers(bool backBuffer, bool zBuffer, bool stencilBuffer, ColourValue color);
		//更新硬件顶点缓冲区
		bool updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		//更新硬件顶点索引缓冲区
		bool updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		//更新剪裁平面
		void uploadClipPlane(UINT32 index);

		//! 在所有平台上初始化OpenGL 驱动部分参数
		bool genericDriverInit();
		//! 从一个软件表面IImage返回一个设备依赖的纹理
		virtual ITexture* createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData);

		//! 创建一个支持GLfloat数组的转置矩阵传递给OpenGL
		inline void getGLMatrix(GLfloat gl_matrix[16], const Matrix4& m);
		inline void getGLTextureMatrix(GLfloat gl_matrix[16], const Matrix4& m);

		//! 设置OpenGL 管线的材质的纹理包装模式
		void setWrapMode(const SMaterial& material);

		//! 设置本地的包装模式值
		GLint getTextureWrapMode(const UINT8 clamp);

		//! 设置需要的渲染状态
		void setRenderStates3DMode();

		//! 设置需要的渲染状态
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);

		// 返回当前屏幕或渲染目标的大小
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const;

		void createMaterialRenderers();

		//! 分配一个硬件光源到指定请求的光源，如果有空闲的硬件光源存在的话
		//! \param[in] lightIndex: the index of the requesting light
		void assignHardwareLight(UINT32 lightIndex);

		//! 对于渲染设置的帮助函数
		void getColorBuffer(const void* vertices, UINT32 vertexCount, E_VERTEX_TYPE vType);

		//! helper function doing the actual rendering.
		// 实际渲染的辅助函数
		void renderArray(const void* indexList, UINT32 primitiveCount,
			E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		StringW Name;
		Matrix4 Matrices[ETS_COUNT];
		//颜色缓冲区
		vector<UINT8>::type ColorBuffer;

		//! 渲染模式的枚举类型
		//enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,		// 2d drawing rendermode
			ERM_3D		// 3d rendering mode
		};

		E_RENDER_MODE CurrentRenderMode;
		//! 如果设置为true，所有渲染状态将重置
		bool ResetRenderStates;
		bool Transformation3DChanged;
		UINT8 AntiAlias;

		SMaterial Material, LastMaterial;
		COpenGLTexture* RenderTargetTexture;
		vector<IRenderTarget>::type MRTargets;
		//阶段纹理缓存
		class STextureStageCache
		{
			const ITexture* CurrentTexture[MATERIAL_MAX_TEXTURES];
		public:
			STextureStageCache()
			{
				for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				{
					CurrentTexture[i] = 0;
				}
			}

			~STextureStageCache()
			{
				clear();
			}

			void set(UINT32 stage, const ITexture* tex)
			{
				if (stage<MATERIAL_MAX_TEXTURES)
				{
					const ITexture* oldTexture = CurrentTexture[stage];
					if (tex)
						tex->grab();
					CurrentTexture[stage] = tex;
					if (oldTexture)
						oldTexture->drop();
				}
			}

			const ITexture* operator[](int stage) const
			{
				if ((UINT32)stage<MATERIAL_MAX_TEXTURES)
					return CurrentTexture[stage];
				else
					return 0;
			}

			void remove(const ITexture* tex)
			{
				for (SINT32 i = MATERIAL_MAX_TEXTURES - 1; i >= 0; --i)
				{
					if (CurrentTexture[i] == tex)
					{
						tex->drop();
						CurrentTexture[i] = 0;
					}
				}
			}

			void clear()
			{
				// Drop all the CurrentTexture handles
				for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				{
					if (CurrentTexture[i])
					{
						CurrentTexture[i]->drop();
						CurrentTexture[i] = 0;
					}
				}
			}
		};
		STextureStageCache CurrentTexture;
		vector<ITexture*>::type DepthTextures;
		struct SUserClipPlane
		{
			SUserClipPlane() : Enabled(false) {}
			Plane Plane;
			bool Enabled;
		};
		vector<SUserClipPlane>::type UserClipPlanes;

		dimension2d<UINT32> CurrentRendertargetSize;

		String VendorName;

		Matrix4 TextureFlipMatrix;

		//! 颜色缓冲区格式
		ECOLOR_FORMAT ColorFormat;

		//! 渲染操作的渲染目标类型
		E_RENDER_TARGET CurrentTarget;

		SSapphireCreationParameters Params;

		//! 所有光源都被已经被请求；直到在同一时刻使用的硬件限制数
		struct RequestedLight
		{
			RequestedLight(SLight const & lightData)
				: LightData(lightData), HardwareLightIndex(-1), DesireToBeOn(true) { }

			SLight	LightData;
			SINT32	HardwareLightIndex; // GL_LIGHT0 - GL_LIGHT7
			bool	DesireToBeOn;
		};
		vector<RequestedLight>::type RequestedLights;

#if   SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
		HDC HDc; // Private GDI Device Context
		HWND Window;
#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
		CSapphireDeviceWin32 *Win32Device;
#endif
#endif
#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_X11
		GLXDrawable Drawable;
		Display* X11Display;
		CSapphireDeviceLinux *X11Device;
#endif
#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
		CSapphireDeviceMacOSX *OSXDevice;
#endif
#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_SDL
		CSapphireDeviceSDL *SDLDevice;
#endif
#ifdef _SAPPHIRE_COMPILE_WITH_CG_
		CGcontext CgContext;
#endif

		E_DEVICE_TYPE DeviceType;
	};
}


#endif
#endif