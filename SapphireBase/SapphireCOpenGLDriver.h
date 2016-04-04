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
		//! ��ʼ��windows��opengl�������ض�����
		bool initDriver(CSapphireDeviceWin32* device);
		bool changeRenderContext(const SExposedVideoData& videoData, CSapphireDeviceWin32* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_X11
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceLinux* device);
		//! ��ʼ��GLX��opengl�������ض�����
		bool initDriver(CSapphireDeviceLinux* device);
		bool changeRenderContext(const SExposedVideoData& videoData, CSapphireDeviceLinux* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_SDL
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceSDL* device);
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
		COpenGLDriver(const SSapphireCreationParameters& params, IFileSystem* io, CSapphireDeviceMacOSX *device);
#endif

		//! ����δʵ�ֵ�ͨ�ð汾
		bool changeRenderContext(const SExposedVideoData& videoData, void* device) { return false; }

		//!��������
		virtual ~COpenGLDriver();

		
		//! ���ZBUFFER
		//��ʼ��Ⱦ����
		//��ʼ��������Ⱦ����
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue::getColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0);

		//! չʾ��Ⱦ��ĳ�������Ļ�ϣ����ʧ�ܣ�����false
		virtual bool endScene();

		//! ���ñ任
		virtual void setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat);


		struct SHWBufferLink_opengl : public SHWBufferLink
		{
			SHWBufferLink_opengl(const IMeshBuffer *_MeshBuffer) : SHWBufferLink(_MeshBuffer), vbo_verticesID(0), vbo_indicesID(0){}

			GLuint vbo_verticesID; //tmp
			GLuint vbo_indicesID; //tmp

			GLuint vbo_verticesSize; //tmp
			GLuint vbo_indicesSize; //tmp
		};

		//! �����Ҫ����Ӳ��������
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer);

		//! Ϊһ�����񴴽�Ӳ��������
		virtual SHWBufferLink *createHardwareBuffer(const IMeshBuffer* mb);

		//! ɾ��Ӳ����������ֻ��ĳЩ�������ԣ�
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! ����Ӳ��������
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer);

		//! �����ڱβ�ѯ 
		/** �Խڵ���б�ʶ�Ͷ���������ڱβ���*/
		virtual void addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh = 0);

		//! �Ƴ��ڱβ�ѯ
		virtual void removeOcclusionQuery(ISceneNode* node);

		//! �����ڱβ�ѯ���������ڲ�ѯ������� 
		/** ������񲻿ɼ�����overrideMaterial �ر���ɫ����Ȼ�����*/
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false);

		//! �����ڱβ�ѯ�� ��GPU���ؽ��
		/** �����ѯ���赲�����ñ�־Ϊfalse����������¸��¿��ܲ��ᷢ��
		*/
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true);

		//! ���ز�ѯ���
		/** �����ڱβ��Ժ�ɼ�������/Ƭ�ε������� ���ֵ��һ����ȫ�Ľ���ֵ�����ܴ���ʵ�ʵ�
		����ֵ
		 */
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const;

		//! ���ƶ���ͼԪ�б�
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		//! ����һ������ͼԪ�б�2D��
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		//! ��ֵ������������������������Ч����true
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
		{
			return FeatureEnabled[feature] && COpenGLExtensionHandler::queryFeature(feature);
		}

		//! ����һ�����ʡ�����3d���ƺ������Ƽ����嶼���������
		//! \param material: ��ǰҪʹ�õĲ���
		virtual void setMaterial(const SMaterial& material);

		//! ����һ��2dͼ��ļ��ϣ�������һ����alphaͨ������ɫ������
		void draw2DImageBatch(const ITexture* texture,
			const vector<Position2d >::type& positions,
			const vector<rect<SINT32> >::type& sourceRects,
			const rect<SINT32>* clipRect,
			ColourValue color,
			bool useAlphaChannelOfTexture);

		//! ʹ��һ����ɫ��alphaͨ�����������һ��2dͼ��
		virtual void draw2DImage(const ITexture* texture, const Position2d& destPos,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);

		//! ��һ����ɫ��alphaͨ�����������һ��2dͼ�񼯺ϡ�
		/** 
		���ͼ��ȫ�����ƴ�ͬһ��λ�ÿ�ʼ�����ӵ�һ��ֱ���ϣ����еĻ��ƶ���clipRect���ã������Ϊ0��
		��������sourceRects����͸�����indices����
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

		//! ���������һ���ֵ�������������
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue* const colors = 0, bool useAlphaChannelOfTexture = false);

		//! ����һ��2d����
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos,
			const rect<SINT32>* clip = 0);

		//!��һ���ݶȻ���һ��2d����
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0);

		//! ����һ��2d �߶�
		virtual void draw2DLine(const Position2d& start,
			const Position2d& end,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����һ������
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue & color);

		//! ����һ��3d�߶�
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end,
			ColourValue color = ColourValue(255, 255, 255, 255));

		//! \return ������Ƶ�������� 
		virtual const wchar_t* getName() const;

		//! ɾ�����ж�̬��Դ
		virtual void deleteAllDynamicLights();

		//! ���һ����̬��Դ�����������Դ��һ������
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual SINT32 addDynamicLight(const SLight& light);

		//! �򿪻�ر�һ����̬��Դ
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn);

		//! ��ȡ�豸����֧�ֵ����̬��Դ������
		virtual UINT32 getMaximalDynamicLightAmount() const;

		//! ���ö�̬��������ɫ��Ĭ����ɫΪ(0,0,0,0) ��ζ����ɫ�Ǻ�ɫ
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const ColourValue& color);

		//! ������Ӱ�ݻ���ģ�建�����С�Ҫ����һ��ģ����Ӱ��
		//����,�������м����塣Ȼ��ʹ�������������������Ӱ�ݻ���Ȼ����ʹ��IVideoDriver::drawStencilShadow()�����ӻ���Ӱ
		virtual void drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible = 0);

		//! ����ɫ�����ģ����Ӱ������IVideoDriver::drawStencilShadowVolume()��Ӱ�ݻ������Ƶ�ģ�建�����к���
		// �������������Ӱ����ɫ
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(0, 0, 0, 0));

		//! ����һ���ӿ�
		virtual void setViewPort(const rect<SINT32>& area);

		//! �������ģʽ
		virtual void setFog(ColourValue color, E_FOG_TYPE fogType, FLOAT32 start,
			FLOAT32 end, FLOAT32 density, bool pixelFog, bool rangeFog);

		//! ֻ���������ڲ����á������������������ڷ����˸ı�
		virtual void OnResize(const dimension2d<UINT32>& size);

		//! ������Ƶ����������
		virtual E_DRIVER_TYPE getDriverType() const;

		//! ��ȡ��ǰ��ɫ����������ɫ��ʽ
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! ����setTransform���õı任
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! ͨ������һ��IMaterialRenderer��ʹ��������Щ
		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
			bool resetAllRenderstates);

		//! ����һ������shader ����
		virtual void setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);

		//! ����һ������shader ����
		virtual void setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1);

		//! ͨ��һ����������һ��vertex shader����
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

		//! ���õ�ǰ����
		//! Returns whether setting was a success or not.
		bool setActiveTexture(UINT32 stage, const ITexture* texture);

		//! �رմ�fromStage������ʼ�����е����� 
		//! Returns whether disabling was successful or not.
		bool disableTextures(UINT32 fromStage = 0);

		//! ���һ���µĲ�����Ⱦ���������Ƶ��������extGLGetObjectParameteriv(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status)
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, SINT32 userData);

		//! ���һ���µĲ�����Ⱦ���������Ƶ��������GLSL����Ⱦ������
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

		//! ����һ��ָ����Ƶ������ָ��(��IMaterialRendererServices��ʵ��) 
		virtual IVideoDriver* getVideoDriver();

		//!��������豸֧������drawIndexedTriangleList���Ƶ�ͼԪ���������
		virtual UINT32 getMaximalPrimitiveCount() const;

		virtual ITexture* addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);

		//! ���û�������ȾĿ��
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
			bool clearZBuffer, ColourValue color);

		//! set or reset render target texture
		virtual bool setRenderTarget(ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, ColourValue color);

		//! ���ö����ȾĿ��
		virtual bool setRenderTarget(const vector<IRenderTarget>::type& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true, ColourValue color = ColourValue(0, 0, 0, 0));

		//! Clears the ZBuffer.
		virtual void clearZBuffer();

		//! ���������Ⱦ֡��ͼ��
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);

		//! ���OpenGL�Ĵ���,���Ҵ�ӡ�����,ֻ��debugģʽ����Ч
		//checks if an OpenGL error has happend and prints it
		//! for performance reasons only available in debug mode
		bool testGLError();

		//! ����/ȡ������һ������ƽ��
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param plane: The plane itself.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false);

		//! ��/�ر�һ������ƽ��
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual void enableClipPlane(UINT32 index, bool enable);

		//! ��2d���ǲ���
		virtual void enableMaterial2D(bool enable = true);

		//! �����Կ���Ӧ����
		virtual String getVendorInfo() { return VendorName; }

		//! ����֧�ֵ���������С
		virtual dimension2du getMaxTextureSize() const;

		ITexture* createDepthTexture(ITexture* texture, bool shared = true);
		void removeDepthTexture(ITexture* texture);

		//! ��һ�����������Ƴ�һ��������ɾ�������ͷŴ����ڴ�
		void removeTexture(ITexture* texture);

		//! ת��E_PRIMITIVE_TYPE����OpenGL�ȼ۵�����
		GLenum primitiveTypeToGL(E_PRIMITIVE_TYPE type) const;

		//! ת��E_BLEND_FACTOR��OpenGL�ȼ۵�����
		GLenum getGLBlend(E_BLEND_FACTOR factor) const;

		//! ��ȡZBufferλ��
		GLenum getZBufferBits() const;

		//! Get Cg context
#ifdef _SAPPHIRE_COMPILE_WITH_CG_
		const CGcontext& getCgContext();
#endif

	private:

		//! ͬʱ���zbuffer��color buffer
		void clearBuffers(bool backBuffer, bool zBuffer, bool stencilBuffer, ColourValue color);
		//����Ӳ�����㻺����
		bool updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		//����Ӳ����������������
		bool updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		//���¼���ƽ��
		void uploadClipPlane(UINT32 index);

		//! ������ƽ̨�ϳ�ʼ��OpenGL �������ֲ���
		bool genericDriverInit();
		//! ��һ���������IImage����һ���豸����������
		virtual ITexture* createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData);

		//! ����һ��֧��GLfloat�����ת�þ��󴫵ݸ�OpenGL
		inline void getGLMatrix(GLfloat gl_matrix[16], const Matrix4& m);
		inline void getGLTextureMatrix(GLfloat gl_matrix[16], const Matrix4& m);

		//! ����OpenGL ���ߵĲ��ʵ������װģʽ
		void setWrapMode(const SMaterial& material);

		//! ���ñ��صİ�װģʽֵ
		GLint getTextureWrapMode(const UINT8 clamp);

		//! ������Ҫ����Ⱦ״̬
		void setRenderStates3DMode();

		//! ������Ҫ����Ⱦ״̬
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);

		// ���ص�ǰ��Ļ����ȾĿ��Ĵ�С
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const;

		void createMaterialRenderers();

		//! ����һ��Ӳ����Դ��ָ������Ĺ�Դ������п��е�Ӳ����Դ���ڵĻ�
		//! \param[in] lightIndex: the index of the requesting light
		void assignHardwareLight(UINT32 lightIndex);

		//! ������Ⱦ���õİ�������
		void getColorBuffer(const void* vertices, UINT32 vertexCount, E_VERTEX_TYPE vType);

		//! helper function doing the actual rendering.
		// ʵ����Ⱦ�ĸ�������
		void renderArray(const void* indexList, UINT32 primitiveCount,
			E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		StringW Name;
		Matrix4 Matrices[ETS_COUNT];
		//��ɫ������
		vector<UINT8>::type ColorBuffer;

		//! ��Ⱦģʽ��ö������
		//enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,		// 2d drawing rendermode
			ERM_3D		// 3d rendering mode
		};

		E_RENDER_MODE CurrentRenderMode;
		//! �������Ϊtrue��������Ⱦ״̬������
		bool ResetRenderStates;
		bool Transformation3DChanged;
		UINT8 AntiAlias;

		SMaterial Material, LastMaterial;
		COpenGLTexture* RenderTargetTexture;
		vector<IRenderTarget>::type MRTargets;
		//�׶�������
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

		//! ��ɫ��������ʽ
		ECOLOR_FORMAT ColorFormat;

		//! ��Ⱦ��������ȾĿ������
		E_RENDER_TARGET CurrentTarget;

		SSapphireCreationParameters Params;

		//! ���й�Դ�����Ѿ�������ֱ����ͬһʱ��ʹ�õ�Ӳ��������
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