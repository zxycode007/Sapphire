#ifndef _SAPPHIRE_ALIGNED_ALLOCATOR_
#define _SAPPHIRE_ALIGNED_ALLOCATOR_

namespace Sapphire {

	/** 
	
	提供计算分配对齐内存的类
	所有的SIMD处理都需要对齐内存， 有些SIMD被设计为只能对齐内存才能工作。 如果数据打算使用SIMD来处理，
	它需要对齐才能获得更高的性能。
	另外，即使不使用SIMD，大多数缓冲区边界对齐数据也能提高性能。所以这个类提供了一对分配对齐内存的函数
	
	无论如何，通常情况下，你不需要直接使用这个类，Sapphire内部将尽可能使用SIMD和cache优化。
	这些并非一步完成，有许多潜在的工作要做、如果你不知道你要怎样去进行，那么可以跳过这个类
	
	*/
	class _SapphireExport AlignedMemory
	{
	public:
		/** 用给定对齐值来分配内存
		@param
		size 需要分配的内存大小
		@param
		alignment 对齐值的结果指针，必须为2的幂，范围必须在1~128
		@return
		The 分配内存后的指针
		@par
		On 如果失败抛出异常
		*/
		static void* allocate(size_t size, size_t alignment);

		/** 分配内存用平台默认的对齐值
		@remarks
		这个默认的对齐值依赖于Sapphire默认的设定值，这个函数SIMD和缓冲区边界使用对齐的内存
		@param
		size 分配内存的大小
		@return
		The 分配内存后的指针
		@par
		如果失败抛出异常
		*/
		static void* allocate(size_t size);

		/** 释放掉这个类分配的内存
		@param
		p 指向这个类分配内存的指针或者NULL指针
		@par
		如果是空指针，则不会进行操作
		*/
		static void deallocate(void* p);
	};

	/** @} */
	/** @} */

}


#endif