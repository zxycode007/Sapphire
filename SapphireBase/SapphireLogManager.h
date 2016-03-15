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
	�����־�������������һָ�Ӧ�ó������־
	@remarks
	����ཫ��������־�ļ����ҽ����ش��ڵ�ʵ����������ϣ�������־����ܴ���һ���µ���־���߷���һ�����ڵ���־�������
	���ҵ���������־����������ʱ�������д��

	@par
	Ĭ�ϣ������ڹ���ʱʵ��һ��LogManager (������Ϊ��ʵ��) ,���ҽ����ڴ���һ��Ĭ�ϵ���־
	����������Ĺ���������������Ҫ����Ŀ��ƣ��������Ҫ����Ŀ��ƣ������ض�����־�ӿ�ʼ������debug�����,
	�ڴ���һ����ʵ��֮ǰ����Ҫ����һ��LogManager�� Ҫ����һ��Ĭ�ϵ���־��������������Ѿ��������ˣ���ô�������ٵ���������
	����õ�һ��ʵ������¼�����еĲο���Ϣ
	*/
	class _SapphireExport LogManager : public Singleton<LogManager>, public LogAlloc,virtual public IReferenceCounter
	{
	protected:
		typedef map<String, Log*>::type	LogList;

		/// һ�������ܹ����ʵ�������־���б�
		LogList mLogs;

		/// �����Ĭ�ϵ���־������������
		Log* mDefaultLog;

	public:
		SAPPHIRE_AUTO_MUTEX // ����

			LogManager();
		~LogManager();

		/** 
		
		�ø��������ִ���һ���µ���־
		@param
	    name  ����־��������'Sapphire.log'
		@param
		defaultLog ���Ϊtrue�� ������������ͨ����־����������������������ã����Ĭ����־������ᷢ�� 
		��һ����־�Ѿ�����һ����ΪĬ��		
		@param
		debuggerOutput ���Ϊtrue����������־����·�����������������
		@param
		suppressFileOutput ���Ϊtrue������һ���߼�������һ��������־����û���ļ������
		�����Ҫע��һ����־����������ô�߼�����ᱻ����
		*/
		Log* createLog(const String& name, bool defaultLog = false, bool debuggerOutput = true,
			bool suppressFileOutput = false);

		/**

		�ø��������ִ���һ���µ���־
		@param
		name  ����־��������'Sapphire.log'
		@param
		defaultLog ���Ϊtrue�� ������������ͨ����־����������������������ã����Ĭ����־������ᷢ��
		��һ����־�Ѿ�����һ����ΪĬ��
		@param
		recevier �¼�������
		@param
		debuggerOutput ���Ϊtrue����������־����·�����������������
		@param
		suppressFileOutput ���Ϊtrue������һ���߼�������һ��������־����û���ļ������
		�����Ҫע��һ����־����������ô�߼�����ᱻ����
		*/
		Log* createLog(const String& name, IEventReceiver* recevier, bool defaultLog = false, bool debuggerOutput = true,
			bool suppressFileOutput = false);

		/** ����һ����־
		*/
		Log* getLog(const String& name);

		/** ����һ��ָ��Ĭ����־��ָ��
		*/
		Log* getDefaultLog();

		/** �رղ��Ƴ�һ����������־
		*/
		void destroyLog(const String& name);
		/** �رպ��Ƴ���־. */
		void destroyLog(Log* log);

		/** ���ò�����һ��log��ΪĬ�ϵ���־
		@return ǰһ��Ĭ����־
		*/
		Log* setDefaultLog(Log* newLog);

		/** ��¼һ����Ϣ��Ĭ�ϵ���־
		*/
		void logMessage(const String& message, LogMessageLevel lml = LML_NORMAL,
			bool maskDebug = false);

		/** ��¼һ����Ϣ��Ĭ�ϵ���־��Ϊ�������ݣ�
		*/
		void logMessage(LogMessageLevel lml, const String& message,
			bool maskDebug = false) {
			logMessage(message, lml, maskDebug);
		}

		/** ��Ĭ�ϵ���־��ȡһ���� */
		Log::Stream stream(LogMessageLevel lml = LML_NORMAL,
			bool maskDebug = false);

		/** ���õ�Ĭ����־�ļ���
		*/
		void setLogDetail(LoggingLevel ll);
		/** ���Ǳ�׼��̬����
		@remarks
		Ϊʲô����Щ���������ڵ�̬ʵ����һ��.hͷ�ļ�������ζ���κΰ����������ᱻ���뵽�κβ����С�
		����Ҫ�Ե�̬ģ����й�������ʵ����ֻ�������뵽���ڵ�̬��ʵ�����У����������С�
		����������������Դ��ⲿdllʹ�û��ڵ�̬�����õ����Ӵ���
		@par
		������������κεط������ģ��汾������ʵ��ͣ��������������뵥Ԫ����ֹ�˱������
		*/
		static LogManager& getSingleton(void);
		/** ���Ǳ�׼��̬�� 
		@remarks
		Ϊʲô����Щ���������ڵ�̬ʵ����һ��.hͷ�ļ�������ζ���κΰ����������ᱻ���뵽�κβ����С�
		����Ҫ�Ե�̬ģ����й�������ʵ����ֻ�������뵽���ڵ�̬��ʵ�����У����������С�
		����������������Դ��ⲿdllʹ�û��ڵ�̬�����õ����Ӵ���
		@par
		������������κεط������ģ��汾������ʵ��ͣ��������������뵥Ԫ����ֹ�˱������
		*/
		static LogManager* getSingletonPtr(void);

	};

	//��LogManager�ض���ΪILogger
	typedef LogManager* ILogger;
	 
}

#endif
