#ifndef _SAPPHIRE_MEMORY_TRACKER_
#define _SAPPHIRE_MEMORY_TRACKER_

//#include "SapphirePrerequisites.h"


// ����Ҫ���� prerequisites, ���ڻ�״����
// ����ļ����뱻�����Ѿ�����prerequisites�ļ��İ���
//#include "OgrePrerequisites.h"
#ifndef SAPPHIRE_COMPILER
#	pragma message "MemoryTracker included somewhere OgrePrerequisites.h wasn't!"
#endif

// �����ʹ��GCC 3.1 std��
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC && SAPPHIRE_COMP_VER >= 310 && !defined(STLPORT)
// ��void*��Ҫ����һ��hash���� 
//���� gcc 4.3 �� http://gcc.gnu.org/gcc-4.3/changes.html
#   if SAPPHIRE_COMP_VER >= 430
#       include <tr1/unordered_map>
#   else
#       include <ext/hash_map>
namespace __gnu_cxx
{
	template <> struct hash< void* >
	{
		size_t operator()(void* const & ptr) const
		{
			return (size_t)ptr;
		}
	};
}

#   endif
#endif

namespace Sapphire
{


#if SAPPHIRE_MEMORY_TRACKER

	/** �������ٷ���ͻ��ղ��������ұ����ڴ�й¶
	@note
	�����ֻ����Debugģʽ�²Żᱻ����
	*/
	class _SapphireExport MemoryTracker
	{
	protected:
		//�ڴ��������ͬ��������
		SAPPHIRE_AUTO_MUTEX

			// �����¼�ṹ
		struct Alloc
		{
			//�����ֽ���
			size_t bytes;
			//�����ڴ���ڴ��
			unsigned int pool;
			//��¼���䷢��ʱ���ļ���
			string filename;
			//��¼���䷢��ʱ���к�
			size_t line;
			//���䷢��ʱ�Ļص�����
			string function;
			//Ĭ�ϵĹ��캯��
			Alloc() :bytes(0), line(0) {}   //��ʼ����Ա����bytes��line
			//��׼���캯��
			Alloc(size_t sz, unsigned int p, const char* file, size_t ln, const char* func)
				:bytes(sz), pool(p), line(ln)
			{
				if (file)
					filename = file;
				if (func)
					function = func;
			}
		};
		//�ڴ�й¶�ļ���
		string mLeakFileName;
		//�ڴ�ת�����stdout
		bool mDumpToStdOut;
		//���� ��¼ר�õķ���ӳ���hashMap  AllocationMap
		typedef HashMap<void*, Alloc> AllocationMap;
		AllocationMap mAllocations;

		//����ռ���ܴ�С
		size_t mTotalAllocations;
		//ͨ���ط�����ڴ�
		typedef vector<size_t> AllocationsByPool;
		AllocationsByPool mAllocationsByPool;
		//�Ƿ���м�¼
		bool mRecordEnable;
		//�����ڴ�й¶
		void reportLeaks();

		// �������캯��
		MemoryTracker()
			: mLeakFileName("SapphireLeaks.log"), mDumpToStdOut(true),
			mTotalAllocations(0), mRecordEnable(true)
		{
		}
	public:

		/** ���ñ����ļ��� */
		void setReportFileName(const string& name)
		{
			mLeakFileName = name;
		}
		/// �����ļ���
		const string& getReportFileName() const
		{
			return mLeakFileName;
		}
		/// ���õ��ڴ汨���Ƿ��͵�stdout
		void setReportToStdOut(bool rep)
		{
			mDumpToStdOut = rep;
		}
		///��ȡ�ڴ汨���Ƿ����stdout
		bool getReportToStdOut() const
		{
			return mDumpToStdOut;
		}



		/// ȡ�õ�ǰ�����ڴ������
		size_t getTotalMemoryAllocated() const;
		/// ȡ���ڸ����ڴ����Get the amount of memory allocated in a given pool
		size_t getMemoryAllocatedForPool(unsigned int pool) const;


		/** ֻ���ڵ����ڴ������ϵͳʱ����¼һ��������Ϊ
		@param ptr �ڴ��ָ��
		@param sz �ڴ���ֽڵĴ�С
		@param pool ����������ڴ��
		@param file ���з�����ļ���
		@param ln ���з�����ļ�����
		@param func ���з���ĺ���
		*/
		void _recordAlloc(void* ptr, size_t sz, unsigned int pool = 0,
			const char* file = 0, size_t ln = 0, const char* func = 0);

		/** ��¼�����ڴ���Ϊ */
		void _recordDealloc(void* ptr);

		/// �����Ƿ��¼���������
		void setRecordEnable(bool recordEnable)
		{
			mRecordEnable = recordEnable;
		}

		/// �����Ƿ��¼���������
		bool getRecordEnable() const
		{
			return mRecordEnable;
		}

		~MemoryTracker()
		{
			reportLeaks();
		}

		/// ��̬���߷������ڻ�ȡMemory Tracker ��ʵ������
		static MemoryTracker& get();


	};

	//����������ڴ����������SapphirePrerequisites���ж��Ƿ���Debugģʽ
	//�������Debugģʽ������ڴ����


#endif
}

#endif