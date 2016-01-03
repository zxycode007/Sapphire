#ifndef _SAPPHIRE_I_SHADOW_VOLUME_SCENE_NODE_
#define _SAPPHIRE_I_SHADOW_VOLUME_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class IMesh;

	//! ��Ⱦ��ģ�建������һ����Ӱ�ݻ������ڵ�
	class IShadowVolumeSceneNode : public ISceneNode
	{
	public:

		 
		IShadowVolumeSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id)
			: ISceneNode(parent, mgr, id) {}

		//! ������Ӱ�ݻ���Ҫ����������
		/** Ҫ�Ż���Ӱ��Ⱦ����һ���򵥵������ʾ��Ӱ
		*/
		virtual void setShadowMesh(const IMesh* mesh) = 0;

		//! �Ե�ǰ��Դλ�ø�����Ӱ�ݻ�
		virtual void updateShadowVolumes() = 0;
	};
}

#endif