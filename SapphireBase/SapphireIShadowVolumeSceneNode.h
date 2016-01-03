#ifndef _SAPPHIRE_I_SHADOW_VOLUME_SCENE_NODE_
#define _SAPPHIRE_I_SHADOW_VOLUME_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class IMesh;

	//! 渲染到模板缓冲区的一个阴影容积场景节点
	class IShadowVolumeSceneNode : public ISceneNode
	{
	public:

		 
		IShadowVolumeSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id)
			: ISceneNode(parent, mgr, id) {}

		//! 设置阴影容积将要创建的网格
		/** 要优化阴影渲染，用一个简单的网格表示阴影
		*/
		virtual void setShadowMesh(const IMesh* mesh) = 0;

		//! 对当前光源位置更新阴影容积
		virtual void updateShadowVolumes() = 0;
	};
}

#endif