#include "SapphirePrerequisites.h"
#include "SapphirePlane.h"
#include "SapphireVector3.h"
#include "SapphireMatrix3.h"
#include "SapphireRay.h"
#include "SapphireAxisAlignedBox.h"


namespace Sapphire {


	 
	Plane::Plane()
	{
		normal = Vector3::ZERO;
		d = 0.0;
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const Plane& rhs)
	{
		normal = rhs.normal;
		d = rhs.d;
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const Vector3& rkNormal, Real fConstant)
	{
		normal = rkNormal;
		d = -fConstant;
	}
	//---------------------------------------------------------------------
	Plane::Plane(Real a, Real b, Real c, Real _d)
		: normal(a, b, c), d(_d)
	{
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const Vector3& rkNormal, const Vector3& rkPoint)
	{
		redefine(rkNormal, rkPoint);
	}
	//-----------------------------------------------------------------------
	Plane::Plane(const Vector3& rkPoint0, const Vector3& rkPoint1,
		const Vector3& rkPoint2)
	{
		redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	Real Plane::getDistance(const Vector3& rkPoint) const
	{
		return normal.dotProduct(rkPoint) + d;
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide(const Vector3& rkPoint) const
	{
		Real fDistance = getDistance(rkPoint);

		if (fDistance < 0.0)
			return Plane::NEGATIVE_SIDE;

		if (fDistance > 0.0)
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}


	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide(const AxisAlignedBox& box) const
	{
		if (box.isNull())
			return NO_SIDE;
		if (box.isInfinite())
			return BOTH_SIDE;

		return getSide(box.getCenter(), box.getHalfSize());
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide(const Vector3& centre, const Vector3& halfSize) const
	{
		// ����������ĺ����ƽ��ľ���
		Real dist = getDistance(centre);

		// ������ں������ĺ�ƽ����������������ֵ
		Real maxAbsDist = normal.absDotProduct(halfSize);

		if (dist < -maxAbsDist)
			return Plane::NEGATIVE_SIDE;

		if (dist > +maxAbsDist)
			return Plane::POSITIVE_SIDE;

		return Plane::BOTH_SIDE;
	}
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
		const Vector3& rkPoint2)
	{
		Vector3 kEdge1 = rkPoint1 - rkPoint0;
		Vector3 kEdge2 = rkPoint2 - rkPoint0;
		normal = kEdge1.crossProduct(kEdge2);
		normal.normalise();
		d = -normal.dotProduct(rkPoint0);
	}
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3& rkNormal, const Vector3& rkPoint)
	{
		normal = rkNormal;
		d = -rkNormal.dotProduct(rkPoint);
	}
	//-----------------------------------------------------------------------
	Vector3 Plane::projectVector(const Vector3& p) const
	{
		// ����֪��ƽ�淨����һ����λ���ȣ������ü򵥵ķ���
		Matrix3 xform;
		xform[0][0] = 1.0f - normal.x * normal.x;
		xform[0][1] = -normal.x * normal.y;
		xform[0][2] = -normal.x * normal.z;
		xform[1][0] = -normal.y * normal.x;
		xform[1][1] = 1.0f - normal.y * normal.y;
		xform[1][2] = -normal.y * normal.z;
		xform[2][0] = -normal.z * normal.x;
		xform[2][1] = -normal.z * normal.y;
		xform[2][2] = 1.0f - normal.z * normal.z;
		return xform * p;

	}
	//-----------------------------------------------------------------------
	Real Plane::normalise(void)
	{
		Real fLength = normal.length();

		 
		if (fLength > Real(0.0f))
		{
			Real fInvLength = 1.0f / fLength;
			normal *= fInvLength;
			d *= fInvLength;
		}

		return fLength;
	}
	//-----------------------------------------------------------------------
	void Plane::transformPlane(const Matrix4& matrix)
	{
		Vector3 newP;
		normal.normalise();
		//d��ƽ�浽ԭ��ľ��룬 ƽ�浽ԭ��������Ƿ��ߵĸ��������ԣ�-d*normal = ԭ�㵽ƽ�������
		newP = -d * normal;
		newP = matrix * newP;
		Vector3 _normal = normal;
		_normal = matrix.inverse() * _normal;
		redefine(_normal, newP);

	}
	//-----------------------------------------------------------------------
	bool Plane::getIntersectionWithPlane(const Plane& other,
		Vector3& outLinePoint,
		Vector3& outLineVect) const
	{
		//�����ǰƽ��ķ��߳���
		const Real fn00 = normal.length();
		//�����ǰƽ��������һƽ��ĵ��
		const Real fn01 = normal.dotProduct(other.normal);
		//����һƽ�淨�߳���
		const Real fn11 = other.normal.length();
		//�ÿ���Ĭ��������Է�����
		const Real det = fn00*fn11 - fn01*fn01;

		if (fabs(det) <= std::numeric_limits<Real>::epsilon())
			return false;

		const Real invdet = 1.0 / det;
		const Real fc0 = (fn11*-d + fn01*other.d) * invdet;
		const Real fc1 = (fn00*-other.d + fn01*d) * invdet;

		//������������ˣ��õ��ֱཻ�ߵķ�������
		outLineVect = normal.crossProduct(other.normal);
		outLinePoint = normal*fc0 + other.normal*fc1;
		return true;
	}
	//-----------------------------------------------------------------------
	//! ����еĻ�����ȡ����������ƽ��Ľ���
	bool Plane::getIntersectionWithPlanes(const Plane& o1,
		const Plane& o2, Vector3& outPoint) const 
	{
		Vector3 linePoint, lineVect;
		//��ȡ��ƽ����o1ƽ�潻��
		if (getIntersectionWithPlane(o1, linePoint, lineVect))
		{
			Ray ray = Ray(linePoint, lineVect);
			Plane plane = *this;
			std::pair<bool, Real> t = Math::intersects(ray, plane);
			//���first Ϊtrue���ཻ��false ƽ��
			if (t.first)
			{
				//�ж�o2ƽ����֮ǰray�Ľ���
				t = Math::intersects(ray, o2);
				//�������
				outPoint = ray.getPoint(t.second);
				return t.first;
			}
			return false;
			//return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);
		}
		return false;
	}
	//-----------------------------------------------------------------------
	std::ostream& operator<< (std::ostream& o, const Plane& p)
	{
		o << "Plane(normal=" << p.normal << ", d=" << p.d << ")";
		return o;
	}

}

