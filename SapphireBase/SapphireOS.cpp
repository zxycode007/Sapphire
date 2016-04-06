#include "SapphirePrerequisites.h"
#include "SapphireOS.h"
#include "SapphireQMath.h"

#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_SDL)
#include <SDL/SDL_endian.h>
#define bswap_16(X) SDL_Swap16(X)
#define bswap_32(X) SDL_Swap32(X)
#elif (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32) && defined(_MSC_VER) && (_MSC_VER > 1298)
#include <stdlib.h>
#define bswap_16(X) _byteswap_ushort(X)
#define bswap_32(X) _byteswap_ulong(X)
#if (_MSC_VER >= 1400)
//#define localtime _localtime_s
#endif
#elif ( SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE)
#include <libkern/OSByteOrder.h>
#define bswap_16(X) OSReadSwapInt16(&X,0)
#define bswap_32(X) OSReadSwapInt32(&X,0)
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
#include <sys/endian.h>
#define bswap_16(X) bswap16(X)
#define bswap_32(X) bswap32(X)
#elif !( SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32) 
#include <byteswap.h>
#else
#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&0xFF00) >> 8))
#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
#endif

namespace Sapphire
{
	UINT16 Byteswap::byteswap(UINT16 num) { return bswap_16(num); }
	SINT16 Byteswap::byteswap(SINT16 num) { return bswap_16(num); }
	UINT32 Byteswap::byteswap(UINT32 num) { return bswap_32(num); }
	SINT32 Byteswap::byteswap(SINT32 num) { return bswap_32(num); }
	Real Byteswap::byteswap(Real num) { UINT32 tmp = IR(num); tmp = bswap_32(tmp); return (FR(tmp)); }
	// prevent accidental byte swapping of chars
	UINT8  Byteswap::byteswap(UINT8 num)  { return num; }
	c8  Byteswap::byteswap(c8 num)  { return num; }
}

#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
// ----------------------------------------------------------------
// Windows specific functions
// ----------------------------------------------------------------

#ifdef SAPPHIRE_XBOX_PLATFORM
#include <xtl.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#endif


namespace Sapphire
{

	//! prints a debuginfo string
	void Printer::print(const c8* message)
	{
#if defined (_WIN32_WCE )
		core::stringw tmp(message);
		tmp += L"\n";
		OutputDebugStringW(tmp.c_str());
#else
		String tmp(message);
		tmp += "\n";
		OutputDebugStringA(tmp.c_str());	
		printf("%s", tmp.c_str());
#endif
	}

	static LARGE_INTEGER HighPerformanceFreq;
	static BOOL HighPerformanceTimerSupport = FALSE;
	static BOOL MultiCore = FALSE;

	void Timer::initTimer(bool usePerformanceTimer)
	{
#if !defined(_WIN32_WCE) && !defined (SAPPHIRE_XBOX_PLATFORM)
		// workaround for hires timer on multiple core systems, bios bugs result in bad hires timers.
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		MultiCore = (sysinfo.dwNumberOfProcessors > 1);
#endif
		if (usePerformanceTimer)
			HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
		else
			HighPerformanceTimerSupport = FALSE;
		initVirtualTimer();
	}

