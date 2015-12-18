#ifndef _SAPPHIRE_IMETATRIANGLE_SELECTOR_
#define _SAPPHIRE_IMETATRIANGLE_SELECTOR_

#include "SapphireITriangleSelector.h"

namespace Sapphire
{
	//! 使多个三角形选择器作为一个大型选择器工作的接口
	/** 
	这只不过是一个或多个三角形选择器提供一个共同的三角形选择器接口集合。
	这样，碰撞测试可以一次在不同的triangle soup中进行测试
	*/
	class IMetaTriangleSelector : public ITriangleSelector
	{
	public:

		//! 添加一个三角形选择器到三角形选择器的集合
		/** \param toAdd: 要添加到列表的三角形选择器的指针*/
		virtual void addTriangleSelector(ITriangleSelector* toAdd) = 0;

		//! 从一个集合中移除一个特定的三角形选择器
		/** \param toRemove: 要从列表中移除的三角形选择器的指针
		\return 如果成功返回true*/
		virtual bool removeTriangleSelector(ITriangleSelector* toRemove) = 0;

		//! 从集合移除所有的三角形选择器
		virtual void removeAllTriangleSelectors() = 0;
	};
}

#endif