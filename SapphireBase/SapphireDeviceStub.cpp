#include "SapphireDeviceStub.h"
#include "SapphireIScenesManager.h"
#include "SapphireIEventRecevier.h"
#include "SapphireIFileSystem.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireOS.h"
#include "SapphireCompileConifg.h"
#include "SapphireCTimer.h"
#include "SapphireLogManager.h"
#include "SapphireLog.h"
#include "SapphireIRandomizer.h"
#include "SapphireICursorControl.h"
#include "SapphireIOSOperator.h"
#include "SapphireIVideoDriver.h"

namespace Sapphire
{
	//! constructor
	CSapphireDeviceStub::CSapphireDeviceStub(const SSapphireCreationParameters& params)
		: Device(), VideoDriver(0), GUIEnvironment(0), SceneManager(0),
		Timer(0), CursorControl(0), UserReceiver(params.EventReceiver),
		Logger(0), Operator(0), Randomizer(0), FileSystem(0),
		InputReceivingSceneManager(0), VideoModeList(0),
		CreationParams(params), Close(false)
	{
		Timer = new CTimer(params.UsePerformanceTimer);
		if (Printer::Logger)
		{
			Printer::Logger->grab();
			Logger = Printer::Logger;
			//Logger->setReceiver(UserReceiver);
			Logger->getDefaultLog()->setReceiver(UserReceiver);
		}
		else
		{
			//Logger = new CLogger(UserReceiver);
			Logger = SAPPHIRE_NEW LogManager();
			LogManager::getSingletonPtr()->createLog("sapphire.log");
			Logger->getDefaultLog()->setReceiver(UserReceiver);
			Printer::Logger = Logger;
		}
		//Logger->setLogLevel(CreationParams.LoggingLevel);
		Logger->setLogDetail(CreationParams.LogLevel);

		Printer::Logger = Logger;
		Randomizer = createDefaultRandomizer();

		FileSystem = createFileSystem();
		VideoModeList = new CVideoModeList();

		String s = "Sapphire Engine version ";
		s.append(getVersion());
		Printer::log(s.c_str(), LML_NORMAL);

		checkVersion(StringUtil::int32ToString(params.SDK_version_do_not_use).c_str());
	}


	CSapphireDeviceStub::~CSapphireDeviceStub()
	{
		VideoModeList->drop();
		FileSystem->drop();

		if (GUIEnvironment)
			GUIEnvironment->drop();

		if (VideoDriver)
			VideoDriver->drop();

		if (SceneManager)
			SceneManager->drop();

		if (InputReceivingSceneManager)
			InputReceivingSceneManager->drop();

		if (CursorControl)
			CursorControl->drop();

		if (Operator)
			Operator->drop();

		if (Randomizer)
			Randomizer->drop();

		CursorControl = 0;

		if (Timer)
			Timer->drop();

		if (Logger->drop())
			Printer::Logger = 0;
	}


	void CSapphireDeviceStub::createGUIAndScene()
	{
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
		// create gui environment
		GUIEnvironment = createGUIEnvironment(FileSystem, VideoDriver, Operator);
#endif

		// create Scene manager
		SceneManager = createSceneManager(VideoDriver, FileSystem, CursorControl, GUIEnvironment);

		setEventReceiver(UserReceiver);
	}


	//! returns the video driver
	IVideoDriver* CSapphireDeviceStub::getVideoDriver()
	{
		return VideoDriver;
	}



	//! return file system
	IFileSystem* CSapphireDeviceStub::getFileSystem()
	{
		return FileSystem;
	}



	//! returns the gui environment
	IGUIEnvironment* CSapphireDeviceStub::getGUIEnvironment()
	{
		return GUIEnvironment;
	}



	//! returns the scene manager
	ISceneManager* CSapphireDeviceStub::getSceneManager()
	{
		return SceneManager;
	}


	//! \return Returns a pointer to the ITimer object. With it the
	//! current Time can be received.
	ITimer* CSapphireDeviceStub::getTimer()
	{
		return Timer;
	}


	//! Returns the version of the engine.
	const String CSapphireDeviceStub::getVersion() const
	{
		return  StringUtil::int32ToString(SAPPHIRE_VERSION);
	}

