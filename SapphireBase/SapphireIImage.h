#ifndef _SAPPHIRE_I_IMAGE_
#define _SAPPHIRE_I_IMAGE_

#include "SapphireIReferenceCounter.h"
#include "SapphireVector2.h"
#include "SapphireDimension2D.h"
#include "SapphireRectangle.h"
#include "SapphireColorValue.h"


namespace Sapphire
{
	//! 软件图像数据的接口
	/** 
	图像加载器通过文件创建这些图像数据。  IVideoDrivers 转换这些图像到硬件纹理中
	*/
	class IImage : public virtual IReferenceCounter
	{
	public:

		//! 锁定函数。会得到一个图像数据的指针
		/** 当你不需要使用这个指针时，你必须调用unlock().
		\return 指向图像数据的指针。数据的类型依赖于图像的颜色的格式。例如：这个颜色格式
		是ECF_A8R8G8B8， 它是UINT32. 
	  */
		virtual void* lock() = 0;

		//! 解锁函数
		/** 在lock()返回指针不再需要后调用 */
		virtual void unlock() = 0;

		//! 返回图像数据到宽度和高度
		virtual const dimension2d<UINT32>& getDimension() const = 0;

		//! 返回每个像素的位深
		virtual UINT32 getBitsPerPixel() const = 0;

		//! 返回每个像素的字节数
		virtual UINT32 getBytesPerPixel() const = 0;

		//!返回图像数据的大小（字节）
		virtual UINT32 getImageDataSizeInBytes() const = 0;

		//! 返回图像数据大小（像素）
		virtual UINT32 getImageDataSizeInPixels() const = 0;

		//! 返回一个像素颜色
		virtual ColourValue getPixel(UINT32 x, UINT32 y) const = 0;

		//! 设置一个像素
		virtual void setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend = false) = 0;

		//! 返回纹理格式
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! 返回一个像素的红色掩码
		virtual UINT32 getRedMask() const = 0;

		//! 返回一个像素的绿色掩码
		virtual UINT32 getGreenMask() const = 0;

		//! 返回一个像素的蓝色掩码
		virtual UINT32 getBlueMask() const = 0;

		//! 返回一个像素的alpha值掩码
		virtual UINT32 getAlphaMask() const = 0;

		//! 返回图像的行跨距
		virtual UINT32 getPitch() const = 0;

		//! 复制图像到目标，缩放图像进行修正
		virtual void copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format = ECF_A8R8G8B8, UINT32 pitch = 0) = 0;

		//!复制图像到目标，缩放图像进行修正
		virtual void copyToScaling(IImage* target) = 0;

		//! 复制这个表面到其它的
		virtual void copyTo(IImage* target, const Position2d& pos = Position2d(0, 0)) = 0;

		//! 复制这个表面到其它的
		virtual void copyTo(IImage* target, const Position2d& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0) = 0;

		//! 复制这个表面到其它的，添加一个颜色做alpha掩码并且进行区域剪裁
		virtual void copyToWithAlpha(IImage* target, const Position2d& pos,
			const rect<SINT32>& sourceRect, const ColourValue &color,
			const rect<SINT32>* clipRect = 0) = 0;

		//! 复制这个表面到其它的，缩放图像进行修正，应用box滤波器
		virtual void copyToScalingBoxFilter(IImage* target, SINT32 bias = 0, bool blend = false) = 0;

		//! 用给定的颜色填充表面
		virtual void fill(const ColourValue &color) = 0;

		//! 获取给定颜色格式每像素的位深度
		static UINT32 getBitsPerPixelFromFormat(const ECOLOR_FORMAT format)
		{
			switch (format)
			{
			case ECF_A1R5G5B5:
				return 16;
			case ECF_R5G6B5:
				return 16;
			case ECF_R8G8B8:
				return 24;
			case ECF_A8R8G8B8:
				return 32;
			case ECF_R16F:
				return 16;
			case ECF_G16R16F:
				return 32;
			case ECF_A16B16G16R16F:
				return 64;
			case ECF_R32F:
				return 32;
			case ECF_G32R32F:
				return 64;
			case ECF_A32B32G32R32F:
				return 128;
			default:
				return 0;
			}
		}

		//! 测试颜色格式是否是渲染目标纹理唯一可用的
		/** Since we don't have support for e.g. floating point IImage formats
		one should test if the color format can be used for arbitrary usage, or
		if it is restricted to RTTs. */
		static bool isRenderTargetOnlyFormat(const ECOLOR_FORMAT format)
		{
			switch (format)
			{
			case ECF_A1R5G5B5:
			case ECF_R5G6B5:
			case ECF_R8G8B8:
			case ECF_A8R8G8B8:
				return false;
			default:
				return true;
			}
		}

	};
}
#endif