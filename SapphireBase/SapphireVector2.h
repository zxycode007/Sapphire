#ifndef _SAPPHIRE_VECTOR2_
#define _SAPPHIRE_VECTOR2_

#include "SapphirePrerequisites.h"
#include "SapphireMath.h"


namespace Sapphire
{
	/** 标准2d向量
	@remarks
	在2d空间中一个方向，表述为沿着正交坐标系上的距离(x,y) 注意位置，方向和缩放因子都可以被表述为一个向量
	取决于你如何解释这个值
	*/
	class _SapphireExport Vector2
	{
	public:
		Real x, y;

	public:
		inline Vector2()
		{
		}

		inline Vector2(const Real fX, const Real fY)
			: x(fX), y(fY)
		{
		}

		inline explicit Vector2(const Real scaler)
			: x(scaler), y(scaler)
		{
		}

		inline explicit Vector2(const Real afCoordinate[2])
			: x(afCoordinate[0]),
			y(afCoordinate[1])
		{
		}

		inline explicit Vector2(const int afCoordinate[2])
		{
			x = (Real)afCoordinate[0];
			y = (Real)afCoordinate[1];
		}

		inline explicit Vector2(Real* const r)
			: x(r[0]), y(r[1])
		{
		}

		/** 
		交换两个向量的内容
		*/
		inline void swap(Vector2& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
		}
		//重定义成员选取符
		inline Real operator [] (const size_t i) const
		{
			assert(i < 2);

			return *(&x + i);
		}
		//重定义成员选取符
		inline Real& operator [] (const size_t i)
		{
			assert(i < 2);

			return *(&x + i);
		}

		/// 指针访问器用于直接拷贝
		inline Real* ptr()
		{
			return &x;
		}
		////// 指针访问器用于直接拷贝
		inline const Real* ptr() const
		{
			return &x;
		}
 
		inline Vector2& operator = (const Vector2& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;

			return *this;
		}

		inline Vector2& operator = (const Real fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

		inline bool operator == (const Vector2& rkVector) const
		{
			return (x == rkVector.x && y == rkVector.y);
		}

		inline bool operator != (const Vector2& rkVector) const
		{
			return (x != rkVector.x || y != rkVector.y);
		}

		
		inline Vector2 operator + (const Vector2& rkVector) const
		{
			return Vector2(
				x + rkVector.x,
				y + rkVector.y);
		}

		inline Vector2 operator - (const Vector2& rkVector) const
		{
			return Vector2(
				x - rkVector.x,
				y - rkVector.y);
		}

		inline Vector2 operator * (const Real fScalar) const
		{
			return Vector2(
				x * fScalar,
				y * fScalar);
		}

		inline Vector2 operator * (const Vector2& rhs) const
		{
			return Vector2(
				x * rhs.x,
				y * rhs.y);
		}

		inline Vector2 operator / (const Real fScalar) const
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			return Vector2(
				x * fInv,
				y * fInv);
		}

		inline Vector2 operator / (const Vector2& rhs) const
		{
			return Vector2(
				x / rhs.x,
				y / rhs.y);
		}

		inline const Vector2& operator + () const
		{
			return *this;
		}

		inline Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

		// overloaded operators to help Vector2
		inline friend Vector2 operator * (const Real fScalar, const Vector2& rkVector)
		{
			return Vector2(
				fScalar * rkVector.x,
				fScalar * rkVector.y);
		}

		inline friend Vector2 operator / (const Real fScalar, const Vector2& rkVector)
		{
			return Vector2(
				fScalar / rkVector.x,
				fScalar / rkVector.y);
		}

		inline friend Vector2 operator + (const Vector2& lhs, const Real rhs)
		{
			return Vector2(
				lhs.x + rhs,
				lhs.y + rhs);
		}

		inline friend Vector2 operator + (const Real lhs, const Vector2& rhs)
		{
			return Vector2(
				lhs + rhs.x,
				lhs + rhs.y);
		}

		inline friend Vector2 operator - (const Vector2& lhs, const Real rhs)
		{
			return Vector2(
				lhs.x - rhs,
				lhs.y - rhs);
		}

		inline friend Vector2 operator - (const Real lhs, const Vector2& rhs)
		{
			return Vector2(
				lhs - rhs.x,
				lhs - rhs.y);
		}

		// arithmetic updates
		inline Vector2& operator += (const Vector2& rkVector)
		{
			x += rkVector.x;
			y += rkVector.y;

			return *this;
		}

		inline Vector2& operator += (const Real fScaler)
		{
			x += fScaler;
			y += fScaler;

			return *this;
		}
		inline Vector2& operator -= (const Vector2& rkVector)
		{
			x -= rkVector.x;
			y -= rkVector.y;

			return *this;
		}

