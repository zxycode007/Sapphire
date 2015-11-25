#ifndef _SAPPHIRE_EDRIVER_TYPES_
#define _SAPPHIRE_EDRIVER_TYPES_

namespace Sapphire
{
	//! һ������֧�ֵ�������������
	enum E_DRIVER_TYPE
	{
		//! �����������ڶ�Ӧ�ó���ǿ��ӻ��ķ�ʽ���г���
		/** û�п��õ��豸��������������Ⱦ����ʾ�κ�ͼ��*/
		EDT_NULL,

		//! �����Ⱦ����
		/** ������������ƽ̨���κ�Ӳ������ֻ������2dͼ����Ҳ����Ӧ��һ��򵥵�3dͼԪ������
		��Щ�������Ʒǳ��죬���ǳ�����ȥ�����Ҳ���֧��3d����
		*/
		EDT_SOFTWARE,

		//! Burning�������Ⱦ��, һ������һ�������Ⱦ�� 
		/** */
		EDT_BURNINGSVIDEO,

		//! Direct3D8 �豸, ֻ������WIN32ƽ̨ʹ�� 
		/**  */
		EDT_DIRECT3D8,

		//! Direct3D9 �豸, ֻ������WIN32ƽ̨ʹ��   
		EDT_DIRECT3D9,

		//! OpenGL �豸, ��Ӧ��������ƽ̨ 
	 
		EDT_OPENGL,

		//����������
		EDT_COUNT
	};

}

#endif