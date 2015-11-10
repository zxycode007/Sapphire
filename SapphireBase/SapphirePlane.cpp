#include "SapphirePrerequisites.h"
#include "SapphirePlane.h"
#include "SapphireVector3.h"
#include "SapphireMatrix3.h"
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
		// 计算盒子中心和这个平面的距离
		Real dist = getDistance(centre);

		// 计算对于盒子中心和平面距离的最大允许绝对值
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
		// 我们知道平面法线是一个单位长度，所以用简单的方法
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
	std::ostream& operator<< (std::ostream& o, const Plane& p)
	{
		o << "Plane(normal=" << p.normal << ", d=" << p.d << ")";
		return o;
	}

}

