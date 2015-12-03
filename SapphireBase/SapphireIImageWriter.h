#ifndef _SAPPHIRE_IIWRITER_
#define _SAPPHIRE_IIWRITER_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IWriteFile; 
	class IImage;

	//! д�����ͼ�����ݵĽӿ�
	class IImageWriter 
	{
	public:
		//! ͨ��һ��������չ�����Writer�ܷ�д��һ���ļ�
		/** \param filename �ļ���
		\return ����ļ���չ����һ����д������ */
		virtual bool isAWriteableFileExtension(const  path& filename) const = 0;

		//! д��ͼ���ļ�
		/** \param file д����ļ����
		\param image д��ͼ����ļ�
		\param param Writer�ض��Ĳ��� ��Ӱ��������
		\return ���ͼ��ɹ�д�룬����true */
		virtual bool writeImage(IWriteFile *file, IImage *image, UINT32 param = 0) const = 0;
	};
}

#endif
