#ifndef _SAPPHIRE_MESH_
#define _SAPPHIRE_MESH_
#include "SapphireEHardwareBufferFlags.h"
#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireSMaterial.h"

namespace Sapphire
{

	class IMeshBuffer;

	//! 保存一个对象网格的类
	/** 一个IMesh无非是一些网格缓冲区的集合
	(IMeshBuffer). SMesh 是一个简单IMesh实现
	一个网格为了渲染通常添加到一个IMeshSceneNode里
	*/
	class IMesh : public virtual IReferenceCounter
	{
	public:
		//! 获取网格缓冲区的数量
		/** \return 这个网格中IMeshBuffer的数量 */
		virtual UINT32 getMeshBufferCount() const = 0;

		//! 获取一个网格缓冲的指针
		/** \param nr: 网格缓冲区索引，基于0下标开始，最大值
		getMeshBufferCount() - 1;
		\return 网格缓冲区指针，如果没有，返回0 */
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const = 0;

		//! 获取一个网格缓冲的指针，它组合了一个材质
		/** \param material: 要找的材质
		\return 网格缓冲区指针，如果没有，返回0  */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const = 0;

		//! 获取AABB盒子
		/** \return 这个网格的AABB盒子 */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! 设置用户定义的AABB盒子
		/** \param box 对于这个网格新绑定的盒子*/
		virtual void setBoundingBox(const AxisAlignedBox& box) = 0;

		//! 设置所有包含的材质的标志到一个新值
		/** \param flag: 设置所有材质的标志
		\param newvalue: 设置所有材质的新值 */
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue) = 0;

		//! 设置硬件映射提示
		/** 这个方法允许定义对于硬件优化提示信息。开启使用在这些硬件缓冲支持的这些特征
		这能够带来显著的性能增益
		*/
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//! 缓冲区标准改变，重载硬件缓冲区
		/** 这个方法在顶点或索引每次改变时调用。否则，改变不会在GPU的下一次渲染循环中更新 */
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

	};
}

#endif