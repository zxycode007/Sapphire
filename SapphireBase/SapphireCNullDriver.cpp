
#include "SapphirePrerequisites.h"	
#include "SapphireCompileConifg.h"
#include "SapphireCAtrributes.h"
#include "SapphireOS.h"
#include "SapphireCNullDriver.h"
#include "SapphireCImage.h"
#include "SapphireIReadFile.h"
#include "SapphireIWriteFile.h"
#include "SapphireCColourConverter.h"
#include "SapphireIImageLoader.h"
#include "SapphireIImageWriter.h"
#include "SapphireIMaterialRenderer.h"
#include "SapphireCMeshManipulator.h"
#include "SapphireIMeshSceneNode.h"
#include "SapphireIAnimatedMeshSceneNode.h"



namespace Sapphire
{
	//! ����һ�����Լ���BMP�ļ�����
	IImageLoader* createImageLoaderBMP();

	//! ����һ�����Լ���JPG�ļ����� 
	IImageLoader* createImageLoaderJPG();

	//! ����һ�����Լ���TGA�ļ����� 
	IImageLoader* createImageLoaderTGA();

	//! ����һ�����Լ���PSD�ļ����� 
	IImageLoader* createImageLoaderPSD();

	//! ����һ�����Լ���DDS�ļ�����
	IImageLoader* createImageLoaderDDS();

	//! ����һ�����Լ���PCX�ļ����� 
	IImageLoader* createImageLoaderPCX();

	//! ����һ�����Լ���PNG�ļ�����
	IImageLoader* createImageLoaderPNG();

	//! ����һ�����Լ���WAL�ļ�����
	IImageLoader* createImageLoaderWAL();

	//! ����һ������HalfLife��ͼ��ļ�����
	IImageLoader* createImageLoaderHalfLife();

	//! ����һ�����Լ���LMPͼ��ļ�����
	IImageLoader* createImageLoaderLMP();

	//! ����һ�����Լ���ppm/pgm/pbmͼ��ļ�����
	IImageLoader* createImageLoaderPPM();

	//! ����һ��RGBͼ��ļ�����
	IImageLoader* createImageLoaderRGB();


	//! ����һ�����Ա���BMPͼ���д����
	IImageWriter* createImageWriterBMP();

	//! ����һ�����Ա���JPGͼ���д����
	IImageWriter* createImageWriterJPG();

	//! ����һ�����Ա���TGAͼ���д����
	IImageWriter* createImageWriterTGA();

	//! ����һ�����Ա���PSDͼ���д����
	IImageWriter* createImageWriterPSD();

	//! ����һ�����Ա���PCXͼ���д����
	IImageWriter* createImageWriterPCX();

	//! ����һ�����Ա���PNGͼ���д����
	IImageWriter* createImageWriterPNG();

	//! ����һ�����Ա���PPMͼ���д����
	IImageWriter* createImageWriterPPM();

