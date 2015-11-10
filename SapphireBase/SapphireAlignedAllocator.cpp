#include "SapphirePrerequisites.h"
#include "SapphireAlignedAllocator.h"
#include "SapphireMath.h"

/**
*
* |___2___|3|_________5__________|__6__|
* ^         ^
* 1         4
*
* 1 -> 指向通过new 分配的内存块的起始地址
* 2 -> 对于定对齐值用4字节对齐的间隙
* 3 -> 1到4直接的字节偏移量
* 4 -> 指针对于数据块的起始地址
* 5 -> 数据块
* 6 -> 数据块尾部浪费的内存
*/

namespace Sapphire {

	//---------------------------------------------------------------------
	void* AlignedMemory::allocate(size_t size, size_t alignment)
	{
		//判断对齐的范围是是否合法 2的幂，并且在1~128之间
		assert(0 < alignment && alignment <= 128 && BitWise::isPO2(alignment));

		//分配内存
		unsigned char* p = new unsigned char[size + alignment];
		//计算偏移地址   通过p的源地址和对齐值的
		size_t offset = alignment - (size_t(p) & (alignment - 1));   //(size_t(p) & (alignment - 1))实际上是求p的地址与alignment的模运算   /实际求余原理 余数为alignment右移log2（alignment）位，并对右移到边界外的值和log2（alignment）个1进行与运算

		unsigned char* result = p + offset;
		result[-1] = (unsigned char)offset; //前一个保存偏移值

		return result;
	}
	//---------------------------------------------------------------------
	void* AlignedMemory::allocate(size_t size)
	{
		return allocate(size, SAPPHIRE_SIMD_ALIGNMENT);
	}
	//---------------------------------------------------------------------
	void AlignedMemory::deallocate(void* p)
	{
		if (p)
		{
			unsigned char* mem = (unsigned char*)p;
			mem = mem - mem[-1];
			delete[] mem;
		}
	}

}


