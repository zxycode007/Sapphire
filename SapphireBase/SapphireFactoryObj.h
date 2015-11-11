#ifndef _SAPPHIRE_FACTORY_OBJ_
#define _SAPPHIRE_FACTORY_OBJ_
#include "SapphirePrerequisites.h"

namespace Sapphire
{
	/** 
	���󹤳��࣬ ���Լ�������ʲô���������������ӹ���
	*/
	template< typename T > class FactoryObj
	{
	public:
		virtual ~FactoryObj() {}

		/** 
		���ع�������
		@return
		��������
		*/
		virtual const String& getType() const = 0;

		/** 
		����һ���¶���
		@param name Ҫ�����Ķ�����
		@return
		ͨ����������һ�����������������������������
		*/
		virtual T* createInstance(const String& name) = 0;
		/** 
		����һ��ͨ��������������Ķ���
		@param ptr ָ��������ٶ����ָ��
		*/
		virtual void destroyInstance(T*) = 0;
	};
}


#endif