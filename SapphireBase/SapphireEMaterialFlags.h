#ifndef _SAPPHIRE_EMATERIAL_FLAGS_
#define _SAPPHIRE_EMATERIAL_FLAGS_

namespace Sapphire
{
	//! ���ʱ�־
	enum E_MATERIAL_FLAG
	{
		//! �����߿������������  Ĭ�ϣ�false
		EMF_WIREFRAME = 0x1,

		//! ���Ƶ��ƻ�����������  Ĭ�ϣ�false 
		EMF_POINTCLOUD = 0x2,

		//! Flat �� Gouraud ��ɫ? Ĭ�ϣ�true
		EMF_GOURAUD_SHADING = 0x4,

		//! ���ʽ��й��ռ���  Ĭ�ϣ�true	
		EMF_LIGHTING = 0x8,

		//! ��Zbuffer  Ĭ�ϣ� true
		EMF_ZBUFFER = 0x10,

		//! Zbuffer��д����ֻ����  Ĭ�ϣ�true
		/** �����־��Ч�� �������������һ��͸�����͵Ļ� */
		EMF_ZWRITE_ENABLE = 0x20,

		//! �����޳�  Ĭ�ϣ�true
		EMF_BACK_FACE_CULLING = 0x40,

		//! �����޳�? Ĭ�ϣ�false
		EMF_FRONT_FACE_CULLING = 0x80,

		//! ˫�����˲�  Ĭ�ϣ� true
		EMF_BILINEAR_FILTER = 0x100,

		//! �������˲�   Ĭ�ϣ� false
		/** ����������˲���������ô˫�����˲���־��Ч */
		EMF_TRILINEAR_FILTER = 0x200,

		//! ���������˲�  Ĭ�ϣ� false
		EMF_ANISOTROPIC_FILTER = 0x400,

		//! ������ Ĭ�ϣ� false
		EMF_FOG_ENABLE = 0x800,

		//! ��׼������  Ĭ�� false
		/** �������Ҫ���Ŷ�̬��Դģʽ�� ����Դ����ģʽ
		���ķ��߿������űȽϴ�����䰵�������EMF_NORMALIZE_NORMALS��־���ⷨ�߻ᱻ��׼����
		ģ�Ϳ�������Ӧ���е�����
		 */
		EMF_NORMALIZE_NORMALS = 0x1000,

		//! �������в������warpģʽ��  ��д���������
		EMF_TEXTURE_WRAP = 0x2000,

		//! �����ģʽ
		EMF_ANTI_ALIASING = 0x4000,

		//! ��ɫ����λ ���ڴ���ɫƽ��
		EMF_COLOR_MASK = 0x8000,

		//! ��ɫ����ColorMaterial ���ڶ�����ɫ��ֵ��ö��
		EMF_COLOR_MATERIAL = 0x10000,

		//! �򿪹ر�ʹ��mipmap�ı�־
		EMF_USE_MIP_MAPS = 0x20000,

		//! ��ϲ�����־
		EMF_BLEND_OPERATION = 0x40000,

		//! �����ƫ�Ʊ�־
		EMF_POLYGON_OFFSET = 0x80000
	};
}

#endif