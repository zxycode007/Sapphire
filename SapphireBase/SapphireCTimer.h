#ifndef _SAPPHIRE_C_TIMER_
#define _SAPPHIRE_C_TIMER_

#include "SapphireITimer.h"
#include "SapphireOS.h"


namespace Sapphire
{
	//! Device����Ҫ��timer��ʵ��
	class CTimer : public ITimer
	{
	public:

		CTimer(bool usePerformanceTimer = true)
		{
			Timer::initTimer(usePerformanceTimer);
		}

		//! ����һ��ϵͳ��ʵʱ��ĺ�����
		/** ���ֵ�ڳ���ʼ����ʱ������0��ʼ�� ���磺��һ��ʵ������ֵ���ص���
		��ϵͳ��ʼ�����ĺ�����
		*/
		virtual UINT32 getRealTime() const
		{
			return Timer::getRealTime();
		}

		//! ��ȡ��ǰʱ�������
		virtual RealTimeDate getRealTimeAndDate() const
		{
			return Timer::getRealTimeAndDate();
		}

		//! ���ص�ǰ����ʱ��ĺ�����
		/** ���ֵ�Ǵ�0��ʼ���ҿ���ͨ��setTime(),stopTimer(), startTimer()���ơ����timerֹͣ�����ֵ����timer���õ��ٶȣ�
		�������Ҫϵͳʱ�䣬��getRealTime()
	    */
		virtual UINT32 getTime() const
		{
			return Timer::getTime();
		}

		//! ���õ�ǰ����ʱ��
		virtual void setTime(UINT32 time)
		{
			Timer::setTime(time);
		}

		//! ֹͣ��Ϸtimer
		/**  ÿ��ֹͣǰ�������ȿ�������Ȼ����� */
		virtual void stop()
		{
			Timer::stopTimer();
		}

		//! ������Ϸtimer
		/** ÿ��ֹͣǰ�������ȿ�������Ȼ����� */
		virtual void start()
		{
			Timer::startTimer();
		}

		//! ����timer�ٶ�
		/** ����ٶ���ʱ�����е�����ʵϵͳʱ���������������*/
		virtual void setSpeed(Real speed = 1.0f)
		{
			Timer::setSpeed(speed);
		}

		//! ���ص�ǰ��ʱ���ٶ�����
		/** ����ٶ���ʱ�����е�����ʵϵͳʱ��������������� */
		virtual Real getSpeed() const
		{
			return Timer::getSpeed();
		}

		//! ������Ϸtimer�Ƿ�ֹͣ
		virtual bool isStopped() const
		{
			bool ret = Timer::isStopped();
			return ret;
		}

		//! �߼�����ʱ��
		/** ��ʹ����timer�ڻ�����ʵʱ������ϸ���ʱ��ֵ�����ǵ�Device::run()����ʱ�Զ����õģ����������
		���������������Ҳ�����ֶ�����
		  */
		virtual void tick()
		{
			Timer::tick();
		}
	};
}

#endif