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
	//! 保存用单材质的网格
	/** 一个IMesh的一个部分的这一组的每个面都有同样的材质
	一个IMesh的逻辑组不会放到独立的分离缓冲区中，但是能够这个网格分离的动画部分必须被放到
	独立的网格缓冲区中。
	有些网格缓冲区的实现限制了缓冲区能保存的顶点数量。这种情况下，逻辑组有所帮助，此外，顶点数量应该为上传GPU做优化。
	它依赖gfx开的类型。通常数字是每个缓冲区1000~10000个顶点
	SMeshBuffer是一个简单的MeshBuffer的实现，它支持65535个顶点

	自网格缓冲区被用于绘制，并且因此在面对驱动，会有很高几率在某处被grab（）。
	因此它需要动态分配网格缓冲区，传递到一个视频驱动并且仅在丢弃这个缓冲区时，它不在当前代码块使用
	*/
	class IMeshBuffer //: public virtual IReferenceCounted
	{
	public:

		//! 获取这个网格缓冲区的材质
		/** \return Material of this buffer. */
		virtual  SMaterial& getMaterial() = 0;

		//! 获取这个网格缓冲区的材质
		/** \return 返回这个缓冲区使用的材质 */
		virtual const  SMaterial& getMaterial() const = 0;

		//! 获取可以存放在顶点缓冲区内顶点数据的类型
		/** \return 顶点的类型 */
		virtual  E_VERTEX_TYPE getVertexType() const = 0;

		//! 获取访问顶点数据，这个数据是一个顶点的数组（只读）
		/** 顶点类型可以用getVertexType()取得
		\return 指向顶点数组的指针 */
		virtual const void* getVertices() const = 0;

		//! 获取访问顶点数据，这个数据是一个顶点的数组
		/** 顶点类型可以用getVertexType()取得
		\return指向顶点数组的指针 */
		virtual void* getVertices() = 0;

		//! 获取在网格缓冲区里顶点的数量
		/** \return 网格缓冲区里顶点的数量 */
		virtual UINT32 getVertexCount() const = 0;

		//! 获取保存在网格缓冲区的索引类型
		/** \return 这个缓冲区的索引类型 */
		virtual E_INDEX_TYPE getIndexType() const = 0;

		//! 获取获取索引数据（只读）
		/** \return 索引数据的指针 */
		virtual const UINT16* getIndices() const = 0;

		//! 获取获取索引数据
		/** \return 索引数据的指针 */
		virtual UINT16* getIndices() = 0;

		//! 获取在这个网格缓冲区中索引的数量
		/** \return 在这个网格缓冲区中索引的数量 */
		virtual UINT32 getIndexCount() const = 0;

		//! 获取这个网格缓冲区的AABB盒子
		/** \return 这个网格缓冲区的AABB盒子 */
		virtual const  AxisAlignedBox& getBoundingBox() const = 0;

		//! 设置AABB盒子
		/** \param box 用户为这个网格缓冲区定义的AABB盒子
		for this buffer. */
		virtual void setBoundingBox(const  AxisAlignedBox& box) = 0;

		//! 重新计算碰撞盒子，如果网格改变的话应该调用
		virtual void recalculateBoundingBox() = 0;

		//! 返回顶点I的位置信息（只读）
		virtual const Vector3& getPosition(UINT32 i) const = 0;

		//! 返回顶点I的位置信息
		virtual Vector3& getPosition(UINT32 i) = 0;

		//! 返回顶点I的法线信息（只读）
		virtual const Vector3& getNormal(UINT32 i) const = 0;

		//! 返回顶点I的法线信息
		virtual Vector3& getNormal(UINT32 i) = 0;

		//! 返回顶点I的纹理坐标（只读）
		virtual const Vector2& getTCoords(UINT32 i) const = 0;

		//!返回顶点I的法线信息
		virtual Vector2& getTCoords(UINT32 i) = 0;

		//! 添加顶点和它的索引到当前缓冲区
		/**只工作在兼容的顶点类型
		\param vertices 顶点数组指针
		\param numVertices 顶点数量
		\param indices 索引指针
		\param numIndices 索引数量 */
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices) = 0;

		//! 添加网格缓冲区到当前缓冲区
		/** 只工作在兼容的顶点类型
		\param other 要添加的缓冲区 */
		virtual void append(const IMeshBuffer* const other) = 0;

		//! 获得当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const = 0;

		//! 获得当前硬件映射提示
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const = 0;

		//! 为驱动设置硬件映射提示
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//! 网格缓冲区标准改变，重新装载硬件缓冲区
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//!对于改变的确认获取当前使用ID  
		/** 这不应该用于任何外部视频驱动*/
		virtual UINT32 getChangedID_Vertex() const = 0;

		//! 对于改变的确认获取当前使用ID
		/**这不应该用于任何外部视频驱动 */
		virtual UINT32 getChangedID_Index() const = 0;
	};
}

#endif