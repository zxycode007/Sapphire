#include "SapphireCImageLoaderJPEG.h"

#ifdef SAPPHIRE_COMPILE_WITH_JPG_LOADER_

#include "SapphireIReadFile.h"
#include "SapphireCImage.h"
#include "SapphireOS.h"
#include "SapphireString.h"

namespace Sapphire
{
#ifdef SAPPHIRE_COMPILE_WITH_LIBJPEG_
	// Static members
	path CImageLoaderJPG::Filename;
#endif

	//! constructor
	CImageLoaderJPG::CImageLoaderJPG()
	{
#ifdef _DEBUG
		setDebugName("CImageLoaderJPG");
#endif
	}



	//! destructor
	CImageLoaderJPG::~CImageLoaderJPG()
	{
	}



	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	bool CImageLoaderJPG::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "jpg", "jpeg");
	}


#ifdef SAPPHIRE_COMPILE_WITH_LIBJPEG_

	// struct for handling jpeg errors
	struct irr_jpeg_error_mgr
	{
		// public jpeg error fields
		struct jpeg_error_mgr pub;

		// for longjmp, to return to caller on a fatal error
		jmp_buf setjmp_buffer;
	};

	void CImageLoaderJPG::init_source(j_decompress_ptr cinfo)
	{
		// DO NOTHING
	}



	boolean CImageLoaderJPG::fill_input_buffer(j_decompress_ptr cinfo)
	{
		// DO NOTHING
		return 1;
	}



	void CImageLoaderJPG::skip_input_data(j_decompress_ptr cinfo, long count)
	{
		jpeg_source_mgr * src = cinfo->src;
		if (count > 0)
		{
			src->bytes_in_buffer -= count;
			src->next_input_byte += count;
		}
	}



	void CImageLoaderJPG::term_source(j_decompress_ptr cinfo)
	{
		// DO NOTHING
	}


	void CImageLoaderJPG::error_exit(j_common_ptr cinfo)
	{
		// unfortunately we need to use a goto rather than throwing an exception
		// as gcc crashes under linux crashes when using throw from within
		// extern c code

		// Always display the message
		(*cinfo->err->output_message) (cinfo);

		// cinfo->err really points to a irr_error_mgr struct
		irr_jpeg_error_mgr *myerr = (irr_jpeg_error_mgr*)cinfo->err;

		longjmp(myerr->setjmp_buffer, 1);
	}


	void CImageLoaderJPG::output_message(j_common_ptr cinfo)
	{
		// display the error message.
		c8 temp1[JMSG_LENGTH_MAX];
		(*cinfo->err->format_message)(cinfo, temp1);
		String errMsg("JPEG FATAL ERROR in ");
		errMsg += String(Filename);
		Printer::log(errMsg.c_str(), temp1, LML_ERROR);
	}
