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
		virtual bool setRenderTarget(const vector<IRenderTarget>& texture,
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
		/** 如果需要一个纹理通道。这个图像是channel of the texture if desired. The images are drawn
		beginning at pos and concatenated in one line. All drawings
		are clipped against clipRect (if != 0).
		The subtextures are defined by the vector of sourceRects
		and are chosen by the indices given.
		\param texture: Texture to be drawn.
		\param pos: Upper left 2d destination position where the image will be drawn.
		\param sourceRects: Source rectangles of the image.
		\param indices: List of indices which choose the actual rectangle used each time.
		\param kerningWidth: offset on position
		\param clipRect: Pointer to rectangle on the screen where the image is clipped to.
		This pointer can be 0. Then the image is not clipped.
		\param color: Color with which the image is colored.
		Note that the alpha component is used: If alpha is other than 255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of the texture is
		used to draw the image. */
		virtual void draw2DImageBatch(const ITexture* texture,
			const Vector2& pos,
			const vector<rect<SINT32> >& sourceRects,
			const vector<SINT32>& indices,
			SINT32 kerningWidth = 0,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! Draws a set of 2d images, using a color and the alpha channel of the texture.
		/** All drawings are clipped against clipRect (if != 0).
		The subtextures are defined by the vector of sourceRects and are
		positioned using the vector of positions.
		\param texture Texture to be drawn.
		\param pos Array of upper left 2d destinations where the images
		will be drawn.
		\param sourceRects Source rectangles of the image.
		\param clipRect Pointer to rectangle on the screen where the
		images are clipped to.
		If this pointer is 0 then the image is not clipped.
		\param color Color with which the image is drawn.
		Note that the alpha component is used. If alpha is other than
		255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of
		the texture is used to draw the image. */
		virtual void draw2DImageBatch(const ITexture* texture,
			const vector<Vector2>& positions,
			const vector<rect<SINT32> >& sourceRects,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! Draws a 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
		virtual void draw2DImage(const ITexture* texture, const Vector2& destPos,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);

		//! Draws a part of the texture into the rectangle.
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue* const colors = 0, bool useAlphaChannelOfTexture = false);

		//! Draws a 2d rectangle
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip = 0);

		//! Draws a 2d rectangle with a gradient.
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0);

		//! Draws the outline of a 2d rectangle
		virtual void draw2DRectangleOutline(const recti& pos, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! Draws a 2d line.
		virtual void draw2DLine(const Vector2& start,
			const Vector2& end,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! Draws a pixel
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue & color);

		//! Draws a non filled concyclic reqular 2d polyon.
		virtual void draw2DPolygon(Vector2 center,
			Real radius, ColourValue Color, SINT32 vertexCount);

		virtual void setFog(ColourValue color = ColourValue::getColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false);

		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog);

		//! get color format of the current color buffer
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! get screen size
		virtual const dimension2d<UINT32>& getScreenSize() const;

		//! get render target size
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const;

		// get current frames per second value
		virtual SINT32 getFPS() const;

		//! returns amount of primitives (mostly triangles) were drawn in the last frame.
		//! very useful method for statistics.
		virtual UINT32 getPrimitiveCountDrawn(UINT32 param = 0) const;

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights();

		//! adds a dynamic light, returning an index to the light
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual SINT32 addDynamicLight(const SLight& light);

		//! Turns a dynamic light on or off
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn);

		//! returns the maximal amount of dynamic lights the device can handle
		virtual UINT32 getMaximalDynamicLightAmount() const;

		//! \return Returns the name of the video driver. Example: In case of the DIRECT3D8
		//! driver, it would return "Direct3D8.1".
		virtual const wchar_t* getName() const;

		//! Sets the dynamic ambient light color. The default color is
		//! (0,0,0,0) which means it is dark.
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const ColourValue& color);

		//! Adds an external image loader to the engine.
		virtual void addExternalImageLoader(IImageLoader* loader);

		//! Adds an external image writer to the engine.
		virtual void addExternalImageWriter(IImageWriter* writer);

		//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
		//! this: Frist, draw all geometry. Then use this method, to draw the shadow
		//! volume. Then, use IVideoDriver::drawStencilShadow() to visualize the shadow.
		virtual void drawStencilShadowVolume(const vector<Vector3>& triangles, bool zfail = true, UINT32 debugDataVisible = 0);

		//! Fills the stencil shadow with color. After the shadow volume has been drawn
		//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
		//! to draw the color of the shadow.
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(0, 0, 0, 0));

		//! Returns current amount of dynamic lights set
		//! \return Current amount of dynamic lights set
		virtual UINT32 getDynamicLightCount() const;

		//! Returns light data which was previously set with IVideDriver::addDynamicLight().
		//! \param idx: Zero based index of the light. Must be greater than 0 and smaller
		//! than IVideoDriver()::getDynamicLightCount.
		//! \return Light data.
		virtual const SLight& getDynamicLight(UINT32 idx) const;

		//! Removes a texture from the texture cache and deletes it, freeing lot of
		//! memory.
		virtual void removeTexture(ITexture* texture);

		//! Removes all texture from the texture cache and deletes them, freeing lot of
		//! memory.
		virtual void removeAllTextures();

		//! Creates a render target texture.
		virtual ITexture* addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);

		//! Creates an 1bit alpha channel of the texture based of an color key.
		virtual void makeColorKeyTexture(ITexture* texture, ColourValue color, bool zeroTexels) const;

		//! Creates an 1bit alpha channel of the texture based of an color key position.
		virtual void makeColorKeyTexture(ITexture* texture, Vector2 colorKeyPixelPos, bool zeroTexels) const;

		//! Creates a normal map from a height map texture.
		//! \param amplitude: Constant value by which the height information is multiplied.
		virtual void makeNormalMapTexture(ITexture* texture, Real amplitude = 1.0f) const;

		//! Returns the maximum amount of primitives (mostly vertices) which
		//! the device is able to render with one drawIndexedTriangleList
		//! call.
		virtual UINT32 getMaximalPrimitiveCount() const;

		//! Enables or disables a texture creation flag.
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled);

		//! Returns if a texture creation flag is enabled or disabled.
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const;

		//! Creates a software image from a file.
		virtual IImage* createImageFromFile(const path& filename);

		//! Creates a software image from a file.
		virtual IImage* createImageFromFile(IReadFile* file);

		//! Creates a software image from a byte vector.
		/** \param useForeignMemory: If true, the image will use the data pointer
		directly and own it from now on, which means it will also try to delete [] the
		data when the image will be destructed. If false, the memory will by copied. */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = true, bool deleteForeignMemory = true);

		//! Creates an empty software image.
		virtual IImage* createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);


		//! Creates a software image from another image.
		virtual IImage* createImage(ECOLOR_FORMAT format, IImage *imageToCopy);

		//! Creates a software image from part of another image.
		virtual IImage* createImage(IImage* imageToCopy,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! Creates a software image from part of a texture.
		virtual IImage* createImage(ITexture* texture,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! Draws a mesh buffer
		virtual void drawMeshBuffer(const IMeshBuffer* mb);

		//! Draws the normals of a mesh buffer
		virtual void drawMeshBufferNormals(const IMeshBuffer* mb, Real length = 10.f, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

	protected:
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

			const IMeshBuffer *MeshBuffer;
			UINT32 ChangedID_Vertex;
			UINT32 ChangedID_Index;
			UINT32 LastUsed;
			E_HARDWARE_MAPPING Mapped_Vertex;
			E_HARDWARE_MAPPING Mapped_Index;
		};

		//! Gets hardware buffer link from a meshbuffer (may create or update buffer)
		virtual SHWBufferLink *getBufferLink(const IMeshBuffer* mb);

		//! updates hardware buffer if needed  (only some drivers can)
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer) { return false; }

		//! Draw hardware buffer (only some drivers can)
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer) {}

		//! Delete hardware buffer
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! Create hardware buffer from mesh (only some drivers can)
		virtual SHWBufferLink *createHardwareBuffer(const IMeshBuffer* mb) { return 0; }

	public:
		//! Update all hardware buffers, remove unused ones
		virtual void updateAllHardwareBuffers();

		//! Remove hardware buffer
		virtual void removeHardwareBuffer(const IMeshBuffer* mb);

		//! Remove all hardware buffers
		virtual void removeAllHardwareBuffers();

		//! is vbo recommended on this mesh?
		virtual bool isHardwareBufferRecommend(const IMeshBuffer* mb);

		//! Create occlusion query.
		/** Use node for identification and mesh for occlusion test. */
		virtual void addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh = 0);

		//! Remove occlusion query.
		virtual void removeOcclusionQuery(ISceneNode* node);

		//! Remove all occlusion queries.
		virtual void removeAllOcclusionQueries();

		//! Run occlusion query. Draws mesh stored in query.
		/** If the mesh shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false);

		//! Run all occlusion queries. Draws all meshes stored in queries.
		/** If the meshes shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runAllOcclusionQueries(bool visible = false);

		//! Update occlusion query. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true);

		//! Update all occlusion queries. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateAllOcclusionQueries(bool block = true);

		//! Return query result.
		/** Return value is the number of visible pixels/fragments.
		The value is a safe approximation, i.e. can be larger than the
		actual value of pixels. */
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const;

		//! Only used by the engine internally.
		/** Used to notify the driver that the window was resized. */
		virtual void OnResize(const dimension2d<UINT32>& size);

		//! Adds a new material renderer to the video device.
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer,
			const char* name = 0);

		//! Returns driver and operating system specific data about the IVideoDriver.
		virtual const SExposedVideoData& getExposedVideoData();

		//! Returns type of video driver
		virtual E_DRIVER_TYPE getDriverType() const;

		//! Returns the transformation set by setTransform
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! Returns pointer to the IGPUProgrammingServices interface.
		virtual IGPUProgrammingServices* getGPUProgrammingServices();

		//! Adds a new material renderer to the VideoDriver, using pixel and/or
		//! vertex shaders to render geometry.
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram = 0,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
		//! programs from files.
		virtual SINT32 addShaderMaterialFromFiles(IReadFile* vertexShaderProgram = 0,
			IReadFile* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
		//! programs from files.
		virtual SINT32 addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! Returns pointer to material renderer or null
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx);

		//! Returns amount of currently available material renderers.
		virtual UINT32 getMaterialRendererCount() const;

		//! Returns name of the material renderer
		virtual const char* getMaterialRendererName(UINT32 idx) const;

		//! Adds a new material renderer to the VideoDriver, based on a high level shading
		//! language. Currently only HLSL in D3D9 is supported.
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

		//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
		//! but tries to load the programs from files.
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

		//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
		//! but tries to load the programs from files.
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

		//! Returns a pointer to the mesh manipulator.
		virtual IMeshManipulator* getMeshManipulator();

		//! Clears the ZBuffer.
		virtual void clearZBuffer();

		//! Returns an image created from the last rendered frame.
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);

		//! Writes the provided image to disk file
		virtual bool writeImageToFile(IImage* image, const path& filename, UINT32 param = 0);

		//! Writes the provided image to a file.
		virtual bool writeImageToFile(IImage* image, IWriteFile * file, UINT32 param = 0);

		//! Sets the name of a material renderer.
		virtual void setMaterialRendererName(SINT32 idx, const char* name);

		//! Creates material attributes list from a material, usable for serialization and more.
		virtual IAttributes* createAttributesFromMaterial(const SMaterial& material,
			SAttributeReadWriteOptions* options = 0);

		//! Fills an SMaterial structure from attributes.
		virtual void fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attributes);

		//! looks if the image is already loaded
		virtual ITexture* findTexture(const path& filename);

		//! Set/unset a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param plane: The plane itself.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false);

		//! Enable/disable a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual void enableClipPlane(UINT32 index, bool enable);

		//! Returns the graphics card vendor name.
		virtual String getVendorInfo() { return "Not available on this driver."; }

		//! Set the minimum number of vertices for which a hw buffer will be created
		/** \param count Number of vertices to set as minimum. */
		virtual void setMinHardwareBufferVertexCount(UINT32 count);

		//! Get the global Material, which might override local materials.
		/** Depending on the enable flags, values from this Material
		are used to override those of local materials of some
		meshbuffer being rendered. */
		virtual SOverrideMaterial& getOverrideMaterial();

		//! Get the 2d override material for altering its values
		virtual SMaterial& getMaterial2D();

		//! Enable the 2d override material
		virtual void enableMaterial2D(bool enable = true);

		//! Only used by the engine internally.
		virtual void setAllowZWriteOnTransparent(bool flag)
		{
			AllowZWriteOnTransparent = flag;
		}

		//! Returns the maximum texture size supported.
		virtual dimension2du getMaxTextureSize() const;

		//! Color conversion convenience function
		/** Convert an image (as vector of pixels) from source to destination
		vector, thereby converting the color format. The pixel size is
		determined by the color formats.
		\param sP Pointer to source
		\param sF Color format of source
		\param sN Number of pixels to convert, both vector must be large enough
		\param dP Pointer to destination
		\param dF Color format of destination
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const;

		//! deprecated method
		virtual ITexture* createRenderTargetTexture(const dimension2d<UINT32>& size,
			const c8* name = 0);

		virtual bool checkDriverReset() { return false; }
	protected:

		//! deletes all textures
		void deleteAllTextures();

		//! opens the file and loads it into the surface
		ITexture* loadTextureFromFile(IReadFile* file, const path& hashName = "");

		//! adds a surface, not loaded or created by the Irrlicht Engine
		void addTexture(ITexture* surface);

		//! Creates a texture from a loaded IImage.
		virtual ITexture* addTexture(const path& name, IImage* image, void* mipmapData = 0);

		//! returns a device dependent texture from a software surface (IImage)
		//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
		virtual ITexture* createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData = 0);

		//! checks triangle count and print warning if wrong
		bool checkPrimitiveCount(UINT32 prmcnt) const;

		// adds a material renderer and drops it afterwards. To be used for internal creation
		SINT32 addAndDropMaterialRenderer(IMaterialRenderer* m);

		//! deletes all material renderers
		void deleteMaterialRenders();

		// prints renderer version
		void printVersion();

		//! normal map lookup 32 bit version
		inline Real nml32(int x, int y, int pitch, int height, SINT32 *p) const
		{
			if (x < 0) x = pitch - 1; if (x >= pitch) x = 0;
			if (y < 0) y = height - 1; if (y >= height) y = 0;
			return (Real)(((p[(y * pitch) + x]) >> 16) & 0xff);
		}

		//! normal map lookup 16 bit version
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
		};

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
		vector<SSurface> Textures;

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
		vector<SOccQuery> OcclusionQueries;

		vector<IImageLoader*> SurfaceLoader;
		vector<IImageWriter*> SurfaceWriter;
		vector<SLight> Lights;
		vector<SMaterialRenderer> MaterialRenderers;

		//vector<SHWBufferLink*> HWBufferLinks;
		map< const IMeshBuffer*, SHWBufferLink* > HWBufferMap;

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