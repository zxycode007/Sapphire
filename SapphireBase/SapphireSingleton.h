
#ifndef _SAPPHIRE_SINGLETON_H__
#define _SAPPHIRE_SINGLETON_H__

#include "SapphirePrerequisites.h"
#include "SapphireHeaderPrefix.h"

#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
// ���ڵ�̬ģʽ�رվ���Turn off 
#   pragma warning (disable : 4311)
#   pragma warning (disable : 4312)
#endif

#if defined ( SAPPHIRE_GCC_VISIBILITY )
#   pragma GCC visibility push(default)
#endif
namespace Sapphire {
	

	//
	/** ������̬ȫ�����ģ����
	Ҫʹ�õ�̬ģʽ����Ӧ�ü̳и���
	*/
	template <typename T> class Singleton
	{
	private:
		/** \brief Explicit ˽�еĿ���������,����ֹ�Ĳ���*/
		Singleton(const Singleton<T> &);

		/** \brief ˽�в�����= ����ֹ�Ĳ��� */
		Singleton& operator=(const Singleton<T> &);

	protected:

		static T* msSingleton;

	public:
		Singleton(void)
		{
			assert(!msSingleton);
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			msSingleton = (T*)((int)this + offset);
#else
			msSingleton = static_cast< T* >(this);
#endif
		}
		~Singleton(void)
		{
			assert(msSingleton);  msSingleton = 0;
		}
		static T& getSingleton(void)
		{
			assert(msSingleton);  return (*msSingleton);
		}
		static T* getSingletonPtr(void)
		{
			return msSingleton;
		}
	};
	

}
#if defined ( SAPPHIRE_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

#include "SapphireHeaderSuffix.h"
#endif
