#ifndef _SAPPHIRE_IIMAGELOADER_
#define _SAPPHIRE_IIMAGELOADER_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class IImage;

	//! 用于从一个文件创建一个图像的类
	/** 
	如果你想要引擎可以加载不能支持的文件类型（如gif），
	那么实现这个接口和你用添加IVideoDriver::addExternalImageLoader()添加到引擎的新的表面加载器。
    */
	class IImageLoader  :public virtual IReferenceCounter
	{
	public:

		//! 检查这个文件能否通过这个类加载
		/** 通过文件扩展名检查(e.g. ".tga")
		\param filename 要文件名
		\return 如果这个文件看起来可以加载 */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! 检查这个文件能否通过这个类加载
		/** 检查看起来可以加载这个文件
		\param file 要检查的文件句柄
		\return 如果文件看上去可以被加载true */
		virtual bool isALoadableFileFormat( IReadFile* file) const = 0;

		//! 从一个文件创建表面
		/** \param file 要检查文件句柄
		\return 指向新创建的图像， 0表示出现错误 */
		virtual IImage* loadImage( IReadFile* file) const = 0;
	};


}

#endif