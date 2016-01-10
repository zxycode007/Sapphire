#ifndef _SAPPHIRE_C_IMAGE_WRITER_TGA_
#define _SAPPHIRE_C_IMAGE_WRITER_TGA_

#include "SapphireCompileConifg.h"

#ifdef SAPPHIRE_COMPILE_WITH_TGA_WRITER_

#include "SapphireIImageWriter.h"

namespace Sapphire
{
	class CImageWriterTGA : public IImageWriter
	{
	public:
		//! constructor
		CImageWriterTGA();

		//! return true if this writer can write a file with the given extension
		virtual bool isAWriteableFileExtension(const path& filename) const;

		//! write image to file
		virtual bool writeImage(IWriteFile *file, IImage *image, UINT32 param) const;
	};
}


#endif

#endif