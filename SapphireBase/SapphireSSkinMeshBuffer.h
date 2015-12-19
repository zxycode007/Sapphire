#ifndef _SAPPHIRE_SSKIN_MESH_BUFFER_
#define _SAPPHIRE_SSKIN_MESH_BUFFER_

#include "SapphirePrerequisites.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	//! һ����������ʱ�̶�S3DVertex2TCoords, S3DVertex �� S3DVertexTangents֮�����ѡ�񻺳�����񻺳���
	struct SSkinMeshBuffer : public IMeshBuffer
	{
		//! Ĭ�ϵĹ�����
		SSkinMeshBuffer(E_VERTEX_TYPE vt = EVT_STANDARD) :
			ChangedID_Vertex(1), ChangedID_Index(1), VertexType(vt),
			MappingHint_Vertex(EHM_NEVER), MappingHint_Index(EHM_NEVER),
			BoundingBoxNeedsRecalculated(true)
		{
//#ifdef _DEBUG
//			setDebugName("SSkinMeshBuffer");
//#endif
		}

		//! ��ȡ����������Ĳ���
		virtual const SMaterial& getMaterial() const
		{
			return Material;
		}

		//! ��ȡ����������Ĳ���
		virtual SMaterial& getMaterial()
		{
			return Material;
		}

		//! ��ȡ���������ı�׼����
		virtual S3DVertex *getVertex(UINT32 index)
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return (S3DVertex*)&Vertices_2TCoords[index];
			case EVT_TANGENTS:
				return (S3DVertex*)&Vertices_Tangents[index];
			default:
				return &Vertices_Standard[index];
			}
		}

		//! ��ȡ���������ָ��
		virtual const void* getVertices() const
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords.cbegin()._Ptr;
			case EVT_TANGENTS:
				return Vertices_Tangents.cbegin()._Ptr;
			default:
				return Vertices_Standard.cbegin()._Ptr;
			}
		}

		//! ��ȡ���������ָ��
		virtual void* getVertices()
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords.begin()._Ptr;
			case EVT_TANGENTS:
				return Vertices_Tangents.begin()._Ptr;
			default:
				return Vertices_Standard.begin()._Ptr;
			}
		}

		//! ��ȡ���������
		virtual UINT32 getVertexCount() const
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords.size();
			case EVT_TANGENTS:
				return Vertices_Tangents.size();
			default:
				return Vertices_Standard.size();
			}
		}

		//! ��ȡ������������񻺳����е�������������
		/** \return Index type of this buffer. */
		virtual E_INDEX_TYPE getIndexType() const
		{
			return EIT_16BIT;
		}

		//! ��ȡ���������ָ��
		virtual const UINT16* getIndices() const
		{   			
			return Indices.begin()._Ptr;
			 
		}

		//!  ��ȡ���������ָ��
		virtual UINT16* getIndices()
		{
			return Indices.begin()._Ptr;
		}

		//! ��ȡ���������С
		virtual UINT32 getIndexCount() const
		{
			return Indices.size();
		}

		//! ��ȡ��ײ����
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! ������ײ����
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}

		//! ���¼���AABB����
		virtual void recalculateBoundingBox()
		{
			if (!BoundingBoxNeedsRecalculated)
				return;

			BoundingBoxNeedsRecalculated = false;

			switch (VertexType)
			{
			case EVT_STANDARD:
			{
				if (Vertices_Standard.empty())
					BoundingBox.reset(Vector3(0, 0, 0));
				else
				{
					BoundingBox.reset(Vertices_Standard[0].Pos);
					for (UINT32 i = 1; i<Vertices_Standard.size(); ++i)
						BoundingBox.addInternalPoint(Vertices_Standard[i].Pos);
				}
				break;
			}
			case EVT_2TCOORDS:
			{
				if (Vertices_2TCoords.empty())
					BoundingBox.reset(Vector3(0, 0, 0));
				else
				{
					BoundingBox.reset(Vertices_2TCoords[0].Pos);
					for (UINT32 i = 1; i<Vertices_2TCoords.size(); ++i)
						BoundingBox.addInternalPoint(Vertices_2TCoords[i].Pos);
				}
				break;
			}
			case EVT_TANGENTS:
			{
				if (Vertices_Tangents.empty())
					BoundingBox.reset(Vector3(0, 0, 0));
				else
				{
					BoundingBox.reset(Vertices_Tangents[0].Pos);
					for (UINT32 i = 1; i<Vertices_Tangents.size(); ++i)
						BoundingBox.addInternalPoint(Vertices_Tangents[i].Pos);
				}
				break;
			}
			}
		}

		//! ��ȡ��������
		virtual E_VERTEX_TYPE getVertexType() const
		{
			return VertexType;
		}

		//! ��ȡ����������
		virtual void convertTo2TCoords()
		{
			if (VertexType == EVT_STANDARD)
			{
				for (UINT32 n = 0; n<Vertices_Standard.size(); ++n)
				{
					S3DVertex2TCoords Vertex;
					Vertex.Color = Vertices_Standard[n].Color;
					Vertex.Pos = Vertices_Standard[n].Pos;
					Vertex.Normal = Vertices_Standard[n].Normal;
					Vertex.TCoords = Vertices_Standard[n].TCoords;
					Vertices_2TCoords.push_back(Vertex);
				}
				Vertices_Standard.clear();
				VertexType = EVT_2TCOORDS;
			}
		}

		//! ת��Ϊ���߶�������
		virtual void convertToTangents()
		{
			if (VertexType == EVT_STANDARD)
			{
				for (UINT32 n = 0; n<Vertices_Standard.size(); ++n)
				{
					S3DVertexTangents Vertex;
					Vertex.Color = Vertices_Standard[n].Color;
					Vertex.Pos = Vertices_Standard[n].Pos;
					Vertex.Normal = Vertices_Standard[n].Normal;
					Vertex.TCoords = Vertices_Standard[n].TCoords;
					Vertices_Tangents.push_back(Vertex);
				}
				Vertices_Standard.clear();
				VertexType = EVT_TANGENTS;
			}
			else if (VertexType == EVT_2TCOORDS)
			{
				for (UINT32 n = 0; n<Vertices_2TCoords.size(); ++n)
				{
					S3DVertexTangents Vertex;
					Vertex.Color = Vertices_2TCoords[n].Color;
					Vertex.Pos = Vertices_2TCoords[n].Pos;
					Vertex.Normal = Vertices_2TCoords[n].Normal;
					Vertex.TCoords = Vertices_2TCoords[n].TCoords;
					Vertices_Tangents.push_back(Vertex);
				}
				Vertices_2TCoords.clear();
				VertexType = EVT_TANGENTS;
			}
		}

		//! ���ض���i��λ��
		virtual const Vector3& getPosition(UINT32 i) const
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].Pos;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].Pos;
			default:
				return Vertices_Standard[i].Pos;
			}
		}

		//! ���ض���i��λ��
		virtual Vector3& getPosition(UINT32 i)
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].Pos;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].Pos;
			default:
				return Vertices_Standard[i].Pos;
			}
		}

		//! ���ض���i�ķ���
		virtual const Vector3& getNormal(UINT32 i) const
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].Normal;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].Normal;
			default:
				return Vertices_Standard[i].Normal;
			}
		}

		//! ���ض���i�ķ���
		virtual Vector3& getNormal(UINT32 i)
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].Normal;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].Normal;
			default:
				return Vertices_Standard[i].Normal;
			}
		}

		//! ���ض���i����������
		virtual const Vector2& getTCoords(UINT32 i) const
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].TCoords;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].TCoords;
			default:
				return Vertices_Standard[i].TCoords;
			}
		}

		//!  ���ض���i����������
		virtual Vector2& getTCoords(UINT32 i)
		{
			switch (VertexType)
			{
			case EVT_2TCOORDS:
				return Vertices_2TCoords[i].TCoords;
			case EVT_TANGENTS:
				return Vertices_Tangents[i].TCoords;
			default:
				return Vertices_Standard[i].TCoords;
			}
		}

		//! ��Ӷ������������ǰ������
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices) {}

		//! ������񻺳�������ǰ������
		virtual void append(const IMeshBuffer* const other) {}

		//! ��ȡ���ڶ��㻺�����ĵ�ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const
		{
			return MappingHint_Vertex;
		}

		//! ��ȡ���ڶ��㻺�����ĵ�ǰӲ��ӳ����ʾ
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const
		{
			return MappingHint_Index;
		}

		//! ����������Ӳ��ӳ����ʾ
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX)
				MappingHint_Vertex = NewMappingHint;
			else if (Buffer == EBT_INDEX)
				MappingHint_Index = NewMappingHint;
			else if (Buffer == EBT_VERTEX_AND_INDEX)
			{
				MappingHint_Vertex = NewMappingHint;
				MappingHint_Index = NewMappingHint;
			}
		}

		//! �������ı��־������Ӳ��������
		virtual void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				++ChangedID_Vertex;
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				++ChangedID_Index;
		}

		virtual UINT32 getChangedID_Vertex() const { return ChangedID_Vertex; }

		virtual UINT32 getChangedID_Index() const { return ChangedID_Index; }

		//! AABB��Ҫ���¼��㣬�ڸı����ⶥ��λ�ú����
		void boundingBoxNeedsRecalculated(void) { BoundingBoxNeedsRecalculated = true; }

		//���߶����б�
		std::vector<S3DVertexTangents> Vertices_Tangents;
		//�������б�
		std::vector<S3DVertex2TCoords> Vertices_2TCoords;
		//��׼�����б�
		std::vector<S3DVertex> Vertices_Standard;
		//��������
		std::vector<UINT16> Indices;

		UINT32 ChangedID_Vertex;
		UINT32 ChangedID_Index;

		//ISkinnedMesh::SJoint *AttachedJoint;
		Matrix4 Transformation;

		SMaterial Material;
		E_VERTEX_TYPE VertexType;

		AxisAlignedBox BoundingBox;

		// Ӳ��ӳ����ʾ
		E_HARDWARE_MAPPING MappingHint_Vertex : 3;
		E_HARDWARE_MAPPING MappingHint_Index : 3;

		bool BoundingBoxNeedsRecalculated : 1;
	};
}

#endif