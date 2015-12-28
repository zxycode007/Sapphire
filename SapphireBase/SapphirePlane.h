#ifndef _SAPPHIRE_PLANE_
#define _SAPPHIRE_PLANE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"


namespace Sapphire
{

	//! 3d对象相交关系枚举
	enum EIntersectionRelation3D
	{
		ISREL3D_FRONT = 0,
		ISREL3D_BACK,
		ISREL3D_PLANAR,
		ISREL3D_SPANNING,
		ISREL3D_CLIPPED
	};
	/** 
	定义了一个3d空间的一个平面
	@remarks
	一个3d空间中的平面方程
	Ax + By + Cz + D = 0
	 
	*/
	class _SapphireExport Plane
	{
	public: 
		 
		Plane();
		Plane(const Plane& rhs);
		/** 通过一个法线和一个一个沿着法线在平面上移动的距离*/
		Plane(const Vector3& rkNormal, Real fConstant);
		/** 用四个常量直接构造一个平面 **/
		Plane(Real a, Real b, Real c, Real d);
		Plane(const Vector3& rkNormal, const Vector3& rkPoint);
		Plane(const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		/** 
		平面的正面是平面法线所指向的半空间，负面这另外一个半空间
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
		返回AligneBox的一平面。这个标志BOTH_SIDE指定一个交界的盒子，
		在平面的角充分考虑到盒子与平面的交界
		*/
		Side getSide(const AxisAlignedBox& rkBox) const;

		/** 
		返回给定盒子的指定平面哪一步，这个盒子由中心/半大小来定义
		@param centre 这盒子的中心
		@param halfSize 这盒子半空间向量 盒子中心->盒子max点
		@return
		POSITIVE_SIDE 如果盒子完全在平面的正面
		NEGATIVE_SIDE 如何盒子在平面的负面
		and BOTH_SIDE 如果盒子交界于两面
		*/
		Side getSide(const Vector3& centre, const Vector3& halfSize) const;

		/** 
		这是一个伪距离。返回值的符号如果点在正面则为正，如点是在负面则为负
		@par
		这个返回值是实际距离的绝对值，仅当平面法线是一个单位向量时
		*/
		Real getDistance(const Vector3& rkPoint) const;

		/** 通过三个点重定义这个平面*/
		void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
			const Vector3& rkPoint2);

		/** 通过一个点和一个法线*/
		void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

		/** 
		投影一个向量到一个平面上
		*/
		Vector3 projectVector(const Vector3& v) const;

		/** 标准化这个平面
		@remarks
		这个方面标准化平面法线长度
		@note
		这个函数不会因为0向量而崩溃，它不会改变他们的分量
		@return 返回平面法线之前的长度
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

		//! 通过矩阵变换一个平面
		void transformPlane(const Matrix4& matrix);
		
		
		//! 求这个平面和另一平面的相交性
		/** \param other 另外一平面
		\param outLinePoint 相交直线的基点
		\param outLineVect 相交直线的向量
		\return 如果相交，返回true，否则返回false*/
		bool getIntersectionWithPlane(const Plane& other,
			Vector3& outLinePoint,
			Vector3& outLineVect) const;
		
		//! 如果有的话，获取两个平面的交点
		bool getIntersectionWithPlanes(const Plane& o1,
			const Plane& o2, Vector3& outPoint) const;
		

		//! 判定点与这个平面的关系
		/** \param point 点
		\return ISREL3D_FRONT 如果点在平面正前面
		ISREL3D_BACK  如果点在平面后面, 
		ISREL3D_PLANAR 如果点在平面上 */
		EIntersectionRelation3D classifyPointRelation(const Vector3& point) const
		{
			const Real d = normal.dotProduct(point) + this->d;

			if (d < -std::numeric_limits<Real>::epsilon())
				return ISREL3D_BACK;

			if (d > std::numeric_limits<Real>::epsilon())
				return ISREL3D_FRONT;

			return ISREL3D_PLANAR;
		}

		//! 获取平面相交的线段分割比例
		/** 只用于有相交发生的情况
		\param linePoint1 线段的1个点
		\param linePoint2 线段的另1个点
		\return 当相交在线段的两点之间的比例，例如：0.5是两点间的中点
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