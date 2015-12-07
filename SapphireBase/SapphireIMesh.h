#ifndef _SAPPHIRE_MESH_
#define _SAPPHIRE_MESH_
#include "SapphireEHardwareBufferFlags.h"
#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireSMaterial.h"

namespace Sapphire
{

	class IMeshBuffer;

	//! ����һ�������������
	/** һ��IMesh�޷���һЩ���񻺳����ļ���
	(IMeshBuffer). SMesh ��һ����IMeshʵ��
	һ������Ϊ����Ⱦͨ����ӵ�һ��IMeshSceneNode��
	*/
	class IMesh : public virtual IReferenceCounter
	{
	public:
		//! ��ȡ���񻺳���������
		/** \return ���������IMeshBuffer������ */
		virtual UINT32 getMeshBufferCount() const = 0;

		//! ��ȡһ�����񻺳��ָ��
		/** \param nr: ���񻺳�������������0�±꿪ʼ�����ֵ
		getMeshBufferCount() - 1;
		\return ���񻺳���ָ�룬���û�У�����0 */
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const = 0;

		//! ��ȡһ�����񻺳��ָ�룬�������һ������
		/** \param material: Ҫ�ҵĲ���
		\return ���񻺳���ָ�룬���û�У�����0  */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const = 0;

		//! ��ȡAABB����
		/** \return ��������AABB���� */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! �����û������AABB����
		/** \param box ������������°󶨵ĺ���*/
		virtual void setBoundingBox(const AxisAlignedBox& box) = 0;

		//! �������а����Ĳ��ʵı�־��һ����ֵ
		/** \param flag: �������в��ʵı�־
		\param newvalue: �������в��ʵ���ֵ */
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue) = 0;

		//! ����Ӳ��ӳ����ʾ
		/** ����������������Ӳ���Ż���ʾ��Ϣ������ʹ������ЩӲ������֧�ֵ���Щ����
		���ܹ�������������������
		*/
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

		//! ��������׼�ı䣬����Ӳ��������
		/** ��������ڶ��������ÿ�θı�ʱ���á����򣬸ı䲻����GPU����һ����Ⱦѭ���и��� */
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;

	};
}

#endif