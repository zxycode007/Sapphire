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
		// 避免交换字符出现意外
		static UINT8  byteswap(UINT8  num);
		static c8  byteswap(c8  num);
	};

	class Printer
	{
	public:
		// 输出一个字符串到控制台输出(stdout或debug log或者其他)
		static void print(const c8* message);
		static void log(const c8* message, LogMessageLevel ll = LML_NORMAL);
		static void log(const String message, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const c8* hint, LogMessageLevel ll = LML_NORMAL);
		static void log(const c8* message, const path& hint, LogMessageLevel ll = LML_NORMAL);
		static LogManager* Logger;
		//static ILogger* Logger;
	};


	// 混合线性同余创建器(MLCG)
	class Randomizer
	{
	public:

		//! 重设randomizer
		static void reset(SINT32 value = 0x0f0f0f0f);

		//! 创建一个伪随机数到范围0..randMax()
		static SINT32 rand();

		//! 创建一个伪随机数到范围0...1
		static Real frand();

		//! 通过rand()获取创建的最大数
		static SINT32 randMax();

	private:

		static SINT32 seed;
		static const SINT32 m = 2147483399;	// 一个非梅森质数
		static const SINT32 a = 40692;			 
		static const SINT32 q = m / a;
		static const SINT32 r = m%a;			
		static const SINT32 rMax = m - 1;
	};




	class Timer
	{
	public:

		//! 返回当前时间毫秒数
		static UINT32 getTime();

		//! 返回一个系统真实时间的毫秒数
		static ITimer::RealTimeDate getRealTimeAndDate();

		//! 初始真实timer
		static void initTimer(bool usePerformanceTimer = true);

		//! 设置当前虚拟时间
		static void setTime(UINT32 time);

		//! 停止虚拟timer
		static void stopTimer();

		//! 开始游戏timer
		static void startTimer();

		//! 设置虚拟timer的速度
		static void setSpeed(Real speed);

		//! 获取虚拟timer的速度
		static Real getSpeed();

		//! 返回timer当前是否停止
		static bool isStopped();

		//! 使虚拟timer根据真实时间更新timer
		static void tick();

		//! 返回当前真实时间毫秒数
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