#ifndef _SAPPHIRE_RECTANGLE__
#define _SAPPHIRE_RECTANGLE__

#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"
#include "SapphireDimension2D.h"

namespace Sapphire {

	

	struct _SapphireExport Rectangle
	{
		Real left;
		Real top;
		Real right;
		Real bottom;

		inline bool inside(Real x, Real y) const { return x >= left && x <= right && y >= top && y <= bottom; }
	};

	/** ��������������ཻ�Բ��� 
	*/
	inline Rectangle intersect(const Rectangle& lhs, const Rectangle& rhs)
	{
		Rectangle r;

		r.left = lhs.left   > rhs.left ? lhs.left : rhs.left;
		r.top = lhs.top    > rhs.top ? lhs.top : rhs.top;
		r.right = lhs.right  < rhs.right ? lhs.right : rhs.right;
		r.bottom = lhs.bottom < rhs.bottom ? lhs.bottom : rhs.bottom;

		return r;
	}

	typedef Vector2 position2d;
	
	//! ����ģ��
	/** ��Ҫ���ڻ���2D GUIԪ�غ�2D���Ʒ���
	���꣨0��0�������Ͻǣ� �������Һ������ӵ��ײ������·�
	*/
	template <class T>
	class rect
	{
	public:

		 
		rect() : UpperLeftCorner(0, 0), LowerRightCorner(0, 0) {}

