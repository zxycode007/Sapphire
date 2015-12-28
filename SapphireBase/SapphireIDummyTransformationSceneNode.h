#ifndef _SAPPHIRE_I_DUMMY_TRANSFORMATION_SCENE_NODE_
#define _SAPPHIRE_I_DUMMY_TRANSFORMATION_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! Ϊ���Ӹ��ӱ任������ͼ�����������ڵ�
	/** ��������ڵ㲻�ᱻ��Ⱦ�����Ҳ�����Ҫset/getPosition(),set/getRotation �� set/getScale��
	��ֻ��һ����һ����ر任����ļ򵥵ĳ����ڵ㣬
	 ��������ڵ��������磺IAnimatedMeshSceneNode�ڲ��Ź�������ʱģ��joint�����ڵ�
	*/
	class IDummyTransformationSceneNode : public ISceneNode
	{
	public:

		//! ������
		IDummyTransformationSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id)
			: ISceneNode(parent, mgr, id) {}

		//! ����һ���Ե�ǰ��ؾ��������
		/** ����һ��������������ڵ����ڴ������ţ��任����ת */
		virtual Matrix4& getRelativeTransformationMatrix() = 0;
	};
}

#endif