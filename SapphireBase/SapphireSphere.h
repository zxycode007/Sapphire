#ifndef _SAPPHIRE_SPHERE_
#define _SAPPHIRE_SPHERE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"

namespace Sapphire {



	/** 
	一个球体图元，用于碰撞图元
	@remarks
	一个球体图元，用于函数x^2 + y^2 + z^2 = r^2 (对于球体中心在原点)， Sapphire 存放球体中心点并且一个半径
	*/
	class _SapphireExport Sphere
	{
	protected:
		Real mRadius;
		Vector3 mCenter;
	public:
		/** 
		标准构建器， 创建一个环绕的原点单元球体
		*/
		Sphere() : mRadius(1.0), mCenter(Vector3::ZERO) {}
		/** 
		构建任意球体 
		@param center 球体中心
		@param radius 球半径
		*/
		Sphere(const Vector3& center, Real radius)
			: mRadius(radius), mCenter(center) {}

		/** 返回球半径. */
		Real getRadius(void) const { return mRadius; }

		/** 设置球半径 */
		void setRadius(Real radius) { mRadius = radius; }

		/** 返回球中心 */
		const Vector3& getCenter(void) const { return mCenter; }

		/** 设置球中心 */
		void setCenter(const Vector3& center) { mCenter = center; }

		/** 返回当球与另外一个球的相交性判断 */
		bool intersects(const Sphere& s) const
		{
			return (s.mCenter - mCenter).squaredLength() <=
				Math::Sqr(s.mRadius + mRadius);
		}
		/** 返回球与另外一个盒子的相交性判断 */
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
		/** 返回球与另外一个平面的相交性判断 */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
		/** 返回球与一个点相交*/
		bool intersects(const Vector3& v) const
		{
			return ((v - mCenter).squaredLength() <= Math::Sqr(mRadius));
		}
		/** 合并另外的球体到当前球体*/
		void merge(const Sphere& oth)
		{
			Vector3 diff = oth.getCenter() - mCenter;
			Real lengthSq = diff.squaredLength();
			Real radiusDiff = oth.getRadius() - mRadius;

			 
			if (Math::Sqr(radiusDiff) >= lengthSq)
			{
				// 一个完全包含另一个 
				if (radiusDiff <= 0.0f)
					return; // 
				else
				{
					mCenter = oth.getCenter();
					mRadius = oth.getRadius();
					return;
				}
			}

			Real length = Math::Sqrt(lengthSq);

			Vector3 newCenter;
			Real newRadius;
			if ((length + oth.getRadius()) > mRadius)
			{
				Real t = (length + radiusDiff) / (2.0f * length);
				newCenter = mCenter + diff * t;
			}
			// 另外，保存存在的中心

			newRadius = 0.5f * (length + mRadius + oth.getRadius());

			mCenter = newCenter;
			mRadius = newRadius;
		}


	};
	/** @} */
	/** @} */

}


#endif