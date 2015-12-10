#ifndef _SAPPHIRE_SLIGHT_
#define _SAPPHIRE_SLIGHT_

#include "SapphirePrerequisites.h"
#include "SapphireColorValue.h"
#include "SapphireVector3.h"

namespace Sapphire
{
	//! ��ͬ��Դ���͵�ö��
	enum E_LIGHT_TYPE
	{
		//! point light, it has a position in space and radiates light in all directions
		ELT_POINT,
		//! spot light, it has a position in space, a direction, and a limited cone of influence
		ELT_SPOT,
		//! directional light, coming from a direction from an infinite distance
		ELT_DIRECTIONAL,

		//! Only used for counting the elements of this enum
		ELT_COUNT
	};

	//! ��Դ������
	const c8* const LightTypeNames[] =
	{
		"Point",
		"Spot",
		"Directional",
		0
	};

	//! ����������̬���Դ�Ľ��
	/** ֧�ֵ��Դ���۹�ƣ�ƽ�й�
	*/
	struct SLight
	{
		SLight() : AmbientColor(0.f, 0.f, 0.f), DiffuseColor(1.f, 1.f, 1.f),
			SpecularColor(1.f, 1.f, 1.f), Attenuation(1.f, 0.f, 0.f),
			OuterCone(45.f), InnerCone(0.f), Falloff(2.f),
			Position(0.f, 0.f, 0.f), Direction(0.f, 0.f, 1.f),
			Radius(100.f), Type(ELT_POINT), CastShadows(true)
		{}

		//! ��Դ�ķ���Ļ�������ɫ 
		ColourValue AmbientColor;

		//! ��Դ��������������ɫ
		/** ������Ҫ���õĹ�Դ����ɫ*/
		ColourValue DiffuseColor;

		//! ��Դ����ĸ߹���ɫ
		/** ����ӵ�о��淴��߹�ʹ�õ�ϸ��  ��SMaterial::Shininess */
		ColourValue SpecularColor;

		//! ˥������(����, ����, ���ε�)
		/** �ı��Դǿ�������Ӱ��ĳ̶ȣ���ͬʱ���ð뾶���޸ģ�˥�����ı�(0,1.f/radius,0)��
		�����ڰ뾶���ú󸲸�
		 */
		Vector3 Attenuation;

		//! �۹����׶�Ƕȣ���������Դ��Ч
		Real OuterCone;

		//! �۹����׶�Ƕȣ���������Դ��Ч
		Real InnerCone;

		//! ����׶����׶֮�併�͵Ĺ�Դǿ��
		Real Falloff;

		//! ֻ��! ��Դ��λ��
		/** �����������ΪELT_DIRECTIONAL, ��Ч. ͨ����Դ�����ڵ�λ�����ı� */
		Vector3 Position;

		//! ֻ��! ��Դ�ķ���
		/** ��������趨ΪELT_POINT, ��Ч. ͨ����Դ�����ڵ���ת���ı�. */
		Vector3 Direction;

		//! ֻ��! ��Դ�İ뾶. �뾶�����еĶ������ᱻ���뵽�˹��ռ���
		Real Radius;

		//! ֻ��! ��Դ������ Ĭ�ϣ�ELT_POINT
		E_LIGHT_TYPE Type;

		//! ֻ��! ��Դ�Ƿ�Ͷ����Ӱ��
		bool CastShadows : 1;
	};
}

#endif