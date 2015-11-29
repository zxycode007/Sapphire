
#ifndef __SAPPHIRE_SHARED_PTR__
#define __SAPPHIRE_SHARED_PTR__

#include "SapphirePrerequisites.h"

namespace Sapphire {
	

	/// 这个方法用于释放内存
	enum SharedPtrFreeMethod
	{
		///用 SAPPHIRE_DELETE 去释放内存
		SPFM_DELETE,
		/// 用 SAPPHIRE_DELETE_T 去释放MEMCATEGORY_GENERAL
		SPFM_DELETE_T,
		///用SAPPHIRE_FREE 去释放 MEMCATEGORY_GENERAL
		SPFM_FREE
	};

	/** 
	引用计数shared指针，用于需要隐式销毁的对象
	@remarks
	这是一个标准shared指针的实现，它用一个引用的计数 
	这是一个标准的shared pointer 这是一个标准的shared指针用于实现当去删除一个对象引用计数
	@par
	如SAPPHIRE_THREAD_SUPPORT 定义为1，用于这个类是线程安全的
	*/
	template<class T> class SharedPtr
	{
	protected:
		T* pRep;         //引用对象
		unsigned int* pUseCount;       //引用计数
		SharedPtrFreeMethod useFreeMethod; // 应该使用SAPPHIRE_FREE 替代SAPPHIRE_DELETE
	public:
		SAPPHIRE_AUTO_SHARED_MUTEX // 公共允许额外锁
			/** 
			构造器，不会初始化SharedPtr
			@remarks
			危险 ，你必须在使用SharedPtr之前调用bind()
			*/
			SharedPtr() : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
		}

		/** 构造器.
		@param rep 指针
		@param freeMode 释放指针的机制
		*/
		template< class Y>
		explicit SharedPtr(Y* rep, SharedPtrFreeMethod inFreeMethod = SPFM_DELETE)
			: pRep(rep)
			, pUseCount(rep ? SAPPHIRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1) : 0)
			, useFreeMethod(inFreeMethod)
		{
			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
				if (rep)
				{
					SAPPHIRE_NEW_AUTO_SHARED_MUTEX
				}
		} 
		SharedPtr(const SharedPtr& r)
			: pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// 锁定和复制这个mutex指针

			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
				SAPPHIRE_MUTEX_CONDITIONAL(r.SAPPHIRE_AUTO_MUTEX_NAME)
			{
				SAPPHIRE_LOCK_MUTEX(*r.SAPPHIRE_AUTO_MUTEX_NAME)
					SAPPHIRE_COPY_AUTO_SHARED_MUTEX(r.SAPPHIRE_AUTO_MUTEX_NAME)
					pRep = r.pRep;
				pUseCount = r.pUseCount;
				useFreeMethod = r.useFreeMethod;
				// Handle zero pointer gracefully to manage STL containers
				if (pUseCount)
				{
					++(*pUseCount);
				}
			}
		}
		SharedPtr& operator=(const SharedPtr& r) {
			if (pRep == r.pRep)
				return *this;
			// 交换当前数据到局部局部变量中，复制这个确保我们处理RHS
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
			: pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// 锁定并且复制其它的mutex指针

			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
				SAPPHIRE_MUTEX_CONDITIONAL(r.SAPPHIRE_AUTO_MUTEX_NAME)
			{
				SAPPHIRE_LOCK_MUTEX(*r.SAPPHIRE_AUTO_MUTEX_NAME)
					SAPPHIRE_COPY_AUTO_SHARED_MUTEX(r.SAPPHIRE_AUTO_MUTEX_NAME)
					pRep = r.getPointer();
				pUseCount = r.useCountPointer();
				useFreeMethod = r.freeMethod();
				// 控制0指针管理STL容器
				if (pUseCount)
				{
					++(*pUseCount);
				}
			}
		}
		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r) {
			if (pRep == r.getPointer())
				return *this;
			
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		virtual ~SharedPtr() {
			release();
		}


		inline T& operator*() const { assert(pRep); return *pRep; }
		inline T* operator->() const { assert(pRep); return pRep; }
		inline T* get() const { return pRep; }

		/** 绑定引用到SharedPtr
		@remarks
		关联这个SharedPtr 是未初始化的
		*/
		void bind(T* rep, SharedPtrFreeMethod inFreeMethod = SPFM_DELETE) {
			assert(!pRep && !pUseCount);
			SAPPHIRE_NEW_AUTO_SHARED_MUTEX
				SAPPHIRE_LOCK_AUTO_SHARED_MUTEX
				pUseCount = SAPPHIRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1);
			pRep = rep;
			useFreeMethod = inFreeMethod;
		}

		inline bool unique() const { SAPPHIRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
		inline unsigned int useCount() const { SAPPHIRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
		inline unsigned int* useCountPointer() const { return pUseCount; }

		inline T* getPointer() const { return pRep; }
		inline SharedPtrFreeMethod freeMethod() const { return useFreeMethod; }

		inline bool isNull(void) const { return pRep == 0; }

		inline void setNull(void) {
			if (pRep)
			{
				// 不能范围锁定mutex前，以免被删除的情况先释放
				release();
				pRep = 0;
				pUseCount = 0;
			}
		}

	protected:

		//释放
		inline void release(void)
		{
			bool destroyThis = false;

			/* 
			如果这个mutex没有初始化为非0值，那么不是pUseCount也不是rRep 
			*/

			SAPPHIRE_MUTEX_CONDITIONAL(SAPPHIRE_AUTO_MUTEX_NAME)
			{
				// 锁定mutex在限定的范围,必须在销毁前解锁
				SAPPHIRE_LOCK_AUTO_SHARED_MUTEX
					if (pUseCount)
					{
						if (--(*pUseCount) == 0)
						{
							destroyThis = true;
						}
					}
			}
			if (destroyThis)
				destroy();

			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
		}

		//删除 （最好不直接调用）
		virtual void destroy(void)
		{
			// 如果取得一个崩溃，在关闭前放弃释放指针
			// 用UsetNull() 前关闭或者确定指针脱离Sapphire的范围，再关闭无效化
			switch (useFreeMethod)
			{
			case SPFM_DELETE:
				SAPPHIRE_DELETE pRep;
				break;
			case SPFM_DELETE_T:
				SAPPHIRE_DELETE_T(pRep, T, MEMCATEGORY_GENERAL);
				break;
			case SPFM_FREE:
				SAPPHIRE_FREE(pRep, MEMCATEGORY_GENERAL);
				break;
			};
			// 用于SAPPHIRE_FREE替代SAPPHIRE_DELETE_T 因为'unsigned int'没有构造器
			// 我们只用SAPPHIRE_NEW_T来使用构造器
			SAPPHIRE_FREE(pUseCount, MEMCATEGORY_GENERAL);
			SAPPHIRE_DELETE_AUTO_SHARED_MUTEX
		}

		//拿这个指针的数据与其它指针交换
		virtual void swap(SharedPtr<T> &other)
		{
			//数据交换
			std::swap(pRep, other.pRep);
			//引用次数交换
			std::swap(pUseCount, other.pUseCount);
			//释放方法交换
			std::swap(useFreeMethod, other.useFreeMethod);
#if SAPPHIRE_THREAD_SUPPORT
			std::swap(SAPPHIRE_AUTO_MUTEX_NAME, other.SAPPHIRE_AUTO_MUTEX_NAME);
#endif
		}
	};
	//
	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}
	 
	 
}



#endif
