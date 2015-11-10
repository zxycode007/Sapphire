#ifndef _SAPPHIRE_VECTOR3_
#define _SAPPHIRE_VECTOR3_

#include "SapphirePrerequisites.h"
#include "SapphireQuaternion.h"
#include "SapphireMath.h"



namespace Sapphire
{

	/*
	参考Vector2
	*/
	class _SapphireExport Vector3
	{
	public:
		Real x, y, z;

	public:
		inline Vector3()
		{
		}

		inline Vector3(const Real fX, const Real fY, const Real fZ)
			: x(fX), y(fY), z(fZ)
		{
		}

		inline explicit Vector3(const Real afCoordinate[3])
			: x(afCoordinate[0]),
			y(afCoordinate[1]),
			z(afCoordinate[2])
		{
		}

		inline explicit Vector3(const int afCoordinate[3])
		{
			x = (Real)afCoordinate[0];
			y = (Real)afCoordinate[1];
			z = (Real)afCoordinate[2];
		}

		inline explicit Vector3(Real* const r)
			: x(r[0]), y(r[1]), z(r[2])
		{
		}

		inline explicit Vector3(const Real scaler)
			: x(scaler)
			, y(scaler)
			, z(scaler)
		{
		}


		 
		inline void swap(Vector3& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		inline Real operator [] (const size_t i) const
		{
			assert(i < 3);

			return *(&x + i);
		}

		inline Real& operator [] (const size_t i)
		{
			assert(i < 3);

			return *(&x + i);
		}
	 
		inline Real* ptr()
		{
			return &x;
		}
		 
		inline const Real* ptr() const
		{
			return &x;
		}

		 
		inline Vector3& operator = (const Vector3& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;

			return *this;
		}

		inline Vector3& operator = (const Real fScaler)
		{
			x = fScaler;
			y = fScaler;
			z = fScaler;

			return *this;
		}

		inline bool operator == (const Vector3& rkVector) const
		{
			return (x == rkVector.x && y == rkVector.y && z == rkVector.z);
		}

		inline bool operator != (const Vector3& rkVector) const
		{
			return (x != rkVector.x || y != rkVector.y || z != rkVector.z);
		}

		 
		inline Vector3 operator + (const Vector3& rkVector) const
		{
			return Vector3(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z);
		}

		inline Vector3 operator - (const Vector3& rkVector) const
		{
			return Vector3(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z);
		}

		inline Vector3 operator * (const Real fScalar) const
		{
			return Vector3(
				x * fScalar,
				y * fScalar,
				z * fScalar);
		}

		inline Vector3 operator * (const Vector3& rhs) const
		{
			return Vector3(
				x * rhs.x,
				y * rhs.y,
				z * rhs.z);
		}

		inline Vector3 operator / (const Real fScalar) const
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			return Vector3(
				x * fInv,
				y * fInv,
				z * fInv);
		}

		inline Vector3 operator / (const Vector3& rhs) const
		{
			return Vector3(
				x / rhs.x,
				y / rhs.y,
				z / rhs.z);
		}

		inline const Vector3& operator + () const
		{
			return *this;
		}

		inline Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}

		 
		inline friend Vector3 operator * (const Real fScalar, const Vector3& rkVector)
		{
			return Vector3(
				fScalar * rkVector.x,
				fScalar * rkVector.y,
				fScalar * rkVector.z);
		}

		inline friend Vector3 operator / (const Real fScalar, const Vector3& rkVector)
		{
			return Vector3(
				fScalar / rkVector.x,
				fScalar / rkVector.y,
				fScalar / rkVector.z);
		}

		inline friend Vector3 operator + (const Vector3& lhs, const Real rhs)
		{
			return Vector3(
				lhs.x + rhs,
				lhs.y + rhs,
				lhs.z + rhs);
		}

