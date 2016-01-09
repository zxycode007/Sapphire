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

		//��ʼ��Ⱦ����
		virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
			ColourValue color = ColourValue::getColourValue(255, 0, 0, 0),
			//ColourValue color = ColourValue(255, 0, 0, 0),
			const SExposedVideoData& videoData = SExposedVideoData(),
			rect<SINT32>* sourceRect = 0);
		//������Ⱦ����
		virtual bool endScene();

		//! �ر�������һ������
		virtual void disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag = true);

		//! ��ѯ�������������������Ч������true
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

		//! ��ȡʵ����Ƶ����������
		const IAttributes& getDriverAttributes() const;

		//! ���ñ任
		virtual void setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat);

		//! ����ͼ�������������
		virtual UINT32 getImageLoaderCount() const;

		//! ���ظ�����ͼ�������
		virtual IImageLoader* getImageLoader(UINT32 n);

		//! ����ͼ��д����������
		virtual UINT32 getImageWriterCount() const;

		//! ���ظ���ͼ��д����
		virtual IImageWriter* getImageWriter(UINT32 n);

		//! ����һ������
		virtual void setMaterial(const SMaterial& material);

		//! ����һ������
		virtual ITexture* getTexture(const path& filename);

		//! ����һ������
		virtual ITexture* getTexture(IReadFile* file);

		//! ��һ����������һ������
		virtual ITexture* getTextureByIndex(UINT32 index);

		//! ���ص�ǰ�������������
		virtual UINT32 getTextureCount() const;

		//! ������һ������
		virtual void renameTexture(ITexture* texture, const path& newName);

		//! ����һ������
		virtual ITexture* addTexture(const dimension2d<UINT32>& size, const path& name, ECOLOR_FORMAT format = ECF_A8R8G8B8);

		//! ����һ����ȾĿ��
		virtual bool setRenderTarget(ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, ColourValue color);

		//! ���û�������ָ������ȾĿ��
		virtual bool setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
			bool clearZBuffer, ColourValue color);

		//! ���ö����ȾĿ��
		virtual bool setRenderTarget(const vector<IRenderTarget>::type& texture,
			bool clearBackBuffer = true, bool clearZBuffer = true, ColourValue color = ColourValue(0, 0, 0, 0));

		//! ����һ���ӿ�
		virtual void setViewPort(const rect<SINT32>& area);

		//! ��ȡ��ǰ�ӿ�����
		virtual const rect<SINT32>& getViewPort() const;

		//! ����һ������ͼԪ�б�
		virtual void drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType = EVT_STANDARD, E_PRIMITIVE_TYPE pType = EPT_TRIANGLES, E_INDEX_TYPE iType = EIT_16BIT);

		//! ����2d ͼԪ�б�
		virtual void draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType = EVT_STANDARD, E_PRIMITIVE_TYPE pType = EPT_TRIANGLES, E_INDEX_TYPE iType = EIT_16BIT);

		//! ����һ��3d�߶�
		virtual void draw3DLine(const Vector3& start,
			const Vector3& end, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����һ��3d ������
		virtual void draw3DTriangle(const triangle3df& triangle,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����һ��3d AABB���� 
		virtual void draw3DBox(const AxisAlignedBox& box,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����2dͼ��
		virtual void draw2DImage(const ITexture* texture, const Vector2& destPos);

		//! ����һ��2dͼ��ļ��ϣ� ʹ��һ����ɫ��һ��alpha
		/** �����Ҫһ������ͨ�������ͼ���Ǳ�������posλ�ã�������������һ��ֱ����.
		���л��ƶ�Ҫͨ��clipRect���м��á���������sourceRects���鶨�岢ͨ����������
		ѡ��
		\param texture: Ҫ���Ƶ�����
		\param pos: Ҫ����ͼ������Ͻ�2dĿ��λ��
		\param sourceRects: ͼԪԴ��������
		\param indices: ����ÿ��ѡ��ʵ�ʾ��ε������б�
		\param kerningWidth: ��λ�õ�ƫ����
		\param clipRect: ָ����Ļ���������ָ�룬ͼ��ᱻ���õ�����ȥ�����pointer=0����ôͼ�񲻻ᱻ����
		\param color: �����ͼ���еĵ������ɫ��ע�⣺������alpha����ʹ�õģ����alphaֵ����255����ô���ͼ��ᱻ͸������
		\param useAlphaChannelOfTexture: ���Ϊtrue����������alphaͨ���ᱻʹ�����������ͼ��*/
		virtual void draw2DImageBatch(const ITexture* texture,
			const Vector2& pos,
			const vector<rect<SINT32> >::type& sourceRects,
			const vector<SINT32>::type& indices,
			SINT32 kerningWidth = 0,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! ����һ��2dͼ��ļ��ϣ�ʹ��һ����ɫ��һ�������alphaͨ��
		/** �����Ҫһ������ͨ�������ͼ���Ǳ�������posλ�ã�������������һ��ֱ����.
		���л��ƶ�Ҫͨ��clipRect���м��á���������sourceRects���鶨�岢ͨ����������
		ѡ��
		\param texture: Ҫ���Ƶ�����
		\param pos: Ҫ����ͼ������Ͻ�2dĿ��λ��
		\param sourceRects ͼԪԴ��������
		\param clipRect ָ����Ļ���������ָ�룬ͼ��ᱻ���õ�����ȥ�����pointer=0����ôͼ�񲻻ᱻ����
		\param color �����ͼ���еĵ������ɫ��ע�⣺������alpha����ʹ�õģ����alphaֵ����255����ô���ͼ��ᱻ͸������
		\param useAlphaChannelOfTexture: ���Ϊtrue����������alphaͨ���ᱻʹ�����������ͼ�� */
		virtual void draw2DImageBatch(const ITexture* texture,
			const vector<Vector2>::type& positions,
			const vector<rect<SINT32> >::type& sourceRects,
			const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255),
			bool useAlphaChannelOfTexture = false);

		//! ��һ����ɫ�������alphatͨ���������Ҫ������һ��2dͼ��
		virtual void draw2DImage(const ITexture* texture, const Vector2& destPos,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255), bool useAlphaChannelOfTexture = false);

		//! ��һ�������л��������һ����
		virtual void draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0,
			const ColourValue* const colors = 0, bool useAlphaChannelOfTexture = false);

		//! ����һ��2d����
		virtual void draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip = 0);

		//! ����һ������ľ���
		virtual void draw2DRectangle(const rect<SINT32>& pos,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip = 0);

		//! ����һ�����ߵ�2d����
		virtual void draw2DRectangleOutline(const recti& pos, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����һ��2���߶�
		virtual void draw2DLine(const Vector2& start,
			const Vector2& end,
			ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

		//! ����һ������
		virtual void drawPixel(UINT32 x, UINT32 y, const ColourValue & color);

		//! ����һ�������Ĺ�Բ������2d�����
		virtual void draw2DPolygon(Vector2 center,
			Real radius, ColourValue Color, SINT32 vertexCount);

		virtual void setFog(ColourValue color = ColourValue::getColourValue(0, 255, 255, 255),
			E_FOG_TYPE fogType = EFT_FOG_LINEAR,
			Real start = 50.0f, Real end = 100.0f, Real density = 0.01f,
			bool pixelFog = false, bool rangeFog = false);

		virtual void getFog(ColourValue& color, E_FOG_TYPE& fogType,
			Real& start, Real& end, Real& density,
			bool& pixelFog, bool& rangeFog);

		//! ��ȡ��ǰ��ɫ�������е���ɫ��ʽ
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! ��ȡ��Ļ��С
		virtual const dimension2d<UINT32>& getScreenSize() const;

		//! ��ȡ��ȾĿ���С
		virtual const dimension2d<UINT32>& getCurrentRenderTargetSize() const;

		// ��ȡ��ǰFPS��
		virtual SINT32 getFPS() const;

		//! ��������һ֡�л��Ƶ�ͼԪ����������������Σ�
		virtual UINT32 getPrimitiveCountDrawn(UINT32 param = 0) const;

		//! ɾ�����ж�̬��Դ
		virtual void deleteAllDynamicLights();

		//! ���һ����̬��Դ�����������Դ������
		//! \param light: Ҫ������Դ�Ĺ�Դ����
		//! \return �����Դ��������-1�Ƿ�������
		virtual SINT32 addDynamicLight(const SLight& light);

		//! �򿪻�ر�һ����̬��Դ
		//! \param lightIndex: addDynamicLight�����ص�����
		//! \param turnOn: trueΪ�򿪣�falseΪ�ر�
		virtual void turnLightOn(SINT32 lightIndex, bool turnOn);

		//! ��������豸�ܹ�֧�ֵĶ�̬��Դ���������
		virtual UINT32 getMaximalDynamicLightAmount() const;

		//! \return ������Ƶ������: ����DIRECT3D8���������ء�Direct3D8.1��
		virtual const wchar_t* getName() const;

		//! ���ö�̬��������ɫ�����Ĭ��ֵ�ǣ�0��0��0��0����ɫ
		//! \param color: �����õĻ�������ɫ
		virtual void setAmbientLight(const ColourValue& color);

		//! ���һ�������ͼ�������������
		virtual void addExternalImageLoader(IImageLoader* loader);

		//! ���һ�������ͼ��д����������
		virtual void addExternalImageWriter(IImageWriter* writer);

		//! ����һ����Ӱ�ݻ���ģ�建�����С�Ҫ����һ��ģ����Ӱ������,�����м����塣�������������ȥ������Ӱ�ݻ���
		//Ȼ����IVideoDriver::drawStencilShadow()ȥ���ӻ���Ӱ
		virtual void drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail = true, UINT32 debugDataVisible = 0);

		//! ����IVideoDriver::drawStencilShadowVolume()��Ӱ�ݻ����Ƶ�ģ�建����������ɫ���ģ����Ӱ��
		//! �������������Ӱ����ɫ
		virtual void drawStencilShadow(bool clearStencilBuffer = false,
			ColourValue leftUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightUpEdge = ColourValue(0, 0, 0, 0),
			ColourValue leftDownEdge = ColourValue(0, 0, 0, 0),
			ColourValue rightDownEdge = ColourValue(0, 0, 0, 0));

		//! ���ص�ǰ���õĶ�̬��Դ����
		//! \return ��ǰ���õĶ�̬��Դ����
		virtual UINT32 getDynamicLightCount() const;

		//! ����֮ǰͨ��IVideDriver::addDynamicLight()���õĹ�Դ����
		//! \param idx: ������0��ʼ��С��IVideoDriver()::getDynamicLightCount
		//! \return ��Դ����SLight
		virtual const SLight& getDynamicLight(UINT32 idx) const;

		//! �����������Ƴ�������ɾ�������ͷŴ����ڴ�
		virtual void removeTexture(ITexture* texture);

		//!  �����������Ƴ�����������ɾ�����ǣ��ͷŴ����ڴ�
		virtual void removeAllTextures();

		//! ����һ����ȾĿ������
		virtual ITexture* addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);

		//! ����һ��1λ�Ļ���һ��ɫ��������alphaͨ��
		virtual void makeColorKeyTexture(ITexture* texture, ColourValue color, bool zeroTexels) const;

		//! ����һ��1λ�Ļ���һ��ɫ��λ�õ�����alphaͨ��
		virtual void makeColorKeyTexture(ITexture* texture, Vector2 colorKeyPixelPos, bool zeroTexels) const;

		//! �߳�����ͼ����һ��������ͼ
		//! \param amplitude: �߶���Ϣ����������ֵ
		virtual void makeNormalMapTexture(ITexture* texture, FLOAT32 amplitude = 1.0f) const;

		//! �����豸���ܹ���drawIndexedTriangleList��Ⱦ��ͼԪ��������� 
		virtual UINT32 getMaximalPrimitiveCount() const;

		//! �򿪻�ر�һ����������׼
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled);

		//! ����һ����������־�Ƿ�򿪻�ر�
		virtual bool getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const;

		//! ��һ���ļ�����һ����ͼ��
		virtual IImage* createImageFromFile(const path& filename);

		//! ��һ���ļ�����һ����ͼ��
		virtual IImage* createImageFromFile(IReadFile* file);

		//! ��һ���ֽ����鴴��һ����ͼ��
		/** \param useForeignMemory: ���Ϊtrue�����ͼ��ֱ��ʹ��ԭʼͼ������ָ�룬��������ͨ��delete[]������ͼ�����ݡ�
		���Ϊfalse��ͼ�񽫱�����һ�ݡ�
		 */
		virtual IImage* createImageFromData(ECOLOR_FORMAT format,
			const dimension2d<UINT32>& size, void *data,
			bool ownForeignMemory = true, bool deleteForeignMemory = true);

		//! ����һ���յ����ͼ��
		virtual IImage* createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);


		//! ������ͼ�񴴽�һ�����ͼ��
		virtual IImage* createImage(ECOLOR_FORMAT format, IImage *imageToCopy);

		//!������ͼ���һ���ִ���һ�����ͼ�� Creates a software image from part of another image.
		virtual IImage* createImage(IImage* imageToCopy,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! ������ͼ�񴴽�һ�����ͼ��
		virtual IImage* createImage(ITexture* texture,
			const Vector2& pos,
			const dimension2d<UINT32>& size);

		//! ����һ�����񻺳���
		virtual void drawMeshBuffer(const IMeshBuffer* mb);

		//! ����һ�����񻺳����ķ���
		virtual void drawMeshBufferNormals(const IMeshBuffer* mb, Real length = 10.f, ColourValue color = ColourValue::getColourValue(255, 255, 255, 255));

	protected:
		//Ӳ������������
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
			//���񻺳���ָ��
			const IMeshBuffer *MeshBuffer;
			UINT32 ChangedID_Vertex;
			UINT32 ChangedID_Index;
			UINT32 LastUsed;
			E_HARDWARE_MAPPING Mapped_Vertex;
			E_HARDWARE_MAPPING Mapped_Index;
		};

		//! ��һ�����񻺳�����ȡӲ������������(���ڴ�������»�����)
		virtual SHWBufferLink *getBufferLink(const IMeshBuffer* mb);

		//! �����Ҫ����Ӳ��������(ֻ��ĳЩ������Ч)
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer) { return false; }

		//! ����Ӳ��������(ֻ��ĳЩ������Ч)
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer) {}

		//! ɾ��Ӳ��������
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! �����񴴽�Ӳ��������(ֻ��ĳЩ������Ч)
		virtual SHWBufferLink *createHardwareBuffer(const IMeshBuffer* mb) { return 0; }

	public:
		//! ��������Ӳ�����������Ƴ����õ�
		virtual void updateAllHardwareBuffers();

		//! �Ƴ�Ӳ��������
		virtual void removeHardwareBuffer(const IMeshBuffer* mb);

		//! �Ƴ�����Ӳ��������
		virtual void removeAllHardwareBuffers();

		//! ��������Ƿ��Ƽ�ʹ��VBO
		virtual bool isHardwareBufferRecommend(const IMeshBuffer* mb);

		//! ��������ڱβ�ѯ
		/**  ����ȷ�Ϻ��ڱβ��ԵĽڵ�*/
		virtual void addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh = 0);

		//! �Ƴ��ڱβ�ѯ
		virtual void removeOcclusionQuery(ISceneNode* node);

		//! �Ƴ��ڱβ�ѯ
		virtual void removeAllOcclusionQueries();

		//! �����ڱβ�ѯ�����ƴ���ڲ�ѯ�е�����
		/** ������񲻱���ȾΪ�ɼ��ģ���overrideMaterial���ر���ɫ����Ȼ��� */
		virtual void runOcclusionQuery(ISceneNode* node, bool visible = false);

		//! �������е��ڱβ�ѯ���������д���ڲ�ѯ�е�����
		/** ������񲻱���ȾΪ�ɼ��ģ���overrideMaterial���ر���ɫ����Ȼ��� */
		virtual void runAllOcclusionQueries(bool visible = false);

		//! �����ڱβ�ѯ����GPU���ؽ��
		/** �����ѯ�������������ñ�־Ϊfalse �����������ﲻ�ᷢ�� */
		virtual void updateOcclusionQuery(ISceneNode* node, bool block = true);

		//! �������е��ڱβ�ѯ����GPU���ؽ��
		/**�����ѯ�������������ñ�־Ϊfalse �����������ﲻ�ᷢ�� */
		virtual void updateAllOcclusionQueries(bool block = true);

		//! ���ز�ѯ���
		/** ����ֵ�ǿɼ�������/Ƭ�ε����������ֵ��һ����ȫ�Ľ���ֵ���ܹ�����ʵ������ֵ
		*/
		virtual UINT32 getOcclusionQueryResult(ISceneNode* node) const;

		//! ֻ�������ڲ�ʹ��
		/**�����������������ڴ�С�Ѿ��ı� */
		virtual void OnResize(const dimension2d<UINT32>& size);

		//! ���һ���²�����Ⱦ������Ƶ�豸
		virtual SINT32 addMaterialRenderer(IMaterialRenderer* renderer,
			const char* name = 0);

		//! ���������Ͳ���ϵͳ����IVideoDriver��ָ������
		virtual const SExposedVideoData& getExposedVideoData();

		//! ������Ƶ��������
		virtual E_DRIVER_TYPE getDriverType() const;

		//! ����ͨ��setTransform���õı任����
		virtual const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! ����ָ��IGPUProgrammingServices�ӿڵ�ָ��
		virtual IGPUProgrammingServices* getGPUProgrammingServices();

		//! ���һ���µĲ�����Ⱦ������Ƶ��������vertex shader��pixel shader����Ⱦ������
		virtual SINT32 addShaderMaterial(const c8* vertexShaderProgram = 0,
			const c8* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! ��IGPUProgrammingServices::addShaderMaterial(), ���ǳ��Դ��ļ��м��س���
		virtual SINT32 addShaderMaterialFromFiles(IReadFile* vertexShaderProgram = 0,
			IReadFile* pixelShaderProgram = 0,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! ��IGPUProgrammingServices::addShaderMaterial(), ���ǳ��Դ��ļ��м��س���
		virtual SINT32 addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
			const path& pixelShaderProgramFileName,
			IShaderConstantSetCallBack* callback = 0,
			E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
			SINT32 userData = 0);

		//! ����ָ�������Ⱦ����NULL��ָ��
		virtual IMaterialRenderer* getMaterialRenderer(UINT32 idx);

		//! ���ص�ǰ���õĲ�����Ⱦ��������
		virtual UINT32 getMaterialRendererCount() const;

		//! ���ز�����Ⱦ��������
		virtual const char* getMaterialRendererName(UINT32 idx) const;

		//! ���һ���µĲ�����Ⱦ������Ƶ����������HLSL����ǰֻ��D3D9֧��
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

		//! ����IGPUProgrammingServices::addShaderMaterial()  
		//! �����Դ��ļ�����س���
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

		//! ����IGPUProgrammingServices::addShaderMaterial()  
		//! �����Դ��ļ�����س���
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

		//! ����һ��ָ��mesh manipulator.��ָ��
		virtual IMeshManipulator* getMeshManipulator();

		//! ���ZBuffer.
		virtual void clearZBuffer();

		//! ����һ����һ֡������ͼ��
		virtual IImage* createScreenShot(ECOLOR_FORMAT format = ECF_UNKNOWN, E_RENDER_TARGET target = ERT_FRAME_BUFFER);

		//! д���ṩ��ͼ�񵽴����ļ�
		virtual bool writeImageToFile(IImage* image, const path& filename, UINT32 param = 0);

		//! д���ṩ��ͼ�񵽴����ļ�
		virtual bool writeImageToFile(IImage* image, IWriteFile * file, UINT32 param = 0);

		//! ����һ��������Ⱦ��������
		virtual void setMaterialRendererName(SINT32 idx, const char* name);

		//! ��һ�����ʴ������������б�
		virtual IAttributes* createAttributesFromMaterial(const SMaterial& material,
			SAttributeReadWriteOptions* options = 0);

		//! ��attributes���һ���ṹ
		virtual void fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attributes);

		//! ����ͼ���Ƿ��Ѿ�������
		virtual ITexture* findTexture(const path& filename);

		//! ����/ȡ������ һ������ƽ��
		//! ������6������ƽ���û���������
		//! \param index: ƽ���������������0��MaxUserClipPlanes.
		//! \param plane: ƽ������
		//! \param enable: ���Ϊtrue���򿪼���ƽ�棬����ر���
		virtual bool setClipPlane(UINT32 index, const Plane& plane, bool enable = false);

		//! ��/�ر�һ������ƽ��
		//! ������6������ƽ���û���������
		//! \param plane: ƽ������
		//! \param enable: ���Ϊtrue���򿪼���ƽ�棬����ر���
		virtual void enableClipPlane(UINT32 index, bool enable);

		//! �����Կ���Ӧ������
		virtual String getVendorInfo() { return "Not available on this driver."; }

		//! ����һ��Ӳ������������������С������
		/** \param count ������Ŀ����Сֵ */
		virtual void setMinHardwareBufferVertexCount(UINT32 count);

		//! ��ȡȫ�ֲ��ʣ������Ը��Ǳ��ز���
		/** ����enable��־�� ������ʵ�ֵ���ڸ���ĳЩ����Ⱦ�����񻺳�����һЩ���ز���*/
		virtual SOverrideMaterial& getOverrideMaterial();

		//! Ϊ���޸�����ֵ��ȡ2d���ǲ���
		virtual SMaterial& getMaterial2D();

		//! ��2d���ǲ���
		virtual void enableMaterial2D(bool enable = true);

		//! ֻ���������ڲ�
		virtual void setAllowZWriteOnTransparent(bool flag)
		{
			AllowZWriteOnTransparent = flag;
		}

		//! ������֧�ֵ���������С
		virtual dimension2du getMaxTextureSize() const;

		//! ��ɫת������
		/** ��һ��ͼ��һ���������飩ת����Ŀ�ĸ�ʽ��ͼ���������飩.
		���ش�С����ɫ��ʽ����
		\param sP Դͼ��ָ��
		\param sF Դͼ����ɫ��ʽ
		\param sN ת�������ظ�ʽ�������������һ����С
		\param dP Ŀ��ͼ������
		\param dF Ŀ��ͼ���ʽ
		*/
		virtual void convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
			void* dP, ECOLOR_FORMAT dF) const;

		 
		virtual ITexture* createRenderTargetTexture(const dimension2d<UINT32>& size,
			const c8* name = 0);

		virtual bool checkDriverReset() { return false; }
	protected:

		//! ɾ����������
		void deleteAllTextures();

		//! ���ļ����Ҽ�����������
		ITexture* loadTextureFromFile(IReadFile* file, const path& hashName = "");

		//! ���һ�����棬����������ػ򴴽�
		void addTexture(ITexture* surface);

		//! ��һ���Ѽ���ͼ�񴴽�����
		virtual ITexture* addTexture(const path& name, IImage* image, void* mipmapData = 0);

		//! ��һ��������棨IImage������һ���豸����������
		//! ������������豸����ʹ���Լ�������������
		virtual ITexture* createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData = 0);

		//! �������������������������������
		bool checkPrimitiveCount(UINT32 prmcnt) const;

		// ���һ��������Ⱦ������֮�������������ڲ�����
		SINT32 addAndDropMaterialRenderer(IMaterialRenderer* m);

		//! ɾ�����в�����Ⱦ��
		void deleteMaterialRenders();

		// �����Ⱦ���汾
		void printVersion();

		//! ����ͼ��ѯ32λ�汾
		inline Real nml32(int x, int y, int pitch, int height, SINT32 *p) const
		{
			if (x < 0) x = pitch - 1; if (x >= pitch) x = 0;
			if (y < 0) y = height - 1; if (y >= height) y = 0;
			return (Real)(((p[(y * pitch) + x]) >> 16) & 0xff);
		}

		//!  ����ͼ��ѯ16λ�汾
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
		//�ڱβ�ѯ����
		vector<SOccQuery>::type OcclusionQueries;
		//�������������
		vector<IImageLoader*>::type SurfaceLoader;
		//����д��������
		vector<IImageWriter*>::type SurfaceWriter;
		//��Դ����
		vector<SLight>::type Lights;
		vector<SMaterialRenderer>::type MaterialRenderers;

		//vector<SHWBufferLink*> HWBufferLinks;
		//!Ӳ��������HashMap����
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