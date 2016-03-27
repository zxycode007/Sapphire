#include "SapphireCOpenGLDriver.h"
#include "SapphireCNullDriver.h"

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_
#include "SapphireCOpenGLTexture.h"
#include "SapphireCOpenGLMaterialRenderer.h"
#include "SapphireCOpenGLShaderMaterialRenderer.h"
#include "SapphireCOpenGLGLSLMaterialRenderer.h"
#include "SapphireCOpenGLCgRenderer.h"
#include "SapphireCOpenGLNormalMapRenderer.h"
#include "SapphireCOpenGLParallexMapRenderer.h"
#include "SapphireOS.h"

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include "MacOSX/CIrrDeviceMacOSX.h"
#endif

#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_SDL
#include <SDL/SDL.h>
#endif

namespace Sapphire
{

	

		// -----------------------------------------------------------------------
		// WINDOWS CONSTRUCTOR
		// -----------------------------------------------------------------------
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
		//! Windows constructor and init code
		COpenGLDriver::COpenGLDriver(const SSapphireCreationParameters& params,
			IFileSystem* io, CSapphireDeviceWin32* device)
			: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
			CurrentRenderMode(ERM_NONE), ResetRenderStates(true), Transformation3DChanged(true),
			AntiAlias(params.AntiAlias), RenderTargetTexture(0),
			CurrentRendertargetSize(0, 0), ColorFormat(ECF_R8G8B8),
			CurrentTarget(ERT_FRAME_BUFFER), Params(params),
			HDc(0), Window(static_cast<HWND>(params.WindowId)), Win32Device(device),
			DeviceType(EIDT_WIN32)
		{
#ifdef _DEBUG
			setDebugName("COpenGLDriver");
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			CgContext = 0;
#endif
		}


		bool COpenGLDriver::changeRenderContext(const SExposedVideoData& videoData, CSapphireDeviceWin32* device)
		{
			if (videoData.OpenGLWin32.HWnd && videoData.OpenGLWin32.HDc && videoData.OpenGLWin32.HRc)
			{
				if (!wglMakeCurrent((HDC)videoData.OpenGLWin32.HDc, (HGLRC)videoData.OpenGLWin32.HRc))
				{
					Printer::log("Render Context switch failed.");
					return false;
				}
				else
				{
					HDc = (HDC)videoData.OpenGLWin32.HDc;
				}
			}
			// set back to main context
			else if (HDc != ExposedData.OpenGLWin32.HDc)
			{
				if (!wglMakeCurrent((HDC)ExposedData.OpenGLWin32.HDc, (HGLRC)ExposedData.OpenGLWin32.HRc))
				{
					Printer::log("Render Context switch failed.");
					return false;
				}
				else
				{
					HDc = (HDC)ExposedData.OpenGLWin32.HDc;
				}
			}
			return true;
		}

		//! inits the open gl driver
		bool COpenGLDriver::initDriver(CSapphireDeviceWin32* device)
		{
			// Create a window to test antialiasing support
			const fschar_t* ClassName = __TEXT("GLCSapphireDeviceWin32");
			HINSTANCE lhInstance = GetModuleHandle(0);

			// Register Class
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = (WNDPROC)DefWindowProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = lhInstance;
			wcex.hIcon = NULL;
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = 0;
			wcex.lpszClassName = ClassName;
			wcex.hIconSm = 0;
			wcex.hIcon = 0;
			RegisterClassEx(&wcex);

			RECT clientSize;
			clientSize.top = 0;
			clientSize.left = 0;
			clientSize.right = Params.WindowSize.Width;
			clientSize.bottom = Params.WindowSize.Height;

			DWORD style = WS_POPUP;
			if (!Params.Fullscreen)
				style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

			AdjustWindowRect(&clientSize, style, FALSE);

			const SINT32 realWidth = clientSize.right - clientSize.left;
			const SINT32 realHeight = clientSize.bottom - clientSize.top;

			const SINT32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
			const SINT32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

			HWND temporary_wnd = CreateWindow(ClassName, __TEXT(""), style, windowLeft,
				windowTop, realWidth, realHeight, NULL, NULL, lhInstance, NULL);

			if (!temporary_wnd)
			{
				Printer::log("Cannot create a temporary window.", LML_ERROR);
				UnregisterClass(ClassName, lhInstance);
				return false;
			}

			HDc = GetDC(temporary_wnd);

			// Set up pixel format descriptor with desired parameters
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),             // Size Of This Pixel Format Descriptor
				1,                                         // Version Number
				PFD_DRAW_TO_WINDOW |                       // Format Must Support Window
				PFD_SUPPORT_OPENGL |                       // Format Must Support OpenGL
				(Params.Doublebuffer ? PFD_DOUBLEBUFFER : 0) | // Must Support Double Buffering
				(Params.Stereobuffer ? PFD_STEREO : 0),        // Must Support Stereo Buffer
				PFD_TYPE_RGBA,                             // Request An RGBA Format
				Params.Bits,                               // Select Our Color Depth
				0, 0, 0, 0, 0, 0,                          // Color Bits Ignored
				0,                                         // No Alpha Buffer
				0,                                         // Shift Bit Ignored
				0,                                         // No Accumulation Buffer
				0, 0, 0, 0,	                               // Accumulation Bits Ignored
				Params.ZBufferBits,                        // Z-Buffer (Depth Buffer)
				BYTE(Params.Stencilbuffer ? 1 : 0),        // Stencil Buffer Depth
				0,                                         // No Auxiliary Buffer
				PFD_MAIN_PLANE,                            // Main Drawing Layer
				0,                                         // Reserved
				0, 0, 0                                    // Layer Masks Ignored
			};

			GLuint PixelFormat;

			for (UINT32 i = 0; i<6; ++i)
			{
				if (i == 1)
				{
					if (Params.Stencilbuffer)
					{
						Printer::log("Cannot create a GL device with stencil buffer, disabling stencil shadows.", LML_WARNING);
						Params.Stencilbuffer = false;
						pfd.cStencilBits = 0;
					}
					else
						continue;
				}
				else
					if (i == 2)
					{
						pfd.cDepthBits = 24;
					}
					else
						if (i == 3)
						{
							if (Params.Bits != 16)
								pfd.cDepthBits = 16;
							else
								continue;
						}
						else
							if (i == 4)
							{
								// try single buffer
								if (Params.Doublebuffer)
									pfd.dwFlags &= ~PFD_DOUBLEBUFFER;
								else
									continue;
							}
							else
								if (i == 5)
								{
									Printer::log("Cannot create a GL device context", "No suitable format for temporary window.", LML_ERROR);
									ReleaseDC(temporary_wnd, HDc);
									DestroyWindow(temporary_wnd);
									UnregisterClass(ClassName, lhInstance);
									return false;
								}

				// choose pixelformat
				PixelFormat = ChoosePixelFormat(HDc, &pfd);
				if (PixelFormat)
					break;
			}

			SetPixelFormat(HDc, PixelFormat, &pfd);
			HGLRC hrc = wglCreateContext(HDc);
			if (!hrc)
			{
				Printer::log("Cannot create a temporary GL rendering context.", LML_ERROR);
				ReleaseDC(temporary_wnd, HDc);
				DestroyWindow(temporary_wnd);
				UnregisterClass(ClassName, lhInstance);
				return false;
			}

			SExposedVideoData data;
			data.OpenGLWin32.HDc = HDc;
			data.OpenGLWin32.HRc = hrc;
			data.OpenGLWin32.HWnd = temporary_wnd;


			if (!changeRenderContext(data, device))
			{
				Printer::log("Cannot activate a temporary GL rendering context.", LML_ERROR);
				wglDeleteContext(hrc);
				ReleaseDC(temporary_wnd, HDc);
				DestroyWindow(temporary_wnd);
				UnregisterClass(ClassName, lhInstance);
				return false;
			}

			String wglExtensions;
#ifdef WGL_ARB_extensions_string
			PFNWGLGETEXTENSIONSSTRINGARBPROC irrGetExtensionsString = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
			if (irrGetExtensionsString)
				wglExtensions = irrGetExtensionsString(HDc);
#elif defined(WGL_EXT_extensions_string)
			PFNWGLGETEXTENSIONSSTRINGEXTPROC irrGetExtensionsString = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
			if (irrGetExtensionsString)
				wglExtensions = irrGetExtensionsString(HDc);
#endif
			const bool pixel_format_supported = (wglExtensions.find("WGL_ARB_pixel_format") != -1);
			const bool multi_sample_supported = ((wglExtensions.find("WGL_ARB_multisample") != -1) ||
				(wglExtensions.find("WGL_EXT_multisample") != -1) || (wglExtensions.find("WGL_3DFX_multisample") != -1));
#ifdef _DEBUG
			Printer::log("WGL_extensions", wglExtensions);
#endif

#ifdef WGL_ARB_pixel_format
			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat_ARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			if (pixel_format_supported && wglChoosePixelFormat_ARB)
			{
				// This value determines the number of samples used for antialiasing
				// My experience is that 8 does not show a big
				// improvement over 4, but 4 shows a big improvement
				// over 2.

				if (AntiAlias > 32)
					AntiAlias = 32;

				FLOAT32 fAttributes[] = { 0.0, 0.0 };
				SINT32 iAttributes[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, 1,
					WGL_SUPPORT_OPENGL_ARB, 1,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB, (Params.Bits == 32) ? 24 : 15,
					WGL_ALPHA_BITS_ARB, (Params.Bits == 32) ? 8 : 1,
					WGL_DEPTH_BITS_ARB, Params.ZBufferBits, // 10,11
					WGL_STENCIL_BITS_ARB, Params.Stencilbuffer ? 1 : 0,
					WGL_DOUBLE_BUFFER_ARB, Params.Doublebuffer ? 1 : 0,
					WGL_STEREO_ARB, Params.Stereobuffer ? 1 : 0,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
#ifdef WGL_ARB_multisample
					WGL_SAMPLES_ARB, AntiAlias, // 20,21
					WGL_SAMPLE_BUFFERS_ARB, 1,
#elif defined(WGL_EXT_multisample)
					WGL_SAMPLES_EXT, AntiAlias, // 20,21
					WGL_SAMPLE_BUFFERS_EXT, 1,
#elif defined(WGL_3DFX_multisample)
					WGL_SAMPLES_3DFX, AntiAlias, // 20,21
					WGL_SAMPLE_BUFFERS_3DFX, 1,
#endif
#ifdef WGL_ARB_framebuffer_sRGB
					WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, Params.HandleSRGB ? 1 : 0,
#elif defined(WGL_EXT_framebuffer_sRGB)
					WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT, Params.HandleSRGB ? 1 : 0,
#endif
					//			WGL_DEPTH_FLOAT_EXT, 1,
					0, 0, 0, 0
				};
				int iAttrSize = sizeof(iAttributes) / sizeof(int);
				const bool framebuffer_srgb_supported = ((wglExtensions.find("WGL_ARB_framebuffer_sRGB") != -1) ||
					(wglExtensions.find("WGL_EXT_framebuffer_sRGB") != -1));
				if (!framebuffer_srgb_supported)
				{
					memmove(&iAttributes[24], &iAttributes[26], sizeof(int)*(iAttrSize - 26));
					iAttrSize -= 2;
				}
				if (!multi_sample_supported)
				{
					memmove(&iAttributes[20], &iAttributes[24], sizeof(int)*(iAttrSize - 24));
					iAttrSize -= 4;
				}

				SINT32 rv = 0;
				// Try to get an acceptable pixel format
				do
				{
					int pixelFormat = 0;
					UINT numFormats = 0;
					const BOOL valid = wglChoosePixelFormat_ARB(HDc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);

					if (valid && numFormats)
						rv = pixelFormat;
					else
						iAttributes[21] -= 1;
				} while (rv == 0 && iAttributes[21]>1);
				if (rv)
				{
					PixelFormat = rv;
					AntiAlias = iAttributes[21];
				}
			}
			else
#endif
				AntiAlias = 0;

			wglMakeCurrent(HDc, NULL);
			wglDeleteContext(hrc);
			ReleaseDC(temporary_wnd, HDc);
			DestroyWindow(temporary_wnd);
			UnregisterClass(ClassName, lhInstance);

			// get hdc
			HDc = GetDC(Window);
			if (!HDc)
			{
				Printer::log("Cannot create a GL device context.", LML_ERROR);
				return false;
			}

			// search for pixel format the simple way
			if (PixelFormat == 0 || (!SetPixelFormat(HDc, PixelFormat, &pfd)))
			{
				for (UINT32 i = 0; i<5; ++i)
				{
					if (i == 1)
					{
						if (Params.Stencilbuffer)
						{
							Printer::log("Cannot create a GL device with stencil buffer, disabling stencil shadows.", LML_WARNING);
							Params.Stencilbuffer = false;
							pfd.cStencilBits = 0;
						}
						else
							continue;
					}
					else
						if (i == 2)
						{
							pfd.cDepthBits = 24;
						}
					if (i == 3)
					{
						if (Params.Bits != 16)
							pfd.cDepthBits = 16;
						else
							continue;
					}
					else
						if (i == 4)
						{
							Printer::log("Cannot create a GL device context", "No suitable format.", LML_ERROR);
							return false;
						}

					// choose pixelformat
					PixelFormat = ChoosePixelFormat(HDc, &pfd);
					if (PixelFormat)
						break;
				}

				// set pixel format
				if (!SetPixelFormat(HDc, PixelFormat, &pfd))
				{
					Printer::log("Cannot set the pixel format.", LML_ERROR);
					return false;
				}
			}

			Printer::log("Pixel Format", StringUtil::int32ToString(PixelFormat).c_str(), LML_DEBUG);

			// create rendering context
#ifdef WGL_ARB_create_context
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs_ARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribs_ARB)
			{
				int iAttribs[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
					WGL_CONTEXT_MINOR_VERSION_ARB, 1,
					0
				};
				hrc = wglCreateContextAttribs_ARB(HDc, 0, iAttribs);
			}
			else
#endif
				hrc = wglCreateContext(HDc);

			if (!hrc)
			{
				Printer::log("Cannot create a GL rendering context.", LML_ERROR);
				return false;
			}

			// set exposed data
			ExposedData.OpenGLWin32.HDc = HDc;
			ExposedData.OpenGLWin32.HRc = hrc;
			ExposedData.OpenGLWin32.HWnd = Window;

			// activate rendering context

			if (!changeRenderContext(ExposedData, device))
			{
				Printer::log("Cannot activate GL rendering context", LML_ERROR);
				wglDeleteContext(hrc);
				return false;
			}

			int pf = GetPixelFormat(HDc);
			DescribePixelFormat(HDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
			if (pfd.cAlphaBits != 0)
			{
				if (pfd.cRedBits == 8)
					ColorFormat = ECF_A8R8G8B8;
				else
					ColorFormat = ECF_A1R5G5B5;
			}
			else
			{
				if (pfd.cRedBits == 8)
					ColorFormat = ECF_R8G8B8;
				else
					ColorFormat = ECF_R5G6B5;
			}

			genericDriverInit();

			extGlSwapInterval(Params.Vsync ? 1 : 0);
			return true;
		}

#endif // _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_

		// -----------------------------------------------------------------------
		// MacOSX CONSTRUCTOR
		// -----------------------------------------------------------------------
#ifdef _SAPPHIRE_COMPILE_WITH_OSX_DEVICE_
		//! Windows constructor and init code
		COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
			IFileSystem* io, CIrrDeviceMacOSX *device)
			: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
			CurrentRenderMode(ERM_NONE), ResetRenderStates(true), Transformation3DChanged(true),
			AntiAlias(params.AntiAlias), RenderTargetTexture(0),
			CurrentRendertargetSize(0, 0), ColorFormat(ECF_R8G8B8),
			CurrentTarget(ERT_FRAME_BUFFER), Params(params),
			OSXDevice(device), DeviceType(EIDT_OSX)
		{
#ifdef _DEBUG
			setDebugName("COpenGLDriver");
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			CgContext = 0;
#endif

			genericDriverInit();
		}

#endif

		// -----------------------------------------------------------------------
		// LINUX CONSTRUCTOR
		// -----------------------------------------------------------------------
#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
		//! Linux constructor and init code
		COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
			IFileSystem* io, CIrrDeviceLinux* device)
			: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
			CurrentRenderMode(ERM_NONE), ResetRenderStates(true),
			Transformation3DChanged(true), AntiAlias(params.AntiAlias),
			RenderTargetTexture(0), CurrentRendertargetSize(0, 0), ColorFormat(ECF_R8G8B8),
			CurrentTarget(ERT_FRAME_BUFFER), Params(params),
			X11Device(device), DeviceType(EIDT_X11)
		{
#ifdef _DEBUG
			setDebugName("COpenGLDriver");
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			CgContext = 0;
#endif
		}


		bool COpenGLDriver::changeRenderContext(const SExposedVideoData& videoData, CIrrDeviceLinux* device)
		{
			if (videoData.OpenGLLinux.X11Window)
			{
				if (videoData.OpenGLLinux.X11Display && videoData.OpenGLLinux.X11Context)
				{
					if (!glXMakeCurrent((Display*)videoData.OpenGLLinux.X11Display, videoData.OpenGLLinux.X11Window, (GLXContext)videoData.OpenGLLinux.X11Context))
					{
						Printer::log("Render Context switch failed.");
						return false;
					}
					else
					{
						Drawable = videoData.OpenGLLinux.X11Window;
						X11Display = (Display*)videoData.OpenGLLinux.X11Display;
					}
				}
				else
				{
					// in case we only got a window ID, try with the existing values for display and context
					if (!glXMakeCurrent((Display*)ExposedData.OpenGLLinux.X11Display, videoData.OpenGLLinux.X11Window, (GLXContext)ExposedData.OpenGLLinux.X11Context))
					{
						Printer::log("Render Context switch failed.");
						return false;
					}
					else
					{
						Drawable = videoData.OpenGLLinux.X11Window;
						X11Display = (Display*)ExposedData.OpenGLLinux.X11Display;
					}
				}
			}
			// set back to main context
			else if (X11Display != ExposedData.OpenGLLinux.X11Display)
			{
				if (!glXMakeCurrent((Display*)ExposedData.OpenGLLinux.X11Display, ExposedData.OpenGLLinux.X11Window, (GLXContext)ExposedData.OpenGLLinux.X11Context))
				{
					Printer::log("Render Context switch failed.");
					return false;
				}
				else
				{
					Drawable = ExposedData.OpenGLLinux.X11Window;
					X11Display = (Display*)ExposedData.OpenGLLinux.X11Display;
				}
			}
			return true;
		}


		//! inits the open gl driver
		bool COpenGLDriver::initDriver(CIrrDeviceLinux* device)
		{
			ExposedData.OpenGLLinux.X11Context = glXGetCurrentContext();
			ExposedData.OpenGLLinux.X11Display = glXGetCurrentDisplay();
			ExposedData.OpenGLLinux.X11Window = (unsigned long)Params.WindowId;
			Drawable = glXGetCurrentDrawable();
			X11Display = (Display*)ExposedData.OpenGLLinux.X11Display;

			genericDriverInit();

			// set vsync
			extGlSwapInterval(Params.Vsync ? 1 : 0);
			return true;
		}

#endif // _SAPPHIRE_COMPILE_WITH_X11_DEVICE_


		// -----------------------------------------------------------------------
		// SDL CONSTRUCTOR
		// -----------------------------------------------------------------------
