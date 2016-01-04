#ifndef _SAPPHIRE_C_IMAGE_
#define _SAPPHIRE_C_IMAGE_

#include "SapphireIImage.h"
#include "SapphireRectangle.h"

namespace Sapphire
{
	// !IImageʵ��������16λA1R5G5B5/32λA8R8G8B8ͼ����������������������豸
	class CImage : public IImage
	{
	public:

		//! ��ԭʼͼ�����ݹ���
		/** \param useForeignMemory: ���Ϊtrue�����ͼ��ֱ��ʹ������ָ�룬��ζ�ŵ�ͼ����������ʱ��������ͨ��delete[]�����false������һ���ڴ濽��*/
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size,
			void* data, bool ownForeignMemory = true, bool deleteMemory = true);

		//! �ӿ����ݹ���
		CImage(ECOLOR_FORMAT format, const dimension2d<UINT32>& size);

		//! ����
		virtual ~CImage();

		//! ��������
		virtual void* lock()
		{
			return Data;
		}

		//!��������
		virtual void unlock() {}

		//! �������ͼ��ĸ߶ȺͿ��
		virtual const dimension2d<UINT32>& getDimension() const;

		//! ����ÿ�����ص�λ��
		virtual UINT32 getBitsPerPixel() const;

		//! ����ÿ�����ص��ֽ�
		virtual UINT32 getBytesPerPixel() const;

		//! ����ͼ�������ֽ���
		virtual UINT32 getImageDataSizeInBytes() const;

		//! ����ͼ������ش�С
		virtual UINT32 getImageDataSizeInPixels() const;

		//! ����ÿ�����غ�ɫ������
		virtual UINT32 getRedMask() const;

		//! ����ÿ��������ɫ������
		virtual UINT32 getGreenMask() const;

		//! ����ÿ��������ɫ������
		virtual UINT32 getBlueMask() const;

		//! ����ÿ������alpha������
		virtual UINT32 getAlphaMask() const;

		//! ����һ������
		virtual ColourValue getPixel(UINT32 x, UINT32 y) const;

		//! ����һ������
		virtual void setPixel(UINT32 x, UINT32 y, const ColourValue &color, bool blend = false);

		//! ����һ����ɫ��ʽ
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! ����ͼ���ڴ���
		virtual UINT32 getPitch() const { return Pitch; }

		//! ����������浽����һ����������������
		virtual void copyToScaling(void* target, UINT32 width, UINT32 height, ECOLOR_FORMAT format, UINT32 pitch = 0);

		//! ����������浽����һ����������������
		virtual void copyToScaling(IImage* target);

		//! ����������浽����һ��
		virtual void copyTo(IImage* target, const Vector2& pos = Vector2(0, 0));

		//! ����������浽����һ��
		virtual void copyTo(IImage* target, const Vector2& pos, const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect = 0);

		//! ����������浽����һ���� ��alpha���룬һ����������һ��ɫ�������
		virtual void copyToWithAlpha(IImage* target, const Vector2& pos,
			const rect<SINT32>& sourceRect, const ColourValue &color,
			const rect<SINT32>* clipRect = 0);

		//! ����������浽����һ��,������������Ӧ��box�˲���
		virtual void copyToScalingBoxFilter(IImage* target, SINT32 bias = 0, bool blend = false);

		//! �ø�������ɫ������
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