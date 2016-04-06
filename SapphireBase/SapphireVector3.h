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
		inline Vector3() :x(0), y(0), z(0)
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

		 
		bool equals(const Vector3& other, const Real tolerance = std::numeric_limits<Real>::epsilon()) const
		{
			return Math::equals(x, other.x, tolerance) &&
				Math::equals(y, other.y, tolerance) &&
				Math::equals(z, other.z, tolerance);
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

		inline Vector3 operator + (const Real rhs)
		{
			return Vector3(this->x + rhs, this->y + rhs, this->z + rhs);
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

		inline void set(Real X, Real Y, Real Z)
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
		}

		inline void set(Vector3 v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
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

		//! Sets the length of the vector to a new value
		inline Vector3& setLength(Real newlength)
		{
			normalize();
			return (*this *= newlength);
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


		inline Vector3& normalize()
		{
			FLOAT64 length = x*x + y*y + z*z;
			if (length == 0) // this check isn't an optimization but prevents getting NAN in the sqrt.
				return *this;
			//length = Math::Sqrt(length);
			length = Math::InvSqrt(length);

			x = (Real)(x * length);
			y = (Real)(y * length);
			z = (Real)(z * length);
			return *this;

			
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
			const Vector3& fallbackAxis = Vector3::ZERO) const;
		

		 
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

		//! 创建一个在这个向量和另外一个向量之间插值的向量
		/** \param other 要进行插值的另外一个向量
		\param d 插值的值在0.0f（本向量）到1.0f之间（另外的一个向量）
		注意：这是与getInterpolated_quadratic()插值方向相反
		\return 一个插值的向量，本向量自身没有改动 */
		Vector3 getInterpolated(const Vector3& other, Real d) const
		{
			const Real inv = 1.0 - d;
			return Vector3(other.x*inv + x*d, other.y*inv + y*d, other.z*inv + z*d);
		}


		//! Creates a quadratically interpolated vector between this and two other vectors.
		/** \param v2 Second vector to interpolate with.
		\param v3 Third vector to interpolate with (maximum at 1.0f)
		\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
		Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
		\return An interpolated vector. This vector is not modified. */
		Vector3 getInterpolated_quadratic(const Vector3& v2, const Vector3& v3, Real d) const
		{
			// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
			const Real inv =  1.0 - d;
			const Real mul0 = inv * inv;
			const Real mul1 =  2.0 * d * inv;
			const Real mul2 = d * d;

			return Vector3 ( (x * mul0 + v2.x * mul1 + v3.x * mul2),
				(y * mul0 + v2.y * mul1 + v3.y * mul2),
				(z * mul0 + v2.z * mul1 + v3.z * mul2));
		}
		 
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z);
		}

		//! 返回这个向量是否是在一条线段两点间线性插值的某个点
		/** 假定这个点在线段两点之间
		\param begin 起始向量
		\param end 结束向量
		\return 如果这个点在线段起始和结尾之间返回true，否则返回false */
		inline bool isBetweenPoints(const Vector3& begin, const Vector3& end) const
		{
			const Real f = (end - begin).squaredLength();	
			return squaredDistance(begin) <= f &&
				squaredDistance(end) <= f;
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

		//! Get the rotations that would make a (0,0,1) direction vector point in the same direction as this direction vector.
		/** Thanks to Arras on the Irrlicht forums for this method.  This utility method is very useful for
		orienting scene nodes towards specific targets.  For example, if this vector represents the difference
		between two scene nodes, then applying the result of getHorizontalAngle() to one scene node will point
		it at the other one.
		Example code:
		// Where target and seeker are of type ISceneNode*
		const vector3df toTarget(target->getAbsolutePosition() - seeker->getAbsolutePosition());
		const vector3df requiredRotation = toTarget.getHorizontalAngle();
		seeker->setRotation(requiredRotation);

		\return A rotation vector containing the X (pitch) and Y (raw) rotations (in degrees) that when applied to a
		+Z (e.g. 0, 0, 1) direction vector would make it point in the same direction as this vector. The Z (roll) rotation
		is always 0, since two Euler rotations are sufficient to point in any given direction. */
		Vector3 getHorizontalAngle() const
		{
			Vector3 angle;

			const Real tmp = (atan2((Real)x, (Real)z) * RADTODEG64);
			angle.y = tmp;

			if (angle.y < 0)
				angle.y += 360;
			if (angle.y >= 360)
				angle.y -= 360;

			const Real z1 = Math::Sqrt(x*x + z*z);

			angle.x = (atan2((Real)z1, (Real)y) * RADTODEG64 - 90.0);

			if (angle.x < 0)
				angle.x += 360;
			if (angle.x >= 360)
				angle.x -= 360;

			return angle;
		}

		//! Builds a direction vector from (this) rotation vector.
		/** This vector is assumed to be a rotation vector composed of 3 Euler angle rotations, in degrees.
		The implementation performs the same calculations as using a matrix to do the rotation.

		\param[in] forwards  The direction representing "forwards" which will be rotated by this vector.
		If you do not provide a direction, then the +Z axis (0, 0, 1) will be assumed to be forwards.
		\return A direction vector calculated by rotating the forwards direction by the 3 Euler angles
		(in degrees) represented by this vector. */
		Vector3 rotationToDirection(const Vector3 & forwards = Vector3(0, 0, 1)) const
		{
			const Real cr = cos(DEGTORAD64 * x);
			const Real sr = sin(DEGTORAD64 * x);
			const Real cp = cos(DEGTORAD64 * y);
			const Real sp = sin(DEGTORAD64 * y);
			const Real cy = cos(DEGTORAD64 * z);
			const Real sy = sin(DEGTORAD64 * z);

			const Real srsp = sr*sp;
			const Real crsp = cr*sp;

			const Real pseudoMatrix[] = {
				(cp*cy), (cp*sy), (-sp),
				(srsp*cy - cr*sy), (srsp*sy + cr*cy), (sr*cp),
				(crsp*cy + sr*sy), (crsp*sy - sr*cy), (cr*cp) };

			return Vector3(
				(forwards.x * pseudoMatrix[0] +
				forwards.y * pseudoMatrix[3] +
				forwards.z * pseudoMatrix[6]),
				(forwards.x * pseudoMatrix[1] +
				forwards.y * pseudoMatrix[4] +
				forwards.z * pseudoMatrix[7]),
				(forwards.x * pseudoMatrix[2] +
				forwards.y * pseudoMatrix[5] +
				forwards.z * pseudoMatrix[8]));
		}

		//! Rotates the vector by a specified number of degrees around the Z axis and the specified center.
		/** \param degrees: Number of degrees to rotate around the Z axis.
		\param center: The center of the rotation. */
		void rotateXYBy(FLOAT64 degrees, const Vector3& center = Vector3())
		{
			degrees *= DEGTORAD64;
			FLOAT64 cs = cos(degrees);
			FLOAT64 sn = sin(degrees);
			x -= center.x;
			y -= center.y;
			set((x*cs - y*sn), (x*sn + y*cs), z);
			x += center.x;
			y += center.y;
		}

		//! Rotates the vector by a specified number of degrees around the X axis and the specified center.
		/** \param degrees: Number of degrees to rotate around the X axis.
		\param center: The center of the rotation. */
		void rotateYZBy(FLOAT64 degrees, const Vector3& center = Vector3())
		{
			degrees *= DEGTORAD64;
			FLOAT64 cs = cos(degrees);
			FLOAT64 sn = sin(degrees);
			z -= center.z;
			y -= center.y;
			set(x, (y*cs - z*sn), (y*sn + z*cs));
			z += center.z;
			y += center.y;
		}

		//! Rotates the vector by a specified number of degrees around the Y axis and the specified center.
		/** \param degrees Number of degrees to rotate around the Y axis.
		\param center The center of the rotation. */
		void rotateXZBy(FLOAT64 degrees, const Vector3& center = Vector3())
		{
			degrees *= DEGTORAD64;
			FLOAT64 cs = cos(degrees);
			FLOAT64 sn = sin(degrees);
			x -= center.x;
			z -= center.z;
			set((x*cs - z*sn), y, (x*sn + z*cs));
			x += center.x;
			z += center.z;
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