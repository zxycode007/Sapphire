#ifndef _SAPPHIRE_DEVICE_CONSOLE_
#define _SAPPHIRE_DEVICE_CONSOLE_

#include "SapphireCompileConifg.h"

#ifdef SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_

#include "SapphireSCreationParameters.h"
#include "SapphireDeviceStub.h"
#include "SapphireIImagePresenter.h"
// for console font
#include "SapphireFont.h"
#include "SapphireICursorControl.h"

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#if !defined(_SAPPHIRE_XBOX_PLATFORM_)
#include <windows.h>
#endif
#if(_WIN32_WINNT >= 0x0500)
#define _SAPPHIRE_WINDOWS_NT_CONSOLE_
#endif
#else
#include <time.h>
#endif

#ifndef _SAPPHIRE_WINDOWS_NT_CONSOLE_
#define _SAPPHIRE_VT100_CONSOLE_
#endif

namespace Sapphire
{
	

	class CSapphirDeviceConsole : public CSapphireDeviceStub, IImagePresenter
	{
	public:

		//! constructor
		CSapphirDeviceConsole(const SSapphireCreationParameters& params);

		//! destructor
		virtual ~CSapphirDeviceConsole();

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

		//! Sets if the window should be resizable in windowed mode.
		virtual void setResizable(bool resize = false);

		//! Minimizes the window.
		virtual void minimizeWindow();

		//! Maximizes the window.
		virtual void maximizeWindow();

		//! Restores the window size.
		virtual void restoreWindow();

		//! Get the device type
		virtual E_DEVICE_TYPE getType() const
		{
			return EIDT_CONSOLE;
		}

		void addPostPresentText(SINT16 X, SINT16 Y, const wchar_t *text);

		//! Implementation of the win32 console mouse cursor
		class CCursorControl : public ICursorControl
		{
		public:

			CCursorControl(const dimension2d<UINT32>& wsize)
				: WindowSize(wsize), InvWindowSize(0.0f, 0.0f), IsVisible(true), UseReferenceRect(false)
			{
				if (WindowSize.Width != 0)
					InvWindowSize.Width = 1.0f / WindowSize.Width;

				if (WindowSize.Height != 0)
					InvWindowSize.Height = 1.0f / WindowSize.Height;
			}

			//! Changes the visible state of the mouse cursor.
			virtual void setVisible(bool visible)
			{
				if (visible != IsVisible)
				{
					IsVisible = visible;
					setPosition(CursorPos.x, CursorPos.y);
				}
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
					setPosition((SINT32)(x*WindowSize.Width), (SINT32)(y*WindowSize.Height));
				else
					setPosition((SINT32)(x*ReferenceRect.getWidth()), (SINT32)(y*ReferenceRect.getHeight()));
			}

			//! Sets the new position of the cursor.
			//virtual void setPosition(const Position2d &pos)
			//{
			//	setPosition(pos.x, pos.y);
			//}

			//! Sets the new position of the cursor.
			virtual void setPosition(SINT32 x, SINT32 y)
			{
				setInternalCursorPosition(Position2d(x, y));
			}

			
			//! Returns the current position of the mouse cursor.
			virtual const Position2d& getPosition()
			{
				return CursorPos;
			}

			//! Returns the current position of the mouse cursor.
			virtual Position2d getRelativePosition()
			{
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


			//! Updates the internal cursor position
			void setInternalCursorPosition(const Position2d &pos)
			{
				CursorPos = pos;

				if (UseReferenceRect)
					CursorPos -= ReferenceRect.UpperLeftCorner;
			}

		private:

			Position2d  CursorPos;
			dimension2d<UINT32> WindowSize;
			dimension2d<FLOAT32> InvWindowSize;
			bool                   IsVisible,
				UseReferenceRect;
			rect<SINT32>        ReferenceRect;
		};

	private:

		//! Set the position of the text caret
		void setTextCursorPos(SINT16 x, SINT16 y);

		// text to be added after drawing the screen
		struct SPostPresentText
		{
			Position2d Pos;
			String         Text;
		};

		bool IsWindowFocused;

		vector<String>::type OutputBuffer;
		IGUIFont  *ConsoleFont;
		vector<SPostPresentText>::type Text;

		FILE *OutFile;

#ifdef _SAPPHIRE_WINDOWS_NT_CONSOLE_
		HANDLE WindowsSTDIn, WindowsSTDOut;
		UINT32 MouseButtonStates;
#endif
	};
}

#endif

#endif