#ifdef _SAPPHIRE_COMPILE_WITH_SDL_DEVICE_
		//! SDL constructor and init code
		COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
			IFileSystem* io, CIrrDeviceSDL* device)
			: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
			CurrentRenderMode(ERM_NONE), ResetRenderStates(true),
			Transformation3DChanged(true), AntiAlias(params.AntiAlias),
			RenderTargetTexture(0), CurrentRendertargetSize(0, 0), ColorFormat(ECF_R8G8B8),
			CurrentTarget(ERT_FRAME_BUFFER), Params(params),
			SDLDevice(device), DeviceType(EIDT_SDL)
		{
#ifdef _DEBUG
			setDebugName("COpenGLDriver");
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			CgContext = 0;
#endif

			genericDriverInit();
		}

#endif // _SAPPHIRE_COMPILE_WITH_SDL_DEVICE_


		//! destructor
		COpenGLDriver::~COpenGLDriver()
		{
#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			if (CgContext)
				cgDestroyContext(CgContext);
#endif

			RequestedLights.clear();

			deleteMaterialRenders();

			CurrentTexture.clear();
			// I get a blue screen on my laptop, when I do not delete the
			// textures manually before releasing the dc. Oh how I love this.
			deleteAllTextures();
			removeAllOcclusionQueries();
			removeAllHardwareBuffers();

#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
			if (DeviceType == EIDT_WIN32)
			{

				if (ExposedData.OpenGLWin32.HRc)
				{
					if (!wglMakeCurrent(HDc, 0))
						Printer::log("Release of dc and rc failed.", LML_WARNING);

					if (!wglDeleteContext((HGLRC)ExposedData.OpenGLWin32.HRc))
						Printer::log("Release of rendering context failed.", LML_WARNING);
				}

				if (HDc)
					ReleaseDC(Window, HDc);
			}
#endif
		}

		// -----------------------------------------------------------------------
		// METHODS
		// -----------------------------------------------------------------------

		bool COpenGLDriver::genericDriverInit()
		{
			Name = L"OpenGL ";
			//Name.append(glGetString(GL_VERSION));
			Name.append(StringUtil::StringToStringW((const c8*)glGetString(GL_VERSION)));
			SINT32 pos = Name.find(L' ', 7);
			if (pos != -1)
				Name = Name.substr(0, pos);
			printVersion();

			// print renderer information
			const GLubyte* renderer = glGetString(GL_RENDERER);
			const GLubyte* vendor = glGetString(GL_VENDOR);
			if (renderer && vendor)
			{
				Printer::log(reinterpret_cast<const c8*>(renderer), reinterpret_cast<const c8*>(vendor), LML_NORMAL);
				VendorName = reinterpret_cast<const c8*>(vendor);
			}

			UINT32 i;
			CurrentTexture.clear();
			// load extensions
			initExtensions(Params.Stencilbuffer);
			if (queryFeature(EVDF_ARB_GLSL))
			{
				char buf[32];
				const UINT32 maj = ShaderLanguageVersion / 100;
				snprintf(buf, 32, "%u.%u", maj, ShaderLanguageVersion - maj * 100);
				Printer::log("GLSL version", buf, LML_NORMAL);
			}
			else
				Printer::log("GLSL not available.", LML_NORMAL);
			DriverAttributes->setAttribute("MaxTextures", MaxTextureUnits);
			DriverAttributes->setAttribute("MaxSupportedTextures", MaxSupportedTextures);
			DriverAttributes->setAttribute("MaxLights", MaxLights);
			DriverAttributes->setAttribute("MaxAnisotropy", MaxAnisotropy);
			DriverAttributes->setAttribute("MaxUserClipPlanes", MaxUserClipPlanes);
			DriverAttributes->setAttribute("MaxAuxBuffers", MaxAuxBuffers);
			DriverAttributes->setAttribute("MaxMultipleRenderTargets", MaxMultipleRenderTargets);
			DriverAttributes->setAttribute("MaxIndices", (SINT32)MaxIndices);
			DriverAttributes->setAttribute("MaxTextureSize", (SINT32)MaxTextureSize);
			DriverAttributes->setAttribute("MaxGeometryVerticesOut", (SINT32)MaxGeometryVerticesOut);
			DriverAttributes->setAttribute("MaxTextureLODBias", MaxTextureLODBias);
			DriverAttributes->setAttribute("Version", Version);
			DriverAttributes->setAttribute("ShaderLanguageVersion", ShaderLanguageVersion);
			DriverAttributes->setAttribute("AntiAlias", AntiAlias);

			glPixelStorei(GL_PACK_ALIGNMENT, 1);

			// Reset The Current Viewport
			glViewport(0, 0, Params.WindowSize.Width, Params.WindowSize.Height);

			//UserClipPlanes.reallocate(MaxUserClipPlanes);
			UserClipPlanes.reserve(MaxUserClipPlanes);
			for (i = 0; i<MaxUserClipPlanes; ++i)
				UserClipPlanes.push_back(SUserClipPlane());

			for (i = 0; i<ETS_COUNT; ++i)
				//setTransform(static_cast<E_TRANSFORMATION_STATE>(i), IdentityMatrix);
				setTransform(static_cast<E_TRANSFORMATION_STATE>(i), Matrix4::IDENTITY);

			setAmbientLight(ColourValue(0.0f, 0.0f, 0.0f, 0.0f));
#ifdef GL_EXT_separate_specular_color
			if (FeatureAvailable[SAPPHIRE_EXT_separate_specular_color])
				glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

			Params.HandleSRGB &= ((FeatureAvailable[SAPPHIRE_ARB_framebuffer_sRGB] || FeatureAvailable[SAPPHIRE_EXT_framebuffer_sRGB]) &&
				FeatureAvailable[SAPPHIRE_EXT_texture_sRGB]);
#if defined(GL_ARB_framebuffer_sRGB)
			if (Params.HandleSRGB)
				glEnable(GL_FRAMEBUFFER_SRGB);
#elif defined(GL_EXT_framebuffer_sRGB)
			if (Params.HandleSRGB)
				glEnable(GL_FRAMEBUFFER_SRGB_EXT);
#endif

			// This is a fast replacement for NORMALIZE_NORMALS
			//	if ((Version>101) || FeatureAvailable[SAPPHIRE_EXT_rescale_normal])
			//		glEnable(GL_RESCALE_NORMAL_EXT);

			glClearDepth(1.0);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
			glDepthFunc(GL_LEQUAL);
			glFrontFace(GL_CW);
			// adjust flat coloring scheme to DirectX version
#if defined(GL_ARB_provoking_vertex) || defined(GL_EXT_provoking_vertex)
			extGlProvokingVertex(GL_FIRST_VERTEX_CONVENTION_EXT);
#endif

			// create material renderers
			createMaterialRenderers();

			// set the renderstates
			setRenderStates3DMode();

			glAlphaFunc(GL_GREATER, 0.f);

			// set fog mode
			setFog(FogColor, FogType, FogStart, FogEnd, FogDensity, PixelFog, RangeFog);

			// create matrix for flipping textures
			TextureFlipMatrix.buildTextureTransform(0.0f, Vector2(0, 0), Vector2(0, 1.0f), Vector2(1.0f, -1.0f));

			// We need to reset once more at the beginning of the first rendering.
			// This fixes problems with intermediate changes to the material during texture load.
			ResetRenderStates = true;

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			CgContext = cgCreateContext();
#endif

			return true;
		}


		void COpenGLDriver::createMaterialRenderers()
		{
			// create OpenGL material renderers

			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_SOLID(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_SOLID_2_LAYER(this));

			// add the same renderer for all lightmap types
			COpenGLMaterialRenderer_LIGHTMAP* lmr = new COpenGLMaterialRenderer_LIGHTMAP(this);
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_ADD:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M2:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M4:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M2:
			addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M4:
			lmr->drop();

			// add remaining material renderer
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_DETAIL_MAP(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_SPHERE_MAP(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_REFLECTION_2_LAYER(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(this));
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(this));

			// add normal map renderers
			SINT32 tmp = 0;
			IMaterialRenderer* renderer = 0;
			renderer = (IMaterialRenderer*)new COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
			renderer->drop();
			renderer = (IMaterialRenderer*) new COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
			renderer->drop();
			renderer = (IMaterialRenderer*) new COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
			renderer->drop();

			// add parallax map renderers
			renderer = (IMaterialRenderer*)new COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
			renderer->drop();
			renderer = (IMaterialRenderer*)new COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
			renderer->drop();
			renderer = (IMaterialRenderer*)new COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
			renderer->drop();

			// add basic 1 texture blending
			addAndDropMaterialRenderer(new COpenGLMaterialRenderer_ONETEXTURE_BLEND(this));
		}


		//! presents the rendered scene on the screen, returns false if failed
		bool COpenGLDriver::endScene()
		{
			CNullDriver::endScene();

			glFlush();

#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
			if (DeviceType == EIDT_WIN32)
				return SwapBuffers(HDc) == TRUE;
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
			if (DeviceType == EIDT_X11)
			{
				glXSwapBuffers(X11Display, Drawable);
				return true;
			}
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_OSX_DEVICE_
			if (DeviceType == EIDT_OSX)
			{
				OSXDevice->flush();
				return true;
			}
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_SDL_DEVICE_
			if (DeviceType == EIDT_SDL)
			{
				SDL_GL_SwapBuffers();
				return true;
			}
#endif

			// todo: console device present

			return false;
		}


		//! clears the zbuffer and color buffer
		void COpenGLDriver::clearBuffers(bool backBuffer, bool zBuffer, bool stencilBuffer, ColourValue
 color)
		{
			GLbitfield mask = 0;
			if (backBuffer)
			{
				const FLOAT32 inv = 1.0f / 255.0f;
				glClearColor(color.getRed() * inv, color.getGreen() * inv,
					color.getBlue() * inv, color.getAlpha() * inv);

				mask |= GL_COLOR_BUFFER_BIT;
			}

			if (zBuffer)
			{
				glDepthMask(GL_TRUE);
				LastMaterial.ZWriteEnable = true;
				mask |= GL_DEPTH_BUFFER_BIT;
			}

			if (stencilBuffer)
				mask |= GL_STENCIL_BUFFER_BIT;

			if (mask)
				glClear(mask);
		}


		//! init call for rendering start
		bool COpenGLDriver::beginScene(bool backBuffer, bool zBuffer, ColourValue color,
			const SExposedVideoData& videoData, rect<SINT32>* sourceRect)
		{
			CNullDriver::beginScene(backBuffer, zBuffer, color, videoData, sourceRect);

			switch (DeviceType)
			{
#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
			case EIDT_WIN32:
				changeRenderContext(videoData, Win32Device);
				break;
#endif
#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
			case EIDT_X11:
				changeRenderContext(videoData, X11Device);
				break;
#endif
			default:
				changeRenderContext(videoData, (void*)0);
				break;
			}

#if defined(_SAPPHIRE_COMPILE_WITH_SDL_DEVICE_)
			if (DeviceType == EIDT_SDL)
			{
				// todo: SDL sets glFrontFace(GL_CCW) after driver creation,
				// it would be better if this was fixed elsewhere.
				glFrontFace(GL_CW);
			}
#endif

			clearBuffers(backBuffer, zBuffer, false, color);
			return true;
		}


		//! Returns the transformation set by setTransform
		const Matrix4& COpenGLDriver::getTransform(E_TRANSFORMATION_STATE state) const
		{
			return Matrices[state];
		}


		//! sets transformation
		void COpenGLDriver::setTransform(E_TRANSFORMATION_STATE state, const Matrix4& mat)
		{
			Matrices[state] = mat;
			Transformation3DChanged = true;

			switch (state)
			{
			case ETS_VIEW:
			case ETS_WORLD:
			{
				// OpenGL only has a model matrix, view and world is not existent. so lets fake these two.
				glMatrixMode(GL_MODELVIEW);

				// first load the viewing transformation for user clip planes
				glLoadMatrixf((Matrices[ETS_VIEW]).pointer());

				// we have to update the clip planes to the latest view matrix
				for (UINT32 i = 0; i<MaxUserClipPlanes; ++i)
					if (UserClipPlanes[i].Enabled)
						uploadClipPlane(i);

				// now the real model-view matrix
				glMultMatrixf(Matrices[ETS_WORLD].pointer());
			}
			break;
			case ETS_PROJECTION:
			{
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(mat.cpointer());
			}
			break;
			case ETS_COUNT:
				return;
			default:
			{
				const UINT32 i = state - ETS_TEXTURE_0;
				if (i >= MATERIAL_MAX_TEXTURES)
					break;

				const bool isRTT = Material.getTexture(i) && Material.getTexture(i)->isRenderTarget();

				if (MultiTextureExtension)
					extGlActiveTexture(GL_TEXTURE0_ARB + i);

				glMatrixMode(GL_TEXTURE);
				if (!isRTT && mat.isIdentify())
					glLoadIdentity();
				else
				{
					GLfloat glmat[16];
					if (isRTT)
						getGLTextureMatrix(glmat, mat * TextureFlipMatrix);
					else
						getGLTextureMatrix(glmat, mat);
					glLoadMatrixf(glmat);
				}
				break;
			}
			}
		}


		bool COpenGLDriver::updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer)
		{
			if (!HWBuffer)
				return false;

			if (!FeatureAvailable[SAPPHIRE_ARB_vertex_buffer_object])
				return false;

#if defined(GL_ARB_vertex_buffer_object)
			const IMeshBuffer* mb = HWBuffer->MeshBuffer;
			const void* vertices = mb->getVertices();
			const UINT32 vertexCount = mb->getVertexCount();
			const E_VERTEX_TYPE vType = mb->getVertexType();
			const UINT32 vertexSize = getVertexPitchFromType(vType);

			const c8* vbuf = static_cast<const c8*>(vertices);
			vector<c8>::type buffer;
			if (!FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] && !FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra])
			{
				//buffer vertex data, and convert colors...
				//buffer.set_used(vertexSize * vertexCount);
				buffer.resize(vertexSize * vertexCount);
				memcpy(buffer.begin()._Ptr, vertices, vertexSize * vertexCount);
				vbuf = buffer.cbegin()._Ptr;

				// in order to convert the colors into opengl format (RGBA)
				switch (vType)
				{
				case EVT_STANDARD:
				{
					S3DVertex* pb = reinterpret_cast<S3DVertex*>(buffer.begin()._Ptr);
					const S3DVertex* po = static_cast<const S3DVertex*>(vertices);
					for (UINT32 i = 0; i<vertexCount; i++)
					{
						po[i].Color.toOpenGLColor((UINT8*)&(pb[i].Color));
					}
				}
				break;
				case EVT_2TCOORDS:
				{
					S3DVertex2TCoords* pb = reinterpret_cast<S3DVertex2TCoords*>(buffer.begin()._Ptr);
					const S3DVertex2TCoords* po = static_cast<const S3DVertex2TCoords*>(vertices);
					for (UINT32 i = 0; i<vertexCount; i++)
					{
						po[i].Color.toOpenGLColor((UINT8*)&(pb[i].Color));
					}
				}
				break;
				case EVT_TANGENTS:
				{
					S3DVertexTangents* pb = reinterpret_cast<S3DVertexTangents*>(buffer.begin()._Ptr);
					const S3DVertexTangents* po = static_cast<const S3DVertexTangents*>(vertices);
					for (UINT32 i = 0; i<vertexCount; i++)
					{
						po[i].Color.toOpenGLColor((UINT8*)&(pb[i].Color));
					}
				}
				break;
				default:
				{
					return false;
				}
				}
			}

			//get or create buffer
			bool newBuffer = false;
			if (!HWBuffer->vbo_verticesID)
			{
				extGlGenBuffers(1, &HWBuffer->vbo_verticesID);
				if (!HWBuffer->vbo_verticesID)
					return false;
				newBuffer = true;
			}
			else if (HWBuffer->vbo_verticesSize < vertexCount*vertexSize)
			{
				newBuffer = true;
			}

			extGlBindBuffer(GL_ARRAY_BUFFER, HWBuffer->vbo_verticesID);

			//copy data to graphics card
			glGetError(); // clear error storage
			if (!newBuffer)
				extGlBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * vertexSize, vbuf);
			else
			{
				HWBuffer->vbo_verticesSize = vertexCount*vertexSize;

				if (HWBuffer->Mapped_Vertex == EHM_STATIC)
					extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vbuf, GL_STATIC_DRAW);
				else if (HWBuffer->Mapped_Vertex == EHM_DYNAMIC)
					extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vbuf, GL_DYNAMIC_DRAW);
				else //EHM_STREAM
					extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vbuf, GL_STREAM_DRAW);
			}

			extGlBindBuffer(GL_ARRAY_BUFFER, 0);

			return (glGetError() == GL_NO_ERROR);
#else
			return false;
#endif
		}


		bool COpenGLDriver::updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer)
		{
			if (!HWBuffer)
				return false;

			if (!FeatureAvailable[SAPPHIRE_ARB_vertex_buffer_object])
				return false;

#if defined(GL_ARB_vertex_buffer_object)
			const IMeshBuffer* mb = HWBuffer->MeshBuffer;

			const void* indices = mb->getIndices();
			UINT32 indexCount = mb->getIndexCount();

			GLenum indexSize;
			switch (mb->getIndexType())
			{
			case EIT_16BIT:
			{
				indexSize = sizeof(UINT16);
				break;
			}
			case EIT_32BIT:
			{
				indexSize = sizeof(UINT32);
				break;
			}
			default:
			{
				return false;
			}
			}


			//get or create buffer
			bool newBuffer = false;
			if (!HWBuffer->vbo_indicesID)
			{
				extGlGenBuffers(1, &HWBuffer->vbo_indicesID);
				if (!HWBuffer->vbo_indicesID)
					return false;
				newBuffer = true;
			}
			else if (HWBuffer->vbo_indicesSize < indexCount*indexSize)
			{
				newBuffer = true;
			}

			extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HWBuffer->vbo_indicesID);

			//copy data to graphics card
			glGetError(); // clear error storage
			if (!newBuffer)
				extGlBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexCount * indexSize, indices);
			else
			{
				HWBuffer->vbo_indicesSize = indexCount*indexSize;

				if (HWBuffer->Mapped_Index == EHM_STATIC)
					extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_STATIC_DRAW);
				else if (HWBuffer->Mapped_Index == EHM_DYNAMIC)
					extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_DYNAMIC_DRAW);
				else //EHM_STREAM
					extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_STREAM_DRAW);
			}

			extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			return (glGetError() == GL_NO_ERROR);
#else
			return false;
