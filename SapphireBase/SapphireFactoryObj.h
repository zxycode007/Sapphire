#ifndef _SAPPHIRE_FACTORY_OBJ_
#define _SAPPHIRE_FACTORY_OBJ_
#include "SapphirePrerequisites.h"

namespace Sapphire
{
	/** 
	抽象工厂类， 它自己不能做什么，但派生类可以添加功能
	*/
	template< typename T > class FactoryObj
	{
	public:
		virtual ~FactoryObj() {}

		/** 
		返回工厂类型
		@return
		工厂类型
		*/
		virtual const String& getType() const = 0;

		/** 
		创建一个新对象
		@param name 要创建的对象名
		@return
		通过工厂创建一个对象。这个对象的类型依赖这个工厂
		*/
		virtual T* createInstance(const String& name) = 0;
		/** 
		销毁一个通过这个工厂创建的对象
		@param ptr 指向这个销毁对象的指针
		*/
		virtual void destroyInstance(T*) = 0;
	};
}


#endif