#include "SapphireCImage.h"
#include "SapphireString.h"
#include "SapphireQMath.h"
#include "SapphireSoftDriverHelper.h"
#include "SapphireCColourConverter.h"
#include "SapphireCBlit.h"

namespace Sapphire
{
	//! Constructor of empty image
	CImage::CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size)
		:Data(0), Size(size), Format(format), DeleteMemory(true)
	{
		initData();
	}


	//! Constructor from raw data
	CImage::CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size, void* data,
		bool ownForeignMemory, bool deleteForeignMemory)
		: Data(0), Size(size), Format(format), DeleteMemory(deleteForeignMemory)
	{
		if (ownForeignMemory)
		{
			Data = (UINT8*)0xbadf00d;
			initData();
			Data = (UINT8*)data;
		}
		else
		{
			Data = 0;
			initData();
			memcpy(Data, data, Size.Height * Pitch);
		}
	}


	//! assumes format and size has been set and creates the rest
	void CImage::initData()
	{
#ifdef _DEBUG
		setDebugName("CImage");
#endif
		BytesPerPixel = getBitsPerPixelFromFormat(Format) / 8;

		// Pitch should be aligned...
		Pitch = BytesPerPixel * Size.Width;

		if (!Data)
		{
			DeleteMemory = true;
			Data = new UINT8[Size.Height * Pitch];
		}
	}


	//! destructor
	CImage::~CImage()
	{
		if (DeleteMemory)
			delete[] Data;
	}


	//! Returns width and height of image data.
	const dimension2d<UINT32>& CImage::getDimension() const
	{
		return Size;
	}


	//! Returns bits per pixel.
	UINT32 CImage::getBitsPerPixel() const
	{
		return getBitsPerPixelFromFormat(Format);
	}


	//! Returns bytes per pixel
	UINT32 CImage::getBytesPerPixel() const
	{
		return BytesPerPixel;
	}


	//! Returns image data size in bytes
	UINT32 CImage::getImageDataSizeInBytes() const
	{
		return Pitch * Size.Height;
	}


	//! Returns image data size in pixels
	UINT32 CImage::getImageDataSizeInPixels() const
	{
		return Size.Width * Size.Height;
	}


	//! returns mask for red value of a pixel
	UINT32 CImage::getRedMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F << 10;
		case ECF_R5G6B5:
			return 0x1F << 11;
		case ECF_R8G8B8:
			return 0x00FF0000;
		case ECF_A8R8G8B8:
			return 0x00FF0000;
		default:
			return 0x0;
		}
	}


	//! returns mask for green value of a pixel
	UINT32 CImage::getGreenMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F << 5;
		case ECF_R5G6B5:
			return 0x3F << 5;
		case ECF_R8G8B8:
			return 0x0000FF00;
		case ECF_A8R8G8B8:
			return 0x0000FF00;
		default:
			return 0x0;
		}
	}


	//! returns mask for blue value of a pixel
	UINT32 CImage::getBlueMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F;
		case ECF_R5G6B5:
			return 0x1F;
		case ECF_R8G8B8:
			return 0x000000FF;
		case ECF_A8R8G8B8:
			return 0x000000FF;
		default:
			return 0x0;
		}
	}


	//! returns mask for alpha value of a pixel
	UINT32 CImage::getAlphaMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1 << 15;
		case ECF_R5G6B5:
			return 0x0;
		case ECF_R8G8B8:
			return 0x0;
		case ECF_A8R8G8B8:
			return 0xFF000000;
		default:
			return 0x0;
		}
	}


	//! sets a pixel
	void CImage::setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend)
	{
		ColourValue c = color;
		if (x >= Size.Width || y >= Size.Height)
			return;

		switch (Format)
		{
		case ECF_A1R5G5B5:
		{
			UINT16 * dest = (UINT16*)(Data + (y * Pitch) + (x << 1));
			*dest = A8R8G8B8toA1R5G5B5(color.getAsARGB());
		} break;

		case ECF_R5G6B5:
		{
			UINT16 * dest = (UINT16*)(Data + (y * Pitch) + (x << 1));
			*dest = A8R8G8B8toR5G6B5(color.getAsARGB());
		} break;

		case ECF_R8G8B8:
		{
			UINT8* dest = Data + (y * Pitch) + (x * 3);
			
			dest[0] = (UINT8)c.getRed();
			dest[1] = (UINT8)c.getGreen();
			dest[2] = (UINT8)c.getBlue();
		} break;

		case ECF_A8R8G8B8:
		{
			UINT32 * dest = (UINT32*)(Data + (y * Pitch) + (x << 2));
			*dest = blend ? PixelBlend32(*dest, color.getAsARGB()) : color.getAsARGB();
		} break;
#ifndef _DEBUG
		default:
			break;
#endif
		}
	}


	//! returns a pixel
	ColourValue CImage::getPixel(UINT32 x, UINT32 y) const
	{
		ColourValue c;
		if (x >= Size.Width || y >= Size.Height)
			return c;
		
		switch (Format)
		{
		case ECF_A1R5G5B5:
			c.setAsARGB(A1R5G5B5toA8R8G8B8(((UINT16*)Data)[y*Size.Width + x]));
			return c; //A1R5G5B5toA8R8G8B8(((UINT16*)Data)[y*Size.Width + x]);
		case ECF_R5G6B5:
			c.setAsARGB(R5G6B5toA8R8G8B8(((UINT16*)Data)[y*Size.Width + x]));
			return c;//R5G6B5toA8R8G8B8(((UINT16*)Data)[y*Size.Width + x]);
		case ECF_A8R8G8B8:
			c.setAsARGB(((UINT32*)Data)[y*Size.Width + x]);
			return c;// ((UINT32*)Data)[y*Size.Width + x];
		case ECF_R8G8B8:
		{
			UINT8* p = Data + (y * 3)*Size.Width + (x * 3);
			c.setAlpha(255);
			c.setRed(p[0]);
			c.setBlue(p[1]);
			c.setGreen(p[2]);
			return c;
		}
#ifndef _DEBUG
		default:
			break;
#endif
		}

		return ColourValue(0);
	}


	//! returns the color format
	ECOLOR_FORMAT CImage::getColorFormat() const
	{
		return Format;
	}


	//! copies this surface into another at given position
	void CImage::copyTo(IImage* target, const Vector2& pos)
	{
		Blit(BLITTER_TEXTURE, target, 0, &pos, this, 0, 0);
	}


	//! copies this surface partially into another at given position
	void CImage::copyTo(IImage* target, const Vector2& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect)
	{
		Blit(BLITTER_TEXTURE, target, clipRect, &pos, this, &sourceRect, 0);
	}


	//! copies this surface into another, using the alpha mask, a cliprect and a color to add with
	void CImage::copyToWithAlpha(IImage* target, const Vector2& pos, const rect<SINT32>& sourceRect, const ColourValue &color, const rect<SINT32>* clipRect)
	{
		// color blend only necessary on not full spectrum aka. color.color != 0xFFFFFFFF
		Blit(color.getAsARGB() == 0xFFFFFFFF ? BLITTER_TEXTURE_ALPHA_BLEND : BLITTER_TEXTURE_ALPHA_COLOR_BLEND,
			target, clipRect, &pos, this, &sourceRect, color.getAsARGB());
	}


	//! copies this surface into another, scaling it to the target image size
	// note: this is very very slow.
	void CImage::copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format, UINT32 pitch)
	{
		if (!target || !width || !height)
			return;

		const UINT32 bpp = getBitsPerPixelFromFormat(format) / 8;
		if (0 == pitch)
			pitch = width*bpp;

		if (Format == format && Size.Width == width && Size.Height == height)
		{
			if (pitch == Pitch)
			{
				memcpy(target, Data, height*pitch);
				return;
			}
			else
			{
				UINT8* tgtpos = (UINT8*)target;
				UINT8* srcpos = Data;
				const UINT32 bwidth = width*bpp;
				const UINT32 rest = pitch - bwidth;
				for (UINT32 y = 0; y<height; ++y)
				{
					// copy scanline
					memcpy(tgtpos, srcpos, bwidth);
					// clear pitch
					memset(tgtpos + bwidth, 0, rest);
					tgtpos += pitch;
					srcpos += Pitch;
				}
				return;
			}
		}

		const Real sourceXStep = (Real)Size.Width / (Real)width;
		const Real sourceYStep = (Real)Size.Height / (Real)height;
		SINT32 yval = 0, syval = 0;
		Real sy = 0.0f;
		for (UINT32 y = 0; y<height; ++y)
		{
			Real sx = 0.0f;
			for (UINT32 x = 0; x<width; ++x)
			{
				CColorConverter::convert_viaFormat(Data + syval + ((SINT32)sx)*BytesPerPixel, Format, 1, ((UINT8*)target) + yval + (x*bpp), format);
				sx += sourceXStep;
			}
			sy += sourceYStep;
			syval = ((SINT32)sy)*Pitch;
			yval += pitch;
		}
	}


	//! copies this surface into another, scaling it to the target image size
	// note: this is very very slow.
	void CImage::copyToScaling(IImage* target)
	{
		if (!target)
			return;

		const dimension2d<UINT32>& targetSize = target->getDimension();

		if (targetSize == Size)
		{
			copyTo(target);
			return;
		}

		copyToScaling(target->lock(), targetSize.Width, targetSize.Height, target->getColorFormat());
		target->unlock();
	}


	//! copies this surface into another, scaling it to fit it.
	void CImage::copyToScalingBoxFilter(IImage* target, SINT32 bias, bool blend)
	{
		const dimension2d<UINT32> destSize = target->getDimension();

		const Real sourceXStep = (Real)Size.Width / (Real)destSize.Width;
		const Real sourceYStep = (Real)Size.Height / (Real)destSize.Height;

		target->lock();

		SINT32 fx = ceil32(sourceXStep);
		SINT32 fy = ceil32(sourceYStep);
		Real sx;
		Real sy;

		sy = 0.f;
		for (UINT32 y = 0; y != destSize.Height; ++y)
		{
			sx = 0.f;
			for (UINT32 x = 0; x != destSize.Width; ++x)
			{
				target->setPixel(x, y,
					getPixelBox(floor32(sx), floor32(sy), fx, fy, bias), blend);
				sx += sourceXStep;
			}
			sy += sourceYStep;
		}

		target->unlock();
	}


	//! fills the surface with given color
	void CImage::fill(const ColourValue &color)
	{
		UINT32 c;

		switch (Format)
		{
		case ECF_A1R5G5B5:
			c = A8R8G8B8toA1R5G5B5(color.getAsARGB());
			c |= c << 16;
			break;
		case ECF_R5G6B5:
			c = A8R8G8B8toR5G6B5(color.getAsARGB());
			c |= c << 16;
			break;
		case ECF_A8R8G8B8:
			c = color.getAsARGB();
			break;
		case ECF_R8G8B8:
		{
			UINT8 rgb[3];
			CColorConverter::convert_A8R8G8B8toR8G8B8(&color, 1, rgb);
			const UINT32 size = getImageDataSizeInBytes();
			for (UINT32 i = 0; i<size; i += 3)
			{
				memcpy(Data + i, rgb, 3);
			}
			return;
		}
		break;
		default:
			// TODO: Handle other formats
			return;
		}
		memset32(Data, c, getImageDataSizeInBytes());
	}


	//! get a filtered pixel
	inline ColourValue CImage::getPixelBox(SINT32 x, SINT32 y, SINT32 fx, SINT32 fy, SINT32 bias) const
	{
		ColourValue c;
		SINT32 a = 0, r = 0, g = 0, b = 0;

		for (SINT32 dx = 0; dx != fx; ++dx)
		{
			for (SINT32 dy = 0; dy != fy; ++dy)
			{
				c = getPixel(SINT32_min(x + dx, Size.Width - 1),
					SINT32_min(y + dy, Size.Height - 1)
					);

				a += c.getAlpha();
				r += c.getRed();
				g += c.getGreen();
				b += c.getBlue();
			}

		}

		SINT32 sdiv = s32_log2_f32(fx * fy);

		a = SINT32_clamp((a >> sdiv) + bias, 0, 255);
		r = SINT32_clamp((r >> sdiv) + bias, 0, 255);
		g = SINT32_clamp((g >> sdiv) + bias, 0, 255);
		b = SINT32_clamp((b >> sdiv) + bias, 0, 255);

		c.setAlpha(a);
		c.setRed(r);
		c.setBlue(b);
		c.setGreen(g);
		//c.set(a, r, g, b);
		return c;
	}
}