	//! \return Returns a pointer to the mouse cursor control interface.
	ICursorControl* CSapphireDeviceStub::getCursorControl()
	{
		return CursorControl;
	}


	//! \return Returns a pointer to a list with all video modes supported
	//! by the gfx adapter.
	IVideoModeList* CSapphireDeviceStub::getVideoModeList()
	{
		return VideoModeList;
	}


	//! checks version of sdk and prints warning if there might be a problem
	bool CSapphireDeviceStub::checkVersion(const char* version)
	{
		if (strcmp(getVersion().c_str(), version))
		{
			String w;
			w = "Warning: The library version of the Sapphire Engine (";
			w += getVersion();
			w += ") does not match the version the application was compiled with (";
			w += version;
			w += "). This may cause problems.";
			Printer::log(w.c_str(), LML_WARNING);
		
			return false;
		}

		return true;
	}


	//! Compares to the last call of this function to return double and triple clicks.
	UINT32 CSapphireDeviceStub::checkSuccessiveClicks(SINT32 mouseX, SINT32 mouseY, EMOUSE_INPUT_EVENT inputEvent)
	{
		const SINT32 MAX_MOUSEMOVE = 3;

		UINT32 clickTime = getTimer()->getRealTime();

		if ((clickTime - MouseMultiClicks.LastClickTime) < MouseMultiClicks.DoubleClickTime
			&& Math::Abs(MouseMultiClicks.LastClick.x - mouseX) <= MAX_MOUSEMOVE
			&& Math::Abs(MouseMultiClicks.LastClick.y - mouseY) <= MAX_MOUSEMOVE
			&& MouseMultiClicks.CountSuccessiveClicks < 3
			&& MouseMultiClicks.LastMouseInputEvent == inputEvent
			)
		{
			++MouseMultiClicks.CountSuccessiveClicks;
		}
		else
		{
			MouseMultiClicks.CountSuccessiveClicks = 1;
		}

		MouseMultiClicks.LastMouseInputEvent = inputEvent;
		MouseMultiClicks.LastClickTime = clickTime;
		MouseMultiClicks.LastClick.x = mouseX;
		MouseMultiClicks.LastClick.y = mouseY;

		return MouseMultiClicks.CountSuccessiveClicks;
	}


	//! send the event to the right receiver
	bool CSapphireDeviceStub::postEventFromUser(const SEvent& event)
	{
		bool absorbed = false;

		if (UserReceiver)
			absorbed = UserReceiver->OnEvent(event);

		if (!absorbed && GUIEnvironment)
			absorbed = GUIEnvironment->postEventFromUser(event);

		ISceneManager* inputReceiver = InputReceivingSceneManager;
		if (!inputReceiver)
			inputReceiver = SceneManager;

		if (!absorbed && inputReceiver)
			absorbed = inputReceiver->postEventFromUser(event);

		
		return absorbed;
	}


	//! Sets a new event receiver to receive events
	void CSapphireDeviceStub::setEventReceiver(IEventReceiver* receiver)
	{
		UserReceiver = receiver;
		//Logger->setReceiver(receiver);
		Logger->getDefaultLog()->setReceiver(receiver);
		if (GUIEnvironment)
			GUIEnvironment->setUserEventReceiver(receiver);
	}


	//! Returns poinhter to the current event receiver. Returns 0 if there is none.
	IEventReceiver* CSapphireDeviceStub::getEventReceiver()
	{
		return UserReceiver;
	}


	//! \return Returns a pointer to the logger.
	LogManager* CSapphireDeviceStub::getLogger()
	{
		return Logger;
	}


	//! Returns the operation system opertator object.
	IOSOperator* CSapphireDeviceStub::getOSOperator()
	{
		return Operator;
	}


	//! Provides access to the engine's currently set randomizer.
	IRandomizer* CSapphireDeviceStub::getRandomizer() const
	{
		return Randomizer;
	}

	//! Sets a new randomizer.
	void CSapphireDeviceStub::setRandomizer(IRandomizer* r)
	{
		if (r != Randomizer)
		{
			if (Randomizer)
				Randomizer->drop();
			Randomizer = r;
			if (Randomizer)
				Randomizer->grab();
		}
	}