		inline Vector2& operator -= (const Real fScaler)
		{
			x -= fScaler;
			y -= fScaler;

			return *this;
		}

		inline Vector2& operator *= (const Real fScalar)
		{
			x *= fScalar;
			y *= fScalar;

			return *this;
		}

		inline Vector2& operator *= (const Vector2& rkVector)
		{
			x *= rkVector.x;
			y *= rkVector.y;

			return *this;
		}

		inline Vector2& operator /= (const Real fScalar)
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;

			return *this;
		}

		inline Vector2& operator /= (const Vector2& rkVector)
		{
			x /= rkVector.x;
			y /= rkVector.y;

			return *this;
		}

		/** 返回向量的长度
		@warning
		这个操作需要一个平方根并且是大量的CPU开销，如果你不需要精确的长度用squredLength（）替代
		*/
		inline Real length() const
		{
			return Math::Sqrt(x * x + y * y);
		}

		/** 
		返回这个向量的长度的平方
		@remarks
		这个方法比较有效率 - 计算向量实际长度需要开根号，这些操作开销很大。
		这个方法返回一个向量长度的平方。但在去平方根之前也可等同于长度。如果你要找到最长或最短的向量无需要求平方根
		*/
		inline Real squaredLength() const
		{
			return x * x + y * y;
		}

		inline SINT32 getXi() const
		{
			return (SINT32)(x + 0.5);
		}

		inline SINT32 getYi() const
		{
			 
			return (SINT32)(y+0.5);
		}
		inline void set(Real X, Real Y)
		{
			x = X;
			x = Y;
		}

		/** 
		
		返回与另一个向量的距离
		@warning
		这个操作也需要大量的CPU操作周期，因为需要开平方根，如无需精确值，请用 squaredDistance()
		*/
		inline Real distance(const Vector2& rhs) const
		{
			return (*this - rhs).length();
		}

		 
		inline Real squaredDistance(const Vector2& rhs) const
		{
			return (*this - rhs).squaredLength();
		}

		/** 
		计算与另一个向量的点积（标量）
		@remarks
		U.V =|U||V| cosθ 
		点积通常用于计算两个向量的角度。如果两个都是单位向量，这个点积这个角度的余弦值
		另外，否则点积必须除以这两个向量的长度的积来求余弦角。这个结果可以用来计算点到平面的距离
		@param
		*/
		inline Real dotProduct(const Vector2& vec) const
		{
			return x * vec.x + y * vec.y;
		}

		/** 标准化向量
		@remarks
		这个方法标准化向量，使其向量长度为1，作为一个单位向量		
		@note
		这个函数不能用于0向量，否则会崩溃。但这不会改变他们的分量
		@返回这个向量的之前的长度
		*/

		inline Real normalise()
		{
			Real fLength = Math::Sqrt(x * x + y * y);

			if (fLength > Real(0.0f))
			{
				Real fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
			}

			return fLength;
		}

		/** 
		返回向量与传递向量之间的中点
		*/
		inline Vector2 midPoint(const Vector2& vec) const
		{
			return Vector2(
				(x + vec.x) * 0.5f,
				(y + vec.y) * 0.5f);
		}

