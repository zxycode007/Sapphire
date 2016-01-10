#ifndef _SAPPHIRE_C_IMAGE_LOADER_PNG_
#define _SAPPHIRE_C_IMAGE_LOADER_PNG_

#include "SapphireCompileConifg.h"

#if defined(SAPPHIRE_COMPILE_WITH_PNG_LOADER_) || defined(SAPPHIRE_COMPILE_WITH_PNG_WRITER_)

#include "SapphireIImageLoader.h"

namespace Sapphire
{
	//!  Surface Loader for PNG files
	class CImageLoaderPng : public IImageLoader
	{
	public:

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".png")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! returns true if the file maybe is able to be loaded by this class
		virtual bool isALoadableFileFormat(IReadFile* file) const;

		//! creates a surface from the file
		virtual IImage* loadImage(IReadFile* file) const;
	};
}
#endif
#endif