#endif // SAPPHIRE_COMPILE_WITH_LIBJPEG_

	//! returns true if the file maybe is able to be loaded by this class
	bool CImageLoaderJPG::isALoadableFileFormat(IReadFile* file) const
	{
#ifndef SAPPHIRE_COMPILE_WITH_LIBJPEG_
		return false;
#else

		if (!file)
			return false;

		SINT32 jfif = 0;
		file->seek(6);
		file->read(&jfif, sizeof(SINT32));
		return (jfif == 0x4a464946 || jfif == 0x4649464a);

#endif
	}

	//! creates a surface from the file
	IImage* CImageLoaderJPG::loadImage(IReadFile* file) const
	{
#ifndef SAPPHIRE_COMPILE_WITH_LIBJPEG_
		Printer::log("Can't load as not compiled with SAPPHIRE_COMPILE_WITH_LIBJPEG_:", file->getFileName(), LML_CRITICAL);
		return 0;
#else

		if (!file)
			return 0;

		Filename = file->getFileName();

		UINT8 **rowPtr = 0;
		UINT8* input = new UINT8[file->getSize()];
		file->read(input, file->getSize());

		// allocate and initialize JPEG decompression object
		struct jpeg_decompress_struct cinfo;
		struct irr_jpeg_error_mgr jerr;

		//We have to set up the error handler first, in case the initialization
		//step fails.  (Unlikely, but it could happen if you are out of memory.)
		//This routine fills in the contents of struct jerr, and returns jerr's
		//address which we place into the link field in cinfo.

		cinfo.err = jpeg_std_error(&jerr.pub);
		cinfo.err->error_exit = error_exit;
		cinfo.err->output_message = output_message;

		// compatibility fudge:
		// we need to use setjmp/longjmp for error handling as gcc-linux
		// crashes when throwing within external c code
		if (setjmp(jerr.setjmp_buffer))
		{
			// If we get here, the JPEG code has signaled an error.
			// We need to clean up the JPEG object and return.

			jpeg_destroy_decompress(&cinfo);

			delete[] input;
			// if the row pointer was created, we delete it.
			if (rowPtr)
				delete[] rowPtr;

			// return null pointer
			return 0;
		}

		// Now we can initialize the JPEG decompression object.
		jpeg_create_decompress(&cinfo);

		// specify data source
		jpeg_source_mgr jsrc;

		// Set up data pointer
		jsrc.bytes_in_buffer = file->getSize();
		jsrc.next_input_byte = (JOCTET*)input;
		cinfo.src = &jsrc;

		jsrc.init_source = init_source;
		jsrc.fill_input_buffer = fill_input_buffer;
		jsrc.skip_input_data = skip_input_data;
		jsrc.resync_to_restart = jpeg_resync_to_restart;
		jsrc.term_source = term_source;

		// Decodes JPG input from whatever source
		// Does everything AFTER jpeg_create_decompress
		// and BEFORE jpeg_destroy_decompress
		// Caller is responsible for arranging these + setting up cinfo

		// read file parameters with jpeg_read_header()
		jpeg_read_header(&cinfo, TRUE);

		bool useCMYK = false;
		if (cinfo.jpeg_color_space == JCS_CMYK)
		{
			cinfo.out_color_space = JCS_CMYK;
			cinfo.out_color_components = 4;
			useCMYK = true;
		}
		else
		{
			cinfo.out_color_space = JCS_RGB;
			cinfo.out_color_components = 3;
		}
		cinfo.output_gamma = 2.2;
		cinfo.do_fancy_upsampling = FALSE;

		// Start decompressor
		jpeg_start_decompress(&cinfo);

		// Get image data
		UINT16 rowspan = cinfo.image_width * cinfo.out_color_components;
		UINT32 width = cinfo.image_width;
		UINT32 height = cinfo.image_height;

		// Allocate memory for buffer
		UINT8* output = new UINT8[rowspan * height];

		// Here we use the library's state variable cinfo.output_scanline as the
		// loop counter, so that we don't have to keep track ourselves.
		// Create array of row pointers for lib
		rowPtr = new UINT8*[height];

		for (UINT32 i = 0; i < height; i++)
			rowPtr[i] = &output[i * rowspan];

		UINT32 rowsRead = 0;

		while (cinfo.output_scanline < cinfo.output_height)
			rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);

		delete[] rowPtr;
		// Finish decompression

		jpeg_finish_decompress(&cinfo);

		// Release JPEG decompression object
		// This is an important step since it will release a good deal of memory.
		jpeg_destroy_decompress(&cinfo);

		// convert image
		IImage* image = 0;
		if (useCMYK)
		{
			image = new CImage(ECF_R8G8B8,
				dimension2d<UINT32>(width, height));
			const UINT32 size = 3 * width*height;
			UINT8* data = (UINT8*)image->lock();
			if (data)
			{
				for (UINT32 i = 0, j = 0; i<size; i += 3, j += 4)
				{
					// Also works without K, but has more contrast with K multiplied in
					//				data[i+0] = output[j+2];
					//				data[i+1] = output[j+1];
					//				data[i+2] = output[j+0];
					data[i + 0] = (char)(output[j + 2] * (output[j + 3] / 255.f));
					data[i + 1] = (char)(output[j + 1] * (output[j + 3] / 255.f));
					data[i + 2] = (char)(output[j + 0] * (output[j + 3] / 255.f));
				}
			}
			image->unlock();
			delete[] output;
		}
		else
			image = new CImage(ECF_R8G8B8,
			dimension2d<UINT32>(width, height), output);

		delete[] input;

		return image;

#endif
	}



	//! creates a loader which is able to load jpeg images
	IImageLoader* createImageLoaderJPG()
	{
		return new CImageLoaderJPG();
	}
}


#endif