		/** 
		如果这个向量标量单元分量全部大于给定向量，返回true.
		*/
		inline bool operator < (const Vector2& rhs) const
		{
			if ((x < rhs.x) && (y < rhs.y))
			{
				return true;
			}
			else
			{
				return false;
			}
				
			
		}

		 
		inline bool operator >(const Vector2& rhs) const
		{
			if ((x > rhs.x) && (y > rhs.y))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		/** 
		设置向量的分量(最下限)，如果小于比较向量，则传递值给本向量
		 
		*/
		inline void makeFloor(const Vector2& cmp)
		{
			if (cmp.x < x) x = cmp.x;
			if (cmp.y < y) y = cmp.y;
		}

		/** 
		设置向量的分量（最上限），如果大于比较向量，则传递值给本向量
		*/
		inline void makeCeil(const Vector2& cmp)
		{
			if (cmp.x > x) x = cmp.x;
			if (cmp.y > y) y = cmp.y;
		}

		/** 
		创建一个向量的垂直正上方向量（up向量）
		@remarks
		这个方法将返回一个与这个向量正交（垂直）的向量，这有无限种可能，但是这个方法保证创建他们中一个。如果你
		需要更多的控制请用四元数Quaternion类
		*/
		inline Vector2 perpendicular(void) const
		{
			return Vector2(-y, x);
		}

		/** 
		计算两个向量叉积，这个结果是一个单浮点数，它是这个三角形面积的2倍
		*/
		inline Real crossProduct(const Vector2& rkVector) const
		{
			return x * rkVector.y - y * rkVector.x;
		}

		/** 
		创建一个新的通过给定一个随机角度而脱离这个向量的随机向量
		@remarks
		这个方法假定了随机数生成器准备好了
		@param
		angle  偏离角度的弧度
		*/
		inline Vector2 randomDeviant(Real angle) const
		{

			angle *= Math::UnitRandom() * Math::TWO_PI;
			Real cosa = cos(angle);
			Real sina = sin(angle);
			return  Vector2(cosa * x - sina * y,
				sina * x + cosa * y);
		}

		/** 如果向量0长度返回true */
		inline bool isZeroLength(void) const
		{
			Real sqlen = (x * x) + (y * y);
			return (sqlen < (1e-06 * 1e-06));

		}

		/** 做标准化, 做一个复制然后标准化，本向量不受影响*/
		inline Vector2 normalisedCopy(void) const
		{
			Vector2 ret = *this;
			ret.normalise();
			return ret;
		}

		/** 
		用给定的法线计算一个反射向量到平面
		@remarks NB 假设 'this' 指向远离的屏幕，如果不是，翻转它
		*/
		inline Vector2 reflect(const Vector2& normal) const
		{
			return Vector2(*this - (2 * this->dotProduct(normal) * normal));
		}

		/// 检测这个向量是否包含有效的值
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y);
		}

		/**	
		获取2个向量间的角度
		@remarks
		向量可以不用单位长度，但是必须能表示方向
		*/
		inline Sapphire::Radian angleBetween(const Sapphire::Vector2& other) const
		{
			//两个长度的乘积
			Sapphire::Real lenProduct = length() * other.length();
			// 除零检查
			if (lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			Sapphire::Real f = dotProduct(other) / lenProduct;
			//做限制
			f = Sapphire::Math::Clamp(f, (Sapphire::Real) - 1.0, (Sapphire::Real)1.0);
			return Sapphire::Math::ACos(f);
		}

		/**	取得两个向量间的朝向角 
		@remarks
		向量可以不用单位长度，但是必须表示一个方向
		角度在0~2PI
		*/
		inline Sapphire::Radian angleTo(const Sapphire::Vector2& other) const
		{
			//求出夹角
			Sapphire::Radian angle = angleBetween(other);
			//本向量在other向量的逆时针方向
			if (crossProduct(other)<0)
				angle = (Sapphire::Radian)Sapphire::Math::TWO_PI - angle;

			return angle;
		}


		//! 返回这个向量是否是在一条线段两点间线性插值的某个点
		/** 假定这个点在线段两点之间
		\param begin 起始向量
		\param end 结束向量
		\return 如果这个点在线段起始和结尾之间返回true，否则返回false */
		inline bool isBetweenPoints(const Vector2& begin, const Vector2& end) const
		{
			if (begin.x != end.x)
			{
				return ((begin.x <= x && x <= end.x) ||
					(begin.x >= x && x >= end.x));
			}
			else
			{
				return ((begin.y <= y && y <= end.y) ||
					(begin.y >= y && y >= end.y));
			}
		}


		//! 创建一个在向量与另一个向量之间的插值向量
		/** \param other 与其插值的另一个向量
		\param d 插值的值（0.0f到1.0f之间） 注意：这是与getInterpolated_quadratic()的插值方向相反的
		\return 一个插值的向量，并不修改本向量 */
		Vector2 getInterpolated(const Vector2& other, Real d) const
		{
			Real inv = 1.0f - d;
			return Vector2((other.x*inv + x*d),(other.y*inv + y*d));
		}


		//! 创建一个在向量与另一个向量之间的二次插值向量
		/** \param v2 第二个参与插值向量
		\param v3 第三个参与插值的向量 (maximum at 1.0f)
		\param d 插值的值 由0.0f（所有向量）和1.0f（所有第三个向量）之间
		注意:这是与getInterpolated()的插值方向相反的 
		\return 一个插值的向量，并不修改本向量 */
		Vector2 getInterpolated_quadratic(const Vector2& v2, const Vector2 & v3, Real d) const
		{
			// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
			const Real inv = 1.0f - d;
			const Real mul0 = inv * inv;
			const Real mul1 = 2.0f * d * inv;
			const Real mul2 = d * d;

			return Vector2( (x * mul0 + v2.x * mul1 + v3.x * mul2),
				 (y * mul0 + v2.y * mul1 + v3.y * mul2));
		}


		// 特殊的点special points
		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 UNIT_SCALE;

		/** 输出到流的
		*/
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Vector2& v)
		{
			o << "Vector2(" << v.x << ", " << v.y << ")";
			return o;
		}
	};

}

#define Position2d Vector2

#endif