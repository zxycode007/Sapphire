#include "SapphireCompileConifg.h"


static const char* const copyright = "Sapphire Engine (c)";


#ifdef _SAPPHIRE_WINDOWS_
#include <windows.h>
#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(_WIN32_WCE)
#include <crtdbg.h>
#endif // _DEBUG
#endif

#include "Sapphire.h"
#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
#include "SapphireDeviceW32.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_OSX_DEVICE_
#include "MacOSX/SapphireDeviceMacOSX.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_CE_DEVICE_
#include "SapphireDeviceWinCE.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
#include "SapphireDeviceLinux.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_SDL_DEVICE_
#include "SapphireDeviceSDL.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_FB_DEVICE_
#include "SapphireDeviceFB.h"
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_
#include "SapphireDeviceConsole.h"
#endif

namespace Sapphire
{
	//! stub for calling createDeviceEx
	_SapphireExport Device* SAPPHIRECALLCONV createDevice(E_DRIVER_TYPE driverType,
		const dimension2d<UINT32>& windowSize,
		UINT32 bits, bool fullscreen,
		bool stencilbuffer, bool vsync, IEventReceiver* res)
	{
		SSapphireCreationParameters p;
		p.DriverType = driverType;
		p.WindowSize = windowSize;
		p.Bits = (UINT8)bits;
		p.Fullscreen = fullscreen;
		p.Stencilbuffer = stencilbuffer;
		p.Vsync = vsync;
		p.EventReceiver = res;

		return createDeviceEx(p);
	}

	extern "C" _SapphireExport Device* SAPPHIRECALLCONV createDeviceEx(const SSapphireCreationParameters& params)
	{

		Device* dev = 0;

#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
		if (params.DeviceType == EIDT_WIN32 || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CSapphireDeviceWin32(params);
#ifdef SAPPHIRE_ALLOW_WIN32_CONSOLE
		if (AllocConsole())
		{
			freopen("CONOUT$", "w", stdout);
			printf("SAPPHIRE CONSOLE!\n");
		}
#endif
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_OSX_DEVICE_
		if (params.DeviceType == EIDT_OSX || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceMacOSX(params);
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_CE_DEVICE_
		if (params.DeviceType == EIDT_WINCE || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceWinCE(params);
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
		if (params.DeviceType == EIDT_X11 || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceLinux(params);
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_SDL_DEVICE_
		if (params.DeviceType == EIDT_SDL || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceSDL(params);
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_FB_DEVICE_
		if (params.DeviceType == EIDT_FRAMEBUFFER || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceFB(params);
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_
		if (params.DeviceType == EIDT_CONSOLE || (!dev && params.DeviceType == EIDT_BEST))
			dev = new CIrrDeviceConsole(params);
#endif

		if (dev && !dev->getVideoDriver() && params.DriverType != EDT_NULL)
		{
			dev->closeDevice(); // destroy window
			dev->run(); // consume quit message
			dev->drop();
			dev = 0;
		}

		return dev;
	}

	namespace core
	{
	
	}
	const Matrix4 IdentityMatrix(Matrix4::IDENTITY);
	String LOCALE_DECIMAL_POINTS(".");

	namespace video
	{
		
	}
	SMaterial IdentityMaterial;

} // end namespace irr


#if defined(_SAPPHIRE_WINDOWS_API_)

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	// _crtBreakAlloc = 139;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(__BORLANDC__) && !defined (_WIN32_WCE) && !defined (_SAPPHIRE_XBOX_PLATFORM_)
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // defined(_SAPPHIRE_WINDOWS_)