	UINT32 Timer::getRealTime()
	{
		if (HighPerformanceTimerSupport)
		{
#if !defined(_WIN32_WCE) && !defined (SAPPHIRE_XBOX_PLATFORM)
			// Avoid potential timing inaccuracies across multiple cores by
			// temporarily setting the affinity of this process to one core.
			DWORD_PTR affinityMask = 0;
			if (MultiCore)
				affinityMask = SetThreadAffinityMask(GetCurrentThread(), 1);
#endif
			LARGE_INTEGER nTime;
			BOOL queriedOK = QueryPerformanceCounter(&nTime);

#if !defined(_WIN32_WCE)  && !defined (SAPPHIRE_XBOX_PLATFORM)
			// Restore the true affinity.
			if (MultiCore)
				(void)SetThreadAffinityMask(GetCurrentThread(), affinityMask);
#endif
			if (queriedOK)
				return UINT32((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

		}

		return GetTickCount();
	}

} // end namespace os


#else

// ----------------------------------------------------------------
// linux/ansi version
// ----------------------------------------------------------------

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

namespace Sapphire
{
	//! prints a debuginfo string
	void Printer::print(const c8* message)
	{
		printf("%s\n", message);
	}

	void Timer::initTimer(bool usePerformanceTimer)
	{
		initVirtualTimer();
	}

	UINT32 Timer::getRealTime()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return (UINT32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}

#endif // end linux / windows

	namespace Sapphire
	{
		// The platform independent implementation of the printer
		LogManager* Printer::Logger = LogManager::getSingletonPtr();

		void Printer::log(const c8* message, LogMessageLevel ll)
		{
			if (Logger)
				Logger->logMessage(message, ll);
		}

		void Printer::log(const String message, LogMessageLevel ll)
		{
			 
			if (Logger)
				Logger->logMessage(message, ll);
		}

		void Printer::log(const c8* message, const c8* hint, LogMessageLevel ll)
		{
			String empty = " ";
			String str = message + empty + hint;
			if (Logger)
				Logger->logMessage(str, ll);
		}

		void Printer::log(const c8* message, const path& hint, LogMessageLevel ll)
		{
			String empty = " ";
			String str = message + empty +hint;
			if (Logger)
				Logger->logMessage(str, ll);
		}

		// our Randomizer is not really os specific, so we
		// code one for all, which should work on every platform the same,
		// which is desireable.

		SINT32 Randomizer::seed = 0x0f0f0f0f;

		//! generates a pseudo random number
		SINT32 Randomizer::rand()
		{
			// (a*seed)%m with Schrage's method
			seed = a * (seed%q) - r* (seed / q);
			if (seed<0)
				seed += m;

			return seed;
		}

		//! generates a pseudo random number
		Real Randomizer::frand()
		{
			return rand()*(1.f / rMax);
		}

		SINT32 Randomizer::randMax()
		{
			return rMax;
		}

		//! resets the randomizer
		void Randomizer::reset(SINT32 value)
		{
			seed = value;
		}


		// ------------------------------------------------------
		// virtual timer implementation

		Real Timer::VirtualTimerSpeed = 1.0f;
		SINT32 Timer::VirtualTimerStopCounter = 0;
		UINT32 Timer::LastVirtualTime = 0;
		UINT32 Timer::StartRealTime = 0;
		UINT32 Timer::StaticTime = 0;

		//! Get real time and date in calendar form
		ITimer::RealTimeDate Timer::getRealTimeAndDate()
		{
			time_t rawtime;
			time(&rawtime);

			struct tm * timeinfo;
			timeinfo = localtime(&rawtime);

			// init with all 0 to indicate error
			ITimer::RealTimeDate date = { 0 };
			// at least Windows returns NULL on some illegal dates
			if (timeinfo)
			{
				// set useful values if succeeded
				date.Hour = (UINT32)timeinfo->tm_hour;
				date.Minute = (UINT32)timeinfo->tm_min;
				date.Second = (UINT32)timeinfo->tm_sec;
				date.Day = (UINT32)timeinfo->tm_mday;
				date.Month = (UINT32)timeinfo->tm_mon + 1;
				date.Year = (UINT32)timeinfo->tm_year + 1900;
				date.Weekday = (ITimer::EWeekday)timeinfo->tm_wday;
				date.Yearday = (UINT32)timeinfo->tm_yday + 1;
				date.IsDST = timeinfo->tm_isdst != 0;
			}
			return date;
		}

		//! returns current virtual time
		// 返回当前虚拟时间
		UINT32 Timer::getTime()
		{
			if (isStopped())
				return LastVirtualTime;

			return LastVirtualTime + (UINT32)((StaticTime - StartRealTime) * VirtualTimerSpeed);
		}

		//! ticks, advances the virtual timer
		void Timer::tick()
		{
			StaticTime = getRealTime();
		}

		//! sets the current virtual time
		void Timer::setTime(UINT32 time)
		{
			StaticTime = getRealTime();
			LastVirtualTime = time;
			StartRealTime = StaticTime;
		}

		//! stops the virtual timer
		void Timer::stopTimer()
		{
			if (!isStopped())
			{
				// stop the virtual timer
				LastVirtualTime = getTime();
			}

			--VirtualTimerStopCounter;
		}

		//! starts the virtual timer
		void Timer::startTimer()
		{
			++VirtualTimerStopCounter;

			if (!isStopped())
			{
				// restart virtual timer
				setTime(LastVirtualTime);
			}
		}

		//! sets the speed of the virtual timer
		void Timer::setSpeed(Real speed)
		{
			setTime(getTime());

			VirtualTimerSpeed = speed;
			if (VirtualTimerSpeed < 0.0f)
				VirtualTimerSpeed = 0.0f;
		}

		//! gets the speed of the virtual timer
		Real Timer::getSpeed()
		{
			return VirtualTimerSpeed;
		}

		//! returns if the timer currently is stopped
		bool Timer::isStopped()
		{
			return VirtualTimerStopCounter < 0;
		}

		void Timer::initVirtualTimer()
		{
			StaticTime = getRealTime();
			StartRealTime = StaticTime;
		}
}