#ifndef _SAPPHIRE_C_IMAGE_
#define _SAPPHIRE_C_IMAGE_

#include "SapphireIImage.h"
#include "SapphireRectangle.h"

namespace Sapphire
{
	// !IImage implementation with a lot of special image operations for
		//! 16 bit A1R5G5B5/32 Bit A8R8G8B8 images, which are used by the SoftwareDevice.
	class CImage : public IImage
	{
	public:

		//! constructor from raw image data
		/** \param useForeignMemory: If true, the image will use the data pointer
		directly and own it from now on, which means it will also try to delete [] the
		data when the image will be destructed. If false, the memory will by copied. */
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size,
			void* data, bool ownForeignMemory = true, bool deleteMemory = true);

		//! constructor for empty image
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);

		//! destructor
		virtual ~CImage();

		//! Lock function.
		virtual void* lock()
		{
			return Data;
		}

		//! Unlock function.
		virtual void unlock() {}

		//! Returns width and height of image data.
		virtual const dimension2d<UINT32>& getDimension() const;

		//! Returns bits per pixel.
		virtual UINT32 getBitsPerPixel() const;

		//! Returns bytes per pixel
		virtual UINT32 getBytesPerPixel() const;

		//! Returns image data size in bytes
		virtual UINT32 getImageDataSizeInBytes() const;

		//! Returns image data size in pixels
		virtual UINT32 getImageDataSizeInPixels() const;

		//! returns mask for red value of a pixel
		virtual UINT32 getRedMask() const;

		//! returns mask for green value of a pixel
		virtual UINT32 getGreenMask() const;

		//! returns mask for blue value of a pixel
		virtual UINT32 getBlueMask() const;

		//! returns mask for alpha value of a pixel
		virtual UINT32 getAlphaMask() const;

		//! returns a pixel
		virtual ColourValue getPixel(UINT32 x, UINT32 y) const;

		//! sets a pixel
		virtual void setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend = false);

		//! returns the color format
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! returns pitch of image
		virtual UINT32 getPitch() const { return Pitch; }

		//! copies this surface into another, scaling it to fit.
		virtual void copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format, UINT32 pitch = 0);

		//! copies this surface into another, scaling it to fit.
		virtual void copyToScaling(IImage* target);

		//! copies this surface into another
		virtual void copyTo(IImage* target, const Vector2& pos = Vector2(0, 0));

		//! copies this surface into another
		virtual void copyTo(IImage* target, const Vector2& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0);

		//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
		virtual void copyToWithAlpha(IImage* target, const Vector2& pos,
			const rect<SINT32>& sourceRect, const ColourValue &color,
			const rect<SINT32>* clipRect = 0);

		//! copies this surface into another, scaling it to fit, appyling a box filter
		virtual void copyToScalingBoxFilter(IImage* target, SINT32 bias = 0, bool blend = false);

		//! fills the surface with given color
		virtual void fill(const ColourValue &color);

	private:

		//! assumes format and size has been set and creates the rest
		void initData();

		inline ColourValue getPixelBox(SINT32 x, SINT32 y, SINT32 fx, SINT32 fy, SINT32 bias) const;

		UINT8* Data;
		dimension2d<UINT32> Size;
		UINT32 BytesPerPixel;
		UINT32 Pitch;
		ECOLOR_FORMAT Format;

		bool DeleteMemory;
	};
}

#endif