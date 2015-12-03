#ifndef _SAPPHIRE_IIWRITER_
#define _SAPPHIRE_IIWRITER_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IWriteFile; 
	class IImage;

	//! 写入软件图像数据的接口
	class IImageWriter 
	{
	public:
		//! 通过一个给定扩展名检查Writer能否写入一个文件
		/** \param filename 文件名
		\return 如果文件扩展名是一个可写的类型 */
		virtual bool isAWriteableFileExtension(const  path& filename) const = 0;

		//! 写入图像到文件
		/** \param file 写入的文件句柄
		\param image 写入图像的文件
		\param param Writer特定的参数 （影响质量）
		\return 如果图像成功写入，返回true */
		virtual bool writeImage(IWriteFile *file, IImage *image, UINT32 param = 0) const = 0;
	};
}

#endif
