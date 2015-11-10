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
	һ������ʹ��AllocatedObj��STLAllocator��׼�ķ�����ԡ�
	���������ʵ�ʷ�����ͷ������ڴ棬�������Ҫ�ı���η����ڴ棬���ṩһ���Զ���İ汾
	
	������Ǵ�����ȫ�ֵ�malloc��free
	*/
	class _SapphireExport StdAllocPolicy
	{
	public:
		//�����ڴ�
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
			// ʹ���ڴ�����������������Բ��ֱ�ݻ�����ڴ��
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

		///ȡ�õ��η�������ֵ
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		//������ʵ����
		StdAllocPolicy()
		{ }
	};

	/**	
	����ķ���Ȩ��
	һ������ʹ��AllocatedObj��STLAllocator��׼�ķ�����ԡ�
	���������ʵ�ʷ�����ͷ������ڴ棬�������Ҫ�ı���η����ڴ棬���ṩһ���Զ���İ汾
	
	������Ǵ�����ȫ�ֵ�malloc��free
	@note
	Ĭ�ϵĶ���ֵ=0����ζ������ƽ̨Ĭ��ֵ

	*/
	template <size_t Alignment = 0>
	class StdAlignedAllocPolicy
	{
	public:
		// �ڱ������������ĺϷ���
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