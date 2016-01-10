#ifndef _SAPPHIRE_C_IMAGE_LOADER_TGA_
#define _SAPPHIRE_C_IMAGE_LOADER_TGA_
#include "SapphireCompileConifg.h"
#include "SapphireIImageLoader.h"

namespace Sapphire
{
#if defined(SAPPHIRE_COMPILE_WITH_TGA_LOADER_) || defined(SAPPHIRE_COMPILE_WITH_TGA_WRITER_)


	// 字节对齐结构
#include "SapphirePack.h"

	// these structs are also used in the TGA writer
	struct STGAHeader{
		UINT8 IdLength;
		UINT8 ColorMapType;
		UINT8 ImageType;
		UINT8 FirstEntryIndex[2];
		UINT16 ColorMapLength;
		UINT8 ColorMapEntrySize;
		UINT8 XOrigin[2];
		UINT8 YOrigin[2];
		UINT16 ImageWidth;
		UINT16 ImageHeight;
		UINT8 PixelDepth;
		UINT8 ImageDescriptor;
	} PACK_STRUCT;

	struct STGAFooter
	{
		UINT32 ExtensionOffset;
		UINT32 DeveloperOffset;
		c8  Signature[18];
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

#endif // defined with loader or writer

#ifdef SAPPHIRE_COMPILE_WITH_BMP_LOADER_

	/*!
	Surface Loader for targa images
	*/
	class CImageLoaderTGA : public IImageLoader
	{
	public:

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".tga")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! returns true if the file maybe is able to be loaded by this class
		virtual bool isALoadableFileFormat(IReadFile* file) const;

		//! creates a surface from the file
		virtual IImage* loadImage(IReadFile* file) const;

	private:

		//! loads a compressed tga. Was written and sent in by Jon Pry, thank you very much!
		UINT8* loadCompressedImage(IReadFile *file, const STGAHeader& header) const;
	};


#endif // compiled with loader
}

#endif