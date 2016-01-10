#ifndef _SAPPHIRE_C_IMAGE_LOADER_BMP_
#define _SAPPHIRE_C_IMAGE_LOADER_BMP_

#include "SapphireCompileConifg.h"
#include "SapphireIImageLoader.h"

namespace Sapphire
{
#if defined(SAPPHIRE_COMPILE_WITH_BMP_LOADER_) || defined(SAPPHIRE_COMPILE_WITH_BMP_WRITER_)


	// 字节对齐结构
#include "SapphirePack.h"

	struct SBMPHeader
	{
		UINT16	Id;					//	BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
		//	BA - OS/2 Bitmap Array
		//	CI - OS/2 Color Icon
		//	CP - OS/2 Color Pointer
		//	IC - OS/2 Icon
		//	PT - OS/2 Pointer
		UINT32	FileSize;
		UINT32	Reserved;
		UINT32	BitmapDataOffset;
		UINT32	BitmapHeaderSize;	// should be 28h for windows bitmaps or
		// 0Ch for OS/2 1.x or F0h for OS/2 2.x
		UINT32 Width;
		UINT32 Height;
		UINT16 Planes;
		UINT16 BPP;					// 1: Monochrome bitmap
		// 4: 16 color bitmap
		// 8: 256 color bitmap
		// 16: 16bit (high color) bitmap
		// 24: 24bit (true color) bitmap
		// 32: 32bit (true color) bitmap

		UINT32  Compression;			// 0: none (Also identified by BI_RGB)
		// 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
		// 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
		// 3: Bitfields  (Also identified by BI_BITFIELDS)

		UINT32  BitmapDataSize;		// Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
		UINT32  PixelPerMeterX;
		UINT32  PixelPerMeterY;
		UINT32  Colors;
		UINT32  ImportantColors;
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

#endif // defined with loader or writer

#ifdef SAPPHIRE_COMPILE_WITH_BMP_LOADER_

	/*!
	Surface Loader for Windows bitmaps
	*/
	class CImageLoaderBMP : public IImageLoader
	{
	public:

		//! constructor
		CImageLoaderBMP();

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".tga")
		virtual bool isALoadableFileExtension(const  path& filename) const;

		//! returns true if the file maybe is able to be loaded by this class
		virtual bool isALoadableFileFormat(IReadFile* file) const;

		//! creates a surface from the file
		virtual IImage* loadImage(IReadFile* file) const;

	private:

		void decompress8BitRLE(UINT8*& BmpData, SINT32 size, SINT32 width, SINT32 height, SINT32 pitch) const;

		void decompress4BitRLE(UINT8*& BmpData, SINT32 size, SINT32 width, SINT32 height, SINT32 pitch) const;
	};


#endif // compiled with loader
}

#endif