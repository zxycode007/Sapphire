
//#include "SapphireStableHeaders.h"

#include "SapphireLogManager.h"
#include "SapphireIEventRecevier.h"
#include "SapphireException.h"
#include <algorithm>
namespace Sapphire { 

	//-----------------------------------------------------------------------
	//模板特化 为静态变量初始化
	template<> LogManager* Singleton<LogManager>::msSingleton = 0;


	LogManager* LogManager::getSingletonPtr(void)
	{
		return msSingleton;
	}
	LogManager& LogManager::getSingleton(void)
	{
		assert(msSingleton);  return (*msSingleton);
	}
	//-----------------------------------------------------------------------
	LogManager::LogManager()
	{
		mDefaultLog = NULL;
	}
	//-----------------------------------------------------------------------
	LogManager::~LogManager()
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			
			LogList::iterator i;
		for (i = mLogs.begin(); i != mLogs.end(); ++i)
		{
			SAPPHIRE_DELETE i->second;
		}
	}
	//-----------------------------------------------------------------------
	Log* LogManager::createLog(const String& name, bool defaultLog, bool debuggerOutput,
		bool suppressFileOutput)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX

			Log* newLog = SAPPHIRE_NEW Log(name, debuggerOutput, suppressFileOutput);

		if (!mDefaultLog || defaultLog)
		{
			mDefaultLog = newLog;
		}

		mLogs.insert(LogList::value_type(name, newLog));

		return newLog;
	}
	//-----------------------------------------------------------------------
	Log* LogManager::createLog(const String& name, IEventReceiver* recevier, bool defaultLog, bool debuggerOutput,
		bool suppressFileOutput)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX

			Log* newLog = SAPPHIRE_NEW Log(name, recevier, debuggerOutput, suppressFileOutput);

		if (!mDefaultLog || defaultLog)
		{
			mDefaultLog = newLog;
		}

		mLogs.insert(LogList::value_type(name, newLog));

		return newLog;
	}
	//-----------------------------------------------------------------------
	Log* LogManager::getDefaultLog()
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			return mDefaultLog;
	}
	//-----------------------------------------------------------------------
	Log* LogManager::setDefaultLog(Log* newLog)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			Log* oldLog = mDefaultLog;
		mDefaultLog = newLog;
		return oldLog;
	}
	//-----------------------------------------------------------------------
	Log* LogManager::getLog(const String& name)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			LogList::iterator i = mLogs.find(name);
		if (i != mLogs.end())
			return i->second;
		else
			SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Log not found. ", "LogManager::getLog");


	}
	//-----------------------------------------------------------------------
	void LogManager::destroyLog(const String& name)
	{
		LogList::iterator i = mLogs.find(name);
		if (i != mLogs.end())
		{
			if (mDefaultLog == i->second)
			{
				mDefaultLog = 0;
			}
			SAPPHIRE_DELETE i->second;
			mLogs.erase(i);
		}

		//如果它没有被移除， 设置其他默认的日志
		if (!mDefaultLog && !mLogs.empty())
		{
			mDefaultLog = mLogs.begin()->second;
		}
	}
	//-----------------------------------------------------------------------
	void LogManager::destroyLog(Log* log)
	{
		destroyLog(log->getName());
	}
	//-----------------------------------------------------------------------
	void LogManager::logMessage(const String& message, LogMessageLevel lml, bool maskDebug)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			if (mDefaultLog)
			{
				mDefaultLog->logMessage(message, lml, maskDebug);
			}
	}
	//-----------------------------------------------------------------------
	void LogManager::setLogDetail(LoggingLevel ll)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			if (mDefaultLog)
			{
				mDefaultLog->setLogDetail(ll);
			}
	}
	//---------------------------------------------------------------------
	Log::Stream LogManager::stream(LogMessageLevel lml, bool maskDebug)
	{
		SAPPHIRE_LOCK_AUTO_MUTEX
			if (mDefaultLog)
				return mDefaultLog->stream(lml, maskDebug);
			else
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Default log not found. ", "LogManager::stream");  //抛出异常

	}
}