#endif
		}


		//! updates hardware buffer if needed
		bool COpenGLDriver::updateHardwareBuffer(SHWBufferLink *HWBuffer)
		{
			if (!HWBuffer)
				return false;

			if (HWBuffer->Mapped_Vertex != EHM_NEVER)
			{
				if (HWBuffer->ChangedID_Vertex != HWBuffer->MeshBuffer->getChangedID_Vertex()
					|| !((SHWBufferLink_opengl*)HWBuffer)->vbo_verticesID)
				{

					HWBuffer->ChangedID_Vertex = HWBuffer->MeshBuffer->getChangedID_Vertex();

					if (!updateVertexHardwareBuffer((SHWBufferLink_opengl*)HWBuffer))
						return false;
				}
			}

			if (HWBuffer->Mapped_Index != EHM_NEVER)
			{
				if (HWBuffer->ChangedID_Index != HWBuffer->MeshBuffer->getChangedID_Index()
					|| !((SHWBufferLink_opengl*)HWBuffer)->vbo_indicesID)
				{

					HWBuffer->ChangedID_Index = HWBuffer->MeshBuffer->getChangedID_Index();

					if (!updateIndexHardwareBuffer((SHWBufferLink_opengl*)HWBuffer))
						return false;
				}
			}

			return true;
		}


		//! Create hardware buffer from meshbuffer
		COpenGLDriver::SHWBufferLink *COpenGLDriver::createHardwareBuffer(const IMeshBuffer* mb)
		{
#if defined(GL_ARB_vertex_buffer_object)
			if (!mb || (mb->getHardwareMappingHint_Index() == EHM_NEVER && mb->getHardwareMappingHint_Vertex() == EHM_NEVER))
				return 0;

			SHWBufferLink_opengl *HWBuffer = new SHWBufferLink_opengl(mb);

			//add to map
			//HWBufferMap.insert(HWBuffer->MeshBuffer, HWBuffer);
			HWBufferMap.insert(pair<const IMeshBuffer*, SHWBufferLink* >(HWBuffer->MeshBuffer, HWBuffer));
			

			HWBuffer->ChangedID_Vertex = HWBuffer->MeshBuffer->getChangedID_Vertex();
			HWBuffer->ChangedID_Index = HWBuffer->MeshBuffer->getChangedID_Index();
			HWBuffer->Mapped_Vertex = mb->getHardwareMappingHint_Vertex();
			HWBuffer->Mapped_Index = mb->getHardwareMappingHint_Index();
			HWBuffer->LastUsed = 0;
			HWBuffer->vbo_verticesID = 0;
			HWBuffer->vbo_indicesID = 0;
			HWBuffer->vbo_verticesSize = 0;
			HWBuffer->vbo_indicesSize = 0;

			if (!updateHardwareBuffer(HWBuffer))
			{
				deleteHardwareBuffer(HWBuffer);
				return 0;
			}

			return HWBuffer;
#else
			return 0;
#endif
		}


		void COpenGLDriver::deleteHardwareBuffer(SHWBufferLink *_HWBuffer)
		{
			if (!_HWBuffer)
				return;

#if defined(GL_ARB_vertex_buffer_object)
			SHWBufferLink_opengl *HWBuffer = (SHWBufferLink_opengl*)_HWBuffer;
			if (HWBuffer->vbo_verticesID)
			{
				extGlDeleteBuffers(1, &HWBuffer->vbo_verticesID);
				HWBuffer->vbo_verticesID = 0;
			}
			if (HWBuffer->vbo_indicesID)
			{
				extGlDeleteBuffers(1, &HWBuffer->vbo_indicesID);
				HWBuffer->vbo_indicesID = 0;
			}
#endif

			CNullDriver::deleteHardwareBuffer(_HWBuffer);
		}


		//! Draw hardware buffer
		void COpenGLDriver::drawHardwareBuffer(SHWBufferLink *_HWBuffer)
		{
			if (!_HWBuffer)
				return;

			updateHardwareBuffer(_HWBuffer); //check if update is needed
			_HWBuffer->LastUsed = 0; //reset count

#if defined(GL_ARB_vertex_buffer_object)
			SHWBufferLink_opengl *HWBuffer = (SHWBufferLink_opengl*)_HWBuffer;

			const IMeshBuffer* mb = HWBuffer->MeshBuffer;
			const void *vertices = mb->getVertices();
			const void *indexList = mb->getIndices();

			if (HWBuffer->Mapped_Vertex != EHM_NEVER)
			{
				extGlBindBuffer(GL_ARRAY_BUFFER, HWBuffer->vbo_verticesID);
				vertices = 0;
			}

			if (HWBuffer->Mapped_Index != EHM_NEVER)
			{
				extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HWBuffer->vbo_indicesID);
				indexList = 0;
			}

			drawVertexPrimitiveList(vertices, mb->getVertexCount(), indexList, mb->getIndexCount() / 3, mb->getVertexType(), EPT_TRIANGLES, mb->getIndexType());

			if (HWBuffer->Mapped_Vertex != EHM_NEVER)
				extGlBindBuffer(GL_ARRAY_BUFFER, 0);
			if (HWBuffer->Mapped_Index != EHM_NEVER)
				extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
		}


		//! Create occlusion query.
		/** Use node for identification and mesh for occlusion test. */
		void COpenGLDriver::addOcclusionQuery(ISceneNode* node,
			const IMesh* mesh)
		{
			if (!queryFeature(EVDF_OCCLUSION_QUERY))
				return;

			CNullDriver::addOcclusionQuery(node, mesh);
			//const SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
			vector<SOccQuery>::const_iterator index = find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
			if ((index != OcclusionQueries.end()) && (index->UID == 0))//if ((index != -1) && (OcclusionQueries[index].UID == 0))
				extGlGenQueries(1, reinterpret_cast<GLuint*>(index->UID));//extGlGenQueries(1, reinterpret_cast<GLuint*>(&OcclusionQueries[index].UID));
		}


		//! Remove occlusion query.
		void COpenGLDriver::removeOcclusionQuery(ISceneNode* node)
		{
			//const SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
			vector<SOccQuery>::const_iterator index = find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
			if (index != OcclusionQueries.end())//if (index != -1)
			{
				if (index->UID != 0)//if (OcclusionQueries[index].UID != 0)
					extGlDeleteQueries(1, reinterpret_cast<GLuint*>(index->UID));//extGlDeleteQueries(1, reinterpret_cast<GLuint*>(&OcclusionQueries[index].UID));
				CNullDriver::removeOcclusionQuery(node);
			}
		}


		//! Run occlusion query. Draws mesh stored in query.
		/** If the mesh shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		void COpenGLDriver::runOcclusionQuery(ISceneNode* node, bool visible)
		{
			if (!node)
				return;

			//const SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
			vector<SOccQuery>::const_iterator index = find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
			if (index != OcclusionQueries.begin())
			{
				if (index->UID)//if (OcclusionQueries[index].UID)
					extGlBeginQuery(
#ifdef GL_ARB_occlusion_query
					GL_SAMPLES_PASSED_ARB,
#else
					0,
#endif
					index->UID);
				CNullDriver::runOcclusionQuery(node, visible);
				if (index->UID)
					extGlEndQuery(
#ifdef GL_ARB_occlusion_query
					GL_SAMPLES_PASSED_ARB);
#else
					0);
#endif
				testGLError();
			}
		}


		//! Update occlusion query. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		void COpenGLDriver::updateOcclusionQuery(ISceneNode* node, bool block)
		{
			//const SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
			vector<SOccQuery>:: iterator index = find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
			if (index != OcclusionQueries.end())
			{
				// not yet started
				if (index->Run == UINT32(~0))
					return;
				GLint available = block ? GL_TRUE : GL_FALSE;
				if (!block)
					extGlGetQueryObjectiv(index->UID,
#ifdef GL_ARB_occlusion_query
					GL_QUERY_RESULT_AVAILABLE_ARB,
#elif defined(GL_NV_occlusion_query)
					GL_PIXEL_COUNT_AVAILABLE_NV,
#else
					0,
#endif
					&available);
				testGLError();
				if (available == GL_TRUE)
				{
					extGlGetQueryObjectiv(index->UID,
#ifdef GL_ARB_occlusion_query
						GL_QUERY_RESULT_ARB,
#elif defined(GL_NV_occlusion_query)
						GL_PIXEL_COUNT_NV,
#else
						0,
#endif
						&available);
					if (queryFeature(EVDF_OCCLUSION_QUERY))
						index->Result = available;
				}
				testGLError();
			}
		}


		//! Return query result.
		/** Return value is the number of visible pixels/fragments.
		The value is a safe approximation, i.e. can be larger than the
		actual value of pixels. */
		UINT32 COpenGLDriver::getOcclusionQueryResult(ISceneNode* node) const
		{
			//const SINT32 index = OcclusionQueries.linear_search(SOccQuery(node));
			vector<SOccQuery>::const_iterator index = find(OcclusionQueries.begin(), OcclusionQueries.end(), SOccQuery(node));
			if (index != OcclusionQueries.begin())
				return index->Result;
			else
				return ~0;
		}


		// small helper function to create vertex buffer object adress offsets
		static inline UINT8* buffer_offset(const long offset)
		{
			return ((UINT8*)0 + offset);
		}


		//! draws a vertex primitive list
		void COpenGLDriver::drawVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
		{
			if (!primitiveCount || !vertexCount)
				return;

			if (!checkPrimitiveCount(primitiveCount))
				return;

			CNullDriver::drawVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType);

			if (vertices && !FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] && !FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra])
				getColorBuffer(vertices, vertexCount, vType);

			// draw everything
			setRenderStates3DMode();

			if (MultiTextureExtension)
				extGlClientActiveTexture(GL_TEXTURE0_ARB);

			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			if ((pType != EPT_POINTS) && (pType != EPT_POINT_SPRITES))
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if ((pType != EPT_POINTS) && (pType != EPT_POINT_SPRITES))
				glEnableClientState(GL_NORMAL_ARRAY);

			//due to missing defines in OSX headers, we have to be more specific with this check
			//#if defined(GL_ARB_vertex_array_bgra) || defined(GL_EXT_vertex_array_bgra)
#ifdef GL_BGRA
			const GLint colorSize = (FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] || FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra]) ? GL_BGRA : 4;
#else
			const GLint colorSize = 4;
#endif
			if (vertices)
			{
				if (FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] || FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra])
				{
					switch (vType)
					{
					case EVT_STANDARD:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Color);
						break;
					case EVT_2TCOORDS:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Color);
						break;
					case EVT_TANGENTS:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Color);
						break;
					}
				}
				else
				{
					// avoid passing broken pointer to OpenGL
					assert(ColorBuffer.size() == 0);
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, 0, &ColorBuffer[0]);
				}
			}

			switch (vType)
			{
			case EVT_STANDARD:
				if (vertices)
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Pos);
				}
				else
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex), buffer_offset(12));
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex), 0);
				}

				if (MultiTextureExtension && CurrentTexture[1])
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
				}
				break;
			case EVT_2TCOORDS:
				if (vertices)
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Pos);
				}
				else
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(12));
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex2TCoords), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(0));
				}


				if (MultiTextureExtension)
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords2);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(36));
				}
				break;
			case EVT_TANGENTS:
				if (vertices)
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Pos);
				}
				else
				{
					glNormalPointer(GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(12));
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertexTangents), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(0));
				}

				if (MultiTextureExtension)
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Tangent);
					else
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(36));

					extGlClientActiveTexture(GL_TEXTURE2_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Binormal);
					else
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(48));
				}
				break;
			}

			renderArray(indexList, primitiveCount, pType, iType);

			if (MultiTextureExtension)
			{
				if (vType == EVT_TANGENTS)
				{
					extGlClientActiveTexture(GL_TEXTURE2_ARB);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				if ((vType != EVT_STANDARD) || CurrentTexture[1])
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				extGlClientActiveTexture(GL_TEXTURE0_ARB);
			}
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}


		void COpenGLDriver::getColorBuffer(const void* vertices, UINT32 vertexCount, E_VERTEX_TYPE vType)
		{
			// convert colors to gl color format.
			vertexCount *= 4; //reused as color component count
			//ColorBuffer.set_used(vertexCount);
			ColorBuffer.resize(vertexCount);
			UINT32 i;

			switch (vType)
			{
			case EVT_STANDARD:
			{
				const S3DVertex* p = static_cast<const S3DVertex*>(vertices);
				for (i = 0; i<vertexCount; i += 4)
				{
					p->Color.toOpenGLColor(&ColorBuffer[i]);
					++p;
				}
			}
			break;
			case EVT_2TCOORDS:
			{
				const S3DVertex2TCoords* p = static_cast<const S3DVertex2TCoords*>(vertices);
				for (i = 0; i<vertexCount; i += 4)
				{
					p->Color.toOpenGLColor(&ColorBuffer[i]);
					++p;
				}
			}
			break;
			case EVT_TANGENTS:
			{
				const S3DVertexTangents* p = static_cast<const S3DVertexTangents*>(vertices);
				for (i = 0; i<vertexCount; i += 4)
				{
					p->Color.toOpenGLColor(&ColorBuffer[i]);
					++p;
				}
			}
			break;
			}
		}


		void COpenGLDriver::renderArray(const void* indexList, UINT32 primitiveCount,
			E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
		{
			GLenum indexSize = 0;

			switch (iType)
			{
			case EIT_16BIT:
			{
				indexSize = GL_UNSIGNED_SHORT;
				break;
			}
			case EIT_32BIT:
			{
				indexSize = GL_UNSIGNED_INT;
				break;
			}
			}

			switch (pType)
			{
			case EPT_POINTS:
			case EPT_POINT_SPRITES:
			{
#ifdef GL_ARB_point_sprite
				if (pType == EPT_POINT_SPRITES && FeatureAvailable[SAPPHIRE_ARB_point_sprite])
					glEnable(GL_POINT_SPRITE_ARB);
#endif

				// prepare size and attenuation (where supported)
				GLfloat particleSize = Material.Thickness;
				//			if (AntiAlias)
				//				particleSize=clamp(particleSize, DimSmoothedPoint[0], DimSmoothedPoint[1]);
				//			else
				particleSize = Math::Clamp(particleSize, DimAliasedPoint[0], DimAliasedPoint[1]);
#if defined(GL_VERSION_1_4) || defined(GL_ARB_point_parameters) || defined(GL_EXT_point_parameters) || defined(GL_SGIS_point_parameters)
				const float att[] = { 1.0f, 1.0f, 0.0f };
#if defined(GL_VERSION_1_4)
				extGlPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, att);
				//			extGlPointParameterf(GL_POINT_SIZE_MIN,1.f);
				extGlPointParameterf(GL_POINT_SIZE_MAX, particleSize);
				extGlPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 1.0f);
#elif defined(GL_ARB_point_parameters)
				extGlPointParameterfv(GL_POINT_DISTANCE_ATTENUATION_ARB, att);
				//			extGlPointParameterf(GL_POINT_SIZE_MIN_ARB,1.f);
				extGlPointParameterf(GL_POINT_SIZE_MAX_ARB, particleSize);
				extGlPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 1.0f);
#elif defined(GL_EXT_point_parameters)
				extGlPointParameterfv(GL_DISTANCE_ATTENUATION_EXT, att);
				//			extGlPointParameterf(GL_POINT_SIZE_MIN_EXT,1.f);
				extGlPointParameterf(GL_POINT_SIZE_MAX_EXT, particleSize);
				extGlPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE_EXT, 1.0f);
#elif defined(GL_SGIS_point_parameters)
				extGlPointParameterfv(GL_DISTANCE_ATTENUATION_SGIS, att);
				//			extGlPointParameterf(GL_POINT_SIZE_MIN_SGIS,1.f);
				extGlPointParameterf(GL_POINT_SIZE_MAX_SGIS, particleSize);
				extGlPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE_SGIS, 1.0f);
#endif
#endif
				glPointSize(particleSize);

#ifdef GL_ARB_point_sprite
				if (pType == EPT_POINT_SPRITES && FeatureAvailable[SAPPHIRE_ARB_point_sprite])
					glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE, GL_TRUE);
#endif
				glDrawArrays(GL_POINTS, 0, primitiveCount);
#ifdef GL_ARB_point_sprite
				if (pType == EPT_POINT_SPRITES && FeatureAvailable[SAPPHIRE_ARB_point_sprite])
				{
					glDisable(GL_POINT_SPRITE_ARB);
					glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE, GL_FALSE);
				}
#endif
			}
			break;
			case EPT_LINE_STRIP:
				glDrawElements(GL_LINE_STRIP, primitiveCount + 1, indexSize, indexList);
				break;
			case EPT_LINE_LOOP:
				glDrawElements(GL_LINE_LOOP, primitiveCount, indexSize, indexList);
				break;
			case EPT_LINES:
				glDrawElements(GL_LINES, primitiveCount * 2, indexSize, indexList);
				break;
			case EPT_TRIANGLE_STRIP:
				glDrawElements(GL_TRIANGLE_STRIP, primitiveCount + 2, indexSize, indexList);
				break;
			case EPT_TRIANGLE_FAN:
				glDrawElements(GL_TRIANGLE_FAN, primitiveCount + 2, indexSize, indexList);
				break;
			case EPT_TRIANGLES:
				glDrawElements(GL_TRIANGLES, primitiveCount * 3, indexSize, indexList);
				break;
			case EPT_QUAD_STRIP:
				glDrawElements(GL_QUAD_STRIP, primitiveCount * 2 + 2, indexSize, indexList);
				break;
			case EPT_QUADS:
				glDrawElements(GL_QUADS, primitiveCount * 4, indexSize, indexList);
				break;
			case EPT_POLYGON:
				glDrawElements(GL_POLYGON, primitiveCount, indexSize, indexList);
				break;
			}
		}


		//! draws a vertex primitive list in 2d
		void COpenGLDriver::draw2DVertexPrimitiveList(const void* vertices, UINT32 vertexCount,
			const void* indexList, UINT32 primitiveCount,
			E_VERTEX_TYPE vType, E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
		{
			if (!primitiveCount || !vertexCount)
				return;

			if (!checkPrimitiveCount(primitiveCount))
				return;

			CNullDriver::draw2DVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType);

			if (vertices && !FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] && !FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra])
				getColorBuffer(vertices, vertexCount, vType);

			// draw everything
			this->setActiveTexture(0, Material.getTexture(0));
			if (Material.MaterialType == EMT_ONETEXTURE_BLEND)
			{
				E_BLEND_FACTOR srcFact;
				E_BLEND_FACTOR dstFact;
				E_MODULATE_FUNC modulo;
				UINT32 alphaSource;
				unpack_textureBlendFunc(srcFact, dstFact, modulo, alphaSource, Material.MaterialTypeParam);
				setRenderStates2DMode(alphaSource&EAS_VERTEX_COLOR, (Material.getTexture(0) != 0), (alphaSource&EAS_TEXTURE) != 0);
			}
			else
				setRenderStates2DMode(Material.MaterialType == EMT_TRANSPARENT_VERTEX_ALPHA, (Material.getTexture(0) != 0), Material.MaterialType == EMT_TRANSPARENT_ALPHA_CHANNEL);

			if (MultiTextureExtension)
				extGlClientActiveTexture(GL_TEXTURE0_ARB);

			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			if ((pType != EPT_POINTS) && (pType != EPT_POINT_SPRITES))
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//due to missing defines in OSX headers, we have to be more specific with this check
			//#if defined(GL_ARB_vertex_array_bgra) || defined(GL_EXT_vertex_array_bgra)
#ifdef GL_BGRA
			const GLint colorSize = (FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] || FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra]) ? GL_BGRA : 4;
#else
			const GLint colorSize = 4;
