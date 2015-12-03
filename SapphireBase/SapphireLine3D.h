#ifndef _SAPPHIRE_LINE3D_
#define _SAPPHIRE_LINE3D_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireMath.h"

namespace Sapphire
{
	//! 两个点之间的3d线段
	template <class T>
	class line3d
	{
	public:

		//! 默认构造函数
		/** 从(0,0,0) 到 (1,1,1)的线段 */
		line3d() : start(0, 0, 0), end(1, 1, 1) {}
		 
		line3d(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb) {}
		 
		line3d(const Vector3& start, const Vector3& end) : start(start), end(end) {}

		 

		line3d<T> operator+(const Vector3& point) const { return line3d<T>(start + point, end + point); }
		line3d<T>& operator+=(const Vector3& point) { start += point; end += point; return *this; }

		line3d<T> operator-(const Vector3& point) const { return line3d<T>(start - point, end - point); }
		line3d<T>& operator-=(const Vector3& point) { start -= point; end -= point; return *this; }

		bool operator==(const line3d<T>& other) const
		{
			return (start == other.start && end == other.end) || (end == other.start && start == other.end);
		}
		bool operator!=(const line3d<T>& other) const
		{
			return !(start == other.start && end == other.end) || (end == other.start && start == other.end);
		}

		 
	 
		void setLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb)
		{
			start = Vector3(xa, ya, za); end = Vector3(xb, yb, zb);
		}
	 
		void setLine(const Vector3& nstart, const Vector3& nend)
		{
			start = nstart; end = nend;
		}
		 
		void setLine(const line3d<T>& line)
		{
			start = line.start;
			end = line.end;
		}

		//! 获取线段长度
		/** \return 线段长度 */
		T getLength() const { return start.distance(end); }

		//! 获取线段长度的平方（避免开方）
		/** \return 线段长度的平方 */
		T getLengthSQ() const { return start.squaredDistance(end); }

		//! 获取线段中点
		/** \return 线段中点. */
		Vector3 getMiddle() const
		{
			return (start + end) / (T)2;
		}

		//! 获取线段向量
		/** \return 线段向量 */
		Vector3 getVector() const
		{
			return end - start;
		}

		//! 是否给定点在这条线段的起始和结尾点之间
		/** 假定这个点以及在这条线段的某处
		\param point 测试点
		\return True 如果这个点在线段起始和结尾之间返回true，否则返回false
		*/
		bool isPointBetweenStartAndEnd(const Vector3& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//! 获取这条线段上到与一个点的最接近的点
		/** \param point 要比较的点
		\return 最接近的点 */
		Vector3 getClosestPoint(const Vector3& point) const
		{
			Vector3 c = point - start;
			Vector3 v = end - start;
			T d = (T)v.length();
			v /= d;
			T t = v.dotProduct(c);

			if (t < (T)0.0)
				return start;
			if (t > d)
				return end;

			v *= t;
			return start + v;
		}

		//! 检测这条线段是否与一个球体相交
		/** \param sorigin: 球体原点
		\param sradius: 球体半径
		\param outdistance: 到第一个相交点的距离
		\return 如果为ture相交
		如果有交点的话，到第一个相交点的距离存放在outdistance中
		  */
		bool getIntersectionWithSphere(Vector3 sorigin, T sradius, Real& outdistance) const
		{
			const Vector3 q = sorigin - start;
			T c = q.length();
			T v = q.dotProduct(getVector().normalise());
			T d = sradius * sradius - (c*c - v*v);

			if (d < 0.0)
				return false;

			outdistance = v - Math::Sqr(d);
			return true;
		}

		// 成员函数

		//! 线段起始点
		Vector3 start;
		//! 线段结束点
		Vector3 end;
	};

	//! Typedef for an Real line.
	typedef line3d<Real> line3df;
	//! Typedef for an integer line.
	typedef line3d<SINT32> line3di;
}

#endif 