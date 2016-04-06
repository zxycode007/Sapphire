//#include "SapphireStableHeaders.h"

#include "SapphireLog.h"
#include "SapphireLogManager.h"
#include "SapphireString.h"
#include "SapphireIEventRecevier.h"
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_NACL
#   include "ppapi/cpp/var.h"
#   include "ppapi/cpp/instance.h"
#endif

namespace Sapphire
{
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_NACL
	pp::Instance* Log::mInstance = NULL;
#endif

	//-----------------------------------------------------------------------
	Log::Log(const String& name, bool debuggerOuput, bool suppressFile) :
		mLogLevel(LL_NORMAL), mDebugOut(debuggerOuput),
		mSuppressFile(suppressFile), mTimeStamp(true), mLogName(name)
	{
		if (!mSuppressFile)
		{
			mLog.open(name.c_str());
		}
	}
	//-----------------------------------------------------------------------
	Log::Log(const String& name, IEventReceiver* recevier, bool debuggerOuput, bool suppressFile) :
		mLogLevel(LL_NORMAL), mDebugOut(debuggerOuput),
		mSuppressFile(suppressFile), mTimeStamp(true), mLogName(name)
	{
		if (!mSuppressFile)
		{
			mLog.open(name.c_str());
		}
		if (recevier)
		{
			Receiver = recevier;				 
		}
	}
	//-----------------------------------------------------------------------
	Log::~Log()
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			if (!mSuppressFile)
			{
				mLog.close();
			}
	}
	//-----------------------------------------------------------------------
	void Log::logMessage(const String& message, LogMessageLevel lml, bool maskDebug)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			if ((mLogLevel + lml) >= SAPPHIRE_LOG_THRESHOLD)
			{
				bool skipThisMessage = false;
				for (mtLogListener::iterator i = mListeners.begin(); i != mListeners.end(); ++i)
					(*i)->messageLogged(message, lml, maskDebug, mLogName, skipThisMessage);

				if (!skipThisMessage)
				{
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_NACL
					if (mInstance != NULL)
					{
						mInstance->PostMessage(message.c_str());
					}
#else
					if (mDebugOut && !maskDebug)
						std::cerr << message << std::endl;
#endif
					if (Receiver)
					{
						SEvent event;
						event.EventType = EET_LOG_TEXT_EVENT;
						event.LogEvent.Text = message.c_str();
						event.LogEvent.Level = lml;
						if (Receiver->OnEvent(event));
					}
					
					// 写出时间到日志
					if (!mSuppressFile)
					{
						if (mTimeStamp)
						{
							struct tm *pTime;
							time_t ctTime; time(&ctTime);
							pTime = localtime(&ctTime);
							mLog << std::setw(2) << std::setfill('0') << pTime->tm_hour
								<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
								<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
								<< ": ";
						}
						mLog << message << std::endl;
						 
						mLog.flush();
					}
				}
			}
		
	}

	//-----------------------------------------------------------------------
	void Log::setTimeStampEnabled(bool timeStamp)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			mTimeStamp = timeStamp;
	}

	//-----------------------------------------------------------------------
	void Log::setDebugOutputEnabled(bool debugOutput)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			mDebugOut = debugOutput;
	}

	//-----------------------------------------------------------------------
	void Log::setLogDetail(LoggingLevel ll)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			mLogLevel = ll;
	}

	//-----------------------------------------------------------------------
	void Log::addListener(LogListener* listener)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			mListeners.push_back(listener);
	}

	//-----------------------------------------------------------------------
	void Log::removeListener(LogListener* listener)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			mListeners.erase(std::find(mListeners.begin(), mListeners.end(), listener));
	}
	//---------------------------------------------------------------------
	Log::Stream Log::stream(LogMessageLevel lml, bool maskDebug)
	{
		return Stream(this, lml, maskDebug);

	}
	void Log::setReceiver(IEventReceiver* r)	{
		Receiver = r;
	}
}