#endif
			if (vertices)
			{
				if (FeatureAvailable[SAPPHIRE_ARB_vertex_array_bgra] || FeatureAvailable[SAPPHIRE_EXT_vertex_array_bgra])
				{
					switch (vType)
					{
					case EVT_STANDARD:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Color);
						break;
					case EVT_2TCOORDS:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Color);
						break;
					case EVT_TANGENTS:
						glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Color);
						break;
					}
				}
				else
				{
					// avoid passing broken pointer to OpenGL
					assert(ColorBuffer.size() == 0);
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, 0, &ColorBuffer[0]);
				}
			}

			switch (vType)
			{
			case EVT_STANDARD:
				if (vertices)
				{
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Pos);
				}
				else
				{
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertex), 0);
				}

				if (MultiTextureExtension && CurrentTexture[1])
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
				}
				break;
			case EVT_2TCOORDS:
				if (vertices)
				{
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords);
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Pos);
				}
				else
				{
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertex2TCoords), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(28));
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(0));
				}

				if (MultiTextureExtension)
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords2);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(36));
				}
				break;
			case EVT_TANGENTS:
				if (vertices)
				{
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].TCoords);
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Pos);
				}
				else
				{
					glColorPointer(colorSize, GL_UNSIGNED_BYTE, sizeof(S3DVertexTangents), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(28));
					glVertexPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(0));
				}

				break;
			}

			renderArray(indexList, primitiveCount, pType, iType);

			if (MultiTextureExtension)
			{
				if ((vType != EVT_STANDARD) || CurrentTexture[1])
				{
					extGlClientActiveTexture(GL_TEXTURE1_ARB);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				extGlClientActiveTexture(GL_TEXTURE0_ARB);
			}
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}


		//! draws a set of 2d images, using a color and the alpha channel of the
		//! texture if desired.
		void COpenGLDriver::draw2DImageBatch(const ITexture* texture,
			const vector<Position2d>::type& positions,
			const vector<rect<SINT32> >::type& sourceRects,
			const rect<SINT32>* clipRect,
			ColourValue color,
			bool useAlphaChannelOfTexture)
		{
			if (!texture)
				return;

			const UINT32 drawCount = Math::min_<UINT32>(positions.size(), sourceRects.size());

			const dimension2d<UINT32>& ss = texture->getOriginalSize();
			const FLOAT32 invW = 1.f / static_cast<FLOAT32>(ss.Width);
			const FLOAT32 invH = 1.f / static_cast<FLOAT32>(ss.Height);
			const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();

			disableTextures(1);
			if (!setActiveTexture(0, texture))
				return;
			setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture);

			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			glBegin(GL_QUADS);

			for (UINT32 i = 0; i<drawCount; ++i)
			{
				if (!sourceRects[i].isValid())
					continue;

				Position2d targetPos(positions[i]);
				Position2d sourcePos(sourceRects[i].UpperLeftCorner);
				// This needs to be signed as it may go negative.
				dimension2d<SINT32> sourceSize(sourceRects[i].getSize());
				if (clipRect)
				{
					if (targetPos.x < clipRect->UpperLeftCorner.x)
					{
						sourceSize.Width += targetPos.x - clipRect->UpperLeftCorner.x;
						if (sourceSize.Width <= 0)
							continue;

						sourcePos.x -= targetPos.x - clipRect->UpperLeftCorner.x;
						targetPos.x = clipRect->UpperLeftCorner.x;
					}

					if (targetPos.x + sourceSize.Width > clipRect->LowerRightCorner.x)
					{
						sourceSize.Width -= (targetPos.x + sourceSize.Width) - clipRect->LowerRightCorner.x;
						if (sourceSize.Width <= 0)
							continue;
					}

					if (targetPos.y < clipRect->UpperLeftCorner.y)
					{
						sourceSize.Height += targetPos.y - clipRect->UpperLeftCorner.y;
						if (sourceSize.Height <= 0)
							continue;

						sourcePos.y -= targetPos.y - clipRect->UpperLeftCorner.y;
						targetPos.y = clipRect->UpperLeftCorner.y;
					}

					if (targetPos.y + sourceSize.Height > clipRect->LowerRightCorner.y)
					{
						sourceSize.Height -= (targetPos.y + sourceSize.Height) - clipRect->LowerRightCorner.y;
						if (sourceSize.Height <= 0)
							continue;
					}
				}

				// clip these coordinates

				if (targetPos.x<0)
				{
					sourceSize.Width += targetPos.x;
					if (sourceSize.Width <= 0)
						continue;

					sourcePos.x -= targetPos.x;
					targetPos.x = 0;
				}

				if (targetPos.x + sourceSize.Width >(SINT32)renderTargetSize.Width)
				{
					sourceSize.Width -= (targetPos.x + sourceSize.Width) - renderTargetSize.Width;
					if (sourceSize.Width <= 0)
						continue;
				}

				if (targetPos.y<0)
				{
					sourceSize.Height += targetPos.y;
					if (sourceSize.Height <= 0)
						continue;

					sourcePos.y -= targetPos.y;
					targetPos.y = 0;
				}

				if (targetPos.y + sourceSize.Height >(SINT32)renderTargetSize.Height)
				{
					sourceSize.Height -= (targetPos.y + sourceSize.Height) - renderTargetSize.Height;
					if (sourceSize.Height <= 0)
						continue;
				}

				// ok, we've clipped everything.
				// now draw it.

				const rect<FLOAT32> tcoords(
					sourcePos.x * invW,
					sourcePos.y * invH,
					(sourcePos.x + sourceSize.Width) * invW,
					(sourcePos.y + sourceSize.Height) * invH);

				const rect<SINT32> poss(targetPos, sourceSize);

				glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.UpperLeftCorner.y);
				glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.UpperLeftCorner.y));

				glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.UpperLeftCorner.y);
				glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.UpperLeftCorner.y));

				glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.LowerRightCorner.y);
				glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.LowerRightCorner.y));

				glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.LowerRightCorner.y);
				glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.LowerRightCorner.y));
			}
			glEnd();
		}


		//! draws a 2d image, using a color and the alpha channel of the texture if
		//! desired. The image is drawn at pos, clipped against clipRect (if != 0).
		//! Only the subtexture defined by sourceRect is used.
		void COpenGLDriver::draw2DImage(const ITexture* texture,
			const Position2d& pos,
			const rect<SINT32>& sourceRect,
			const rect<SINT32>* clipRect, ColourValue color,
			bool useAlphaChannelOfTexture)
		{
			if (!texture)
				return;

			if (!sourceRect.isValid())
				return;

			Position2d targetPos(pos);
			Position2d sourcePos(sourceRect.UpperLeftCorner);
			// This needs to be signed as it may go negative.
			dimension2d<SINT32> sourceSize(sourceRect.getSize());
			if (clipRect)
			{
				if (targetPos.x < clipRect->UpperLeftCorner.x)
				{
					sourceSize.Width += targetPos.x - clipRect->UpperLeftCorner.x;
					if (sourceSize.Width <= 0)
						return;

					sourcePos.x -= targetPos.x - clipRect->UpperLeftCorner.x;
					targetPos.x = clipRect->UpperLeftCorner.x;
				}

				if (targetPos.x + sourceSize.Width > clipRect->LowerRightCorner.x)
				{
					sourceSize.Width -= (targetPos.x + sourceSize.Width) - clipRect->LowerRightCorner.x;
					if (sourceSize.Width <= 0)
						return;
				}

				if (targetPos.y < clipRect->UpperLeftCorner.y)
				{
					sourceSize.Height += targetPos.y - clipRect->UpperLeftCorner.y;
					if (sourceSize.Height <= 0)
						return;

					sourcePos.y -= targetPos.y - clipRect->UpperLeftCorner.y;
					targetPos.y = clipRect->UpperLeftCorner.y;
				}

				if (targetPos.y + sourceSize.Height > clipRect->LowerRightCorner.y)
				{
					sourceSize.Height -= (targetPos.y + sourceSize.Height) - clipRect->LowerRightCorner.y;
					if (sourceSize.Height <= 0)
						return;
				}
			}

			// clip these coordinates

			if (targetPos.x<0)
			{
				sourceSize.Width += targetPos.x;
				if (sourceSize.Width <= 0)
					return;

				sourcePos.x -= targetPos.x;
				targetPos.x = 0;
			}

			const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();

			if (targetPos.x + sourceSize.Width > (SINT32)renderTargetSize.Width)
			{
				sourceSize.Width -= (targetPos.x + sourceSize.Width) - renderTargetSize.Width;
				if (sourceSize.Width <= 0)
					return;
			}

			if (targetPos.y<0)
			{
				sourceSize.Height += targetPos.y;
				if (sourceSize.Height <= 0)
					return;

				sourcePos.y -= targetPos.y;
				targetPos.y = 0;
			}

			if (targetPos.y + sourceSize.Height >(SINT32)renderTargetSize.Height)
			{
				sourceSize.Height -= (targetPos.y + sourceSize.Height) - renderTargetSize.Height;
				if (sourceSize.Height <= 0)
					return;
			}

			// ok, we've clipped everything.
			// now draw it.

			const dimension2d<UINT32>& ss = texture->getOriginalSize();
			const FLOAT32 invW = 1.f / static_cast<FLOAT32>(ss.Width);
			const FLOAT32 invH = 1.f / static_cast<FLOAT32>(ss.Height);
			const rect<FLOAT32> tcoords(
				sourcePos.x * invW,
				sourcePos.y * invH,
				(sourcePos.x + sourceSize.Width) * invW,
				(sourcePos.y + sourceSize.Height) * invH);

			const rect<SINT32> poss(targetPos, sourceSize);

			disableTextures(1);
			if (!setActiveTexture(0, texture))
				return;
			setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture);

			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			glBegin(GL_QUADS);

			glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.UpperLeftCorner.y);
			glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.UpperLeftCorner.y));

			glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.UpperLeftCorner.y);
			glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.UpperLeftCorner.y));

			glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.LowerRightCorner.y);
			glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.LowerRightCorner.y));

			glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.LowerRightCorner.y);
			glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.LowerRightCorner.y));

			glEnd();
		}


		//! The same, but with a four element array of colors, one for each vertex
		void COpenGLDriver::draw2DImage(const ITexture* texture, const rect<SINT32>& destRect,
			const rect<SINT32>& sourceRect, const rect<SINT32>* clipRect,
			const ColourValue* const colors, bool useAlphaChannelOfTexture)
		{
			if (!texture)
				return;

			const dimension2d<UINT32>& ss = texture->getOriginalSize();
			const FLOAT32 invW = 1.f / static_cast<FLOAT32>(ss.Width);
			const FLOAT32 invH = 1.f / static_cast<FLOAT32>(ss.Height);
			const rect<FLOAT32> tcoords(
				sourceRect.UpperLeftCorner.x * invW,
				sourceRect.UpperLeftCorner.y * invH,
				sourceRect.LowerRightCorner.x * invW,
				sourceRect.LowerRightCorner.y *invH);

			const ColourValue temp[4] =
			{
				0xFFFFFFFF,
				0xFFFFFFFF,
				0xFFFFFFFF,
				0xFFFFFFFF
			};

			const ColourValue* const useColor = colors ? colors : temp;

			disableTextures(1);
			setActiveTexture(0, texture);
			setRenderStates2DMode(useColor[0].getAlpha()<255 || useColor[1].getAlpha()<255 ||
				useColor[2].getAlpha()<255 || useColor[3].getAlpha()<255,
				true, useAlphaChannelOfTexture);

			if (clipRect)
			{
				if (!clipRect->isValid())
					return;

				glEnable(GL_SCISSOR_TEST);
				const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();
				glScissor(clipRect->UpperLeftCorner.x, renderTargetSize.Height - clipRect->LowerRightCorner.y,
					clipRect->getWidth(), clipRect->getHeight());
			}

			glBegin(GL_QUADS);

			glColor4ub(useColor[0].getRed(), useColor[0].getGreen(), useColor[0].getBlue(), useColor[0].getAlpha());
			glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.UpperLeftCorner.y);
			glVertex2f(GLfloat(destRect.UpperLeftCorner.x), GLfloat(destRect.UpperLeftCorner.y));

			glColor4ub(useColor[3].getRed(), useColor[3].getGreen(), useColor[3].getBlue(), useColor[3].getAlpha());
			glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.UpperLeftCorner.y);
			glVertex2f(GLfloat(destRect.LowerRightCorner.x), GLfloat(destRect.UpperLeftCorner.y));

			glColor4ub(useColor[2].getRed(), useColor[2].getGreen(), useColor[2].getBlue(), useColor[2].getAlpha());
			glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.LowerRightCorner.y);
			glVertex2f(GLfloat(destRect.LowerRightCorner.x), GLfloat(destRect.LowerRightCorner.y));

			glColor4ub(useColor[1].getRed(), useColor[1].getGreen(), useColor[1].getBlue(), useColor[1].getAlpha());
			glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.LowerRightCorner.y);
			glVertex2f(GLfloat(destRect.UpperLeftCorner.x), GLfloat(destRect.LowerRightCorner.y));

			glEnd();

			if (clipRect)
				glDisable(GL_SCISSOR_TEST);
		}


		//! draws a set of 2d images, using a color and the alpha channel of the
		//! texture if desired. The images are drawn beginning at pos and concatenated
		//! in one line. All drawings are clipped against clipRect (if != 0).
		//! The subtextures are defined by the array of sourceRects and are chosen
		//! by the indices given.
		void COpenGLDriver::draw2DImage(const ITexture* texture,
			const Position2d& pos,
			const vector<rect<SINT32> >::type& sourceRects,
			const vector<SINT32>::type& indices,
			const rect<SINT32>* clipRect, ColourValue color,
			bool useAlphaChannelOfTexture)
		{
			if (!texture)
				return;

			disableTextures(1);
			if (!setActiveTexture(0, texture))
				return;
			setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture);

			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			if (clipRect)
			{
				if (!clipRect->isValid())
					return;

				glEnable(GL_SCISSOR_TEST);
				const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();
				glScissor(clipRect->UpperLeftCorner.x, renderTargetSize.Height - clipRect->LowerRightCorner.y,
					clipRect->getWidth(), clipRect->getHeight());
			}

			const dimension2d<UINT32>& ss = texture->getOriginalSize();
			Position2d targetPos(pos);
			const FLOAT32 invW = 1.f / static_cast<FLOAT32>(ss.Width);
			const FLOAT32 invH = 1.f / static_cast<FLOAT32>(ss.Height);

			for (UINT32 i = 0; i<indices.size(); ++i)
			{
				const SINT32 currentIndex = indices[i];
				if (!sourceRects[currentIndex].isValid())
					break;

				const rect<FLOAT32> tcoords(
					sourceRects[currentIndex].UpperLeftCorner.x * invW,
					sourceRects[currentIndex].UpperLeftCorner.y * invH,
					sourceRects[currentIndex].LowerRightCorner.x * invW,
					sourceRects[currentIndex].LowerRightCorner.y * invH);

				const rect<SINT32> poss(targetPos, sourceRects[currentIndex].getSize());

				glBegin(GL_QUADS);

				glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.UpperLeftCorner.y);
				glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.UpperLeftCorner.y));

				glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.UpperLeftCorner.y);
				glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.UpperLeftCorner.y));

				glTexCoord2f(tcoords.LowerRightCorner.x, tcoords.LowerRightCorner.y);
				glVertex2f(GLfloat(poss.LowerRightCorner.x), GLfloat(poss.LowerRightCorner.y));

				glTexCoord2f(tcoords.UpperLeftCorner.x, tcoords.LowerRightCorner.y);
				glVertex2f(GLfloat(poss.UpperLeftCorner.x), GLfloat(poss.LowerRightCorner.y));

				glEnd();
				targetPos.x += sourceRects[currentIndex].getWidth();
			}
			if (clipRect)
				glDisable(GL_SCISSOR_TEST);
		}


		//! draw a 2d rectangle
		void COpenGLDriver::draw2DRectangle(ColourValue color, const rect<SINT32>& position,
			const rect<SINT32>* clip)
		{
			disableTextures();
			setRenderStates2DMode(color.getAlpha() < 255, false, false);

			rect<SINT32> pos = position;

			if (clip)
				pos.clipAgainst(*clip);

			if (!pos.isValid())
				return;

			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			glRectf(GLfloat(pos.UpperLeftCorner.x), GLfloat(pos.UpperLeftCorner.y),
				GLfloat(pos.LowerRightCorner.x), GLfloat(pos.LowerRightCorner.y));
		}


		//! draw an 2d rectangle
		void COpenGLDriver::draw2DRectangle(const rect<SINT32>& position,
			ColourValue colorLeftUp, ColourValue colorRightUp, ColourValue colorLeftDown, ColourValue colorRightDown,
			const rect<SINT32>* clip)
		{
			rect<SINT32> pos = position;

			if (clip)
				pos.clipAgainst(*clip);

			if (!pos.isValid())
				return;

			disableTextures();

			setRenderStates2DMode(colorLeftUp.getAlpha() < 255 ||
				colorRightUp.getAlpha() < 255 ||
				colorLeftDown.getAlpha() < 255 ||
				colorRightDown.getAlpha() < 255, false, false);

			glBegin(GL_QUADS);
			glColor4ub(colorLeftUp.getRed(), colorLeftUp.getGreen(),
				colorLeftUp.getBlue(), colorLeftUp.getAlpha());
			glVertex2f(GLfloat(pos.UpperLeftCorner.x), GLfloat(pos.UpperLeftCorner.y));

			glColor4ub(colorRightUp.getRed(), colorRightUp.getGreen(),
				colorRightUp.getBlue(), colorRightUp.getAlpha());
			glVertex2f(GLfloat(pos.LowerRightCorner.x), GLfloat(pos.UpperLeftCorner.y));

			glColor4ub(colorRightDown.getRed(), colorRightDown.getGreen(),
				colorRightDown.getBlue(), colorRightDown.getAlpha());
			glVertex2f(GLfloat(pos.LowerRightCorner.x), GLfloat(pos.LowerRightCorner.y));

			glColor4ub(colorLeftDown.getRed(), colorLeftDown.getGreen(),
				colorLeftDown.getBlue(), colorLeftDown.getAlpha());
			glVertex2f(GLfloat(pos.UpperLeftCorner.x), GLfloat(pos.LowerRightCorner.y));

			glEnd();
		}


		//! Draws a 2d line.
		void COpenGLDriver::draw2DLine(const Position2d& start,
			const Position2d& end, ColourValue color)
		{
			if (start == end)
				drawPixel(start.x, start.y, color);
			else
			{
				disableTextures();
				setRenderStates2DMode(color.getAlpha() < 255, false, false);

				glBegin(GL_LINES);
				glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
				GLfloat x = (GLfloat)start.x;
				GLfloat y = (GLfloat)start.y;
				if (x>end.x)
					x += 0.5f;
				if (y>end.y)
					y += 0.5f;
				glVertex2f(GLfloat(x), GLfloat(y));
				x = (GLfloat)end.x;
				y = (GLfloat)end.y;
				if (x>start.x)
					x += 0.5f;
				if (y>start.y)
					y += 0.5f;
				glVertex2f(GLfloat(x), GLfloat(y));
				glEnd();
			}
		}

		//! Draws a pixel
		void COpenGLDriver::drawPixel(UINT32 x, UINT32 y, const ColourValue &color)
		{
			const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();
			if (x > (UINT32)renderTargetSize.Width || y > (UINT32)renderTargetSize.Height)
				return;

			disableTextures();
			setRenderStates2DMode(color.getAlpha() < 255, false, false);

			glBegin(GL_POINTS);
			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			glVertex2i(x, y);
			glEnd();
		}

		bool COpenGLDriver::setActiveTexture(UINT32 stage, const ITexture* texture)
		{
			if (stage >= MaxSupportedTextures)
				return false;

			if (CurrentTexture[stage] == texture)
				return true;

			if (MultiTextureExtension)
				extGlActiveTexture(GL_TEXTURE0_ARB + stage);

			CurrentTexture.set(stage, texture);

			if (!texture)
			{
				glDisable(GL_TEXTURE_2D);
				return true;
			}
			else
			{
				if (texture->getDriverType() != EDT_OPENGL)
				{
					glDisable(GL_TEXTURE_2D);
					CurrentTexture.set(stage, 0);
					Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", LML_ERROR);
					return false;
				}

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,
					static_cast<const COpenGLTexture*>(texture)->getOpenGLTextureName());
			}
			return true;
		}


		//! disables all textures beginning with the optional fromStage parameter. Otherwise all texture stages are disabled.
		//! Returns whether disabling was successful or not.
		bool COpenGLDriver::disableTextures(UINT32 fromStage)
		{
			bool result = true;
			for (UINT32 i = fromStage; i<MaxSupportedTextures; ++i)
				result &= setActiveTexture(i, 0);
			return result;
		}


		//! creates a matrix in supplied GLfloat array to pass to OpenGL
		inline void COpenGLDriver::getGLMatrix(GLfloat gl_matrix[16], const Matrix4& m)
		{
			memcpy(gl_matrix, m.cpointer(), 16 * sizeof(FLOAT32));
		}


		//! creates a opengltexturematrix from a D3D style texture matrix
		inline void COpenGLDriver::getGLTextureMatrix(GLfloat *o, const Matrix4& m)
		{
			Matrix4 mat = m;
			o[0] = mat.getIndex(0);
			o[1] = mat.getIndex(1);
			o[2] = 0.f;
			o[3] = 0.f;

			o[4] = mat.getIndex(4);
			o[5] = mat.getIndex(5);
			o[6] = 0.f;
			o[7] = 0.f;

			o[8] = 0.f;
			o[9] = 0.f;
			o[10] = 1.f;
			o[11] = 0.f;

			o[12] = mat.getIndex(8);
			o[13] = mat.getIndex(9);
			o[14] = 0.f;
			o[15] = 1.f;
		}


		//! returns a device dependent texture from a software surface (IImage)
		ITexture* COpenGLDriver::createDeviceDependentTexture(IImage* surface, const path& name, void* mipmapData)
		{
			return new COpenGLTexture(surface, name, mipmapData, this);
		}


		//! Sets a material. All 3d drawing functions draw geometry now using this material.
		void COpenGLDriver::setMaterial(const SMaterial& material)
		{
			Material = material;
			OverrideMaterial.apply(Material);

			for (SINT32 i = MaxTextureUnits - 1; i >= 0; --i)
			{
				setActiveTexture(i, material.getTexture(i));
				setTransform((E_TRANSFORMATION_STATE)(ETS_TEXTURE_0 + i),
					Material.getTextureMatrix(i));
			}
		}


		//! prints error if an error happened.
		bool COpenGLDriver::testGLError()
		{
#ifdef _DEBUG
			GLenum g = glGetError();
			switch (g)
			{
			case GL_NO_ERROR:
				return false;
			case GL_INVALID_ENUM:
				Printer::log("GL_INVALID_ENUM", LML_ERROR); break;
			case GL_INVALID_VALUE:
				Printer::log("GL_INVALID_VALUE", LML_ERROR); break;
			case GL_INVALID_OPERATION:
				Printer::log("GL_INVALID_OPERATION", LML_ERROR); break;
			case GL_STACK_OVERFLOW:
				Printer::log("GL_STACK_OVERFLOW", LML_ERROR); break;
			case GL_STACK_UNDERFLOW:
				Printer::log("GL_STACK_UNDERFLOW", LML_ERROR); break;
			case GL_OUT_OF_MEMORY:
				Printer::log("GL_OUT_OF_MEMORY", LML_ERROR); break;
			case GL_TABLE_TOO_LARGE:
				Printer::log("GL_TABLE_TOO_LARGE", LML_ERROR); break;
#if defined(GL_EXT_framebuffer_object)
			case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
				Printer::log("GL_INVALID_FRAMEBUFFER_OPERATION", LML_ERROR); break;
#endif
			};
			//	_SAPPHIRE_DEBUG_BREAK_IF(true);
			return true;
#else
			return false;
#endif
		}


		//! sets the needed renderstates
		void COpenGLDriver::setRenderStates3DMode()
		{
			if (CurrentRenderMode != ERM_3D)
			{
				// Reset Texture Stages
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				// switch back the matrices
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf((Matrices[ETS_VIEW] * Matrices[ETS_WORLD]).pointer());

				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(Matrices[ETS_PROJECTION].pointer());

				ResetRenderStates = true;
#ifdef GL_EXT_clip_volume_hint
				if (FeatureAvailable[SAPPHIRE_EXT_clip_volume_hint])
					glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT, GL_NICEST);
#endif
			}

			if (ResetRenderStates || LastMaterial != Material)
			{
				// unset old material

				if (LastMaterial.MaterialType != Material.MaterialType &&
					static_cast<UINT32>(LastMaterial.MaterialType) < MaterialRenderers.size())
					MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();

				// set new material.
				if (static_cast<UINT32>(Material.MaterialType) < MaterialRenderers.size())
					MaterialRenderers[Material.MaterialType].Renderer->OnSetMaterial(
					Material, LastMaterial, ResetRenderStates, this);

				LastMaterial = Material;
				ResetRenderStates = false;
			}

			if (static_cast<UINT32>(Material.MaterialType) < MaterialRenderers.size())
				MaterialRenderers[Material.MaterialType].Renderer->OnRender(this, EVT_STANDARD);

			CurrentRenderMode = ERM_3D;
		}


		//! Get native wrap mode value
		GLint COpenGLDriver::getTextureWrapMode(const UINT8 clamp)
		{
			GLint mode = GL_REPEAT;
			switch (clamp)
			{
			case ETC_REPEAT:
				mode = GL_REPEAT;
				break;
			case ETC_CLAMP:
				mode = GL_CLAMP;
				break;
			case ETC_CLAMP_TO_EDGE:
#ifdef GL_VERSION_1_2
				if (Version>101)
					mode = GL_CLAMP_TO_EDGE;
				else
#endif
#ifdef GL_SGIS_texture_edge_clamp
					if (FeatureAvailable[SAPPHIRE_SGIS_texture_edge_clamp])
						mode = GL_CLAMP_TO_EDGE_SGIS;
					else
#endif
						// fallback
						mode = GL_CLAMP;
				break;
			case ETC_CLAMP_TO_BORDER:
#ifdef GL_VERSION_1_3
				if (Version>102)
					mode = GL_CLAMP_TO_BORDER;
				else
#endif
#ifdef GL_ARB_texture_border_clamp
					if (FeatureAvailable[SAPPHIRE_ARB_texture_border_clamp])
						mode = GL_CLAMP_TO_BORDER_ARB;
					else
#endif
#ifdef GL_SGIS_texture_border_clamp
						if (FeatureAvailable[SAPPHIRE_SGIS_texture_border_clamp])
							mode = GL_CLAMP_TO_BORDER_SGIS;
						else
#endif
							// fallback
							mode = GL_CLAMP;
				break;
			case ETC_MIRROR:
#ifdef GL_VERSION_1_4
				if (Version>103)
					mode = GL_MIRRORED_REPEAT;
				else
#endif
#ifdef GL_ARB_texture_border_clamp
					if (FeatureAvailable[SAPPHIRE_ARB_texture_mirrored_repeat])
						mode = GL_MIRRORED_REPEAT_ARB;
					else
#endif
#ifdef GL_IBM_texture_mirrored_repeat
						if (FeatureAvailable[SAPPHIRE_IBM_texture_mirrored_repeat])
							mode = GL_MIRRORED_REPEAT_IBM;
						else
#endif
							mode = GL_REPEAT;
				break;
			case ETC_MIRROR_CLAMP:
#ifdef GL_EXT_texture_mirror_clamp
				if (FeatureAvailable[SAPPHIRE_EXT_texture_mirror_clamp])
					mode = GL_MIRROR_CLAMP_EXT;
				else
#endif
#if defined(GL_ATI_texture_mirror_once)
					if (FeatureAvailable[SAPPHIRE_ATI_texture_mirror_once])
						mode = GL_MIRROR_CLAMP_ATI;
					else
#endif
						mode = GL_CLAMP;
				break;
			case ETC_MIRROR_CLAMP_TO_EDGE:
#ifdef GL_EXT_texture_mirror_clamp
				if (FeatureAvailable[SAPPHIRE_EXT_texture_mirror_clamp])
					mode = GL_MIRROR_CLAMP_TO_EDGE_EXT;
				else
#endif
#if defined(GL_ATI_texture_mirror_once)
					if (FeatureAvailable[SAPPHIRE_ATI_texture_mirror_once])
						mode = GL_MIRROR_CLAMP_TO_EDGE_ATI;
					else
#endif
						mode = GL_CLAMP;
				break;
			case ETC_MIRROR_CLAMP_TO_BORDER:
#ifdef GL_EXT_texture_mirror_clamp
				if (FeatureAvailable[SAPPHIRE_EXT_texture_mirror_clamp])
					mode = GL_MIRROR_CLAMP_TO_BORDER_EXT;
				else
#endif
					mode = GL_CLAMP;
				break;
			}
			return mode;
		}


		void COpenGLDriver::setWrapMode(const SMaterial& material)
		{
			// texture address mode
			// Has to be checked always because it depends on the textures
			for (UINT32 u = 0; u<MaxTextureUnits; ++u)
			{
				if (!CurrentTexture[u])
					continue;
				if (MultiTextureExtension)
					extGlActiveTexture(GL_TEXTURE0_ARB + u);
				else if (u>0)
					break; // stop loop

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getTextureWrapMode(material.TextureLayer[u].TextureWrapU));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getTextureWrapMode(material.TextureLayer[u].TextureWrapV));
			}
		}


		//! Can be called by an IMaterialRenderer to make its work easier.
		void COpenGLDriver::setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
			bool resetAllRenderStates)
		{
			if (resetAllRenderStates ||
				lastmaterial.ColorMaterial != material.ColorMaterial)
			{
				switch (material.ColorMaterial)
				{
				case ECM_NONE:
					glDisable(GL_COLOR_MATERIAL);
					break;
				case ECM_DIFFUSE:
					glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
					break;
				case ECM_AMBIENT:
					glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
					break;
				case ECM_EMISSIVE:
					glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
					break;
				case ECM_SPECULAR:
					glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
					break;
				case ECM_DIFFUSE_AND_AMBIENT:
					glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
					break;
				}
				if (material.ColorMaterial != ECM_NONE)
					glEnable(GL_COLOR_MATERIAL);
			}

			if (resetAllRenderStates ||
				lastmaterial.AmbientColor != material.AmbientColor ||
				lastmaterial.DiffuseColor != material.DiffuseColor ||
				lastmaterial.EmissiveColor != material.EmissiveColor ||
				lastmaterial.ColorMaterial != material.ColorMaterial)
			{
				GLfloat color[4];

				const FLOAT32 inv = 1.0f / 255.0f;

				if ((material.ColorMaterial != ECM_AMBIENT) &&
					(material.ColorMaterial != ECM_DIFFUSE_AND_AMBIENT))
				{
					color[0] = material.AmbientColor.getRed() * inv;
					color[1] = material.AmbientColor.getGreen() * inv;
					color[2] = material.AmbientColor.getBlue() * inv;
					color[3] = material.AmbientColor.getAlpha() * inv;
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
				}

				if ((material.ColorMaterial != ECM_DIFFUSE) &&
					(material.ColorMaterial != ECM_DIFFUSE_AND_AMBIENT))
				{
					color[0] = material.DiffuseColor.getRed() * inv;
					color[1] = material.DiffuseColor.getGreen() * inv;
					color[2] = material.DiffuseColor.getBlue() * inv;
					color[3] = material.DiffuseColor.getAlpha() * inv;
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
				}

				if (material.ColorMaterial != ECM_EMISSIVE)
				{
					color[0] = material.EmissiveColor.getRed() * inv;
					color[1] = material.EmissiveColor.getGreen() * inv;
					color[2] = material.EmissiveColor.getBlue() * inv;
					color[3] = material.EmissiveColor.getAlpha() * inv;
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
				}
			}

			if (resetAllRenderStates ||
				lastmaterial.SpecularColor != material.SpecularColor ||
				lastmaterial.Shininess != material.Shininess ||
				lastmaterial.ColorMaterial != material.ColorMaterial)
			{
				GLfloat color[4] = { 0.f, 0.f, 0.f, 1.f };
				const FLOAT32 inv = 1.0f / 255.0f;

				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.Shininess);
				// disable Specular colors if no shininess is set
				if ((material.Shininess != 0.0f) &&
					(material.ColorMaterial != ECM_SPECULAR))
				{
#ifdef GL_EXT_separate_specular_color
					if (FeatureAvailable[SAPPHIRE_EXT_separate_specular_color])
						glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif
					color[0] = material.SpecularColor.getRed() * inv;
					color[1] = material.SpecularColor.getGreen() * inv;
					color[2] = material.SpecularColor.getBlue() * inv;
					color[3] = material.SpecularColor.getAlpha() * inv;
				}
#ifdef GL_EXT_separate_specular_color
				else if (FeatureAvailable[SAPPHIRE_EXT_separate_specular_color])
					glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
#endif
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
			}

			// Texture filter
			// Has to be checked always because it depends on the textures
			// Filtering has to be set for each texture layer
			for (UINT32 i = 0; i<MaxTextureUnits; ++i)
			{
				if (!CurrentTexture[i])
					continue;
				if (MultiTextureExtension)
					extGlActiveTexture(GL_TEXTURE0_ARB + i);
				else if (i>0)
					break;

#ifdef GL_EXT_texture_lod_bias
				if (FeatureAvailable[SAPPHIRE_EXT_texture_lod_bias])
				{
					if (material.TextureLayer[i].LODBias)
					{
						const float tmp = Math::Clamp(material.TextureLayer[i].LODBias * 0.125f, -MaxTextureLODBias, MaxTextureLODBias);
						glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, tmp);
					}
					else
						glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, 0.f);
				}
