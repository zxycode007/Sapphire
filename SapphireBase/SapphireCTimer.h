#ifndef _SAPPHIRE_C_TIMER_
#define _SAPPHIRE_C_TIMER_

#include "SapphireITimer.h"
#include "SapphireOS.h"


namespace Sapphire
{
	//! Device所需要的timer的实现
	class CTimer : public ITimer
	{
	public:

		CTimer(bool usePerformanceTimer = true)
		{
			Timer::initTimer(usePerformanceTimer);
		}

		//! 返回一个系统真实时间的毫秒数
		/** 这个值在程序开始运行时不是以0开始。 例如：在一个实现里，这个值返回的是
		自系统开始以来的毫秒数
		*/
		virtual UINT32 getRealTime() const
		{
			return Timer::getRealTime();
		}

		//! 获取当前时间和日期
		virtual RealTimeDate getRealTimeAndDate() const
		{
			return Timer::getRealTimeAndDate();
		}

		//! 返回当前虚拟时间的毫秒数
		/** 这个值是从0开始并且可以通过setTime(),stopTimer(), startTimer()控制。如果timer停止，这个值依赖timer设置的速度，
		如果你需要系统时间，用getRealTime()
	    */
		virtual UINT32 getTime() const
		{
			return Timer::getTime();
		}

		//! 设置当前虚拟时间
		virtual void setTime(UINT32 time)
		{
			Timer::setTime(time);
		}

		//! 停止游戏timer
		/**  每次停止前，必须先开启，不然会出错 */
		virtual void stop()
		{
			Timer::stopTimer();
		}

		//! 开启游戏timer
		/** 每次停止前，必须先开启，不然会出错 */
		virtual void start()
		{
			Timer::startTimer();
		}

		//! 设置timer速度
		/** 这个速度是时间运行的与真实系统时间更快或更慢的因子*/
		virtual void setSpeed(Real speed = 1.0f)
		{
			Timer::setSpeed(speed);
		}

		//! 返回当前的时间速度因子
		/** 这个速度是时间运行的与真实系统时间更快或更慢的因子 */
		virtual Real getSpeed() const
		{
			return Timer::getSpeed();
		}

		//! 返回游戏timer是否停止
		virtual bool isStopped() const
		{
			bool ret = Timer::isStopped();
			return ret;
		}

		//! 高级虚拟时间
		/** 将使虚拟timer在基于真实时间基础上更新时间值。这是当Device::run()调用时自动调用的，但是如果你
		不想用这个方法，也可以手动调用
		  */
		virtual void tick()
		{
			Timer::tick();
		}
	};
}

#endif