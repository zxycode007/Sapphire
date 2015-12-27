#ifndef _SAPPHIRE_I_IMAGE_
#define _SAPPHIRE_I_IMAGE_

#include "SapphireIReferenceCounter.h"
#include "SapphireVector2.h"
#include "SapphireDimension2D.h"
#include "SapphireRectangle.h"
#include "SapphireColorValue.h"


namespace Sapphire
{
	//! ���ͼ�����ݵĽӿ�
	/** 
	ͼ�������ͨ���ļ�������Щͼ�����ݡ�  IVideoDrivers ת����Щͼ��Ӳ��������
	*/
	class IImage : public virtual IReferenceCounter
	{
	public:

		//! ������������õ�һ��ͼ�����ݵ�ָ��
		/** ���㲻��Ҫʹ�����ָ��ʱ����������unlock().
		\return ָ��ͼ�����ݵ�ָ�롣���ݵ�����������ͼ�����ɫ�ĸ�ʽ�����磺�����ɫ��ʽ
		��ECF_A8R8G8B8�� ����UINT32. 
	  */
		virtual void* lock() = 0;

		//! ��������
		/** ��lock()����ָ�벻����Ҫ����� */
		virtual void unlock() = 0;

		//! ����ͼ�����ݵ���Ⱥ͸߶�
		virtual const dimension2d<UINT32>& getDimension() const = 0;

		//! ����ÿ�����ص�λ��
		virtual UINT32 getBitsPerPixel() const = 0;

		//! ����ÿ�����ص��ֽ���
		virtual UINT32 getBytesPerPixel() const = 0;

		//!����ͼ�����ݵĴ�С���ֽڣ�
		virtual UINT32 getImageDataSizeInBytes() const = 0;

		//! ����ͼ�����ݴ�С�����أ�
		virtual UINT32 getImageDataSizeInPixels() const = 0;

		//! ����һ��������ɫ
		virtual ColourValue getPixel(UINT32 x, UINT32 y) const = 0;

		//! ����һ������
		virtual void setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend = false) = 0;

		//! ���������ʽ
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! ����һ�����صĺ�ɫ����
		virtual UINT32 getRedMask() const = 0;

		//! ����һ�����ص���ɫ����
		virtual UINT32 getGreenMask() const = 0;

		//! ����һ�����ص���ɫ����
		virtual UINT32 getBlueMask() const = 0;

		//! ����һ�����ص�alphaֵ����
		virtual UINT32 getAlphaMask() const = 0;

		//! ����ͼ����п��
		virtual UINT32 getPitch() const = 0;

		//! ����ͼ��Ŀ�꣬����ͼ���������
		virtual void copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format = ECF_A8R8G8B8, UINT32 pitch = 0) = 0;

		//!����ͼ��Ŀ�꣬����ͼ���������
		virtual void copyToScaling(IImage* target) = 0;

		//! ����������浽������
		virtual void copyTo(IImage* target, const Position2d& pos = Position2d(0, 0)) = 0;

		//! ����������浽������
		virtual void copyTo(IImage* target, const Position2d& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0) = 0;

		//! ����������浽�����ģ����һ����ɫ��alpha���벢�ҽ����������
		virtual void copyToWithAlpha(IImage* target, const Position2d& pos,
			const rect<SINT32>& sourceRect, const ColourValue &color,
			const rect<SINT32>* clipRect = 0) = 0;

		//! ����������浽�����ģ�����ͼ�����������Ӧ��box�˲���
		virtual void copyToScalingBoxFilter(IImage* target, SINT32 bias = 0, bool blend = false) = 0;

		//! �ø�������ɫ������
		virtual void fill(const ColourValue &color) = 0;

		//! ��ȡ������ɫ��ʽÿ���ص�λ���
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

		//! ������ɫ��ʽ�Ƿ�����ȾĿ������Ψһ���õ�
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