		//! �������ǹ���
		rect(T x, T y, T x2, T y2)
			: UpperLeftCorner(x, y), LowerRightCorner(x2, y2) {}

		 
		rect(const position2d& upperLeft, const position2d& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {}

	    template<class U>
		rect(const position2d& pos, const dimension2d<U>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.x + size.Width, pos.y + size.Height) {}

		//! �����ƶ���������
		rect<T> operator+(const position2d& pos) const
		{
			rect<T> ret(*this);
			return ret += pos;
		}

		//! �����ƶ���������
		rect<T>& operator+=(const position2d& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

		//! �����ƶ���������
		rect<T> operator-(const position2d& pos) const
		{
			rect<T> ret(*this);
			return ret -= pos;
		}

		//! �����ƶ���������
		rect<T>& operator-=(const position2d& pos)
		{
			UpperLeftCorner -= pos;
			LowerRightCorner -= pos;
			return *this;
		}

		 
		bool operator==(const rect<T>& other) const
		{
			return (UpperLeftCorner == other.UpperLeftCorner &&
				LowerRightCorner == other.LowerRightCorner);
		}

		 
		bool operator!=(const rect<T>& other) const
		{
			return (UpperLeftCorner != other.UpperLeftCorner ||
				LowerRightCorner != other.LowerRightCorner);
		}

		//! �ȽϾ��δ�С
		bool operator<(const rect<T>& other) const
		{
			return getArea() < other.getArea();
		}

		//! ���ؾ��δ�С
		T getArea() const
		{
			return getWidth() * getHeight();
		}

		//! ����һ��2d�ĵ��Ƿ������������
		/** \param pos ���Ե�
		\return True ����ڷ���true������false */
		bool isPointInside(const position2d& pos) const
		{
			return (UpperLeftCorner.x <= pos.x &&
				UpperLeftCorner.y <= pos.y &&
				LowerRightCorner.x >= pos.x &&
				LowerRightCorner.y >= pos.y);
		}

		//! ������������Ƿ������һ�����η�����ײ
		/** \param other ��һ������
		\return ���������ײ������true. */
		bool isRectCollided(const rect<T>& other) const
		{
			return (LowerRightCorner.y > other.UpperLeftCorner.y &&
				UpperLeftCorner.y < other.LowerRightCorner.y &&
				LowerRightCorner.x > other.UpperLeftCorner.x &&
				UpperLeftCorner.x < other.LowerRightCorner.x);
		}

		//! ������һ�����ζ�������ν��м���
		/** \param other ��һ������ */
		void clipAgainst(const rect<T>& other)
		{
			if (other.LowerRightCorner.x < LowerRightCorner.x)
				LowerRightCorner.x = other.LowerRightCorner.x;
			if (other.LowerRightCorner.y < LowerRightCorner.y)
				LowerRightCorner.y = other.LowerRightCorner.y;

			if (other.UpperLeftCorner.x > UpperLeftCorner.x)
				UpperLeftCorner.x = other.UpperLeftCorner.x;
			if (other.UpperLeftCorner.y > UpperLeftCorner.y)
				UpperLeftCorner.y = other.UpperLeftCorner.y;

			// correct possible invalid rect resulting from clipping
			if (UpperLeftCorner.y > LowerRightCorner.y)
				UpperLeftCorner.y = LowerRightCorner.y;
			if (UpperLeftCorner.x > LowerRightCorner.x)
				UpperLeftCorner.x = LowerRightCorner.x;
		}

		//! �жϾ����Ƿ�������һ���ڲ�
		/** \return ����ǣ�����true*/
		bool constrainTo(const rect<T>& other)
		{
			if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
				return false;

			T diff = other.LowerRightCorner.x - LowerRightCorner.x;
			if (diff < 0)
			{
				LowerRightCorner.x += diff;
				UpperLeftCorner.x += diff;
			}

			diff = other.LowerRightCorner.y - LowerRightCorner.y;
			if (diff < 0)
			{
				LowerRightCorner.y += diff;
				UpperLeftCorner.y += diff;
			}

			diff = UpperLeftCorner.x - other.UpperLeftCorner.x;
			if (diff < 0)
			{
				UpperLeftCorner.x -= diff;
				LowerRightCorner.x -= diff;
			}

			diff = UpperLeftCorner.y - other.UpperLeftCorner.y;
			if (diff < 0)
			{
				UpperLeftCorner.y -= diff;
				LowerRightCorner.y -= diff;
			}

			return true;
		}

		//! ��ȡ���ο��
		T getWidth() const
		{
			return LowerRightCorner.x - UpperLeftCorner.x;
		}

		//! ��ȡ���θ߶�
		T getHeight() const
		{
			return LowerRightCorner.y - UpperLeftCorner.y;
		}

		//! ����������½�С�����Ͻǣ�������������
		void repair()
		{
			if (LowerRightCorner.x < UpperLeftCorner.x)
			{
				T t = LowerRightCorner.x;
				LowerRightCorner.x = UpperLeftCorner.x;
				UpperLeftCorner.x = t;
			}

			if (LowerRightCorner.y < UpperLeftCorner.y)
			{
				T t = LowerRightCorner.y;
				LowerRightCorner.y = UpperLeftCorner.y;
				UpperLeftCorner.y = t;
			}
		}

		//! ���ؾ��λ��Ƶ���Ч��
		/** ������Ͻ��Ƿ�����½Ǹ����һ������  */
		bool isValid() const
		{
			return ((LowerRightCorner.x >= UpperLeftCorner.x) &&
				(LowerRightCorner.y >= UpperLeftCorner.y));
		}

		//! ��ȡ���ε�����
		position2d getCenter() const
		{
			return position2d(
				(UpperLeftCorner.x + LowerRightCorner.x) / 2,
				(UpperLeftCorner.y + LowerRightCorner.y) / 2);
		}

		//! ��ȡ���ε���۳ߴ�
		dimension2d<T> getSize() const
		{
			return dimension2d<T>(getWidth(), getHeight());
		}


		//! ���һ���㵽����
		/** ���������ں����⣬�ᵼ�������������
		\param p ��ӵ�������ĵ� */
		void addInternalPoint(const position2d& p)
		{
			addInternalPoint(p.x, p.y);
		}

		//! ���һ���㵽��Χ������
		/**���������ں����⣬�ᵼ�������������
		\param x ��ӵ�������ӵĵ��x����
		\param y ��ӵ�������ӵĵ��y���� */
		void addInternalPoint(T x, T y)
		{
			if (x>LowerRightCorner.x)
				LowerRightCorner.x = x;
			if (y>LowerRightCorner.y)
				LowerRightCorner.y = y;

			if (x<UpperLeftCorner.x)
				UpperLeftCorner.x = x;
			if (y<UpperLeftCorner.y)
				UpperLeftCorner.y = y;
		}

		//! �������Ͻ�
		Position2d UpperLeftCorner;
		//! �������½�
		Position2d LowerRightCorner;
	};

	//! �������
	typedef rect<Real> rectf;
	//! ���ξ���
	typedef rect<SINT32> recti;
}

#endif 
