#ifndef _SAPPHIRE_EXPOSED_VIDEO_DATA_
#define _SAPPHIRE_EXPOSED_VIDEO_DATA_

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3D8;
struct IDirect3DDevice8;


//! 这个结构保存描述一个驱动数据和操作系统指定数据
/** 这些数据能够通过IVideoDriver::getExposedVideoData()返回， 使用这个要谨慎。
这只是为了扩展引擎更容易而不用修改源码。注意，如果你用软件模拟和空设备 这个结构没有任何有效的数据。
*/
struct SExposedVideoData
{
	SExposedVideoData() { OpenGLWin32.HDc = 0; OpenGLWin32.HRc = 0; OpenGLWin32.HWnd = 0; }
	explicit SExposedVideoData(void* Window) { OpenGLWin32.HDc = 0; OpenGLWin32.HRc = 0; OpenGLWin32.HWnd = Window; }

	union
	{
		struct
		{
			//! Pointer to the IDirect3D9 interface
			IDirect3D9* D3D9;

			//! Pointer to the IDirect3DDevice9 interface
			IDirect3DDevice9* D3DDev9;

			//! Window handle.
			/** Get with for example HWND h = reinterpret_cast<HWND>(exposedData.D3D9.HWnd) */
			void* HWnd;

		} D3D9;

		struct
		{
			//! Pointer to the IDirect3D8 interface
			IDirect3D8* D3D8;

			//! Pointer to the IDirect3DDevice8 interface
			IDirect3DDevice8* D3DDev8;

			//! Window handle.
			/** Get with for example with: HWND h = reinterpret_cast<HWND>(exposedData.D3D8.HWnd) */
			void* HWnd;

		} D3D8;

		struct
		{
			//! Private GDI Device Context.
			/** Get if for example with: HDC h = reinterpret_cast<HDC>(exposedData.OpenGLWin32.HDc) */
			void* HDc;

			//! Permanent Rendering Context.
			/** Get if for example with: HGLRC h = reinterpret_cast<HGLRC>(exposedData.OpenGLWin32.HRc) */
			void* HRc;

			//! Window handle.
			/** Get with for example with: HWND h = reinterpret_cast<HWND>(exposedData.OpenGLWin32.HWnd) */
			void* HWnd;
		} OpenGLWin32;

		struct
		{
			// XWindow handles
			void* X11Display;
			void* X11Context;
			unsigned long X11Window;
		} OpenGLLinux;
	};
};

#endif