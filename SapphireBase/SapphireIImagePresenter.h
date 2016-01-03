#ifndef _SAPPHIRE_I_IMAGE_PRESENTER_
#define _SAPPHIRE_I_IMAGE_PRESENTER_

#include "SapphireIImage.h"

namespace Sapphire
{
	/*!
	����ӿ����ʾһ����ʾ��ƽ���ϵ�ͼ�� ���������Ⱦ��ͨ��ֻͨ��Deviceʵ��


	�����Ӧ��ֻ���ڲ�ʹ��
	*/

	class IImagePresenter
	{
	public:

		virtual ~IImagePresenter() {};
		//! չʾһ�����浽�ͻ�������
		virtual bool present(IImage* surface, void* windowId = 0, rect<SINT32>* src = 0) = 0;
	};
}

#endif