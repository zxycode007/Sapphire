#ifndef _SAPPHIRE_OS_
#define _SAPPHIRE_OS_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"
#include "SapphireLogManager.h"
#include "SapphireLog.h"
#include "SapphireITimer.h"



namespace Sapphire
{
	class Byteswap
	{
	public:
		static UINT16 byteswap(UINT16 num);
		static SINT16 byteswap(SINT16 num);
		static UINT32 byteswap(UINT32 num);
		static SINT32 byteswap(SINT32 num);
		static Real byteswap(Real num);
		// prevent accidental swapping of chars
		static UINT8  byteswap(UINT8  num);
		static c8  byteswap(c8  num);
	};

	class Printer
	{
	public:
		// prints out a string to the console out stdout or debug log or whatever
		static void print(const c8* message);
		static void log(const c8* message, LogMessageLevel ll = LML_NORMAL);
		static void log(const String message, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const c8* hint, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const path& hint, LogMessageLevel ll = LML_NORMAL);
		static LogManager* Logger;
		//static ILogger* Logger;
	};


	// mixed linear congruential generator (MLCG)
	// numbers chosen according to L'Ecuyer, Commun. ACM 31 (1988) 742
	// period is somewhere around m-1
	class Randomizer
	{
	public:

		//! resets the randomizer
		static void reset(SINT32 value = 0x0f0f0f0f);

		//! generates a pseudo random number in the range 0..randMax()
		static SINT32 rand();

		//! generates a pseudo random number in the range 0..1
		static Real frand();

		//! get maxmimum number generated by rand()
		static SINT32 randMax();

	private:

		static SINT32 seed;
		static const SINT32 m = 2147483399;	// a non-Mersenne prime
		static const SINT32 a = 40692;			// another spectral success story
		static const SINT32 q = m / a;
		static const SINT32 r = m%a;			// again less than q
		static const SINT32 rMax = m - 1;
	};




	class Timer
	{
	public:

		//! returns the current time in milliseconds
		static UINT32 getTime();

		//! get current time and date in calendar form
		static ITimer::RealTimeDate getRealTimeAndDate();

		//! initializes the real timer
		static void initTimer(bool usePerformanceTimer = true);

		//! sets the current virtual (game) time
		static void setTime(UINT32 time);

		//! stops the virtual (game) timer
		static void stopTimer();

		//! starts the game timer
		static void startTimer();

		//! sets the speed of the virtual timer
		static void setSpeed(Real speed);

		//! gets the speed of the virtual timer
		static Real getSpeed();

		//! returns if the timer currently is stopped
		static bool isStopped();

		//! makes the virtual timer update the time value based on the real time
		static void tick();

		//! returns the current real time in milliseconds
		static UINT32 getRealTime();

	private:

		static void initVirtualTimer();

		static Real VirtualTimerSpeed;
		static SINT32 VirtualTimerStopCounter;
		static UINT32 StartRealTime;
		static UINT32 LastVirtualTime;
		static UINT32 StaticTime;
	};
}

#endif