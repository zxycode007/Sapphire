#ifndef _SAPPHIRE_RAY_
#define _SAPPHIRE_RAY_

#include "SapphirePrerequisites.h"
#include "SapphirePlaneBoundedVolume.h"
#include "SapphireVector3.h"




namespace Sapphire 
{

	 
	/** 表示空间中的一条射线，一条由源到目的的直线*/
	class _SapphireExport Ray
	{
	protected:
		Vector3 mOrigin;
		Vector3 mDirection;
	public:
		Ray() :mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Z) {}
		Ray(const Vector3& origin, const Vector3& direction)
			:mOrigin(origin), mDirection(direction) {}

		/** 设置这条射线的原点 */
		void setOrigin(const Vector3& origin) { mOrigin = origin; }
		/** 取得这条射线的原点 */
		const Vector3& getOrigin(void) const { return mOrigin; }

		/** 设置这条射线的方向 */
		void setDirection(const Vector3& dir) { mDirection = dir; }
		/** 取得这条射线的方向*/
		const Vector3& getDirection(void) const { return mDirection; }

		/** 取得沿这条射线t单位位置的点*/
		Vector3 getPoint(Real t) const {
			return Vector3(mOrigin + (mDirection * t));
		}

		/** 取得沿这条射线t单位位置的点 */
		Vector3 operator*(Real t) const {
			return getPoint(t);
		}

		/** 
		测试射线与指定平面是否相交
		@return 返回一个pair结果，首个元素指向是否有相交发生，如果为true，第二个元素将指定相交处沿直线距离
		它可以通过getPoint()被转换为空间内的一个点 
		*/
		std::pair<bool, Real> intersects(const Plane& p) const
		{
			return Math::intersects(*this, p);
		}
		/**
		测试射线与指定平面碰撞容积是否相交
		@return 返回一个pair结果，首个元素指向是否有相交发生，如果为true，第二个元素将指定相交处沿直线距离
		它可以通过getPoint()被转换为空间内的一个点
		*/
		std::pair<bool, Real> intersects(const PlaneBoundedVolume& p) const
		{
			return Math::intersects(*this, p.planes, p.outside == Plane::POSITIVE_SIDE);
		}
		/**
		测试射线与指定球体是否相交
		@return 返回一个pair结果，首个元素指向是否有相交发生，如果为true，第二个元素将指定相交处沿直线距离
		它可以通过getPoint()被转换为空间内的一个点
		*/
		std::pair<bool, Real> intersects(const Sphere& s) const
		{
			return Math::intersects(*this, s);
		}
		/**
		测试射线与指定AABB是否相交
		@return 返回一个pair结果，首个元素指向是否有相交发生，如果为true，第二个元素将指定相交处沿直线距离
		它可以通过getPoint()被转换为空间内的一个点
		*/
		std::pair<bool, Real> intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}

	};


}

#endif