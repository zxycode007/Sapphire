#ifndef _SAPPHIRE_IMETATRIANGLE_SELECTOR_
#define _SAPPHIRE_IMETATRIANGLE_SELECTOR_

#include "SapphireITriangleSelector.h"

namespace Sapphire
{
	//! ʹ���������ѡ������Ϊһ������ѡ���������Ľӿ�
	/** 
	��ֻ������һ������������ѡ�����ṩһ����ͬ��������ѡ�����ӿڼ��ϡ�
	��������ײ���Կ���һ���ڲ�ͬ��triangle soup�н��в���
	*/
	class IMetaTriangleSelector : public ITriangleSelector
	{
	public:

		//! ���һ��������ѡ������������ѡ�����ļ���
		/** \param toAdd: Ҫ��ӵ��б��������ѡ������ָ��*/
		virtual void addTriangleSelector(ITriangleSelector* toAdd) = 0;

		//! ��һ���������Ƴ�һ���ض���������ѡ����
		/** \param toRemove: Ҫ���б����Ƴ���������ѡ������ָ��
		\return ����ɹ�����true*/
		virtual bool removeTriangleSelector(ITriangleSelector* toRemove) = 0;

		//! �Ӽ����Ƴ����е�������ѡ����
		virtual void removeAllTriangleSelectors() = 0;
	};
}

#endif