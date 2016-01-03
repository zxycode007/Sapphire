#ifndef _SAPPHIRE_I_TEXT_SCENE_NODE_
#define _SAPPHIRE_I_TEXT_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! 一个在三维空间一个位置显示2d文本的场景节点
	class ITextSceneNode : public ISceneNode
	{
	public:

		 
		ITextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: ISceneNode(parent, mgr, id, position) {}

	 
		virtual void setText(const wchar_t* text) = 0;

		 
		virtual void setTextColor(ColourValue color) = 0;
	};
}

#endif