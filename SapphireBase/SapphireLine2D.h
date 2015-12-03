#ifndef _SAPPHIRE_LINE2D_
#define _SAPPHIRE_LINE2D_

#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"

namespace Sapphire
{
	//! 两个点之间的2d线段和相交方法
	template <class T>
	class line2d
	{
	public:

		line2d() : start(0, 0), end(1, 1) {}

		line2d(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb) {}

		line2d(const Vector2& start, const Vector2& end) : start(start), end(end) {}

		line2d(const line2d<T>& other) : start(other.start), end(other.end) {}



		line2d<T> operator+(const Vector2& point) const { return line2d<T>(start + point, end + point); }
		line2d<T>& operator+=(const Vector2& point) { start += point; end += point; return *this; }

		line2d<T> operator-(const Vector2& point) const { return line2d<T>(start - point, end - point); }
		line2d<T>& operator-=(const Vector2& point) { start -= point; end -= point; return *this; }

		bool operator==(const line2d<T>& other) const
		{
			return (start == other.start && end == other.end) || (end == other.start && start == other.end);
		}
		bool operator!=(const line2d<T>& other) const
		{
			return !(start == other.start && end == other.end) || (end == other.start && start == other.end);
		}

		 
		void setLine(const T& xa, const T& ya, const T& xb, const T& yb)
		{
			start = Vector2(xa, ya);
			end = Vector2(xb, yb);
		}
	 
		void setLine(const Vector2& nstart, const Vector2& nend)
		{
			start = nstart;
			end = nend;
		}
		 
		void setLine(const line2d<T>& line){ start = line.start; end = line.end; }

		//! 获取线段长度
		/** \return 线段长度 */
		T getLength() const { return start.distance(end); }

		//! 获取线段长度的平方（避免开方）
		/** \return 线段长度的平方 */
		T getLengthSQ() const { return start.squaredDistance(end); }

		//! 获取线段中点
		/** \return 线段中点. */
		Vector2 getMiddle() const
		{
			return (start + end) / (T)2;
		}

		//! 获取线段向量
		/** \return 线段向量 */
		Vector2 getVector() const { return Vector2(end.x - start.x, end.y - start.y); }

		//! 测试这条是否与另一条线段相交
		/** \param l: 要测试的另一条线段
		\param checkOnlySegments: 默认在起始点和结束点之间检测的相交性。当设置为false，这个函数会检查超出线段范围的首个交点
		\param out: 如果有一个交点，这个向量保存位置
		\return 如果有交点返回true*/
		bool intersectWith(const line2d<T>& l, Vector2& out, bool checkOnlySegments = true) const
		{
			 
			const Real commonDenominator = (Real)(l.end.y - l.start.y)*(end.x - start.x) -
				(l.end.x - l.start.x)*(end.y - start.y);

			const Real numeratorA = (Real)(l.end.x - l.start.x)*(start.y - l.start.y) -
				(l.end.y - l.start.y)*(start.x - l.start.x);

			const Real numeratorB = (Real)(end.x - start.x)*(start.y - l.start.y) -
				(end.y - start.y)*(start.x - l.start.x);

			if (Math::equals(commonDenominator, 0.f))
			{
				// 这线段要么重合要么平行
				// 如果两个分子都是0，它们重合
				if (Math::equals(numeratorA, 0.f) && Math::equals(numeratorB, 0.f))
				{
					// 首先尝试找到一个公共终点
					if (l.start == start || l.end == start)
						out = start;
					else if (l.end == end || l.start == end)
						out = end;
					// 现在检查两个线段是否是间断的
					else if (l.start.x > start.x && l.end.x > start.x && l.start.x > end.x && l.end.x > end.x)
						return false;
					else if (l.start.y > start.y && l.end.y > start.y && l.start.y > end.y && l.end.y > end.y)
						return false;
					else if (l.start.x < start.x && l.end.x<start.x && l.start.x < end.x && l.end.x < end.x)
						return false;
					else if (l.start.y < start.y && l.end.y < start.y && l.start.y<end.y && l.end.y<end.y)
						return false;
					// 否则这些线段某些程度是重合的 
					else
					{
						// 找到这个与公共部分不相干的点
						Vector2 maxp;
						Vector2 minp;
						if ((start.x>l.start.x && start.x>l.end.x && start.x > end.x) || (start.y > l.start.y && start.y > l.end.y && start.y > end.y))
							maxp = start;
						else if ((end.x>l.start.x && end.x > l.end.x && end.x > start.x) || (end.y > l.start.y && end.y > l.end.y && end.y > start.y))
							maxp = end;
						else if ((l.start.x > start.x && l.start.x > l.end.x && l.start.x > end.x) || (l.start.y > start.y && l.start.y > l.end.y && l.start.y > end.y))
							maxp = l.start;
						else
							maxp = l.end;
						if (maxp != start && ((start.x < l.start.x && start.x < l.end.x && start.x < end.x) || (start.y < l.start.y && start.y < l.end.y && start.y < end.y)))
							minp = start;
						else if (maxp != end && ((end.x < l.start.x && end.x < l.end.x && end.x < start.x) || (end.y < l.start.y && end.y < l.end.y && end.y < start.y)))
							minp = end;
						else if (maxp != l.start && ((l.start.x < start.x && l.start.x < l.end.x && l.start.x < end.x) || (l.start.y < start.y && l.start.y < l.end.y && l.start.y < end.y)))
							minp = l.start;
						else
							minp = l.end;

						// 一个线段包含另一个，拾取确定重叠剩余部分的中心点
						out =  Vector2();
						if (start != maxp && start != minp)
							out += start;
						if (end != maxp && end != minp)
							out += end;
						if (l.start != maxp && l.start != minp)
							out += l.start;
						if (l.end != maxp && l.end != minp)
							out += l.end;
						out.x = (T)(out.x / 2);
						out.y = (T)(out.y / 2);
					}

					return true; // 重合
				}

				return false; // 平行
			}

			// 获取这个点在这个条直线上相交性， 剪裁它是否在线段中
			const Real uA = numeratorA / commonDenominator;
			if (checkOnlySegments && (uA < 0.f || uA > 1.f))
				return false; //不在这条线段上

			const Real uB = numeratorB / commonDenominator;
			if (checkOnlySegments && (uB < 0.f || uB > 1.f))
				return false; //不在这条线段上

			// 计算交点
			out.x = (T)(start.x + uA * (end.x - start.x));
			out.y = (T)(start.y + uA * (end.y - start.y));
			return true;
		}

