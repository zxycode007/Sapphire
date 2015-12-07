#ifndef _SAPPHIRE_MEMORY_ALLOCATED_OBJECT_
#define _SAPPHIRE_MEMORY_ALLOCATED_OBJECT_

#include "SapphirePlatfrom.h"
//#include "SapphirePrerequisites.h"

//取消c++ 的new delete 内存分配和回收方式，用自定义的的new delete管理内存
#ifdef new
#  undef new
#endif
#ifdef delete
#  undef delete
#endif

namespace Sapphire
{
	//这个类作为所有使用自定义内存分配器的类的对象的父类
	//需要一个模板类参数定义内存分配策略（stdAllocPolicy）
	//如 AllocatedObj<StdAllocPolicy>
	template<class Alloc>
	class _SapphireExport AllocatedObj
	{
	public:
		explicit AllocatedObj()
		{ }

		~AllocatedObj()
		{ }

		//重定义new运算符
		void* operator new(size_t sz, const char* file, int line, const char* func)
		{
			return Alloc::allocateBytes(sz, file, line, func);
		}

			void* operator new(size_t sz)
		{
			return Alloc::allocateBytes(sz);
		}

			/// 替换new，无操作
			void* operator new(size_t sz, void* ptr)
		{
			(void)sz;
			return ptr;
		}

			/// 数组的分配， 带上debug的行信息
			void* operator new[](size_t sz, const char* file, int line, const char* func)
		{
			return Alloc::allocateBytes(sz, file, line, func);
		}

			void* operator new[](size_t sz)
		{
			return Alloc::allocateBytes(sz);
		}

			void operator delete(void* ptr)
		{
			Alloc::deallocateBytes(ptr);
		}

		void operator delete(void* ptr, void*)
		{
			Alloc::deallocateBytes(ptr);
		}

		void operator delete(void* ptr, const char*, int, const char*)
		{
			Alloc::deallocateBytes(ptr);
		}

		void operator delete[](void* ptr)
		{
			Alloc::deallocateBytes(ptr);
		}


			void operator delete[](void* ptr, const char*, int, const char*)
		{
			Alloc::deallocateBytes(ptr);
		}

	};


}


#endif

