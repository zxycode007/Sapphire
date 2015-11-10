
#ifndef _SAPPHIRE_SINGLETON_H__
#define _SAPPHIRE_SINGLETON_H__

#include "SapphirePrerequisites.h"
#include "SapphireHeaderPrefix.h"

#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
// 对于单态模式关闭警告Turn off 
#   pragma warning (disable : 4311)
#   pragma warning (disable : 4312)
#endif

#if defined ( SAPPHIRE_GCC_VISIBILITY )
#   pragma GCC visibility push(default)
#endif
namespace Sapphire {
	

	//
	/** 创建单态全局类的模板类
	要使用单态模式的类应该继承该类
	*/
	template <typename T> class Singleton
	{
	private:
		/** \brief Explicit 私有的拷贝构造器,被禁止的操作*/
		Singleton(const Singleton<T> &);

		/** \brief 私有操作符= 被禁止的操作 */
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
