#ifndef _SAPPHIRE_MEMORY_STL_ALLOCATOR_
#define _SAPPHIRE_MEMORY_STL_ALLOCATOR_

//#include "SapphirePrerequisites.h"

namespace Sapphire
{

	/** 
	��װ��STL�����ķ��������������������һ���������
	

	���������ģ�����
    T			: �������������
	AllocPolicy : �ڴ�������
	
    �ο�һ��ʵ�ֵĲ��Ե�����StdAllocPolicy�� ����Ҫʹʵ��һ��������ԣ�ʹ��ʵ��һ���Զ�����������򵥡�
	�������allocate/deallocate��ʵ�����ṩ���滻�Ĳ��ԣ�������������STL
	*/

	//STL Allocator�Ļ�����
	template<typename T>
	struct STLAllocatorBase
	{	// ͨ��Allocator�Ļ���
		typedef T value_type;
	};

	// STL allocator�Ļ��� (const T ��).
	template<typename T>
	struct STLAllocatorBase<const T>
	{	
		typedef T value_type;
	};

	template
		<
		typename T,
		typename AllocPolicy
		>
	class STLAllocator : public STLAllocatorBase<T>
	{
	public:
		// �������ǵ����ͣ���ISO C++��׼
		//���¶���
		typedef STLAllocatorBase<T>			Base;        //���ͽṹ��
		typedef typename Base::value_type	value_type;   //ֵ����  T������
		typedef value_type*					pointer;      //ֵ����ָ��
		typedef const value_type*			const_pointer;  //��ָ��
		typedef value_type&					reference;      //����
		typedef const value_type&			const_reference;   //������
		typedef std::size_t					size_type;      //��С����
		typedef std::ptrdiff_t				difference_type;      //��ͬ����


		/// ��׼�����°󶨻���
		template<typename U>
		struct rebind
		{
			typedef STLAllocator<U, AllocPolicy> other;
		};

		/// ���캯��  explicit��ֹʹ����ʽ���캯��
		inline explicit STLAllocator()
		{ }

		/// ��������
		virtual ~STLAllocator()
		{ }

		/// �������캯��
		inline STLAllocator(STLAllocator const&)
		{ }

		/// ����ת��
		template <typename U>
		inline STLAllocator(STLAllocator<U, AllocPolicy> const&)
		{ }

		/// ����ת��
		template <typename U, typename P>
		inline STLAllocator(STLAllocator<U, P> const&)
		{ }

		/// �ڴ����  (Ԫ��, ��STL)
		inline pointer allocate(size_type count,
			typename std::allocator<void>::const_pointer ptr = 0)     //typename ���ض���Ϊ Base::value_type  Base������ΪSTLAlloctor�� value_type����ΪT
		{
			(void)ptr;
			// �������T������ֽ���
			register size_type sz = count*sizeof(T);
			pointer p = static_cast<pointer>(AllocPolicy::allocateBytes(sz));
			return p;
		}

		/// �ڴ��ͷ�(Ԫ��, ��STL)
		inline void deallocate(pointer ptr, size_type)
		{
			//����Ҫ�������T������ֽ���
			// ע�� size_type sz = count*sizeof( T );
			AllocPolicy::deallocateBytes(ptr);
		}

		pointer address(reference x) const
		{
			return &x;
		}

		const_pointer address(const_reference x) const
		{
			return &x;
		}

		size_type max_size() const throw()
		{
			// ȡ�����ɷ����С 
			return AllocPolicy::getMaxAllocationSize();
		}

		void construct(pointer p)
		{
			// ����placement new ���ѷ���ĵ�ַp�����ٷ��䴴������T
			new(static_cast<void*>(p)) T();
		}

		void construct(pointer p, const T& val)
		{
			// ����placement new ���ѷ���ĵ�ַp�����ٷ��䴴������T������T���������췽��
			new(static_cast<void*>(p)) T(val);
		}

		void destroy(pointer p)
		{
			 
			p->~T();
		}

	};

	/// ����== �ܷ�ͨ��������������ͷ��������������ڴ�
	template<typename T, typename T2, typename P>
	inline bool operator==(STLAllocator<T, P> const&,
		STLAllocator<T2, P> const&)
	{
		// ��ͬ�ķ������P���ڴ���Ա��ͷŵ�
		return true;
	}

	//���� == �ܷ�ͨ��������������ͷ��������������ڴ�
	template<typename T, typename P, typename OtherAllocator>
	inline bool operator==(STLAllocator<T, P> const&,
		OtherAllocator const&)
	{
		return false;
	}
	//���� == �ܷ�ͨ��������������ͷ��������������ڴ�
	template<typename T, typename T2, typename P>
	inline bool operator!=(STLAllocator<T, P> const&,
		STLAllocator<T2, P> const&)
	{
		return false;
	}

	//���� == �ܷ�ͨ��������������ͷ��������������ڴ�
	template<typename T, typename P, typename OtherAllocator>
	inline bool operator!=(STLAllocator<T, P> const&,
		OtherAllocator const&)
	{
		return true;
	}


	/** @} */
	/** @} */
}

#endif