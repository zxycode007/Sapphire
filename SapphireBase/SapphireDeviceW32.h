#ifndef _SAPPHIRE_DEVICE_W32_
#define _SAPPHIRE_DEVICE_W32_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
#include "SapphireDeviceStub.h"
#include "SapphireDevice.h"
#include "SapphireIImagePresenter.h"
#include "SapphireICursorControl.h"

#define WIN32_LEAN_AND_MEAN
#if !defined(_SAPPHIRE_XBOX_PLATFORM_)
#include <windows.h>
#include <mmsystem.h> // For JOYCAPS
#include <windowsx.h>
#endif
#if !defined(GET_X_LPARAM)
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

namespace Sapphire
{
	struct SJoystickWin32Control;
	class ITexture;

	class CSapphireDeviceWin32 : public CSapphireDeviceStub, IImagePresenter
	{
		friend struct SJoystickWin32Control;
	public:

		//! constructor
		CSapphireDeviceWin32(const SSapphireCreationParameters& params);

		//! destructor
		virtual ~CSapphireDeviceWin32();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run();

		//! Cause the device to temporarily pause execution and let other processes to run
		// This should bring down processor usage without major performance loss for Irrlicht
		virtual void yield();

		//! Pause execution and let other processes to run for a specified amount of time.
		virtual void sleep(UINT32 timeMs, bool pauseTimer);

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text);

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive() const;

		//! returns if window has focus
		virtual bool isWindowFocused() const;

		//! returns if window is minimized
		virtual bool isWindowMinimized() const;

		//! presents a surface in the client area
		virtual bool present(IImage* surface, void* windowId = 0, rect<SINT32>* src = 0);

		//! notifies the device that it should close itself
		virtual void closeDevice();

		//! \return Returns a pointer to a list with all video modes
		//! supported by the gfx adapter.
		IVideoModeList* getVideoModeList();

		//! Notifies the device, that it has been resized
		void OnResized();

		//! Sets if the window should be resizable in windowed mode.
		virtual void setResizable(bool resize = false);

		//! Minimizes the window.
		virtual void minimizeWindow();

		//! Maximizes the window.
		virtual void maximizeWindow();

		//! Restores the window size.
		virtual void restoreWindow();

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(vector<SJoystickInfo>::type & joystickInfo);

		//! Set the current Gamma Value for the Display
		virtual bool setGammaRamp(FLOAT32 red, FLOAT32 green, FLOAT32 blue, FLOAT32 brightness, FLOAT32 contrast);

		//! Get the current Gamma Value for the Display
		virtual bool getGammaRamp(FLOAT32 &red, FLOAT32 &green, FLOAT32 &blue, FLOAT32 &brightness, FLOAT32 &contrast);

		//! Remove all messages pending in the system message loop
		virtual void clearSystemMessages();

		//! Get the device type
		virtual E_DEVICE_TYPE getType() const
		{
			return EIDT_WIN32;
		}

		//! Compares to the last call of this function to return double and triple clicks.
		//! \return Returns only 1,2 or 3. A 4th click will start with 1 again.
		virtual UINT32 checkSuccessiveClicks(SINT32 mouseX, SINT32 mouseY, EMOUSE_INPUT_EVENT inputEvent)
		{
			// we just have to make it public
			return CSapphireDeviceStub::checkSuccessiveClicks(mouseX, mouseY, inputEvent);
		}

		//! switchs to fullscreen
		bool switchToFullScreen(bool reset = false);

		//! Check for and show last Windows API error to help internal debugging.
		//! Does call GetLastError and on errors formats the errortext and displays it in a messagebox.
		static void ReportLastWinApiError();

		// convert an Irrlicht texture to a windows cursor
		HCURSOR TextureToCursor(HWND hwnd, ITexture * tex, const rect<SINT32>& sourceRect, const Position2d &hotspot);

		//! Implementation of the win32 cursor control
		class CCursorControl : public ICursorControl
		{
		public:

			CCursorControl(CSapphireDeviceWin32* device, const dimension2d<UINT32>& wsize, HWND hwnd, bool fullscreen);
			~CCursorControl();

			//! Changes the visible state of the mouse cursor.
			virtual void setVisible(bool visible)
			{
				CURSORINFO info;
				info.cbSize = sizeof(CURSORINFO);
				BOOL gotCursorInfo = GetCursorInfo(&info);
				while (gotCursorInfo)
				{
#ifdef CURSOR_SUPPRESSED
					// new flag for Windows 8, where cursor
					// might be suppressed for touch interface
					if (info.flags == CURSOR_SUPPRESSED)
					{
						visible = false;
						break;
					}
#endif
					if ((visible && info.flags == CURSOR_SHOWING) || // visible
						(!visible && info.flags == 0)) // hidden
					{
						break;
					}
					// this only increases an internal
					// display counter in windows, so it
					// might have to be called some more
					const int showResult = ShowCursor(visible);
					// if result has correct sign we can
					// stop here as well
					if ((!visible && showResult < 0) ||
						(visible && showResult >= 0))
						break;
					// yes, it really must be set each time
					info.cbSize = sizeof(CURSORINFO);
					gotCursorInfo = GetCursorInfo(&info);
				}
				IsVisible = visible;
			}

			//! Returns if the cursor is currently visible.
			virtual bool isVisible() const
			{
				
				return IsVisible;
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(const Position2d &pos)
			{
				setPosition(pos.x, pos.y);
			}

			//! Sets the new position of the cursor.
			virtual void setPosition(FLOAT32 x, FLOAT32 y)
			{
				if (!UseReferenceRect)
					setPosition(round32(x*WindowSize.Width), round32(y*WindowSize.Height));
				else
					setPosition(round32(x*ReferenceRect.getWidth()), round32(y*ReferenceRect.getHeight()));
			}

			//! Sets the new position of the cursor.
			//virtual void setPosition(const Position2d &pos)
			//{
			//	setPosition(pos.X, pos.Y);
			//}

			//! Sets the new position of the cursor.
			virtual void setPosition(SINT32 x, SINT32 y)
			{
				if (UseReferenceRect)
				{
					SetCursorPos(ReferenceRect.UpperLeftCorner.x + x,
						ReferenceRect.UpperLeftCorner.y + y);
				}
				else
				{
					RECT rect;
					if (GetWindowRect(HWnd, &rect))
						SetCursorPos(x + rect.left + BorderX, y + rect.top + BorderY);
				}

				CursorPos.x = x;
				CursorPos.y = y;
			}

			//! Returns the current position of the mouse cursor.
			virtual const Position2d& getPosition()
			{
				updateInternalCursorPosition();
				return CursorPos;
			}

			//! Returns the current position of the mouse cursor.
			virtual Position2d getRelativePosition()
			{
				updateInternalCursorPosition();

				if (!UseReferenceRect)
				{
					return Position2d(CursorPos.x * InvWindowSize.Width,
						CursorPos.y * InvWindowSize.Height);
				}

				return Position2d(CursorPos.x / (FLOAT32)ReferenceRect.getWidth(),
					CursorPos.y / (FLOAT32)ReferenceRect.getHeight());
			}

			//! Sets an absolute reference rect for calculating the cursor position.
			virtual void setReferenceRect(rect<SINT32>* rect = 0)
			{
				if (rect)
				{
					ReferenceRect = *rect;
					UseReferenceRect = true;

					// prevent division through zero and uneven sizes

					if (!ReferenceRect.getHeight() || ReferenceRect.getHeight() % 2)
						ReferenceRect.LowerRightCorner.y += 1;

					if (!ReferenceRect.getWidth() || ReferenceRect.getWidth() % 2)
						ReferenceRect.LowerRightCorner.x += 1;
				}
				else
					UseReferenceRect = false;
			}

			/** Used to notify the cursor that the window was resized. */
			virtual void OnResize(const dimension2d<UINT32>& size)
			{
				WindowSize = size;
				if (size.Width != 0)
					InvWindowSize.Width = 1.0f / size.Width;
				else
					InvWindowSize.Width = 0.f;

				if (size.Height != 0)
					InvWindowSize.Height = 1.0f / size.Height;
				else
					InvWindowSize.Height = 0.f;
			}

			/** Used to notify the cursor that the window resizable settings changed. */
			void updateBorderSize(bool fullscreen, bool resizable)
			{
				if (!fullscreen)
				{
					if (resizable)
					{
						BorderX = GetSystemMetrics(SM_CXSIZEFRAME);
						BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME);
					}
					else
					{
						BorderX = GetSystemMetrics(SM_CXDLGFRAME);
						BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
					}
				}
				else
				{
					BorderX = BorderY = 0;
				}
			}


