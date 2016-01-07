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
		// ���⽻���ַ���������
		static UINT8  byteswap(UINT8  num);
		static c8  byteswap(c8  num);
	};

	class Printer
	{
	public:
		// ���һ���ַ���������̨���(stdout��debug log��������)
		static void print(const c8* message);
		static void log(const c8* message, LogMessageLevel ll = LML_NORMAL);
		static void log(const String message, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const c8* hint, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const path& hint, LogMessageLevel ll = LML_NORMAL);
		static LogManager* Logger;
		//static ILogger* Logger;
	};


	// �������ͬ�ഴ����(MLCG)
	class Randomizer
	{
	public:

		//! ����randomizer
		static void reset(SINT32 value = 0x0f0f0f0f);

		//! ����һ��α���������Χ0..randMax()
		static SINT32 rand();

		//! ����һ��α���������Χ0...1
		static Real frand();

		//! ͨ��rand()��ȡ�����������
		static SINT32 randMax();

	private:

		static SINT32 seed;
		static const SINT32 m = 2147483399;	// һ����÷ɭ����
		static const SINT32 a = 40692;			 
		static const SINT32 q = m / a;
		static const SINT32 r = m%a;			
		static const SINT32 rMax = m - 1;
	};




	class Timer
	{
	public:

		//! ���ص�ǰʱ�������
		static UINT32 getTime();

		//! ����һ��ϵͳ��ʵʱ��ĺ�����
		static ITimer::RealTimeDate getRealTimeAndDate();

		//! ��ʼ��ʵtimer
		static void initTimer(bool usePerformanceTimer = true);

		//! ���õ�ǰ����ʱ��
		static void setTime(UINT32 time);

		//! ֹͣ����timer
		static void stopTimer();

		//! ��ʼ��Ϸtimer
		static void startTimer();

		//! ��������timer���ٶ�
		static void setSpeed(Real speed);

		//! ��ȡ����timer���ٶ�
		static Real getSpeed();

		//! ����timer��ǰ�Ƿ�ֹͣ
		static bool isStopped();

		//! ʹ����timer������ʵʱ�����timer
		static void tick();

		//! ���ص�ǰ��ʵʱ�������
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