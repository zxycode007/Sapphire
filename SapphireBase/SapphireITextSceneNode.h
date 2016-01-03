#ifndef _SAPPHIRE_I_TEXT_SCENE_NODE_
#define _SAPPHIRE_I_TEXT_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! һ������ά�ռ�һ��λ����ʾ2d�ı��ĳ����ڵ�
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