#ifndef _SAPPHIRE_DEVICE_STUB_
#define _SAPPHIRE_DEVICE_STUB_

#include "SapphireDevice.h"
#include "SapphireIImagePresenter.h"
#include "SapphireSCreationParameters.h"
#include "SapphireCVideoModelList.h"

namespace Sapphire
{

	// lots of prototypes:
	class ILogger;
	class CLogger;
	class IRandomizer;

	namespace gui
	{
		class IGUIEnvironment;
		IGUIEnvironment* createGUIEnvironment(IFileSystem* fs,
			IVideoDriver* Driver, IOSOperator* op);
	}

	namespace scene
	{
		ISceneManager* createSceneManager(IVideoDriver* driver,
			IFileSystem* fs, ICursorControl* cc, IGUIEnvironment *gui);
	}

	namespace io
	{
		IFileSystem* createFileSystem();
	}

	namespace video
	{
		IVideoDriver* createSoftwareDriver(const dimension2d<UINT32>& windowSize,
			bool fullscreen, IFileSystem* io,
			IImagePresenter* presenter);
		IVideoDriver* createBurningVideoDriver(const SSapphireCreationParameters& params,
			IFileSystem* io, IImagePresenter* presenter);
		IVideoDriver* createNullDriver(IFileSystem* io, const dimension2d<UINT32>& screenSize);
	}



	//! Stub for an Irrlicht Device implementation
	class CSapphireDeviceStub : public Device
	{
	public:

		//! constructor
		CSapphireDeviceStub(const SSapphireCreationParameters& param);

		//! destructor
		virtual ~CSapphireDeviceStub();

		//! returns the video driver
		virtual IVideoDriver* getVideoDriver();

		//! return file system
		virtual IFileSystem* getFileSystem();

		//! returns the gui environment
		virtual IGUIEnvironment* getGUIEnvironment();

		//! returns the scene manager
		virtual ISceneManager* getSceneManager();

		//! \return Returns a pointer to the mouse cursor control interface.
		virtual ICursorControl* getCursorControl();

		//! Returns a pointer to a list with all video modes supported by the gfx adapter.
		virtual IVideoModeList* getVideoModeList();

		//! Returns a pointer to the ITimer object. With it the current Time can be received.
		virtual ITimer* getTimer();

		//! Returns the version of the engine.
		virtual const String getVersion() const;

		//! send the event to the right receiver
		virtual bool postEventFromUser(const SEvent& event);

		//! Sets a new event receiver to receive events
		virtual void setEventReceiver(IEventReceiver* receiver);

		//! Returns pointer to the current event receiver. Returns 0 if there is none.
		virtual IEventReceiver* getEventReceiver();

		//! Sets the input receiving scene manager.
		/** If set to null, the main scene manager (returned by GetSceneManager()) will receive the input */
		virtual void setInputReceivingSceneManager(ISceneManager* sceneManager);

		//! Returns a pointer to the logger.
		virtual ILogger* getLogger();

		//! Provides access to the engine's currently set randomizer.
		virtual IRandomizer* getRandomizer() const;

		//! Sets a new randomizer.
		virtual void setRandomizer(IRandomizer* r);

		//! Creates a new default randomizer.
		virtual IRandomizer* createDefaultRandomizer() const;

		//! Returns the operation system opertator object.
		virtual IOSOperator* getOSOperator();

		//! Checks if the window is running in fullscreen mode.
		virtual bool isFullscreen() const;

		//! get color format of the current window
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! Activate any joysticks, and generate events for them.
		virtual bool activateJoysticks(vector<SJoystickInfo>::type & joystickInfo);

		//! Set the current Gamma Value for the Display
		virtual bool setGammaRamp(FLOAT32 red, FLOAT32 green, FLOAT32 blue, FLOAT32 brightness, FLOAT32 contrast);

		//! Get the current Gamma Value for the Display
		virtual bool getGammaRamp(FLOAT32 &red, FLOAT32 &green, FLOAT32 &blue, FLOAT32 &brightness, FLOAT32 &contrast);

		//! Set the maximal elapsed time between 2 clicks to generate doubleclicks for the mouse. It also affects tripleclick behavior.
		//! When set to 0 no double- and tripleclicks will be generated.
		virtual void setDoubleClickTime(UINT32 timeMs);

		//! Get the maximal elapsed time between 2 clicks to generate double- and tripleclicks for the mouse.
		virtual UINT32 getDoubleClickTime() const;

		//! Remove all messages pending in the system message loop
		virtual void clearSystemMessages();


	protected:

		void createGUIAndScene();

		//! checks version of SDK and prints warning if there might be a problem
		bool checkVersion(const char* version);

		//! Compares to the last call of this function to return double and triple clicks.
		//! \return Returns only 1,2 or 3. A 4th click will start with 1 again.
		virtual UINT32 checkSuccessiveClicks(SINT32 mouseX, SINT32 mouseY, EMOUSE_INPUT_EVENT inputEvent);

		void calculateGammaRamp(UINT16 *ramp, FLOAT32 gamma, FLOAT32 relativebrightness, FLOAT32 relativecontrast);
		void calculateGammaFromRamp(FLOAT32 &gamma, const UINT16 *ramp);

		IVideoDriver* VideoDriver;
		IGUIEnvironment* GUIEnvironment;
		ISceneManager* SceneManager;
		ITimer* Timer;
		ICursorControl* CursorControl;
		IEventReceiver* UserReceiver;
		CLogger* Logger;
		IOSOperator* Operator;
		IRandomizer* Randomizer;
		IFileSystem* FileSystem;
		ISceneManager* InputReceivingSceneManager;

		struct SMouseMultiClicks
		{
			SMouseMultiClicks()
				: DoubleClickTime(500), CountSuccessiveClicks(0), LastClickTime(0), LastMouseInputEvent(EMIE_COUNT)
			{}

			UINT32 DoubleClickTime;
			UINT32 CountSuccessiveClicks;
			UINT32 LastClickTime;
			Position2d LastClick;
			EMOUSE_INPUT_EVENT LastMouseInputEvent;
		};
		SMouseMultiClicks MouseMultiClicks;
		CVideoModeList* VideoModeList;
		SSapphireCreationParameters CreationParams;
		bool Close;
	};

}

#endif