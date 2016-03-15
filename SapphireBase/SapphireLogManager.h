#ifndef __SAPPHIRE_MANAGER__
#define __SAPPHIRE_MANAGER__

#include "SapphirePrerequisites.h"

#include "SapphireLog.h"
#include "SapphireSingleton.h"
#include "SapphireString.h"

namespace Sapphire
{
	class IEventReceiver;
	 
	/**
	这个日志管理句柄创建并且恢复应用程序的日志
	@remarks
	这个类将创建新日志文件并且将返回存在的实例。其它类希望这个日志输出能创建一个新的日志或者返回一个存在的日志来输出它
	并且当这个类的日志方法被调用时对其进行写入

	@par
	默认，根会在构造时实例一个LogManager (它将变为单实例) ,并且将基于创建一个默认的日志
	基于这个根的构造参数。如果你想要跟多的控制，如果你想要更多的控制，例如重定向日志从开始或抑制debug的输出,
	在创建一个根实例之前你需要创建一个LogManager， 要创建一个默认的日志。根会检查如果你已经创建有了，那么它不会再单独创建，
	因此用第一个实例来记录你所有的参考信息
	*/
	class _SapphireExport LogManager : public Singleton<LogManager>, public LogAlloc,virtual public IReferenceCounter
	{
	protected:
		typedef map<String, Log*>::type	LogList;

		/// 一个管理能够访问的所有日志的列表
		LogList mLogs;

		/// 这个是默认的日志，它会进行输出
		Log* mDefaultLog;

	public:
		SAPPHIRE_AUTO_MUTEX // 加锁

			LogManager();
		~LogManager();

		/** 
		
		用给定的名字创建一个新的日志
		@param
	    name  给日志的命名如'Sapphire.log'
		@param
		defaultLog 如果为true， 如果在用这个类通用日志方法，除非这个参数被设置，这个默认日志输出将会发送 
		第一个日志已经创建一种作为默认		
		@param
		debuggerOutput 如果为true，输出这个日志将按路径发个调试输出窗口
		@param
		suppressFileOutput 如果为true，这是一个逻辑而不是一个物理日志并且没有文件输出。
		如果你要注册一个日志监听器，那么逻辑输出会被丢弃
		*/
		Log* createLog(const String& name, bool defaultLog = false, bool debuggerOutput = true,
			bool suppressFileOutput = false);

		/**

		用给定的名字创建一个新的日志
		@param
		name  给日志的命名如'Sapphire.log'
		@param
		defaultLog 如果为true， 如果在用这个类通用日志方法，除非这个参数被设置，这个默认日志输出将会发送
		第一个日志已经创建一种作为默认
		@param
		recevier 事件接收器
		@param
		debuggerOutput 如果为true，输出这个日志将按路径发个调试输出窗口
		@param
		suppressFileOutput 如果为true，这是一个逻辑而不是一个物理日志并且没有文件输出。
		如果你要注册一个日志监听器，那么逻辑输出会被丢弃
		*/
		Log* createLog(const String& name, IEventReceiver* recevier, bool defaultLog = false, bool debuggerOutput = true,
			bool suppressFileOutput = false);

		/** 返回一个日志
		*/
		Log* getLog(const String& name);

		/** 返回一个指向默认日志的指针
		*/
		Log* getDefaultLog();

		/** 关闭并移除一个命名的日志
		*/
		void destroyLog(const String& name);
		/** 关闭和移除日志. */
		void destroyLog(Log* log);

		/** 设置并传递一个log作为默认的日志
		@return 前一个默认日志
		*/
		Log* setDefaultLog(Log* newLog);

		/** 记录一个消息的默认的日志
		*/
		void logMessage(const String& message, LogMessageLevel lml = LML_NORMAL,
			bool maskDebug = false);

		/** 记录一个消息到默认的日志（为了向后兼容）
		*/
		void logMessage(LogMessageLevel lml, const String& message,
			bool maskDebug = false) {
			logMessage(message, lml, maskDebug);
		}

		/** 在默认的日志获取一个流 */
		Log::Stream stream(LogMessageLevel lml = LML_NORMAL,
			bool maskDebug = false);

		/** 设置的默认日志的级别
		*/
		void setLogDetail(LoggingLevel ll);
		/** 覆盖标准单态返回
		@remarks
		为什么做这些，这是由于单态实现是一个.h头文件，它意味着任何包含了它都会被编译到任何部分中。
		这需要对单态模板进行工作，但实际上只想它编译到基于单态的实现类中，而不是所有。
		如果不这样，当尝试从外部dll使用基于单态的类会得到链接错误
		@par
		这个方法就在任何地方代理的模板版本，但是实现停留在这个单个编译单元，阻止了编译错误
		*/
		static LogManager& getSingleton(void);
		/** 覆盖标准单态检 
		@remarks
		为什么做这些，这是由于单态实现是一个.h头文件，它意味着任何包含了它都会被编译到任何部分中。
		这需要对单态模板进行工作，但实际上只想它编译到基于单态的实现类中，而不是所有。
		如果不这样，当尝试从外部dll使用基于单态的类会得到链接错误
		@par
		这个方法就在任何地方代理的模板版本，但是实现停留在这个单个编译单元，阻止了编译错误
		*/
		static LogManager* getSingletonPtr(void);

	};

	//将LogManager重定义为ILogger
	typedef LogManager* ILogger;
	 
}

#endif
