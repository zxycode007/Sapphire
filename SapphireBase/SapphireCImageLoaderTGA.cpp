#include "SapphireCImageLoaderTGA.h"
#ifdef SAPPHIRE_COMPILE_WITH_TGA_LOADER_
#include "SapphireIReadFile.h"
#include "SapphireColorValue.h"
#include "SapphireCColourConverter.h"	
#include "SapphireCImage.h"
#include "SapphireOS.h"
#include "SapphireString.h"


namespace Sapphire
{



	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	bool CImageLoaderTGA::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "tga");
	}


	//! loads a compressed tga.
	UINT8 *CImageLoaderTGA::loadCompressedImage(IReadFile *file, const STGAHeader& header) const
	{
		// This was written and sent in by Jon Pry, thank you very much!
		// I only changed the formatting a little bit.

		SINT32 bytesPerPixel = header.PixelDepth / 8;
		SINT32 imageSize = header.ImageHeight * header.ImageWidth * bytesPerPixel;
		UINT8* data = new UINT8[imageSize];
		SINT32 currentByte = 0;

		while (currentByte < imageSize)
		{
			UINT8 chunkheader = 0;
			file->read(&chunkheader, sizeof(UINT8)); // Read The Chunk's Header

			if (chunkheader < 128) // If The Chunk Is A 'RAW' Chunk
			{
				chunkheader++; // Add 1 To The Value To Get Total Number Of Raw Pixels

				file->read(&data[currentByte], bytesPerPixel * chunkheader);
				currentByte += bytesPerPixel * chunkheader;
			}
			else
			{
				// thnx to neojzs for some fixes with this code

				// If It's An RLE Header
				chunkheader -= 127; // Subtract 127 To Get Rid Of The ID Bit

				SINT32 dataOffset = currentByte;
				file->read(&data[dataOffset], bytesPerPixel);

				currentByte += bytesPerPixel;

				for (SINT32 counter = 1; counter < chunkheader; counter++)
				{
					for (SINT32 elementCounter = 0; elementCounter < bytesPerPixel; elementCounter++)
						data[currentByte + elementCounter] = data[dataOffset + elementCounter];

					currentByte += bytesPerPixel;
				}
			}
		}

		return data;
	}



	//! returns true if the file maybe is able to be loaded by this class
	bool CImageLoaderTGA::isALoadableFileFormat(IReadFile* file) const
	{
		if (!file)
			return false;

		STGAFooter footer;
		memset(&footer, 0, sizeof(STGAFooter));
		file->seek(file->getSize() - sizeof(STGAFooter));
		file->read(&footer, sizeof(STGAFooter));
		return (!strcmp(footer.Signature, "TRUEVISION-XFILE.")); // very old tgas are refused.
	}



	//! creates a surface from the file
	IImage* CImageLoaderTGA::loadImage(IReadFile* file) const
	{
		STGAHeader header;
		UINT32 *palette = 0;

		file->read(&header, sizeof(STGAHeader));

#ifdef __BIG_ENDIAN__
		header.ColorMapLength = os::Byteswap::byteswap(header.ColorMapLength);
		header.ImageWidth = os::Byteswap::byteswap(header.ImageWidth);
		header.ImageHeight = os::Byteswap::byteswap(header.ImageHeight);
#endif

		// skip image identification field
		if (header.IdLength)
			file->seek(header.IdLength, true);

		if (header.ColorMapType)
		{
			// create 32 bit palette
			palette = new UINT32[header.ColorMapLength];

			// read color map
			UINT8 * colorMap = new UINT8[header.ColorMapEntrySize / 8 * header.ColorMapLength];
			file->read(colorMap, header.ColorMapEntrySize / 8 * header.ColorMapLength);

			// convert to 32-bit palette
			switch (header.ColorMapEntrySize)
			{
			case 16:
				CColorConverter::convert_A1R5G5B5toA8R8G8B8(colorMap, header.ColorMapLength, palette);
				break;
			case 24:
				CColorConverter::convert_B8G8R8toA8R8G8B8(colorMap, header.ColorMapLength, palette);
				break;
			case 32:
				CColorConverter::convert_B8G8R8A8toA8R8G8B8(colorMap, header.ColorMapLength, palette);
				break;
			}
			delete[] colorMap;
		}

		// read image

		UINT8* data = 0;

		if (header.ImageType == 1 || // Uncompressed, color-mapped images.
			header.ImageType == 2 || // Uncompressed, RGB images
			header.ImageType == 3 // Uncompressed, black and white images
			)
		{
			const SINT32 imageSize = header.ImageHeight * header.ImageWidth * header.PixelDepth / 8;
			data = new UINT8[imageSize];
			file->read(data, imageSize);
		}
		else
			if (header.ImageType == 10)
			{
				// Runlength encoded RGB images
				data = loadCompressedImage(file, header);
			}
			else
			{
				Printer::log("Unsupported TGA file type", file->getFileName(), LML_ERROR);
				delete[] palette;
				return 0;
			}

		IImage* image = 0;

		switch (header.PixelDepth)
		{
		case 8:
		{
			if (header.ImageType == 3) // grey image
			{
				image = new CImage(ECF_R8G8B8,
					dimension2d<UINT32>(header.ImageWidth, header.ImageHeight));
				if (image)
					CColorConverter::convert8BitTo24Bit((UINT8*)data,
					(UINT8*)image->lock(),
					header.ImageWidth, header.ImageHeight,
					0, 0, (header.ImageDescriptor & 0x20) == 0);
			}
			else
			{
				image = new CImage(ECF_A1R5G5B5,
					dimension2d<UINT32>(header.ImageWidth, header.ImageHeight));
				if (image)
					CColorConverter::convert8BitTo16Bit((UINT8*)data,
					(SINT16*)image->lock(),
					header.ImageWidth, header.ImageHeight,
					(SINT32*)palette, 0,
					(header.ImageDescriptor & 0x20) == 0);
			}
		}
		break;
		case 16:
			image = new CImage(ECF_A1R5G5B5,
				dimension2d<UINT32>(header.ImageWidth, header.ImageHeight));
			if (image)
				CColorConverter::convert16BitTo16Bit((SINT16*)data,
				(SINT16*)image->lock(), header.ImageWidth, header.ImageHeight, 0, (header.ImageDescriptor & 0x20) == 0);
			break;
		case 24:
			image = new CImage(ECF_R8G8B8,
				dimension2d<UINT32>(header.ImageWidth, header.ImageHeight));
			if (image)
				CColorConverter::convert24BitTo24Bit(
				(UINT8*)data, (UINT8*)image->lock(), header.ImageWidth, header.ImageHeight, 0, (header.ImageDescriptor & 0x20) == 0, true);
			break;
		case 32:
			image = new CImage(ECF_A8R8G8B8,
				dimension2d<UINT32>(header.ImageWidth, header.ImageHeight));
			if (image)
				CColorConverter::convert32BitTo32Bit((SINT32*)data,
				(SINT32*)image->lock(), header.ImageWidth, header.ImageHeight, 0, (header.ImageDescriptor & 0x20) == 0);
			break;
		default:
			Printer::log("Unsupported TGA format", file->getFileName(), LML_ERROR);
			break;
		}
		if (image)
			image->unlock();

		delete[] data;
		delete[] palette;

		return image;
	}


	//! creates a loader which is able to load tgas
	IImageLoader* createImageLoaderTGA()
	{
		return new CImageLoaderTGA();
	}
}

#endif