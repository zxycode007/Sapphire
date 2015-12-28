#ifndef _SAPPHIRE_I_LIGHT_SCENE_NODE_
#define _SAPPHIRE_I_LIGHT_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class IVolumeLightSceneNode : public ISceneNode
	{
	public:

		 
		IVolumeLightSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position,
			const Vector3& rotation,
			const Vector3& scale)
			: ISceneNode(parent, mgr, id, position, rotation, scale) {};

		//! 返回这个场景节点的类型
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_VOLUME_LIGHT; }

		//! 设置在U轴的分段数
		virtual void setSubDivideU(const UINT32 inU) = 0;

		//! 设置在V轴的分段数
		virtual void setSubDivideV(const UINT32 inV) = 0;

		//! 返回在U轴的分段数
		virtual UINT32 getSubDivideU() const = 0;

		//! 返回在V轴的分段数
		virtual UINT32 getSubDivideV() const = 0;

		//! 设置这个光源的基颜色
		virtual void setFootColor(const ColourValue inColor) = 0;

		//! 设置这个光源的尾部颜色
		virtual void setTailColor(const ColourValue inColor) = 0;

		//! 返回这个光源的基颜色
		virtual ColourValue getFootColor() const = 0;

		//! 返回这个光源的尾部颜色
		virtual ColourValue getTailColor() const = 0;
	};
}

#endif