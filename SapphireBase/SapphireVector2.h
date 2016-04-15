#ifndef _SAPPHIRE_VECTOR2_
#define _SAPPHIRE_VECTOR2_

#include "SapphirePrerequisites.h"
#include "SapphireMath.h"


namespace Sapphire
{
	/** ��׼2d����
	@remarks
	��2d�ռ���һ�����򣬱���Ϊ������������ϵ�ϵľ���(x,y) ע��λ�ã�������������Ӷ����Ա�����Ϊһ������
	ȡ��������ν������ֵ
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
		������������������
		*/
		inline void swap(Vector2& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
		}
		//�ض����Աѡȡ��
		inline Real operator [] (const size_t i) const
		{
			assert(i < 2);

			return *(&x + i);
		}
		//�ض����Աѡȡ��
		inline Real& operator [] (const size_t i)
		{
			assert(i < 2);

			return *(&x + i);
		}

		/// ָ�����������ֱ�ӿ���
		inline Real* ptr()
		{
			return &x;
		}
		////// ָ�����������ֱ�ӿ���
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

		/** ���������ĳ���
		@warning
		���������Ҫһ��ƽ���������Ǵ�����CPU����������㲻��Ҫ��ȷ�ĳ�����squredLength�������
		*/
		inline Real length() const
		{
			return Math::Sqrt(x * x + y * y);
		}

		/** 
		������������ĳ��ȵ�ƽ��
		@remarks
		��������Ƚ���Ч�� - ��������ʵ�ʳ�����Ҫ�����ţ���Щ���������ܴ�
		�����������һ���������ȵ�ƽ��������ȥƽ����֮ǰҲ�ɵ�ͬ�ڳ��ȡ������Ҫ�ҵ������̵���������Ҫ��ƽ����
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
		
		��������һ�������ľ���
		@warning
		�������Ҳ��Ҫ������CPU�������ڣ���Ϊ��Ҫ��ƽ�����������辫ȷֵ������ squaredDistance()
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
		��������һ�������ĵ����������
		@remarks
		U.V =|U||V| cos�� 
		���ͨ�����ڼ������������ĽǶȡ�����������ǵ�λ����������������Ƕȵ�����ֵ
		���⣬������������������������ĳ��ȵĻ��������ҽǡ�������������������㵽ƽ��ľ���
		@param
		*/
		inline Real dotProduct(const Vector2& vec) const
		{
			return x * vec.x + y * vec.y;
		}

		/** ��׼������
		@remarks
		���������׼��������ʹ����������Ϊ1����Ϊһ����λ����		
		@note
		���������������0�������������������ⲻ��ı����ǵķ���
		@�������������֮ǰ�ĳ���
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
		���������봫������֮����е�
		*/
		inline Vector2 midPoint(const Vector2& vec) const
		{
			return Vector2(
				(x + vec.x) * 0.5f,
				(y + vec.y) * 0.5f);
		}

		/** 
		����������������Ԫ����ȫ�����ڸ�������������true.
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
		���������ķ���(������)�����С�ڱȽ��������򴫵�ֵ��������
		 
		*/
		inline void makeFloor(const Vector2& cmp)
		{
			if (cmp.x < x) x = cmp.x;
			if (cmp.y < y) y = cmp.y;
		}

		/** 
		���������ķ����������ޣ���������ڱȽ��������򴫵�ֵ��������
		*/
		inline void makeCeil(const Vector2& cmp)
		{
			if (cmp.x > x) x = cmp.x;
			if (cmp.y > y) y = cmp.y;
		}

		/** 
		����һ�������Ĵ�ֱ���Ϸ�������up������
		@remarks
		�������������һ�������������������ֱ�������������������ֿ��ܣ��������������֤����������һ���������
		��Ҫ����Ŀ���������Ԫ��Quaternion��
		*/
		inline Vector2 perpendicular(void) const
		{
			return Vector2(-y, x);
		}

