#ifndef _SAPPHIRE_TRIANGLE3D_
#define _SAPPHIRE_TRIANGLE3D_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireLine3D.h"
#include "SapphirePlane.h"

namespace Sapphire
{
	//! ������ģ����������ײ������������
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

		//! ����������Ƿ���ȫ��һ��AABB������
		/** \param box Ҫ���ĺ���
		\return ����ڷ���true */
		bool isTotalInsideBox(const AxisAlignedBox& box) const
		{
			return (box.contains(pointA) &&
				box.contains(pointB) &&
				box.contains(pointC));
		}

		//! ����������Ƿ���ȫ��һ��AABB��������
		/** \param box Ҫ���ĺ���
		\return ����ں����ⷵ��true */
		bool isTotalOutsideBox(const AxisAlignedBox& box) const
		{
			return ((pointA.x > box.getMaximum().x && pointB.x > box.getMaximum().x && pointC.x > box.getMaximum().x) ||

				(pointA.y > box.getMaximum().y && pointB.y > box.getMaximum().y && pointC.y > box.getMaximum().y) ||
				(pointA.z > box.getMaximum().z && pointB.z > box.getMaximum().z && pointC.z > box.getMaximum().z) ||
				(pointA.x < box.getMinimum().x && pointB.x < box.getMinimum().x && pointC.x < box.getMinimum().x) ||
				(pointA.y < box.getMinimum().y && pointB.y < box.getMinimum().y && pointC.y < box.getMinimum().y) ||
				(pointA.z < box.getMinimum().z && pointB.z < box.getMinimum().z && pointC.z < box.getMinimum().z));
		}

		//! ��ȡ��һ����������һ��ͬ��ƽ���ϵ�һ�������ӽ��ĵ�
		/** \param p ��������������ͬһ��ƽ����
		\return �������������ӽ��ĵ� */
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

		//! ����һ�����Ƿ������������� ���ڱ���Ҳ�㣩
		/*
		\param p ���Ե� �ٶ�������Ѿ��������ε�ƽ����
		\return  ��������������淵��true�����򷵻�false*/
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

		//! ����һ�����Ƿ������������� ���ڱ���Ҳ�㣩�����٣�
		/** �������ʹ����������ϵͳ������isPointInside�����٣����������ܵ��������뾫�ȴ���Ӱ�졣
		���ر�ע����Ǵ�ʱFPU���ڵ�����ģʽ�£�����Direct3DĬ�ϣ�
		\param p ���Ե㣬�ٶ�������Ѿ�����������ε�ƽ����
		\return ��������������淵��true�����򷵻�false */
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

			// ��ȡ��������ϵͳ�������
			const Real invDenom = 1 / (dotAA * dotBB - dotAB * dotAB);
			const Real u = (dotBB * dotAC - dotAB * dotBC) * invDenom;
			const Real v = (dotAA * dotBC - dotAB * dotAC) * invDenom;

			// ���ǰѱ߽��Ҳ�����ڣ����������
			// ���������ҪһЩ��������
			return (u > -EPSILON_E6) && (v >= 0) && (u + v < 1 + EPSILON_E6);

		}


		//! ��ȡ��һ��3dֱ�ߵ��ཻ���ж�
		/** \param line Ҫ�����ཻ�Բ��Ե�ֱ��
		\param outIntersection Ҫ����Ľ��㣬����еĻ�
		\return ������ཻ����true�� ���򷵻�false */
		bool getIntersectionWithLimitedLine(const line3d<T>& line,
			Vector3& outIntersection) const
		{
			return getIntersectionWithLine(line.start,
				line.getVector(), outIntersection) &&
				outIntersection.isBetweenPoints(line.start, line.end);
		}


		//! ��ȡ��һ��3dֱ�ߵ��ཻ���ж�
		/** ��ע�ⷵ�صĵ�����ֱ���ϵ�,������ֻ����ʼ�ͽ�����֮��ĵ㡣
		�������Ҫ��������ʼ��ͽ�����֮�������getIntersectionWithLimitedLine().
		\param linePoint Ҫ�����ཻ���Ե�ֱ�ߵĵ�
		\param lineVect Ҫ�����ཻ���Ե�ֱ�ߵ����� 
		\param outIntersection ���潻�㣬����еĻ�
		\return  ������ཻ����true�� ���򷵻�false*/
		bool getIntersectionWithLine(const Vector3& linePoint,
			const Vector3& lineVect, Vector3& outIntersection) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
				return isPointInside(outIntersection);

			return false;
		}


		//! ����3dֱ�ߺ�����������ƽ����ཻ��
		/** 
		\param linePoint Ҫ�����ཻ���Ե�ֱ�ߵĵ�
		\param lineVect Ҫ�����ཻ���Ե�ֱ�ߵ����� 
		\param outIntersection ���潻�㣬����еĻ�
		\return  ������ཻ����true�� ���򷵻�false*/
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


		//! ��ȡ�����εķ���
		/** ��ע�⣺ �������û�б�׼��*/
		Vector3 getNormal() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA);
		}

		//! ���������ζ�����������滹�Ƿ���
		/** 
		��������ٶ��������Ĺ۲췽��۲�����������ǿɼ���һ�����λ��
		���Ե�λ����ʹ���������������ó�����Ľ��
		\param lookDirection �۲췽�� ���ӹ۲�㵽������ƽ�淽�򣬵��Ϊ��ʱ���棬��Ϊ���档��ʹ�������ƽ��㵽�۲��������෴��
		\return ������ƽ�����泯�򷵻�true��������淵��false */
		bool isFrontFacing(const Vector3& lookDirection) const
		{
			const Vector3 n = getNormal().normalize();
			const Real d = (Real)n.dotProduct(lookDirection);
			return ((d) <= 0.0f);
		}

		//! ��ȡ��������ε�ƽ��
		Plane getPlane() const
		{
			return Plane(pointA, pointB, pointC);
		}

		//! ��ȡ�����ε����
		T getArea() const
		{
			return (pointB - pointA).crossProduct(pointC - pointA).length() * 0.5f;

		}

		//! �������ǵĵ�
		void set(const Vector3& a, const Vector3& b, const Vector3& c)
		{
			pointA = a;
			pointB = b;
			pointC = c;
		}

		//! �����ε�������
		Vector3 pointA;
		Vector3 pointB;
		Vector3 pointC;

	private:
		// �Ƿ���ͬһ��
		bool isOnSameSide(const Vector3& p1, const Vector3& p2,
			const Vector3& a, const Vector3& b) const
		{
			Vector3 bminusa = b - a;
			Vector3 cp1 = bminusa.crossProduct(p1 - a);
			Vector3 cp2 = bminusa.crossProduct(p2 - a);
			Real res = cp1.dotProduct(cp2);
			if (res < 0)
			{
				// ������׽����������
				// ��ϧ�����е㰺��
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