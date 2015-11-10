#ifndef _SAPPHIRE_PLANE_
#define _SAPPHIRE_PLANE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"

namespace Sapphire
{

	
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

		_SapphireExport friend std::ostream& operator<< (std::ostream& o, const Plane& p);
	};

	typedef vector<Plane>::type PlaneList;
	/** @} */
	/** @} */
}

#endif