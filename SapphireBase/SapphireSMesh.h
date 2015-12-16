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
	//! IMesh�ӿڵļ�ʵ��
	struct SMesh : public IMesh
	{

		//! ���������������񻺳��������б�
		std::vector<IMeshBuffer*> MeshBuffers;

		//! �������İ�Χ�����
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


		//! �������񻺳���������
		virtual UINT32 getMeshBufferCount() const
		{
			return MeshBuffers.size();
		}

		//! �������񻺳�����ָ��
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const
		{
			return MeshBuffers[nr];
		}

		//! ����ƥ����һ�����ʵ����񻺳���
		/** ����Ѱ�� */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial & material) const
		{
			for (SINT32 i = (SINT32)MeshBuffers.size() - 1; i >= 0; --i)
			{
				if (material == MeshBuffers[i]->getMaterial())
					return MeshBuffers[i];
			}

			return 0;
		}

		//! ���ذ�Χ�����
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! �����û�����İ�Χ�����
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}

		//! ���¼����Χ�����
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
			


		//! ���һ�����񻺳���
		/** �����Χ����Ӳ����Զ����µ� */
		void addMeshBuffer(IMeshBuffer* buf)
		{
			if (buf)
			{
				buf->grab();
				MeshBuffers.push_back(buf);
			}
		}

		//! �����µ����а������ʵı�־
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->getMaterial().setFlag(flag, newvalue);
		}

		//! ����ȡ�ã�����Ӳ��ӳ����ʾ
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
		}

		//! ��������׼�ı䣬���û�����
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->setDirty(buffer);
		}

		
	};
}


#endif