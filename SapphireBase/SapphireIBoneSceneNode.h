#ifndef _SAPPHIRE_I_BONE_SCENE_NODE_
#define _SAPPHIRE_I_BONE_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	//! ��ͬ��������ģʽ��ö��
	enum E_BONE_ANIMATION_MODE
	{
		//! �������ͨ����������������ڵ㲻������
		EBAM_AUTOMATIC = 0,

		//! �������ͨ��Ƥ����� ������ĸ��ڵ㲻�������ô������Ȼ���� 
		EBAM_ANIMATED,

		//! �����������ͨ��Ƥ������
		EBAM_UNANIMATED,

		//! ��������ģʽ����
		EBAM_COUNT

	};

	enum E_BONE_SKINNING_SPACE
	{
		//! ������Ƥ����־
		EBSS_LOCAL = 0,

		//! ȫ����Ƥ
		EBSS_GLOBAL,

		EBSS_COUNT
	};

	//! ��������ģʽ��
	const c8* const BoneAnimationModeNames[] =
	{
		"automatic",
		"animated",
		"unanimated",
		0,
	};


	//! ���ڹ��������Ĺ����ӿ�
	/** ����ISkinnedMesh �� IAnimatedMeshSceneNode. */
	class IBoneSceneNode : public ISceneNode
	{
	public:

		IBoneSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id = -1) :
			ISceneNode(parent, mgr, id), positionHint(-1), scaleHint(-1), rotationHint(-1) { }

		//! ��ȡ����������
		virtual UINT32 getBoneIndex() const = 0;

		//! ���ù�������ģʽ
		/** \return ����ɹ�����true ������ʹ�ã�*/
		virtual bool setAnimationMode(E_BONE_ANIMATION_MODE mode) = 0;

		//! ��ȡ�������ǰ�Ķ���ģʽ
		virtual E_BONE_ANIMATION_MODE getAnimationMode() const = 0;

		//! ��ȡ����ڵ��AABB����
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! ������������ڵ����ر任
		//virtual Matrix getRelativeTransformation() const = 0;

		//! �����ķ���
		virtual void OnAnimate(UINT32 timeMs) = 0;

		//! ��Ⱦ����
		/** ��Ϊ���ɼ��Ĺ���ʲô�������� */
		virtual void render() { }

		//! ������Ƥ�ռ�
		//��������ر任���ʹ��
		virtual void setSkinningSpace(E_BONE_SKINNING_SPACE space) = 0;

		//! ��ȡ��Ƥ�ռ�
		//! ��ȡ��������ر任���ʹ��
		virtual E_BONE_SKINNING_SPACE getSkinningSpace() const = 0;

		//! ͨ����������ڸ��ڵ��λ�ø��¾���λ��
		virtual void updateAbsolutePositionOfAllChildren() = 0;

		SINT32 positionHint;
		SINT32 scaleHint;
		SINT32 rotationHint;
	};
}

#endif