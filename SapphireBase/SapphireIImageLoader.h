#ifndef _SAPPHIRE_IIMAGELOADER_
#define _SAPPHIRE_IIMAGELOADER_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class IImage;

	//! ���ڴ�һ���ļ�����һ��ͼ�����
	/** 
	�������Ҫ������Լ��ز���֧�ֵ��ļ����ͣ���gif����
	��ôʵ������ӿں��������IVideoDriver::addExternalImageLoader()��ӵ�������µı����������
    */
	class IImageLoader  :public virtual IReferenceCounter
	{
	public:

		//! �������ļ��ܷ�ͨ����������
		/** ͨ���ļ���չ�����(e.g. ".tga")
		\param filename Ҫ�ļ���
		\return �������ļ����������Լ��� */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! �������ļ��ܷ�ͨ����������
		/** ��鿴�������Լ�������ļ�
		\param file Ҫ�����ļ����
		\return ����ļ�����ȥ���Ա�����true */
		virtual bool isALoadableFileFormat( IReadFile* file) const = 0;

		//! ��һ���ļ���������
		/** \param file Ҫ����ļ����
		\return ָ���´�����ͼ�� 0��ʾ���ִ��� */
		virtual IImage* loadImage( IReadFile* file) const = 0;
	};


}

#endif