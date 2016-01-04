#ifndef _SAPPHIRE_C_IMAGE_
#define _SAPPHIRE_C_IMAGE_

#include "SapphireIImage.h"
#include "SapphireRectangle.h"

namespace Sapphire
{
	// !IImage实现了许多对16位A1R5G5B5/32位A8R8G8B8图像的特殊操作，它用于软件设备
	class CImage : public IImage
	{
	public:

		//! 从原始图像数据构造
		/** \param useForeignMemory: 如果为true，这个图像将直接使用数据指针，意味着当图像数据析构时，它可以通过delete[]。如果false，创建一份内存拷贝*/
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size,
			void* data, bool ownForeignMemory = true, bool deleteMemory = true);

		//! 从空数据构造
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);

		//! 析构
		virtual ~CImage();

		//! 锁定函数
		virtual void* lock()
		{
			return Data;
		}

		//!解锁函数
		virtual void unlock() {}

		//! 返回这个图像的高度和宽度
		virtual const dimension2d<UINT32>& getDimension() const;

		//! 返回每个像素的位深
		virtual UINT32 getBitsPerPixel() const;

		//! 返回每个像素的字节
		virtual UINT32 getBytesPerPixel() const;

		//! 返回图像数据字节数
		virtual UINT32 getImageDataSizeInBytes() const;

		//! 返回图像的像素大小
		virtual UINT32 getImageDataSizeInPixels() const;

		//! 返回每个像素红色的掩码
		virtual UINT32 getRedMask() const;

		//! 返回每个像素绿色的掩码
		virtual UINT32 getGreenMask() const;

		//! 返回每个像素蓝色的掩码
		virtual UINT32 getBlueMask() const;

		//! 返回每个像素alpha的掩码
		virtual UINT32 getAlphaMask() const;

		//! 返回一个像素
		virtual ColourValue getPixel(UINT32 x, UINT32 y) const;

		//! 设置一个像素
		virtual void setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend = false);

		//! 返回一个颜色格式
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! 设置图像内存跨距
		virtual UINT32 getPitch() const { return Pitch; }

		//! 复制这个表面到另外一个，并缩放它修正
		virtual void copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format, UINT32 pitch = 0);

		//! 复制这个表面到另外一个，并缩放它修正
		virtual void copyToScaling(IImage* target);

		//! 复制这个表面到另外一个
		virtual void copyTo(IImage* target, const Vector2& pos = Vector2(0, 0));

		//! 复制这个表面到另外一个
		virtual void copyTo(IImage* target, const Vector2& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0);

		//! 复制这个表面到另外一个， 用alpha掩码，一个剪裁区和一个色彩来添加
		virtual void copyToWithAlpha(IImage* target, const Vector2& pos,
			const rect<SINT32>& sourceRect, const ColourValue &color,
			const rect<SINT32>* clipRect = 0);

		//! 复制这个表面到另外一个,缩放它修正，应用box滤波器
		virtual void copyToScalingBoxFilter(IImage* target, SINT32 bias = 0, bool blend = false);

		//! 用给定的颜色填充表面
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