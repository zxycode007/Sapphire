#ifndef _SAPPHIRE_IVIDEO_MODE_LIST_
#define _SAPPHIRE_IVIDEO_MODE_LIST_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireDimension2D.h"

namespace Sapphire
{
	//! һ�����п�����Ƶģʽ���б�
	/** �����ͨ��Device::getVideoModeList()��������ȡһ���б���������ڸ㲻�����
	��Ϊ������Ƶģʽ����Device֮ǰ��Ҫ�ܹ���ȡ��Ƶ�б�
	 */
	class IVideoModeList : public virtual IReferenceCounter
	{
	public:

		//! ��ȡ�б����Ƶģʽ������
		/** \return ������Ƶģʽ������ */
		virtual SINT32 getVideoModeCount() const = 0;

		//! ��ȡ��Ƶģʽ�Ĵ��ڵ����ش�С
		/** \param modeNumber: ��Ƶģʽ��������0��ʼ
		\return ��Ƶģʽ�Ĵ��ڵ����ش�С*/
		virtual  dimension2d<UINT32> getVideoModeResolution(SINT32 modeNumber) const = 0;

		//! ��ȡһ��ĳЩ����Լ������Ļ��С
		/** \param minSize: �������С�ߴ�
		\param maxSize: ��������ߴ�
		\return ƥ����Щ�����ĵ���Ƶģʽ�Ĵ��ڵ����ش�С*/
		virtual  dimension2d<UINT32> getVideoModeResolution(const  dimension2d<UINT32>& minSize, const  dimension2d<UINT32>& maxSize) const = 0;

		//! ��ȡ��Ƶģʽ������λ��
		/** \param modeNumber: ��Ƶģʽ��������0��ʼ
		\return ָ����Ƶģʽ��ÿ������ɫλ�� */
		virtual SINT32 getVideoModeDepth(SINT32 modeNumber) const = 0;

		//! ��ȡ��ǰ������Ļ�ֱ���
		/** \return ��ǰ������Ƶģʽ�����ش�С*/
		virtual const  dimension2d<UINT32>& getDesktopResolution() const = 0;

		//! ��ȡ��ǰ��Ƶģʽ����λ��
		/** \returnָ����ǰ������Ƶģʽ��ÿ������ɫλ�� */
		virtual SINT32 getDesktopDepth() const = 0;
	};
}

#endif