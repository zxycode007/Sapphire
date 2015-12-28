#ifndef _SAPPHIRE_I_BILLBOARD_SCENE_NODE_
#define _SAPPHIRE_I_BILLBOARD_SCENE_NODE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! һ��billboard�����ڵ�
	/** һ��billboard����3d sprite��һ��2dԪ�أ���һֱ�����������ͨ��������ʾ��ը�����棬�ƹ⣬���Ӻ����ƶ���
	*/
	class IBillboardSceneNode : public ISceneNode
	{
	public:

		//!  
		IBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: ISceneNode(parent, mgr, id, position) {}

		//! ����billboard�Ĵ�С������
		virtual void setSize(const dimension2d<Real>& size) = 0;

		//! ͨ�������������billboard�Ĵ�С
		/** \param[in] height billboard�ĸ�
		\param[in] bottomEdgeWidth billboard�ĵױ߿��
		\param[in] topEdgeWidth billboard�Ķ��߿��
		*/
		virtual void setSize(Real height, Real bottomEdgeWidth, Real topEdgeWidth) = 0;

		//! �������billboard�Ĵ�С
		/** �⽫����billboard�Ŀ�Ⱥ͵ױߣ���getWidth()�������ص׺Ͷ���
		\return billboard�Ĵ�С
		*/
		virtual const dimension2d<Real>& getSize() const = 0;

		//! ��ȡ���billboard�Ĵ�С
		/** \param[out] height billboard�ĸ�
		\param[out] bottomEdgeWidth billboard�ĵױ߿��
		\param[out] topEdgeWidth billboard�Ķ��߿��
		*/
		virtual void getSize(Real& height, Real& bottomEdgeWidth, Real& topEdgeWidth) const = 0;

		//! �������billboard�����ж������ɫ
		/** \param[in] overallColor ��ɫ���� */
		virtual void setColor(const ColourValue& overallColor) = 0;

		//! �������billboard�ײ��Ͷ����������ɫ
		/** \param[in] topColor �����������ɫ
		\param[in] bottomColor �ײ��������ɫ */
		virtual void setColor(const ColourValue& topColor,
			const ColourValue& bottomColor) = 0;

		//! ��ȡ���billboard�ĵײ��Ͷ���������ɫ
		/** \param[out] topColor ���涥��������ɫ
		\param[out] bottomColor ����ײ��������ɫ*/
		virtual void getColor(ColourValue& topColor,
			ColourValue& bottomColor) const = 0;
	};
}

#endif