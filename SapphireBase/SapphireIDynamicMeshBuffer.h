#ifndef _SAPPHIRE_I_DYNAMIC_MESH_BUFFER_
#define _SAPPHIRE_I_DYNAMIC_MESH_BUFFER_

#include "SapphireIMeshBuffer.h"
#include "SapphireIVertexBuffer.h"
#include "SapphireIIndexBuffer.h"


namespace Sapphire
{
	/** һ����̬���񻺳��� */
	class IDynamicMeshBuffer : public IMeshBuffer
	{
	public:
		virtual IVertexBuffer &getVertexBuffer() const = 0;
		virtual IIndexBuffer &getIndexBuffer() const = 0;

		virtual void setVertexBuffer(IVertexBuffer *vertexBuffer) = 0;
		virtual void setIndexBuffer(IIndexBuffer *indexBuffer) = 0;

		//! ��ȡ������񻺳����Ĳ���
		/** \return ����������Ĳ��� */
		virtual SMaterial& getMaterial() = 0;

		//! ��ȡ������񻺳����Ĳ���
		/** \return �������Ĳ��� */
		virtual const SMaterial& getMaterial() const = 0;

		//! ��ȡ������񻺳�����AABB����
		/** \return ������񻺳�����AABB���� */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! ����AABB����
		/** \param box �û�����������������AABB����
		  */
		virtual void setBoundingBox(const AxisAlignedBox& box) = 0;

		//! ���¼�����ײ���ӣ��������ı�ʱ����
		virtual void recalculateBoundingBox() = 0;

		//! ���Ӷ�������е���ǰ������
		/** ֻ�Լ��ݵĶ�������
		\param vertices ���������ָ��
		\param numVertices ���������Ԫ������
		\param indices �������������ָ��
		\param numIndices ����������*/
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices)
		{

		}

		//! ���һ�����񻺳�������ǰ������
		/** ֻ�Լ��ݵĶ�������
		\param other Ҫ��ӵ����񻺳���*/
		virtual void append(const IMeshBuffer* const other)
		{

		}

		// -------------------Ҫ�Ƴ� -------------------  //

		//! ��ȡ��ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const
		{
			return getVertexBuffer().getHardwareMappingHint();
		}

		//! ��ȡ��ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const
		{
			return getIndexBuffer().getHardwareMappingHint();
		}

		//! ����������Ӳ��ӳ����ʾ
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				getVertexBuffer().setHardwareMappingHint(NewMappingHint);
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				getIndexBuffer().setHardwareMappingHint(NewMappingHint);
		}

		//! ����ı��־������Ӳ��������
		virtual void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				getVertexBuffer().setDirty();
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				getIndexBuffer().setDirty();
		}

		virtual UINT32 getChangedID_Vertex() const
		{
			return getVertexBuffer().getChangedID();
		}

		virtual UINT32 getChangedID_Index() const
		{
			return getIndexBuffer().getChangedID();
		}

		// ------------------- �ɽӿ� -------------------  //

		//! ��ȡ�������ݵĵ����ͣ������������񻺳�����
		/** \return �����������������*/
		virtual E_VERTEX_TYPE getVertexType() const
		{
			return getVertexBuffer().getType();
		}

		//! ��ȡ�������ݣ����������һ����������
		/** �������Ϳ���getVertexType()�õ�
		\return ���������ָ�� */
		virtual const void* getVertices() const
		{
			return getVertexBuffer().getData();
		}

		//! ��ȡ�������ݣ����������һ����������
		/** �������Ϳ���getVertexType()�õ�
		\return ���������ָ�� */
		virtual void* getVertices()
		{
			return getVertexBuffer().getData();
		}

		//! ��ȡ���񻺳����ж��������
		/** \return ���񻺳����ж�������� */
		virtual UINT32 getVertexCount() const
		{
			return getVertexBuffer().size();
		}

		//! ��ȡ������񻺳������������ݵ�����
		/** \return �������е��������� */
		virtual E_INDEX_TYPE getIndexType() const
		{
			return getIndexBuffer().getType();
		}

		//! ȡ����������
		/** \return ���������ָ��*/
		virtual const UINT16* getIndices() const
		{
			return (UINT16*)getIndexBuffer().getData();
		}

		//! ȡ����������
		/** \return ���������ָ��*/
		virtual UINT16* getIndices()
		{
			return (UINT16*)getIndexBuffer().getData();
		}

		//! ��ȡ���񻺳���������������
		/** \return Number of indices in this buffer. */
		virtual UINT32 getIndexCount() const
		{
			return getIndexBuffer().size();
		}

		//! ���ض���i��λ��
		virtual const Vector3& getPosition(UINT32 i) const
		{
			return getVertexBuffer()[i].Pos;
		}

		//! ���ض���i��λ��
		virtual Vector3& getPosition(UINT32 i)
		{
			return getVertexBuffer()[i].Pos;
		}

		//! ���ض���i����������
		virtual const Vector2& getTCoords(UINT32 i) const
		{
			return getVertexBuffer()[i].TCoords;
		}

		//! ���ض���i����������
		virtual Vector2& getTCoords(UINT32 i)
		{
			return getVertexBuffer()[i].TCoords;
		}

		//! ���ض���i�ķ���
		virtual const Vector3& getNormal(UINT32 i) const
		{
			return getVertexBuffer()[i].Normal;
		}

		//! ���ض���i�ķ���
		virtual Vector3& getNormal(UINT32 i)
		{
			return getVertexBuffer()[i].Normal;
		}
	};
}

#endif