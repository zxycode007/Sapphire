#ifndef _SAPPHIRE_IMESH_SCENE_NODE_
#define _SAPPHIRE_IMESH_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	class IShadowVolumeSceneNode;
	class IMesh;

	//! ��ʾΪһ����̬����ĳ����ڵ�
	class IMeshSceneNode : public ISceneNode
	{
	public:

		//! 
		/** ʹ��setMesh����������Ҫ��ʾ������
		*/
		IMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1, 1, 1))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! ����һ��Ҫ��ʾ��������
		/** \param mesh Ҫ��ʾ������ */
		virtual void setMesh(IMesh* mesh) = 0;

		//! ��ʾ��ȡ��ǰ��ʾ����Ķ���
		/** \return ������ڵ���ʾ�����ָ��*/
		virtual IMesh* getMesh(void) = 0;

		//! ������Ӱ�ݻ������ڵ���Ϊ����ڵ���ӽڵ�
		/** 
		�����Ӱ������ZPass��Zfail������Ⱦ��ZPassҪ�Կ�һ�㣬���ڴ�����Ӱ�ݻ�������׵㣬
		������������ڵ��������Ӱ�ݻ��������ֿ�������ª�Ķ�������Щ������ZFail��������
		������
		\param shadowMesh: ��ѡ��  ������Ӱ�ݻ��Զ�������
		\param id: ��Ӱ�����ڵ��ID��  ���id�����Ժ��ʶ����ڵ�
		\param zfailmethod: �������Ϊtrue�������Ӱ��ʹ��zfail����������ʹ��zpass
		\param infinity: ��Ӱ�ݻ��㷨���������Ӱ�ݻ���ֵ(����zfail ��Ӱ�ݻ�������ֻ֧��finite ��Ӱ
		���������zfar������������Ӱ��back cap����������infinity����)
		\return ��������Ӱ�ڵ��ָ�롣*/
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
			SINT32 id = -1, bool zfailmethod = true, Real infinity = 1000.0f) = 0;

		//! ������������ڵ㲻�����������Ĳ����Ƿ���ֻ��ģʽʹ������
		/** ����������ܻ�ı�һ������Ĳ��ʣ�������������������������ڵ㷢���ı䡣
		\param readonly ������������Ƿ�Ӧ��ֻ�� */
		virtual void setReadOnlyMaterials(bool readonly) = 0;

		//! ��ⳡ���ڵ㲻����������ʣ����Ƿ���ֻ��ģʽʹ�����ǡ�
		/** �����־����ͨ��setReadOnlyMaterials()����
		\return ��������Ƿ�ֻ�� */
		virtual bool isReadOnlyMaterials() const = 0;
	};
}

#endif