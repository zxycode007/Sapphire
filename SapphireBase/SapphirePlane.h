#ifndef _SAPPHIRE_PLANE_
#define _SAPPHIRE_PLANE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"

namespace Sapphire
{

	
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

		_SapphireExport friend std::ostream& operator<< (std::ostream& o, const Plane& p);
	};

	typedef vector<Plane>::type PlaneList;
	/** @} */
	/** @} */
}

#endif