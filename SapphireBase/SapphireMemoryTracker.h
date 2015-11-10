#ifndef _SAPPHIRE_MEMORY_TRACKER_
#define _SAPPHIRE_MEMORY_TRACKER_

//#include "SapphirePrerequisites.h"


// 不需要包含 prerequisites, 由于环状依赖
// 这个文件必须被其他已经包含prerequisites文件的包含
//#include "OgrePrerequisites.h"
#ifndef SAPPHIRE_COMPILER
#	pragma message "MemoryTracker included somewhere OgrePrerequisites.h wasn't!"
#endif

// 如果你使用GCC 3.1 std库
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC && SAPPHIRE_COMP_VER >= 310 && !defined(STLPORT)
// 对void*需要定义一个hash函数 
//对于 gcc 4.3 看 http://gcc.gnu.org/gcc-4.3/changes.html
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

	/** 这个类跟踪分配和回收操作，并且报告内存泄露
	@note
	这个类只有在Debug模式下才会被编译
	*/
	class _SapphireExport MemoryTracker
	{
	protected:
		//内存区域访问同步互斥量
		SAPPHIRE_AUTO_MUTEX

			// 分配记录结构
		struct Alloc
		{
			//分配字节数
			size_t bytes;
			//分配内存的内存池
			unsigned int pool;
			//记录分配发生时的文件名
			string filename;
			//记录分配发生时的行号
			size_t line;
			//分配发生时的回调函数
			string function;
			//默认的构造函数
			Alloc() :bytes(0), line(0) {}   //初始化成员函数bytes和line
			//标准构造函数
			Alloc(size_t sz, unsigned int p, const char* file, size_t ln, const char* func)
				:bytes(sz), pool(p), line(ln)
			{
				if (file)
					filename = file;
				if (func)
					function = func;
			}
		};
		//内存泄露文件名
		string mLeakFileName;
		//内存转储输出stdout
		bool mDumpToStdOut;
		//定义 记录专用的分配映射的hashMap  AllocationMap
		typedef HashMap<void*, Alloc> AllocationMap;
		AllocationMap mAllocations;

		//分配空间的总大小
		size_t mTotalAllocations;
		//通过池分配的内存
		typedef vector<size_t> AllocationsByPool;
		AllocationsByPool mAllocationsByPool;
		//是否进行记录
		bool mRecordEnable;
		//报告内存泄露
		void reportLeaks();

		// 保护构造函数
		MemoryTracker()
			: mLeakFileName("SapphireLeaks.log"), mDumpToStdOut(true),
			mTotalAllocations(0), mRecordEnable(true)
		{
		}
	public:

		/** 设置报告文件名 */
		void setReportFileName(const string& name)
		{
			mLeakFileName = name;
		}
		/// 返回文件名
		const string& getReportFileName() const
		{
			return mLeakFileName;
		}
		/// 设置当内存报告是否发送到stdout
		void setReportToStdOut(bool rep)
		{
			mDumpToStdOut = rep;
		}
		///获取内存报告是否输出stdout
		bool getReportToStdOut() const
		{
			return mDumpToStdOut;
		}



		/// 取得当前分配内存的总量
		size_t getTotalMemoryAllocated() const;
		/// 取得在给定内存池中Get the amount of memory allocated in a given pool
		size_t getMemoryAllocatedForPool(unsigned int pool) const;


		/** 只有在调用内存管理子系统时，记录一个分配行为
		@param ptr 内存的指针
		@param sz 内存的字节的大小
		@param pool 发生分配的内存池
		@param file 进行分配的文件名
		@param ln 进行分配的文件的行
		@param func 进行分配的函数
		*/
		void _recordAlloc(void* ptr, size_t sz, unsigned int pool = 0,
			const char* file = 0, size_t ln = 0, const char* func = 0);

		/** 记录销毁内存行为 */
		void _recordDealloc(void* ptr);

		/// 是则是否记录分配和销毁
		void setRecordEnable(bool recordEnable)
		{
			mRecordEnable = recordEnable;
		}

		/// 返回是否记录分配和销毁
		bool getRecordEnable() const
		{
			return mRecordEnable;
		}

		~MemoryTracker()
		{
			reportLeaks();
		}

		/// 静态工具方法用于获取Memory Tracker 的实例方法
		static MemoryTracker& get();


	};

	//如果定义了内存跟踪器，在SapphirePrerequisites里判断是否处于Debug模式
	//如果处于Debug模式，则打开内存跟踪


#endif
}

#endif