#endif

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					(material.TextureLayer[i].BilinearFilter || material.TextureLayer[i].TrilinearFilter) ? GL_LINEAR : GL_NEAREST);

				if (material.UseMipMaps && CurrentTexture[i]->hasMipMaps())
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					material.TextureLayer[i].TrilinearFilter ? GL_LINEAR_MIPMAP_LINEAR :
					material.TextureLayer[i].BilinearFilter ? GL_LINEAR_MIPMAP_NEAREST :
					GL_NEAREST_MIPMAP_NEAREST);
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					(material.TextureLayer[i].BilinearFilter || material.TextureLayer[i].TrilinearFilter) ? GL_LINEAR : GL_NEAREST);

#ifdef GL_EXT_texture_filter_anisotropic
				if (FeatureAvailable[SAPPHIRE_EXT_texture_filter_anisotropic])
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
					material.TextureLayer[i].AnisotropicFilter>1 ? Math::min_(MaxAnisotropy, material.TextureLayer[i].AnisotropicFilter) : 1);
#endif
			}

			// fillmode
			if (resetAllRenderStates || (lastmaterial.Wireframe != material.Wireframe) || (lastmaterial.PointCloud != material.PointCloud))
				glPolygonMode(GL_FRONT_AND_BACK, material.Wireframe ? GL_LINE : material.PointCloud ? GL_POINT : GL_FILL);

			// shademode
			if (resetAllRenderStates || (lastmaterial.GouraudShading != material.GouraudShading))
			{
				if (material.GouraudShading)
					glShadeModel(GL_SMOOTH);
				else
					glShadeModel(GL_FLAT);
			}

			// lighting
			if (resetAllRenderStates || (lastmaterial.Lighting != material.Lighting))
			{
				if (material.Lighting)
					glEnable(GL_LIGHTING);
				else
					glDisable(GL_LIGHTING);
			}

			// zbuffer
			if (resetAllRenderStates || lastmaterial.ZBuffer != material.ZBuffer)
			{
				switch (material.ZBuffer)
				{
				case ECFN_NEVER:
					glDisable(GL_DEPTH_TEST);
					break;
				case ECFN_LESSEQUAL:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);
					break;
				case ECFN_EQUAL:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_EQUAL);
					break;
				case ECFN_LESS:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
					break;
				case ECFN_NOTEQUAL:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_NOTEQUAL);
					break;
				case ECFN_GREATEREQUAL:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_GEQUAL);
					break;
				case ECFN_GREATER:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_GREATER);
					break;
				case ECFN_ALWAYS:
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_ALWAYS);
					break;
				}
			}

			// zwrite
			//	if (resetAllRenderStates || lastmaterial.ZWriteEnable != material.ZWriteEnable)
	{
		if (material.ZWriteEnable && (AllowZWriteOnTransparent || !material.isTransparent()))
		{
			glDepthMask(GL_TRUE);
		}
		else
			glDepthMask(GL_FALSE);
	}

	// back face culling
	if (resetAllRenderStates || (lastmaterial.FrontfaceCulling != material.FrontfaceCulling) || (lastmaterial.BackfaceCulling != material.BackfaceCulling))
	{
		if ((material.FrontfaceCulling) && (material.BackfaceCulling))
		{
			glCullFace(GL_FRONT_AND_BACK);
			glEnable(GL_CULL_FACE);
		}
		else
			if (material.BackfaceCulling)
			{
				glCullFace(GL_BACK);
				glEnable(GL_CULL_FACE);
			}
			else
				if (material.FrontfaceCulling)
				{
					glCullFace(GL_FRONT);
					glEnable(GL_CULL_FACE);
				}
				else
					glDisable(GL_CULL_FACE);
	}

	// fog
	if (resetAllRenderStates || lastmaterial.FogEnable != material.FogEnable)
	{
		if (material.FogEnable)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
	}

	// normalization
	if (resetAllRenderStates || lastmaterial.NormalizeNormals != material.NormalizeNormals)
	{
		if (material.NormalizeNormals)
			glEnable(GL_NORMALIZE);
		else
			glDisable(GL_NORMALIZE);
	}

	// Color Mask
	if (resetAllRenderStates || lastmaterial.ColorMask != material.ColorMask)
	{
		glColorMask(
			(material.ColorMask & ECP_RED) ? GL_TRUE : GL_FALSE,
			(material.ColorMask & ECP_GREEN) ? GL_TRUE : GL_FALSE,
			(material.ColorMask & ECP_BLUE) ? GL_TRUE : GL_FALSE,
			(material.ColorMask & ECP_ALPHA) ? GL_TRUE : GL_FALSE);
	}

	if (queryFeature(EVDF_BLEND_OPERATIONS) &&
		(resetAllRenderStates || lastmaterial.BlendOperation != material.BlendOperation))
	{
		if (material.BlendOperation == EBO_NONE)
			glDisable(GL_BLEND);
		else
		{
			glEnable(GL_BLEND);
#if defined(GL_EXT_blend_subtract) || defined(GL_EXT_blend_minmax) || defined(GL_EXT_blend_logic_op) || defined(GL_VERSION_1_2)
			switch (material.BlendOperation)
			{
			case EBO_SUBTRACT:
#if defined(GL_EXT_blend_subtract)
				if (FeatureAvailable[SAPPHIRE_EXT_blend_subtract] || (Version >= 120))
					extGlBlendEquation(GL_FUNC_SUBTRACT_EXT);
#elif defined(GL_VERSION_1_2)
				if (Version >= 120)
					extGlBlendEquation(GL_FUNC_SUBTRACT);
#endif
				break;
			case EBO_REVSUBTRACT:
#if defined(GL_EXT_blend_subtract)
				if (FeatureAvailable[SAPPHIRE_EXT_blend_subtract] || (Version >= 120))
					extGlBlendEquation(GL_FUNC_REVERSE_SUBTRACT_EXT);
#elif defined(GL_VERSION_1_2)
				if (Version >= 120)
					extGlBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
#endif
				break;
			case EBO_MIN:
#if defined(GL_EXT_blend_minmax)
				if (FeatureAvailable[SAPPHIRE_EXT_blend_minmax] || (Version >= 120))
					extGlBlendEquation(GL_MIN_EXT);
#elif defined(GL_VERSION_1_2)
				if (Version >= 120)
					extGlBlendEquation(GL_MIN);
#endif
				break;
			case EBO_MAX:
#if defined(GL_EXT_blend_minmax)
				if (FeatureAvailable[SAPPHIRE_EXT_blend_minmax] || (Version >= 120))
					extGlBlendEquation(GL_MAX_EXT);
#elif defined(GL_VERSION_1_2)
				if (Version >= 120)
					extGlBlendEquation(GL_MAX);
#endif
				break;
			case EBO_MIN_FACTOR:
#if defined(GL_AMD_blend_minmax_factor)
				if (FeatureAvailable[SAPPHIRE_AMD_blend_minmax_factor])
					extGlBlendEquation(GL_FACTOR_MIN_AMD);
#endif
				// fallback in case of missing extension
#if defined(GL_VERSION_1_2)
#if defined(GL_AMD_blend_minmax_factor)
				else
#endif
					if (Version >= 120)
						extGlBlendEquation(GL_MIN);
#endif
				break;
			case EBO_MAX_FACTOR:
#if defined(GL_AMD_blend_minmax_factor)
				if (FeatureAvailable[SAPPHIRE_AMD_blend_minmax_factor])
					extGlBlendEquation(GL_FACTOR_MAX_AMD);
#endif
				// fallback in case of missing extension
#if defined(GL_VERSION_1_2)
#if defined(GL_AMD_blend_minmax_factor)
				else
#endif
					if (Version >= 120)
						extGlBlendEquation(GL_MAX);
#endif
				break;
			case EBO_MIN_ALPHA:
#if defined(GL_SGIX_blend_alpha_minmax)
				if (FeatureAvailable[SAPPHIRE_SGIX_blend_alpha_minmax])
					extGlBlendEquation(GL_ALPHA_MIN_SGIX);
				// fallback in case of missing extension
				else
					if (FeatureAvailable[SAPPHIRE_EXT_blend_minmax])
						extGlBlendEquation(GL_MIN_EXT);
#endif
				break;
			case EBO_MAX_ALPHA:
#if defined(GL_SGIX_blend_alpha_minmax)
				if (FeatureAvailable[SAPPHIRE_SGIX_blend_alpha_minmax])
					extGlBlendEquation(GL_ALPHA_MAX_SGIX);
				// fallback in case of missing extension
				else
					if (FeatureAvailable[SAPPHIRE_EXT_blend_minmax])
						extGlBlendEquation(GL_MAX_EXT);
#endif
				break;
			default:
#if defined(GL_EXT_blend_subtract) || defined(GL_EXT_blend_minmax) || defined(GL_EXT_blend_logic_op)
				extGlBlendEquation(GL_FUNC_ADD_EXT);
#elif defined(GL_VERSION_1_2)
				extGlBlendEquation(GL_FUNC_ADD);
#endif
				break;
			}
#endif
		}
	}

	// Polygon Offset
	if (queryFeature(EVDF_POLYGON_OFFSET) && (resetAllRenderStates ||
		lastmaterial.PolygonOffsetDirection != material.PolygonOffsetDirection ||
		lastmaterial.PolygonOffsetFactor != material.PolygonOffsetFactor))
	{
		glDisable(lastmaterial.Wireframe ? GL_POLYGON_OFFSET_LINE : lastmaterial.PointCloud ? GL_POLYGON_OFFSET_POINT : GL_POLYGON_OFFSET_FILL);
		if (material.PolygonOffsetFactor)
		{
			glDisable(material.Wireframe ? GL_POLYGON_OFFSET_LINE : material.PointCloud ? GL_POLYGON_OFFSET_POINT : GL_POLYGON_OFFSET_FILL);
			glEnable(material.Wireframe ? GL_POLYGON_OFFSET_LINE : material.PointCloud ? GL_POLYGON_OFFSET_POINT : GL_POLYGON_OFFSET_FILL);
		}
		if (material.PolygonOffsetDirection == EPO_BACK)
			glPolygonOffset(1.0f, (GLfloat)material.PolygonOffsetFactor);
		else
			glPolygonOffset(-1.0f, (GLfloat)-material.PolygonOffsetFactor);
	}

	// thickness
	if (resetAllRenderStates || lastmaterial.Thickness != material.Thickness)
	{
		if (AntiAlias)
		{
			//			glPointSize(clamp(static_cast<GLfloat>(material.Thickness), DimSmoothedPoint[0], DimSmoothedPoint[1]));
			// we don't use point smoothing
			glPointSize(Math::Clamp(static_cast<GLfloat>(material.Thickness), DimAliasedPoint[0], DimAliasedPoint[1]));
			glLineWidth(Math::Clamp(static_cast<GLfloat>(material.Thickness), DimSmoothedLine[0], DimSmoothedLine[1]));
		}
		else
		{
			glPointSize(Math::Clamp(static_cast<GLfloat>(material.Thickness), DimAliasedPoint[0], DimAliasedPoint[1]));
			glLineWidth(Math::Clamp(static_cast<GLfloat>(material.Thickness), DimAliasedLine[0], DimAliasedLine[1]));
		}
	}

	// Anti aliasing
	if (resetAllRenderStates || lastmaterial.AntiAliasing != material.AntiAliasing)
	{
		if (FeatureAvailable[SAPPHIRE_ARB_multisample])
		{
			if (material.AntiAliasing & EAAM_ALPHA_TO_COVERAGE)
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
			else if (lastmaterial.AntiAliasing & EAAM_ALPHA_TO_COVERAGE)
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);

			if ((AntiAlias >= 2) && (material.AntiAliasing & (EAAM_SIMPLE | EAAM_QUALITY)))
			{
				glEnable(GL_MULTISAMPLE_ARB);
#ifdef GL_NV_multisample_filter_hint
				if (FeatureAvailable[SAPPHIRE_NV_multisample_filter_hint])
				{
					if ((material.AntiAliasing & EAAM_QUALITY) == EAAM_QUALITY)
						glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
					else
						glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
				}
#endif
			}
			else
				glDisable(GL_MULTISAMPLE_ARB);
		}
		if ((material.AntiAliasing & EAAM_LINE_SMOOTH) != (lastmaterial.AntiAliasing & EAAM_LINE_SMOOTH))
		{
			if (material.AntiAliasing & EAAM_LINE_SMOOTH)
				glEnable(GL_LINE_SMOOTH);
			else if (lastmaterial.AntiAliasing & EAAM_LINE_SMOOTH)
				glDisable(GL_LINE_SMOOTH);
		}
		if ((material.AntiAliasing & EAAM_POINT_SMOOTH) != (lastmaterial.AntiAliasing & EAAM_POINT_SMOOTH))
		{
			if (material.AntiAliasing & EAAM_POINT_SMOOTH)
				// often in software, and thus very slow
				glEnable(GL_POINT_SMOOTH);
			else if (lastmaterial.AntiAliasing & EAAM_POINT_SMOOTH)
				glDisable(GL_POINT_SMOOTH);
		}
	}

	setWrapMode(material);

	// be sure to leave in texture stage 0
	if (MultiTextureExtension)
		extGlActiveTexture(GL_TEXTURE0_ARB);
		}


		//! Enable the 2d override material
		void COpenGLDriver::enableMaterial2D(bool enable)
		{
			if (!enable)
				CurrentRenderMode = ERM_NONE;
			CNullDriver::enableMaterial2D(enable);
		}


		//! sets the needed renderstates
		void COpenGLDriver::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel)
		{
			if (CurrentRenderMode != ERM_2D || Transformation3DChanged)
			{
				// unset last 3d material
				if (CurrentRenderMode == ERM_3D)
				{
					if (static_cast<UINT32>(LastMaterial.MaterialType) < MaterialRenderers.size())
						MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();
				}
				if (Transformation3DChanged)
				{
					glMatrixMode(GL_PROJECTION);

					const dimension2d<UINT32>& renderTargetSize = getCurrentRenderTargetSize();
					Matrix4 m(Matrix4::ZERO);
					m.buildProjectionMatrixOrthoLH(FLOAT32(renderTargetSize.Width), FLOAT32(-(SINT32)(renderTargetSize.Height)), -1.0f, 1.0f);
					m.setTrans(Vector3(-1, 1, 0));
					glLoadMatrixf(m.pointer());

					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(0.375f, 0.375f, 0.0f);

					// Make sure we set first texture matrix
					if (MultiTextureExtension)
						extGlActiveTexture(GL_TEXTURE0_ARB);

					Transformation3DChanged = false;
				}
				if (!OverrideMaterial2DEnabled)
				{
					setBasicRenderStates(InitMaterial2D, LastMaterial, true);
					LastMaterial = InitMaterial2D;
				}
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef GL_EXT_clip_volume_hint
				if (FeatureAvailable[SAPPHIRE_EXT_clip_volume_hint])
					glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT, GL_FASTEST);
#endif

			}
			if (OverrideMaterial2DEnabled)
			{
				OverrideMaterial2D.Lighting = false;
				setBasicRenderStates(OverrideMaterial2D, LastMaterial, false);
				LastMaterial = OverrideMaterial2D;
			}

			// no alphaChannel without texture
			alphaChannel &= texture;

			if (alphaChannel || alpha)
			{
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.f);
			}
			else
			{
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
			}

			if (texture)
			{
				if (!OverrideMaterial2DEnabled)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				Material.setTexture(0, const_cast<ITexture*>(CurrentTexture[0]));
				setTransform(ETS_TEXTURE_0, Matrix4::IDENTITY);
				// Due to the transformation change, the previous line would call a reset each frame
				// but we can safely reset the variable as it was false before
				Transformation3DChanged = false;

				if (alphaChannel)
				{
					// if alpha and alpha texture just modulate, otherwise use only the alpha channel
					if (alpha)
					{
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					}
					else
					{
#if defined(GL_ARB_texture_env_combine) || defined(GL_EXT_texture_env_combine)
						if (FeatureAvailable[SAPPHIRE_ARB_texture_env_combine] || FeatureAvailable[SAPPHIRE_EXT_texture_env_combine])
						{
#ifdef GL_ARB_texture_env_combine
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
							// rgb always modulates
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB);
#else
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
							// rgb always modulates
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
#endif
						}
						else
#endif
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					}
				}
				else
				{
					if (alpha)
					{
#if defined(GL_ARB_texture_env_combine) || defined(GL_EXT_texture_env_combine)
						if (FeatureAvailable[SAPPHIRE_ARB_texture_env_combine] || FeatureAvailable[SAPPHIRE_EXT_texture_env_combine])
						{
#ifdef GL_ARB_texture_env_combine
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
							// rgb always modulates
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB);
#else
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
							// rgb always modulates
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
							glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
#endif
						}
						else
#endif
							glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					}
					else
					{
						glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					}
				}
			}

			CurrentRenderMode = ERM_2D;
		}


		//! \return Returns the name of the video driver.
		const wchar_t* COpenGLDriver::getName() const
		{
			return Name.c_str();
		}


		//! deletes all dynamic lights there are
		void COpenGLDriver::deleteAllDynamicLights()
		{
			for (SINT32 i = 0; i<MaxLights; ++i)
				glDisable(GL_LIGHT0 + i);

			RequestedLights.clear();

			CNullDriver::deleteAllDynamicLights();
		}


		//! adds a dynamic light
		SINT32 COpenGLDriver::addDynamicLight(const SLight& light)
		{
			CNullDriver::addDynamicLight(light);

			RequestedLights.push_back(RequestedLight(light));

			UINT32 newLightIndex = RequestedLights.size() - 1;

			// Try and assign a hardware light just now, but don't worry if I can't
			assignHardwareLight(newLightIndex);

			return (SINT32)newLightIndex;
		}


		void COpenGLDriver::assignHardwareLight(UINT32 lightIndex)
		{
			setTransform(ETS_WORLD, Matrix4());

			SINT32 lidx;
			for (lidx = GL_LIGHT0; lidx < GL_LIGHT0 + MaxLights; ++lidx)
			{
				if (!glIsEnabled(lidx))
				{
					RequestedLights[lightIndex].HardwareLightIndex = lidx;
					break;
				}
			}

			if (lidx == GL_LIGHT0 + MaxLights) // There's no room for it just now
				return;

			GLfloat data[4];
			const SLight & light = RequestedLights[lightIndex].LightData;

			switch (light.Type)
			{
			case ELT_SPOT:
				data[0] = light.Direction.x;
				data[1] = light.Direction.y;
				data[2] = light.Direction.z;
				data[3] = 0.0f;
				glLightfv(lidx, GL_SPOT_DIRECTION, data);

				// set position
				data[0] = light.Position.x;
				data[1] = light.Position.y;
				data[2] = light.Position.z;
				data[3] = 1.0f; // 1.0f for positional light
				glLightfv(lidx, GL_POSITION, data);

				glLightf(lidx, GL_SPOT_EXPONENT, light.Falloff);
				glLightf(lidx, GL_SPOT_CUTOFF, light.OuterCone);
				break;
			case ELT_POINT:
				// set position
				data[0] = light.Position.x;
				data[1] = light.Position.y;
				data[2] = light.Position.z;
				data[3] = 1.0f; // 1.0f for positional light
				glLightfv(lidx, GL_POSITION, data);

				glLightf(lidx, GL_SPOT_EXPONENT, 0.0f);
				glLightf(lidx, GL_SPOT_CUTOFF, 180.0f);
				break;
			case ELT_DIRECTIONAL:
				// set direction
				data[0] = -light.Direction.x;
				data[1] = -light.Direction.y;
				data[2] = -light.Direction.z;
				data[3] = 0.0f; // 0.0f for directional light
				glLightfv(lidx, GL_POSITION, data);

				glLightf(lidx, GL_SPOT_EXPONENT, 0.0f);
				glLightf(lidx, GL_SPOT_CUTOFF, 180.0f);
				break;
			default:
				break;
			}

			// set diffuse color
			data[0] = light.DiffuseColor.r;
			data[1] = light.DiffuseColor.g;
			data[2] = light.DiffuseColor.b;
			data[3] = light.DiffuseColor.a;
			glLightfv(lidx, GL_DIFFUSE, data);

			// set specular color
			data[0] = light.SpecularColor.r;
			data[1] = light.SpecularColor.g;
			data[2] = light.SpecularColor.b;
			data[3] = light.SpecularColor.a;
			glLightfv(lidx, GL_SPECULAR, data);

			// set ambient color
			data[0] = light.AmbientColor.r;
			data[1] = light.AmbientColor.g;
			data[2] = light.AmbientColor.b;
			data[3] = light.AmbientColor.a;
			glLightfv(lidx, GL_AMBIENT, data);

			// 1.0f / (constant + linear * d + quadratic*(d*d);

			// set attenuation
			glLightf(lidx, GL_CONSTANT_ATTENUATION, light.Attenuation.x);
			glLightf(lidx, GL_LINEAR_ATTENUATION, light.Attenuation.y);
			glLightf(lidx, GL_QUADRATIC_ATTENUATION, light.Attenuation.z);

			glEnable(lidx);
		}


		//! Turns a dynamic light on or off
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		void COpenGLDriver::turnLightOn(SINT32 lightIndex, bool turnOn)
		{
			if (lightIndex < 0 || lightIndex >= (SINT32)RequestedLights.size())
				return;

			RequestedLight & requestedLight = RequestedLights[lightIndex];

			requestedLight.DesireToBeOn = turnOn;

			if (turnOn)
			{
				if (-1 == requestedLight.HardwareLightIndex)
					assignHardwareLight(lightIndex);
			}
			else
			{
				if (-1 != requestedLight.HardwareLightIndex)
				{
					// It's currently assigned, so free up the hardware light
					glDisable(requestedLight.HardwareLightIndex);
					requestedLight.HardwareLightIndex = -1;

					// Now let the first light that's waiting on a free hardware light grab it
					for (UINT32 requested = 0; requested < RequestedLights.size(); ++requested)
						if (RequestedLights[requested].DesireToBeOn
							&&
							-1 == RequestedLights[requested].HardwareLightIndex)
						{
							assignHardwareLight(requested);
							break;
						}
				}
			}
		}


		//! returns the maximal amount of dynamic lights the device can handle
		UINT32 COpenGLDriver::getMaximalDynamicLightAmount() const
		{
			return MaxLights;
		}


		//! Sets the dynamic ambient light color. The default color is
		//! (0,0,0,0) which means it is dark.
		//! \param color: New color of the ambient light.
		void COpenGLDriver::setAmbientLight(const ColourValue& color)
		{
			GLfloat data[4] = { color.r, color.g, color.b, color.a };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, data);
		}


		// this code was sent in by Oliver Klems, thank you! (I modified the glViewport
		// method just a bit.
		void COpenGLDriver::setViewPort(const rect<SINT32>& area)
		{
			if (area == ViewPort)
				return;
			rect<SINT32> vp = area;
			rect<SINT32> rendert(0, 0, getCurrentRenderTargetSize().Width, getCurrentRenderTargetSize().Height);
			vp.clipAgainst(rendert);

			if (vp.getHeight()>0 && vp.getWidth()>0)
			{
				glViewport(vp.UpperLeftCorner.x,
					getCurrentRenderTargetSize().Height - vp.UpperLeftCorner.y - vp.getHeight(),
					vp.getWidth(), vp.getHeight());

				ViewPort = vp;
			}
		}


		//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
		//! this: First, draw all geometry. Then use this method, to draw the shadow
		//! volume. Next use IVideoDriver::drawStencilShadow() to visualize the shadow.
		void COpenGLDriver::drawStencilShadowVolume(const vector<Vector3>::type& triangles, bool zfail, UINT32 debugDataVisible)
		{
			const UINT32 count = triangles.size();
			if (!StencilBuffer || !count)
				return;

			// unset last 3d material
			if (CurrentRenderMode == ERM_3D &&
				static_cast<UINT32>(Material.MaterialType) < MaterialRenderers.size())
			{
				MaterialRenderers[Material.MaterialType].Renderer->OnUnsetMaterial();
				ResetRenderStates = true;
			}

			// store current OpenGL state
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
				GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

			glDisable(GL_LIGHTING);
			glDisable(GL_FOG);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_FALSE); // no depth buffer writing
			if (debugDataVisible & EDS_MESH_WIRE_OVERLAY)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (!(debugDataVisible & (EDS_SKELETON | EDS_MESH_WIRE_OVERLAY)))
			{
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // no color buffer drawing
				glEnable(GL_STENCIL_TEST);
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Vector3), triangles.cbegin()._Ptr);
			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			GLenum incr = GL_INCR;
			GLenum decr = GL_DECR;
