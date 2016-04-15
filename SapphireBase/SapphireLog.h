#ifndef __SAPPHIRE_LOG__
#define __SAPPHIRE_LOG__

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireString.h"

namespace Sapphire {

	class IEventReceiver;

	// LogMessageLevel + LoggingLevel > SAPPHIRE_LOG_THRESHOLD = message logged
#define SAPPHIRE_LOG_THRESHOLD 4

	/**
	将进入更详细的日志细节等级
	*/
	enum LoggingLevel
	{
		LL_LOW = 1,
		LL_NORMAL = 2,
		LL_BOREME = 3
	};

	/**
	日志消息的价值
	*/
	enum LogMessageLevel
	{
		LML_TRIVIAL = 1,
		LML_NORMAL = 2,
		LML_CRITICAL = 3,
		LML_WARNING = 4,
		LML_ERROR = 5,
		LML_DEBUG = 6
	};

	/** @remarks 纯抽象类，导出这个类注册到日志并监听日志信息 */
	class LogListener
	{
	public:
		virtual ~LogListener() {}

		/**
		@remarks
		每当日志收到一个信息并且将要将它写入输出时，调用它
		@param message
		将要记录的信息
		@param lml
		要使用的信息级别
		@param maskDebug
		决定我们是否输出到控制台
		@param logName
		这个日志的名字,(对于不同的日志你可以使用几个监听器，并且确定它)
		@param skipThisMessage
		通过messageLogged()如果设置为true
		*/
		virtual void messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String &logName, bool& skipThisMessage) = 0;
	};


	/**
	@remarks
	日志类对于输出debug/log 数据到文件
	@note
	<br>不能被直接使用，插入到LogManager中
	*/
	class _SapphireExport Log : public LogAlloc, virtual IReferenceCounter
	{
	private:
		IEventReceiver* Receiver;
	protected:
		std::ofstream	mLog;    //文件输出流
		LoggingLevel	mLogLevel;     //log等级
		bool			mDebugOut;
		bool			mSuppressFile;
		bool			mTimeStamp;
		String			mLogName;

		StringStream stringout;
		StringWStream wstringout;


		typedef vector<LogListener*>::type mtLogListener;
		mtLogListener mListeners;
	public:

		class Stream;

		SAPPHIRE_AUTO_MUTEX //公共的启用外部锁
			/**
			@remarks
			构造函数，由LogManager调用
			*/
			Log(const String& name, bool debugOutput = true, bool suppressFileOutput = false);
		/**
			@remarks
			构造函数，由LogManager调用
			*/
		Log(const String& name, IEventReceiver* recevier, bool debugOutput = true, bool suppressFileOutput = false);

		/**
		@remarks
		默认析构函数
		*/
		~Log();


		//! 设置event receiver
		void setReceiver(IEventReceiver* r);
		/// 返回log的名字
		const String& getName() const { return mLogName; }
		/// 获取是否要debug输出日志的开启
		bool isDebugOutputEnabled() const { return mDebugOut; }
		/// 获取这个日志是否抑制这个log
		bool isFileOutputSuppressed() const { return mSuppressFile; }
		/// 获取是否加上时间输出到这个日志
		bool isTimeStampEnabled() const { return mTimeStamp; }

		/**
		记录信息到debugger并且记录文件(这个默认是SAPPHIRE.log),
		*/
		void logMessage(const String& message, LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

		/** 获取一个流对象作为这个日志的目标*/
		Stream stream(LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

		/**
		@remarks
		开启或关闭输出日志信息到debugger
		*/
		void setDebugOutputEnabled(bool debugOutput);
		/**
		@remarks
		设置日志级别的细节
		*/
		void setLogDetail(LoggingLevel ll);
		/**
		@remarks
		开启或者关闭时间标志
		*/
		void setTimeStampEnabled(bool timeStamp);
		/** 获取日志级别
		*/
		LoggingLevel getLogDetail() const { return mLogLevel; }
		/**
		@remarks
		注册一个监听器到这个日志
		@param
		一个有效的监听器派生类
		*/
		void addListener(LogListener* listener);

		/**
		@remarks
		从这个日志取消注册一个监听器
		@param
		一个有效的监听器派生类
		*/
		void removeListener(LogListener* listener);

		/**
		日志的目标流对象
		@remarks
		一个流日志对象使它更简单的发送不同东西到一个日志.
		你能用操作符<<实现所有事情到流到日志。在调用Stream：：Flush之前它会缓冲。
		或者这个流自己被销毁，此时缓冲内容会被发送到默认的日志。
		可以不用关联它到一个局部变量，而直接使用Log::stream()并且一旦流完成，这个对象将被销毁，日志被记录。

		@par
		你能够用流控制操作这个对象，就如同std::setw() 并且 std::setfill() 去控制格式
		@note
		每个流对象不是线程安全的，所以在线程间传递，多线程能够保持他们自己的流实例点在同样的日志并且是线程安全的
		*/
		class _SapphirePrivate Stream
		{
		protected:
			Log* mTarget;
			LogMessageLevel mLevel;
			bool mMaskDebug;
			typedef StringUtil::StrStreamType BaseStream;
			BaseStream mCache;

		public:

			/// 指定一个流的flush结果到日志的简单类型
			struct Flush {};

			Stream(Log* target, LogMessageLevel lml, bool maskDebug)
				:mTarget(target), mLevel(lml), mMaskDebug(maskDebug)
			{

			}
			// 复制构造器
			Stream(const Stream& rhs)
				: mTarget(rhs.mTarget), mLevel(rhs.mLevel), mMaskDebug(rhs.mMaskDebug)
			{
				// 明确复制需要复制的流
				mCache.str(rhs.mCache.str());
			}
			~Stream()
			{
				// flush和销毁
				if (mCache.tellp() > 0)
				{
					mTarget->logMessage(mCache.str(), mLevel, mMaskDebug);
				}
			}

			template <typename T>
			Stream& operator<< (const T& v)
			{
				mCache << v;
				return *this;
			}

			Stream& operator<< (const Flush& v)
			{
				(void)v;
				mTarget->logMessage(mCache.str(), mLevel, mMaskDebug);
				mCache.str(StringUtil::BLANK);
				return *this;
			}


			};
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_NACL
	protected:
		static pp::Instance* mInstance;
	public:
		static void setInstance(pp::Instance* instance) { mInstance = instance; };
#endif

		};

	}

#endif
