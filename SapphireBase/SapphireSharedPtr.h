
#ifndef __SAPPHIRE_SHARED_PTR__
#define __SAPPHIRE_SHARED_PTR__

#include "SapphirePrerequisites.h"

namespace Sapphire {
	

	/// ������������ͷ��ڴ�
	enum SharedPtrFreeMethod
	{
		///�� SAPPHIRE_DELETE ȥ�ͷ��ڴ�
		SPFM_DELETE,
		/// �� SAPPHIRE_DELETE_T ȥ�ͷ�MEMCATEGORY_GENERAL
		SPFM_DELETE_T,
		///��SAPPHIRE_FREE ȥ�ͷ� MEMCATEGORY_GENERAL
		SPFM_FREE
	};

	/** 
	���ü���sharedָ�룬������Ҫ��ʽ���ٵĶ���
	@remarks
	����һ����׼sharedָ���ʵ�֣�����һ�����õļ��� 
	����һ����׼��shared pointer ����һ����׼��sharedָ������ʵ�ֵ�ȥɾ��һ���������ü���
	@par
	��SAPPHIRE_THREAD_SUPPORT ����Ϊ1��������������̰߳�ȫ��
	*/
	template<class T> class SharedPtr
	{
	protected:
		T* pRep;         //���ö���
		unsigned int* pUseCount;       //���ü���
		SharedPtrFreeMethod useFreeMethod; // Ӧ��ʹ��SAPPHIRE_FREE ���SAPPHIRE_DELETE
	public:
		SAPPHIRE_AUTO_SHARED_MUTEX // �������������
			/** 
			�������������ʼ��SharedPtr
			@remarks
			Σ�� ���������ʹ��SharedPtr֮ǰ����bind()
			*/
			SharedPtr() : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
		}

		/** ������.
		@param rep ָ��
		@param freeMode �ͷ�ָ��Ļ���
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
			// �����͸������mutexָ��

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
			// ������ǰ���ݵ��ֲ��ֲ������У��������ȷ�����Ǵ���RHS
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
			: pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// �������Ҹ���������mutexָ��

			SAPPHIRE_SET_AUTO_SHARED_MUTEX_NULL
				SAPPHIRE_MUTEX_CONDITIONAL(r.SAPPHIRE_AUTO_MUTEX_NAME)
			{
				SAPPHIRE_LOCK_MUTEX(*r.SAPPHIRE_AUTO_MUTEX_NAME)
					SAPPHIRE_COPY_AUTO_SHARED_MUTEX(r.SAPPHIRE_AUTO_MUTEX_NAME)
					pRep = r.getPointer();
				pUseCount = r.useCountPointer();
				useFreeMethod = r.freeMethod();
				// ����0ָ�����STL����
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

		/** �����õ�SharedPtr
		@remarks
		�������SharedPtr ��δ��ʼ����
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
				// ���ܷ�Χ����mutexǰ�����ⱻɾ����������ͷ�
				release();
				pRep = 0;
				pUseCount = 0;
			}
		}

	protected:

		//�ͷ�
		inline void release(void)
		{
			bool destroyThis = false;

			/* 
			������mutexû�г�ʼ��Ϊ��0ֵ����ô����pUseCountҲ����rRep 
			*/

			SAPPHIRE_MUTEX_CONDITIONAL(SAPPHIRE_AUTO_MUTEX_NAME)
			{
				// ����mutex���޶��ķ�Χ,����������ǰ����
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

		//ɾ�� ����ò�ֱ�ӵ��ã�
		virtual void destroy(void)
		{
			// ���ȡ��һ���������ڹر�ǰ�����ͷ�ָ��
			// ��UsetNull() ǰ�رջ���ȷ��ָ������Sapphire�ķ�Χ���ٹر���Ч��
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
			// ����SAPPHIRE_FREE���SAPPHIRE_DELETE_T ��Ϊ'unsigned int'û�й�����
			// ����ֻ��SAPPHIRE_NEW_T��ʹ�ù�����
			SAPPHIRE_FREE(pUseCount, MEMCATEGORY_GENERAL);
			SAPPHIRE_DELETE_AUTO_SHARED_MUTEX
		}

		//�����ָ�������������ָ�뽻��
		virtual void swap(SharedPtr<T> &other)
		{
			//���ݽ���
			std::swap(pRep, other.pRep);
			//���ô�������
			std::swap(pUseCount, other.pUseCount);
			//�ͷŷ�������
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