#ifdef GL_EXT_stencil_wrap
			if (FeatureAvailable[SAPPHIRE_EXT_stencil_wrap])
			{
				incr = GL_INCR_WRAP_EXT;
				decr = GL_DECR_WRAP_EXT;
			}
#endif
#ifdef GL_NV_depth_clamp
			if (FeatureAvailable[SAPPHIRE_NV_depth_clamp])
				glEnable(GL_DEPTH_CLAMP_NV);
#endif

			// The first parts are not correctly working, yet.
#if 0
#ifdef GL_EXT_stencil_two_side
			if (FeatureAvailable[SAPPHIRE_EXT_stencil_two_side])
			{
				glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
				glDisable(GL_CULL_FACE);
				if (zfail)
				{
					extGlActiveStencilFace(GL_BACK);
					glStencilOp(GL_KEEP, incr, GL_KEEP);
					glStencilMask(~0);
					glStencilFunc(GL_ALWAYS, 0, ~0);

					extGlActiveStencilFace(GL_FRONT);
					glStencilOp(GL_KEEP, decr, GL_KEEP);
				}
				else // zpass
				{
					extGlActiveStencilFace(GL_BACK);
					glStencilOp(GL_KEEP, GL_KEEP, decr);
					glStencilMask(~0);
					glStencilFunc(GL_ALWAYS, 0, ~0);

					extGlActiveStencilFace(GL_FRONT);
					glStencilOp(GL_KEEP, GL_KEEP, incr);
				}
				glStencilMask(~0);
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glDrawArrays(GL_TRIANGLES, 0, count);
				glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
			}
			else
