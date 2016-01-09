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

	typedef Vector2 position2d;
	
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

		 
		rect(const position2d& upperLeft, const position2d& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {}

	    template<class U>
		rect(const position2d& pos, const dimension2d<U>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.x + size.Width, pos.y + size.Height) {}

		//! 向右移动给定的量
		rect<T> operator+(const position2d& pos) const
		{
			rect<T> ret(*this);
			return ret += pos;
		}

		//! 向右移动给定的量
		rect<T>& operator+=(const position2d& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

		//! 向左移动给定的量
		rect<T> operator-(const position2d& pos) const
		{
			rect<T> ret(*this);
			return ret -= pos;
		}

		//! 向左移动给定的量
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
		bool isPointInside(const position2d& pos) const
		{
			return (UpperLeftCorner.x <= pos.x &&
				UpperLeftCorner.y <= pos.y &&
				LowerRightCorner.x >= pos.x &&
				LowerRightCorner.y >= pos.y);
		}

		//! 测试如果矩形是否和另外一个矩形发生碰撞
		/** \param other 另一个矩形
		\return 如果发生碰撞，返回true. */
		bool isRectCollided(const rect<T>& other) const
		{
			return (LowerRightCorner.y > other.UpperLeftCorner.y &&
				UpperLeftCorner.y < other.LowerRightCorner.y &&
				LowerRightCorner.x > other.UpperLeftCorner.x &&
				UpperLeftCorner.x < other.LowerRightCorner.x);
		}

		//! 用另外一个矩形对这个矩形进行剪裁
		/** \param other 另一个矩形 */
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

		//! 判断矩形是否在另外一个内部
		/** \return 如果是，返回true*/
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

		//! 获取矩形宽度
		T getWidth() const
		{
			return LowerRightCorner.x - UpperLeftCorner.x;
		}

		//! 获取矩形高度
		T getHeight() const
		{
			return LowerRightCorner.y - UpperLeftCorner.y;
		}

		//! 如果矩形右下角小于左上角，交换这两个点
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

		//! 返回矩形绘制的有效性
		/** 如果左上角是否比右下角更靠右或更靠左  */
		bool isValid() const
		{
			return ((LowerRightCorner.x >= UpperLeftCorner.x) &&
				(LowerRightCorner.y >= UpperLeftCorner.y));
		}

		//! 获取矩形的中心
		position2d getCenter() const
		{
			return position2d(
				(UpperLeftCorner.x + LowerRightCorner.x) / 2,
				(UpperLeftCorner.y + LowerRightCorner.y) / 2);
		}

		//! 获取矩形的外观尺寸
		dimension2d<T> getSize() const
		{
			return dimension2d<T>(getWidth(), getHeight());
		}


		//! 添加一个点到矩形
		/** 如果这个点在盒子外，会导致这个矩形增大
		\param p 添加到盒子里的点 */
		void addInternalPoint(const position2d& p)
		{
			addInternalPoint(p.x, p.y);
		}

		//! 添加一个点到范围化矩形
		/**如果这个点在盒子外，会导致这个矩形增大
		\param x 添加到这个盒子的点的x坐标
		\param y 添加到这个盒子的点的y坐标 */
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

		//! 矩形左上角
		Position2d UpperLeftCorner;
		//! 矩形右下角
		Position2d LowerRightCorner;
	};

	//! 浮点矩形
	typedef rect<Real> rectf;
	//! 整形矩形
	typedef rect<SINT32> recti;
}

#endif 
