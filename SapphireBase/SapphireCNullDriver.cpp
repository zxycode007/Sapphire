
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
	//! creates a loader which is able to load windows bitmaps
	IImageLoader* createImageLoaderBMP();

	//! creates a loader which is able to load jpeg images
	IImageLoader* createImageLoaderJPG();

	//! creates a loader which is able to load targa images
	IImageLoader* createImageLoaderTGA();

	//! creates a loader which is able to load psd images
	IImageLoader* createImageLoaderPSD();

	//! creates a loader which is able to load dds images
	IImageLoader* createImageLoaderDDS();

	//! creates a loader which is able to load pcx images
	IImageLoader* createImageLoaderPCX();

	//! creates a loader which is able to load png images
	IImageLoader* createImageLoaderPNG();

	//! creates a loader which is able to load WAL images
	IImageLoader* createImageLoaderWAL();

	//! creates a loader which is able to load halflife images
	IImageLoader* createImageLoaderHalfLife();

	//! creates a loader which is able to load lmp images
	IImageLoader* createImageLoaderLMP();

	//! creates a loader which is able to load ppm/pgm/pbm images
	IImageLoader* createImageLoaderPPM();

	//! creates a loader which is able to load rgb images
	IImageLoader* createImageLoaderRGB();


	//! creates a writer which is able to save bmp images
	IImageWriter* createImageWriterBMP();

	//! creates a writer which is able to save jpg images
	IImageWriter* createImageWriterJPG();

	//! creates a writer which is able to save tga images
	IImageWriter* createImageWriterTGA();

	//! creates a writer which is able to save psd images
	IImageWriter* createImageWriterPSD();

	//! creates a writer which is able to save pcx images
	IImageWriter* createImageWriterPCX();

	//! creates a writer which is able to save png images
	IImageWriter* createImageWriterPNG();

	//! creates a writer which is able to save ppm images
	IImageWriter* createImageWriterPPM();

	//! constructor
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

		// create manipulator
		MeshManipulator = new CMeshManipulator();

		if (FileSystem)
			FileSystem->grab();

		// create surface loader

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


		// set ExposedData to 0
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


	//! destructor
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

		// delete material renderers
		deleteMaterialRenders();

		// delete hardware mesh buffers
		removeAllHardwareBuffers();
	}


	//! Adds an external surface loader to the engine.
	void CNullDriver::addExternalImageLoader(IImageLoader* loader)
	{
		if (!loader)
			return;

		loader->grab();
		SurfaceLoader.push_back(loader);
	}


	//! Adds an external surface writer to the engine.
	void CNullDriver::addExternalImageWriter(IImageWriter* writer)
	{
		if (!writer)
			return;

		writer->grab();
		SurfaceWriter.push_back(writer);
	}


	//! Retrieve the number of image loaders
	UINT32 CNullDriver::getImageLoaderCount() const
	{
		return SurfaceLoader.size();
	}


	//! Retrieve the given image loader
	IImageLoader* CNullDriver::getImageLoader(UINT32 n)
	{
		if (n < SurfaceLoader.size())
			return SurfaceLoader[n];
		return 0;
	}


	//! Retrieve the number of image writers
	UINT32 CNullDriver::getImageWriterCount() const
	{
		return SurfaceWriter.size();
	}


	//! Retrieve the given image writer
	IImageWriter* CNullDriver::getImageWriter(UINT32 n)
	{
		if (n < SurfaceWriter.size())
			return SurfaceWriter[n];
		return 0;
	}


	//! deletes all textures
	void CNullDriver::deleteAllTextures()
	{
		// we need to remove previously set textures which might otherwise be kept in the
		// last set material member. Could be optimized to reduce state changes.
		setMaterial(SMaterial());

		for (UINT32 i = 0; i<Textures.size(); ++i)
			Textures[i].Surface->drop();

		Textures.clear();
	}



	//! applications must call this method before performing any rendering. returns false if failed.
	bool CNullDriver::beginScene(bool backBuffer, bool zBuffer, ColourValue color,
		const SExposedVideoData& videoData, rect<SINT32>* sourceRect)
	{
		clearFPUException();
		PrimitivesDrawn = 0;
		return true;
	}


	//! applications must call this method after performing any rendering. returns false if failed.
	bool CNullDriver::endScene()
	{
		FPSCounter.registerFrame(Timer::getRealTime(), PrimitivesDrawn);
		updateAllHardwareBuffers();
		updateAllOcclusionQueries();
		return true;
	}


	//! Disable a feature of the driver.
	void CNullDriver::disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag)
	{
		FeatureEnabled[feature] = !flag;
	}


	//! queries the features of the driver, returns true if feature is available
	bool CNullDriver::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
	{
		return false;
	}


	//! Get attributes of the actual video driver
	const IAttributes& CNullDriver::getDriverAttributes() const
	{
		return *DriverAttributes;
	}


	//! sets transformation
	void CNullDriver::setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat)
	{
	}


	//! Returns the transformation set by setTransform
	const Matrix4& CNullDriver::getTransform(E_TRANSFORMATION_STATE state) const
	{
		return TransformationMatrix;
	}


	//! sets a material
	void CNullDriver::setMaterial(const SMaterial& material)
	{
	}


	//! Removes a texture from the texture cache and deletes it, freeing lot of
	//! memory.
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


	//! Removes all texture from the texture cache and deletes them, freeing lot of
	//! memory.
	void CNullDriver::removeAllTextures()
	{
		setMaterial(SMaterial());
		deleteAllTextures();
	}


	//! Returns a texture by index
	ITexture* CNullDriver::getTextureByIndex(UINT32 i)
	{
		if (i < Textures.size())
			return Textures[i].Surface;

		return 0;
	}


	//! Returns amount of textures currently loaded
	UINT32 CNullDriver::getTextureCount() const
	{
		return Textures.size();
	}


	//! Renames a texture
	void CNullDriver::renameTexture(ITexture* texture, const path& newName)
	{
		// we can do a const_cast here safely, the name of the ITexture interface
		// is just readonly to prevent the user changing the texture name without invoking
		// this method, because the textures will need resorting afterwards

		SNamedPath& name = const_cast<SNamedPath&>(texture->getName());
		name.setPath(newName);
		
		std::stable_sort(Textures.begin(), Textures.end(), ITextureCMP);
	}


	//! loads a Texture
	ITexture* CNullDriver::getTexture(const path& filename)
	{
		// Identify textures by their absolute filenames if possible.
		const path absolutePath = FileSystem->getAbsolutePath(filename);

		ITexture* texture = findTexture(absolutePath);
		if (texture)
			return texture;

		// Then try the raw filename, which might be in an Archive
		texture = findTexture(filename);
		if (texture)
			return texture;

		// Now try to open the file using the complete path.
		IReadFile* file = FileSystem->createAndOpenFile(absolutePath);

		if (!file)
		{
			// Try to open it using the raw filename.
			file = FileSystem->createAndOpenFile(filename);
		}

		if (file)
		{
			// Re-check name for actual archive names
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
				texture->drop(); // drop it because we created it, one grab too much
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


	//! loads a Texture
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
				texture->drop(); // drop it because we created it, one grab too much
			}

			if (!texture)
				Printer::log("Could not load texture", file->getFileName(), LML_WARNING);
		}

		return texture;
	}


	//! opens the file and loads it into the surface
	ITexture* CNullDriver::loadTextureFromFile(IReadFile* file, const path& hashName)
	{
		ITexture* texture = 0;
		IImage* image = createImageFromFile(file);

		if (image)
		{
			// create texture from surface
			texture = createDeviceDependentTexture(image, hashName.size() ? hashName : file->getFileName());
			Printer::log("Loaded texture", file->getFileName());
			image->drop();
		}

		return texture;
	}


	//! adds a surface, not loaded or created by the Irrlicht Engine
	void CNullDriver::addTexture(ITexture* texture)
	{
		if (texture)
		{
			SSurface s;
			s.Surface = texture;
			texture->grab();

			Textures.push_back(s);

			// the new texture is now at the end of the texture list. when searching for
			// the next new texture, the texture array will be sorted and the index of this texture
			// will be changed. to let the order be more consistent to the user, sort
			// the textures now already although this isn't necessary:

			std::stable_sort(Textures.begin(), Textures.end(), ITextureCMP);
		}
	}


	//! looks if the image is already loaded
	ITexture* CNullDriver::findTexture(const path& filename)
	{
		SSurface s;
		SDummyTexture dummy(filename);
		s.Surface = &dummy;

		SINT32 index = Textures.binary_search(s);
		if (index != -1)
			return Textures[index].Surface;

		return 0;
	}


	//! Creates a texture from a loaded IImage.
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


	//! creates a Texture
	ITexture* CNullDriver::addTexture(const dimension2d<UINT32>& size,
		const path& name, ECOLOR_FORMAT format)
	{
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create ITexture, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		if (0 == name.size())
			return 0;

		IImage* image = new CImage(format, size);
		ITexture* t = createDeviceDependentTexture(image, name);
		image->drop();
		addTexture(t);

		if (t)
			t->drop();

		return t;
	}



	//! returns a device dependent texture from a software surface (IImage)
	//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
	ITexture* CNullDriver::createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData)
	{
		return new SDummyTexture(name);
	}


	//! set or reset special render targets
	bool CNullDriver::setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
		bool clearZBuffer, ColourValue color)
	{
		if (ERT_FRAME_BUFFER == target)
			return setRenderTarget(0, clearTarget, clearZBuffer, color);
		else
			return false;
	}


	//! sets a render target
	bool CNullDriver::setRenderTarget(ITexture* texture, bool clearBackBuffer,
		bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! Sets multiple render targets
	bool CNullDriver::setRenderTarget(const vector<IRenderTarget>::type& texture,
		bool clearBackBuffer, bool clearZBuffer, ColourValue color)
	{
		return false;
	}


	//! sets a viewport
	void CNullDriver::setViewPort(const rect<SINT32>& area)
	{
	}


	//! gets the area of the current viewport
	const rect<SINT32>& CNullDriver::getViewPort() const
	{
		return ViewPort;
	}


	//! draws a vertex primitive list
	void CNullDriver::drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! draws a vertex primitive list in 2d
	void CNullDriver::draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount, const void* indexList, UINT32 primitiveCount, E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
	{
		if ((iType == EIT_16BIT) && (vertexCount>65536))
			Printer::log("Too many vertices for 16bit index type, render artifacts may occur.");
		PrimitivesDrawn += primitiveCount;
	}


	//! Draws a 3d line.
	void CNullDriver::draw3DLine(const Vector3& start,
		const Vector3& end, ColourValue color)
	{
	}


	//! Draws a 3d triangle.
	void CNullDriver::draw3DTriangle(const triangle3df& triangle, ColourValue color)
	{
		S3DVertex vertices[3];
		vertices[0].Pos = triangle.pointA;
		vertices[0].Color = color;
		vertices[0].Normal = triangle.getNormal().normalise();
		vertices[0].TCoords.set(0.f, 0.f);
		vertices[1].Pos = triangle.pointB;
		vertices[1].Color = color;
		vertices[1].Normal = vertices[0].Normal;
		vertices[1].TCoords.set(0.5f, 1.f);
		vertices[2].Pos = triangle.pointC;
		vertices[2].Color = color;
		vertices[2].Normal = vertices[0].Normal;
		vertices[2].TCoords.set(1.f, 0.f);
		const UINT16 indexList[] = { 0, 1, 2 };
		drawVertexPrimitiveList(vertices, 3, indexList, 1, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}


	//! Draws a 3d axis aligned box.
	void CNullDriver::draw3DBox(const AxisAlignedBox& box, ColourValue color)
	{
		Vector3 edges[8];
		box.getEdges(edges);

		// TODO: optimize into one big drawIndexPrimitive call.

		draw3DLine(edges[5], edges[1], color);
		draw3DLine(edges[1], edges[3], color);
		draw3DLine(edges[3], edges[7], color);
		draw3DLine(edges[7], edges[5], color);
		draw3DLine(edges[0], edges[2], color);
		draw3DLine(edges[2], edges[6], color);
		draw3DLine(edges[6], edges[4], color);
		draw3DLine(edges[4], edges[0], color);
		draw3DLine(edges[1], edges[0], color);
		draw3DLine(edges[3], edges[2], color);
		draw3DLine(edges[7], edges[6], color);
		draw3DLine(edges[5], edges[4], color);
	}



	//! draws an 2d image
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos)
	{
		if (!texture)
			return;

		draw2DImage(texture, destPos, rect<SINT32>(Position2d(0, 0),
			dimension2di(texture->getOriginalSize())));
	}



	//! draws a set of 2d images, using a color and the alpha channel of the
	//! texture if desired. The images are drawn beginning at pos and concatenated
	//! in one line. All drawings are clipped against clipRect (if != 0).
	//! The subtextures are defined by the array of sourceRects and are chosen
	//! by the indices given.
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

	//! draws a set of 2d images, using a color and the alpha channel of the
	//! texture if desired.
	void CNullDriver::draw2DImageBatch(const ITexture* texture,
		const vector<Position2d>::type& positions,
		const vector<rect<SINT32> >::type& sourceRects,
		const rect<SINT32>* clipRect,
		ColourValue color,
		bool useAlphaChannelOfTexture)
	{
		const UINT32 drawCount = min_<UINT32>(positions.size(), sourceRects.size());

		for (UINT32 i = 0; i<drawCount; ++i)
		{
			draw2DImage(texture, positions[i], sourceRects[i],
				clipRect, color, useAlphaChannelOfTexture);
		}
	}


	//! Draws a part of the texture into the rectangle.
	void CNullDriver::draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
		const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect,
		const ColourValue* const colors, bool useAlphaChannelOfTexture)
	{
		if (destRect.isValid())
			draw2DImage(texture, Position2d(destRect.UpperLeftCorner),
			sourceRect, clipRect, colors ? colors[0] : ColourValue(0xffffffff),
			useAlphaChannelOfTexture);
	}


	//! Draws a 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
	void CNullDriver::draw2DImage(const ITexture* texture, const Position2d& destPos,
		const rect<SINT32>& sourceRect,
		const rect<SINT32>* clipRect, ColourValue color,
		bool useAlphaChannelOfTexture)
	{
	}


	//! Draws the outline of a 2d rectangle
	void CNullDriver::draw2DRectangleOutline(const recti& pos, ColourValue color)
	{
		draw2DLine(pos.UpperLeftCorner, Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), color);
		draw2DLine(Position2d(pos.LowerRightCorner.x, pos.UpperLeftCorner.y), pos.LowerRightCorner, color);
		draw2DLine(pos.LowerRightCorner, Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), color);
		draw2DLine(Position2d(pos.UpperLeftCorner.x, pos.LowerRightCorner.y), pos.UpperLeftCorner, color);
	}


	//! Draw a 2d rectangle
	void CNullDriver::draw2DRectangle(ColourValue color, const rect<SINT32>& pos, const rect<SINT32>* clip)
	{
		draw2DRectangle(pos, color, color, color, color, clip);
	}



	//! Draws a 2d rectangle with a gradient.
	void CNullDriver::draw2DRectangle(const rect<SINT32>& pos,
		ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
		const rect<SINT32>* clip)
	{
	}



	//! Draws a 2d line.
	void CNullDriver::draw2DLine(const Position2d& start,
		const Position2d& end, ColourValue color)
	{
	}

	//! Draws a pixel
	void CNullDriver::drawPixel(UINT32 x, UINT32 y, const ColourValue & color)
	{
	}


	//! Draws a non filled concyclic regular 2d polyon.
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


	//! returns color format
	ECOLOR_FORMAT CNullDriver::getColorFormat() const
	{
		return ECF_R5G6B5;
	}


	//! returns screen size
	const dimension2d<UINT32>& CNullDriver::getScreenSize() const
	{
		return ScreenSize;
	}


	//! returns the current render target size,
	//! or the screen size if render targets are not implemented
	const dimension2d<UINT32>& CNullDriver::getCurrentRenderTargetSize() const
	{
		return ScreenSize;
	}


	// returns current frames per second value
	SINT32 CNullDriver::getFPS() const
	{
		return FPSCounter.getFPS();
	}



	//! returns amount of primitives (mostly triangles) were drawn in the last frame.
	//! very useful method for statistics.
	UINT32 CNullDriver::getPrimitiveCountDrawn(UINT32 param) const
	{
		return (0 == param) ? FPSCounter.getPrimitive() : (1 == param) ? FPSCounter.getPrimitiveAverage() : FPSCounter.getPrimitiveTotal();
	}



	//! Sets the dynamic ambient light color. The default color is
	//! (0,0,0,0) which means it is dark.
	//! \param color: New color of the ambient light.
	void CNullDriver::setAmbientLight(const ColourValue& color)
	{
	}



	//! \return Returns the name of the video driver. Example: In case of the DIRECT3D8
	//! driver, it would return "Direct3D8".

	const wchar_t* CNullDriver::getName() const
	{
		return L"Irrlicht NullDevice";
	}



	//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
	//! this: Frist, draw all geometry. Then use this method, to draw the shadow
	//! volume. Then, use IVideoDriver::drawStencilShadow() to visualize the shadow.
	void CNullDriver::drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible)
	{
	}


	//! Fills the stencil shadow with color. After the shadow volume has been drawn
	//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
	//! to draw the color of the shadow.
	void CNullDriver::drawStencilShadow(bool clearStencilBuffer,
		ColourValue leftUpEdge, ColourValue rightUpEdge,
		ColourValue leftDownEdge, ColourValue rightDownEdge)
	{
	}


	//! deletes all dynamic lights there are
	void CNullDriver::deleteAllDynamicLights()
	{
		Lights.set_used(0);
	}


	//! adds a dynamic light
	SINT32 CNullDriver::addDynamicLight(const SLight& light)
	{
		Lights.push_back(light);
		return Lights.size() - 1;
	}

	//! Turns a dynamic light on or off
	//! \param lightIndex: the index returned by addDynamicLight
	//! \param turnOn: true to turn the light on, false to turn it off
	void CNullDriver::turnLightOn(SINT32 lightIndex, bool turnOn)
	{
		// Do nothing
	}


	//! returns the maximal amount of dynamic lights the device can handle
	UINT32 CNullDriver::getMaximalDynamicLightAmount() const
	{
		return 0;
	}


	//! Returns current amount of dynamic lights set
	//! \return Current amount of dynamic lights set
	UINT32 CNullDriver::getDynamicLightCount() const
	{
		return Lights.size();
	}


	//! Returns light data which was previously set by IVideoDriver::addDynamicLight().
	//! \param idx: Zero based index of the light. Must be greater than 0 and smaller
	//! than IVideoDriver()::getDynamicLightCount.
	//! \return Light data.
	const SLight& CNullDriver::getDynamicLight(UINT32 idx) const
	{
		if (idx < Lights.size())
			return Lights[idx];
		else
			return *((SLight*)0);
	}


	//! Creates a boolean alpha channel of the texture based of an color key.
	void CNullDriver::makeColorKeyTexture(ITexture* texture,
		ColourValue color,
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

		if (texture->getColorFormat() == ECF_A1R5G5B5)
		{
			UINT16 *p = (UINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making color key channel.", LML_CRITICAL);
				return;
			}

			const dimension2d<UINT32> dim = texture->getSize();
			const UINT32 pitch = texture->getPitch() / 2;

			// color with alpha disabled (i.e. fully transparent)
			const UINT16 refZeroAlpha = (0x7fff & color.toA1R5G5B5());

			const UINT32 pixels = pitch * dim.Height;

			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// If the color matches the reference color, ignoring alphas,
				// set the alpha to zero.
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

			dimension2d<UINT32> dim = texture->getSize();
			UINT32 pitch = texture->getPitch() / 4;

			// color with alpha disabled (fully transparent)
			const UINT32 refZeroAlpha = 0x00ffffff & color.color;

			const UINT32 pixels = pitch * dim.Height;
			for (UINT32 pixel = 0; pixel < pixels; ++pixel)
			{
				// If the color matches the reference color, ignoring alphas,
				// set the alpha to zero.
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
		texture->regenerateMipMapLevels();
	}



	//! Creates an boolean alpha channel of the texture based of an color key position.
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

			const UINT16 key16Bit = 0x7fff & p[colorKeyPixelPos.Y*pitch + colorKeyPixelPos.X];

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
			colorKey = 0x00ffffff & p[colorKeyPixelPos.Y*pitch + colorKeyPixelPos.X];
		}

		texture->unlock();
		makeColorKeyTexture(texture, colorKey, zeroTexels);
	}



	//! Creates a normal map from a height map texture.
	//! \param amplitude: Constant value by which the height information is multiplied.
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

		dimension2d<UINT32> dim = texture->getSize();
		amplitude = amplitude / 255.0f;
		FLOAT32 vh = dim.Height / (FLOAT32)dim.Width;
		FLOAT32 hh = dim.Width / (FLOAT32)dim.Height;

		if (texture->getColorFormat() == ECF_A8R8G8B8)
		{
			// ECF_A8R8G8B8 version

			SINT32 *p = (SINT32*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			// copy texture

			UINT32 pitch = texture->getPitch() / 4;

			SINT32* in = new SINT32[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 4);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: this could be optimized really a lot

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
					n += Vector3(0.5f, 0.5f, 0.5f); // now between 0 and 1
					n *= 255.0f;

					SINT32 height = (SINT32)nml32(x, y, pitch, dim.Height, in);
					p[y*pitch + x] = ColourValue(
						height, // store height in alpha
						(SINT32)n.x, (SINT32)n.z, (SINT32)n.y).color;
				}

			delete[] in;
			texture->unlock();
		}
		else
		{
			// ECF_A1R5G5B5 version

			SINT16 *p = (SINT16*)texture->lock();

			if (!p)
			{
				Printer::log("Could not lock texture for making normal map.", LML_CRITICAL);
				return;
			}

			UINT32 pitch = texture->getPitch() / 2;

			// copy texture

			SINT16* in = new SINT16[dim.Height * pitch];
			memcpy(in, p, dim.Height * pitch * 2);

			for (SINT32 x = 0; x < SINT32(pitch); ++x)
				for (SINT32 y = 0; y < SINT32(dim.Height); ++y)
				{
					// TODO: this could be optimized really a lot

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

					p[y*pitch + x] = RGBA16((UINT32)n.X, (UINT32)n.Z, (UINT32)n.Y);
				}

			delete[] in;
			texture->unlock();
		}

		texture->regenerateMipMapLevels();
	}


	//! Returns the maximum amount of primitives (mostly vertices) which
	//! the device is able to render with one drawIndexedTriangleList
	//! call.
	UINT32 CNullDriver::getMaximalPrimitiveCount() const
	{
		return 0xFFFFFFFF;
	}


	//! checks triangle count and print warning if wrong
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

	//! Enables or disables a texture creation flag.
	void CNullDriver::setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled)
	{
		if (enabled && ((flag == ETCF_ALWAYS_16_BIT) || (flag == ETCF_ALWAYS_32_BIT)
			|| (flag == ETCF_OPTIMIZED_FOR_QUALITY) || (flag == ETCF_OPTIMIZED_FOR_SPEED)))
		{
			// disable other formats
			setTextureCreationFlag(ETCF_ALWAYS_16_BIT, false);
			setTextureCreationFlag(ETCF_ALWAYS_32_BIT, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, false);
			setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED, false);
		}

		// set flag
		TextureCreationFlags = (TextureCreationFlags & (~flag)) |
			((((UINT32)!enabled) - 1) & flag);
	}


	//! Returns if a texture creation flag is enabled or disabled.
	bool CNullDriver::getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const
	{
		return (TextureCreationFlags & flag) != 0;
	}


	//! Creates a software image from a file.
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


	//! Creates a software image from a file.
	IImage* CNullDriver::createImageFromFile(IReadFile* file)
	{
		if (!file)
			return 0;

		IImage* image = 0;

		SINT32 i;

		// try to load file based on file extension
		for (i = SurfaceLoader.size() - 1; i >= 0; --i)
		{
			if (SurfaceLoader[i]->isALoadableFileExtension(file->getFileName()))
			{
				// reset file position which might have changed due to previous loadImage calls
				file->seek(0);
				image = SurfaceLoader[i]->loadImage(file);
				if (image)
					return image;
			}
		}

		// try to load file based on what is in it
		for (i = SurfaceLoader.size() - 1; i >= 0; --i)
		{
			// dito
			file->seek(0);
			if (SurfaceLoader[i]->isALoadableFileFormat(file))
			{
				file->seek(0);
				image = SurfaceLoader[i]->loadImage(file);
				if (image)
					return image;
			}
		}

		return 0; // failed to load
	}


	//! Writes the provided image to disk file
	bool CNullDriver::writeImageToFile(IImage* image, const path& filename, UINT32 param)
	{
		IWriteFile* file = FileSystem->createAndWriteFile(filename);
		if (!file)
			return false;

		bool result = writeImageToFile(image, file, param);
		file->drop();

		return result;
	}

	//! Writes the provided image to a file.
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
		return false; // failed to write
	}


	//! Creates a software image from a byte array.
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


	//! Creates an empty software image.
	IImage* CNullDriver::createImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size)
	{
		if (IImage::isRenderTargetOnlyFormat(format))
		{
			Printer::log("Could not create IImage, format only supported for render target textures.", LML_WARNING);
			return 0;
		}

		return new CImage(format, size);
	}


	//! Creates a software image from another image.
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


	//! Creates a software image from part of another image.
	IImage* CNullDriver::createImage(IImage* imageToCopy, const Position2d& pos, const dimension2d<UINT32>& size)
	{
		Printer::log("Deprecated method, please create an empty image instead and use copyTo().", LML_WARNING);
		CImage* tmp = new CImage(imageToCopy->getColorFormat(), imageToCopy->getDimension());
		imageToCopy->copyTo(tmp, Position2d(0, 0), recti(pos, size));
		return tmp;
	}


	//! Creates a software image from part of a texture.
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
			// make sure to avoid buffer overruns
			// make the vector a separate variable for g++ 3.x
			const Vector2 leftUpper(clamp(static_cast<UINT32>(pos.x), 0u, texture->getSize().Width),
				clamp(static_cast<UINT32>(pos.y), 0u, texture->getSize().Height));
			const rect<UINT32> clamped(leftUpper,
				dimension2du(clamp(static_cast<UINT32>(size.Width), 0u, texture->getSize().Width),
				clamp(static_cast<UINT32>(size.Height), 0u, texture->getSize().Height)));
			if (!clamped.isValid())
				return 0;
			UINT8* src = static_cast<UINT8*>(texture->lock(ETLM_READ_ONLY));
			if (!src)
				return 0;
			IImage* image = new CImage(texture->getColorFormat(), clamped.getSize());
			UINT8* dst = static_cast<UINT8*>(image->lock());
			src += clamped.UpperLeftCorner.Y * texture->getPitch() + image->getBytesPerPixel() * clamped.UpperLeftCorner.X;
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


	//! Sets the fog mode.
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

	//! Gets the fog mode.
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

	//! Draws a mesh buffer
	void CNullDriver::drawMeshBuffer(const IMeshBuffer* mb)
	{
		if (!mb)
			return;

		//IVertexBuffer and IIndexBuffer later
		SHWBufferLink *HWBuffer = getBufferLink(mb);

		if (HWBuffer)
			drawHardwareBuffer(HWBuffer);
		else
			drawVertexPrimitiveList(mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount() / 3, mb->getVertexType(), EPT_TRIANGLES, mb->getIndexType());
	}


	//! Draws the normals of a mesh buffer
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


	CNullDriver::SHWBufferLink *CNullDriver::getBufferLink(const IMeshBuffer* mb)
	{
		if (!mb || !isHardwareBufferRecommend(mb))
			return 0;

		//search for hardware links
		map< const IMeshBuffer*, SHWBufferLink* >::Node* node = HWBufferMap.find(mb);
		if (node)
			return node->getValue();

		return createHardwareBuffer(mb); //no hardware links, and mesh wants one, create it
	}


	//! Update all hardware buffers, remove unused ones
	void CNullDriver::updateAllHardwareBuffers()
	{
		map<const IMeshBuffer*, SHWBufferLink*>::ParentFirstIterator Iterator = HWBufferMap.getParentFirstIterator();

		for (; !Iterator.atEnd(); Iterator++)
		{
			SHWBufferLink *Link = Iterator.getNode()->getValue();

			Link->LastUsed++;
			if (Link->LastUsed>20000)
			{
				deleteHardwareBuffer(Link);

				// todo: needs better fix
				Iterator = HWBufferMap.getParentFirstIterator();
			}
		}
	}


	void CNullDriver::deleteHardwareBuffer(SHWBufferLink *HWBuffer)
	{
		if (!HWBuffer)
			return;
		HWBufferMap.remove(HWBuffer->MeshBuffer);
		delete HWBuffer;
	}


	//! Remove hardware buffer
	void CNullDriver::removeHardwareBuffer(const IMeshBuffer* mb)
	{
		map<const IMeshBuffer*, SHWBufferLink*>::Node* node = HWBufferMap.find(mb);
		if (node)
			deleteHardwareBuffer(node->getValue());
	}


	//! Remove all hardware buffers
	void CNullDriver::removeAllHardwareBuffers()
	{
		while (HWBufferMap.size())
			deleteHardwareBuffer(HWBufferMap.getRoot()->getValue());
	}


	bool CNullDriver::isHardwareBufferRecommend(const IMeshBuffer* mb)
	{
		if (!mb || (mb->getHardwareMappingHint_Index() == EHM_NEVER && mb->getHardwareMappingHint_Vertex() == EHM_NEVER))
			return false;

		if (mb->getVertexCount()<MinVertexCountForVBO)
			return false;

		return true;
	}


	//! Create occlusion query.
	/** Use node for identification and mesh for occlusion test. */
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

		//search for query
		SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		if (index != -1)
		{
			if (OcclusionQueries[index].Mesh != mesh)
			{
				OcclusionQueries[index].Mesh->drop();
				OcclusionQueries[index].Mesh = mesh;
				mesh->grab();
			}
		}
		else
		{
			OcclusionQueries.push_back(SOccQuery(node, mesh));
			node->setAutomaticCulling(node->getAutomaticCulling() | EAC_OCC_QUERY);
		}
	}


	//! Remove occlusion query.
	void CNullDriver::removeOcclusionQuery(ISceneNode* node)
	{
		//search for query
		SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		if (index != -1)
		{
			node->setAutomaticCulling(node->getAutomaticCulling() & ~EAC_OCC_QUERY);
			OcclusionQueries.erase();
		}
	}


	//! Remove all occlusion queries.
	void CNullDriver::removeAllOcclusionQueries()
	{
		for (SINT32 i = OcclusionQueries.size() - 1; i >= 0; --i)
		{
			removeOcclusionQuery(OcclusionQueries[i].Node);
		}
	}


	//! Run occlusion query. Draws mesh stored in query.
	/** If the mesh shall be rendered visible, use
	flag to enable the proper material setting. */
	void CNullDriver::runOcclusionQuery(ISceneNode* node, bool visible)
	{
		if (!node)
			return;
		SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
		if (index == -1)
			return;
		OcclusionQueries[index].Run = 0;
		if (!visible)
		{
			SMaterial mat;
			mat.Lighting = false;
			mat.AntiAliasing = 0;
			mat.ColorMask = ECP_NONE;
			mat.GouraudShading = false;
			mat.ZWriteEnable = false;
			setMaterial(mat);
		}
		setTransform(ETS_WORLD, node->getAbsoluteTransformation());
		const IMesh* mesh = OcclusionQueries[index].Mesh;
		for (UINT32 i = 0; i<mesh->getMeshBufferCount(); ++i)
		{
			if (visible)
				setMaterial(mesh->getMeshBuffer(i)->getMaterial());
			drawMeshBuffer(mesh->getMeshBuffer(i));
		}
	}


	//! Run all occlusion queries. Draws all meshes stored in queries.
	/** If the meshes shall not be rendered visible, use
	overrideMaterial to disable the color and depth buffer. */
	void CNullDriver::runAllOcclusionQueries(bool visible)
	{
		for (UINT32 i = 0; i<OcclusionQueries.size(); ++i)
			runOcclusionQuery(OcclusionQueries[i].Node, visible);
	}


	//! Update occlusion query. Retrieves results from GPU.
	/** If the query shall not block, set the flag to false.
	Update might not occur in this case, though */
	void CNullDriver::updateOcclusionQuery(ISceneNode* node, bool block)
	{
	}


	//! Update all occlusion queries. Retrieves results from GPU.
	/** If the query shall not block, set the flag to false.
	Update might not occur in this case, though */
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


	//! Return query result.
	/** Return value is the number of visible pixels/fragments.
	The value is a safe approximation, i.e. can be larger then the
	actual value of pixels. */
	UINT32 CNullDriver::getOcclusionQueryResult(ISceneNode* node) const
	{
		return ~0;
	}


	//! Only used by the internal engine. Used to notify the driver that
	//! the window was resized.
	void CNullDriver::OnResize(const dimension2d<UINT32>& size)
	{
		if (ViewPort.getWidth() == (SINT32)ScreenSize.Width &&
			ViewPort.getHeight() == (SINT32)ScreenSize.Height)
			ViewPort = rect<SINT32>(Position2d(0, 0),
			dimension2di(size));

		ScreenSize = size;
	}


	// adds a material renderer and drops it afterwards. To be used for internal creation
	SINT32 CNullDriver::addAndDropMaterialRenderer(IMaterialRenderer* m)
	{
		SINT32 i = addMaterialRenderer(m);

		if (m)
			m->drop();

		return i;
	}


	//! Adds a new material renderer to the video device.
	SINT32 CNullDriver::addMaterialRenderer(IMaterialRenderer* renderer, const char* name)
	{
		if (!renderer)
			return -1;

		SMaterialRenderer r;
		r.Renderer = renderer;
		r.Name = name;

		if (name == 0 && (MaterialRenderers.size() < (sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1))
		{
			// set name of built in renderer so that we don't have to implement name
			// setting in all available renderers.
			r.Name = sBuiltInMaterialTypeNames[MaterialRenderers.size()];
		}

		MaterialRenderers.push_back(r);
		renderer->grab();

		return MaterialRenderers.size() - 1;
	}


	//! Sets the name of a material renderer.
	void CNullDriver::setMaterialRendererName(SINT32 idx, const char* name)
	{
		if (idx < SINT32(sizeof(sBuiltInMaterialTypeNames) / sizeof(char*)) - 1 ||
			idx >= (SINT32)MaterialRenderers.size())
			return;

		MaterialRenderers[idx].Name = name;
	}


	//! Creates material attributes list from a material, usable for serialization and more.
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
			if (options && (options->Flags&EARWF_USE_RELATIVE_PATHS) && options->Filename && material.getTexture(i))
			{
				path path = FileSystem->getRelativeFilename(
					FileSystem->getAbsolutePath(material.getTexture(i)->getName()), options->Filename);
				attr->addTexture((prefix + String(i + 1)).c_str(), material.getTexture(i), path);
			}
			else
				attr->addTexture((prefix + String(i + 1)).c_str(), material.getTexture(i));
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
			attr->addBool((prefix + String(i + 1)).c_str(), material.TextureLayer[i].BilinearFilter);
		prefix = "TrilinearFilter";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addBool((prefix + String(i + 1)).c_str(), material.TextureLayer[i].TrilinearFilter);
		prefix = "AnisotropicFilter";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addInt((prefix + String(i + 1)).c_str(), material.TextureLayer[i].AnisotropicFilter);
		prefix = "TextureWrapU";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addEnum((prefix + String(i + 1)).c_str(), material.TextureLayer[i].TextureWrapU, aTextureClampNames);
		prefix = "TextureWrapV";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addEnum((prefix + String(i + 1)).c_str(), material.TextureLayer[i].TextureWrapV, aTextureClampNames);
		prefix = "LODBias";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			attr->addInt((prefix + String(i + 1)).c_str(), material.TextureLayer[i].LODBias);

		return attr;
	}


	//! Fills an SMaterial structure from attributes.
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
			outMaterial.setTexture(i, attr->getAttributeAsTexture((prefix + String(i + 1)).c_str()));

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
				outMaterial.TextureLayer[i].BilinearFilter = attr->getAttributeAsBool((prefix + String(i + 1)).c_str());

		prefix = "TrilinearFilter";
		if (attr->existsAttribute(prefix.c_str())) // legacy
			outMaterial.setFlag(EMF_TRILINEAR_FILTER, attr->getAttributeAsBool(prefix.c_str()));
		else
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				outMaterial.TextureLayer[i].TrilinearFilter = attr->getAttributeAsBool((prefix + String(i + 1)).c_str());

		prefix = "AnisotropicFilter";
		if (attr->existsAttribute(prefix.c_str())) // legacy
			outMaterial.setFlag(EMF_ANISOTROPIC_FILTER, attr->getAttributeAsBool(prefix.c_str()));
		else
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
				outMaterial.TextureLayer[i].AnisotropicFilter = attr->getAttributeAsInt((prefix + String(i + 1)).c_str());

		prefix = "TextureWrap";
		if (attr->existsAttribute(prefix.c_str())) // legacy
		{
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + String(i + 1)).c_str(), aTextureClampNames);
				outMaterial.TextureLayer[i].TextureWrapV = outMaterial.TextureLayer[i].TextureWrapU;
			}
		}
		else
		{
			for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				outMaterial.TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "U" + String(i + 1)).c_str(), aTextureClampNames);
				outMaterial.TextureLayer[i].TextureWrapV = (E_TEXTURE_CLAMP)attr->getAttributeAsEnumeration((prefix + "V" + String(i + 1)).c_str(), aTextureClampNames);
			}
		}

		// default 0 is ok
		prefix = "LODBias";
		for (i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			outMaterial.TextureLayer[i].LODBias = attr->getAttributeAsInt((prefix + String(i + 1)).c_str());
	}


	//! Returns driver and operating system specific data about the IVideoDriver.
	const SExposedVideoData& CNullDriver::getExposedVideoData()
	{
		return ExposedData;
	}


	//! Returns type of video driver
	E_DRIVER_TYPE CNullDriver::getDriverType() const
	{
		return EDT_NULL;
	}


	//! deletes all material renderers
	void CNullDriver::deleteMaterialRenders()
	{
		// delete material renderers
		for (UINT32 i = 0; i<MaterialRenderers.size(); ++i)
			if (MaterialRenderers[i].Renderer)
				MaterialRenderers[i].Renderer->drop();

		MaterialRenderers.clear();
	}


	//! Returns pointer to material renderer or null
	IMaterialRenderer* CNullDriver::getMaterialRenderer(UINT32 idx)
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Renderer;
		else
			return 0;
	}


	//! Returns amount of currently available material renderers.
	UINT32 CNullDriver::getMaterialRendererCount() const
	{
		return MaterialRenderers.size();
	}


	//! Returns name of the material renderer
	const char* CNullDriver::getMaterialRendererName(UINT32 idx) const
	{
		if (idx < MaterialRenderers.size())
			return MaterialRenderers[idx].Name.c_str();

		return 0;
	}


	//! Returns pointer to the IGPUProgrammingServices interface.
	IGPUProgrammingServices* CNullDriver::getGPUProgrammingServices()
	{
		return this;
	}


	//! Adds a new material renderer to the VideoDriver, based on a high level shading language.
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


	//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
	//! but tries to load the programs from files.
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


	//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
	//! but tries to load the programs from files.
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


	//! Adds a new material renderer to the VideoDriver, using pixel and/or
	//! vertex shaders to render geometry.
	SINT32 CNullDriver::addShaderMaterial(const c8* vertexShaderProgram,
		const c8* pixelShaderProgram,
		IShaderConstantSetCallBack* callback,
		E_MATERIAL_TYPE baseMaterial,
		SINT32 userData)
	{
		Printer::log("Shader materials not implemented yet in this driver, sorry.");
		return -1;
	}


	//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
	//! programs from files.
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


	//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the
	//! programs from files.
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


	//! Creates a render target texture.
	ITexture* CNullDriver::addRenderTargetTexture(const dimension2d<UINT32>& size,
		const path&name, const ECOLOR_FORMAT format)
	{
		return 0;
	}


	//! Clears the ZBuffer.
	void CNullDriver::clearZBuffer()
	{
	}


	//! Returns a pointer to the mesh manipulator.
	IMeshManipulator* CNullDriver::getMeshManipulator()
	{
		return MeshManipulator;
	}


	//! Returns an image created from the last rendered frame.
	IImage* CNullDriver::createScreenShot(ECOLOR_FORMAT format, E_RENDER_TARGET target)
	{
		return 0;
	}


	// prints renderer version
	void CNullDriver::printVersion()
	{
		StringW namePrint = L"Using renderer: ";
		namePrint += getName();
		Printer::log(StringUtil::StringWToString(namePrint).c_str(), LML_NORMAL); 
	}


	//! creates a video driver
	IVideoDriver* createNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize)
	{
		CNullDriver* nullDriver = new CNullDriver(io, screenSize);

		// create empty material renderers
		for (UINT32 i = 0; sBuiltInMaterialTypeNames[i]; ++i)
		{
			IMaterialRenderer* imr = new IMaterialRenderer();
			nullDriver->addMaterialRenderer(imr);
			imr->drop();
		}

		return nullDriver;
	}


	//! Set/unset a clipping plane.
	//! There are at least 6 clipping planes available for the user to set at will.
	//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
	//! \param plane: The plane itself.
	//! \param enable: If true, enable the clipping plane else disable it.
	bool CNullDriver::setClipPlane(UINT32 index, const Plane& plane, bool enable)
	{
		return false;
	}


	//! Enable/disable a clipping plane.
	void CNullDriver::enableClipPlane(UINT32 index, bool enable)
	{
		// not necessary
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


	//! Get the 2d override material for altering its values
	SMaterial& CNullDriver::getMaterial2D()
	{
		return OverrideMaterial2D;
	}


	//! Enable the 2d override material
	void CNullDriver::enableMaterial2D(bool enable)
	{
		OverrideMaterial2DEnabled = enable;
	}


	dimension2du CNullDriver::getMaxTextureSize() const
	{
		return dimension2du(0x10000, 0x10000); // maybe large enough
	}


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
	void CNullDriver::convertColor(const void* sP, ECOLOR_FORMAT sF, SINT32 sN,
		void* dP, ECOLOR_FORMAT dF) const
	{
		CColorConverter::convert_viaFormat(sP, sF, sN, dP, dF);
	}

}