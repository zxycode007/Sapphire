#ifndef _SAPPHIRE_I_TIMER_
#define _SAPPHIRE_I_TIMER_

#include "SapphireIReferenceCounter.h"

namespace Sapphire
{

	//! Interface for getting and manipulating the virtual time
	class ITimer : public virtual IReferenceCounter
	{
	public:
		//! Returns current real time in milliseconds of the system.
		/** This value does not start with 0 when the application starts.
		For example in one implementation the value returned could be the
		amount of milliseconds which have elapsed since the system was started.
		*/
		virtual UINT32 getRealTime() const = 0;

		enum EWeekday
		{
			EWD_SUNDAY = 0,
			EWD_MONDAY,
			EWD_TUESDAY,
			EWD_WEDNESDAY,
			EWD_THURSDAY,
			EWD_FRIDAY,
			EWD_SATURDAY
		};

		struct RealTimeDate
		{
			// Hour of the day, from 0 to 23
			UINT32 Hour;
			// Minute of the hour, from 0 to 59
			UINT32 Minute;
			// Second of the minute, due to extra seconds from 0 to 61
			UINT32 Second;
			// Year of the gregorian calender
			SINT32 Year;
			// Month of the year, from 1 to 12
			UINT32 Month;
			// Day of the month, from 1 to 31
			UINT32 Day;
			// Weekday for the current day
			EWeekday Weekday;
			// Day of the year, from 1 to 366
			UINT32 Yearday;
			// Whether daylight saving is on
			bool IsDST;
		};

		virtual RealTimeDate getRealTimeAndDate() const = 0;

		//! Returns current virtual time in milliseconds.
		/** This value starts with 0 and can be manipulated using setTime(),
		stopTimer(), startTimer(), etc. This value depends on the set speed of
		the timer if the timer is stopped, etc. If you need the system time,
		use getRealTime() */
		virtual UINT32 getTime() const = 0;

		//! sets current virtual time
		virtual void setTime(UINT32 time) = 0;

		//! Stops the virtual timer.
		/** The timer is reference counted, which means everything which calls
		stop() will also have to call start(), otherwise the timer may not
		start/stop correctly again. */
		virtual void stop() = 0;

		//! Starts the virtual timer.
		/** The timer is reference counted, which means everything which calls
		stop() will also have to call start(), otherwise the timer may not
		start/stop correctly again. */
		virtual void start() = 0;

		//! Sets the speed of the timer
		/** The speed is the factor with which the time is running faster or
		slower then the real system time. */
		virtual void setSpeed(Real speed = 1.0f) = 0;

		//! Returns current speed of the timer
		/** The speed is the factor with which the time is running faster or
		slower then the real system time. */
		virtual Real getSpeed() const = 0;

		//! Returns if the virtual timer is currently stopped
		virtual bool isStopped() const = 0;

		//! Advances the virtual time
		/** Makes the virtual timer update the time value based on the real
		time. This is called automatically when calling IrrlichtDevice::run(),
		but you can call it manually if you don't use this method. */
		virtual void tick() = 0;
	};
}

#endif