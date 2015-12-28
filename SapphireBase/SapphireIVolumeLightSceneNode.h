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

		//! ������������ڵ������
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_VOLUME_LIGHT; }

		//! ������U��ķֶ���
		virtual void setSubDivideU(const UINT32 inU) = 0;

		//! ������V��ķֶ���
		virtual void setSubDivideV(const UINT32 inV) = 0;

		//! ������U��ķֶ���
		virtual UINT32 getSubDivideU() const = 0;

		//! ������V��ķֶ���
		virtual UINT32 getSubDivideV() const = 0;

		//! ���������Դ�Ļ���ɫ
		virtual void setFootColor(const ColourValue inColor) = 0;

		//! ���������Դ��β����ɫ
		virtual void setTailColor(const ColourValue inColor) = 0;

		//! ���������Դ�Ļ���ɫ
		virtual ColourValue getFootColor() const = 0;

		//! ���������Դ��β����ɫ
		virtual ColourValue getTailColor() const = 0;
	};
}

#endif