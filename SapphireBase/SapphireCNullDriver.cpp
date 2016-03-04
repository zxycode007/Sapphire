
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
	//! 创建一个可以加载BMP的加载器
	IImageLoader* createImageLoaderBMP();

	//! 创建一个可以加载JPG的加载器 
	IImageLoader* createImageLoaderJPG();

	//! 创建一个可以加载TGA的加载器 
	IImageLoader* createImageLoaderTGA();

	//! 创建一个可以加载PSD的加载器 
	IImageLoader* createImageLoaderPSD();

	//! 创建一个可以加载DDS的加载器
	IImageLoader* createImageLoaderDDS();

	//! 创建一个可以加载PCX的加载器 
	IImageLoader* createImageLoaderPCX();

	//! 创建一个可以加载PNG的加载器
	IImageLoader* createImageLoaderPNG();

	//! 创建一个可以加载WAL的加载器
	IImageLoader* createImageLoaderWAL();

	//! 创建一个加载HalfLife的图像的加载器
	IImageLoader* createImageLoaderHalfLife();

	//! 创建一个可以加载LMP图像的加载器
	IImageLoader* createImageLoaderLMP();

	//! 创建一个可以加载ppm/pgm/pbm图像的加载器
	IImageLoader* createImageLoaderPPM();

	//! 创建一个RGB图像的加载器
	IImageLoader* createImageLoaderRGB();


	//! 创建一个可以保存BMP图像的写入器
	IImageWriter* createImageWriterBMP();

	//! 创建一个可以保存JPG图像的写入器
	IImageWriter* createImageWriterJPG();

	//! 创建一个可以保存TGA图像的写入器
	IImageWriter* createImageWriterTGA();

	//! 创建一个可以保存PSD图像的写入器
	IImageWriter* createImageWriterPSD();

	//! 创建一个可以保存PCX图像的写入器
	IImageWriter* createImageWriterPCX();

	//! 创建一个可以保存PNG图像的写入器
	IImageWriter* createImageWriterPNG();

	//! 创建一个可以保存PPM图像的写入器
	IImageWriter* createImageWriterPPM();

	//! CNULLDRIVER构造器
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

		// 创建 manipulator
		MeshManipulator = new CMeshManipulator();

		if (FileSystem)
			FileSystem->grab();

		// 创建 surface loader

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


		// 用0填充ExposedData 
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

		// 删除材质渲染器
		deleteMaterialRenders();

		// 删除所有硬件网格缓冲区
		removeAllHardwareBuffers();
	}


	//! 添加一个扩展的surface loader 到引擎
	void CNullDriver::addExternalImageLoader(IImageLoader* loader)
	{
		if (!loader)
			return;

		loader->grab();
		SurfaceLoader.push_back(loader);
	}


	//! 添加一个扩展surface writer到引擎
	void CNullDriver::addExternalImageWriter(IImageWriter* writer)
	{
		if (!writer)
			return;

		writer->grab();
		SurfaceWriter.push_back(writer);
	}


	//! 返回图像加载器的数量
	UINT32 CNullDriver::getImageLoaderCount() const
	{
		return SurfaceLoader.size();
	}


	//! 返回给定的图像加载器
	IImageLoader* CNullDriver::getImageLoader(UINT32 n)
	{
		if (n < SurfaceLoader.size())
			return SurfaceLoader[n];
		return 0;
	}


	//! 返回图像写入器的数量
	UINT32 CNullDriver::getImageWriterCount() const
	{
		return SurfaceWriter.size();
	}


	//! 返回给定图像写入器
	IImageWriter* CNullDriver::getImageWriter(UINT32 n)
	{
		if (n < SurfaceWriter.size())
			return SurfaceWriter[n];
		return 0;
	}


	//! 设置一个材质
	void CNullDriver::deleteAllTextures()
	{
		// 需要一次之前设置的纹理，要不然它们会保存之前设置材质成员。可以优化减少状态改变
		setMaterial(SMaterial());

		for (UINT32 i = 0; i<Textures.size(); ++i)
			Textures[i].Surface->drop();

		Textures.clear();
	}



	//! 应用程序必须在任何渲染开始之前调用这个方法，返回false则表示失败
	bool CNullDriver::beginScene(bool backBuffer, bool zBuffer, ColourValue color,
		const SExposedVideoData& videoData, rect<SINT32>* sourceRect)
	{
		clearFPUException();
		PrimitivesDrawn = 0;
		return true;
	}


	//! //结束渲染场景  应用程序必须在所有渲染方法结束后调用， 返回false表示失败
	bool CNullDriver::endScene()
	{
		FPSCounter.registerFrame(Timer::getRealTime(), PrimitivesDrawn);
		updateAllHardwareBuffers();
		updateAllOcclusionQueries();
		return true;
	}


	//! 关闭驱动的一个特征
	void CNullDriver::disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag)
	{
		FeatureEnabled[feature] = !flag;
	}


	//! 查询驱动特征，如果特征有效，返回true
	bool CNullDriver::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
	{
		return false;
	}


	//! 获取实际视频驱动的属性
	const IAttributes& CNullDriver::getDriverAttributes() const
	{
		return *DriverAttributes;
	}


	//! 设置变换  （交给具体D3D或OGL驱动去实现的）
	void CNullDriver::setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat)
	{
	}


	//! 返回图像加载器的数量
	const Matrix4& CNullDriver::getTransform(E_TRANSFORMATION_STATE state) const
	{
		return TransformationMatrix;
	}


	//! 设置一个材质 （交给具体D3D或OGL驱动去实现的）
	void CNullDriver::setMaterial(const SMaterial& material)
	{
	}


	//! 从纹理缓存移除一个纹理并且删除它以释放内存
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


	//! 从纹理缓存移除所有纹理并且删除它以释放内存
	void CNullDriver::removeAllTextures()
	{
		setMaterial(SMaterial());
		deleteAllTextures();
	}


	//! 返回一个纹理索引
	ITexture* CNullDriver::getTextureByIndex(UINT32 i)
	{
		if (i < Textures.size())
			return Textures[i].Surface;

		return 0;
	}


	//! 返回当前加载的纹理数量
	UINT32 CNullDriver::getTextureCount() const
	{
		return Textures.size();
	}


	//! 重命名一个纹理 
	void CNullDriver::renameTexture(ITexture* texture, const path& newName)
	{
		// 我们能够在这儿做一个安全的const_cast, 因为这些纹理在重命名后需要重新排序，这个ITexture接口的名字是只读的，以避免用户不通过调用这个方法来改变
		SNamedPath& name = const_cast<SNamedPath&>(texture->getName());
		name.setPath(newName);
		
		std::stable_sort(Textures.begin(), Textures.end());
	}


	//! 从路径文件名加载一个纹理
	ITexture* CNullDriver::getTexture(const path& filename)
	{
		// 通过绝对文件名标示纹理
		const path absolutePath = FileSystem->getAbsolutePath(filename);
		//查找纹理
		ITexture* texture = findTexture(absolutePath);
		if (texture)
			return texture;

		// 上面失败，那么尝试源文件名，它可以加入一个Archive
		texture = findTexture(filename);
		if (texture)
			return texture;

		// 上面失败，现在再尝试用完整的路径打开这个文件
		IReadFile* file = FileSystem->createAndOpenFile(absolutePath);
		//如果文件存在
		if (!file)
		{
			// 尝试用源文件名打开
			file = FileSystem->createAndOpenFile(filename);
		}

		if (file)
		{
			// 再次检查实际档案名
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
				texture->drop(); // 创建完成，以后不会用到，丢弃一个引用
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


	//! 从IReadFile加载一个纹理
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


	//! 打开纹理文件并加载它到表面
	ITexture* CNullDriver::loadTextureFromFile(IReadFile* file, const path& hashName)
	{
		ITexture* texture = 0;
		//先加载图像
		IImage* image = createImageFromFile(file);

		if (image)
		{
			// 从表面创建纹理
			texture = createDeviceDependentTexture(image, hashName.size() ? hashName : file->getFileName());
			Printer::log("Loaded texture", file->getFileName());
			image->drop();
		}

		return texture;
	}


	//! 添加一个表面， 不加载或通过引擎创建
	void CNullDriver::addTexture(ITexture* texture)
	{
		if (texture)
		{
			SSurface s;
			s.Surface = texture;
			//增加一个引用
			texture->grab();

			Textures.push_back(s);

			// 新的纹理现在在纹理列表的末尾，当查询下一个新纹理，这个纹理数组将会会排序并且纹理的索引会被改变。
			

			//std::stable_sort(Textures.begin(), Textures.end(), ITextureCMP);
			std::stable_sort(Textures.begin(), Textures.end());
		}
	}


	//! 查询图像是否被加载
	ITexture* CNullDriver::findTexture(const path& filename)
	{
		SSurface s;   //一个图像表面
		SDummyTexture dummy(filename);  
		s.Surface = &dummy;

		vector<SSurface>::type::iterator it = std::find(Textures.begin(), Textures.end(), s);// Textures.binary_search(s);
		if (it != std::end(Textures))
			return it->Surface;
			//return //Textures[index].Surface;

		return 0;
	}


	//! 从一个已加载的图像创建一个纹理
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


	//! 创建一个纹理
	ITexture* CNullDriver::addTexture(const dimension2d<UINT32>& size,
		const path& name, ECOLOR_FORMAT format)
	{
		//判断图像是否是支持的格式
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create ITexture, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		if (0 == name.size())
			return 0;

		IImage* image = new CImage(format, size);
		//创建设备依赖的纹理
		ITexture* t = createDeviceDependentTexture(image, name);
		image->drop();
		addTexture(t);

		if (t)
			t->drop();

		return t;
	}



	//! 从一个软件表面IImage返回一个设备依赖的纹理
	//! 这个方法会被派生驱动自己的纹理所覆盖
	ITexture* CNullDriver::createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData)
	{
		return new SDummyTexture(name);
	}


	//! 设置或重设一个特定的渲染目标
	bool CNullDriver::setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
		bool clearZBuffer, ColourValue color)
	{
		if (ERT_FRAME_BUFFER == target)  //如果是帧缓冲的话,清除颜色缓冲区，Z缓冲
			return setRenderTarget(0, clearTarget, clearZBuffer, color);
		else
			return false;
	}


	//! 设置渲染目标（由派生设备驱动覆盖）
	bool CNullDriver::setRenderTarget(ITexture* texture, bool clearBackBuffer,
		bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! 设置多个渲染目标（由派生设备驱动覆盖）
	bool CNullDriver::setRenderTarget(const vector<IRenderTarget>::type& texture,
		bool clearBackBuffer, bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! 设置一个视口（由派生设备驱动覆盖）
	void CNullDriver::setViewPort(const rect<SINT32>& area)
	{
	}


	//! 获取当前视口区域
	const rect<SINT32>& CNullDriver::getViewPort() const
	{
		return ViewPort;
	}


	//! 绘制一个顶点图元列表（由派生设备驱动覆盖）
	void CNullDriver::drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! 绘制2d的顶点的图元列表（由派生设备驱动覆盖）
	void CNullDriver::draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! 绘制一条3d线段（由派生设备驱动覆盖）
	void CNullDriver::draw3DLine(const Vector3& start,
		const Vector3& end, ColourValue color)
	{
	}


	//! 绘制一个3d三角形
	void CNullDriver::draw3DTriangle(const triangle3df& triangle, ColourValue color)
	{
		//设置三角形顶点属性
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
		//绘制顶点图元列表
		drawVertexPrimitiveList(vertices, 3, indexList, 1, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}


	//! 绘制一条3d AABB盒子
	void CNullDriver::draw3DBox(const AxisAlignedBox& box, ColourValue color)
	{
		//Vector3 edges[8];
		//box.getEdges(edges);

		// TODO: 可优化到一个大型的drawIndexPrimitive调用
		

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



	//! 绘制一个2d图像
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos)
	{
		if (!texture)
			return;

		draw2DImage(texture, destPos, rect<SINT32>(Position2d(0, 0),
			dimension2di(texture->getOriginalSize())));
	}




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

	//! 用一个颜色和纹理的alphat通道（如果需要）绘制一个2d图像
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


	//! 在一个矩形中绘制纹理的一部分
	void CNullDriver::draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
		const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect,
		const ColourValue* const colors, bool useAlphaChannelOfTexture)
	{
		if (destRect.isValid())
			draw2DImage(texture, Position2d(destRect.UpperLeftCorner),
			sourceRect, clipRect, colors ? colors[0] : ColourValue(0xffffffff),
			useAlphaChannelOfTexture);
	}


	//! 用一个颜色和纹理的alphat通道（如果需要）绘制一个2d图像
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos,
		const rect<SINT32>& sourceRect,
		const rect<SINT32>* clipRect, ColourValue color,
		bool useAlphaChannelOfTexture)
	{
	}


	//! 绘制一个离线的2d矩形
	void CNullDriver::draw2DRectangleOutline(const recti& pos, ColourValue color)
	{
		draw2DLine(pos.UpperLeftCorner, Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), color);
		draw2DLine(Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), pos.LowerRightCorner, color);
		draw2DLine(pos.LowerRightCorner, Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), color);
		draw2DLine(Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), pos.UpperLeftCorner, color);
	}


	//!绘制一个2d矩形
	void CNullDriver::draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip)
	{
		draw2DRectangle(pos, color, color, color, color, clip);
	}



	//! 绘制一个渐变的矩形
	void CNullDriver::draw2DRectangle(const rect<SINT32>& pos,
		ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
		const rect<SINT32>* clip)
	{
	}



	//! 绘制一个2D的线段
	void CNullDriver::draw2DLine(const Position2d& start,
		const Position2d& end, ColourValue color)
	{
	}

	//! 绘制一个像素
	void CNullDriver::drawPixel(UINT32 x, UINT32 y, const ColourValue & color)
	{
	}


	//! 绘制一个非填充的共圆的正规2d多边形
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


	//! 返回颜色格式
	ECOLOR_FORMAT CNullDriver::getColorFormat() const
	{
		return ECF_R5G6B5;
	}


	//! 返回屏幕大小
	const dimension2d<UINT32>& CNullDriver::getScreenSize() const
	{
		return ScreenSize;
	}


	//! 返回了当前渲染目标大小 
	const dimension2d<UINT32>& CNullDriver::getCurrentRenderTargetSize() const
	{
		return ScreenSize;
	}


	// 获取当前FPS数
	SINT32 CNullDriver::getFPS() const
	{
		return FPSCounter.getFPS();
	}



	//! 返回在上一帧中绘制的图元数量（大多是三角形）
	UINT32 CNullDriver::getPrimitiveCountDrawn(UINT32 param) const
	{
		return (0 == param) ? FPSCounter.getPrimitive() : (1 == param) ? FPSCounter.getPrimitiveAverage() : FPSCounter.getPrimitiveTotal();
	}



	//! 设置动态环境光颜色。这个默认值是（0，0，0，0）黑色
	//! \param color: 新设置的环境光颜色
	void CNullDriver::setAmbientLight(const ColourValue& color)
	{
	}



	//! \return 返回视频驱动名: 例如DIRECT3D8驱动，返回“Direct3D8.1”
	const wchar_t* CNullDriver::getName() const
	{
		return L"Irrlicht NullDevice";
	}



	//! 绘制一个阴影容积到模板缓冲区中。要绘制一个模板阴影：首先,会所有几何体。再用这个方法，去绘制阴影容积。
	//然后，用IVideoDriver::drawStencilShadow()去可视化阴影
	void CNullDriver::drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible)
	{
	}


	//! 在用IVideoDriver::drawStencilShadowVolume()阴影容积绘制到模板缓冲区后，用颜色填充模板阴影。
	//! 这个方法绘制阴影的颜色
	void CNullDriver::drawStencilShadow(bool clearStencilBuffer,
		ColourValue leftUpEdge, ColourValue rightUpEdge,
		ColourValue leftDownEdge, ColourValue rightDownEdge)
	{
	}


	//! 删除所有动态光源
	void CNullDriver::deleteAllDynamicLights()
	{
		Lights.clear();
	}


	//! 添加一个动态光源，返回这个光源的索引
	//! \param light: 要创建光源的光源数据
	//! \return 这个光源的索引，-1是发生错误
	SINT32 CNullDriver::addDynamicLight(const SLight& light)
	{
		Lights.push_back(light);
		return Lights.size() - 1;
	}

	//! 打开或关闭一个动态光源
	//! \param lightIndex: addDynamicLight所返回的索引
	//! \param turnOn: true为打开，false为关闭
	void CNullDriver::turnLightOn(SINT32 lightIndex, bool turnOn)
	{
		// Do nothing
	}


	//! 返回这个设备能够支持的动态光源的最大数量
	UINT32 CNullDriver::getMaximalDynamicLightAmount() const
	{
		return 0;
	}


	//! 返回当前动态光源设置数量
	//! \return 当前设置的动态光源数量
	UINT32 CNullDriver::getDynamicLightCount() const
	{
		return Lights.size();
	}


	//! 返回之前通过IVideDriver::addDynamicLight()设置的光源数据
	//! \param idx: 索引从0开始，小于IVideoDriver()::getDynamicLightCount
	//! \return 光源数据SLight
	const SLight& CNullDriver::getDynamicLight(UINT32 idx) const
	{
		if (idx < Lights.size())
			return Lights[idx];
		else
			return *((SLight*)0);
	}


	//! 创建一个1位的基于一个色键位置的纹理alpha通道
	void CNullDriver::makeColorKeyTexture(ITexture* texture,
		ColourValue color,
		bool zeroTexels) const
	{
		if (!texture)
			return;
		//判断颜色格式
		if (texture->getColorFormat() != ECF_A1R5G5B5 &&
			texture->getColorFormat() != ECF_A8R8G8B8)
		{
			Printer::log("Error: Unsupported texture color format for making color key channel.", LML_ERROR);
			return;
		}
		//A1R5G5B5格式
		if (texture->getColorFormat() == ECF_A1R5G5B5)
		{
			//纹理锁定
			UINT16 *p = (UINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_ERROR);
				return;
			}
			//获取纹理的大小
			const dimension2d<UINT32> dim = texture->getSize();
			//获取纹理的行跨距(每行字节数)（因为每像素占16位，所以行跨距/2）
			const UINT32 pitch = texture->getPitch() / 2;

			// 用alpha关闭的颜色 （颜色和0x7fff相与，等于提取R5G5B5部分）
			const UINT16 refZeroAlpha = (0x7fff & A8R8G8B8toA1R5G5B5(color.getAsARGB()));
			//计算最后一个像素位置
			const UINT32 pixels = pitch * dim.Height;

			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// 如果匹配这个颜色，无效alpha值
				// 设置这个alpha值为0
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
			//纹理大小
			dimension2d<UINT32> dim = texture->getSize();
			//获取纹理的行跨距(每行字节数)（因为每像素占16位，所以行跨距/4）
			UINT32 pitch = texture->getPitch() / 4;

			// 用alpha关闭的颜色 （颜色和0x7fff相与，等于提取R5G5B5部分）
			const UINT32 refZeroAlpha = 0x00ffffff & A8R8G8B8toA1R5G5B5(color.getAsARGB());
			//计算最后一个像素位置
			const UINT32 pixels = pitch * dim.Height;
			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// 如果匹配这个颜色，无效alpha值
				// 设置这个alpha值为0
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
		texture->regenerateMipMapLevels(); //重建MIPMAP
	}



	//! 创建一个1位的基于一个色键位置的纹理alpha通道
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



	//! 高程纹理图创建一个法线贴图
	//! \param amplitude: 高度信息的增幅常量值
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
		//获取纹理大小
		dimension2d<UINT32> dim = texture->getSize();
		amplitude = amplitude / 255.0f;
		FLOAT32 vh = dim.Height / (FLOAT32)dim.Width;  //横纵比
		FLOAT32 hh = dim.Width / (FLOAT32)dim.Height;  //纵横比

		if (texture->getColorFormat() == ECF_A8R8G8B8)
		{
			// ECF_A8R8G8B8 版本

			SINT32 *p = (SINT32*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			// 复制纹理
			UINT32 pitch = texture->getPitch() / 4;
			SINT32* in = new SINT32[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 4);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: 这个可以通过优化

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
					n += Vector3(0.5f, 0.5f, 0.5f); // 0到1之间
					n *= 255.0f;

					SINT32 height = (SINT32)nml32(x, y, pitch, dim.Height, in);
					p[y*pitch + x] = ColourValue(
						height, // 在alpha中保存高度信息
						(SINT32)n.x, (SINT32)n.z, (SINT32)n.y).getAsARGB();
				}

			delete[] in;
			texture->unlock();
		}
		else
		{
			// ECF_A1R5G5B5 版

			SINT16 *p = (SINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			UINT32 pitch = texture->getPitch() / 2;

			// 复制纹理

			SINT16* in = new SINT16[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 2);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: 由很多课优化

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

	//! 返回设备所能够用drawIndexedTriangleList渲染的图元的最大数量 
	UINT32 CNullDriver::getMaximalPrimitiveCount() const
	{
		return 0xFFFFFFFF;
	}


	//! 剪裁三角形数量，如果所谓输出警告
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

	//! 打开或关闭一个纹理创建标准
	void CNullDriver::setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled)
	{
		if (enabled && ((flag == ETCF_ALWAYS_16_BIT) || (flag == ETCF_ALWAYS_32_BIT)
			|| (flag == ETCF_OPTIMIZED_FOR_QUALITY) || (flag == ETCF_OPTIMIZED_FOR_SPEED)))
		{
			// 关闭索引格式
			setTextureCreationFlag(ETCF_ALWAYS_16_BIT, false);
			setTextureCreationFlag(ETCF_ALWAYS_32_BIT, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED, false);
		}

		// 设置标志
		TextureCreationFlags = (TextureCreationFlags & (~flag)) |
			((((UINT32)!enabled) - 1) & flag);
	}


	//! 返回一个纹理创建标准是打开或关闭
	bool CNullDriver::getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const
	{
		return (TextureCreationFlags & flag) != 0;
	}


	//! 从文件创建一个软件图像
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


	//! 从文件创建一个软件图像
	IImage* CNullDriver::createImageFromFile(IReadFile* file)
	{
		if (!file)
			return 0;

		IImage* image = 0;

		SINT32 i;

		// 尝试基于文件扩展名来加载文件
		for (i = SurfaceLoader.size() - 1; i >= 0; --i)
		{
			if (SurfaceLoader[i]->isALoadableFileExtension(file->getFileName()))
			{
				// 重设文件位置，它可能被前一次图像加载改变
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


	//! 写入提供的图像到磁盘文件 
	bool CNullDriver::writeImageToFile(IImage* image, const path& filename, UINT32 param)
	{
		IWriteFile* file = FileSystem->createAndWriteFile(filename);
		if (!file)
			return false;

		bool result = writeImageToFile(image, file, param);
		file->drop();

		return result;
	}

	//! 写入提供的图像到磁盘文件 
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


	//! 从一个字节数组创建一个软件图像
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


	//! 创建一个空软件图像
	IImage* CNullDriver::createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size)
	{
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create IImage, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		return new CImage(format, size);
	}


	//! 从另外的图像创建一个软件图像
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


	//! 从另外一个图像的一部分来创建一个软件图像
	IImage* CNullDriver::createImage(IImage* imageToCopy, const Position2d& pos, const dimension2d<UINT32>& size)
	{
		Printer::log("Deprecated method, please create an empty image instead and use copyTo().", LML_WARNING);
		CImage* tmp = new CImage(imageToCopy->getColorFormat(), imageToCopy->getDimension());
		imageToCopy->copyTo(tmp, Position2d(0, 0), recti(pos, size));
		return tmp;
	}


	//!  从另外一个纹理的一部分来创建一个软件图像
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
			// 确保缓冲区不会溢出
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


	//! 设置雾化模式
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

	//! 获取雾化模式
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

	//! 绘制一个网格缓冲区
	void CNullDriver::drawMeshBuffer(const IMeshBuffer* mb)
	{
		if (!mb)
			return;

		//IVertexBuffer 和 IIndexBuffer 在后面
		SHWBufferLink *HWBuffer = getBufferLink(mb);

		if (HWBuffer)
			drawHardwareBuffer(HWBuffer);
		else
			drawVertexPrimitiveList(mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount() / 3, mb->getVertexType(), EPT_TRIANGLES, mb->getIndexType());
	}


	//! 绘制网格缓冲的法线
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

	//! 从一个网格缓冲区获取硬件缓冲区链接(用于创建或更新缓冲区)
	CNullDriver::SHWBufferLink *CNullDriver::getBufferLink(const IMeshBuffer* mb)
	{
		if (!mb || !isHardwareBufferRecommend(mb))
			return 0;

		//搜寻硬件连接
		//map< const IMeshBuffer*, SHWBufferLink* >::Node* node = HWBufferMap.find(mb);
		map< const IMeshBuffer*, SHWBufferLink* >::type::iterator node = HWBufferMap.find(mb);
		if (node != std::end(HWBufferMap))//if (node)
			return node->second;

		return createHardwareBuffer(mb); //没有硬件连接，如果网格需要一个，创建它
	}


	//! 更新索引网格缓冲区，移除不用的
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

				// todo: 需要做更好的修正
				it = HWBufferMap.begin();
			}
		}
	}

	//删除硬件缓冲区
	void CNullDriver::deleteHardwareBuffer(SHWBufferLink *HWBuffer)
	{
		if (!HWBuffer)
			return;
		//HWBufferMap.remove(HWBuffer->MeshBuffer);
		HWBufferMap.erase(HWBuffer->MeshBuffer);
		delete HWBuffer;
	}


	//! 移除硬件缓冲区
	void CNullDriver::removeHardwareBuffer(const IMeshBuffer* mb)
	{
		map<const IMeshBuffer*, SHWBufferLink*>::type::iterator Node = HWBufferMap.find(mb);
		if (Node != std::end(HWBufferMap))
			deleteHardwareBuffer(Node->second);
	}


	//! 移除所有硬件缓冲区
	void CNullDriver::removeAllHardwareBuffers()
	{
		while (HWBufferMap.size())
		{
			deleteHardwareBuffer((HWBufferMap.begin()->second));
		}
			
	}

	//这个网格是否推荐使用VBO
	bool CNullDriver::isHardwareBufferRecommend(const IMeshBuffer* mb)
	{
		if (!mb || (mb->getHardwareMappingHint_Index() == EHM_NEVER && mb->getHardwareMappingHint_Vertex() == EHM_NEVER))
			return false;

		if (mb->getVertexCount()<MinVertexCountForVBO)
			return false;

		return true;
	}


	//! 创建遮蔽查询occlusion query.
	/**用来确认和遮蔽测试的节点 */
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

		//对query进行查询
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
		{   //没查到
			OcclusionQueries.push_back(SOccQuery(node, mesh));
			node->setAutomaticCulling(node->getAutomaticCulling() | EAC_OCC_QUERY);
		}
	}


	//! 移除遮蔽查询occlusion query.
	void CNullDriver::removeOcclusionQuery(ISceneNode* node)
	{
		//搜索query
		//SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		vector<SOccQuery>::type::iterator index = std::find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
		if (index != std::end(OcclusionQueries))
		{
			node->setAutomaticCulling(node->getAutomaticCulling() & ~EAC_OCC_QUERY);
			OcclusionQueries.erase(index);
		}
	}


	//! 移除所有的遮蔽查询
	void CNullDriver::removeAllOcclusionQueries()
	{
		for (SINT32 i = OcclusionQueries.size() - 1; i >= 0; --i)
		{
			removeOcclusionQuery(OcclusionQueries[i].Node);
		}
	}


	//! 允许遮蔽查询。绘制存放在查询中的网格
	/** 如果网格可见，用标准开启合适的材质的设置 */
	void CNullDriver::runOcclusionQuery(ISceneNode* node, bool visible)
	{
		if (!node)
			return;
		//SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		//在OcclusionQueries中查询节点
		vector<SOccQuery>::type::iterator index = std::find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
		if (index == std::end(OcclusionQueries))  //没找到
			return;
		//OcclusionQueries[index].Run = 0;
		index->Run = 0;
		if (!visible) //如果不可见
		{
			SMaterial mat;
			mat.Lighting = false;
			mat.AntiAliasing = 0;
			mat.ColorMask = ECP_NONE;
			mat.GouraudShading = false;
			mat.ZWriteEnable = false;
			setMaterial(mat);
		}
		//设置变换矩阵
		setTransform(ETS_WORLD, node->getAbsoluteTransformation());
		const IMesh* mesh = index->Mesh;
		for (UINT32 i = 0; i<mesh->getMeshBufferCount(); ++i) //循环变量这个网格包含的所有网格缓冲区
		{
			if (visible)  //如果可见的话，绘制这个网格缓冲区
				setMaterial(mesh->getMeshBuffer(i)->getMaterial());
			drawMeshBuffer(mesh->getMeshBuffer(i));
		}
	}


	//! 允许所有的遮蔽查询。绘制保存在查询中的网格
	/** 如果网格应该不可见，用overrideMaterial关闭颜色和深度缓冲区*/
	void CNullDriver::runAllOcclusionQueries(bool visible)
	{
		for (UINT32 i = 0; i<OcclusionQueries.size(); ++i)
			runOcclusionQuery(OcclusionQueries[i].Node, visible);
	}


	//! 更新遮蔽查询。从GPU返回结果
	/** 如果查询不被阻塞，设置标志为false 。更新在这里不会发生 */
	void CNullDriver::updateOcclusionQuery(ISceneNode* node, bool block)
	{
	}


	//! 更新所有的遮蔽查询。从GPU返回结果
	/**如果查询不被阻塞，设置标志为false 。更新在这里不会发生 */
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


	//! 返回查询结果
	/** 返回值是可见的像素/片段的数量。这个值是一个安全的近似值，能够大于实际像素值
	*/
	UINT32 CNullDriver::getOcclusionQueryResult(ISceneNode* node) const
	{
		return ~0;
	}


	//! 只背引擎内部使用
	/**用于提醒驱动：窗口大小已经改变 */
	void CNullDriver::OnResize(const dimension2d<UINT32>& size)
	{
		if (ViewPort.getWidth() == (SINT32)ScreenSize.Width &&
			ViewPort.getHeight() == (SINT32)ScreenSize.Height)
			ViewPort = rect<SINT32>(Position2d(0, 0),
			dimension2di(size));

		ScreenSize = size;
	}


	// 添加一个材质渲染器并且之后丢弃它。用于内部操作
	SINT32 CNullDriver::addAndDropMaterialRenderer(IMaterialRenderer* m)
	{
		SINT32 i = addMaterialRenderer(m);

		if (m)
			m->drop();

		return i;
	}


	//! 添加一个新材质渲染器到视频设备
	SINT32 CNullDriver::addMaterialRenderer(IMaterialRenderer* renderer, const char* name)
	{
		if (!renderer)
			return -1;

		SMaterialRenderer r;
		r.Renderer = renderer;
		r.Name = name;

		if (name == 0 && (MaterialRenderers.size() < (sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1))
		{
			// 设置内置渲染器的名字，不能在所有的渲染器中都能实现名字设置
			r.Name = sBuiltInMaterialTypeNames[MaterialRenderers.size()];
		}

		MaterialRenderers.push_back(r);
		renderer->grab();

		return MaterialRenderers.size() - 1;
	}


	//! 设置材质渲染器的名字
	void CNullDriver::setMaterialRendererName(SINT32 idx, const char* name)
	{
		if (idx < SINT32(sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1 ||
			idx >= (SINT32)MaterialRenderers.size())
			return;

		MaterialRenderers[idx].Name = name;
	}


	//! 从一个材质创建材质属性列表，对序列化和其它可用
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
			//纹理使用真实路径
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


	//! 从属性填充材质SMaterial结构
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


	//! 返回操作系统和驱动关于IVideoDriver的数据
	const SExposedVideoData& CNullDriver::getExposedVideoData()
	{
		return ExposedData;
	}


	//! 返回视频驱动的类型
	E_DRIVER_TYPE CNullDriver::getDriverType() const
	{
		return EDT_NULL;
	}


	//! 删除所有材质渲染器
	void CNullDriver::deleteMaterialRenders()
	{
		// 删除材质渲染器
		for (UINT32 i = 0; i<MaterialRenderers.size(); ++i)
			if (MaterialRenderers[i].Renderer)
				MaterialRenderers[i].Renderer->drop();

		MaterialRenderers.clear();
	}


	//! 返回材质渲染器的指针或NULL
	IMaterialRenderer* CNullDriver::getMaterialRenderer(UINT32 idx)
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Renderer;
		else
			return 0;
	}


	//! 返回当前有效的材质渲染器
	UINT32 CNullDriver::getMaterialRendererCount() const
	{
		return MaterialRenderers.size();
	}


	//! 返回材质渲染器的名字
	const char* CNullDriver::getMaterialRendererName(UINT32 idx) const
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Name.c_str();

		return 0;
	}


	//! 返回IGPUProgrammingServices接口的指针
	IGPUProgrammingServices* CNullDriver::getGPUProgrammingServices()
	{
		return this;
	}


	//! 添加一个新的材质选房区到驱动，基于一个HLSL
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


	//! 类似IGPUProgrammingServices::addShaderMaterial()  
	//! 但尝试从文件里加载程序
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


	//! 类似IGPUProgrammingServices::addShaderMaterial()  
	//! 但尝试从文件里加载程序
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


	//! 添加一个新材质渲染器到驱动，用pixel 与/或 vertex shader 渲染几何体
	SINT32 CNullDriver::addShaderMaterial(const c8* vertexShaderProgram,
		const c8* pixelShaderProgram,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData)
	{
		Printer::log("Shader materials not implemented yet in this driver, sorry.");
		return -1;
	}


	//! 类似IGPUProgrammingServices::addShaderMaterial(), 但是尝试加载从文件加载程序
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


	//! 类似IGPUProgrammingServices::addShaderMaterial()但是尝试加载从文件加载程序
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


	//! 创建一个渲染目标纹理
	ITexture* CNullDriver::addRenderTargetTexture(const dimension2d<UINT32>& size,
		const path&name, const ECOLOR_FORMAT format)
	{
		return 0;
	}


	//! 清空ZBuffer.
	void CNullDriver::clearZBuffer()
	{
	}


	//! 返回一个指向mesh manipulator.
	IMeshManipulator* CNullDriver::getMeshManipulator()
	{
		return MeshManipulator;
	}


	//! 返回上一帧创建的一个图像
	IImage* CNullDriver::createScreenShot(ECOLOR_FORMAT format, E_RENDER_TARGET target)
	{
		return 0;
	}


	// 打印渲染器版本
	void CNullDriver::printVersion()
	{
		StringW namePrint = L"Using renderer: ";
		namePrint += getName();
		Printer::log(StringUtil::StringWToString(namePrint).c_str(), LML_NORMAL); 
	}


	//! 创建一个视频驱动
	IVideoDriver* createNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize)
	{
		CNullDriver* nullDriver = new CNullDriver(io, screenSize);

		// 创建空材质渲染器
		for (UINT32 i = 0; sBuiltInMaterialTypeNames[i]; ++i)
		{
			IMaterialRenderer* imr = new IMaterialRenderer();
			nullDriver->addMaterialRenderer(imr);
			imr->drop();
		}

		return nullDriver;
	}


	//! 设置/取消设置 一个剪裁平面
	//! 有最少6个剪裁平面用户可以设置
	//! \param index: 平面的索引。必须在0到MaxUserClipPlanes.
	//! \param plane: 平面自身
	//! \param enable: 如果为true，打开剪裁平面，否则关闭它
	bool CNullDriver::setClipPlane(UINT32 index, const Plane& plane, bool enable)
	{
		return false;
	}


	//! 打开/关闭一个剪裁平面
	//! 有最少6个剪裁平面用户可以设置
	//! \param plane: 平面自身
	//! \param enable: 如果为true，打开剪裁平面，否则关闭它
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


	//! 为了修改它的值获取2d覆盖材质
	SMaterial& CNullDriver::getMaterial2D()
	{
		return OverrideMaterial2D;
	}


	//! 打开2d覆盖材质
	void CNullDriver::enableMaterial2D(bool enable)
	{
		OverrideMaterial2DEnabled = enable;
	}


	dimension2du CNullDriver::getMaxTextureSize() const
	{
		return dimension2du(0x10000, 0x10000); // 可以足够大
	}


	//! 颜色转换函数
		/** 将一个图像（一个像素数组）转换到目的格式的图像（像素数组）.
		像素大小由颜色格式决定
		\param sP 源图像指针
		\param sF 源图像颜色格式
		\param sN 转换的像素格式，两个数组必须一样大小
		\param dP 目的图像制作
		\param dF 目的图像格式
		*/
	void CNullDriver::convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
		void* dP, ECOLOR_FORMAT dF) const
	{
		CColorConverter::convert_viaFormat(sP, sF, sN, dP, dF);
	}

}