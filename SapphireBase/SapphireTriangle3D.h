#ifndef _SAPPHIRE_TRIANGLE3D_
#define _SAPPHIRE_TRIANGLE3D_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireLine3D.h"
#include "SapphirePlane.h"

namespace Sapphire
{
	//! 三角形模板类用于碰撞检测和其它事情
	template <class T>
	class triangle3d
	{
	public:

		
		triangle3d() {}
	 
		triangle3d(Vector3 v1, Vector3 v2, Vector3 v3) : pointA(v1), pointB(v2), pointC(v3) {}

	 
		bool operator==(const triangle3d<T>& other) const
		{
			return other.pointA == pointA && other.pointB == pointB && other.pointC == pointC;
		}

		 
		bool operator!=(const triangle3d<T>& other) const
		{
			return !(*this == other);
		}

		//! 检测三角形是否完全在一个AABB盒子中
		/** \param box 要检查的盒子
		\return 如果在返回true */
		bool isTotalInsideBox(const AxisAlignedBox& box) const
		{
			return (box.contains(pointA) &&
				box.contains(pointB) &&
				box.contains(pointC));
		}

		//! 检查三角形是否完全在一个AABB盒子外面
		/** \param box 要检查的盒子
		\return 如果在盒子外返回true */
		bool isTotalOutsideBox(const AxisAlignedBox& box) const
		{
			return ((pointA.x > box.getMaximum().x && pointB.x > box.getMaximum().x && pointC.x > box.getMaximum().x) ||

				(pointA.y > box.getMaximum().y && pointB.y > box.getMaximum().y && pointC.y > box.getMaximum().y) ||
				(pointA.z > box.getMaximum().z && pointB.z > box.getMaximum().z && pointC.z > box.getMaximum().z) ||
				(pointA.x < box.getMinimum().x && pointB.x < box.getMinimum().x && pointC.x < box.getMinimum().x) ||
				(pointA.y < box.getMinimum().y && pointB.y < box.getMinimum().y && pointC.y < box.getMinimum().y) ||
				(pointA.z < box.getMinimum().z && pointB.z < box.getMinimum().z && pointC.z < box.getMinimum().z));
		}

		//! 获取和一个三角形在一个同样平面上的一个点的最接近的点
		/** \param p 这个点和三角形在同一个平面上
		\return 这个三角形上最接近的点 */
		 Vector3 closestPointOnTriangle(const  Vector3& p) const
		{
			const  Vector3 rab = line3d<Real>(pointA, pointB).getClosestPoint(p);
			const  Vector3 rbc = line3d<Real>(pointB, pointC).getClosestPoint(p);
			const  Vector3 rca = line3d<Real>(pointC, pointA).getClosestPoint(p);

			const Real d1 = rab.distance(p);
			const Real d2 = rbc.distance(p);
			const Real d3 = rca.distance(p);

			if (d1 < d2)
				return d1 < d3 ? rab : rca;

			return d2 < d3 ? rbc : rca;
		}

		//! 测试一个点是否在三角形里面 （在边上也算）
		/*
		\param p 测试点 假定这个点已经在三角形的平面上
		\return  如果在三角形里面返回true，否则返回false*/
		bool isPointInside(const Vector3& p) const
		{
			Vector3 af64((Real)pointA.x, (Real)pointA.y, (Real)pointA.z);
			Vector3 bf64((Real)pointB.x, (Real)pointB.y, (Real)pointB.z);
			Vector3 cf64((Real)pointC.x, (Real)pointC.y, (Real)pointC.z);
			Vector3 pf64((Real)p.x, (Real)p.y, (Real)p.z);
			return (isOnSameSide(pf64, af64, bf64, cf64) &&
				isOnSameSide(pf64, bf64, af64, cf64) &&
				isOnSameSide(pf64, cf64, af64, bf64));
		}

		//! 测试一个点是否在三角形里面 （在边上也算）（快速）
		/** 这个方法使用重心坐标系统。它比isPointInside更快速，但是容易受到浮点舍入精度错误影响。
		将特别注意的是此时FPU是在单精度模式下（这是Direct3D默认）
		\param p 测试点，假定这个点已经在这个三角形的平面上
		\return 如果在三角形里面返回true，否则返回false */
		bool isPointInsideFast(const Vector3& p) const
		{
			const Vector3 a = pointC - pointA;
			const Vector3 b = pointB - pointA;
			const Vector3 c = p - pointA;

			const Real dotAA = a.dotProduct(a);
			const Real dotAB = a.dotProduct(b);
			const Real dotAC = a.dotProduct(c);
			const Real dotBB = b.dotProduct(b);
			const Real dotBC = b.dotProduct(c);

			// 获取重心坐标系统里的坐标
			const Real invDenom = 1 / (dotAA * dotBB - dotAB * dotAB);
			const Real u = (dotBB * dotAC - dotAB * dotBC) * invDenom;
			const Real v = (dotAA * dotBC - dotAB * dotAC) * invDenom;

			// 我们把边界点也算在内，保存兼容性
			// 舍入错误需要一些测试用例
			return (u > -EPSILON_E6) && (v >= 0) && (u + v < 1 + EPSILON_E6);

		}