#endif
				if (FeatureAvailable[SAPPHIRE_ATI_separate_stencil])
				{
					glDisable(GL_CULL_FACE);
					if (zfail)
					{
						extGlStencilOpSeparate(GL_BACK, GL_KEEP, incr, GL_KEEP);
						extGlStencilOpSeparate(GL_FRONT, GL_KEEP, decr, GL_KEEP);
					}
					else // zpass
					{
						extGlStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, decr);
						extGlStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, incr);
					}
					extGlStencilFuncSeparate(GL_ALWAYS, GL_ALWAYS, 0, ~0);
					glStencilMask(~0);
					glDrawArrays(GL_TRIANGLES, 0, count);
				}
				else
#endif
				{
					glEnable(GL_CULL_FACE);
					if (zfail)
					{
						glCullFace(GL_FRONT);
						glStencilOp(GL_KEEP, incr, GL_KEEP);
						glDrawArrays(GL_TRIANGLES, 0, count);

						glCullFace(GL_BACK);
						glStencilOp(GL_KEEP, decr, GL_KEEP);
						glDrawArrays(GL_TRIANGLES, 0, count);
					}
					else // zpass
					{
						glCullFace(GL_BACK);
						glStencilOp(GL_KEEP, GL_KEEP, incr);
						glDrawArrays(GL_TRIANGLES, 0, count);

						glCullFace(GL_FRONT);
						glStencilOp(GL_KEEP, GL_KEEP, decr);
						glDrawArrays(GL_TRIANGLES, 0, count);
					}
				}
#ifdef GL_NV_depth_clamp
			if (FeatureAvailable[SAPPHIRE_NV_depth_clamp])
				glDisable(GL_DEPTH_CLAMP_NV);
#endif

			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisableClientState(GL_VERTEX_ARRAY); //not stored on stack
			glPopAttrib();
		}

		//! Fills the stencil shadow with color. After the shadow volume has been drawn
		//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
		//! to draw the color of the shadow.
		void COpenGLDriver::drawStencilShadow(bool clearStencilBuffer, ColourValue leftUpEdge,
			ColourValue rightUpEdge, ColourValue leftDownEdge, ColourValue rightDownEdge)
		{
			if (!StencilBuffer)
				return;

			disableTextures();

			// store attributes
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT | GL_LIGHTING_BIT);

			glDisable(GL_LIGHTING);
			glDisable(GL_FOG);
			glDepthMask(GL_FALSE);

			glShadeModel(GL_FLAT);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 0, ~0);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			// draw a shadow rectangle covering the entire screen using stencil buffer
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();

			glBegin(GL_QUADS);

			glColor4ub(leftDownEdge.getRed(), leftDownEdge.getGreen(), leftDownEdge.getBlue(), leftDownEdge.getAlpha());
			glVertex3f(-1.f, -1.f, -0.9f);

			glColor4ub(leftUpEdge.getRed(), leftUpEdge.getGreen(), leftUpEdge.getBlue(), leftUpEdge.getAlpha());
			glVertex3f(-1.f, 1.f, -0.9f);

			glColor4ub(rightUpEdge.getRed(), rightUpEdge.getGreen(), rightUpEdge.getBlue(), rightUpEdge.getAlpha());
			glVertex3f(1.f, 1.f, -0.9f);

			glColor4ub(rightDownEdge.getRed(), rightDownEdge.getGreen(), rightDownEdge.getBlue(), rightDownEdge.getAlpha());
			glVertex3f(1.f, -1.f, -0.9f);

			glEnd();

			clearBuffers(false, false, clearStencilBuffer, 0x0);

			// restore settings
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glPopAttrib();
		}


		//! Sets the fog mode.
		void COpenGLDriver::setFog(ColourValue c, E_FOG_TYPE fogType, FLOAT32 start,
			FLOAT32 end, FLOAT32 density, bool pixelFog, bool rangeFog)
		{
			CNullDriver::setFog(c, fogType, start, end, density, pixelFog, rangeFog);

			glFogf(GL_FOG_MODE, GLfloat((fogType == EFT_FOG_LINEAR) ? GL_LINEAR : (fogType == EFT_FOG_EXP) ? GL_EXP : GL_EXP2));

#ifdef GL_EXT_fog_coord
			if (FeatureAvailable[SAPPHIRE_EXT_fog_coord])
				glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
#endif
#ifdef GL_NV_fog_distance
			if (FeatureAvailable[SAPPHIRE_NV_fog_distance])
			{
				if (rangeFog)
					glFogi(GL_FOG_DISTANCE_MODE_NV, GL_EYE_RADIAL_NV);
				else
					glFogi(GL_FOG_DISTANCE_MODE_NV, GL_EYE_PLANE_ABSOLUTE_NV);
			}
#endif

			if (fogType == EFT_FOG_LINEAR)
			{
				glFogf(GL_FOG_START, start);
				glFogf(GL_FOG_END, end);
			}
			else
				glFogf(GL_FOG_DENSITY, density);

			if (pixelFog)
				glHint(GL_FOG_HINT, GL_NICEST);
			else
				glHint(GL_FOG_HINT, GL_FASTEST);

			ColourValue color(c);
			GLfloat data[4] = { color.r, color.g, color.b, color.a };
			glFogfv(GL_FOG_COLOR, data);
		}


		//! Draws a 3d line.
		void COpenGLDriver::draw3DLine(const Vector3& start,
			const Vector3& end, ColourValue color)
		{
			setRenderStates3DMode();

			glBegin(GL_LINES);
			glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			glVertex3f(start.x, start.y, start.z);

			glVertex3f(end.x, end.y, end.z);
			glEnd();
		}


		//! Removes a texture from the texture cache and deletes it, freeing lot of memory.
		void COpenGLDriver::removeTexture(ITexture* texture)
		{
			if (!texture)
				return;

			CNullDriver::removeTexture(texture);
			// Remove this texture from CurrentTexture as well
			CurrentTexture.remove(texture);
		}


		//! Only used by the internal engine. Used to notify the driver that
		//! the window was resized.
		void COpenGLDriver::OnResize(const dimension2d<UINT32>& size)
		{
			CNullDriver::OnResize(size);
			glViewport(0, 0, size.Width, size.Height);
			Transformation3DChanged = true;
		}


		//! Returns type of video driver
		E_DRIVER_TYPE COpenGLDriver::getDriverType() const
		{
			return EDT_OPENGL;
		}


		//! returns color format
		ECOLOR_FORMAT COpenGLDriver::getColorFormat() const
		{
			return ColorFormat;
		}


		//! Sets a vertex shader constant.
		void COpenGLDriver::setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount)
		{
#ifdef GL_ARB_vertex_program
			for (SINT32 i = 0; i<constantAmount; ++i)
				extGlProgramLocalParameter4fv(GL_VERTEX_PROGRAM_ARB, startRegister + i, &data[i * 4]);
#endif
		}

		//! Sets a pixel shader constant.
		void COpenGLDriver::setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount)
		{
#ifdef GL_ARB_fragment_program
			for (SINT32 i = 0; i<constantAmount; ++i)
				extGlProgramLocalParameter4fv(GL_FRAGMENT_PROGRAM_ARB, startRegister + i, &data[i * 4]);
#endif
		}

		//! Sets a constant for the vertex shader based on a name.
		bool COpenGLDriver::setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count)
		{
			//pass this along, as in GLSL the same routine is used for both vertex and fragment shaders
			return setPixelShaderConstant(name, floats, count);
		}

		//! Bool interface for the above.
		bool COpenGLDriver::setVertexShaderConstant(const c8* name, const bool* bools, int count)
		{
			return setPixelShaderConstant(name, bools, count);
		}

		//! Int interface for the above.
		bool COpenGLDriver::setVertexShaderConstant(const c8* name, const SINT32* ints, int count)
		{
			return setPixelShaderConstant(name, ints, count);
		}

		//! Sets a constant for the pixel shader based on a name.
		bool COpenGLDriver::setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count)
		{
			Printer::log("Error: Please call services->setPixelShaderConstant(), not VideoDriver->setPixelShaderConstant().");
			return false;
		}

		//! Bool interface for the above.
		bool COpenGLDriver::setPixelShaderConstant(const c8* name, const bool* bools, int count)
		{
			Printer::log("Error: Please call services->setPixelShaderConstant(), not VideoDriver->setPixelShaderConstant().");
			return false;
		}

		//! Int interface for the above.
		bool COpenGLDriver::setPixelShaderConstant(const c8* name, const SINT32* ints, int count)
		{
			Printer::log("Error: Please call services->setPixelShaderConstant(), not VideoDriver->setPixelShaderConstant().");
			return false;
		}


		//! Adds a new material renderer to the VideoDriver, using pixel and/or
		//! vertex shaders to render geometry.
		SINT32 COpenGLDriver::addShaderMaterial(const c8* vertexShaderProgram,
			const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback,
			E_MATERIAL_TYPE baseMaterial, SINT32 userData)
		{
			SINT32 nr = -1;
			COpenGLShaderMaterialRenderer* r = new COpenGLShaderMaterialRenderer(
				this, nr, vertexShaderProgram, pixelShaderProgram,
				callback, getMaterialRenderer(baseMaterial), userData);

			r->drop();
			return nr;
		}


		//! Adds a new material renderer to the VideoDriver, using GLSL to render geometry.
		SINT32 COpenGLDriver::addHighLevelShaderMaterial(
			const c8* vertexShaderProgram,
			const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget,
			const c8* pixelShaderProgram,
			const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget,
			const c8* geometryShaderProgram,
			const c8* geometryShaderEntryPointName,
			E_GEOMETRY_SHADER_TYPE gsCompileTarget,
			E_PRIMITIVE_TYPE inType,
			E_PRIMITIVE_TYPE outType,
			UINT32 verticesOut,
			IShaderConstantSetCallBack* callback,
			E_MATERIAL_TYPE baseMaterial,
			SINT32 userData, E_GPU_SHADING_LANGUAGE shadingLang)
		{
			SINT32 nr = -1;

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
			if (shadingLang == EGSL_CG)
			{
				COpenGLCgMaterialRenderer* r = new COpenGLCgMaterialRenderer(
					this, nr,
					vertexShaderProgram, vertexShaderEntryPointName, vsCompileTarget,
					pixelShaderProgram, pixelShaderEntryPointName, psCompileTarget,
					geometryShaderProgram, geometryShaderEntryPointName, gsCompileTarget,
					inType, outType, verticesOut,
					callback, getMaterialRenderer(baseMaterial), userData);

				r->drop();
			}
			else
#endif
			{
				COpenGLSLMaterialRenderer* r = new COpenGLSLMaterialRenderer(
					this, nr,
					vertexShaderProgram, vertexShaderEntryPointName, vsCompileTarget,
					pixelShaderProgram, pixelShaderEntryPointName, psCompileTarget,
					geometryShaderProgram, geometryShaderEntryPointName, gsCompileTarget,
					inType, outType, verticesOut,
					callback, getMaterialRenderer(baseMaterial), userData);

				r->drop();
			}

			return nr;
		}


		//! Returns a pointer to the IVideoDriver interface. (Implementation for
		//! IMaterialRendererServices)
		IVideoDriver* COpenGLDriver::getVideoDriver()
		{
			return this;
		}


		ITexture* COpenGLDriver::addRenderTargetTexture(const dimension2d<UINT32>& size,
			const path& name,
			const ECOLOR_FORMAT format)
		{
			//disable mip-mapping
			bool generateMipLevels = getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
			setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

			ITexture* rtt = 0;
#if defined(GL_EXT_framebuffer_object)
			// if driver supports FrameBufferObjects, use them
			if (queryFeature(EVDF_FRAMEBUFFER_OBJECT))
			{
				rtt = new COpenGLFBOTexture(size, name, this, format);
				if (rtt)
				{
					bool success = false;
					addTexture(rtt);
					ITexture* tex = createDepthTexture(rtt);
					if (tex)
					{
						success = static_cast<COpenGLFBODepthTexture*>(tex)->attach(rtt);
						if (!success)
						{
							removeDepthTexture(tex);
						}
						tex->drop();
					}
					rtt->drop();
					if (!success)
					{
						removeTexture(rtt);
						rtt = 0;
					}
				}
			}
			else
#endif
			{
				// the simple texture is only possible for size <= screensize
				// we try to find an optimal size with the original constraints
				dimension2du destSize(Math::min_(size.Width, ScreenSize.Width), Math::min_(size.Height, ScreenSize.Height));
				destSize = destSize.getOptimalSize((size == size.getOptimalSize()), false, false);
				rtt = addTexture(destSize, name, ECF_A8R8G8B8);
				if (rtt)
				{
					static_cast<COpenGLTexture*>(rtt)->setIsRenderTarget(true);
				}
			}

			//restore mip-mapping
			setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, generateMipLevels);

			return rtt;
		}


		//! Returns the maximum amount of primitives (mostly vertices) which
		//! the device is able to render with one drawIndexedTriangleList
		//! call.
		UINT32 COpenGLDriver::getMaximalPrimitiveCount() const
		{
			return 0x7fffffff;
		}


		//! set or reset render target
		bool COpenGLDriver::setRenderTarget(E_RENDER_TARGET target, bool clearTarget,
			bool clearZBuffer, ColourValue color)
		{
			if (target != CurrentTarget)
				setRenderTarget(0, false, false, 0x0);

			if (ERT_RENDER_TEXTURE == target)
			{
				Printer::log("For render textures call setRenderTarget with the actual texture as first parameter.", LML_ERROR);
				return false;
			}
			if (ERT_MULTI_RENDER_TEXTURES == target)
			{
				Printer::log("For multiple render textures call setRenderTarget with the texture array as first parameter.", LML_ERROR);
				return false;
			}

			if (Params.Stereobuffer && (ERT_STEREO_RIGHT_BUFFER == target))
			{
				if (Params.Doublebuffer)
					glDrawBuffer(GL_BACK_RIGHT);
				else
					glDrawBuffer(GL_FRONT_RIGHT);
			}
			else if (Params.Stereobuffer && ERT_STEREO_BOTH_BUFFERS == target)
			{
				if (Params.Doublebuffer)
					glDrawBuffer(GL_BACK);
				else
					glDrawBuffer(GL_FRONT);
			}
			else if ((target >= ERT_AUX_BUFFER0) && (target - ERT_AUX_BUFFER0 < MaxAuxBuffers))
			{
				glDrawBuffer(GL_AUX0 + target - ERT_AUX_BUFFER0);
			}
			else
			{
				if (Params.Doublebuffer)
					glDrawBuffer(GL_BACK_LEFT);
				else
					glDrawBuffer(GL_FRONT_LEFT);
				// exit with false, but also with working color buffer
				if (target != ERT_FRAME_BUFFER)
					return false;
			}
			CurrentTarget = target;
			clearBuffers(clearTarget, clearZBuffer, false, color);
			return true;
		}


		//! set or reset render target
		bool COpenGLDriver::setRenderTarget(ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, ColourValue color)
		{
			// check for right driver type

			if (texture && texture->getDriverType() != EDT_OPENGL)
			{
				Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", LML_ERROR);
				return false;
			}

#if defined(GL_EXT_framebuffer_object)
			if (CurrentTarget == ERT_MULTI_RENDER_TEXTURES)
			{
				for (UINT32 i = 0; i<MRTargets.size(); ++i)
				{
					if (MRTargets[i].TargetType == ERT_RENDER_TEXTURE)
					{
						for (++i; i<MRTargets.size(); ++i)
							if (MRTargets[i].TargetType == ERT_RENDER_TEXTURE)
								extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, 0, 0);
					}
				}
				MRTargets.clear();
			}
