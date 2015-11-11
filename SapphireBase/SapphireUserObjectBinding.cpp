#include "SapphirePrerequisites.h"
#include "SapphireUserObjectBinding.h"

namespace Sapphire
{
	Any UserObjectBindings::msEmptyAny;

	//--------------------------------------------------------------------------
	UserObjectBindings::UserObjectBindings()
	{
		mAttributes = NULL;
	}

	//--------------------------------------------------------------------------
	UserObjectBindings::~UserObjectBindings()
	{
		clear();
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::setUserAny(const Any& anything)
	{
		// 按需求分配属性
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		mAttributes->mKeylessAny = anything;
	}

	//-----------------------------------------------------------------------
	const Any& UserObjectBindings::getUserAny(void) const
	{
		// 按需求分配属性
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		return mAttributes->mKeylessAny;
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::setUserAny(const String& key, const Any& anything)
	{
		// 按需求分配属性
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		// Map不存在
		if (mAttributes->mUserObjectsMap == NULL)
			mAttributes->mUserObjectsMap = new UserObjectsMap;

		(*mAttributes->mUserObjectsMap)[key] = anything;
	}

	//-----------------------------------------------------------------------
	const Any& UserObjectBindings::getUserAny(const String& key) const
	{
		// 按需求分配属性
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		// Map不存在
		if (mAttributes->mUserObjectsMap == NULL)
			return msEmptyAny;

		UserObjectsMapConstIterator it = mAttributes->mUserObjectsMap->find(key);

		// 判断数据是否存在
		if (it != mAttributes->mUserObjectsMap->end())
		{
			return it->second;
		}

		return msEmptyAny;
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::eraseUserAny(const String& key)
	{
		// 清除属性和Map分配
		if (mAttributes != NULL && mAttributes->mUserObjectsMap != NULL)
		{
			UserObjectsMapIterator it = mAttributes->mUserObjectsMap->find(key);

			// 如果对象找到，从Map清除它
			if (it != mAttributes->mUserObjectsMap->end())
			{
				mAttributes->mUserObjectsMap->erase(it);
			}
		}
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::clear() const
	{
		if (mAttributes != NULL)
		{
			SAPPHIRE_DELETE mAttributes;
			mAttributes = NULL;
		}
	}
}