		//! 获取和一条3d直线的相交性判断
		/** \param line 要进行相交性测试的直线
		\param outIntersection 要保存的交点，如果有的话
		\return 如果有相交返回true， 否则返回false */
		bool getIntersectionWithLimitedLine(const line3d<T>& line,
			Vector3& outIntersection) const
		{
			return getIntersectionWithLine(line.start,
				line.getVector(), outIntersection) &&
				outIntersection.isBetweenPoints(line.start, line.end);
		}


		//! 获取和一条3d直线的相交性判断
		/** 请注意返回的点是在直线上的,但并非只是起始和结束点之间的点。
		如果你想要返回在起始点和结束点之间的请用getIntersectionWithLimitedLine().
		\param linePoint 要用于相交测试的直线的点
		\param lineVect 要用于相交测试的直线的向量 
		\param outIntersection 保存交点，如果有的话
		\return  如果有相交返回true， 否则返回false*/
		bool getIntersectionWithLine(const Vector3& linePoint,
			const Vector3& lineVect, Vector3& outIntersection) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
				return isPointInside(outIntersection);

			return false;
		}


		//! 计算3d直线和三角形所在平面的相交性
		/** 
		\param linePoint 要用于相交测试的直线的点
		\param lineVect 要用于相交测试的直线的向量 
		\param outIntersection 保存交点，如果有的话
		\return  如果有相交返回true， 否则返回false*/
		bool getIntersectionOfPlaneWithLine(const Vector3& linePoint,
			const Vector3& lineVect, Vector3& outIntersection) const
		{
			const Vector3 linePointf(linePoint.x, linePoint.y, linePoint.z);
			const Vector3 lineVectf(lineVect.x, lineVect.y, lineVect.z);
			Vector3 outIntersectionf;

			triangle3d<Real> trianglef(Vector3((Real)pointA.x, (Real)pointA.y, (Real)pointA.z)
				, Vector3((Real)pointB.x, (Real)pointB.y, (Real)pointB.z)
				, Vector3((Real)pointC.x, (Real)pointC.y, (Real)pointC.z));
 			const Vector3 normalf = trianglef.getNormal().normalize();
			Real t2;

			if ( Math::iszero(t2 = normalf.dotProduct(lineVectf)))
				return false;

			Real d = trianglef.pointA.dotProduct(normalf);
			Real t = -(normalf.dotProduct(linePointf) - d) / t2;
			outIntersectionf = linePointf + (lineVectf * t);

			outIntersection.x = (T)outIntersectionf.x;
			outIntersection.y = (T)outIntersectionf.y;
			outIntersection.z = (T)outIntersectionf.z;
			return true;
		}


		//! 获取三角形的法线
		/** 请注意： 这个法线没有标准化*/
		Vector3 getNormal() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA);
		}

		//! 测试三角形对于任意点正面还是反面
		/** 
		这个方法假定当给定的观察方向观察这个三角形是可见的一个相机位置
		别以点位置来使用这个方法，它会得出错误的结果
		\param lookDirection 观察方向 （从观察点到三角形平面方向，点积为负时正面，正为反面。这和从三角形平面点到观察点点积结果相反）
		\return 如果这个平面正面朝向返回true，如果反面返回false */
		bool isFrontFacing(const Vector3& lookDirection) const
		{
			const Vector3 n = getNormal().normalize();
			const Real d = (Real)n.dotProduct(lookDirection);
			return ((d) <= 0.0f);
		}

		//! 获取这个三角形的平面
		Plane getPlane() const
		{
			return Plane(pointA, pointB, pointC);
		}

		//! 获取三角形的面积
		T getArea() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA).length() * 0.5f;

		}

		//! 设置三角的点
		void set(const Vector3& a, const Vector3& b, const Vector3& c)
		{
			pointA = a;
			pointB = b;
			pointC = c;
		}

		//! 三角形的三个点
		Vector3 pointA;
		Vector3 pointB;
		Vector3 pointC;

	private:
		// 是否在同一侧
		bool isOnSameSide(const Vector3& p1, const Vector3& p2,
			const Vector3& a, const Vector3& b) const
		{
			Vector3 bminusa = b - a;
			Vector3 cp1 = bminusa.crossProduct(p1 - a);
			Vector3 cp2 = bminusa.crossProduct(p2 - a);
			Real res = cp1.dotProduct(cp2);
			if (res < 0)
			{
				// 这是扑捉浮点数问题
				// 可惜代价有点昂贵，
				Vector3 cp1 = bminusa.normalisedCopy().crossProduct((p1 - a).normalisedCopy());
				if (Math::iszero(cp1.x)
					&& Math::iszero(cp1.y)
					&& Math::iszero(cp1.z))
				{
					res = 0.f;
				}
			}
			return (res >= 0.0f);
		}
	};

	typedef triangle3d<Real> triangle3df;
}



#endif