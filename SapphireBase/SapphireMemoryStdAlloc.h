#ifndef _SAPPHIRE_MEMORY_STD_ALLOC_
#define _SAPPHIRE_MEMORY_STD_ALLOC_

#include <memory>
#include <limits>

#include "SapphireAlignedAllocator.h"
#include "SapphireMemoryTracker.h"

namespace Sapphire
{
#if SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_STD

	/**
	一个对于使用AllocatedObj和STLAllocator标准的分配策略。
	这个类做是实际分配和释放物理内存，并如果你要改变如何分配内存，需提供一个自定义的版本
	
	这个类是代理了全局的malloc和free
	*/
	class _SapphireExport StdAllocPolicy
	{
	public:
		//分配内存
		static inline void* allocateBytes(size_t count,
#if SAPPHIRE_MEMORY_TRACKER
			const char* file = 0, int line = 0, const char* func = 0
#else
			const char* = 0, int = 0, const char* = 0
#endif
			)
		{
			void* ptr = malloc(count);
#if SAPPHIRE_MEMORY_TRACKER
			// 使用内存跟踪器，这个分配策略部分被莼处理内存池
			MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
			return ptr;
		}

		static inline void deallocateBytes(void* ptr)
		{
#if SAPPHIRE_MEMORY_TRACKER
			MemoryTracker::get()._recordDealloc(ptr);
#endif
			free(ptr);
		}

		///取得单次分配的最大值
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		//不进行实例化
		StdAllocPolicy()
		{ }
	};

	/**	
	对齐的分配权限
	一个对于使用AllocatedObj和STLAllocator标准的分配策略。
	这个类做是实际分配和释放物理内存，并如果你要改变如何分配内存，需提供一个自定义的版本
	
	这个类是代理了全局的malloc和free
	@note
	默认的对齐值=0，意味这依赖平台默认值

	*/
	template <size_t Alignment = 0>
	class StdAlignedAllocPolicy
	{
	public:
		// 在编译器检测参数的合法性
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

		static inline void* allocateBytes(size_t count,
#if SAPPHIRE_MEMORY_TRACKER
			const char* file = 0, int line = 0, const char* func = 0
#else
			const char* = 0, int = 0, const char* = 0
#endif
			)
		{
			void* ptr = Alignment ? AlignedMemory::allocate(count, Alignment)
				: AlignedMemory::allocate(count);
#if SAPPHIRE_MEMORY_TRACKER
			 
			MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
			return ptr;
		}

		static inline void deallocateBytes(void* ptr)
		{
#if SAPPHIRE_MEMORY_TRACKER
			MemoryTracker::get()._recordDealloc(ptr);
#endif
			AlignedMemory::deallocate(ptr);
		}

		 
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		 
		StdAlignedAllocPolicy()
		{ }
	};

#endif
	 

}


#endif