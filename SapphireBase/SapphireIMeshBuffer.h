#ifndef _SAPPHIRE_IMESH_BUFFER_
#define _SAPPHIRE_IMESH_BUFFER_

#include "SapphireSVertexIndex.h"
#include "SapphireEHardwareBufferFlags.h"
#include "SapphirePrerequisites.h"
#include "SapphireSMaterial.h"
#include "SapphireVector3.h"
#include "SapphireVector2.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	//! �����õ����ʵ�����
	/** һ��IMesh��һ�����ֵ���һ���ÿ���涼��ͬ���Ĳ���
	һ��IMesh���߼��鲻��ŵ������ķ��뻺�����У������ܹ�����������Ķ������ֱ��뱻�ŵ�
	���������񻺳����С�
	��Щ���񻺳�����ʵ�������˻������ܱ���Ķ�����������������£��߼����������������⣬��������Ӧ��Ϊ�ϴ�GPU���Ż���
	������gfx�������͡�ͨ��������ÿ��������1000~10000������
	SMeshBuffer��һ���򵥵�MeshBuffer��ʵ�֣���֧��65535������

	�����񻺳��������ڻ��ƣ����������������������кܸ߼�����ĳ����grab������
	�������Ҫ��̬�������񻺳��������ݵ�һ����Ƶ�������ҽ��ڶ������������ʱ�������ڵ�ǰ�����ʹ��
	*/
	class IMeshBuffer //: public virtual IReferenceCounted
	{
	public:

		//! ��ȡ������񻺳����Ĳ���
		/** \return Material of this buffer. */
		virtual  SMaterial& getMaterial() = 0;

		//! ��ȡ������񻺳����Ĳ���
		/** \return �������������ʹ�õĲ��� */
		virtual const  SMaterial& getMaterial() const = 0;

		//! ��ȡ���Դ���ڶ��㻺�����ڶ������ݵ�����
		/** \return ��������� */
		virtual  E_VERTEX_TYPE getVertexType() const = 0;

		//! ��ȡ���ʶ������ݣ����������һ����������飨ֻ����
		/** �������Ϳ�����getVertexType()ȡ��
		\return ָ�򶥵������ָ�� */
		virtual const void* getVertices() const = 0;

		//! ��ȡ���ʶ������ݣ����������һ�����������
		/** �������Ϳ�����getVertexType()ȡ��
		\returnָ�򶥵������ָ�� */
		virtual void* getVertices() = 0;

		//! ��ȡ�����񻺳����ﶥ�������
		/** \return ���񻺳����ﶥ������� */
		virtual UINT32 getVertexCount() const = 0;

		//! ��ȡ���������񻺳�������������
		/** \return ������������������� */
		virtual E_INDEX_TYPE getIndexType() const = 0;

		//! ��ȡ��ȡ�������ݣ�ֻ����
		/** \return �������ݵ�ָ�� */
		virtual const UINT16* getIndices() const = 0;

		//! ��ȡ��ȡ��������
		/** \return �������ݵ�ָ�� */
		virtual UINT16* getIndices() = 0;

		//! ��ȡ��������񻺳���������������
		/** \return ��������񻺳��������������� */
		virtual UINT32 getIndexCount() const = 0;

		//! ��ȡ������񻺳�����AABB����
		/** \return ������񻺳�����AABB���� */
		virtual const  AxisAlignedBox& getBoundingBox() const = 0;

		//! ����AABB����
		/** \param box �û�Ϊ������񻺳��������AABB����
		for this buffer. */
		virtual void setBoundingBox(const  AxisAlignedBox& box) = 0;

		//! ���¼�����ײ���ӣ��������ı�Ļ�Ӧ�õ���
		virtual void recalculateBoundingBox() = 0;

		//! ���ض���I��λ����Ϣ��ֻ����
		virtual const Vector3& getPosition(UINT32 i) const = 0;

		//! ���ض���I��λ����Ϣ
		virtual Vector3& getPosition(UINT32 i) = 0;

		//! ���ض���I�ķ�����Ϣ��ֻ����
		virtual const Vector3& getNormal(UINT32 i) const = 0;

		//! ���ض���I�ķ�����Ϣ
		virtual Vector3& getNormal(UINT32 i) = 0;

		//! ���ض���I���������ֻ꣨����
		virtual const Vector2& getTCoords(UINT32 i) const = 0;

		//!���ض���I�ķ�����Ϣ
		virtual Vector2& getTCoords(UINT32 i) = 0;

		//! ��Ӷ����������������ǰ������
		/**ֻ�����ڼ��ݵĶ�������
		\param vertices ��������ָ��
		\param numVertices ��������
		\param indices ����ָ��
		\param numIndices �������� */
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices) = 0;

		//! ������񻺳�������ǰ������
		/** ֻ�����ڼ��ݵĶ�������
		\param other Ҫ��ӵĻ����� */
		virtual void append(const IMeshBuffer* const other) = 0;

		//! ��õ�ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const = 0;

		//! ��õ�ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const = 0;

		//! Ϊ��������Ӳ��ӳ����ʾ
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//! ���񻺳�����׼�ı䣬����װ��Ӳ��������
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//!���ڸı��ȷ�ϻ�ȡ��ǰʹ��ID  
		/** �ⲻӦ�������κ��ⲿ��Ƶ����*/
		virtual UINT32 getChangedID_Vertex() const = 0;

		//! ���ڸı��ȷ�ϻ�ȡ��ǰʹ��ID
		/**�ⲻӦ�������κ��ⲿ��Ƶ���� */
		virtual UINT32 getChangedID_Index() const = 0;
	};
}

#endif