#include "SapphireStableHeader.h"
#include "SapphirePlatfrom.h"
#include "SapphirePrerequisites.h"
#include "SapphireMemoryTracker.h"


#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
#   include <windows.h>
#	define Sapphire_OutputCString(str) ::OutputDebugStringA(str)
#	define Sapphire_OutputWString(str) ::OutputDebugStringW(str)
#else
#	define Sapphire_OutputCString(str) std::cerr << str
#	define Sapphire_OutputWString(str) std::cerr << str
#endif


namespace Sapphire
{
#if SAPPHIRE_MEMORY_TRACKER
	//��ȡMemoryTrackerʵ��
	MemoryTracker& MemoryTracker::get()
	{
		static MemoryTracker tracker;  //staticʵ��ֻ����һ��
		return tracker;
}
	//--------------------------------------------------------------------------
	void MemoryTracker::_recordAlloc(void* ptr, size_t sz, unsigned int pool,
		const char* file, size_t ln, const char* func)
	{
		if (mRecordEnable)
		{
			SAPPHIRE_LOCK_AUTO_MUTEX

				assert(mAllocations.find(ptr) == mAllocations.end() && "Double allocation with same address - "
				"this probably means you have a mismatched allocation / deallocation style, "
				"check if you're are using SAPPHIRE_ALLOC_T / SAPPHIRE_FREE and SAPPHIRE_NEW_T / SAPPHIRE_DELETE_T consistently");
			//�����ڴ�
			mAllocations[ptr] = Alloc(sz, pool, file, ln, func);
			if (pool >= mAllocationsByPool.size())       //����صĺ������ڷ���صĴ�С
				mAllocationsByPool.resize(pool + 1, 0);  //�����޸ķ��������Ĵ�С+1
			mAllocationsByPool[pool] += sz;      //����������صĳ�����+sz
			mTotalAllocations += sz;       //�ܷ���ռ�+sz
		}

	}
	//--------------------------------------------------------------------------
	void MemoryTracker::_recordDealloc(void* ptr)
	{
		if (mRecordEnable)
		{
			//������Ч��ָ��
			if (!ptr)
				return;

			SAPPHIRE_LOCK_AUTO_MUTEX

				AllocationMap::iterator i = mAllocations.find(ptr);
			assert(i != mAllocations.end() && "Unable to locate allocation unit - "
				"this probably means you have a mismatched allocation / deallocation style, "
				"check if you're are using SAPPHIRE_ALLOC_T / SAPPHIRE_FREE and SAPPHIRE_NEW_T / SAPPHIRE_DELETE_T consistently");
			// ��������
			mAllocationsByPool[i->second.pool] -= i->second.bytes;
			//ȫ��״̬
			mTotalAllocations -= i->second.bytes;
			mAllocations.erase(i);
		}
	}
	//--------------------------------------------------------------------------
	size_t MemoryTracker::getTotalMemoryAllocated() const
	{
		return mTotalAllocations;
	}
	//--------------------------------------------------------------------------
	size_t MemoryTracker::getMemoryAllocatedForPool(unsigned int pool) const
	{
		return mAllocationsByPool[pool];
	}
	//--------------------------------------------------------------------------
	//�����ڴ�й¶
	void MemoryTracker::reportLeaks()
	{
		if (mRecordEnable)
		{
			StringUtil::StrStreamType os;

			if (mAllocations.empty())
			{
				os << "Sapphire Memory: No memory leaks" << std::endl;
			}
			else
			{
				os << "Sapphire Memory: Detected memory leaks !!! " << std::endl;
				os << "Sapphire Memory: (" << mAllocations.size() << ") Allocation(s) with total " << mTotalAllocations << " bytes." << std::endl;
				os << "Sapphire Memory: Dumping allocations -> " << std::endl;


				for (AllocationMap::const_iterator i = mAllocations.begin(); i != mAllocations.end(); ++i)
				{
					const Alloc& alloc = i->second;
					if (!alloc.filename.empty())
						os << alloc.filename;
					else
						os << "(unknown source):";

					os << "(" << alloc.line << ") : {" << alloc.bytes << " bytes}" << " function: " << alloc.function << std::endl;

				}
				os << std::endl;
			}

			if (mDumpToStdOut)
				std::cout << os.str();

			std::cout << os.str();
			std::ofstream of;
			of.open(mLeakFileName.c_str());
			of << os.str();
			of.close();

			Sapphire_OutputCString(os.str().c_str());
		}
	}


#endif

}