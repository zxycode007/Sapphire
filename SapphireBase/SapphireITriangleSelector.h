#ifndef _SAPPHIRE_ITRIANGLE_SELECTOR_
#define _SAPPHIRE_ITRIANGLE_SELECTOR_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireTriangle3D.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireLine3D.h"


namespace Sapphire
{
	class ISceneNode;

	//! �������ر����Ե������εĽӿ�
	/** ÿ��ISceneNode��������һ��������ѡ������������ISceneNode::getTriangleScelector()��ISceneManager::createTriangleSelector.
	��á���������ײ��⣺���������ָ������һ����ײ���ܷ���������1��1��1���ͣ�10��10��10���������ͨ��ITriangleSelector��ȡ����
	�ĳ����ڵ�����������ڵ����������Σ����Ҽ����Щ�������Ƿ�ÿһ����������ײ
	 */
	class ITriangleSelector : public SceneAlloc, public virtual IReferenceCounter
	{
		//! ��ȡ���selector���п��õ�����������
		virtual SINT32 getTriangleCount() const = 0;

		//! ��ȡһ�������ڵ��������
		/**
		������һ�������ڵ������selector����ϵ������������. 
		����ж���һ�������ڵ��й�ϵ������IMetaTriangleSelector��
		������ú�������ö�η������ж����
		\param triangles ������������������
		\param arraySize �����С
		\param outTriangleCount: д��������������������
		\param transform �����Ƿ���֮ǰ�任��Щ�����εľ���ָ��
	    ���������������������ε�һ����Բ�οռ��С�������ָ����NULL��û�б任*/
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const Matrix4* transform = 0) const = 0;

		//! ��ȡһ����ָ��һ��AABB���ӵĽڵ���ϵ������������
		/**
		������һ�������ڵ������selector����ϵ������������. 
		����ж���һ�������ڵ��й�ϵ������IMetaTriangleSelector��
		������ú�������ö�η������ж����.
		������������ٷ���������ཻ�������Σ�����Ҳ���ܷ���������������
		
		\param triangles ������������������
		\param arraySize �����С
		\param outTriangle д��������������������
		\param box ֻ������AABB�������������д������
		\param transform �����Ƿ���֮ǰ�任��Щ�����εľ���ָ��
	    ���������������������ε�һ����Բ�οռ��С�������ָ����NULL��û�б任 */
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const AxisAlignedBox& box,
			const Matrix4* transform = 0) const = 0;

		//! ��ȡһ����ָ����һ��3d�߶ε��й����Ľڵ������������ 
		/**
		������һ�������ڵ������selector����ϵ������������. 
		����ж���һ�������ڵ��й�ϵ������IMetaTriangleSelector��
		������ú�������ö�η������ж����.
		��ע�⣺������������ٷ���������ཻ�������Σ�����Ҳ���ܷ���������������

		��ע�⣺û�Ż���������ѡ����Ҳ��������Щ��3d�߶�û����ϵ��������
		\param triangles ������������������
		\param arraySize �����С
		\param outTriangle д��������������������
		\param line ֻ�������3d�߶�����ϵ�������λ�д�뵽�������
		\param transform �����Ƿ���֮ǰ�任��Щ�����εľ���ָ��
	    ���������������������ε�һ����Բ�οռ��С�������ָ����NULL��û�б任 */
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3d<Real>& line,
			const Matrix4* transform = 0) const = 0;

		//! ��ȡ��һ��������������صĳ����ڵ�
		/**
		�������ҵ���һ�������ڵ㣨�����м�������һ��ָ������������� 

		\param triangleIndex: Ҫ���ҵ�����������ڵ���ص����������� 
		\return ��ָ����������صĳ����ڵ�
		*/
		virtual ISceneNode* getSceneNodeForTriangle(UINT32 triangleIndex) const = 0;

		//! ��ȡ������ѡ����������
		/** ֻ����MetaTriangleSelector, ��������1
		*/
		virtual UINT32 getSelectorCount() const = 0;

		//! ��ȡ���������ͻ���getSelectorCount��������ѡ����
		/** ֻ����MetaTriangleSelector,�����ķ���'this' �� 0
		*/
		virtual ITriangleSelector* getSelector(UINT32 index) = 0;

		//! ��ȡ���������ͻ���getSelectorCount��������ѡ����������
		/** ֻ����MetaTriangleSelector,�����ķ���'this' �� 0
		*/
		virtual const ITriangleSelector* getSelector(UINT32 index) const = 0;
	};
}



#endif