		/** 
		�������������������������һ������������������������������2��
		*/
		inline Real crossProduct(const Vector2& rkVector) const
		{
			return x * rkVector.y - y * rkVector.x;
		}

		/** 
		����һ���µ�ͨ������һ������Ƕȶ���������������������
		@remarks
		��������ٶ��������������׼������
		@param
		angle  ƫ��ǶȵĻ���
		*/
		inline Vector2 randomDeviant(Real angle) const
		{

			angle *= Math::UnitRandom() * Math::TWO_PI;
			Real cosa = cos(angle);
			Real sina = sin(angle);
			return  Vector2(cosa * x - sina * y,
				sina * x + cosa * y);
		}

		/** �������0���ȷ���true */
		inline bool isZeroLength(void) const
		{
			Real sqlen = (x * x) + (y * y);
			return (sqlen < (1e-06 * 1e-06));

		}

		/** ����׼��, ��һ������Ȼ���׼��������������Ӱ��*/
		inline Vector2 normalisedCopy(void) const
		{
			Vector2 ret = *this;
			ret.normalise();
			return ret;
		}

		/** 
		�ø����ķ��߼���һ������������ƽ��
		@remarks NB ���� 'this' ָ��Զ�����Ļ��������ǣ���ת��
		*/
		inline Vector2 reflect(const Vector2& normal) const
		{
			return Vector2(*this - (2 * this->dotProduct(normal) * normal));
		}

		/// �����������Ƿ������Ч��ֵ
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y);
		}

		/**	
		��ȡ2��������ĽǶ�
		@remarks
		�������Բ��õ�λ���ȣ����Ǳ����ܱ�ʾ����
		*/
		inline Sapphire::Radian angleBetween(const Sapphire::Vector2& other) const
		{
			//�������ȵĳ˻�
			Sapphire::Real lenProduct = length() * other.length();
			// ������
			if (lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			Sapphire::Real f = dotProduct(other) / lenProduct;
			//������
			f = Sapphire::Math::Clamp(f, (Sapphire::Real) - 1.0, (Sapphire::Real)1.0);
			return Sapphire::Math::ACos(f);
		}

		/**	ȡ������������ĳ���� 
		@remarks
		�������Բ��õ�λ���ȣ����Ǳ����ʾһ������
		�Ƕ���0~2PI
		*/
		inline Sapphire::Radian angleTo(const Sapphire::Vector2& other) const
		{
			//����н�
			Sapphire::Radian angle = angleBetween(other);
			//��������other��������ʱ�뷽��
			if (crossProduct(other)<0)
				angle = (Sapphire::Radian)Sapphire::Math::TWO_PI - angle;

			return angle;
		}


		//! ������������Ƿ�����һ���߶���������Բ�ֵ��ĳ����
		/** �ٶ���������߶�����֮��
		\param begin ��ʼ����
		\param end ��������
		\return �����������߶���ʼ�ͽ�β֮�䷵��true�����򷵻�false */
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


		//! ����һ������������һ������֮��Ĳ�ֵ����
		/** \param other �����ֵ����һ������
		\param d ��ֵ��ֵ��0.0f��1.0f֮�䣩 ע�⣺������getInterpolated_quadratic()�Ĳ�ֵ�����෴��
		\return һ����ֵ�������������޸ı����� */
		Vector2 getInterpolated(const Vector2& other, Real d) const
		{
			Real inv = 1.0f - d;
			return Vector2((other.x*inv + x*d),(other.y*inv + y*d));
		}


		//! ����һ������������һ������֮��Ķ��β�ֵ����
		/** \param v2 �ڶ��������ֵ����
		\param v3 �����������ֵ������ (maximum at 1.0f)
		\param d ��ֵ��ֵ ��0.0f��������������1.0f�����е�����������֮��
		ע��:������getInterpolated()�Ĳ�ֵ�����෴�� 
		\return һ����ֵ�������������޸ı����� */
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


		// ����ĵ�special points
		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 UNIT_SCALE;

		/** ���������
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