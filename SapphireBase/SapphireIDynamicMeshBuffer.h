#ifndef _SAPPHIRE_I_DYNAMIC_MESH_BUFFER_
#define _SAPPHIRE_I_DYNAMIC_MESH_BUFFER_

#include "SapphireIMeshBuffer.h"
#include "SapphireIVertexBuffer.h"
#include "SapphireIIndexBuffer.h"


namespace Sapphire
{
	/** 一个动态网格缓冲区 */
	class IDynamicMeshBuffer : public IMeshBuffer
	{
	public:
		virtual IVertexBuffer &getVertexBuffer() const = 0;
		virtual IIndexBuffer &getIndexBuffer() const = 0;

		virtual void setVertexBuffer(IVertexBuffer *vertexBuffer) = 0;
		virtual void setIndexBuffer(IIndexBuffer *indexBuffer) = 0;

		//! 获取这个网格缓冲区的材质
		/** \return 这个缓冲区的材质 */
		virtual SMaterial& getMaterial() = 0;

		//! 获取这个网格缓冲区的材质
		/** \return 缓冲区的材质 */
		virtual const SMaterial& getMaterial() const = 0;

		//! 获取这个网格缓冲区的AABB盒子
		/** \return 这个网格缓冲区的AABB盒子 */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! 设置AABB盒子
		/** \param box 用户对这个缓冲区定义的AABB盒子
		  */
		virtual void setBoundingBox(const AxisAlignedBox& box) = 0;

		//! 重新计算碰撞盒子，如果网格改变时调用
		virtual void recalculateBoundingBox() = 0;

		//! 附加顶点和所有到当前缓冲区
		/** 只对兼容的顶点类型
		\param vertices 顶点数组的指针
		\param numVertices 顶点数组的元素数量
		\param indices 顶点索引数组的指针
		\param numIndices 顶点索引数*/
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices)
		{

		}

		//! 添加一个网格缓冲区到当前缓冲区
		/** 只对兼容的顶点类型
		\param other 要添加的网格缓冲区*/
		virtual void append(const IMeshBuffer* const other)
		{

		}

		// -------------------要移除 -------------------  //

		//! 获取当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const
		{
			return getVertexBuffer().getHardwareMappingHint();
		}

		//! 获取当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const
		{
			return getIndexBuffer().getHardwareMappingHint();
		}

		//! 对驱动设置硬件映射提示
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				getVertexBuffer().setHardwareMappingHint(NewMappingHint);
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				getIndexBuffer().setHardwareMappingHint(NewMappingHint);
		}

		//! 网格改变标志，重载硬件缓冲区
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

		// ------------------- 旧接口 -------------------  //

		//! 获取顶点数据的的类型，它保存在网格缓冲区中
		/** \return 这个缓冲区顶点类型*/
		virtual E_VERTEX_TYPE getVertexType() const
		{
			return getVertexBuffer().getType();
		}

		//! 获取顶点数据，这个数据是一个顶点数组
		/** 顶点类型可用getVertexType()得到
		\return 顶点数组的指针 */
		virtual const void* getVertices() const
		{
			return getVertexBuffer().getData();
		}

		//! 获取顶点数据，这个数据是一个顶点数组
		/** 顶点类型可用getVertexType()得到
		\return 顶点数组的指针 */
		virtual void* getVertices()
		{
			return getVertexBuffer().getData();
		}

		//! 获取网格缓冲区中顶点的数量
		/** \return 网格缓冲区中顶点的数量 */
		virtual UINT32 getVertexCount() const
		{
			return getVertexBuffer().size();
		}

		//! 获取这个网格缓冲区中索引数据的类型
		/** \return 缓冲区中的索引类型 */
		virtual E_INDEX_TYPE getIndexType() const
		{
			return getIndexBuffer().getType();
		}

		//! 取得索引数据
		/** \return 索引数组的指针*/
		virtual const UINT16* getIndices() const
		{
			return (UINT16*)getIndexBuffer().getData();
		}

		//! 取得索引数据
		/** \return 索引数组的指针*/
		virtual UINT16* getIndices()
		{
			return (UINT16*)getIndexBuffer().getData();
		}

		//! 获取网格缓冲区中索引的数量
		/** \return Number of indices in this buffer. */
		virtual UINT32 getIndexCount() const
		{
			return getIndexBuffer().size();
		}

		//! 返回顶点i的位置
		virtual const Vector3& getPosition(UINT32 i) const
		{
			return getVertexBuffer()[i].Pos;
		}

		//! 返回顶点i的位置
		virtual Vector3& getPosition(UINT32 i)
		{
			return getVertexBuffer()[i].Pos;
		}

		//! 返回顶点i的纹理坐标
		virtual const Vector2& getTCoords(UINT32 i) const
		{
			return getVertexBuffer()[i].TCoords;
		}

		//! 返回顶点i的纹理坐标
		virtual Vector2& getTCoords(UINT32 i)
		{
			return getVertexBuffer()[i].TCoords;
		}

		//! 返回顶点i的法线
		virtual const Vector3& getNormal(UINT32 i) const
		{
			return getVertexBuffer()[i].Normal;
		}

		//! 返回顶点i的法线
		virtual Vector3& getNormal(UINT32 i)
		{
			return getVertexBuffer()[i].Normal;
		}
	};
}

#endif