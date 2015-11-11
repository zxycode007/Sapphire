#ifndef _SAPPHIRE_USER_OBJECT_BINDING_
#define _SAPPHIRE_USER_OBJECT_BINDING_
#include "SapphirePrerequisites.h"
#include "SapphireAny.h"

namespace Sapphire
{

	/** 
	�����Ϊ���û��Զ���Ӧ�ó��������Sapphire�ں���֮�佨����ϵ�ṩ����Ľӿ�
	�κν�Ҫ��Ҫ�������ӿڵ��Զ���Ӧ�ó�����������Sapphire��ʵ�����ɴ�������
	*/
	class _SapphireExport UserObjectBindings : public GeneralAllocatedObject
	{
	public:
		/** ���캯��. */
		UserObjectBindings();
				
		/** �������� */
		virtual ~UserObjectBindings();

		/** 
		������������ʵ���ϵ��κ����͵��û�����
		@remarks
		����������������һ��������û���������ࡣ���ܹ�����һ�����Լ���ʵ����ָ��
		@note 
		���������û��Key����ζ��ÿ�����ý���д��ǰ�����õĶ����������Ҫ���������������࣬����չ�Ĵ�key�汾
		*/
		void setUserAny(const Any& anything);

		/** 
		���ع������������Զ����û��Key���û�����
		*/
		const Any& getUserAny(void) const;

		/** �����������ʵ���ϵ������û�����
		@remarks
		���������������������������࣬
		�������Ϊһ���ص�����ʵ�����Լ������ָ�롣��һ��Ψһ��key��������Щ����
		@param key ���������ݵ�Key
		@param anything ����Key�Ĺ�������
		*/
		void setUserAny(const String& key, const Any& anything);

		/** 
		���ع������������Զ����û����� 
		@param key �����û������Key
		@remarks
		���û�ж�����������Key���򷵻ؿ�
		*/
		const Any& getUserAny(const String& key) const;

		/** �����������������key�󶨵��û��Զ������
		@param key �����û������Key
		*/
		void eraseUserAny(const String& key);

		/** ����û��󶨵����ж���	*/
		void clear() const;

		/** ���ؿյ��û��������
		*/
		static const Any& getEmptyUserAny() { return msEmptyAny; }

		// Types.
	protected:
		typedef map<String, Any>::type			UserObjectsMap;
		typedef UserObjectsMap::iterator		UserObjectsMapIterator;
		typedef UserObjectsMap::const_iterator	UserObjectsMapConstIterator;

		/** 
		�������ݴ洢�������ڲ���
		*/
		class Attributes : public GeneralAllocatedObject
		{
		public:
			/** ���Դ洢������ */
			Attributes() : mUserObjectsMap(NULL) {}

			/** ������ */
			~Attributes()
			{
				if (mUserObjectsMap != NULL)
				{
					SAPPHIRE_DELETE mUserObjectsMap;
					mUserObjectsMap = NULL;
				}
			}

			Any					mKeylessAny;		// ��������Key���û��������ڿ��ٷ���
			UserObjectsMap*		mUserObjectsMap;	// ���ڱ���һ��Map�����ڱ����û����������Key
		};

		// ����
	private:
		static Any				msEmptyAny;			// ����һ���յ��������
		mutable Attributes*		mAttributes;		// ������ �����������

	};
}


#endif