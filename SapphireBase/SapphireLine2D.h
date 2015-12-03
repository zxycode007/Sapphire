#ifndef _SAPPHIRE_LINE2D_
#define _SAPPHIRE_LINE2D_

#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"

namespace Sapphire
{
	//! ������֮���2d�߶κ��ཻ����
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

		//! ��ȡ�߶γ���
		/** \return �߶γ��� */
		T getLength() const { return start.distance(end); }

		//! ��ȡ�߶γ��ȵ�ƽ�������⿪����
		/** \return �߶γ��ȵ�ƽ�� */
		T getLengthSQ() const { return start.squaredDistance(end); }

		//! ��ȡ�߶��е�
		/** \return �߶��е�. */
		Vector2 getMiddle() const
		{
			return (start + end) / (T)2;
		}

		//! ��ȡ�߶�����
		/** \return �߶����� */
		Vector2 getVector() const { return Vector2(end.x - start.x, end.y - start.y); }

		//! ���������Ƿ�����һ���߶��ཻ
		/** \param l: Ҫ���Ե���һ���߶�
		\param checkOnlySegments: Ĭ������ʼ��ͽ�����֮������ཻ�ԡ�������Ϊfalse������������鳬���߶η�Χ���׸�����
		\param out: �����һ�����㣬�����������λ��
		\return ����н��㷵��true*/
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
				// ���߶�Ҫô�غ�Ҫôƽ��
				// ����������Ӷ���0�������غ�
				if (Math::equals(numeratorA, 0.f) && Math::equals(numeratorB, 0.f))
				{
					// ���ȳ����ҵ�һ�������յ�
					if (l.start == start || l.end == start)
						out = start;
					else if (l.end == end || l.start == end)
						out = end;
					// ���ڼ�������߶��Ƿ��Ǽ�ϵ�
					else if (l.start.x > start.x && l.end.x > start.x && l.start.x > end.x && l.end.x > end.x)
						return false;
					else if (l.start.y > start.y && l.end.y > start.y && l.start.y > end.y && l.end.y > end.y)
						return false;
					else if (l.start.x < start.x && l.end.x<start.x && l.start.x < end.x && l.end.x < end.x)
						return false;
					else if (l.start.y < start.y && l.end.y < start.y && l.start.y<end.y && l.end.y<end.y)
						return false;
					// ������Щ�߶�ĳЩ�̶����غϵ� 
					else
					{
						// �ҵ�����빫�����ֲ���ɵĵ�
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

						// һ���߶ΰ�����һ����ʰȡȷ���ص�ʣ�ಿ�ֵ����ĵ�
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

					return true; // �غ�
				}

				return false; // ƽ��
			}

			// ��ȡ������������ֱ�����ཻ�ԣ� �������Ƿ����߶���
			const Real uA = numeratorA / commonDenominator;
			if (checkOnlySegments && (uA < 0.f || uA > 1.f))
				return false; //���������߶���

			const Real uB = numeratorB / commonDenominator;
			if (checkOnlySegments && (uB < 0.f || uB > 1.f))
				return false; //���������߶���

			// ���㽻��
			out.x = (T)(start.x + uA * (end.x - start.x));
			out.y = (T)(start.y + uA * (end.y - start.y));
			return true;
		}

		//! ��ȡ�����߶εĵ�λ����
		/** \return �߶εĵ�λ���� */
		Vector2 getUnitVector() const
		{
			T len = (T)(1.0 / getLength());
			return Vector2((end.x - start.x) * len, (end.y - start.y) * len);
		}

		//! ��ȡ�����߶κ͸����߶μ�Ľ���
		/** \param l Ҫ���Ե���һ���߶�
		\return �Ƕ�ֵ */
		Radian getAngleWith(const line2d<T>& l) const
		{
			Vector2 vect = getVector();
			Vector2 vect2 = l.getVector();
			//Ĭ�϶��ǰ�����ֵ����
			return vect.angleBetween(vect2);
		}

		//! ����������������������߶���߻����ұߣ���������ֱ����
		/** \return �������0����ֱ���ϣ�С��0����ߣ� ����0���ұ� */
		T getPointOrientation(const Vector2& point) const
		{
			return ((end.x - start.x) * (point.y - start.y) -
				(point.x - start.x) * (end.y - start.y));
		}

		//! ���������������߶���
		/** \return ������߶ε���ʼ�ͽ���֮�䷵��true*/
		bool isPointOnLine(const Vector2& point) const
		{
			T d = getPointOrientation(point);
			return (d == 0 && point.isBetweenPoints(start, end));
		}

		//! �����������߶εĿ�ʼ�ͽ�����֮��
		/** �ٶ�������Ѿ�������ֱ���� */
		bool isPointBetweenStartAndEnd(const Vector2& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//!��ȡ�����߶��ϵ���һ�������ӽ��ĵ�
		/** \param checkOnlySegments: Ĭ��true ���������߶Σ���ʼ�ͽ�����֮�䣩�ĵ�
		����Ϊfalse������ֱ���ϵĵ㣨�����߶��⣩*/
		Vector2 getClosestPoint(const Vector2& point, bool checkOnlySegments = true) const
		{
			Vector2 c((Real)(point.x - start.x), (Real)(point.y - start.y));
			Vector2 v((Real)(end.x - start.x), (Real)(end.y - start.y));
			Real d = v.length();
			if (d == 0)	// �ж������߶��Ƿ��˻��ɵ�����
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

		//! �߶ε���ʼ��
		Vector2 start;
		//! �߶ε��յ�
		Vector2 end;
	};

	// �����ر��Ż����߶� ������ת����
	template <>
	inline Vector2 line2d<Real>::getClosestPoint(const Vector2& point, bool checkOnlySegments) const
	{
		Vector2 c = point - start;
		Vector2 v = end - start;
		Real d = (Real)v.length();
		if (d == 0)		// �ж������߶��Ƿ��˻��ɵ�����
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


	//! ������2d�߶�
	typedef line2d<Real> line2df;
	//! ����2d�߶�
	typedef line2d<SINT32> line2di;
}

#endif