#ifndef _SAPPHIRE_SPHERE_
#define _SAPPHIRE_SPHERE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"

namespace Sapphire {



	/** 
	һ������ͼԪ��������ײͼԪ
	@remarks
	һ������ͼԪ�����ں���x^2 + y^2 + z^2 = r^2 (��������������ԭ��)�� Sapphire ����������ĵ㲢��һ���뾶
	*/
	class _SapphireExport Sphere
	{
	protected:
		Real mRadius;
		Vector3 mCenter;
	public:
		/** 
		��׼�������� ����һ�����Ƶ�ԭ�㵥Ԫ����
		*/
		Sphere() : mRadius(1.0), mCenter(Vector3::ZERO) {}
		/** 
		������������ 
		@param center ��������
		@param radius ��뾶
		*/
		Sphere(const Vector3& center, Real radius)
			: mRadius(radius), mCenter(center) {}

		/** ������뾶. */
		Real getRadius(void) const { return mRadius; }

		/** ������뾶 */
		void setRadius(Real radius) { mRadius = radius; }

		/** ���������� */
		const Vector3& getCenter(void) const { return mCenter; }

		/** ���������� */
		void setCenter(const Vector3& center) { mCenter = center; }

		/** ���ص���������һ������ཻ���ж� */
		bool intersects(const Sphere& s) const
		{
			return (s.mCenter - mCenter).squaredLength() <=
				Math::Sqr(s.mRadius + mRadius);
		}
		/** ������������һ�����ӵ��ཻ���ж� */
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
		/** ������������һ��ƽ����ཻ���ж� */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
		/** ��������һ�����ཻ*/
		bool intersects(const Vector3& v) const
		{
			return ((v - mCenter).squaredLength() <= Math::Sqr(mRadius));
		}
		/** �ϲ���������嵽��ǰ����*/
		void merge(const Sphere& oth)
		{
			Vector3 diff = oth.getCenter() - mCenter;
			Real lengthSq = diff.squaredLength();
			Real radiusDiff = oth.getRadius() - mRadius;

			 
			if (Math::Sqr(radiusDiff) >= lengthSq)
			{
				// һ����ȫ������һ�� 
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
			// ���⣬������ڵ�����

			newRadius = 0.5f * (length + mRadius + oth.getRadius());

			mCenter = newCenter;
			mRadius = newRadius;
		}


	};
	/** @} */
	/** @} */

}


#endif