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
	void Plane::transformPlane(const Matrix4& matrix)
	{
		Vector3 newP;
		normal.normalise();
		//d是平面到原点的距离， 平面到原点的向量是法线的负方向，所以，-d*normal = 原点到平面的向量
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
		//求出当前平面的法线长度
		const Real fn00 = normal.length();
		//求出当前平面与另外一平面的点积
		const Real fn01 = normal.dotProduct(other.normal);
		//另外一平面法线长度
		const Real fn11 = other.normal.length();
		//用克拉默法则解线性方程组
		const Real det = fn00*fn11 - fn01*fn01;

		if (fabs(det) <= std::numeric_limits<Real>::epsilon())
			return false;

		const Real invdet = 1.0 / det;
		const Real fc0 = (fn11*-d + fn01*other.d) * invdet;
		const Real fc1 = (fn00*-other.d + fn01*d) * invdet;

		//两个法向量相乘，得到相交直线的方向向量
		outLineVect = normal.crossProduct(other.normal);
		outLinePoint = normal*fc0 + other.normal*fc1;
		return true;
	}
	//-----------------------------------------------------------------------
	//! 如果有的话，获取与另外两个平面的交点
	bool Plane::getIntersectionWithPlanes(const Plane& o1,
		const Plane& o2, Vector3& outPoint) const 
	{
		Vector3 linePoint, lineVect;
		//获取本平面与o1平面交线
		if (getIntersectionWithPlane(o1, linePoint, lineVect))
		{
			Ray ray = Ray(linePoint, lineVect);
			Plane plane = *this;
			std::pair<bool, Real> t = Math::intersects(ray, plane);
			//如果first 为true，相交，false 平行
			if (t.first)
			{
				//判断o2平面与之前ray的交点
				t = Math::intersects(ray, o2);
				//求出交点
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

