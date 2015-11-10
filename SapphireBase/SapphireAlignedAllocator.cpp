#include "SapphirePrerequisites.h"
#include "SapphireAlignedAllocator.h"
#include "SapphireMath.h"

/**
*
* |___2___|3|_________5__________|__6__|
* ^         ^
* 1         4
*
* 1 -> ָ��ͨ��new ������ڴ�����ʼ��ַ
* 2 -> ���ڶ�����ֵ��4�ֽڶ���ļ�϶
* 3 -> 1��4ֱ�ӵ��ֽ�ƫ����
* 4 -> ָ��������ݿ����ʼ��ַ
* 5 -> ���ݿ�
* 6 -> ���ݿ�β���˷ѵ��ڴ�
*/

namespace Sapphire {

	//---------------------------------------------------------------------
	void* AlignedMemory::allocate(size_t size, size_t alignment)
	{
		//�ж϶���ķ�Χ���Ƿ�Ϸ� 2���ݣ�������1~128֮��
		assert(0 < alignment && alignment <= 128 && BitWise::isPO2(alignment));

		//�����ڴ�
		unsigned char* p = new unsigned char[size + alignment];
		//����ƫ�Ƶ�ַ   ͨ��p��Դ��ַ�Ͷ���ֵ��
		size_t offset = alignment - (size_t(p) & (alignment - 1));   //(size_t(p) & (alignment - 1))ʵ��������p�ĵ�ַ��alignment��ģ����   /ʵ������ԭ�� ����Ϊalignment����log2��alignment��λ���������Ƶ��߽����ֵ��log2��alignment����1����������

		unsigned char* result = p + offset;
		result[-1] = (unsigned char)offset; //ǰһ������ƫ��ֵ

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


