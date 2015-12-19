#ifndef _SAPPHIRE_SSKIN_MESH_BUFFER_
#define _SAPPHIRE_SSKIN_MESH_BUFFER_

#include "SapphirePrerequisites.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	//! 一个可在运行时刻对S3DVertex2TCoords, S3DVertex 和 S3DVertexTangents之间进行选择缓冲的网格缓冲区
	struct SSkinMeshBuffer : public IMeshBuffer
	{
		//! 默认的构造器
		SSkinMeshBuffer(E_VERTEX_TYPE vt = EVT_STANDARD) :
			ChangedID_Vertex(1), ChangedID_Index(1), VertexType(vt),
			MappingHint_Vertex(EHM_NEVER), MappingHint_Index(EHM_NEVER),
			BoundingBoxNeedsRecalculated(true)
		{
//#ifdef _DEBUG
//			setDebugName("SSkinMeshBuffer");
//#endif
		}

		//! 获取这个缓冲区的材质
		virtual const SMaterial& getMaterial() const
		{
			return Material;
		}

		//! 获取这个缓冲区的材质
		virtual SMaterial& getMaterial()
		{
			return Material;
		}

		//! 获取给定索引的标准顶点
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

		//! 获取顶点数组的指针
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

		//! 获取顶点数组的指针
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

		//! 获取顶点的数量
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

		//! 获取保存在这个网格缓冲区中的索引数据类型
		/** \return Index type of this buffer. */
		virtual E_INDEX_TYPE getIndexType() const
		{
			return EIT_16BIT;
		}

		//! 获取索引数组的指针
		virtual const UINT16* getIndices() const
		{   			
			return Indices.begin()._Ptr;
			 
		}

		//!  获取索引数组的指针
		virtual UINT16* getIndices()
		{
			return Indices.begin()._Ptr;
		}

		//! 获取索引数组大小
		virtual UINT32 getIndexCount() const
		{
			return Indices.size();
		}

		//! 获取碰撞盒子
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! 设置碰撞盒子
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}

		//! 重新计算AABB盒子
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

		//! 获取顶点类型
		virtual E_VERTEX_TYPE getVertexType() const
		{
			return VertexType;
		}

		//! 获取纹理顶点类型
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

		//! 转换为切线顶点类型
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

		//! 返回顶点i的位置
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

		//! 返回顶点i的位置
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

		//! 返回顶点i的法线
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

		//! 返回顶点i的法线
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

		//! 返回顶点i的纹理坐标
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

		//!  返回顶点i的纹理坐标
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

		//! 添加顶点和索引到当前缓冲区
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices) {}

		//! 添加网格缓冲区到当前缓冲区
		virtual void append(const IMeshBuffer* const other) {}

		//! 获取对于顶点缓冲区的当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const
		{
			return MappingHint_Vertex;
		}

		//! 获取对于顶点缓冲区的当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const
		{
			return MappingHint_Index;
		}

		//! 对驱动设置硬件映射提示
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

		//! 网格发生改变标志，重载硬件缓冲区
		virtual void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				++ChangedID_Vertex;
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				++ChangedID_Index;
		}

		virtual UINT32 getChangedID_Vertex() const { return ChangedID_Vertex; }

		virtual UINT32 getChangedID_Index() const { return ChangedID_Index; }

		//! AABB需要重新计算，在改变任意顶点位置后调用
		void boundingBoxNeedsRecalculated(void) { BoundingBoxNeedsRecalculated = true; }

		//切线顶点列表
		std::vector<S3DVertexTangents> Vertices_Tangents;
		//纹理顶点列表
		std::vector<S3DVertex2TCoords> Vertices_2TCoords;
		//标准顶点列表
		std::vector<S3DVertex> Vertices_Standard;
		//顶点索引
		std::vector<UINT16> Indices;

		UINT32 ChangedID_Vertex;
		UINT32 ChangedID_Index;

		//ISkinnedMesh::SJoint *AttachedJoint;
		Matrix4 Transformation;

		SMaterial Material;
		E_VERTEX_TYPE VertexType;

		AxisAlignedBox BoundingBox;

		// 硬件映射提示
		E_HARDWARE_MAPPING MappingHint_Vertex : 3;
		E_HARDWARE_MAPPING MappingHint_Index : 3;

		bool BoundingBoxNeedsRecalculated : 1;
	};
}

#endif