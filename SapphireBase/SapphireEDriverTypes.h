#ifndef _SAPPHIRE_EDRIVER_TYPES_
#define _SAPPHIRE_EDRIVER_TYPES_

namespace Sapphire
{
	//! 一个引擎支持的所有驱动类型
	enum E_DRIVER_TYPE
	{
		//! 无驱动，用于对应用程序非可视化的方式运行程序
		/** 没有可用的设备加载纹理，不能渲染和显示任何图像*/
		EDT_NULL,

		//! 软件渲染引擎
		/** 可运行在所有平台，任何硬件。它只能用于2d图像。它也可以应用一点简单的3d图元函数。
		这些三代绘制非常快，但非常不进去，并且不能支持3d剪裁
		*/
		EDT_SOFTWARE,

		//! Burning的软件渲染器, 一个另外一种软件渲染器 
		/** */
		EDT_BURNINGSVIDEO,

		//! Direct3D8 设备, 只可以在WIN32平台使用 
		/**  */
		EDT_DIRECT3D8,

		//! Direct3D9 设备, 只可以在WIN32平台使用   
		EDT_DIRECT3D9,

		//! OpenGL 设备, 可应用于最多的平台 
	 
		EDT_OPENGL,

		//驱动种类数
		EDT_COUNT
	};

}

#endif