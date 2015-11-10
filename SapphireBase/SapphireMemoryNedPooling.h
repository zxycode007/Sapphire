/*
#ifndef __SAPPHIRE_MEMORY_NED_POOLING__
#define __SAPPHIRE_MEMORY_NED_POOLING__

#if SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_NEDPOOLING

namespace Sapphire
{
	 
	/** 
	非模板化的工具类用于封装nedmalloc
	*/
	class _SapphireExport NedPoolingImpl
	{
	public:
		static void* allocBytes(size_t count,
			const char* file, int line, const char* func);
		static void deallocBytes(void* ptr);
		static void* allocBytesAligned(size_t align, size_t count,
			const char* file, int line, const char* func);
		static void deallocBytesAligned(size_t align, void* ptr);

	};

	/**	
	一个用AllocatedObj和STLAllocator分配策略
	这个类实际上是做分配和释放物理内存
	@par
	分配策略是使用nedmalloc
	*/
	class _SapphireExport NedPoolingPolicy
	{
	public:
		static inline void* allocateBytes(size_t count,
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedPoolingImpl::allocBytes(count, file, line, func);
		}
		static inline void deallocateBytes(void* ptr)
		{
			NedPoolingImpl::deallocBytes(ptr);
		}
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}

	private:
		 
		NedPoolingPolicy()
		{ }
	};


	 
	template <size_t Alignment = 0>
	class NedPoolingAlignedPolicy
	{
	public:
		 
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment - 1)) == 0) ? +1 : -1];

		static inline void* allocateBytes(size_t count,
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedPoolingImpl::allocBytesAligned(Alignment, count, file, line, func);
		}

		static inline void deallocateBytes(void* ptr)
		{
			NedPoolingImpl::deallocBytesAligned(Alignment, ptr);
		}

		 
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		 
		NedPoolingAlignedPolicy()
		{ }
	};




 

} 

#endif 

#endif 

* /