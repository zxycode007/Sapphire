#ifndef _SAPPHIRE_C_IMAGE_WRITER_JPEG_
#define  _SAPPHIRE_C_IMAGE_WRITER_JPEG_

#include "SapphireCompileConifg.h"

#ifdef SAPPHIRE_COMPILE_WITH_JPG_WRITER_
#include "SapphireIImageWriter.h"


namespace Sapphire
{
	class CImageWriterJPG : public IImageWriter
	{
	public:
		//! constructor
		CImageWriterJPG();

		//! return true if this writer can write a file with the given extension
		virtual bool isAWriteableFileExtension(const path& filename) const;

		//! write image to file
		virtual bool writeImage(IWriteFile *file, IImage *image, UINT32 param) const;
	};
}

#endif

#endif