		inline friend Vector3 operator + (const Real lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs + rhs.x,
				lhs + rhs.y,
				lhs + rhs.z);
		}

		inline friend Vector3 operator - (const Vector3& lhs, const Real rhs)
		{
			return Vector3(
				lhs.x - rhs,
				lhs.y - rhs,
				lhs.z - rhs);
		}

		inline friend Vector3 operator - (const Real lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs - rhs.x,
				lhs - rhs.y,
				lhs - rhs.z);
		}

		 
		inline Vector3& operator += (const Vector3& rkVector)
		{
			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;

			return *this;
		}

		inline Vector3& operator += (const Real fScalar)
		{
			x += fScalar;
			y += fScalar;
			z += fScalar;
			return *this;
		}

		inline Vector3& operator -= (const Vector3& rkVector)
		{
			x -= rkVector.x;
			y -= rkVector.y;
			z -= rkVector.z;

			return *this;
		}

		inline Vector3& operator -= (const Real fScalar)
		{
			x -= fScalar;
			y -= fScalar;
			z -= fScalar;
			return *this;
		}

		inline Vector3& operator *= (const Real fScalar)
		{
			x *= fScalar;
			y *= fScalar;
			z *= fScalar;
			return *this;
		}

		inline Vector3& operator *= (const Vector3& rkVector)
		{
			x *= rkVector.x;
			y *= rkVector.y;
			z *= rkVector.z;

			return *this;
		}

		inline Vector3& operator /= (const Real fScalar)
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;

			return *this;
		}

		inline Vector3& operator /= (const Vector3& rkVector)
		{
			x /= rkVector.x;
			y /= rkVector.y;
			z /= rkVector.z;

			return *this;
		}


		 
		inline Real length() const
		{
			return Math::Sqrt(x * x + y * y + z * z);
		}

		 
		inline Real squaredLength() const
		{
			return x * x + y * y + z * z;
		}

		 
		inline Real distance(const Vector3& rhs) const
		{
			return (*this - rhs).length();
		}

		 
		inline Real squaredDistance(const Vector3& rhs) const
		{
			return (*this - rhs).squaredLength();
		}

		 
		inline Real dotProduct(const Vector3& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		/** 
		计算本向量与另一个向量的点积的绝对值
		@remarks
		这个函数和求点积函数差不多,除了他用向量每个分量的绝对值来进行计算
		@param
		vec 要使用与本向量计算点积的向量 
		@return
		一个表示为点积绝对值的Real型变量
		*/
		inline Real absDotProduct(const Vector3& vec) const
		{
			return Math::Abs(x * vec.x) + Math::Abs(y * vec.y) + Math::Abs(z * vec.z);
		}

		 
		inline Real normalise()
		{
			Real fLength = Math::Sqrt(x * x + y * y + z * z);

			 
			if (fLength > Real(0.0f))
			{
				Real fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
				z *= fInvLength;
			}

			return fLength;
		}

		/** 
		计算两个向量的叉积
		@remarks
		叉积是通常用来求一个平面法向量。 通过计算2个非平行在一个平面上的向量的叉积（通常为三角形两条边）
		@param
		vec 要用来计算叉积的向量，与本向量计算叉积
		@return
		叉积的结果是一个向量。如果你以求最大的效率，可以调用Vector3::normalise标准化。
		这个返回的向量是靠在本向量与rkVector向量的逆时针方向旁边。如：UNIT_Y.crossProduct(UNIT_Z)=UNIT_X
		UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.，这是由于SAPPHIRE使用右手坐标系
		 
		*/
		inline Vector3 crossProduct(const Vector3& rkVector) const
		{
			return Vector3(
				y * rkVector.z - z * rkVector.y,
				z * rkVector.x - x * rkVector.z,
				x * rkVector.y - y * rkVector.x);
		}

		 
		inline Vector3 midPoint(const Vector3& vec) const
		{
			return Vector3(
				(x + vec.x) * 0.5f,
				(y + vec.y) * 0.5f,
				(z + vec.z) * 0.5f);
		}

		 
		inline bool operator < (const Vector3& rhs) const
		{
			if (x < rhs.x && y < rhs.y && z < rhs.z)
				return true;
			return false;
		}

		 
		inline bool operator >(const Vector3& rhs) const
		{
			if (x > rhs.x && y > rhs.y && z > rhs.z)
				return true;
			return false;
		}

		 
		inline void makeFloor(const Vector3& cmp)
		{
			if (cmp.x < x) x = cmp.x;
			if (cmp.y < y) y = cmp.y;
			if (cmp.z < z) z = cmp.z;
		}

		 
		inline void makeCeil(const Vector3& cmp)
		{
			if (cmp.x > x) x = cmp.x;
			if (cmp.y > y) y = cmp.y;
			if (cmp.z > z) z = cmp.z;
		}

		/** 
		创建一个向量的垂直正上方向量（up向量）
		@remarks
		这个方法将返回一个与这个向量正交（垂直）的向量，这有无限种可能，但是这个方法保证创建他们中一个。如果你
		需要更多的控制请用四元数Quaternion类
		*/
		inline Vector3 perpendicular(void) const
		{
			//0的平方
			static const Real fSquareZero = (Real)(1e-06 * 1e-06);

			//计算该向量与单位向量(1，0，0)的叉积
			Vector3 perp = this->crossProduct(Vector3::UNIT_X);

			// 计算其长度
			if (perp.squaredLength() < fSquareZero)
			{
				/* 
				这个向量在Y轴乘上一个标量，所以我们用另外一个轴
				*/
				perp = this->crossProduct(Vector3::UNIT_Y);
			}
			perp.normalise();

			return perp;
		}
		/** 
		创建一个新的通过给定一个随机角度而脱离这个向量的随机向量
		 
		@remarks
		这个方法假定了随机数生成器准备好了
		@param
		angle  偏离角度的弧度
		*/
		inline Vector3 randomDeviant(
			const Radian& angle,
			const Vector3& up = Vector3::ZERO) const
		{
			Vector3 newUp;

			if (up == Vector3::ZERO)
			{
				//创建一个up向量
				newUp = this->perpendicular();
			}
			else
			{
				newUp = up;
			}

			// 通过环绕这个随机量来旋转up向量
			Quaternion q;
			q.FromAngleAxis(Radian(Math::UnitRandom() * Math::TWO_PI), *this);
			newUp = q * newUp;

			// 最终通过环绕随机up给定的角度来旋转这个向量
			q.FromAngleAxis(angle, newUp);
			return q * (*this);
		}

		/** 取两个向量之间的角度
		@remarks
		向量不需要单位长度，但是必须是方向向量
		*/
		inline Radian angleBetween(const Vector3& dest) const
		{
			Real lenProduct = length() * dest.length();

			//除0检测
			if (lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			Real f = dotProduct(dest) / lenProduct;

			f = Math::Clamp(f, (Real)-1.0, (Real)1.0);
			return Math::ACos(f);

		}
		/** 
		取得旋转这个向量到目标向量的最短弧四元数
		@remarks
		如果用一个最接近这个向量的反向的目标向量，我们将选择180度
		环绕这个 'fallbackAxis'（如果指定了的话，如果没有创建）
		以确保这种情况下任意轴都是有效的
		*/
		Quaternion getRotationTo(const Vector3& dest,
			const Vector3& fallbackAxis = Vector3::ZERO) const
		{
			// 基于Stan Melax在 Game Programming Gems的文章
			Quaternion q;
			// 复制，不能修改本地原始内容
			Vector3 v0 = *this;
			Vector3 v1 = dest;
			v0.normalise();
			v1.normalise();

			Real d = v0.dotProduct(v1);
			//如果点积 == 1, 向量相同
			if (d >= 1.0f)
			{
				return Quaternion::IDENTITY;
			}
			if (d < (1e-6f - 1.0f))
			{
				if (fallbackAxis != Vector3::ZERO)
				{
					// 旋转 180 度  fallback axis
					q.FromAngleAxis(Radian(Math::_PI), fallbackAxis);
				}
				else
				{
					// 创建这个轴
					Vector3 axis = Vector3::UNIT_X.crossProduct(*this);
					if (axis.isZeroLength()) // pick another if colinear
						axis = Vector3::UNIT_Y.crossProduct(*this);
					axis.normalise();
					q.FromAngleAxis(Radian(Math::_PI), axis);
				}
			}
			else
			{
				Real s = Math::Sqrt((1 + d) * 2);
				Real invs = 1 / s;

				Vector3 c = v0.crossProduct(v1);

				q.x = c.x * invs;
				q.y = c.y * invs;
				q.z = c.z * invs;
				q.w = s * 0.5f;
				q.normalise();
			}
			return q;
		}

		 
		inline bool isZeroLength(void) const
		{
			Real sqlen = (x * x) + (y * y) + (z * z);
			return (sqlen < (1e-06 * 1e-06));

		}

		 
		inline Vector3 normalisedCopy(void) const
		{
			Vector3 ret = *this;
			ret.normalise();
			return ret;
		}

		 
		inline Vector3 reflect(const Vector3& normal) const
		{
			return Vector3(*this - (2 * this->dotProduct(normal) * normal));
		}

		 
		inline bool positionEquals(const Vector3& rhs, Real tolerance = 1e-03) const
		{
			return Math::RealEqual(x, rhs.x, tolerance) &&
				Math::RealEqual(y, rhs.y, tolerance) &&
				Math::RealEqual(z, rhs.z, tolerance);

		}

		/** 
		返回是否这个向量和另一个向量由位置公差(判断是否重叠)
		*/
		inline bool positionCloses(const Vector3& rhs, Real tolerance = 1e-03f) const
		{
			return squaredDistance(rhs) <=
				(squaredLength() + rhs.squaredLength()) * tolerance;
		}

		 
		inline bool directionEquals(const Vector3& rhs,
			const Radian& tolerance) const
		{
			Real dot = dotProduct(rhs);
			Radian angle = Math::ACos(dot);

			return Math::Abs(angle.valueRadians()) <= tolerance.valueRadians();

		}

		 
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z);
		}

	 
		inline Vector3 primaryAxis() const
		{
			Real absx = Math::Abs(x);
			Real absy = Math::Abs(y);
			Real absz = Math::Abs(z);
			if (absx > absy)
				if (absx > absz)
					return x > 0 ? Vector3::UNIT_X : Vector3::NEGATIVE_UNIT_X;
				else
					return z > 0 ? Vector3::UNIT_Z : Vector3::NEGATIVE_UNIT_Z;
			else // absx <= absy
				if (absy > absz)
					return y > 0 ? Vector3::UNIT_Y : Vector3::NEGATIVE_UNIT_Y;
				else
					return z > 0 ? Vector3::UNIT_Z : Vector3::NEGATIVE_UNIT_Z;


		}

		// 特别点
		static const Vector3 ZERO;  //原点
		static const Vector3 UNIT_X;   //单位向量(1,0,0)
		static const Vector3 UNIT_Y;    //(0,1,0)
		static const Vector3 UNIT_Z;    //(0,0,1)
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 UNIT_SCALE;

		 
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Vector3& v)
		{
			o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
			return o;
		}
	};
	/** @} */
	/** @} */

}


#endif