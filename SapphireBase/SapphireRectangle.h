#ifndef _SAPPHIRE_RECTANGLE__
#define _SAPPHIRE_RECTANGLE__

#include "SapphirePrerequisites.h"

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

		 
		rect(const position2d<T>& upperLeft, const position2d<T>& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {}

	 
		template <class U>
		rect(const position2d<T>& pos, const dimension2d<U>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.X + size.Width, pos.Y + size.Height) {}

		//! �����ƶ���������
		rect<T> operator+(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			return ret += pos;
		}

		//! �����ƶ���������
		rect<T>& operator+=(const position2d<T>& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

		//! �����ƶ���������
		rect<T> operator-(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			return ret -= pos;
		}

		//! �����ƶ���������
		rect<T>& operator-=(const position2d<T>& pos)
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
		bool isPointInside(const position2d<T>& pos) const
		{
			return (UpperLeftCorner.X <= pos.X &&
				UpperLeftCorner.Y <= pos.Y &&
				LowerRightCorner.X >= pos.X &&
				LowerRightCorner.Y >= pos.Y);
		}

		//! ������������Ƿ������һ�����η�����ײ
		/** \param other ��һ������
		\return ���������ײ������true. */
		bool isRectCollided(const rect<T>& other) const
		{
			return (LowerRightCorner.Y > other.UpperLeftCorner.Y &&
				UpperLeftCorner.Y < other.LowerRightCorner.Y &&
				LowerRightCorner.X > other.UpperLeftCorner.X &&
				UpperLeftCorner.X < other.LowerRightCorner.X);
		}

		//! ������һ�����ζ�������ν��м���
		/** \param other ��һ������ */
		void clipAgainst(const rect<T>& other)
		{
			if (other.LowerRightCorner.X < LowerRightCorner.X)
				LowerRightCorner.X = other.LowerRightCorner.X;
			if (other.LowerRightCorner.Y < LowerRightCorner.Y)
				LowerRightCorner.Y = other.LowerRightCorner.Y;

			if (other.UpperLeftCorner.X > UpperLeftCorner.X)
				UpperLeftCorner.X = other.UpperLeftCorner.X;
			if (other.UpperLeftCorner.Y > UpperLeftCorner.Y)
				UpperLeftCorner.Y = other.UpperLeftCorner.Y;

			// correct possible invalid rect resulting from clipping
			if (UpperLeftCorner.Y > LowerRightCorner.Y)
				UpperLeftCorner.Y = LowerRightCorner.Y;
			if (UpperLeftCorner.X > LowerRightCorner.X)
				UpperLeftCorner.X = LowerRightCorner.X;
		}

		//! �жϾ����Ƿ�������һ���ڲ�
		/** \return ����ǣ�����true*/
		bool constrainTo(const rect<T>& other)
		{
			if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
				return false;

			T diff = other.LowerRightCorner.X - LowerRightCorner.X;
			if (diff < 0)
			{
				LowerRightCorner.X += diff;
				UpperLeftCorner.X += diff;
			}

			diff = other.LowerRightCorner.Y - LowerRightCorner.Y;
			if (diff < 0)
			{
				LowerRightCorner.Y += diff;
				UpperLeftCorner.Y += diff;
			}

			diff = UpperLeftCorner.X - other.UpperLeftCorner.X;
			if (diff < 0)
			{
				UpperLeftCorner.X -= diff;
				LowerRightCorner.X -= diff;
			}

			diff = UpperLeftCorner.Y - other.UpperLeftCorner.Y;
			if (diff < 0)
			{
				UpperLeftCorner.Y -= diff;
				LowerRightCorner.Y -= diff;
			}

			return true;
		}

		//! ��ȡ���ο��
		T getWidth() const
		{
			return LowerRightCorner.X - UpperLeftCorner.X;
		}

		//! ��ȡ���θ߶�
		T getHeight() const
		{
			return LowerRightCorner.Y - UpperLeftCorner.Y;
		}

		//! ����������½�С�����Ͻǣ�������������
		void repair()
		{
			if (LowerRightCorner.X < UpperLeftCorner.X)
			{
				T t = LowerRightCorner.X;
				LowerRightCorner.X = UpperLeftCorner.X;
				UpperLeftCorner.X = t;
			}

			if (LowerRightCorner.Y < UpperLeftCorner.Y)
			{
				T t = LowerRightCorner.Y;
				LowerRightCorner.Y = UpperLeftCorner.Y;
				UpperLeftCorner.Y = t;
			}
		}

		//! ���ؾ��λ��Ƶ���Ч��
		/** ������Ͻ��Ƿ�����½Ǹ����һ������  */
		bool isValid() const
		{
			return ((LowerRightCorner.X >= UpperLeftCorner.X) &&
				(LowerRightCorner.Y >= UpperLeftCorner.Y));
		}

		//! ��ȡ���ε�����
		position2d<T> getCenter() const
		{
			return position2d<T>(
				(UpperLeftCorner.X + LowerRightCorner.X) / 2,
				(UpperLeftCorner.Y + LowerRightCorner.Y) / 2);
		}

		//! ��ȡ���ε���۳ߴ�
		dimension2d<T> getSize() const
		{
			return dimension2d<T>(getWidth(), getHeight());
		}


		//! ���һ���㵽����
		/** ���������ں����⣬�ᵼ�������������
		\param p ��ӵ�������ĵ� */
		void addInternalPoint(const position2d<T>& p)
		{
			addInternalPoint(p.X, p.Y);
		}

		//! ���һ���㵽��Χ������
		/**���������ں����⣬�ᵼ�������������
		\param x ��ӵ�������ӵĵ��x����
		\param y ��ӵ�������ӵĵ��Y���� */
		void addInternalPoint(T x, T y)
		{
			if (x>LowerRightCorner.X)
				LowerRightCorner.X = x;
			if (y>LowerRightCorner.Y)
				LowerRightCorner.Y = y;

			if (x<UpperLeftCorner.X)
				UpperLeftCorner.X = x;
			if (y<UpperLeftCorner.Y)
				UpperLeftCorner.Y = y;
		}

		//! �������Ͻ�
		position2d<T> UpperLeftCorner;
		//! �������½�
		position2d<T> LowerRightCorner;
	};

	//! �������
	typedef rect<Real> rectf;
	//! ���ξ���
	typedef rect<SINT32> recti;
}

#endif 
