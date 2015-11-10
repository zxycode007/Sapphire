#ifndef _SAPPHIRE_PLANE_BOUNDED_VOLUME_
#define _SAPPHIRE_PLANE_BOUNDED_VOLUME_

#include "SapphirePrerequisites.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireSphere.h"
#include "SapphireMath.h"
#include "SapphirePlane.h"

namespace Sapphire {

	
	/** 表示凸边界平面
	*/
	class _SapphireExport PlaneBoundedVolume
	{
	public:
		typedef vector<Plane>::type PlaneList;
		/// 公共访问平面列表，你可以直接修改
		PlaneList planes;
		Plane::Side outside;

		PlaneBoundedVolume() :outside(Plane::NEGATIVE_SIDE) {}
		/** 构造器，决定半那边作为外部 */
		PlaneBoundedVolume(Plane::Side theOutside)
			: outside(theOutside) {}

		/**
		与AABB的相交性测试
		@remarks 如果没通过返回false
		*/
		inline bool intersects(const AxisAlignedBox& box) const
		{
			if (box.isNull()) return false;
			if (box.isInfinite()) return true;

			 
			Vector3 centre = box.getCenter();
			// 获取盒子半空间向量
			Vector3 halfSize = box.getHalfSize();

			PlaneList::const_iterator i, iend;
			iend = planes.end();
			for (i = planes.begin(); i != iend; ++i)
			{
				const Plane& plane = *i;

				//取得平面
				Plane::Side side = plane.getSide(centre, halfSize);
				if (side == outside)
				{
					// 一个分离的平面，不相交
					return false;
				}
			}

			// 没有找到分离的平面，相交
			return true;

		}
		/** 与球体的相交性测试
		@remarks 如果没通过返回false
		*/
		inline bool intersects(const Sphere& sphere) const
		{
			PlaneList::const_iterator i, iend;
			iend = planes.end();
			for (i = planes.begin(); i != iend; ++i)
			{
				const Plane& plane = *i;

				Real d = plane.getDistance(sphere.getCenter());
				if (outside == Plane::NEGATIVE_SIDE) d = -d;

				if ((d - sphere.getRadius()) > 0)
					return false;
			}

			return true;

		}

		/** 
		如射线相交性判定
		*/
		inline std::pair<bool, Real> intersects(const Ray& ray)
		{
			return Math::intersects(ray, planes, outside == Plane::POSITIVE_SIDE);
		}

	};

	typedef vector<PlaneBoundedVolume>::type PlaneBoundedVolumeList;

	/** @} */
	/** @} */

}

#endif