		//! 获取这条线段的单位向量
		/** \return 线段的单位向量 */
		Vector2 getUnitVector() const
		{
			T len = (T)(1.0 / getLength());
			return Vector2((end.x - start.x) * len, (end.y - start.y) * len);
		}

		//! 获取这条线段和给定线段间的交点
		/** \param l 要测试的另一条线段
		\return 角度值 */
		Radian getAngleWith(const line2d<T>& l) const
		{
			Vector2 vect = getVector();
			Vector2 vect2 = l.getVector();
			//默认都是按弧度值计算
			return vect.angleBetween(vect2);
		}

		//! 告诉我们这个给定点是在线段左边还是右边，或者是在直线上
		/** \return 如果返回0是在直线上，小于0在左边， 大于0在右边 */
		T getPointOrientation(const Vector2& point) const
		{
			return ((end.x - start.x) * (point.y - start.y) -
				(point.x - start.x) * (end.y - start.y));
		}

		//! 检测如果给定点在线段上
		/** \return 如果在线段的起始和结束之间返回true*/
		bool isPointOnLine(const Vector2& point) const
		{
			T d = getPointOrientation(point);
			return (d == 0 && point.isBetweenPoints(start, end));
		}

		//! 检测给定点在线段的开始和结束点之间
		/** 假定这个点已经在这条直线上 */
		bool isPointBetweenStartAndEnd(const Vector2& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//!获取这条线段上到与一个点的最接近的点
		/** \param checkOnlySegments: 默认true 返回这条线段（起始和结束点之间）的点
		当设为false，返回直线上的点（超出线段外）*/
		Vector2 getClosestPoint(const Vector2& point, bool checkOnlySegments = true) const
		{
			Vector2 c((Real)(point.x - start.x), (Real)(point.y - start.y));
			Vector2 v((Real)(end.x - start.x), (Real)(end.y - start.y));
			Real d = v.length();
			if (d == 0)	// 判断这条线段是否退化成单个点
				return start;
			v /= d;
			Real t = v.dotProduct(c);

			if (checkOnlySegments)
			{
				if (t < 0) return Vector2((T)start.x, (T)start.y);
				if (t > d) return Vector2((T)end.x, (T)end.y);
			}

			v *= t;
			return Vector2((T)(start.x + v.x), (T)(start.y + v.y));
		}

		//! 线段的起始点
		Vector2 start;
		//! 线段的终点
		Vector2 end;
	};

	// 部分特别优化的线段 （避免转换）
	template <>
	inline Vector2 line2d<Real>::getClosestPoint(const Vector2& point, bool checkOnlySegments) const
	{
		Vector2 c = point - start;
		Vector2 v = end - start;
		Real d = (Real)v.length();
		if (d == 0)		// 判断这条线段是否退化成单个点
			return start;
		v /= d;
		Real t = v.dotProduct(c);

		if (checkOnlySegments)
		{
			if (t < 0) return start;
			if (t > d) return end;
		}

		v *= t;
		return start + v;
	}


	//! 浮点型2d线段
	typedef line2d<Real> line2df;
	//! 整形2d线段
	typedef line2d<SINT32> line2di;
}

#endif