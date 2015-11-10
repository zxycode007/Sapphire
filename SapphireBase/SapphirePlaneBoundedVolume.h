#ifndef _SAPPHIRE_PLANE_BOUNDED_VOLUME_
#define _SAPPHIRE_PLANE_BOUNDED_VOLUME_

#include "SapphirePrerequisites.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireSphere.h"
#include "SapphireMath.h"
#include "SapphirePlane.h"

namespace Sapphire {

	
	/** ��ʾ͹�߽�ƽ��
	*/
	class _SapphireExport PlaneBoundedVolume
	{
	public:
		typedef vector<Plane>::type PlaneList;
		/// ��������ƽ���б������ֱ���޸�
		PlaneList planes;
		Plane::Side outside;

		PlaneBoundedVolume() :outside(Plane::NEGATIVE_SIDE) {}
		/** ���������������Ǳ���Ϊ�ⲿ */
		PlaneBoundedVolume(Plane::Side theOutside)
			: outside(theOutside) {}

		/**
		��AABB���ཻ�Բ���
		@remarks ���ûͨ������false
		*/
		inline bool intersects(const AxisAlignedBox& box) const
		{
			if (box.isNull()) return false;
			if (box.isInfinite()) return true;

			 
			Vector3 centre = box.getCenter();
			// ��ȡ���Ӱ�ռ�����
			Vector3 halfSize = box.getHalfSize();

			PlaneList::const_iterator i, iend;
			iend = planes.end();
			for (i = planes.begin(); i != iend; ++i)
			{
				const Plane& plane = *i;

				//ȡ��ƽ��
				Plane::Side side = plane.getSide(centre, halfSize);
				if (side == outside)
				{
					// һ�������ƽ�棬���ཻ
					return false;
				}
			}

			// û���ҵ������ƽ�棬�ཻ
			return true;

		}
		/** ��������ཻ�Բ���
		@remarks ���ûͨ������false
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
		�������ཻ���ж�
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