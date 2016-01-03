#ifndef _SAPPHIRE_C_TIMER_
#define _SAPPHIRE_C_TIMER_

#include "SapphireITimer.h"
#include "SapphireOS.h"


namespace Sapphire
{
	//! Device independent implementation of the timer
	class CTimer : public ITimer
	{
	public:

		CTimer(bool usePerformanceTimer = true)
		{
			Timer::initTimer(usePerformanceTimer);
		}

		//! Returns current real time in milliseconds of the system.
		/** This value does not start with 0 when the application starts.
		For example in one implementation the value returned could be the
		amount of milliseconds which have elapsed since the system was started. */
		virtual UINT32 getRealTime() const
		{
			return Timer::getRealTime();
		}

		//! Get current time and date in calendar form
		virtual RealTimeDate getRealTimeAndDate() const
		{
			return Timer::getRealTimeAndDate();
		}

		//! Returns current virtual time in milliseconds.
		/** This value starts with 0 and can be manipulated using setTime(), stopTimer(),
		startTimer(), etc. This value depends on the set speed of the timer if the timer
		is stopped, etc. If you need the system time, use getRealTime() */
		virtual UINT32 getTime() const
		{
			return Timer::getTime();
		}

		//! sets current virtual time
		virtual void setTime(UINT32 time)
		{
			Timer::setTime(time);
		}

		//! Stops the game timer.
		/** The timer is reference counted, which means everything which calls
		stopTimer() will also have to call startTimer(), otherwise the timer may not start/stop
		corretly again. */
		virtual void stop()
		{
			Timer::stopTimer();
		}

		//! Starts the game timer.
		/** The timer is reference counted, which means everything which calls
		stopTimer() will also have to call startTimer(), otherwise the timer may not start/stop
		corretly again. */
		virtual void start()
		{
			Timer::startTimer();
		}

		//! Sets the speed of the timer
		/** The speed is the factor with which the time is running faster or slower then the
		real system time. */
		virtual void setSpeed(Real speed = 1.0f)
		{
			Timer::setSpeed(speed);
		}

		//! Returns current speed of the timer
		/** The speed is the factor with which the time is running faster or slower then the
		real system time. */
		virtual Real getSpeed() const
		{
			return Timer::getSpeed();
		}

		//! Returns if game timer is currently stopped
		virtual bool isStopped() const
		{
			bool ret = Timer::isStopped();
			return ret;
		}

		//! Advances the virtual time
		/** Makes the virtual timer update the time value based on the real time. This is
		called automaticly when calling IrrlichtDevice::run(), but you can call it manually
		if you don't use this method. */
		virtual void tick()
		{
			Timer::tick();
		}
	};
}

#endif