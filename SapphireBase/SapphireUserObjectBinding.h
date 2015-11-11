#ifndef _SAPPHIRE_USER_OBJECT_BINDING_
#define _SAPPHIRE_USER_OBJECT_BINDING_
#include "SapphirePrerequisites.h"
#include "SapphireAny.h"

namespace Sapphire
{

	/** 
	这个类为在用户自定义应用程序对象与Sapphire内核类之间建立联系提供方便的接口
	任何将要与要用这个类接口的自定义应用程序对象关联的Sapphire类实例将由此类派生
	*/
	class _SapphireExport UserObjectBindings : public GeneralAllocatedObject
	{
	public:
		/** 构造函数. */
		UserObjectBindings();
				
		/** 析构函数 */
		virtual ~UserObjectBindings();

		/** 
		设置在这个类的实例上的任何类型的用户对象
		@remarks
		这个方法允许你关联一个任意的用户对象到这个类。这能够返回一个你自己类实例的指针
		@note 
		这个方法是没有Key的意味着每个调用将重写先前的设置的对象，如果你需要关联多个对象到这个类，用扩展的带key版本
		*/
		void setUserAny(const Any& anything);

		/** 
		返回关联到这个类的自定义的没有Key的用户对象
		*/
		const Any& getUserAny(void) const;

		/** 设置在这个类实例上的任意用户对象
		@remarks
		这个方法允许你关联多个对象到这个类，
		这可以作为一个回到对于实例你自己的类的指针。用一个唯一的key来区分这些对象
		@param key 关联的数据的Key
		@param anything 给定Key的关联数据
		*/
		void setUserAny(const String& key, const Any& anything);

		/** 
		返回关联到这个类的自定义用户对象 
		@param key 关联用户对象的Key
		@remarks
		如果没有对象关联到这个Key，则返回空
		*/
		const Any& getUserAny(const String& key) const;

		/** 清除关联到这个类和用key绑定的用户自定义对象
		@param key 关联用户对象的Key
		*/
		void eraseUserAny(const String& key);

		/** 清空用户绑定的所有对象	*/
		void clear() const;

		/** 返回空的用户任意对象
		*/
		static const Any& getEmptyUserAny() { return msEmptyAny; }

		// Types.
	protected:
		typedef map<String, Any>::type			UserObjectsMap;
		typedef UserObjectsMap::iterator		UserObjectsMapIterator;
		typedef UserObjectsMap::const_iterator	UserObjectsMapConstIterator;

		/** 
		用作数据存储容器的内部类
		*/
		class Attributes : public GeneralAllocatedObject
		{
		public:
			/** 属性存储构造器 */
			Attributes() : mUserObjectsMap(NULL) {}

			/** 析构器 */
			~Attributes()
			{
				if (mUserObjectsMap != NULL)
				{
					SAPPHIRE_DELETE mUserObjectsMap;
					mUserObjectsMap = NULL;
				}
			}

			Any					mKeylessAny;		// 将保持无Key的用户对象，用于快速访问
			UserObjectsMap*		mUserObjectsMap;	// 用于保存一个Map，用于保存用户对象和它的Key
		};

		// 属性
	private:
		static Any				msEmptyAny;			// 共享一个空的任意对象
		mutable Attributes*		mAttributes;		// 类属性 ，按需求分配

	};
}


#endif