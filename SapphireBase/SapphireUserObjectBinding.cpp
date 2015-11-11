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
		// �������������
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		mAttributes->mKeylessAny = anything;
	}

	//-----------------------------------------------------------------------
	const Any& UserObjectBindings::getUserAny(void) const
	{
		// �������������
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		return mAttributes->mKeylessAny;
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::setUserAny(const String& key, const Any& anything)
	{
		// �������������
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		// Map������
		if (mAttributes->mUserObjectsMap == NULL)
			mAttributes->mUserObjectsMap = new UserObjectsMap;

		(*mAttributes->mUserObjectsMap)[key] = anything;
	}

	//-----------------------------------------------------------------------
	const Any& UserObjectBindings::getUserAny(const String& key) const
	{
		// �������������
		if (mAttributes == NULL)
			mAttributes = SAPPHIRE_NEW UserObjectBindings::Attributes;

		// Map������
		if (mAttributes->mUserObjectsMap == NULL)
			return msEmptyAny;

		UserObjectsMapConstIterator it = mAttributes->mUserObjectsMap->find(key);

		// �ж������Ƿ����
		if (it != mAttributes->mUserObjectsMap->end())
		{
			return it->second;
		}

		return msEmptyAny;
	}

	//-----------------------------------------------------------------------
	void UserObjectBindings::eraseUserAny(const String& key)
	{
		// ������Ժ�Map����
		if (mAttributes != NULL && mAttributes->mUserObjectsMap != NULL)
		{
			UserObjectsMapIterator it = mAttributes->mUserObjectsMap->find(key);

			// ��������ҵ�����Map�����
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