#ifndef _SAPPHIRE_SMESH_
#define _SAPPHIRE_SMESH_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireIMesh.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireSMaterial.h"



namespace Sapphire
{
	//! IMesh接口的简单实现
	struct SMesh : public IMesh
	{

		//! 属于这个网格的网格缓冲区数组列表
		std::vector<IMeshBuffer*> MeshBuffers;

		//! 这个网格的包围体盒子
		AxisAlignedBox BoundingBox;

	 
		SMesh()
		{

		}

		 
		virtual ~SMesh()
		{
			
			 
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->drop();
		}

		 
		virtual void clear()
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->drop();
			MeshBuffers.clear();
			//BoundingBox.reset(0.f, 0.f, 0.f);
			BoundingBox.setMaximum(Vector3(0.0f, 0.0f, 0.0f));
			BoundingBox.setMinimum(Vector3(0.0f, 0.0f, 0.0f));
		}


		//! 返回网格缓冲区的数量
		virtual UINT32 getMeshBufferCount() const
		{
			return MeshBuffers.size();
		}

		//! 返回网格缓冲区的指针
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const
		{
			return MeshBuffers[nr];
		}

		//! 返回匹配了一个材质的网格缓冲区
		/** 逆向寻找 */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial & material) const
		{
			for (SINT32 i = (SINT32)MeshBuffers.size() - 1; i >= 0; --i)
			{
				if (material == MeshBuffers[i]->getMaterial())
					return MeshBuffers[i];
			}

			return 0;
		}

		//! 返回包围体盒子
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! 设置用户定义的包围体盒子
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}

		//! 重新计算包围体盒子
		void recalculateBoundingBox()
		{
			if (MeshBuffers.size())
			{
				BoundingBox = MeshBuffers[0]->getBoundingBox();
				for (UINT32 i = 1; i<MeshBuffers.size(); ++i)
					//BoundingBox.addInternalBox(MeshBuffers[i]->getBoundingBox());
					BoundingBox = MeshBuffers[i]->getBoundingBox();
			}
			else{
					//BoundingBox.reset(0.0f, 0.0f, 0.0f);
					BoundingBox.setMaximum(Vector3(0.0f, 0.0f, 0.0f));
					BoundingBox.setMinimum(Vector3(0.0f, 0.0f, 0.0f));
				}
		}
			


		//! 添加一个网格缓冲区
		/** 这个包围体盒子不是自动更新的 */
		void addMeshBuffer(IMeshBuffer* buf)
		{
			if (buf)
			{
				buf->grab();
				MeshBuffers.push_back(buf);
			}
		}

		//! 设置新的所有包含材质的标志
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->getMaterial().setFlag(flag, newvalue);
		}

		//! 对于取得，设置硬件映射提示
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
		}

		//! 缓冲区标准改变，重置缓冲区
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->setDirty(buffer);
		}

		
	};
}


#endif