	//! CNULLDRIVER������
	CNullDriver::CNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize)
		: FileSystem(io), MeshManipulator(0), ViewPort(0, 0, 0, 0), ScreenSize(screenSize),
		PrimitivesDrawn(0), MinVertexCountForVBO(500), TextureCreationFlags(0),
		OverrideMaterial2DEnabled(false), AllowZWriteOnTransparent(false)
	{
#ifdef _DEBUG
		setDebugName("CNullDriver");
#endif

		DriverAttributes = new CAttributes();
		DriverAttributes->addInt("MaxTextures", SAPPHIRE_MAX_TEXTURE_LAYERS);
		DriverAttributes->addInt("MaxSupportedTextures", SAPPHIRE_MAX_TEXTURE_LAYERS);
		DriverAttributes->addInt("MaxLights", getMaximalDynamicLightAmount());
		DriverAttributes->addInt("MaxAnisotropy", 1);
		//	DriverAttributes->addInt("MaxUserClipPlanes", 0);
		//	DriverAttributes->addInt("MaxAuxBuffers", 0);
		DriverAttributes->addInt("MaxMultipleRenderTargets", 1);
		DriverAttributes->addInt("MaxIndices", -1);
		DriverAttributes->addInt("MaxTextureSize", -1);
		//	DriverAttributes->addInt("MaxGeometryVerticesOut", 0);
		//	DriverAttributes->addFloat("MaxTextureLODBias", 0.f);
		DriverAttributes->addInt("Version", 1);
		//	DriverAttributes->addInt("ShaderLanguageVersion", 0);
		//	DriverAttributes->addInt("AntiAlias", 0);

		setFog();

		setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

		ViewPort = rect<SINT32>(Position2d(0, 0), dimension2di(screenSize));

		// ���� manipulator
		MeshManipulator = new CMeshManipulator();

		if (FileSystem)
			FileSystem->grab();

		// ���� surface loader

#ifdef SAPPHIRE_COMPILE_WITH_WAL_LOADER_
		SurfaceLoader.push_back(createImageLoaderHalfLife());
		SurfaceLoader.push_back(createImageLoaderWAL());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_LMP_LOADER_
		SurfaceLoader.push_back(createImageLoaderLMP());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PPM_LOADER_
		SurfaceLoader.push_back(createImageLoaderPPM());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_RGB_LOADER_
		SurfaceLoader.push_back(createImageLoaderRGB());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PSD_LOADER_
		SurfaceLoader.push_back(createImageLoaderPSD());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_DDS_LOADER_
		SurfaceLoader.push_back(createImageLoaderDDS());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PCX_LOADER_
		SurfaceLoader.push_back(createImageLoaderPCX());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_TGA_LOADER_
		SurfaceLoader.push_back(createImageLoaderTGA());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PNG_LOADER_
		SurfaceLoader.push_back(createImageLoaderPNG());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_JPG_LOADER_
		SurfaceLoader.push_back(createImageLoaderJPG());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_BMP_LOADER_
		SurfaceLoader.push_back(createImageLoaderBMP());
#endif


#ifdef SAPPHIRE_COMPILE_WITH_PPM_WRITER_
		SurfaceWriter.push_back(createImageWriterPPM());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PCX_WRITER_
		SurfaceWriter.push_back(createImageWriterPCX());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PSD_WRITER_
		SurfaceWriter.push_back(createImageWriterPSD());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_TGA_WRITER_
		SurfaceWriter.push_back(createImageWriterTGA());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_JPG_WRITER_
		SurfaceWriter.push_back(createImageWriterJPG());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_PNG_WRITER_
		SurfaceWriter.push_back(createImageWriterPNG());
#endif
#ifdef SAPPHIRE_COMPILE_WITH_BMP_WRITER_
		SurfaceWriter.push_back(createImageWriterBMP());
#endif


		// ��0���ExposedData 
		memset(&ExposedData, 0, sizeof(ExposedData));
		for (UINT32 i = 0; i<EVDF_COUNT; ++i)
			FeatureEnabled[i] = true;

		InitMaterial2D.AntiAliasing = EAAM_OFF;
		InitMaterial2D.Lighting = false;
		InitMaterial2D.ZWriteEnable = false;
		InitMaterial2D.ZBuffer = ECFN_NEVER;
		InitMaterial2D.UseMipMaps = false;
		for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
		{
			InitMaterial2D.TextureLayer[i].BilinearFilter = false;
			InitMaterial2D.TextureLayer[i].TextureWrapU = ETC_REPEAT;
			InitMaterial2D.TextureLayer[i].TextureWrapV = ETC_REPEAT;
		}
		OverrideMaterial2D = InitMaterial2D;
	}


	 
	CNullDriver::~CNullDriver()
	{
		if (DriverAttributes)
			DriverAttributes->drop();

		if (FileSystem)
			FileSystem->drop();

		if (MeshManipulator)
			MeshManipulator->drop();
		deleteAllTextures();

		UINT32 i;
		for (i = 0; i<SurfaceLoader.size(); ++i)
			SurfaceLoader[i]->drop();

		for (i = 0; i<SurfaceWriter.size(); ++i)
			SurfaceWriter[i]->drop();

		// ɾ��������Ⱦ��
		deleteMaterialRenders();

		// ɾ������Ӳ�����񻺳���
		removeAllHardwareBuffers();
	}


	//! ���һ����չ��surface loader ������
	void CNullDriver::addExternalImageLoader(IImageLoader* loader)
	{
		if (!loader)
			return;

		loader->grab();
		SurfaceLoader.push_back(loader);
	}


	//! ���һ����չsurface writer������
	void CNullDriver::addExternalImageWriter(IImageWriter* writer)
	{
		if (!writer)
			return;

		writer->grab();
		SurfaceWriter.push_back(writer);
	}


	//! ����ͼ�������������
	UINT32 CNullDriver::getImageLoaderCount() const
	{
		return SurfaceLoader.size();
	}


	//! ���ظ�����ͼ�������
	IImageLoader* CNullDriver::getImageLoader(UINT32 n)
	{
		if (n < SurfaceLoader.size())
			return SurfaceLoader[n];
		return 0;
	}


	//! ����ͼ��д����������
	UINT32 CNullDriver::getImageWriterCount() const
	{
		return SurfaceWriter.size();
	}


	//! ���ظ���ͼ��д����
	IImageWriter* CNullDriver::getImageWriter(UINT32 n)
	{
		if (n < SurfaceWriter.size())
			return SurfaceWriter[n];
		return 0;
	}


	//! ����һ������
	void CNullDriver::deleteAllTextures()
	{
		// ��Ҫһ��֮ǰ���õ�����Ҫ��Ȼ���ǻᱣ��֮ǰ���ò��ʳ�Ա�������Ż�����״̬�ı�
		setMaterial(SMaterial());

		for (UINT32 i = 0; i<Textures.size(); ++i)
			Textures[i].Surface->drop();

		Textures.clear();
	}



	//! Ӧ�ó���������κ���Ⱦ��ʼ֮ǰ�����������������false���ʾʧ��
	bool CNullDriver::beginScene(bool backBuffer, bool zBuffer, ColourValue color,
		const SExposedVideoData& videoData, rect<SINT32>* sourceRect)
	{
		clearFPUException();
		PrimitivesDrawn = 0;
		return true;
	}


	//! //������Ⱦ����  Ӧ�ó��������������Ⱦ������������ã� ����false��ʾʧ��
	bool CNullDriver::endScene()
	{
		FPSCounter.registerFrame(Timer::getRealTime(), PrimitivesDrawn);
		updateAllHardwareBuffers();
		updateAllOcclusionQueries();
		return true;
	}


	//! �ر�������һ������
	void CNullDriver::disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag)
	{
		FeatureEnabled[feature] = !flag;
	}


	//! ��ѯ�������������������Ч������true
	bool CNullDriver::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
	{
		return false;
	}


	//! ��ȡʵ����Ƶ����������
	const IAttributes& CNullDriver::getDriverAttributes() const
	{
		return *DriverAttributes;
	}


	//! ���ñ任  ����������D3D��OGL����ȥʵ�ֵģ�
	void CNullDriver::setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat)
	{
	}


	//! ����ͼ�������������
	const Matrix4& CNullDriver::getTransform(E_TRANSFORMATION_STATE state) const
	{
		return TransformationMatrix;
	}


	//! ����һ������ ����������D3D��OGL����ȥʵ�ֵģ�
	void CNullDriver::setMaterial(const SMaterial& material)
	{
	}


	//! ���������Ƴ�һ��������ɾ�������ͷ��ڴ�
	void CNullDriver::removeTexture(ITexture* texture)
	{
		if (!texture)
			return;

		for (UINT32 i = 0; i<Textures.size(); ++i)
		{
			if (Textures[i].Surface == texture)
			{
				texture->drop();
				vector<SSurface>::iterator it = Textures.begin()+i;
				Textures.erase(it);
			}
		}
	}


	//! ���������Ƴ�����������ɾ�������ͷ��ڴ�
	void CNullDriver::removeAllTextures()
	{
		setMaterial(SMaterial());
		deleteAllTextures();
	}


	//! ����һ����������
	ITexture* CNullDriver::getTextureByIndex(UINT32 i)
	{
		if (i < Textures.size())
			return Textures[i].Surface;

		return 0;
	}


	//! ���ص�ǰ���ص���������
	UINT32 CNullDriver::getTextureCount() const
	{
		return Textures.size();
	}


	//! ������һ������ 
	void CNullDriver::renameTexture(ITexture* texture, const path& newName)
	{
		// �����ܹ��������һ����ȫ��const_cast, ��Ϊ��Щ����������������Ҫ�����������ITexture�ӿڵ�������ֻ���ģ��Ա����û���ͨ����������������ı�
		SNamedPath& name = const_cast<SNamedPath&>(texture->getName());
		name.setPath(newName);
		
		std::stable_sort(Textures.begin(), Textures.end());
	}


	//! ��·���ļ�������һ������
	ITexture* CNullDriver::getTexture(const path& filename)
	{
		// ͨ�������ļ�����ʾ����
		const path absolutePath = FileSystem->getAbsolutePath(filename);
		//��������
		ITexture* texture = findTexture(absolutePath);
		if (texture)
			return texture;

		// ����ʧ�ܣ���ô����Դ�ļ����������Լ���һ��Archive
		texture = findTexture(filename);
		if (texture)
			return texture;

		// ����ʧ�ܣ������ٳ�����������·��������ļ�
		IReadFile* file = FileSystem->createAndOpenFile(absolutePath);
		//����ļ�����
		if (!file)
		{
			// ������Դ�ļ�����
			file = FileSystem->createAndOpenFile(filename);
		}

		if (file)
		{
			// �ٴμ��ʵ�ʵ�����
			texture = findTexture(file->getFileName());
			if (texture)
			{
				file->drop();
				return texture;
			}

			texture = loadTextureFromFile(file);
			file->drop();

			if (texture)
			{
				addTexture(texture);
				texture->drop(); // ������ɣ��Ժ󲻻��õ�������һ������
			}
			else
				Printer::log("Could not load texture", filename, LML_ERROR);
			return texture;
		}
		else
		{
			Printer::log("Could not open file of texture", filename, LML_WARNING);
			return 0;
		}
	}


	//! ��IReadFile����һ������
	ITexture* CNullDriver::getTexture(IReadFile* file)
	{
		ITexture* texture = 0;

		if (file)
		{
			texture = findTexture(file->getFileName());

			if (texture)
				return texture;

			texture = loadTextureFromFile(file);

			if (texture)
			{
				addTexture(texture);
				texture->drop();  
			}

			if (!texture)
				Printer::log("Could not load texture", file->getFileName(), LML_WARNING);
		}

		return texture;
	}


	//! �������ļ���������������
	ITexture* CNullDriver::loadTextureFromFile(IReadFile* file, const path& hashName)
	{
		ITexture* texture = 0;
		//�ȼ���ͼ��
		IImage* image = createImageFromFile(file);

		if (image)
		{
			// �ӱ��洴������
			texture = createDeviceDependentTexture(image, hashName.size() ? hashName : file->getFileName());
			Printer::log("Loaded texture", file->getFileName());
			image->drop();
		}

		return texture;
	}


	//! ���һ�����棬 �����ػ�ͨ�����洴��
	void CNullDriver::addTexture(ITexture* texture)
	{
		if (texture)
		{
			SSurface s;
			s.Surface = texture;
			//����һ������
			texture->grab();

			Textures.push_back(s);

			// �µ����������������б��ĩβ������ѯ��һ������������������齫�������������������ᱻ�ı䡣
			

			//std::stable_sort(Textures.begin(), Textures.end(), ITextureCMP);
			std::stable_sort(Textures.begin(), Textures.end());
		}
	}


	//! ��ѯͼ���Ƿ񱻼���
	ITexture* CNullDriver::findTexture(const path& filename)
	{
		SSurface s;   //һ��ͼ�����
		SDummyTexture dummy(filename);  
		s.Surface = &dummy;

		vector<SSurface>::type::iterator it = std::find(Textures.begin(), Textures.end(), s);// Textures.binary_search(s);
		if (it != std::end(Textures))
			return it->Surface;
			//return //Textures[index].Surface;

		return 0;
	}


	//! ��һ���Ѽ��ص�ͼ�񴴽�һ������
	ITexture* CNullDriver::addTexture(const path& name, IImage* image, void* mipmapData)
	{
		if (0 == name.size() || !image)
			return 0;

		ITexture* t = createDeviceDependentTexture(image, name, mipmapData);
		if (t)
		{
			addTexture(t);
			t->drop();
		}
		return t;
	}


	//! ����һ������
	ITexture* CNullDriver::addTexture(const dimension2d<UINT32>& size,
		const path& name, ECOLOR_FORMAT format)
	{
		//�ж�ͼ���Ƿ���֧�ֵĸ�ʽ
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create ITexture, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		if (0 == name.size())
			return 0;

		IImage* image = new CImage(format, size);
		//�����豸����������
		ITexture* t = createDeviceDependentTexture(image, name);
		image->drop();
		addTexture(t);

		if (t)
			t->drop();

		return t;
	}



	//! ��һ���������IImage����һ���豸����������
	//! ��������ᱻ���������Լ�������������
	ITexture* CNullDriver::createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData)
	{
		return new SDummyTexture(name);
	}


	//! ���û�����һ���ض�����ȾĿ��
	bool CNullDriver::setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
		bool clearZBuffer, ColourValue color)
	{
		if (ERT_FRAME_BUFFER == target)  //�����֡����Ļ�,�����ɫ��������Z����
			return setRenderTarget(0, clearTarget, clearZBuffer, color);
		else
			return false;
	}


	//! ������ȾĿ�꣨�������豸�������ǣ�
	bool CNullDriver::setRenderTarget(ITexture* texture, bool clearBackBuffer,
		bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! ���ö����ȾĿ�꣨�������豸�������ǣ�
	bool CNullDriver::setRenderTarget(const vector<IRenderTarget>::type& texture,
		bool clearBackBuffer, bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! ����һ���ӿڣ��������豸�������ǣ�
	void CNullDriver::setViewPort(const rect<SINT32>& area)
	{
	}


	//! ��ȡ��ǰ�ӿ�����
	const rect<SINT32>& CNullDriver::getViewPort() const
	{
		return ViewPort;
	}


	//! ����һ������ͼԪ�б��������豸�������ǣ�
	void CNullDriver::drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! ����2d�Ķ����ͼԪ�б��������豸�������ǣ�
	void CNullDriver::draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! ����һ��3d�߶Σ��������豸�������ǣ�
	void CNullDriver::draw3DLine(const Vector3& start,
		const Vector3& end, ColourValue color)
	{
	}


	//! ����һ��3d������
	void CNullDriver::draw3DTriangle(const triangle3df& triangle, ColourValue color)
	{
		//���������ζ�������
		S3DVertex vertices[3];
		vertices[0].Pos = triangle.pointA;
		vertices[0].Color = color;
		vertices[0].Normal = triangle.getNormal().normalise();
		vertices[0].TCoords = Vector2(0.f, 0.f);
		vertices[1].Pos = triangle.pointB;
		vertices[1].Color = color;
		vertices[1].Normal = vertices[0].Normal;
		vertices[1].TCoords = Vector2(0.5f, 1.f);
		vertices[2].Pos = triangle.pointC;
		vertices[2].Color = color;
		vertices[2].Normal = vertices[0].Normal;
		vertices[2].TCoords = Vector2(1.f, 0.f);
		const UINT16 indexList[] = { 0, 1, 2 };
		//���ƶ���ͼԪ�б�
		drawVertexPrimitiveList(vertices, 3, indexList, 1, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}


	//! ����һ��3d AABB����
	void CNullDriver::draw3DBox(const AxisAlignedBox& box, ColourValue color)
	{
		//Vector3 edges[8];
		//box.getEdges(edges);

		// TODO: ���Ż���һ�����͵�drawIndexPrimitive����
		

		draw3DLine(box.getCorner(box.FAR_LEFT_TOP), box.getCorner(box.FAR_LEFT_BOTTOM), color);
		draw3DLine(box.getCorner(box.FAR_LEFT_TOP), box.getCorner(box.FAR_RIGHT_TOP), color);
		draw3DLine(box.getCorner(box.FAR_LEFT_TOP), box.getCorner(box.NEAR_LEFT_TOP), color);
		draw3DLine(box.getCorner(box.NEAR_LEFT_BOTTOM), box.getCorner(box.NEAR_LEFT_TOP), color);
		draw3DLine(box.getCorner(box.NEAR_LEFT_BOTTOM), box.getCorner(box.NEAR_RIGHT_BOTTOM), color);
		draw3DLine(box.getCorner(box.NEAR_LEFT_BOTTOM), box.getCorner(box.FAR_LEFT_BOTTOM), color);
		draw3DLine(box.getCorner(box.NEAR_RIGHT_TOP), box.getCorner(box.NEAR_LEFT_TOP), color);
		draw3DLine(box.getCorner(box.NEAR_RIGHT_TOP), box.getCorner(box.NEAR_RIGHT_BOTTOM), color);
		draw3DLine(box.getCorner(box.NEAR_RIGHT_TOP), box.getCorner(box.FAR_RIGHT_TOP), color);
		draw3DLine(box.getCorner(box.FAR_RIGHT_BOTTOM), box.getCorner(box.FAR_RIGHT_TOP), color);
		draw3DLine(box.getCorner(box.FAR_RIGHT_BOTTOM), box.getCorner(box.FAR_LEFT_BOTTOM), color);
		draw3DLine(box.getCorner(box.FAR_RIGHT_BOTTOM), box.getCorner(box.NEAR_RIGHT_BOTTOM), color);

	}



	//! ����һ��2dͼ��
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos)
	{
		if (!texture)
			return;

		draw2DImage(texture, destPos, rect<SINT32>(Position2d(0, 0),
			dimension2di(texture->getOriginalSize())));
	}




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
	void CNullDriver::draw2DImageBatch(const ITexture* texture,
		const Position2d& pos,
		const vector<rect<SINT32> >::type& sourceRects,
		const vector<SINT32>::type& indices,
		SINT32 kerningWidth,
		const rect<SINT32>* clipRect, ColourValue color,
		bool useAlphaChannelOfTexture)
	{
		Position2d target(pos);

		for (UINT32 i = 0; i<indices.size(); ++i)
		{
			draw2DImage(texture, target, sourceRects[indices[i]],
				clipRect, color, useAlphaChannelOfTexture);
			target.x += sourceRects[indices[i]].getWidth();
			target.x += kerningWidth;
		}
	}

	//! ��һ����ɫ�������alphatͨ���������Ҫ������һ��2dͼ��
	void CNullDriver::draw2DImageBatch(const ITexture* texture,
		const vector<Position2d>::type& positions,
		const vector<rect<SINT32> >::type& sourceRects,
		const rect<SINT32>* clipRect,
		ColourValue color,
		bool useAlphaChannelOfTexture)
	{
		const UINT32 drawCount = Math::_min<UINT32>(positions.size(), sourceRects.size());

		for (UINT32 i = 0; i<drawCount; ++i)
		{
			draw2DImage(texture, positions[i], sourceRects[i],
				clipRect, color, useAlphaChannelOfTexture);
		}
	}


	//! ��һ�������л��������һ����
	void CNullDriver::draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
		const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect,
		const ColourValue* const colors, bool useAlphaChannelOfTexture)
	{
		if (destRect.isValid())
			draw2DImage(texture, Position2d(destRect.UpperLeftCorner),
			sourceRect, clipRect, colors ? colors[0] : ColourValue(0xffffffff),
			useAlphaChannelOfTexture);
	}


	//! ��һ����ɫ�������alphatͨ���������Ҫ������һ��2dͼ��
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos,
		const rect<SINT32>& sourceRect,
		const rect<SINT32>* clipRect, ColourValue color,
		bool useAlphaChannelOfTexture)
	{
	}


	//! ����һ�����ߵ�2d����
	void CNullDriver::draw2DRectangleOutline(const recti& pos, ColourValue color)
	{
		draw2DLine(pos.UpperLeftCorner, Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), color);
		draw2DLine(Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), pos.LowerRightCorner, color);
		draw2DLine(pos.LowerRightCorner, Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), color);
		draw2DLine(Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), pos.UpperLeftCorner, color);
	}


	//!����һ��2d����
	void CNullDriver::draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip)
	{
		draw2DRectangle(pos, color, color, color, color, clip);
	}



	//! ����һ������ľ���
	void CNullDriver::draw2DRectangle(const rect<SINT32>& pos,
		ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
		const rect<SINT32>* clip)
	{
	}



	//! ����һ��2D���߶�
	void CNullDriver::draw2DLine(const Position2d& start,
		const Position2d& end, ColourValue color)
	{
	}

	//! ����һ������
	void CNullDriver::drawPixel(UINT32 x, UINT32 y, const ColourValue & color)
	{
	}


	//! ����һ�������Ĺ�Բ������2d�����
	void CNullDriver::draw2DPolygon(Position2d center,
		Real radius, ColourValue color, SINT32 count)
	{
		if (count < 2)
			return;

		Position2d first;
		Position2d a, b;

		for (SINT32 j = 0; j<count; ++j)
		{
			b = a;

			Real p = j / (Real)count * (PI * 2);
			a = center + Position2d((SINT32)(sin(p)*radius), (SINT32)(cos(p)*radius));

			if (j == 0)
				first = a;
			else
				draw2DLine(a, b, color);
		}

		draw2DLine(a, first, color);
	}


	//! ������ɫ��ʽ
	ECOLOR_FORMAT CNullDriver::getColorFormat() const
	{
		return ECF_R5G6B5;
	}


	//! ������Ļ��С
	const dimension2d<UINT32>& CNullDriver::getScreenSize() const
	{
		return ScreenSize;
	}


	//! �����˵�ǰ��ȾĿ���С 
	const dimension2d<UINT32>& CNullDriver::getCurrentRenderTargetSize() const
	{
		return ScreenSize;
	}


	// ��ȡ��ǰFPS��
	SINT32 CNullDriver::getFPS() const
	{
		return FPSCounter.getFPS();
	}



	//! ��������һ֡�л��Ƶ�ͼԪ����������������Σ�
	UINT32 CNullDriver::getPrimitiveCountDrawn(UINT32 param) const
	{
		return (0 == param) ? FPSCounter.getPrimitive() : (1 == param) ? FPSCounter.getPrimitiveAverage() : FPSCounter.getPrimitiveTotal();
	}



	//! ���ö�̬��������ɫ�����Ĭ��ֵ�ǣ�0��0��0��0����ɫ
	//! \param color: �����õĻ�������ɫ
	void CNullDriver::setAmbientLight(const ColourValue& color)
	{
	}



	//! \return ������Ƶ������: ����DIRECT3D8���������ء�Direct3D8.1��
	const wchar_t* CNullDriver::getName() const
	{
		return L"Irrlicht NullDevice";
	}



	//! ����һ����Ӱ�ݻ���ģ�建�����С�Ҫ����һ��ģ����Ӱ������,�����м����塣�������������ȥ������Ӱ�ݻ���
	//Ȼ����IVideoDriver::drawStencilShadow()ȥ���ӻ���Ӱ
	void CNullDriver::drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible)
	{
	}


	//! ����IVideoDriver::drawStencilShadowVolume()��Ӱ�ݻ����Ƶ�ģ�建����������ɫ���ģ����Ӱ��
	//! �������������Ӱ����ɫ
	void CNullDriver::drawStencilShadow(bool clearStencilBuffer,
		ColourValue leftUpEdge, ColourValue rightUpEdge,
		ColourValue leftDownEdge, ColourValue rightDownEdge)
	{
	}


	//! ɾ�����ж�̬��Դ
	void CNullDriver::deleteAllDynamicLights()
	{
		Lights.clear();
	}


	//! ���һ����̬��Դ�����������Դ������
	//! \param light: Ҫ������Դ�Ĺ�Դ����
	//! \return �����Դ��������-1�Ƿ�������
	SINT32 CNullDriver::addDynamicLight(const SLight& light)
	{
		Lights.push_back(light);
		return Lights.size() - 1;
	}

	//! �򿪻�ر�һ����̬��Դ
	//! \param lightIndex: addDynamicLight�����ص�����
	//! \param turnOn: trueΪ�򿪣�falseΪ�ر�
	void CNullDriver::turnLightOn(SINT32 lightIndex, bool turnOn)
	{
		// Do nothing
	}


	//! ��������豸�ܹ�֧�ֵĶ�̬��Դ���������
	UINT32 CNullDriver::getMaximalDynamicLightAmount() const
	{
		return 0;
	}


	//! ���ص�ǰ��̬��Դ��������
	//! \return ��ǰ���õĶ�̬��Դ����
	UINT32 CNullDriver::getDynamicLightCount() const
	{
		return Lights.size();
	}


	//! ����֮ǰͨ��IVideDriver::addDynamicLight()���õĹ�Դ����
	//! \param idx: ������0��ʼ��С��IVideoDriver()::getDynamicLightCount
	//! \return ��Դ����SLight
	const SLight& CNullDriver::getDynamicLight(UINT32 idx) const
	{
		if (idx < Lights.size())
			return Lights[idx];
		else
			return *((SLight*)0);
	}


	//! ����һ��1λ�Ļ���һ��ɫ��λ�õ�����alphaͨ��
	void CNullDriver::makeColorKeyTexture(ITexture* texture,
		ColourValue color,
		bool zeroTexels) const
	{
		if (!texture)
			return;
		//�ж���ɫ��ʽ
		if (texture->getColorFormat() != ECF_A1R5G5B5 &&
			texture->getColorFormat() != ECF_A8R8G8B8)
		{
			Printer::log("Error: Unsupported texture color format for making color key channel.", LML_ERROR);
			return;
		}
		//A1R5G5B5��ʽ
		if (texture->getColorFormat() == ECF_A1R5G5B5)
		{
			//��������
			UINT16 *p = (UINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_ERROR);
				return;
			}
			//��ȡ����Ĵ�С
			const dimension2d<UINT32> dim = texture->getSize();
			//��ȡ������п��(ÿ���ֽ���)����Ϊÿ����ռ16λ�������п��/2��
			const UINT32 pitch = texture->getPitch() / 2;

			// ��alpha�رյ���ɫ ����ɫ��0x7fff���룬������ȡR5G5B5���֣�
			const UINT16 refZeroAlpha = (0x7fff & A8R8G8B8toA1R5G5B5(color.getAsARGB()));
			//�������һ������λ��
			const UINT32 pixels = pitch * dim.Height;

			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// ���ƥ�������ɫ����Чalphaֵ
				// �������alphaֵΪ0
				if (((*p) & 0x7fff) == refZeroAlpha)
				{
					if (zeroTexels)
						(*p) = 0;
					else
						(*p) = refZeroAlpha;
				}

				++p;
			}

			texture->unlock();
		}
		else
		{
			UINT32 *p = (UINT32*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_CRITICAL);
				return;
			}
			//�����С
			dimension2d<UINT32> dim = texture->getSize();
			//��ȡ������п��(ÿ���ֽ���)����Ϊÿ����ռ16λ�������п��/4��
			UINT32 pitch = texture->getPitch() / 4;

			// ��alpha�رյ���ɫ ����ɫ��0x7fff���룬������ȡR5G5B5���֣�
			const UINT32 refZeroAlpha = 0x00ffffff & A8R8G8B8toA1R5G5B5(color.getAsARGB());
			//�������һ������λ��
			const UINT32 pixels = pitch * dim.Height;
			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// ���ƥ�������ɫ����Чalphaֵ
				// �������alphaֵΪ0
				if (((*p) & 0x00ffffff) == refZeroAlpha)
				{
					if (zeroTexels)
						(*p) = 0;
					else
						(*p) = refZeroAlpha;
				}

				++p;
			}

			texture->unlock();
		}
		texture->regenerateMipMapLevels(); //�ؽ�MIPMAP
	}



	//! ����һ��1λ�Ļ���һ��ɫ��λ�õ�����alphaͨ��
	void CNullDriver::makeColorKeyTexture(ITexture* texture,
		Position2d colorKeyPixelPos,
		bool zeroTexels) const
	{
		if (!texture)
			return;

		if (texture->getColorFormat() != ECF_A1R5G5B5 &&
			texture->getColorFormat() != ECF_A8R8G8B8)
		{
			Printer::log("Error: Unsupported texture color format for making color key channel.", LML_CRITICAL);
			return;
		}

		ColourValue colorKey;

		if (texture->getColorFormat() == ECF_A1R5G5B5)
		{
			UINT16 *p = (UINT16*)texture->lock(ETLM_READ_ONLY);

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_CRITICAL);
				return;
			}

			UINT32 pitch = texture->getPitch() / 2;

			const UINT16 key16Bit = 0x7fff & p[colorKeyPixelPos.getYi()*pitch + colorKeyPixelPos.getXi()];

			colorKey = A1R5G5B5toA8R8G8B8(key16Bit);
		}
		else
		{
			UINT32 *p = (UINT32*)texture->lock(ETLM_READ_ONLY);

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_CRITICAL);
				return;
			}

			UINT32 pitch = texture->getPitch() / 4;
			colorKey = 0x00ffffff & p[colorKeyPixelPos.getYi()*pitch + colorKeyPixelPos.getXi()];
		}

		texture->unlock();
		makeColorKeyTexture(texture, colorKey, zeroTexels);
	}



	//! �߳�����ͼ����һ��������ͼ
	//! \param amplitude: �߶���Ϣ����������ֵ
	void CNullDriver::makeNormalMapTexture(ITexture* texture, FLOAT32 amplitude) const
	{
		if (!texture)
			return;

		if (texture->getColorFormat() != ECF_A1R5G5B5 &&
			texture->getColorFormat() != ECF_A8R8G8B8)
		{
			Printer::log("Error: Unsupported texture color format for making normal map.", LML_CRITICAL);
			return;
		}
		//��ȡ�����С
		dimension2d<UINT32> dim = texture->getSize();
		amplitude = amplitude / 255.0f;
		FLOAT32 vh = dim.Height / (FLOAT32)dim.Width;  //���ݱ�
		FLOAT32 hh = dim.Width / (FLOAT32)dim.Height;  //�ݺ��

		if (texture->getColorFormat() == ECF_A8R8G8B8)
		{
			// ECF_A8R8G8B8 �汾

			SINT32 *p = (SINT32*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			// ��������
			UINT32 pitch = texture->getPitch() / 4;
			SINT32* in = new SINT32[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 4);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: �������ͨ���Ż�

					Vector3 h1((x - 1)*hh, nml32(x - 1, y, pitch, dim.Height, in)*amplitude, y*vh);
					Vector3 h2((x + 1)*hh, nml32(x + 1, y, pitch, dim.Height, in)*amplitude, y*vh);
					//Vector3 v1(x*hh, nml32(x, y-1, pitch, dim.Height, in)*amplitude, (y-1)*vh);
					//Vector3 v2(x*hh, nml32(x, y+1, pitch, dim.Height, in)*amplitude, (y+1)*vh);
					Vector3 v1(x*hh, nml32(x, y + 1, pitch, dim.Height, in)*amplitude, (y - 1)*vh);
					Vector3 v2(x*hh, nml32(x, y - 1, pitch, dim.Height, in)*amplitude, (y + 1)*vh);

					Vector3 v = v1 - v2;
					Vector3 h = h1 - h2;

					Vector3 n = v.crossProduct(h);
					n.normalise();
					n *= 0.5f;
					n += Vector3(0.5f, 0.5f, 0.5f); // 0��1֮��
					n *= 255.0f;

					SINT32 height = (SINT32)nml32(x, y, pitch, dim.Height, in);
					p[y*pitch + x] = ColourValue(
						height, // ��alpha�б���߶���Ϣ
						(SINT32)n.x, (SINT32)n.z, (SINT32)n.y).getAsARGB();
				}

			delete[] in;
			texture->unlock();
		}
		else
		{
			// ECF_A1R5G5B5 ��

			SINT16 *p = (SINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			UINT32 pitch = texture->getPitch() / 2;

			// ��������

			SINT16* in = new SINT16[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 2);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: �ɺܶ���Ż�

					Vector3 h1((x - 1)*hh, nml16(x - 1, y, pitch, dim.Height, in)*amplitude, y*vh);
					Vector3 h2((x + 1)*hh, nml16(x + 1, y, pitch, dim.Height, in)*amplitude, y*vh);
					Vector3 v1(x*hh, nml16(x, y - 1, pitch, dim.Height, in)*amplitude, (y - 1)*vh);
					Vector3 v2(x*hh, nml16(x, y + 1, pitch, dim.Height, in)*amplitude, (y + 1)*vh);

					Vector3 v = v1 - v2;
					Vector3 h = h1 - h2;

					Vector3 n = v.crossProduct(h);
					n.normalise();
					n *= 0.5f;
					n += Vector3(0.5f, 0.5f, 0.5f); // now between 0 and 1
					n *= 255.0f;

					p[y*pitch + x] = RGBA16((UINT32)n.x, (UINT32)n.z, (UINT32)n.y);
				}

			delete[] in;
			texture->unlock();
		}

		texture->regenerateMipMapLevels();
	}

	//! �����豸���ܹ���drawIndexedTriangleList��Ⱦ��ͼԪ��������� 
	UINT32 CNullDriver::getMaximalPrimitiveCount() const
	{
		return 0xFFFFFFFF;
	}


	//! ���������������������ν�������
	bool CNullDriver::checkPrimitiveCount(UINT32 prmCount) const
	{
		const UINT32 m = getMaximalPrimitiveCount();

		if (prmCount > m)
		{
			char tmp[1024];
			sprintf(tmp, "Could not draw triangles, too many primitives(%u), maxium is %u.", prmCount, m);
			Printer::log(tmp, LML_CRITICAL);
			return false;
		}

		return true;
	}

	//! �򿪻�ر�һ����������׼
	void CNullDriver::setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled)
	{
		if (enabled && ((flag == ETCF_ALWAYS_16_BIT) || (flag == ETCF_ALWAYS_32_BIT)
			|| (flag == ETCF_OPTIMIZED_FOR_QUALITY) || (flag == ETCF_OPTIMIZED_FOR_SPEED)))
		{
			// �ر�������ʽ
			setTextureCreationFlag(ETCF_ALWAYS_16_BIT, false);
			setTextureCreationFlag(ETCF_ALWAYS_32_BIT, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED, false);
		}

		// ���ñ�־
		TextureCreationFlags = (TextureCreationFlags & (~flag)) |
			((((UINT32)!enabled) - 1) & flag);
	}


	//! ����һ����������׼�Ǵ򿪻�ر�
	bool CNullDriver::getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const
	{
		return (TextureCreationFlags & flag) != 0;
	}


	//! ���ļ�����һ�����ͼ��
	IImage* CNullDriver::createImageFromFile(const path& filename)
	{
		if (!filename.size())
			return 0;

		IImage* image = 0;
		IReadFile* file = FileSystem->createAndOpenFile(filename);

		if (file)
		{
			image = createImageFromFile(file);
			file->drop();
		}
		else
			Printer::log("Could not open file of image", filename, LML_WARNING);

		return image;
	}


	//! ���ļ�����һ�����ͼ��
	IImage* CNullDriver::createImageFromFile(IReadFile* file)
	{
		if (!file)
			return 0;

		IImage* image = 0;

		SINT32 i;

		// ���Ի����ļ���չ���������ļ�
		for (i = SurfaceLoader.size() - 1; i >= 0; --i)
		{
			if (SurfaceLoader[i]->isALoadableFileExtension(file->getFileName()))
			{
				// �����ļ�λ�ã������ܱ�ǰһ��ͼ����ظı�
				file->seek(0);
				image = SurfaceLoader[i]->loadImage(file);
				if (image)
					return image;
			}
		}


		for (i = SurfaceLoader.size() - 1; i >= 0; --i)
		{
			 
			file->seek(0);
			if (SurfaceLoader[i]->isALoadableFileFormat(file))
			{
				file->seek(0);
				image = SurfaceLoader[i]->loadImage(file);
				if (image)
					return image;
			}
		}

		return 0;  
	}


	//! д���ṩ��ͼ�񵽴����ļ� 
	bool CNullDriver::writeImageToFile(IImage* image, const path& filename, UINT32 param)
	{
		IWriteFile* file = FileSystem->createAndWriteFile(filename);
		if (!file)
			return false;

		bool result = writeImageToFile(image, file, param);
		file->drop();

		return result;
	}

	//! д���ṩ��ͼ�񵽴����ļ� 
	bool CNullDriver::writeImageToFile(IImage* image, IWriteFile * file, UINT32 param)
	{
		if (!file)
			return false;

		for (SINT32 i = SurfaceWriter.size() - 1; i >= 0; --i)
		{
			if (SurfaceWriter[i]->isAWriteableFileExtension(file->getFileName()))
			{
				bool written = SurfaceWriter[i]->writeImage(file, image, param);
				if (written)
					return true;
			}
		}
		return false;  
	}


	//! ��һ���ֽ����鴴��һ�����ͼ��
	IImage* CNullDriver::createImageFromData(ECOLOR_FORMAT format,
		const dimension2d<UINT32>& size,
		void *data, bool ownForeignMemory,
		bool deleteMemory)
	{
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create IImage, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		return new CImage(format, size, data, ownForeignMemory, deleteMemory);
	}


	//! ����һ�������ͼ��
	IImage* CNullDriver::createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size)
	{
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create IImage, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		return new CImage(format, size);
	}


	//! �������ͼ�񴴽�һ�����ͼ��
	IImage* CNullDriver::createImage(ECOLOR_FORMAT format, IImage *imageToCopy)
	{
		Printer::log("Deprecated method, please create an empty image instead and use copyTo().", LML_WARNING);
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create IImage, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		CImage* tmp = new CImage(format, imageToCopy->getDimension());
		imageToCopy->copyTo(tmp);
		return tmp;
	}


	//! ������һ��ͼ���һ����������һ�����ͼ��
	IImage* CNullDriver::createImage(IImage* imageToCopy, const Position2d& pos, const dimension2d<UINT32>& size)
	{
		Printer::log("Deprecated method, please create an empty image instead and use copyTo().", LML_WARNING);
		CImage* tmp = new CImage(imageToCopy->getColorFormat(), imageToCopy->getDimension());
		imageToCopy->copyTo(tmp, Position2d(0, 0), recti(pos, size));
		return tmp;
	}


	//!  ������һ�������һ����������һ�����ͼ��
	IImage* CNullDriver::createImage(ITexture* texture, const Position2d& pos, const dimension2d<UINT32>& size)
	{
		if ((pos == Position2d(0, 0)) && (size == texture->getSize()))
		{
			IImage* image = new CImage(texture->getColorFormat(), size, texture->lock(ETLM_READ_ONLY), false);
			texture->unlock();
			return image;
		}
		else
		{
			// ȷ���������������
			const Vector2 leftUpper(Math::Clamp(static_cast<UINT32>(pos.x), 0u, texture->getSize().Width),
				Math::Clamp(static_cast<UINT32>(pos.y), 0u, texture->getSize().Height));
			const rect<UINT32> clamped(leftUpper,
				dimension2du(Math::Clamp(static_cast<UINT32>(size.Width), 0u, texture->getSize().Width),
				Math::Clamp(static_cast<UINT32>(size.Height), 0u, texture->getSize().Height)));
			if (!clamped.isValid())
				return 0;
		 	
			UINT8* src = static_cast<UINT8*>(texture->lock(ETLM_READ_ONLY));
			if (!src)
				return 0;
			IImage* image = new CImage(texture->getColorFormat(), clamped.getSize());
			UINT8* dst = static_cast<UINT8*>(image->lock());
			src +=  clamped.UpperLeftCorner.getYi() * texture->getPitch() + image->getBytesPerPixel() * clamped.UpperLeftCorner.getXi();
			for (UINT32 i = 0; i<clamped.getHeight(); ++i)
			{
				CColorConverter::convert_viaFormat(src, texture->getColorFormat(), clamped.getWidth(), dst, image->getColorFormat());
				src += texture->getPitch();
				dst += image->getPitch();
			}
			image->unlock();
			texture->unlock();
			return image;
		}
	}


	//! ������ģʽ
	void CNullDriver::setFog(ColourValue color, E_FOG_TYPE fogType, Real start, Real end,
		Real density, bool pixelFog, bool rangeFog)
	{
		FogColor = color;
		FogType = fogType;
		FogStart = start;
		FogEnd = end;
		FogDensity = density;
		PixelFog = pixelFog;
		RangeFog = rangeFog;
	}

	//! ��ȡ��ģʽ
	void CNullDriver::getFog(ColourValue& color, E_FOG_TYPE& fogType, Real& start, Real& end,
		Real& density, bool& pixelFog, bool& rangeFog)
	{
		color = FogColor;
		fogType = FogType;
		start = FogStart;
		end = FogEnd;
		density = FogDensity;
		pixelFog = PixelFog;
		rangeFog = RangeFog;
	}

	//! ����һ�����񻺳���
	void CNullDriver::drawMeshBuffer(const IMeshBuffer* mb)
	{
		if (!mb)
			return;

		//IVertexBuffer �� IIndexBuffer �ں���
		SHWBufferLink *HWBuffer = getBufferLink(mb);

		if (HWBuffer)
			drawHardwareBuffer(HWBuffer);
		else
			drawVertexPrimitiveList(mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount() / 3, mb->getVertexType(), EPT_TRIANGLES, mb->getIndexType());
	}


	//! �������񻺳�ķ���
	void CNullDriver::drawMeshBufferNormals(const IMeshBuffer* mb, Real length, ColourValue color)
	{
		const UINT32 count = mb->getVertexCount();
		const bool normalize = mb->getMaterial().NormalizeNormals;

		for (UINT32 i = 0; i < count; ++i)
		{
			Vector3 normalizedNormal = mb->getNormal(i);
			if (normalize)
				normalizedNormal.normalise();

			const Vector3& pos = mb->getPosition(i);
			draw3DLine(pos, pos + (normalizedNormal * length), color);
		}
	}

	//! ��һ�����񻺳�����ȡӲ������������(���ڴ�������»�����)
	CNullDriver::SHWBufferLink *CNullDriver::getBufferLink(const IMeshBuffer* mb)
	{
		if (!mb || !isHardwareBufferRecommend(mb))
			return 0;

		//��ѰӲ������
		//map< const IMeshBuffer*, SHWBufferLink* >::Node* node = HWBufferMap.find(mb);
		map< const IMeshBuffer*, SHWBufferLink* >::type::iterator node = HWBufferMap.find(mb);
		if (node != std::end(HWBufferMap))//if (node)
			return node->second;

		return createHardwareBuffer(mb); //û��Ӳ�����ӣ����������Ҫһ����������
	}


	//! �����������񻺳������Ƴ����õ�
	void CNullDriver::updateAllHardwareBuffers()
	{
		//map<const IMeshBuffer*, SHWBufferLink*>::ParentFirstIterator Iterator = HWBufferMap.getParentFirstIterator();
		map<const IMeshBuffer*, SHWBufferLink*>::type::iterator it = HWBufferMap.begin();

		//for (; !Iterator.atEnd(); Iterator++)
		for (; it != std::end(HWBufferMap); it++)
		{
			//SHWBufferLink *Link = Iterator.getNode()->getValue();

			SHWBufferLink *Link = it->second;

			Link->LastUsed++;
			if (Link->LastUsed>20000)
			{
				deleteHardwareBuffer(Link);

				// todo: ��Ҫ�����õ�����
				it = HWBufferMap.begin();
			}
		}
	}

	//ɾ��Ӳ��������
	void CNullDriver::deleteHardwareBuffer(SHWBufferLink *HWBuffer)
	{
		if (!HWBuffer)
			return;
		//HWBufferMap.remove(HWBuffer->MeshBuffer);
		HWBufferMap.erase(HWBuffer->MeshBuffer);
		delete HWBuffer;
	}


	//! �Ƴ�Ӳ��������
	void CNullDriver::removeHardwareBuffer(const IMeshBuffer* mb)
	{
		map<const IMeshBuffer*, SHWBufferLink*>::type::iterator Node = HWBufferMap.find(mb);
		if (Node != std::end(HWBufferMap))
			deleteHardwareBuffer(Node->second);
	}


	//! �Ƴ�����Ӳ��������
	void CNullDriver::removeAllHardwareBuffers()
	{
		while (HWBufferMap.size())
		{
			deleteHardwareBuffer((HWBufferMap.begin()->second));
		}
			
	}

	//��������Ƿ��Ƽ�ʹ��VBO
	bool CNullDriver::isHardwareBufferRecommend(const IMeshBuffer* mb)
	{
		if (!mb || (mb->getHardwareMappingHint_Index() == EHM_NEVER && mb->getHardwareMappingHint_Vertex() == EHM_NEVER))
			return false;

		if (mb->getVertexCount()<MinVertexCountForVBO)
			return false;

		return true;
	}


	//! �����ڱβ�ѯocclusion query.
	/**����ȷ�Ϻ��ڱβ��ԵĽڵ� */
	void CNullDriver::addOcclusionQuery(ISceneNode* node, const IMesh* mesh)
	{
		if (!node)
			return;
		if (!mesh)
		{
			if ((node->getType() != ESNT_MESH) && (node->getType() != ESNT_ANIMATED_MESH))
				return;
			else if (node->getType() == ESNT_MESH)
				mesh = static_cast<IMeshSceneNode*>(node)->getMesh();
			else
				mesh = static_cast<IAnimatedMeshSceneNode*>(node)->getMesh()->getMesh(0);
			if (!mesh)
				return;
		}

		//��query���в�ѯ
		//SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		vector<SOccQuery>::type::iterator index = std::find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
		if (index != std::end(OcclusionQueries))
		{
			if (index->Mesh != mesh)
			{
				index->Mesh->drop();
				index->Mesh = mesh;
				mesh->grab();
			}
		}
		else
		{   //û�鵽
			OcclusionQueries.push_back(SOccQuery(node, mesh));
			node->setAutomaticCulling(node->getAutomaticCulling() | EAC_OCC_QUERY);
		}
	}


	//! �Ƴ��ڱβ�ѯocclusion query.
	void CNullDriver::removeOcclusionQuery(ISceneNode* node)
	{
		//����query
		//SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		vector<SOccQuery>::type::iterator index = std::find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
		if (index != std::end(OcclusionQueries))
		{
			node->setAutomaticCulling(node->getAutomaticCulling() & ~EAC_OCC_QUERY);
			OcclusionQueries.erase(index);
		}
	}


	//! �Ƴ����е��ڱβ�ѯ
	void CNullDriver::removeAllOcclusionQueries()
	{
		for (SINT32 i = OcclusionQueries.size() - 1; i >= 0; --i)
		{
			removeOcclusionQuery(OcclusionQueries[i].Node);
		}
	}


	//! �����ڱβ�ѯ�����ƴ���ڲ�ѯ�е�����
	/** �������ɼ����ñ�׼�������ʵĲ��ʵ����� */
	void CNullDriver::runOcclusionQuery(ISceneNode* node, bool visible)
	{
		if (!node)
			return;
		//SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		//��OcclusionQueries�в�ѯ�ڵ�
		vector<SOccQuery>::type::iterator index = std::find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
		if (index == std::end(OcclusionQueries))  //û�ҵ�
			return;
		//OcclusionQueries[index].Run = 0;
		index->Run = 0;
		if (!visible) //������ɼ�
		{
			SMaterial mat;
			mat.Lighting = false;
			mat.AntiAliasing = 0;
			mat.ColorMask = ECP_NONE;
			mat.GouraudShading = false;
			mat.ZWriteEnable = false;
			setMaterial(mat);
		}
		//���ñ任����
		setTransform(ETS_WORLD, node->getAbsoluteTransformation());
		const IMesh* mesh = index->Mesh;
		for (UINT32 i = 0; i<mesh->getMeshBufferCount(); ++i) //ѭ�������������������������񻺳���
		{
			if (visible)  //����ɼ��Ļ�������������񻺳���
				setMaterial(mesh->getMeshBuffer(i)->getMaterial());
			drawMeshBuffer(mesh->getMeshBuffer(i));
		}
	}


	//! �������е��ڱβ�ѯ�����Ʊ����ڲ�ѯ�е�����
	/** �������Ӧ�ò��ɼ�����overrideMaterial�ر���ɫ����Ȼ�����*/
	void CNullDriver::runAllOcclusionQueries(bool visible)
	{
		for (UINT32 i = 0; i<OcclusionQueries.size(); ++i)
			runOcclusionQuery(OcclusionQueries[i].Node, visible);
	}


	//! �����ڱβ�ѯ����GPU���ؽ��
	/** �����ѯ�������������ñ�־Ϊfalse �����������ﲻ�ᷢ�� */
	void CNullDriver::updateOcclusionQuery(ISceneNode* node, bool block)
	{
	}


	//! �������е��ڱβ�ѯ����GPU���ؽ��
	/**�����ѯ�������������ñ�־Ϊfalse �����������ﲻ�ᷢ�� */
	void CNullDriver::updateAllOcclusionQueries(bool block)
	{
		for (UINT32 i = 0; i<OcclusionQueries.size(); ++i)
		{
			if (OcclusionQueries[i].Run == UINT32(~0))
				continue;
			updateOcclusionQuery(OcclusionQueries[i].Node, block);
			++OcclusionQueries[i].Run;
			if (OcclusionQueries[i].Run>1000)
				removeOcclusionQuery(OcclusionQueries[i].Node);
		}
	}


	//! ���ز�ѯ���
	/** ����ֵ�ǿɼ�������/Ƭ�ε����������ֵ��һ����ȫ�Ľ���ֵ���ܹ�����ʵ������ֵ
	*/
	UINT32 CNullDriver::getOcclusionQueryResult(ISceneNode* node) const
	{
		return ~0;
	}


	//! ֻ�������ڲ�ʹ��
	/**�����������������ڴ�С�Ѿ��ı� */
	void CNullDriver::OnResize(const dimension2d<UINT32>& size)
	{
		if (ViewPort.getWidth() == (SINT32)ScreenSize.Width &&
			ViewPort.getHeight() == (SINT32)ScreenSize.Height)
			ViewPort = rect<SINT32>(Position2d(0, 0),
			dimension2di(size));

		ScreenSize = size;
	}


	// ���һ��������Ⱦ������֮�������������ڲ�����
	SINT32 CNullDriver::addAndDropMaterialRenderer(IMaterialRenderer* m)
	{
		SINT32 i = addMaterialRenderer(m);

		if (m)
			m->drop();

		return i;
	}


	//! ���һ���²�����Ⱦ������Ƶ�豸
	SINT32 CNullDriver::addMaterialRenderer(IMaterialRenderer* renderer, const char* name)
	{
		if (!renderer)
			return -1;

		SMaterialRenderer r;
		r.Renderer = renderer;
		r.Name = name;

		if (name == 0 && (MaterialRenderers.size() < (sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1))
		{
			// ����������Ⱦ�������֣����������е���Ⱦ���ж���ʵ����������
			r.Name = sBuiltInMaterialTypeNames[MaterialRenderers.size()];
		}

		MaterialRenderers.push_back(r);
		renderer->grab();

		return MaterialRenderers.size() - 1;
	}


	//! ���ò�����Ⱦ��������
	void CNullDriver::setMaterialRendererName(SINT32 idx, const char* name)
	{
		if (idx < SINT32(sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1 ||
			idx >= (SINT32)MaterialRenderers.size())
			return;

		MaterialRenderers[idx].Name = name;
	}


	//! ��һ�����ʴ������������б������л�����������
	IAttributes* CNullDriver::createAttributesFromMaterial(const SMaterial& material,
		SAttributeReadWriteOptions* options)
	{
		CAttributes* attr = new CAttributes(this);

		attr->addEnum("Type", material.MaterialType, sBuiltInMaterialTypeNames);

		attr->addColor("Ambient", material.AmbientColor);
		attr->addColor("Diffuse", material.DiffuseColor);
		attr->addColor("Emissive", material.EmissiveColor);
		attr->addColor("Specular", material.SpecularColor);

		attr->addFloat("Shininess", material.Shininess);
		attr->addFloat("Param1", material.MaterialTypeParam);
		attr->addFloat("Param2", material.MaterialTypeParam2);

		String prefix = "Texture";
		UINT32 i;
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
		{
			//����ʹ����ʵ·��
			if (options && (options->Flags&EARWF_USE_RELATIVE_PATHS) && (options->Filename.size()>0) && material.getTexture(i))
			{
				path path = FileSystem->getRelativeFilename(
					FileSystem->getAbsolutePath(material.getTexture(i)->getName().getPath()), options->Filename);
				attr->addTexture((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.getTexture(i), path);
			}
			else
				attr->addTexture((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.getTexture(i));
		}

		attr->addBool("Wireframe", material.Wireframe);
		attr->addBool("GouraudShading", material.GouraudShading);
		attr->addBool("Lighting", material.Lighting);
		attr->addBool("ZWriteEnable", material.ZWriteEnable);
		attr->addInt("ZBuffer", material.ZBuffer);
		attr->addBool("BackfaceCulling", material.BackfaceCulling);
		attr->addBool("FrontfaceCulling", material.FrontfaceCulling);
		attr->addBool("FogEnable", material.FogEnable);
		attr->addBool("NormalizeNormals", material.NormalizeNormals);
		attr->addBool("UseMipMaps", material.UseMipMaps);
		attr->addInt("AntiAliasing", material.AntiAliasing);
		attr->addInt("ColorMask", material.ColorMask);
		attr->addInt("ColorMaterial", material.ColorMaterial);
		attr->addInt("PolygonOffsetFactor", material.PolygonOffsetFactor);
		attr->addEnum("PolygonOffsetDirection", material.PolygonOffsetDirection, PolygonOffsetDirectionNames);

		prefix = "BilinearFilter";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addBool((prefix + StringUtil::uint32ToString(i+1)).c_str(), material.TextureLayer[i].BilinearFilter);
		prefix = "TrilinearFilter";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addBool((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.TextureLayer[i].TrilinearFilter);
		prefix = "AnisotropicFilter";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addInt((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.TextureLayer[i].AnisotropicFilter);
		prefix = "TextureWrapU";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addEnum((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.TextureLayer[i].TextureWrapU, aTextureClampNames);
		prefix = "TextureWrapV";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addEnum((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.TextureLayer[i].TextureWrapV, aTextureClampNames);
		prefix = "LODBias";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addInt((prefix + StringUtil::uint32ToString(i + 1)).c_str(), material.TextureLayer[i].LODBias);

		return attr;
	}


	//! ������������SMaterial�ṹ
	void CNullDriver::fillMaterialStructureFromAttributes(SMaterial& outMaterial, IAttributes* attr)
	{
		outMaterial.MaterialType = EMT_SOLID;

		String name = attr->getAttributeAsString("Type");

		UINT32 i;

		for (i = 0; i < MaterialRenderers.size(); ++i)
			if (name == MaterialRenderers[i].Name)
			{
				outMaterial.MaterialType = (E_MATERIAL_TYPE)i;
				break;
			}

		outMaterial.AmbientColor = attr->getAttributeAsColor("Ambient");
		outMaterial.DiffuseColor = attr->getAttributeAsColor("Diffuse");
		outMaterial.EmissiveColor = attr->getAttributeAsColor("Emissive");
		outMaterial.SpecularColor = attr->getAttributeAsColor("Specular");

		outMaterial.Shininess = attr->getAttributeAsFloat("Shininess");
		outMaterial.MaterialTypeParam = attr->getAttributeAsFloat("Param1");
		outMaterial.MaterialTypeParam2 = attr->getAttributeAsFloat("Param2");

		String prefix = "Texture";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			outMaterial.setTexture(i, attr->getAttributeAsTexture((prefix + StringUtil::uint32ToString(i + 1)).c_str()));

		outMaterial.Wireframe = attr->getAttributeAsBool("Wireframe");
		outMaterial.GouraudShading = attr->getAttributeAsBool("GouraudShading");
		outMaterial.Lighting = attr->getAttributeAsBool("Lighting");
		outMaterial.ZWriteEnable = attr->getAttributeAsBool("ZWriteEnable");
		outMaterial.ZBuffer = (UINT8)attr->getAttributeAsInt("ZBuffer");
		outMaterial.BackfaceCulling = attr->getAttributeAsBool("BackfaceCulling");
		outMaterial.FrontfaceCulling = attr->getAttributeAsBool("FrontfaceCulling");
		outMaterial.FogEnable = attr->getAttributeAsBool("FogEnable");
		outMaterial.NormalizeNormals = attr->getAttributeAsBool("NormalizeNormals");
		if (attr->existsAttribute("UseMipMaps")) // legacy
			outMaterial.UseMipMaps = attr->getAttributeAsBool("UseMipMaps");
		else
			outMaterial.UseMipMaps = true;

		// default 0 is ok
		outMaterial.AntiAliasing = attr->getAttributeAsInt("AntiAliasing");
		if (attr->existsAttribute("ColorMask"))
			outMaterial.ColorMask = attr->getAttributeAsInt("ColorMask");
		if (attr->existsAttribute("ColorMaterial"))
			outMaterial.ColorMaterial = attr->getAttributeAsInt("ColorMaterial");
		if (attr->existsAttribute("PolygonOffsetFactor"))
			outMaterial.PolygonOffsetFactor = attr->getAttributeAsInt("PolygonOffsetFactor");
		if (attr->existsAttribute("PolygonOffsetDirection"))
			outMaterial.PolygonOffsetDirection = (E_POLYGON_OFFSET)attr->getAttributeAsEnumeration("PolygonOffsetDirection", PolygonOffsetDirectionNames);
		prefix = "BilinearFilter";
		if (attr->existsAttribute(prefix.c_str())) // legacy
			outMaterial.setFlag(EMF_BILINEAR_FILTER, attr->getAttributeAsBool(prefix.c_str()));
		else
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				outMaterial.TextureLayer[i].BilinearFilter = attr->getAttributeAsBool((prefix + StringUtil::uint32ToString(i + 1)).c_str());

		prefix = "TrilinearFilter";
		if (attr->existsAttribute(prefix.c_str())) // legacy
			outMaterial.setFlag(EMF_TRILINEAR_FILTER, attr->getAttributeAsBool(prefix.c_str()));
		else
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				outMaterial.TextureLayer[i].TrilinearFilter = attr->getAttributeAsBool((prefix + StringUtil::uint32ToString(i + 1)).c_str());

		prefix = "AnisotropicFilter";
		if (attr->existsAttribute(prefix.c_str())) // legacy
			outMaterial.setFlag(EMF_ANISOTROPIC_FILTER, attr->getAttributeAsBool(prefix.c_str()));
		else
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				outMaterial.TextureLayer[i].AnisotropicFilter = attr->getAttributeAsInt((prefix + StringUtil::uint32ToString(i + 1)).c_str());

		prefix = "TextureWrap";
		if (attr->existsAttribute(prefix.c_str())) // legacy
		{
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + StringUtil::uint32ToString(i + 1)).c_str(), aTextureClampNames);
				outMaterial.TextureLayer[i].TextureWrapV = outMaterial.TextureLayer[i].TextureWrapU;
			}
		}
		else
		{
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "U" + StringUtil::uint32ToString(i + 1)).c_str(), aTextureClampNames);
				outMaterial.TextureLayer[i].TextureWrapV = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "V" + StringUtil::uint32ToString(i + 1)).c_str(), aTextureClampNames);
			}
		}

		// default 0 is ok
		prefix = "LODBias";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			outMaterial.TextureLayer[i].LODBias = attr->getAttributeAsInt((prefix + StringUtil::uint32ToString(i + 1)).c_str());
	}


	//! ���ز���ϵͳ����������IVideoDriver������
	const SExposedVideoData& CNullDriver::getExposedVideoData()
	{
		return ExposedData;
	}


	//! ������Ƶ����������
	E_DRIVER_TYPE CNullDriver::getDriverType() const
	{
		return EDT_NULL;
	}


	//! ɾ�����в�����Ⱦ��
	void CNullDriver::deleteMaterialRenders()
	{
		// ɾ��������Ⱦ��
		for (UINT32 i = 0; i<MaterialRenderers.size(); ++i)
			if (MaterialRenderers[i].Renderer)
				MaterialRenderers[i].Renderer->drop();

		MaterialRenderers.clear();
	}


	//! ���ز�����Ⱦ����ָ���NULL
	IMaterialRenderer* CNullDriver::getMaterialRenderer(UINT32 idx)
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Renderer;
		else
			return 0;
	}


	//! ���ص�ǰ��Ч�Ĳ�����Ⱦ��
	UINT32 CNullDriver::getMaterialRendererCount() const
	{
		return MaterialRenderers.size();
	}


	//! ���ز�����Ⱦ��������
	const char* CNullDriver::getMaterialRendererName(UINT32 idx) const
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Name.c_str();

		return 0;
	}


	//! ����IGPUProgrammingServices�ӿڵ�ָ��
	IGPUProgrammingServices* CNullDriver::getGPUProgrammingServices()
	{
		return this;
	}


	//! ���һ���µĲ���ѡ����������������һ��HLSL
	SINT32 CNullDriver::addHighLevelShaderMaterial(
		const c8* vertexShaderProgram,
		const c8* vertexShaderEntryPointName,
		E_VERTEX_SHADER_TYPE vsCompileTarget,
		const c8* pixelShaderProgram,
		const c8* pixelShaderEntryPointName,
		E_PIXEL_SHADER_TYPE psCompileTarget,
		const c8* geometryShaderProgram,
		const c8* geometryShaderEntryPointName,
		E_GEOMETRY_SHADER_TYPE gsCompileTarget,
		E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
		UINT32 verticesOut,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData, E_GPU_SHADING_LANGUAGE shadingLang)
	{
		Printer::log("High level shader materials not available (yet) in this driver, sorry");
		return -1;
	}


	//! ����IGPUProgrammingServices::addShaderMaterial()  
	//! �����Դ��ļ�����س���
	SINT32 CNullDriver::addHighLevelShaderMaterialFromFiles(
		const path& vertexShaderProgramFileName,
		const c8* vertexShaderEntryPointName,
		E_VERTEX_SHADER_TYPE vsCompileTarget,
		const path& pixelShaderProgramFileName,
		const c8* pixelShaderEntryPointName,
		E_PIXEL_SHADER_TYPE psCompileTarget,
		const path& geometryShaderProgramFileName,
		const c8* geometryShaderEntryPointName,
		E_GEOMETRY_SHADER_TYPE gsCompileTarget,
		E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
		UINT32 verticesOut,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData, E_GPU_SHADING_LANGUAGE shadingLang)
	{
		IReadFile* vsfile = 0;
		IReadFile* psfile = 0;
		IReadFile* gsfile = 0;

		if (vertexShaderProgramFileName.size())
		{
			vsfile = FileSystem->createAndOpenFile(vertexShaderProgramFileName);
			if (!vsfile)
			{
				Printer::log("Could not open vertex shader program file",
					vertexShaderProgramFileName, LML_WARNING);
			}
		}

		if (pixelShaderProgramFileName.size())
		{
			psfile = FileSystem->createAndOpenFile(pixelShaderProgramFileName);
			if (!psfile)
			{
				Printer::log("Could not open pixel shader program file",
					pixelShaderProgramFileName, LML_WARNING);
			}
		}

		if (geometryShaderProgramFileName.size())
		{
			gsfile = FileSystem->createAndOpenFile(geometryShaderProgramFileName);
			if (!gsfile)
			{
				Printer::log("Could not open geometry shader program file",
					geometryShaderProgramFileName, LML_WARNING);
			}
		}

		SINT32 result = addHighLevelShaderMaterialFromFiles(
			vsfile, vertexShaderEntryPointName, vsCompileTarget,
			psfile, pixelShaderEntryPointName, psCompileTarget,
			gsfile, geometryShaderEntryPointName, gsCompileTarget,
			inType, outType, verticesOut,
			callback, baseMaterial, userData, shadingLang);

		if (psfile)
			psfile->drop();

		if (vsfile)
			vsfile->drop();

		if (gsfile)
			gsfile->drop();

		return result;
	}


	//! ����IGPUProgrammingServices::addShaderMaterial()  
	//! �����Դ��ļ�����س���
	SINT32 CNullDriver::addHighLevelShaderMaterialFromFiles(
		IReadFile* vertexShaderProgram,
		const c8* vertexShaderEntryPointName,
		E_VERTEX_SHADER_TYPE vsCompileTarget,
		IReadFile* pixelShaderProgram,
		const c8* pixelShaderEntryPointName,
		E_PIXEL_SHADER_TYPE psCompileTarget,
		IReadFile* geometryShaderProgram,
		const c8* geometryShaderEntryPointName,
		E_GEOMETRY_SHADER_TYPE gsCompileTarget,
		E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
		UINT32 verticesOut,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData, E_GPU_SHADING_LANGUAGE shadingLang)
	{
		c8* vs = 0;
		c8* ps = 0;
		c8* gs = 0;

		if (vertexShaderProgram)
		{
			const long size = vertexShaderProgram->getSize();
			if (size)
			{
				vs = new c8[size + 1];
				vertexShaderProgram->read(vs, size);
				vs[size] = 0;
			}
		}

		if (pixelShaderProgram)
		{
			const long size = pixelShaderProgram->getSize();
			if (size)
			{
				// if both handles are the same we must reset the file
				if (pixelShaderProgram == vertexShaderProgram)
					pixelShaderProgram->seek(0);
				ps = new c8[size + 1];
				pixelShaderProgram->read(ps, size);
				ps[size] = 0;
			}
		}

		if (geometryShaderProgram)
		{
			const long size = geometryShaderProgram->getSize();
			if (size)
			{
				// if both handles are the same we must reset the file
				if ((geometryShaderProgram == vertexShaderProgram) ||
					(geometryShaderProgram == pixelShaderProgram))
					geometryShaderProgram->seek(0);
				gs = new c8[size + 1];
				geometryShaderProgram->read(gs, size);
				gs[size] = 0;
			}
		}

		SINT32 result = this->addHighLevelShaderMaterial(
			vs, vertexShaderEntryPointName, vsCompileTarget,
			ps, pixelShaderEntryPointName, psCompileTarget,
			gs, geometryShaderEntryPointName, gsCompileTarget,
			inType, outType, verticesOut,
			callback, baseMaterial, userData, shadingLang);

		delete[] vs;
		delete[] ps;
		delete[] gs;

		return result;
	}


	//! ���һ���²�����Ⱦ������������pixel ��/�� vertex shader ��Ⱦ������
	SINT32 CNullDriver::addShaderMaterial(const c8* vertexShaderProgram,
		const c8* pixelShaderProgram,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData)
	{
		Printer::log("Shader materials not implemented yet in this driver, sorry.");
		return -1;
	}


	//! ����IGPUProgrammingServices::addShaderMaterial(), ���ǳ��Լ��ش��ļ����س���
	SINT32 CNullDriver::addShaderMaterialFromFiles(IReadFile* vertexShaderProgram,
		IReadFile* pixelShaderProgram,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData)
	{
		c8* vs = 0;
		c8* ps = 0;

		if (vertexShaderProgram)
		{
			const long size = vertexShaderProgram->getSize();
			if (size)
			{
				vs = new c8[size + 1];
				vertexShaderProgram->read(vs, size);
				vs[size] = 0;
			}
		}

		if (pixelShaderProgram)
		{
			const long size = pixelShaderProgram->getSize();
			if (size)
			{
				ps = new c8[size + 1];
				pixelShaderProgram->read(ps, size);
				ps[size] = 0;
			}
		}

		SINT32 result = addShaderMaterial(vs, ps, callback, baseMaterial, userData);

		delete[] vs;
		delete[] ps;

		return result;
	}


	//! ����IGPUProgrammingServices::addShaderMaterial()���ǳ��Լ��ش��ļ����س���
	SINT32 CNullDriver::addShaderMaterialFromFiles(const path& vertexShaderProgramFileName,
		const path& pixelShaderProgramFileName,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData)
	{
		IReadFile* vsfile = 0;
		IReadFile* psfile = 0;

		if (vertexShaderProgramFileName.size())
		{
			vsfile = FileSystem->createAndOpenFile(vertexShaderProgramFileName);
			if (!vsfile)
			{
				Printer::log("Could not open vertex shader program file",
					vertexShaderProgramFileName, LML_WARNING);
				return -1;
			}
		}

		if (pixelShaderProgramFileName.size())
		{
			psfile = FileSystem->createAndOpenFile(pixelShaderProgramFileName);
			if (!psfile)
			{
				Printer::log("Could not open pixel shader program file",
					pixelShaderProgramFileName, LML_WARNING);
				if (vsfile)
					vsfile->drop();
				return -1;
			}
		}

		SINT32 result = addShaderMaterialFromFiles(vsfile, psfile, callback,
			baseMaterial, userData);

		if (psfile)
			psfile->drop();

		if (vsfile)
			vsfile->drop();

		return result;
	}


	//! ����һ����ȾĿ������
	ITexture* CNullDriver::addRenderTargetTexture(const dimension2d<UINT32>& size,
		const path&name, const ECOLOR_FORMAT format)
	{
		return 0;
	}


	//! ���ZBuffer.
	void CNullDriver::clearZBuffer()
	{
	}


	//! ����һ��ָ��mesh manipulator.
	IMeshManipulator* CNullDriver::getMeshManipulator()
	{
		return MeshManipulator;
	}


	//! ������һ֡������һ��ͼ��
	IImage* CNullDriver::createScreenShot(ECOLOR_FORMAT format, E_RENDER_TARGET target)
	{
		return 0;
	}


	// ��ӡ��Ⱦ���汾
	void CNullDriver::printVersion()
	{
		StringW namePrint = L"Using renderer: ";
		namePrint += getName();
		Printer::log(StringUtil::StringWToString(namePrint).c_str(), LML_NORMAL); 
	}


	//! ����һ����Ƶ����
	IVideoDriver* createNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize)
	{
		CNullDriver* nullDriver = new CNullDriver(io, screenSize);

		// �����ղ�����Ⱦ��
		for (UINT32 i = 0; sBuiltInMaterialTypeNames[i]; ++i)
		{
			IMaterialRenderer* imr = new IMaterialRenderer();
			nullDriver->addMaterialRenderer(imr);
			imr->drop();
		}

		return nullDriver;
	}


	//! ����/ȡ������ һ������ƽ��
	//! ������6������ƽ���û���������
	//! \param index: ƽ���������������0��MaxUserClipPlanes.
	//! \param plane: ƽ������
	//! \param enable: ���Ϊtrue���򿪼���ƽ�棬����ر���
	bool CNullDriver::setClipPlane(UINT32 index, const Plane& plane, bool enable)
	{
		return false;
	}


	//! ��/�ر�һ������ƽ��
	//! ������6������ƽ���û���������
	//! \param plane: ƽ������
	//! \param enable: ���Ϊtrue���򿪼���ƽ�棬����ر���
	void CNullDriver::enableClipPlane(UINT32 index, bool enable)
	{
		//  
	}


	ITexture* CNullDriver::createRenderTargetTexture(const dimension2d<UINT32>& size,
		const c8* name)
	{
		Printer::log("createRenderTargetTexture is deprecated, use addRenderTargetTexture instead");
		ITexture* tex = addRenderTargetTexture(size, name);
		tex->grab();
		return tex;
	}


	void CNullDriver::setMinHardwareBufferVertexCount(UINT32 count)
	{
		MinVertexCountForVBO = count;
	}


	SOverrideMaterial& CNullDriver::getOverrideMaterial()
	{
		return OverrideMaterial;
	}


	//! Ϊ���޸�����ֵ��ȡ2d���ǲ���
	SMaterial& CNullDriver::getMaterial2D()
	{
		return OverrideMaterial2D;
	}


	//! ��2d���ǲ���
	void CNullDriver::enableMaterial2D(bool enable)
	{
		OverrideMaterial2DEnabled = enable;
	}


	dimension2du CNullDriver::getMaxTextureSize() const
	{
		return dimension2du(0x10000, 0x10000); // �����㹻��
	}


	//! ��ɫת������
		/** ��һ��ͼ��һ���������飩ת����Ŀ�ĸ�ʽ��ͼ���������飩.
		���ش�С����ɫ��ʽ����
		\param sP Դͼ��ָ��
		\param sF Դͼ����ɫ��ʽ
		\param sN ת�������ظ�ʽ�������������һ����С
		\param dP Ŀ��ͼ������
		\param dF Ŀ��ͼ���ʽ
		*/
	void CNullDriver::convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
		void* dP, ECOLOR_FORMAT dF) const
	{
		CColorConverter::convert_viaFormat(sP, sF, sN, dP, dF);
	}

}