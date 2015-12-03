#ifndef _SAPPHIRE_LINE3D_
#define _SAPPHIRE_LINE3D_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireMath.h"

namespace Sapphire
{
	//! ������֮���3d�߶�
	template <class T>
	class line3d
	{
	public:

		//! Ĭ�Ϲ��캯��
		/** ��(0,0,0) �� (1,1,1)���߶� */
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

		//! ��ȡ�߶γ���
		/** \return �߶γ��� */
		T getLength() const { return start.distance(end); }

		//! ��ȡ�߶γ��ȵ�ƽ�������⿪����
		/** \return �߶γ��ȵ�ƽ�� */
		T getLengthSQ() const { return start.squaredDistance(end); }

		//! ��ȡ�߶��е�
		/** \return �߶��е�. */
		Vector3 getMiddle() const
		{
			return (start + end) / (T)2;
		}

		//! ��ȡ�߶�����
		/** \return �߶����� */
		Vector3 getVector() const
		{
			return end - start;
		}

		//! �Ƿ�������������߶ε���ʼ�ͽ�β��֮��
		/** �ٶ�������Լ��������߶ε�ĳ��
		\param point ���Ե�
		\return True �����������߶���ʼ�ͽ�β֮�䷵��true�����򷵻�false
		*/
		bool isPointBetweenStartAndEnd(const Vector3& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//! ��ȡ�����߶��ϵ���һ�������ӽ��ĵ�
		/** \param point Ҫ�Ƚϵĵ�
		\return ��ӽ��ĵ� */
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

		//! ��������߶��Ƿ���һ�������ཻ
		/** \param sorigin: ����ԭ��
		\param sradius: ����뾶
		\param outdistance: ����һ���ཻ��ľ���
		\return ���Ϊture�ཻ
		����н���Ļ�������һ���ཻ��ľ�������outdistance��
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

		// ��Ա����

		//! �߶���ʼ��
		Vector3 start;
		//! �߶ν�����
		Vector3 end;
	};

	//! Typedef for an Real line.
	typedef line3d<Real> line3df;
	//! Typedef for an integer line.
	typedef line3d<SINT32> line3di;
}

#endif 