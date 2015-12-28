#ifndef _SAPPHIRE_I_BILLBOARD_SCENE_NODE_
#define _SAPPHIRE_I_BILLBOARD_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! 一个billboard场景节点
	/** 一个billboard类似3d sprite：一个2d元素，它一直面向着相机，通常用来表示爆炸，火焰，灯光，粒子和类似东西
	*/
	class IBillboardSceneNode : public ISceneNode
	{
	public:

		//!  
		IBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: ISceneNode(parent, mgr, id, position) {}

		//! 设置billboard的大小，矩形
		virtual void setSize(const dimension2d<Real>& size) = 0;

		//! 通过长宽设置这个billboard的大小
		/** \param[in] height billboard的高
		\param[in] bottomEdgeWidth billboard的底边宽度
		\param[in] topEdgeWidth billboard的顶边宽度
		*/
		virtual void setSize(Real height, Real bottomEdgeWidth, Real topEdgeWidth) = 0;

		//! 返回这个billboard的大小
		/** 这将返回billboard的宽度和底边，用getWidth()单独返回底和定边
		\return billboard的大小
		*/
		virtual const dimension2d<Real>& getSize() const = 0;

		//! 获取这个billboard的大小
		/** \param[out] height billboard的高
		\param[out] bottomEdgeWidth billboard的底边宽度
		\param[out] topEdgeWidth billboard的顶边宽度
		*/
		virtual void getSize(Real& height, Real& bottomEdgeWidth, Real& topEdgeWidth) const = 0;

		//! 设置这边billboard的所有顶点的颜色
		/** \param[in] overallColor 颜色设置 */
		virtual void setColor(const ColourValue& overallColor) = 0;

		//! 设置这个billboard底部和顶部顶点的颜色
		/** \param[in] topColor 顶部顶点的颜色
		\param[in] bottomColor 底部顶点的颜色 */
		virtual void setColor(const ColourValue& topColor,
			const ColourValue& bottomColor) = 0;

		//! 获取这个billboard的底部和顶部顶点颜色
		/** \param[out] topColor 保存顶部顶点颜色
		\param[out] bottomColor 保存底部顶点的颜色*/
		virtual void getColor(ColourValue& topColor,
			ColourValue& bottomColor) const = 0;
	};
}

#endif