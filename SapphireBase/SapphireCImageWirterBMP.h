#ifndef _SAPPHIRE_C_IMAGE_WRITER_BMP_
#define _SAPPHIRE_C_IMAGE_WRITER_BMP_

#include "SapphireCompileConifg.h"

#ifdef SAPPHIRE_COMPILE_WITH_BMP_WRITER_
#include "SapphireIImageWriter.h"

namespace Sapphire
{
	class CImageWriterBMP : public IImageWriter
	{
	public:
		//! constructor
		CImageWriterBMP();

		//! return true if this writer can write a file with the given extension
		virtual bool isAWriteableFileExtension(const path& filename) const;

		//! write image to file
		virtual bool writeImage(IWriteFile *file, IImage *image, UINT32 param) const;
};
}

#endif

#endif