	namespace
	{
		struct SDefaultRandomizer : public IRandomizer
		{
			virtual void reset(SINT32 value = 0x0f0f0f0f)
			{
				Randomizer::reset(value);
			}

			virtual SINT32 rand() const
			{
				return Randomizer::rand();
			}

			virtual FLOAT32 frand() const
			{
				return Randomizer::frand();
			}

			virtual SINT32 randMax() const
			{
				return Randomizer::randMax();
			}
		};
	}

	//! Creates a new default randomizer.
	IRandomizer* CSapphireDeviceStub::createDefaultRandomizer() const
	{
		IRandomizer* r = new SDefaultRandomizer();
		if (r)
			r->reset();
		return r;
	}


	//! Sets the input receiving scene manager.
	void CSapphireDeviceStub::setInputReceivingSceneManager(ISceneManager* sceneManager)
	{
		if (sceneManager)
			sceneManager->grab();
		if (InputReceivingSceneManager)
			InputReceivingSceneManager->drop();

		InputReceivingSceneManager = sceneManager;
	}


	//! Checks if the window is running in fullscreen mode
	bool CSapphireDeviceStub::isFullscreen() const
	{
		return CreationParams.Fullscreen;
	}


	//! returns color format
	ECOLOR_FORMAT CSapphireDeviceStub::getColorFormat() const
	{
		return ECF_R5G6B5;
	}

	//! No-op in this implementation
	bool CSapphireDeviceStub::activateJoysticks(vector<SJoystickInfo>::type & joystickInfo)
	{
		return false;
	}

	/*!
	*/
	void CSapphireDeviceStub::calculateGammaRamp(UINT16 *ramp, FLOAT32 gamma, FLOAT32 relativebrightness, FLOAT32 relativecontrast)
	{
		SINT32 i;
		SINT32 value;
		SINT32 rbright = (SINT32)(relativebrightness * (65535.f / 4));
		FLOAT32 rcontrast = 1.f / (255.f - (relativecontrast * 127.5f));

		gamma = gamma > 0.f ? 1.0f / gamma : 0.f;

		for (i = 0; i < 256; ++i)
		{
			value = (SINT32)(pow(rcontrast * i, gamma)*65535.f + 0.5f);
			ramp[i] = (UINT16)SINT32_clamp(value + rbright, 0, 65535);
		}

	}

	void CSapphireDeviceStub::calculateGammaFromRamp(FLOAT32 &gamma, const UINT16 *ramp)
	{
		/* The following is adapted from a post by Garrett Bass on OpenGL
		Gamedev list, March 4, 2000.
		*/
		FLOAT32 sum = 0.0;
		SINT32 i, count = 0;

		gamma = 1.0;
		for (i = 1; i < 256; ++i) {
			if ((ramp[i] != 0) && (ramp[i] != 65535)) {
				FLOAT32 B = (FLOAT32)i / 256.f;
				FLOAT32 A = ramp[i] / 65535.f;
				sum += (FLOAT32)(logf(A) / logf(B));
				count++;
			}
		}
		if (count && sum) {
			gamma = 1.0f / (sum / count);
		}

	}

	//! Set the current Gamma Value for the Display
	bool CSapphireDeviceStub::setGammaRamp(FLOAT32 red, FLOAT32 green, FLOAT32 blue, FLOAT32 brightness, FLOAT32 contrast)
	{
		return false;
	}

	//! Get the current Gamma Value for the Display
	bool CSapphireDeviceStub::getGammaRamp(FLOAT32 &red, FLOAT32 &green, FLOAT32 &blue, FLOAT32 &brightness, FLOAT32 &contrast)
	{
		return false;
	}

	//! Set the maximal elapsed time between 2 clicks to generate doubleclicks for the mouse. It also affects tripleclick behavior.
	void CSapphireDeviceStub::setDoubleClickTime(UINT32 timeMs)
	{
		MouseMultiClicks.DoubleClickTime = timeMs;
	}

	//! Get the maximal elapsed time between 2 clicks to generate double- and tripleclicks for the mouse.
	UINT32 CSapphireDeviceStub::getDoubleClickTime() const
	{
		return MouseMultiClicks.DoubleClickTime;
	}

	//! Remove all messages pending in the system message loop
	void CSapphireDeviceStub::clearSystemMessages()
	{
	}


}