#ifndef _SAPPHIRE_PRIMITIVE_TYPE_
#define _SAPPHIRE_PRIMITIVE_TYPE_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	//! ö��ͼԪ����
	enum E_PRIMITIVE_TYPE
	{
		//��������
		EPT_POINTS = 0,

		//���ж������ӳ�һ�������߶�
		EPT_LINE_STRIP,

		//!ͬ�ϣ�������λ���
		EPT_LINE_LOOP,

		//! ÿ������������ΪN/2���߶�
		EPT_LINES,

		//!  ��ǰ����������ÿ�����㹹��һ����������N-2
		EPT_TRIANGLE_STRIP,

		//!  ��ǰ����������ÿ�����㹹��һ����������N-2
		//! ���е������ζ����Ƶ�һ������
		EPT_TRIANGLE_FAN,

		//! �������ж��㵽ÿһ��������
		EPT_TRIANGLES,

		//! ����ǰ��������After the first two vertices each further tw vetices create a quad with the preceding two.
		EPT_QUAD_STRIP,

		//! ÿ�ĸ����㹹��һ���ı���
		EPT_QUADS,

		//! ����һ��LINE_LOOP,�����
		EPT_POLYGON,

		//! ��GPU��һ��������չ��һ���ı��εĹ�����
		EPT_POINT_SPRITES
	};
}

#endif