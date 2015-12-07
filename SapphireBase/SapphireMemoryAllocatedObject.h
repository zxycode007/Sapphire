#ifndef _SAPPHIRE_MEMORY_ALLOCATED_OBJECT_
#define _SAPPHIRE_MEMORY_ALLOCATED_OBJECT_

#include "SapphirePlatfrom.h"
//#include "SapphirePrerequisites.h"

//ȡ��c++ ��new delete �ڴ����ͻ��շ�ʽ�����Զ���ĵ�new delete�����ڴ�
#ifdef new
#  undef new
#endif
#ifdef delete
#  undef delete
#endif

namespace Sapphire
{
	//�������Ϊ����ʹ���Զ����ڴ����������Ķ���ĸ���
	//��Ҫһ��ģ������������ڴ������ԣ�stdAllocPolicy��
	//�� AllocatedObj<StdAllocPolicy>
	template<class Alloc>
	class _SapphireExport AllocatedObj
	{
	public:
		explicit AllocatedObj()
		{ }

		~AllocatedObj()
		{ }

		//�ض���new�����
		void* operator new(size_t sz, const char* file, int line, const char* func)
		{
			return Alloc::allocateBytes(sz, file, line, func);
		}

			void* operator new(size_t sz)
		{
			return Alloc::allocateBytes(sz);
		}

			/// �滻new���޲���
			void* operator new(size_t sz, void* ptr)
		{
			(void)sz;
			return ptr;
		}

			/// ����ķ��䣬 ����debug������Ϣ
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

