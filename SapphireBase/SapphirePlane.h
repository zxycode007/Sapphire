#ifndef _SAPPHIRE_PLANE_
#define _SAPPHIRE_PLANE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"


namespace Sapphire
{

	//! 3d�����ཻ��ϵö��
	enum EIntersectionRelation3D
	{
		ISREL3D_FRONT = 0,
		ISREL3D_BACK,
		ISREL3D_PLANAR,
		ISREL3D_SPANNING,
		ISREL3D_CLIPPED
	};
	/** 
	������һ��3d�ռ��һ��ƽ��
	@remarks
	һ��3d�ռ��е�ƽ�淽��
	Ax + By + Cz + D = 0
	 
	*/
	class _SapphireExport Plane
	{
	public: 
		 
		Plane();
		Plane(const Plane& rhs);
		/** ͨ��һ�����ߺ�һ��һ�����ŷ�����ƽ�����ƶ��ľ���*/
		Plane(const Vector3& rkNormal, Real fConstant);
		/** ���ĸ�����ֱ�ӹ���һ��ƽ�� **/
		Plane(Real a, Real b, Real c, Real d);
		Plane(const Vector3& rkNormal, const Vector3& rkPoint);
		Plane(const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		/** 
		ƽ���������ƽ�淨����ָ��İ�ռ䣬����������һ����ռ�
		*/
		enum Side
		{
			NO_SIDE,
			POSITIVE_SIDE,
			NEGATIVE_SIDE,
			BOTH_SIDE
		};

		Side getSide(const Vector3& rkPoint) const;

		/**
		����AligneBox��һƽ�档�����־BOTH_SIDEָ��һ������ĺ��ӣ�
		��ƽ��Ľǳ�ֿ��ǵ�������ƽ��Ľ���
		*/
		Side getSide(const AxisAlignedBox& rkBox) const;

		/** 
		���ظ������ӵ�ָ��ƽ����һ�����������������/���С������
		@param centre ����ӵ�����
		@param halfSize ����Ӱ�ռ����� ��������->����max��
		@return
		POSITIVE_SIDE ���������ȫ��ƽ�������
		NEGATIVE_SIDE ��κ�����ƽ��ĸ���
		and BOTH_SIDE ������ӽ���������
		*/
		Side getSide(const Vector3& centre, const Vector3& halfSize) const;

		/** 
		����һ��α���롣����ֵ�ķ����������������Ϊ����������ڸ�����Ϊ��
		@par
		�������ֵ��ʵ�ʾ���ľ���ֵ������ƽ�淨����һ����λ����ʱ
		*/
		Real getDistance(const Vector3& rkPoint) const;

		/** ͨ���������ض������ƽ��*/
		void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		/** ͨ��һ�����һ������*/
		void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

		/** 
		ͶӰһ��������һ��ƽ����
		*/
		Vector3 projectVector(const Vector3& v) const;

		/** ��׼�����ƽ��
		@remarks
		��������׼��ƽ�淨�߳���
		@note
		�������������Ϊ0������������������ı����ǵķ���
		@return ����ƽ�淨��֮ǰ�ĳ���
		*/
		Real normalise(void);

		Vector3 normal;
		Real d;

		///  
		bool operator==(const Plane& rhs) const
		{
			return (rhs.d == d && rhs.normal == normal);
		}
		bool operator!=(const Plane& rhs) const
		{
			return (rhs.d != d || rhs.normal != normal);
		}

		//! ͨ������任һ��ƽ��
		void transformPlane(const Matrix4& matrix);
		
		
		//! �����ƽ�����һƽ����ཻ��
		/** \param other ����һƽ��
		\param outLinePoint �ֱཻ�ߵĻ���
		\param outLineVect �ֱཻ�ߵ�����
		\return ����ཻ������true�����򷵻�false*/
		bool getIntersectionWithPlane(const Plane& other,
			Vector3& outLinePoint,
			Vector3& outLineVect) const;
		
		//! ����еĻ�����ȡ����ƽ��Ľ���
		bool getIntersectionWithPlanes(const Plane& o1,
			const Plane& o2, Vector3& outPoint) const;
		

		//! �ж��������ƽ��Ĺ�ϵ
		/** \param point ��
		\return ISREL3D_FRONT �������ƽ����ǰ��
		ISREL3D_BACK  �������ƽ�����, 
		ISREL3D_PLANAR �������ƽ���� */
		EIntersectionRelation3D classifyPointRelation(const Vector3& point) const
		{
			const Real d = normal.dotProduct(point) + this->d;

			if (d < -std::numeric_limits<Real>::epsilon())
				return ISREL3D_BACK;

			if (d > std::numeric_limits<Real>::epsilon())
				return ISREL3D_FRONT;

			return ISREL3D_PLANAR;
		}

		//! ��ȡƽ���ཻ���߶ηָ����
		/** ֻ�������ཻ���������
		\param linePoint1 �߶ε�1����
		\param linePoint2 �߶ε���1����
		\return ���ཻ���߶ε�����֮��ı��������磺0.5���������е�
		*/
		Real getKnownIntersectionWithLine(const  Vector3& linePoint1,
			const Vector3& linePoint2) const
		{
			Vector3 vect = linePoint2 - linePoint1;
			Real t2 = (Real)normal.dotProduct(vect);
			return (Real)-((normal.dotProduct(linePoint1) + d) / t2);
		}

		_SapphireExport friend std::ostream& operator<< (std::ostream& o, const Plane& p);
	};

	typedef vector<Plane>::type PlaneList;
	/** @} */
	/** @} */
}

#endif