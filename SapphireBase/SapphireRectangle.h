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

	/** 两个矩形区域的相交性测试 
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
	
	//! 矩形模板
	/** 主要用于绘制2D GUI元素和2D绘制方法
	坐标（0，0）是左上角， 并且向右和下增加到底部的右下方
	*/
	template <class T>
	class rect
	{
	public:

		 
		rect() : UpperLeftCorner(0, 0), LowerRightCorner(0, 0) {}

		//! 用两个角构造
		rect(T x, T y, T x2, T y2)
			: UpperLeftCorner(x, y), LowerRightCorner(x2, y2) {}

		 
		rect(const position2d<T>& upperLeft, const position2d<T>& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {}

	 
		template <class U>
		rect(const position2d<T>& pos, const dimension2d<U>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.X + size.Width, pos.Y + size.Height) {}

		//! 向右移动给定的量
		rect<T> operator+(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			return ret += pos;
		}

		//! 向右移动给定的量
		rect<T>& operator+=(const position2d<T>& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

		//! 向左移动给定的量
		rect<T> operator-(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			return ret -= pos;
		}

		//! 向左移动给定的量
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

		//! 比较矩形大小
		bool operator<(const rect<T>& other) const
		{
			return getArea() < other.getArea();
		}

		//! 返回矩形大小
		T getArea() const
		{
			return getWidth() * getHeight();
		}

		//! 返回一个2d的点是否在这个矩形中
		/** \param pos 测试点
		\return True 如果在返回true，否则false */
		bool isPointInside(const position2d<T>& pos) const
		{
			return (UpperLeftCorner.X <= pos.X &&
				UpperLeftCorner.Y <= pos.Y &&
				LowerRightCorner.X >= pos.X &&
				LowerRightCorner.Y >= pos.Y);
		}

		//! 测试如果矩形是否和另外一个矩形发生碰撞
		/** \param other 另一个矩形
		\return 如果发生碰撞，返回true. */
		bool isRectCollided(const rect<T>& other) const
		{
			return (LowerRightCorner.Y > other.UpperLeftCorner.Y &&
				UpperLeftCorner.Y < other.LowerRightCorner.Y &&
				LowerRightCorner.X > other.UpperLeftCorner.X &&
				UpperLeftCorner.X < other.LowerRightCorner.X);
		}

		//! 用另外一个矩形对这个矩形进行剪裁
		/** \param other 另一个矩形 */
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

		//! 判断矩形是否在另外一个内部
		/** \return 如果是，返回true*/
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

		//! 获取矩形宽度
		T getWidth() const
		{
			return LowerRightCorner.X - UpperLeftCorner.X;
		}

		//! 获取矩形高度
		T getHeight() const
		{
			return LowerRightCorner.Y - UpperLeftCorner.Y;
		}

		//! 如果矩形右下角小于左上角，交换这两个点
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

		//! 返回矩形绘制的有效性
		/** 如果左上角是否比右下角更靠右或更靠左  */
		bool isValid() const
		{
			return ((LowerRightCorner.X >= UpperLeftCorner.X) &&
				(LowerRightCorner.Y >= UpperLeftCorner.Y));
		}

		//! 获取矩形的中心
		position2d<T> getCenter() const
		{
			return position2d<T>(
				(UpperLeftCorner.X + LowerRightCorner.X) / 2,
				(UpperLeftCorner.Y + LowerRightCorner.Y) / 2);
		}

		//! 获取矩形的外观尺寸
		dimension2d<T> getSize() const
		{
			return dimension2d<T>(getWidth(), getHeight());
		}


		//! 添加一个点到矩形
		/** 如果这个点在盒子外，会导致这个矩形增大
		\param p 添加到盒子里的点 */
		void addInternalPoint(const position2d<T>& p)
		{
			addInternalPoint(p.X, p.Y);
		}

		//! 添加一个点到范围化矩形
		/**如果这个点在盒子外，会导致这个矩形增大
		\param x 添加到这个盒子的点的x坐标
		\param y 添加到这个盒子的点的Y坐标 */
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

		//! 矩形左上角
		position2d<T> UpperLeftCorner;
		//! 矩形右下角
		position2d<T> LowerRightCorner;
	};

	//! 浮点矩形
	typedef rect<Real> rectf;
	//! 整形矩形
	typedef rect<SINT32> recti;
}

#endif 
