#include "SapphireCImageLoaderBMP.h"

#ifdef SAPPHIRE_COMPILE_WITH_BMP_LOADER_
#include "SapphireIReadFile.h"
#include "SapphireColorValue.h"
#include "SapphireCColourConverter.h"	
#include "SapphireCImage.h"
#include "SapphireOS.h"
#include "SapphireString.h"

namespace Sapphire
{
	//! constructor
	CImageLoaderBMP::CImageLoaderBMP()
	{
#ifdef _DEBUG
		setDebugName("CImageLoaderBMP");
#endif
	}


	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	bool CImageLoaderBMP::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "bmp");
	}


	//! returns true if the file maybe is able to be loaded by this class
	bool CImageLoaderBMP::isALoadableFileFormat(IReadFile* file) const
	{
		UINT16 headerID;
		file->read(&headerID, sizeof(UINT16));
#if (SAPPHIRE_CONFIG_ENDIAN ==1)
		headerID = os::Byteswap::byteswap(headerID);
#endif
		return headerID == 0x4d42;
	}


	void CImageLoaderBMP::decompress8BitRLE(UINT8*& bmpData, SINT32 size, SINT32 width, SINT32 height, SINT32 pitch) const
	{
		UINT8* p = bmpData;
		UINT8* newBmp = new UINT8[(width + pitch)*height];
		UINT8* d = newBmp;
		UINT8* destEnd = newBmp + (width + pitch)*height;
		SINT32 line = 0;

		while (bmpData - p < size && d < destEnd)
		{
			if (*p == 0)
			{
				++p;

				switch (*p)
				{
				case 0: // end of line
					++p;
					++line;
					d = newBmp + (line*(width + pitch));
					break;
				case 1: // end of bmp
					delete[] bmpData;
					bmpData = newBmp;
					return;
				case 2:
					++p; d += (UINT8)*p;  // delta
					++p; d += ((UINT8)*p)*(width + pitch);
					++p;
					break;
				default:
				{
					// absolute mode
					SINT32 count = (UINT8)*p; ++p;
					SINT32 readAdditional = ((2 - (count % 2)) % 2);
					SINT32 i;

					for (i = 0; i<count; ++i)
					{
						*d = *p;
						++p;
						++d;
					}

					for (i = 0; i<readAdditional; ++i)
						++p;
				}
				}
			}
			else
			{
				SINT32 count = (UINT8)*p; ++p;
				UINT8 color = *p; ++p;
				for (SINT32 i = 0; i<count; ++i)
				{
					*d = color;
					++d;
				}
			}
		}

		delete[] bmpData;
		bmpData = newBmp;
	}


	void CImageLoaderBMP::decompress4BitRLE(UINT8*& bmpData, SINT32 size, SINT32 width, SINT32 height, SINT32 pitch) const
	{
		SINT32 lineWidth = (width + 1) / 2 + pitch;
		UINT8* p = bmpData;
		UINT8* newBmp = new UINT8[lineWidth*height];
		UINT8* d = newBmp;
		UINT8* destEnd = newBmp + lineWidth*height;
		SINT32 line = 0;
		SINT32 shift = 4;

		while (bmpData - p < size && d < destEnd)
		{
			if (*p == 0)
			{
				++p;

				switch (*p)
				{
				case 0: // end of line
					++p;
					++line;
					d = newBmp + (line*lineWidth);
					shift = 4;
					break;
				case 1: // end of bmp
					delete[] bmpData;
					bmpData = newBmp;
					return;
				case 2:
				{
					++p;
					SINT32 x = (UINT8)*p; ++p;
					SINT32 y = (UINT8)*p; ++p;
					d += x / 2 + y*lineWidth;
					shift = x % 2 == 0 ? 4 : 0;
				}
				break;
				default:
				{
					// absolute mode
					SINT32 count = (UINT8)*p; ++p;
					SINT32 readAdditional = ((2 - ((count) % 2)) % 2);
					SINT32 readShift = 4;
					SINT32 i;

					for (i = 0; i<count; ++i)
					{
						SINT32 color = (((UINT8)*p) >> readShift) & 0x0f;
						readShift -= 4;
						if (readShift < 0)
						{
							++*p;
							readShift = 4;
						}

						UINT8 mask = 0x0f << shift;
						*d = (*d & (~mask)) | ((color << shift) & mask);

						shift -= 4;
						if (shift < 0)
						{
							shift = 4;
							++d;
						}

					}

					for (i = 0; i<readAdditional; ++i)
						++p;
				}
				}
			}
			else
			{
				SINT32 count = (UINT8)*p; ++p;
				SINT32 color1 = (UINT8)*p; color1 = color1 & 0x0f;
				SINT32 color2 = (UINT8)*p; color2 = (color2 >> 4) & 0x0f;
				++p;

				for (SINT32 i = 0; i<count; ++i)
				{
					UINT8 mask = 0x0f << shift;
					UINT8 toSet = (shift == 0 ? color1 : color2) << shift;
					*d = (*d & (~mask)) | (toSet & mask);

					shift -= 4;
					if (shift < 0)
					{
						shift = 4;
						++d;
					}
				}
			}
		}

		delete[] bmpData;
		bmpData = newBmp;
	}



	//! creates a surface from the file
	IImage* CImageLoaderBMP::loadImage(IReadFile* file) const
	{
		SBMPHeader header;

		file->read(&header, sizeof(header));

#ifdef __BIG_ENDIAN__
		header.Id = os::Byteswap::byteswap(header.Id);
		header.FileSize = os::Byteswap::byteswap(header.FileSize);
		header.BitmapDataOffset = os::Byteswap::byteswap(header.BitmapDataOffset);
		header.BitmapHeaderSize = os::Byteswap::byteswap(header.BitmapHeaderSize);
		header.Width = os::Byteswap::byteswap(header.Width);
		header.Height = os::Byteswap::byteswap(header.Height);
		header.Planes = os::Byteswap::byteswap(header.Planes);
		header.BPP = os::Byteswap::byteswap(header.BPP);
		header.Compression = os::Byteswap::byteswap(header.Compression);
		header.BitmapDataSize = os::Byteswap::byteswap(header.BitmapDataSize);
		header.PixelPerMeterX = os::Byteswap::byteswap(header.PixelPerMeterX);
		header.PixelPerMeterY = os::Byteswap::byteswap(header.PixelPerMeterY);
		header.Colors = os::Byteswap::byteswap(header.Colors);
		header.ImportantColors = os::Byteswap::byteswap(header.ImportantColors);
#endif

		SINT32 pitch = 0;

		//! return if the header is false

		if (header.Id != 0x4d42)
			return 0;

		if (header.Compression > 2) // we'll only handle RLE-Compression
		{
			Printer::log("Compression mode not supported.", LML_ERROR);
			return 0;
		}

		// adjust bitmap data size to dword boundary
		header.BitmapDataSize += (4 - (header.BitmapDataSize % 4)) % 4;

		// read palette

		long pos = file->getPos();
		SINT32 paletteSize = (header.BitmapDataOffset - pos) / 4;

		SINT32* paletteData = 0;
		if (paletteSize)
		{
			paletteData = new SINT32[paletteSize];
			file->read(paletteData, paletteSize * sizeof(SINT32));
#ifdef __BIG_ENDIAN__
			for (SINT32 i = 0; i<paletteSize; ++i)
				paletteData[i] = os::Byteswap::byteswap(paletteData[i]);
#endif
		}

		// read image data

		if (!header.BitmapDataSize)
		{
			// okay, lets guess the size
			// some tools simply don't set it
			header.BitmapDataSize = static_cast<UINT32>(file->getSize()) - header.BitmapDataOffset;
		}

		file->seek(header.BitmapDataOffset);

		FLOAT32 t = (header.Width) * (header.BPP / 8.0f);
		SINT32 widthInBytes = (SINT32)t;
		t -= widthInBytes;
		if (t != 0.0f)
			++widthInBytes;

		SINT32 lineData = widthInBytes + ((4 - (widthInBytes % 4))) % 4;
		pitch = lineData - widthInBytes;

		UINT8* bmpData = new UINT8[header.BitmapDataSize];
		file->read(bmpData, header.BitmapDataSize);

		// decompress data if needed
		switch (header.Compression)
		{
		case 1: // 8 bit rle
			decompress8BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
			break;
		case 2: // 4 bit rle
			decompress4BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
			break;
		}

		// create surface

		// no default constructor from packed area! ARM problem!
		dimension2d<UINT32> dim;
		dim.Width = header.Width;
		dim.Height = header.Height;

		IImage* image = 0;
		switch (header.BPP)
		{
		case 1:
			image = new CImage(ECF_A1R5G5B5, dim);
			if (image)
				CColorConverter::convert1BitTo16Bit(bmpData, (SINT16*)image->lock(), header.Width, header.Height, pitch, true);
			break;
		case 4:
			image = new CImage(ECF_A1R5G5B5, dim);
			if (image)
				CColorConverter::convert4BitTo16Bit(bmpData, (SINT16*)image->lock(), header.Width, header.Height, paletteData, pitch, true);
			break;
		case 8:
			image = new CImage(ECF_A1R5G5B5, dim);
			if (image)
				CColorConverter::convert8BitTo16Bit(bmpData, (SINT16*)image->lock(), header.Width, header.Height, paletteData, pitch, true);
			break;
		case 16:
			image = new CImage(ECF_A1R5G5B5, dim);
			if (image)
				CColorConverter::convert16BitTo16Bit((SINT16*)bmpData, (SINT16*)image->lock(), header.Width, header.Height, pitch, true);
			break;
		case 24:
			image = new CImage(ECF_R8G8B8, dim);
			if (image)
				CColorConverter::convert24BitTo24Bit(bmpData, (UINT8*)image->lock(), header.Width, header.Height, pitch, true, true);
			break;
		case 32: // thx to Reinhard Ostermeier
			image = new CImage(ECF_A8R8G8B8, dim);
			if (image)
				CColorConverter::convert32BitTo32Bit((SINT32*)bmpData, (SINT32*)image->lock(), header.Width, header.Height, pitch, true);
			break;
		};
		if (image)
			image->unlock();

		// clean up

		delete[] paletteData;
		delete[] bmpData;

		return image;
	}


	//! creates a loader which is able to load windows bitmaps
	IImageLoader* createImageLoaderBMP()
	{
		return new CImageLoaderBMP;
	}
}

#endif