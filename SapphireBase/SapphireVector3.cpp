#include "SapphireVector3.h"
#include "SapphireQuaternion.h"
#include "SapphireMath.h"


namespace Sapphire
{
	const Vector3 Vector3::ZERO(0, 0, 0);

	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);
	const Vector3 Vector3::NEGATIVE_UNIT_X(-1, 0, 0);
	const Vector3 Vector3::NEGATIVE_UNIT_Y(0, -1, 0);
	const Vector3 Vector3::NEGATIVE_UNIT_Z(0, 0, -1);
	const Vector3 Vector3::UNIT_SCALE(1, 1, 1);

	Quaternion Vector3::getRotationTo(const Vector3& dest,
		const Vector3& fallbackAxis) const
	{
		// ����Stan Melax�� Game Programming Gems������
		Quaternion q;
		// ���ƣ������޸ı���ԭʼ����
		Vector3 v0 = *this;
		Vector3 v1 = dest;
		v0.normalise();
		v1.normalise();

		Real d = v0.dotProduct(v1);
		//������ == 1, ������ͬ
		if (d >= 1.0f)
		{
			return Quaternion::IDENTITY;
		}
		if (d < (1e-6f - 1.0f))
		{
			if (fallbackAxis != Vector3::ZERO)
			{
				// ��ת 180 ��  fallback axis
				q.FromAngleAxis(Radian(Math::_PI), fallbackAxis);
			}
			else
			{
				// ���������
				Vector3 axis = Vector3::UNIT_X.crossProduct(*this);
				if (axis.isZeroLength()) // pick another if colinear
					axis = Vector3::UNIT_Y.crossProduct(*this);
				axis.normalise();
				q.FromAngleAxis(Radian(Math::_PI), axis);
			}
		}
		else
		{
			Real s = Math::Sqrt((1 + d) * 2);
			Real invs = 1 / s;

			Vector3 c = v0.crossProduct(v1);

			q.x = c.x * invs;
			q.y = c.y * invs;
			q.z = c.z * invs;
			q.w = s * 0.5f;
			q.normalise();
		}
		return q;
	}

}