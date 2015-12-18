#ifndef _SAPPHIRE_SPARTICLE_
#define _SAPPHIRE_SPARTICLE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireDimension2D.h"
#include "SapphireColorValue.h"


namespace Sapphire
{
	 

	//! �����������ݵĽṹ��
	struct SParticle
	{
		//! ���ӵ�λ��
		Vector3 pos;

		//! ���ӵķ�����ٶ�
		Vector3 vector;

		//! ���ӵ�������ʼʱ��
		UINT32 startTime;

		//! ���ӵ���������ʱ��
		UINT32 endTime;

		//! ���ӵĵ�ǰ��ɫ
		ColourValue color;

		//! ���ӵ�ԭ��ɫ 
		/** �����ӷ���ʱ����ɫ*/
		ColourValue startColor;

		//! ���ӵ�ԭ������ٶ�
		/** �����ӷ���ʱ�ķ�����ٶ� */
		Vector3 startVector;

		//! ���ӵ�����
		/** ���ӵĵ�ǰ���� */
		dimension2df size;

		//! ���ӵ�ԭ����
		/** �����ڷ���ʱ���� */
		dimension2df startSize;
	};
}

#endif