#endif

			// check if we should set the previous RT back
			if ((RenderTargetTexture != texture) ||
				(CurrentTarget == ERT_MULTI_RENDER_TEXTURES))
			{
				setActiveTexture(0, 0);
				ResetRenderStates = true;
				if (RenderTargetTexture != 0)
				{
					RenderTargetTexture->unbindRTT();
				}

				if (texture)
				{
					// we want to set a new target. so do this.
					glViewport(0, 0, texture->getSize().Width, texture->getSize().Height);
					RenderTargetTexture = static_cast<COpenGLTexture*>(texture);
					// calls glDrawBuffer as well
					RenderTargetTexture->bindRTT();
					CurrentRendertargetSize = texture->getSize();
					CurrentTarget = ERT_RENDER_TEXTURE;
				}
				else
				{
					glViewport(0, 0, ScreenSize.Width, ScreenSize.Height);
					RenderTargetTexture = 0;
					CurrentRendertargetSize = dimension2d<UINT32>(0, 0);
					CurrentTarget = ERT_FRAME_BUFFER;
					glDrawBuffer(Params.Doublebuffer ? GL_BACK_LEFT : GL_FRONT_LEFT);
				}
				// we need to update the matrices due to the rendersize change.
				Transformation3DChanged = true;
			}

			clearBuffers(clearBackBuffer, clearZBuffer, false, color);

			return true;
		}


		//! Sets multiple render targets
		bool COpenGLDriver::setRenderTarget(const vector<IRenderTarget>::type& targets,
			bool clearBackBuffer, bool clearZBuffer, ColourValue color)
		{
			// if simply disabling the MRT via array call
			if (targets.size() == 0)
				return setRenderTarget(0, clearBackBuffer, clearZBuffer, color);
			// if disabling old MRT, but enabling new one as well
			if ((MRTargets.size() != 0) && (targets != MRTargets))
				setRenderTarget(0, clearBackBuffer, clearZBuffer, color);
			// if no change, simply clear buffers
			else if (targets == MRTargets)
			{
				clearBuffers(clearBackBuffer, clearZBuffer, false, color);
				return true;
			}

			// copy to storage for correct disabling
			MRTargets = targets;

			UINT32 maxMultipleRTTs = Math::min_(static_cast<UINT32>(MaxMultipleRenderTargets), targets.size());

			// determine common size
			dimension2du rttSize = CurrentRendertargetSize;
			if (targets[0].TargetType == ERT_RENDER_TEXTURE)
			{
				if (!targets[0].RenderTexture)
				{
					Printer::log("Missing render texture for MRT.", LML_ERROR);
					return false;
				}
				rttSize = targets[0].RenderTexture->getSize();
			}

			for (UINT32 i = 0; i < maxMultipleRTTs; ++i)
			{
				// check for right driver type
				if (targets[i].TargetType == ERT_RENDER_TEXTURE)
				{
					if (!targets[i].RenderTexture)
					{
						maxMultipleRTTs = i;
						Printer::log("Missing render texture for MRT.", LML_WARNING);
						break;
					}
					if (targets[i].RenderTexture->getDriverType() != EDT_OPENGL)
					{
						maxMultipleRTTs = i;
						Printer::log("Tried to set a texture not owned by this driver.", LML_WARNING);
						break;
					}

					// check for valid render target
					if (!targets[i].RenderTexture->isRenderTarget() || !static_cast<COpenGLTexture*>(targets[i].RenderTexture)->isFrameBufferObject())
					{
						maxMultipleRTTs = i;
						Printer::log("Tried to set a non FBO-RTT as render target.", LML_WARNING);
						break;
					}

					// check for valid size
					if (rttSize != targets[i].RenderTexture->getSize())
					{
						maxMultipleRTTs = i;
						Printer::log("Render target texture has wrong size.", LML_WARNING);
						break;
					}
				}
			}
			if (maxMultipleRTTs == 0)
			{
				Printer::log("No valid MRTs.", LML_ERROR);
				return false;
			}

			// init FBO, if any
			for (UINT32 i = 0; i<maxMultipleRTTs; ++i)
			{
				if (targets[i].TargetType == ERT_RENDER_TEXTURE)
				{
					setRenderTarget(targets[i].RenderTexture, false, false, 0x0);
					break; // bind only first RTT
				}
			}
			// init other main buffer, if necessary
			if (targets[0].TargetType != ERT_RENDER_TEXTURE)
				setRenderTarget(targets[0].TargetType, false, false, 0x0);

			// attach other textures and store buffers into array
			if (maxMultipleRTTs > 1)
			{
				CurrentTarget = ERT_MULTI_RENDER_TEXTURES;
				vector<GLenum>::type MRTs;
				//MRTs.set_used(maxMultipleRTTs);
				MRTs.resize(maxMultipleRTTs);
				for (UINT32 i = 0; i < maxMultipleRTTs; i++)
				{
					if (FeatureAvailable[SAPPHIRE_EXT_draw_buffers2])
					{
						extGlColorMaskIndexed(i,
							(targets[i].ColorMask & ECP_RED) ? GL_TRUE : GL_FALSE,
							(targets[i].ColorMask & ECP_GREEN) ? GL_TRUE : GL_FALSE,
							(targets[i].ColorMask & ECP_BLUE) ? GL_TRUE : GL_FALSE,
							(targets[i].ColorMask & ECP_ALPHA) ? GL_TRUE : GL_FALSE);
						if (targets[i].BlendOp == EBO_NONE)
							extGlDisableIndexed(GL_BLEND, i);
						else
							extGlEnableIndexed(GL_BLEND, i);
					}
#if defined(GL_AMD_draw_buffers_blend) || defined(GL_ARB_draw_buffers_blend)
					if (FeatureAvailable[SAPPHIRE_AMD_draw_buffers_blend] || FeatureAvailable[SAPPHIRE_ARB_draw_buffers_blend])
					{
						extGlBlendFuncIndexed(i, getGLBlend(targets[i].BlendFuncSrc), getGLBlend(targets[i].BlendFuncDst));
						switch (targets[i].BlendOp)
						{
						case EBO_SUBTRACT:
							extGlBlendEquationIndexed(i, GL_FUNC_SUBTRACT);
							break;
						case EBO_REVSUBTRACT:
							extGlBlendEquationIndexed(i, GL_FUNC_REVERSE_SUBTRACT);
							break;
						case EBO_MIN:
							extGlBlendEquationIndexed(i, GL_MIN);
							break;
						case EBO_MAX:
							extGlBlendEquationIndexed(i, GL_MAX);
							break;
						case EBO_MIN_FACTOR:
						case EBO_MIN_ALPHA:
#if defined(GL_AMD_blend_minmax_factor)
							if (FeatureAvailable[SAPPHIRE_AMD_blend_minmax_factor])
								extGlBlendEquationIndexed(i, GL_FACTOR_MIN_AMD);
							// fallback in case of missing extension
							else
#endif
								extGlBlendEquation(GL_MIN);
							break;
						case EBO_MAX_FACTOR:
						case EBO_MAX_ALPHA:
#if defined(GL_AMD_blend_minmax_factor)
							if (FeatureAvailable[SAPPHIRE_AMD_blend_minmax_factor])
								extGlBlendEquationIndexed(i, GL_FACTOR_MAX_AMD);
							// fallback in case of missing extension
							else
#endif
								extGlBlendEquation(GL_MAX);
							break;
						default:
							extGlBlendEquationIndexed(i, GL_FUNC_ADD);
							break;
						}
					}
#endif
					if (targets[i].TargetType == ERT_RENDER_TEXTURE)
					{
						GLenum attachment = GL_NONE;
#ifdef GL_EXT_framebuffer_object
						// attach texture to FrameBuffer Object on Color [i]
						attachment = GL_COLOR_ATTACHMENT0_EXT + i;
						if ((i != 0) && (targets[i].RenderTexture != RenderTargetTexture))
							extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, static_cast<COpenGLTexture*>(targets[i].RenderTexture)->getOpenGLTextureName(), 0);
#endif
						MRTs[i] = attachment;
					}
					else
					{
						switch (targets[i].TargetType)
						{
						case ERT_FRAME_BUFFER:
							MRTs[i] = GL_BACK_LEFT;
							break;
						case ERT_STEREO_BOTH_BUFFERS:
							MRTs[i] = GL_BACK;
							break;
						case ERT_STEREO_RIGHT_BUFFER:
							MRTs[i] = GL_BACK_RIGHT;
							break;
						case ERT_STEREO_LEFT_BUFFER:
							MRTs[i] = GL_BACK_LEFT;
							break;
						default:
							MRTs[i] = GL_AUX0 + (targets[i].TargetType - ERT_AUX_BUFFER0);
							break;
						}
					}
				}

				extGlDrawBuffers(maxMultipleRTTs, MRTs.cbegin()._Ptr);
			}

			clearBuffers(clearBackBuffer, clearZBuffer, false, color);
			return true;
		}


		// returns the current size of the screen or rendertarget
		const dimension2d<UINT32>& COpenGLDriver::getCurrentRenderTargetSize() const
		{
			if (CurrentRendertargetSize.Width == 0)
				return ScreenSize;
			else
				return CurrentRendertargetSize;
		}


		//! Clears the ZBuffer.
		void COpenGLDriver::clearZBuffer()
		{
			clearBuffers(false, true, false, 0x0);
		}


		//! Returns an image created from the last rendered frame.
		IImage* COpenGLDriver::createScreenShot(ECOLOR_FORMAT format, E_RENDER_TARGET target)
		{
			if (target == ERT_MULTI_RENDER_TEXTURES || target == ERT_RENDER_TEXTURE || target == ERT_STEREO_BOTH_BUFFERS)
				return 0;

			// allows to read pixels in top-to-bottom order
#ifdef GL_MESA_pack_invert
			if (FeatureAvailable[SAPPHIRE_MESA_pack_invert])
				glPixelStorei(GL_PACK_INVERT_MESA, GL_TRUE);
#endif

			if (format == ECF_UNKNOWN)
				format = getColorFormat();
			GLenum fmt;
			GLenum type;
			switch (format)
			{
			case ECF_A1R5G5B5:
				fmt = GL_BGRA;
				type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
				break;
			case ECF_R5G6B5:
				fmt = GL_RGB;
				type = GL_UNSIGNED_SHORT_5_6_5;
				break;
			case ECF_R8G8B8:
				fmt = GL_RGB;
				type = GL_UNSIGNED_BYTE;
				break;
			case ECF_A8R8G8B8:
				fmt = GL_BGRA;
				if (Version > 101)
					type = GL_UNSIGNED_INT_8_8_8_8_REV;
				else
					type = GL_UNSIGNED_BYTE;
				break;
			case ECF_R16F:
				if (FeatureAvailable[SAPPHIRE_ARB_texture_rg])
					fmt = GL_RED;
				else
					fmt = GL_LUMINANCE;
#ifdef GL_ARB_half_float_pixel
				if (FeatureAvailable[SAPPHIRE_ARB_half_float_pixel])
					type = GL_HALF_FLOAT_ARB;
				else
#endif
				{
					type = GL_FLOAT;
					format = ECF_R32F;
				}
				break;
			case ECF_G16R16F:
#ifdef GL_ARB_texture_rg
				if (FeatureAvailable[SAPPHIRE_ARB_texture_rg])
					fmt = GL_RG;
				else
#endif
					fmt = GL_LUMINANCE_ALPHA;
#ifdef GL_ARB_half_float_pixel
				if (FeatureAvailable[SAPPHIRE_ARB_half_float_pixel])
					type = GL_HALF_FLOAT_ARB;
				else
#endif
				{
					type = GL_FLOAT;
					format = ECF_G32R32F;
				}
				break;
			case ECF_A16B16G16R16F:
				fmt = GL_BGRA;
#ifdef GL_ARB_half_float_pixel
				if (FeatureAvailable[SAPPHIRE_ARB_half_float_pixel])
					type = GL_HALF_FLOAT_ARB;
				else
#endif
				{
					type = GL_FLOAT;
					format = ECF_A32B32G32R32F;
				}
				break;
			case ECF_R32F:
				if (FeatureAvailable[SAPPHIRE_ARB_texture_rg])
					fmt = GL_RED;
				else
					fmt = GL_LUMINANCE;
				type = GL_FLOAT;
				break;
			case ECF_G32R32F:
#ifdef GL_ARB_texture_rg
				if (FeatureAvailable[SAPPHIRE_ARB_texture_rg])
					fmt = GL_RG;
				else
#endif
					fmt = GL_LUMINANCE_ALPHA;
				type = GL_FLOAT;
				break;
			case ECF_A32B32G32R32F:
				fmt = GL_BGRA;
				type = GL_FLOAT;
				break;
			default:
				fmt = GL_BGRA;
				type = GL_UNSIGNED_BYTE;
				break;
			}
			IImage* newImage = createImage(format, ScreenSize);

			UINT8* pixels = 0;
			if (newImage)
				pixels = static_cast<UINT8*>(newImage->lock());
			if (pixels)
			{
				GLenum tgt = GL_FRONT;
				switch (target)
				{
				case ERT_FRAME_BUFFER:
					break;
				case ERT_STEREO_LEFT_BUFFER:
					tgt = GL_FRONT_LEFT;
					break;
				case ERT_STEREO_RIGHT_BUFFER:
					tgt = GL_FRONT_RIGHT;
					break;
				default:
					tgt = GL_AUX0 + (target - ERT_AUX_BUFFER0);
					break;
				}
				glReadBuffer(tgt);
				glReadPixels(0, 0, ScreenSize.Width, ScreenSize.Height, fmt, type, pixels);
				testGLError();
				glReadBuffer(GL_BACK);
			}

#ifdef GL_MESA_pack_invert
			if (FeatureAvailable[SAPPHIRE_MESA_pack_invert])
				glPixelStorei(GL_PACK_INVERT_MESA, GL_FALSE);
			else
#endif
				if (pixels)
				{
					// opengl images are horizontally flipped, so we have to fix that here.
					const SINT32 pitch = newImage->getPitch();
					UINT8* p2 = pixels + (ScreenSize.Height - 1) * pitch;
					UINT8* tmpBuffer = new UINT8[pitch];
					for (UINT32 i = 0; i < ScreenSize.Height; i += 2)
					{
						memcpy(tmpBuffer, pixels, pitch);
						//			for (UINT32 j=0; j<pitch; ++j)
						//			{
						//				pixels[j]=(UINT8)(p2[j]*255.f);
						//			}
						memcpy(pixels, p2, pitch);
						//			for (UINT32 j=0; j<pitch; ++j)
						//			{
						//				p2[j]=(UINT8)(tmpBuffer[j]*255.f);
						//			}
						memcpy(p2, tmpBuffer, pitch);
						pixels += pitch;
						p2 -= pitch;
					}
					delete[] tmpBuffer;
				}

			if (newImage)
			{
				newImage->unlock();
				if (testGLError() || !pixels)
				{
					newImage->drop();
					return 0;
				}
			}
			return newImage;
		}


		//! get depth texture for the given render target texture
		ITexture* COpenGLDriver::createDepthTexture(ITexture* texture, bool shared)
		{
			if ((texture->getDriverType() != EDT_OPENGL) || (!texture->isRenderTarget()))
				return 0;
			COpenGLTexture* tex = static_cast<COpenGLTexture*>(texture);

			if (!tex->isFrameBufferObject())
				return 0;

			if (shared)
			{
				for (UINT32 i = 0; i<DepthTextures.size(); ++i)
				{
					if (DepthTextures[i]->getSize() == texture->getSize())
					{
						DepthTextures[i]->grab();
						return DepthTextures[i];
					}
				}
				DepthTextures.push_back(new COpenGLFBODepthTexture(texture->getSize(), "depth1", this));
				return DepthTextures.back();
			}
			return (new COpenGLFBODepthTexture(texture->getSize(), "depth1", this));
		}


		void COpenGLDriver::removeDepthTexture(ITexture* texture)
		{
			for (UINT32 i = 0; i<DepthTextures.size(); ++i)
			{
				if (texture == DepthTextures[i])
				{
					DepthTextures.erase(DepthTextures.begin()+i);
					return;
				}
			}
		}


		//! Set/unset a clipping plane.
		bool COpenGLDriver::setClipPlane(UINT32 index, const Plane& plane, bool enable)
		{
			if (index >= MaxUserClipPlanes)
				return false;

			UserClipPlanes[index].Plane = plane;
			enableClipPlane(index, enable);
			return true;
		}


		void COpenGLDriver::uploadClipPlane(UINT32 index)
		{
			// opengl needs an array of doubles for the plane equation
			GLdouble clip_plane[4];
			clip_plane[0] = UserClipPlanes[index].Plane.normal.x;
			clip_plane[1] = UserClipPlanes[index].Plane.normal.y;
			clip_plane[2] = UserClipPlanes[index].Plane.normal.z;
			clip_plane[3] = UserClipPlanes[index].Plane.d;
			glClipPlane(GL_CLIP_PLANE0 + index, clip_plane);
		}


		//! Enable/disable a clipping plane.
		void COpenGLDriver::enableClipPlane(UINT32 index, bool enable)
		{
			if (index >= MaxUserClipPlanes)
				return;
			if (enable)
			{
				if (!UserClipPlanes[index].Enabled)
				{
					uploadClipPlane(index);
					glEnable(GL_CLIP_PLANE0 + index);
				}
			}
			else
				glDisable(GL_CLIP_PLANE0 + index);

			UserClipPlanes[index].Enabled = enable;
		}


		dimension2du COpenGLDriver::getMaxTextureSize() const
		{
			return dimension2du(MaxTextureSize, MaxTextureSize);
		}


		//! Convert E_PRIMITIVE_TYPE to OpenGL equivalent
		GLenum COpenGLDriver::primitiveTypeToGL(E_PRIMITIVE_TYPE type) const
		{
			switch (type)
			{
			case EPT_POINTS:
				return GL_POINTS;
			case EPT_LINE_STRIP:
				return GL_LINE_STRIP;
			case EPT_LINE_LOOP:
				return GL_LINE_LOOP;
			case EPT_LINES:
				return GL_LINES;
			case EPT_TRIANGLE_STRIP:
				return GL_TRIANGLE_STRIP;
			case EPT_TRIANGLE_FAN:
				return GL_TRIANGLE_FAN;
			case EPT_TRIANGLES:
				return GL_TRIANGLES;
			case EPT_QUAD_STRIP:
				return GL_QUAD_STRIP;
			case EPT_QUADS:
				return GL_QUADS;
			case EPT_POLYGON:
				return GL_POLYGON;
			case EPT_POINT_SPRITES:
#ifdef GL_ARB_point_sprite
				return GL_POINT_SPRITE_ARB;
#else
				return GL_POINTS;
#endif
			}
			return GL_TRIANGLES;
		}


		GLenum COpenGLDriver::getGLBlend(E_BLEND_FACTOR factor) const
		{
			GLenum r = 0;
			switch (factor)
			{
			case EBF_ZERO:			r = GL_ZERO; break;
			case EBF_ONE:			r = GL_ONE; break;
			case EBF_DST_COLOR:		r = GL_DST_COLOR; break;
			case EBF_ONE_MINUS_DST_COLOR:	r = GL_ONE_MINUS_DST_COLOR; break;
			case EBF_SRC_COLOR:		r = GL_SRC_COLOR; break;
			case EBF_ONE_MINUS_SRC_COLOR:	r = GL_ONE_MINUS_SRC_COLOR; break;
			case EBF_SRC_ALPHA:		r = GL_SRC_ALPHA; break;
			case EBF_ONE_MINUS_SRC_ALPHA:	r = GL_ONE_MINUS_SRC_ALPHA; break;
			case EBF_DST_ALPHA:		r = GL_DST_ALPHA; break;
			case EBF_ONE_MINUS_DST_ALPHA:	r = GL_ONE_MINUS_DST_ALPHA; break;
			case EBF_SRC_ALPHA_SATURATE:	r = GL_SRC_ALPHA_SATURATE; break;
			}
			return r;
		}

		GLenum COpenGLDriver::getZBufferBits() const
		{
			GLenum bits = 0;
			switch (Params.ZBufferBits)
			{
			case 16:
				bits = GL_DEPTH_COMPONENT16;
				break;
			case 24:
				bits = GL_DEPTH_COMPONENT24;
				break;
			case 32:
				bits = GL_DEPTH_COMPONENT32;
				break;
			default:
				bits = GL_DEPTH_COMPONENT;
				break;
			}
			return bits;
		}

#ifdef _SAPPHIRE_COMPILE_WITH_CG_
		const CGcontext& COpenGLDriver::getCgContext()
		{
			return CgContext;
		}
#endif


	
		// -----------------------------------
		// WINDOWS VERSION
		// -----------------------------------
#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
		IVideoDriver* createOpenGLDriver(const SSapphireCreationParameters& params,
			IFileSystem* io, CSapphireDeviceWin32* device)
		{
#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_
			COpenGLDriver* ogl = new COpenGLDriver(params, io, device);
			if (!ogl->initDriver(device))
			{
				ogl->drop();
				ogl = 0;
			}
			return ogl;
#else
			return 0;
#endif // _IRR_COMPILE_WITH_OPENGL_
		}
#endif // _IRR_COMPILE_WITH_WINDOWS_DEVICE_
}


#endif