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
	���������ϸ����־ϸ�ڵȼ�
	*/
	enum LoggingLevel
	{
		LL_LOW = 1,
		LL_NORMAL = 2,
		LL_BOREME = 3
	};

	/**
	��־��Ϣ�ļ�ֵ
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

	/** @remarks �������࣬���������ע�ᵽ��־��������־��Ϣ */
	class LogListener
	{
	public:
		virtual ~LogListener() {}

		/**
		@remarks
		ÿ����־�յ�һ����Ϣ���ҽ�Ҫ����д�����ʱ��������
		@param message
		��Ҫ��¼����Ϣ
		@param lml
		Ҫʹ�õ���Ϣ����
		@param maskDebug
		���������Ƿ����������̨
		@param logName
		�����־������,(���ڲ�ͬ����־�����ʹ�ü���������������ȷ����)
		@param skipThisMessage
		ͨ��messageLogged()�������Ϊtrue
		*/
		virtual void messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String &logName, bool& skipThisMessage) = 0;
	};


	/**
	@remarks
	��־��������debug/log ���ݵ��ļ�
	@note
	<br>���ܱ�ֱ��ʹ�ã����뵽LogManager��
	*/
	class _SapphireExport Log : public LogAlloc, virtual IReferenceCounter
	{
	private:
		IEventReceiver* Receiver;
	protected:
		std::ofstream	mLog;    //�ļ������
		LoggingLevel	mLogLevel;     //log�ȼ�
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

		SAPPHIRE_AUTO_MUTEX //�����������ⲿ��
			/**
			@remarks
			���캯������LogManager����
			*/
			Log(const String& name, bool debugOutput = true, bool suppressFileOutput = false);
		/**
			@remarks
			���캯������LogManager����
			*/
		Log(const String& name, IEventReceiver* recevier, bool debugOutput = true, bool suppressFileOutput = false);

		/**
		@remarks
		Ĭ����������
		*/
		~Log();


		//! ����event receiver
		void setReceiver(IEventReceiver* r);
		/// ����log������
		const String& getName() const { return mLogName; }
		/// ��ȡ�Ƿ�Ҫdebug�����־�Ŀ���
		bool isDebugOutputEnabled() const { return mDebugOut; }
		/// ��ȡ�����־�Ƿ��������log
		bool isFileOutputSuppressed() const { return mSuppressFile; }
		/// ��ȡ�Ƿ����ʱ������������־
		bool isTimeStampEnabled() const { return mTimeStamp; }

		/**
		��¼��Ϣ��debugger���Ҽ�¼�ļ�(���Ĭ����SAPPHIRE.log),
		*/
		void logMessage(const String& message, LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

		/** ��ȡһ����������Ϊ�����־��Ŀ��*/
		Stream stream(LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

		/**
		@remarks
		������ر������־��Ϣ��debugger
		*/
		void setDebugOutputEnabled(bool debugOutput);
		/**
		@remarks
		������־�����ϸ��
		*/
		void setLogDetail(LoggingLevel ll);
		/**
		@remarks
		�������߹ر�ʱ���־
		*/
		void setTimeStampEnabled(bool timeStamp);
		/** ��ȡ��־����
		*/
		LoggingLevel getLogDetail() const { return mLogLevel; }
		/**
		@remarks
		ע��һ���������������־
		@param
		һ����Ч�ļ�����������
		*/
		void addListener(LogListener* listener);

		/**
		@remarks
		�������־ȡ��ע��һ��������
		@param
		һ����Ч�ļ�����������
		*/
		void removeListener(LogListener* listener);

		/**
		��־��Ŀ��������
		@remarks
		һ������־����ʹ�����򵥵ķ��Ͳ�ͬ������һ����־.
		�����ò�����<<ʵ���������鵽������־���ڵ���Stream����Flush֮ǰ���Ỻ�塣
		����������Լ������٣���ʱ�������ݻᱻ���͵�Ĭ�ϵ���־��
		���Բ��ù�������һ���ֲ���������ֱ��ʹ��Log::stream()����һ������ɣ�������󽫱����٣���־����¼��

		@par
		���ܹ��������Ʋ���������󣬾���ͬstd::setw() ���� std::setfill() ȥ���Ƹ�ʽ
		@note
		ÿ�����������̰߳�ȫ�ģ��������̼߳䴫�ݣ����߳��ܹ����������Լ�����ʵ������ͬ������־�������̰߳�ȫ��
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

			/// ָ��һ������flush�������־�ļ�����
			struct Flush {};

			Stream(Log* target, LogMessageLevel lml, bool maskDebug)
				:mTarget(target), mLevel(lml), mMaskDebug(maskDebug)
			{

			}
			// ���ƹ�����
			Stream(const Stream& rhs)
				: mTarget(rhs.mTarget), mLevel(rhs.mLevel), mMaskDebug(rhs.mMaskDebug)
			{
				// ��ȷ������Ҫ���Ƶ���
				mCache.str(rhs.mCache.str());
			}
			~Stream()
			{
				// flush������
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
