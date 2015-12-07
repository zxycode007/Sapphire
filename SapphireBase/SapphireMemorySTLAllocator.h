#ifndef _SAPPHIRE_MEMORY_STL_ALLOCATOR_
#define _SAPPHIRE_MEMORY_STL_ALLOCATOR_

//#include "SapphirePrerequisites.h"

namespace Sapphire
{

	/** 
	包装了STL容器的分配器，这个类用于主控一个分配策略
	

	它有下面的模板参数
    T			: 将被分配的类型
	AllocPolicy : 内存分配策略
	
    参看一个实现的策略的例子StdAllocPolicy。 由于要使实现一个分配策略，使得实现一个自定义分配器更简单。
	你必须用allocate/deallocate的实现来提供可替换的策略；这个类关联管理STL
	*/

	//STL Allocator的基类类
	template<typename T>
	struct STLAllocatorBase
	{	// 通用Allocator的基类
		typedef T value_type;
	};

	// STL allocator的基类 (const T 版).
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
		// 定义我们的类型，按ISO C++标准
		//重新定义
		typedef STLAllocatorBase<T>			Base;        //类型结构体
		typedef typename Base::value_type	value_type;   //值类型  T的类型
		typedef value_type*					pointer;      //值类型指针
		typedef const value_type*			const_pointer;  //常指针
		typedef value_type&					reference;      //引用
		typedef const value_type&			const_reference;   //常引用
		typedef std::size_t					size_type;      //大小类型
		typedef std::ptrdiff_t				difference_type;      //不同类型


		/// 标准的重新绑定机制
		template<typename U>
		struct rebind
		{
			typedef STLAllocator<U, AllocPolicy> other;
		};

		/// 构造函数  explicit阻止使用隐式构造函数
		inline explicit STLAllocator()
		{ }

		/// 析构函数
		virtual ~STLAllocator()
		{ }

		/// 拷贝构造函数
		inline STLAllocator(STLAllocator const&)
		{ }

		/// 类型转换
		template <typename U>
		inline STLAllocator(STLAllocator<U, AllocPolicy> const&)
		{ }

		/// 类型转换
		template <typename U, typename P>
		inline STLAllocator(STLAllocator<U, P> const&)
		{ }

		/// 内存分配  (元素, 用STL)
		inline pointer allocate(size_type count,
			typename std::allocator<void>::const_pointer ptr = 0)     //typename 被重定义为 Base::value_type  Base被定义为STLAlloctor， value_type定义为T
		{
			(void)ptr;
			// 求出类型T所需的字节数
			register size_type sz = count*sizeof(T);
			pointer p = static_cast<pointer>(AllocPolicy::allocateBytes(sz));
			return p;
		}

		/// 内存释放(元素, 用STL)
		inline void deallocate(pointer ptr, size_type)
		{
			//不需要求出类型T所需的字节数
			// 注册 size_type sz = count*sizeof( T );
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
			// 取得最大可分配大小 
			return AllocPolicy::getMaxAllocationSize();
		}

		void construct(pointer p)
		{
			// 调用placement new 在已分配的地址p上上再分配创建对象T
			new(static_cast<void*>(p)) T();
		}

		void construct(pointer p, const T& val)
		{
			// 调用placement new 在已分配的地址p上上再分配创建对象T，并用T带参数构造方法
			new(static_cast<void*>(p)) T(val);
		}

		void destroy(pointer p)
		{
			 
			p->~T();
		}

	};

	/// 重载== 能否通过这个分配器来释放其它分配器的内存
	template<typename T, typename T2, typename P>
	inline bool operator==(STLAllocator<T, P> const&,
		STLAllocator<T2, P> const&)
	{
		// 相同的分配策略P，内存可以被释放掉
		return true;
	}

	//重载 == 能否通过这个分配器来释放其它分配器的内存
	template<typename T, typename P, typename OtherAllocator>
	inline bool operator==(STLAllocator<T, P> const&,
		OtherAllocator const&)
	{
		return false;
	}
	//重载 == 能否通过这个分配器来释放其它分配器的内存
	template<typename T, typename T2, typename P>
	inline bool operator!=(STLAllocator<T, P> const&,
		STLAllocator<T2, P> const&)
	{
		return false;
	}

	//重载 == 能否通过这个分配器来释放其它分配器的内存
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