			//! Sets the active cursor icon
			virtual void setActiveIcon(ECURSOR_ICON iconId);

			//! Gets the currently active icon
			virtual ECURSOR_ICON getActiveIcon() const
			{
				return ActiveIcon;
			}

			//! Add a custom sprite as cursor icon.
			virtual ECURSOR_ICON addIcon(const SCursorSprite& icon);

			//! replace the given cursor icon.
			virtual void changeIcon(ECURSOR_ICON iconId, const SCursorSprite& icon);

			//! Return a system-specific size which is supported for cursors. Larger icons will fail, smaller icons might work.
			virtual dimension2di getSupportedIconSize() const;

			void update();

		private:

			//! Updates the internal cursor position
			void updateInternalCursorPosition()
			{
				POINT p;
				if (!GetCursorPos(&p))
				{
					DWORD xy = GetMessagePos();
					p.x = GET_X_LPARAM(xy);
					p.y = GET_Y_LPARAM(xy);
				}

				if (UseReferenceRect)
				{
					CursorPos.x = p.x - ReferenceRect.UpperLeftCorner.x;
					CursorPos.y = p.y - ReferenceRect.UpperLeftCorner.y;
				}
				else
				{
					RECT rect;
					if (GetWindowRect(HWnd, &rect))
					{
						CursorPos.x = p.x - rect.left - BorderX;
						CursorPos.y = p.y - rect.top - BorderY;
					}
					else
					{
						// window seems not to be existent, so set cursor to
						// a negative value
						CursorPos.x = -1;
						CursorPos.y = -1;
					}
				}
			}

			CSapphireDeviceWin32* Device;
			Position2d CursorPos;
			dimension2d<UINT32> WindowSize;
			dimension2d<FLOAT32> InvWindowSize;
			HWND HWnd;

			SINT32 BorderX, BorderY;
			rect<SINT32> ReferenceRect;
			bool UseReferenceRect;
			bool IsVisible;


			struct CursorFrameW32
			{
				CursorFrameW32() : IconHW(0) {}
				CursorFrameW32(HCURSOR icon) : IconHW(icon) {}

				HCURSOR IconHW;	// hardware cursor
			};

			struct CursorW32
			{
				CursorW32() {}
				explicit CursorW32(HCURSOR iconHw, UINT32 frameTime = 0) : FrameTime(frameTime)
				{
					Frames.push_back(CursorFrameW32(iconHw));
				}
				vector<CursorFrameW32>::type Frames;
				UINT32 FrameTime;
			};

			vector<CursorW32>::type Cursors;
			ECURSOR_ICON ActiveIcon;
			UINT32 ActiveIconStartTime;

			void initCursors();
		};

		//! returns the win32 cursor control
		CCursorControl* getWin32CursorControl();

	private:

		//! create the driver
		void createDriver();

		//! Process system events
		void handleSystemMessages();

		void getWindowsVersion(String& version);

		void resizeIfNecessary();

		HWND HWnd;

		bool ChangedToFullScreen;
		bool Resized;
		bool ExternalWindow;
		CCursorControl* Win32CursorControl;
		DEVMODE DesktopMode;

		SJoystickWin32Control* JoyControl;
	};
}


#endif

#endif