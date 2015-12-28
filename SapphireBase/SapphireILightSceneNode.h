#ifndef _SAPPHIRE_I_LIGHT_SCENE_NODE_
#define _SAPPHIRE_I_LIGHT_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireSLight.h"

namespace Sapphire
{
	//! 作为一个动态光源的场景节点
	/** 你可以通过它的可见选项切换光源开关。 它能够通过普通的场景节点animatior来互动。
	如果光源类型是平行光或者点光源，光源方向通过场景节点的旋转来定义的(假定(0,0,1)为光源的本地方向)
	*/
	class ILightSceneNode : public ISceneNode
	{
	public:

		 
		ILightSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: ISceneNode(parent, mgr, id, position) {}

		//! 设置与这个ILightSceneNode相关的光源属性
		/** \param light 新的光源数据 */
		virtual void setLightData(const SLight& light) = 0;

		//! 获取与这个ILightSceneNode相关的光源属性
		/** \return 光源的数据 */
		virtual const SLight& getLightData() const = 0;

		//! 获取与这个ILightSceneNode相关的光源属性
		/** \return 光源的数据. */
		virtual SLight& getLightData() = 0;

		//! 设置这个节点是否可见
		/** 设置为true，所有的子节点将不可见
		\param isVisible 如果节点不可见 */
		virtual void setVisible(bool isVisible) = 0;

		//! 设置光源的影响半径
		/** 在这个半径之外，光源不对半径外的几何体进行光照并且不会投射阴影。
		设置半径将影响衰减，设置它到(0,1/radius,0)。如果你想要重写，在设置半径之后设置衰减
		\param radius 新半径 */
		virtual void setRadius(Real radius) = 0;

		//! 获取光源的影响半径
		/** \return 当前半径 */
		virtual Real getRadius() const = 0;

		//! 设置光源的类型
		/** \param type 新的类型 */
		virtual void setLightType(E_LIGHT_TYPE type) = 0;

		//! 获取光源的类型
		/** \return 当光源类型 */
		virtual E_LIGHT_TYPE getLightType() const = 0;

		//! 设置这个光源是否投射阴影
		/** 打开这个标志并不会自动投射阴影，这些网格仍然需要添加阴影场景节点。
		打开和关闭直接光源对阴影透视有性能的因素
		\param shadow True 如果为true，光源可以投射阴影 */
		virtual void enableCastShadow(bool shadow = true) = 0;

		//! 检测光源阴影透视是否打开
		/** \return 如果为true，此光源投射阴影 */
		virtual bool getCastShadow() const = 0;
	};
}

#endif