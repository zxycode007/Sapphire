#ifndef _SAPPHIRE_VECTOR4_
#define _SAPPHIRE_VECTOR4_

#include "SapphirePrerequisites.h"
#include "SapphireMatrix3.h"

namespace Sapphire
{
	class _SapphireExport Vector4
	{
	public:
		Real x, y, z, w;

	public:
		inline Vector4()
		{
		}

		inline Vector4(const Real fX, const Real fY, const Real fZ, const Real fW)
			: x(fX), y(fY), z(fZ), w(fW)
		{
		}

		inline explicit Vector4(const Real afCoordinate[4])
			: x(afCoordinate[0]),
			y(afCoordinate[1]),
			z(afCoordinate[2]),
			w(afCoordinate[3])
		{
		}

		inline explicit Vector4(const int afCoordinate[4])
		{
			x = (Real)afCoordinate[0];
			y = (Real)afCoordinate[1];
			z = (Real)afCoordinate[2];
			w = (Real)afCoordinate[3];
		}

		inline explicit Vector4(Real* const r)
			: x(r[0]), y(r[1]), z(r[2]), w(r[3])
		{
		}

		inline explicit Vector4(const Real scaler)
			: x(scaler)
			, y(scaler)
			, z(scaler)
			, w(scaler)
		{
		}

		inline explicit Vector4(const Vector3& rhs)
			: x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
		{
		}

		
		inline void swap(Vector4& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
			std::swap(w, other.w);
		}

		inline Real operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&x + i);
		}

		inline Real& operator [] (const size_t i)
		{
			assert(i < 4);

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

		
		inline Vector4& operator = (const Vector4& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;
			w = rkVector.w;

			return *this;
		}

		inline Vector4& operator = (const Real fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

		inline bool operator == (const Vector4& rkVector) const
		{
			return (x == rkVector.x &&
				y == rkVector.y &&
				z == rkVector.z &&
				w == rkVector.w);
		}

		inline bool operator != (const Vector4& rkVector) const
		{
			return (x != rkVector.x ||
				y != rkVector.y ||
				z != rkVector.z ||
				w != rkVector.w);
		}

		inline Vector4& operator = (const Vector3& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = 1.0f;
			return *this;
		}

		// arithmetic operations
		inline Vector4 operator + (const Vector4& rkVector) const
		{
			return Vector4(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z,
				w + rkVector.w);
		}

		inline Vector4 operator - (const Vector4& rkVector) const
		{
			return Vector4(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z,
				w - rkVector.w);
		}

		inline Vector4 operator * (const Real fScalar) const
		{
			return Vector4(
				x * fScalar,
				y * fScalar,
				z * fScalar,
				w * fScalar);
		}

		inline Vector4 operator * (const Vector4& rhs) const
		{
			return Vector4(
				rhs.x * x,
				rhs.y * y,
				rhs.z * z,
				rhs.w * w);
		}

		inline Vector4 operator / (const Real fScalar) const
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			return Vector4(
				x * fInv,
				y * fInv,
				z * fInv,
				w * fInv);
		}

		inline Vector4 operator / (const Vector4& rhs) const
		{
			return Vector4(
				x / rhs.x,
				y / rhs.y,
				z / rhs.z,
				w / rhs.w);
		}

		inline const Vector4& operator + () const
		{
			return *this;
		}

		inline Vector4 operator - () const
		{
			return Vector4(-x, -y, -z, -w);
		}

		inline friend Vector4 operator * (const Real fScalar, const Vector4& rkVector)
		{
			return Vector4(
				fScalar * rkVector.x,
				fScalar * rkVector.y,
				fScalar * rkVector.z,
				fScalar * rkVector.w);
		}

		inline friend Vector4 operator / (const Real fScalar, const Vector4& rkVector)
		{
			return Vector4(
				fScalar / rkVector.x,
				fScalar / rkVector.y,
				fScalar / rkVector.z,
				fScalar / rkVector.w);
		}

		inline friend Vector4 operator + (const Vector4& lhs, const Real rhs)
		{
			return Vector4(
				lhs.x + rhs,
				lhs.y + rhs,
				lhs.z + rhs,
				lhs.w + rhs);
		}

		inline friend Vector4 operator + (const Real lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs + rhs.x,
				lhs + rhs.y,
				lhs + rhs.z,
				lhs + rhs.w);
		}

		inline friend Vector4 operator - (const Vector4& lhs, Real rhs)
		{
			return Vector4(
				lhs.x - rhs,
				lhs.y - rhs,
				lhs.z - rhs,
				lhs.w - rhs);
		}

		inline friend Vector4 operator - (const Real lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs - rhs.x,
				lhs - rhs.y,
				lhs - rhs.z,
				lhs - rhs.w);
		}

		// arithmetic updates
		inline Vector4& operator += (const Vector4& rkVector)
		{
			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;
			w += rkVector.w;

			return *this;
		}

		inline Vector4& operator -= (const Vector4& rkVector)
		{
			x -= rkVector.x;
			y -= rkVector.y;
			z -= rkVector.z;
			w -= rkVector.w;

			return *this;
		}

		inline Vector4& operator *= (const Real fScalar)
		{
			x *= fScalar;
			y *= fScalar;
			z *= fScalar;
			w *= fScalar;
			return *this;
		}

		inline Vector4& operator += (const Real fScalar)
		{
			x += fScalar;
			y += fScalar;
			z += fScalar;
			w += fScalar;
			return *this;
		}

		inline Vector4& operator -= (const Real fScalar)
		{
			x -= fScalar;
			y -= fScalar;
			z -= fScalar;
			w -= fScalar;
			return *this;
		}

		inline Vector4& operator *= (const Vector4& rkVector)
		{
			x *= rkVector.x;
			y *= rkVector.y;
			z *= rkVector.z;
			w *= rkVector.w;

			return *this;
		}

		inline Vector4& operator /= (const Real fScalar)
		{
			assert(fScalar != 0.0);

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;
			w *= fInv;

			return *this;
		}

		inline Vector4& operator /= (const Vector4& rkVector)
		{
			x /= rkVector.x;
			y /= rkVector.y;
			z /= rkVector.z;
			w /= rkVector.w;

			return *this;
		}

		
		inline Real dotProduct(const Vector4& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
		}
		
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}
		
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Vector4& v)
		{
			o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
			return o;
		}
		
		static const Vector4 ZERO;
	};
}

#endif