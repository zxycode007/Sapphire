#ifndef _SAPPHIRE_I_IMAGE_PRESENTER_
#define _SAPPHIRE_I_IMAGE_PRESENTER_

#include "SapphireIImage.h"

namespace Sapphire
{
	/*!
	这个接口类表示一个显示在平面上的图像。 对于软件渲染，通常只通过Device实现


	这个类应该只做内部使用
	*/

	class IImagePresenter
	{
	public:

		virtual ~IImagePresenter() {};
		//! 展示一个表面到客户端区域
		virtual bool present(IImage* surface, void* windowId = 0, rect<SINT32>* src = 0) = 0;
	};
}

#endif