#ifndef _SAPPHIRE_RAY_
#define _SAPPHIRE_RAY_

#include "SapphirePrerequisites.h"
#include "SapphirePlaneBoundedVolume.h"
#include "SapphireVector3.h"




namespace Sapphire 
{

	 
	/** ��ʾ�ռ��е�һ�����ߣ�һ����Դ��Ŀ�ĵ�ֱ��*/
	class _SapphireExport Ray
	{
	protected:
		Vector3 mOrigin;
		Vector3 mDirection;
	public:
		Ray() :mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Z) {}
		Ray(const Vector3& origin, const Vector3& direction)
			:mOrigin(origin), mDirection(direction) {}

		/** �����������ߵ�ԭ�� */
		void setOrigin(const Vector3& origin) { mOrigin = origin; }
		/** ȡ���������ߵ�ԭ�� */
		const Vector3& getOrigin(void) const { return mOrigin; }

		/** �����������ߵķ��� */
		void setDirection(const Vector3& dir) { mDirection = dir; }
		/** ȡ���������ߵķ���*/
		const Vector3& getDirection(void) const { return mDirection; }

		/** ȡ������������t��λλ�õĵ�*/
		Vector3 getPoint(Real t) const {
			return Vector3(mOrigin + (mDirection * t));
		}

		/** ȡ������������t��λλ�õĵ� */
		Vector3 operator*(Real t) const {
			return getPoint(t);
		}

		/** 
		����������ָ��ƽ���Ƿ��ཻ
		@return ����һ��pair������׸�Ԫ��ָ���Ƿ����ཻ���������Ϊtrue���ڶ���Ԫ�ؽ�ָ���ཻ����ֱ�߾���
		������ͨ��getPoint()��ת��Ϊ�ռ��ڵ�һ���� 
		*/
		std::pair<bool, Real> intersects(const Plane& p) const
		{
			return Math::intersects(*this, p);
		}
		/**
		����������ָ��ƽ����ײ�ݻ��Ƿ��ཻ
		@return ����һ��pair������׸�Ԫ��ָ���Ƿ����ཻ���������Ϊtrue���ڶ���Ԫ�ؽ�ָ���ཻ����ֱ�߾���
		������ͨ��getPoint()��ת��Ϊ�ռ��ڵ�һ����
		*/
		std::pair<bool, Real> intersects(const PlaneBoundedVolume& p) const
		{
			return Math::intersects(*this, p.planes, p.outside == Plane::POSITIVE_SIDE);
		}
		/**
		����������ָ�������Ƿ��ཻ
		@return ����һ��pair������׸�Ԫ��ָ���Ƿ����ཻ���������Ϊtrue���ڶ���Ԫ�ؽ�ָ���ཻ����ֱ�߾���
		������ͨ��getPoint()��ת��Ϊ�ռ��ڵ�һ����
		*/
		std::pair<bool, Real> intersects(const Sphere& s) const
		{
			return Math::intersects(*this, s);
		}
		/**
		����������ָ��AABB�Ƿ��ཻ
		@return ����һ��pair������׸�Ԫ��ָ���Ƿ����ཻ���������Ϊtrue���ڶ���Ԫ�ؽ�ָ���ཻ����ֱ�߾���
		������ͨ��getPoint()��ת��Ϊ�ռ��ڵ�һ����
		*/
		std::pair<bool, Real> intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}

	};


}

#endif