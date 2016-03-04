#ifndef __SAPPHIRE_MATH__
#define __SAPPHIRE_MATH__


#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"









namespace Sapphire
{


	/**
	��ָ���ǶȵĻ����ư�װ��
	@remarks
	Radian ��ֵ����Degree����, ��ת�� ��Щ�Զ�����
	*/
	class Radian
	{

		Real mRad;

	public:
		explicit Radian(Real r = 0) : mRad(r) {};
		//ע��Degree��ͷ�ļ����棬��Ҫ��PrerequisitesԤ�ȶ���class Degree ����Ȼ��������Ҳ������ඨ��
		Radian(const Degree& d) ;
		Real valueRadians() const { return mRad; }
		Real valueDegrees() const;

		Radian operator - (const Radian& r) const { return Radian(mRad - r.mRad); }
		Radian& operator = (const Real& f) { mRad = f; return *this; }
		Radian& operator = (const Radian& r) { mRad = r.mRad; return *this; }
		Radian& operator = (const Degree& d);
		const Radian& operator + () const { return *this; }
		Radian operator + (const Radian& r) const { return Radian(mRad + r.mRad); }
		Radian operator + (const Degree& d) const; 
		Radian& operator += (const Radian& r) { mRad += r.mRad; return *this; }
		Radian& operator += (const Degree& d);
		Radian operator - () const { return Radian(-mRad); }

		Radian operator - (const Degree& d) const;
		Radian& operator -= (const Radian& r) { mRad -= r.mRad; return *this; }
		Radian& operator -= (const Degree& d);
		Radian operator * (Real f) const { return Radian(mRad * f); }
		Radian operator * (const Radian& f) const { return Radian(mRad * f.mRad); }
		Radian& operator *= (Real f) { mRad *= f; return *this; }
		Radian operator / (Real f) const { return Radian(mRad / f); }
		Radian& operator /= (Real f) { mRad /= f; return *this; }

		bool operator <  (const Radian& r) const { return mRad <  r.mRad; }
		bool operator <= (const Radian& r) const { return mRad <= r.mRad; }
		bool operator == (const Radian& r) const { return mRad == r.mRad; }
		bool operator != (const Radian& r) const { return mRad != r.mRad; }
		bool operator >= (const Radian& r) const { return mRad >= r.mRad; }
		bool operator >(const Radian& r) const { return mRad >  r.mRad; }
		 

		Real valueAngleUnits() const;

		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Radian& v)
		{
			o << "Radian(" << v.valueRadians() << ")";
			return o;
		}
	};

	

	/**
	��ָ���ǶȵĽǶ��ư�װ��
	@remarks
	Radian ��ֵ����Degree����, ��ת�� ��Щ�Զ�����
	*/
	class Degree
	{
		Real mDeg;

	public:

		explicit Degree(Real d = 0) : mDeg(d) {}
		//ע��Degree��ͷ�ļ����棬��Ҫ��PrerequisitesԤ�ȶ���class Degree ����Ȼ��������Ҳ������ඨ��
		Degree(const Radian& r);

		Real valueDegrees() const { return mDeg; }
		Real valueRadians() const;

		Degree& operator = (const Real& f) { mDeg = f; return *this; }
		Degree& operator = (const Degree& d) { mDeg = d.mDeg; return *this; }
		Degree& operator = (const Radian& r) { mDeg = r.valueDegrees(); return *this; }

		const Degree& operator + () const { return *this; }
		Degree operator + (const Degree& d) const { return Degree(mDeg + d.mDeg); }
		Degree operator + (const Radian& r) const { return Degree(mDeg + r.valueDegrees()); }
		Degree& operator += (const Degree& d) { mDeg += d.mDeg; return *this; }
		Degree& operator += (const Radian& r) { mDeg += r.valueDegrees(); return *this; }
		Degree operator - () const { return Degree(-mDeg); }
		Degree operator - (const Degree& d) const { return Degree(mDeg - d.mDeg); }
		Degree operator - (const Radian& r) const { return Degree(mDeg - r.valueDegrees()); }
		Degree& operator -= (const Degree& d) { mDeg -= d.mDeg; return *this; }
		Degree& operator -= (const Radian& r) { mDeg -= r.valueDegrees(); return *this; }
		Degree operator * (Real f) const { return Degree(mDeg * f); }
		Degree operator * (const Degree& f) const { return Degree(mDeg * f.mDeg); }
		Degree& operator *= (Real f) { mDeg *= f; return *this; }
		Degree operator / (Real f) const { return Degree(mDeg / f); }
		Degree& operator /= (Real f) { mDeg /= f; return *this; }

		bool operator <  (const Degree& d) const { return mDeg <  d.mDeg; }
		bool operator <= (const Degree& d) const { return mDeg <= d.mDeg; }
		bool operator == (const Degree& d) const { return mDeg == d.mDeg; }
		bool operator != (const Degree& d) const { return mDeg != d.mDeg; }
		bool operator >= (const Degree& d) const { return mDeg >= d.mDeg; }
		bool operator >(const Degree& d) const { return mDeg >  d.mDeg; }

		Real valueAngleUnits() const;
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Degree& v)
		{
			o << "Degree(" << v.valueDegrees() << ")";
			return o;
		}
	};

	////////////////////////////Radian���غ�����ʵ��//////////////////////////////////////
	inline Radian::Radian(const Degree& d) : mRad(d.valueRadians()) {
	}
	inline Radian& Radian::operator = (const Degree& d) {
		mRad = d.valueRadians(); return *this;
	}
	inline Radian Radian::operator + (const Degree& d) const {
		return Radian(mRad + d.valueRadians());
	}
	inline Radian& Radian::operator += (const Degree& d) {
		mRad += d.valueRadians();
		return *this;
	}
	inline Radian Radian::operator - (const Degree& d) const {
		return Radian(mRad - d.valueRadians());
	}
	inline Radian& Radian::operator -= (const Degree& d) {
		mRad -= d.valueRadians();
		return *this;
	}




	class _SapphireExport BitWise
	{
	public:

		//���������ټ�����2λ��0-512�������������
		//���������������ֵ�������Чλ��
		static FORCEINLINE unsigned int mostSignificantBitSet(unsigned int value)
		{
			unsigned int result = 0;
			while (value != 0) {
				++result;
				value >>= 1;
			}
			return result - 1;
		}

		/**
		�ж�������Ƿ���2����
		ע�� 0��1Ҳ�᷵��true��  n>1
		*/
		template<typename T>
		static FORCEINLINE bool isPO2(T n)
		{
			return (n & (n - 1)) == 0;
		}
	};

	/**
	  @brief 
	  ���ඨ����һ��ֵ��ΪĬ�ϵĽǶ����ͣ���Math����setAngleUnit
	  @remark
	  �Ƕ�ֵ�����������������Զ�ת��
	*/
	class Angle
	{
		Real mAngle;
	public:
		explicit Angle(Real angle) : mAngle(angle) {}
		operator Radian() const;
		operator Degree() const;
	};


	class _SapphireExport Math
	{


	public:

		//�Ƕȵ�λ
		enum AngleUnit
		{
			AU_DEGREE,
			AU_RADIAN
		};

		static const Real POS_INFINITY;
		static const Real NEG_INFINITY;
		static const Real _PI;
		static const Real TWO_PI;
		static const Real HALF_PI;
		static const Real fDeg2Rad;
		static const Real fRad2Deg;
		/// ��ų��õ�log(2)��ֵ
		static const Real LOG2;


		/** ƽ������
		������fValue��ƽ��
		*/
		static inline Real Sqr(Real fValue) { return fValue*fValue; }

		/** ƽ��������
		@param
		��fValue��ƽ����
		ע�⣺�������CPU��������,�粻��Ҫ��ȷֵ����squredLength���
		*/
		static inline Real Sqrt(Real fValue) { return Real(sqrt(fValue)); }


		/** ��ƽ�����ĵ���
		���������ı�׼��		 
		*/
		static Real InvSqrt(Real fValue);

		/** ����һ����λ���ȵ������
		@return
		һ����Χ��[0,1]�������
		*/
		static Real UnitRandom();

		static inline bool isNaN(Real f)
		{
			return f != f;
		}

		/**
		��ָ���ķ�Χ����һ�������
		@param
		fLow ��Χ���½�
		@param
		fHigh ��Χ���Ͻ�
		@return

		*/
		static Real RangeRandom(Real fLow, Real fHigh);


		/** ����һ�������Χ��[-1,1]���������.
		*/
		static Real SymmetricRandom();

		/** 
		�Ƚ�����Real�ͣ����ʹ�ù���
		*/
		static bool RealEqual(Real a, Real b,
			Real tolerance = std::numeric_limits<Real>::epsilon());

		/*
		�жϸ������Ƿ�Ϊ0
		*/
		static inline bool iszero(const Real a, const Real tolerance = std::numeric_limits<Real>::epsilon())
		{
			return fabs(a) <= tolerance;
		}

		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert(minval <= maxval && "Invalid clamp range");
			return _max(_min(val, maxval), minval);
		}


		/** 
		���ຯ��
		*/
		static Radian ACos(Real fValue);

		/** 
		�����Һ���
		*/
		static Radian ASin(Real fValue);

		/** 
		�����к���
		*/
		static inline Radian ATan(Real fValue) { return Radian(atan(fValue)); }

		/** ����ֵ֮��ķ����к���
		@param
		fY Ҫ���㷴���еĵ�һ��ֵ
		@param
		fX Ҫ���㷴���еĵڶ���ֵ
		*/
		static inline Radian ATan2(Real fY, Real fX) { return Radian(atan2(fY, fX)); }

		/*
		 �Ƕ�ת����
		 ���ص�Real
		*/
		static  Real DegreesToRadians(Real degrees);
		/*
		 ����ת�Ƕ�
		 ����Real
		*/
		static  Real RadiansToDegrees(Real radians);

		static Real RadiansToAngleUnits(Real radians);

		static Real DegreesToAngleUnits(Real degrees);

		static Real AngleUnitsToDegrees(Real units);

		static Real AngleUnitsToRadians(Real units);



		static Real Sign(Real fValue);

		static inline Radian Sign(const Radian& rValue)
		{
			return Radian(Sign(rValue.valueRadians()));
		}
		static inline Degree Sign(const Degree& dValue)
		{
			return Degree(Sign(dValue.valueDegrees()));
		}

		/** ���Һ���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Sin(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}
		/** ���Һ���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Sin(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue)) : SinTable(fValue);
		}
		/** ���Һ���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Cos(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}
		/** ���Һ���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Cos(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		/** ���к���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Tan(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}
		/** ���к���
		@param
		fValue ���Ƚ�
		@param
		useTables ���Ϊtrue��ʹ�����Ǻ������ұ�
		*/
		static inline Real Tan(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue)) : TanTable(fValue);
		}

		/**
		ָ������
		*/
		static inline Real Exp(Real fValue) { return Real(exp(fValue)); }


		/**
		��������
		*/
		static inline Real Log(Real fValue) { return Real(log(fValue)); }

		/**
		2Ϊ�׶�������
		*/
		static inline Real Log2(Real fValue) { return Real(log(fValue) / LOG2); }

		/**
		NΪ�׶�������
		*/
		static inline Real LogN(Real base, Real fValue) { return Real(log(fValue) / log(base)); }

		/**
		�ݺ���
		*/
		static inline Real Pow(Real fBase, Real fExponent) { return Real(pow(fBase, fExponent)); }


		/** 
		����ֵ����
		*/
		static inline Real Abs(Real fValue) { return Real(fabs(fValue)); }

		/** 
		����ֵ����
		@param
		fValue �Ƕ�ֵ�� ��һ���Ƕȵľ���ֵ
		*/
		static inline Degree Abs(const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }

		/** 
		����ֵ����
		@param
		fValue ����ֵ�� ��һ�����ȵľ���ֵ
		*/
		static inline Radian Abs(const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }


		/** ���� / ƽ���ཻ, ����bool����;��� */
		static std::pair<bool, Real> intersects(const Ray& ray, const Plane& plane);

		/** ���� / �����ཻ, ����bool����;��� */
		static std::pair<bool, Real> intersects(const Ray& ray, const Sphere& sphere,
			bool discardInside = true);

		/** ���� / �����ཻ, ����bool��������� */
		static std::pair<bool, Real> intersects(const Ray& ray, const AxisAlignedBox& box);

		/** 
		����/����  �ཻ�� ���ز�������������ཻ���ľ���
		@param
		ray ����
		@param
		box ����
		@param
		d1 һ��Real��ָ��ȥȡ��������ԭ�㵽���ཻ���ľ��룬����ΪNULL,����ζ�Ų��ù��Ľ������ľ���
		@param
		d2 һ��Real��ָ��ȥȡ��������ԭ�㵽Զ�������ľ��룬����ΪNULL,����ζ�Ų��ù��Ľ������ľ���
		@return
		���������������������true�����ҽ����ཻ�����뷵��d1,Զ�ཻ�����뷵��d2����֤0<=d1<=d2
		@par
		������߲��ܺ����ཻ�� false������d1,d2���ܱ��޸� 
		*/
		static bool intersects(const Ray& ray, const AxisAlignedBox& box,
			Real* d1, Real* d2);

		/** ����/ �������ཻ,���ز����;���
		@param
		ray ����
		@param
		a �����εĵ�һ������
		@param
		b �����εĵڶ�������
		@param
		c �����εĵ���������
		@param
		normal ���������ƽ��ķ���(���ݶ����Ǽ��㣬�����߱����������������),����Ҫ��׼��
		@param
		positiveSide �������������ཻ
		@param
		negativeSide �������η����ཻ
		@return
		����������������ཻ��pair����true�ͽ���������ԭ��ľ���
		@par
		�������û�к��������ཻ��pair����false��0
		*/
		static std::pair<bool, Real> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c, const Vector3& normal,
			bool positiveSide = true, bool negativeSide = true);

		
		
		
		/** ����/ �������ཻ,���ز����;��루�������ߣ�
		@param
		ray ����
		@param
		a �����εĵ�һ������
		@param
		b �����εĵڶ�������
		@param
		c �����εĵ���������
		@param
		positiveSide �������������ཻ
		@param
		negativeSide �������η����ཻ
		@return
		����������������ཻ��pair����true�ͽ���������ԭ��ľ���
		@par
		�������û�к��������ཻ��pair����false��0
		*/
		static std::pair<bool, Real> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c,
			bool positiveSide = true, bool negativeSide = true);

		/** ����/���ӵ��ཻ���� */
		static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

		/** ƽ��/�����ཻ���� */
		static bool intersects(const Plane& plane, const AxisAlignedBox& box);

		/** ����/͹ƽ���б��ཻ�Բ��ԣ�vector��
		@param ray ����
		@param planeList һ������͹�ݻ�ƽ���б�
		@param normalIsOutside �����Ƿ����ⲿ
		*/
		static std::pair<bool, Real> intersects(
			const Ray& ray, const vector<Plane>::type& planeList,
			bool normalIsOutside);

		/** ����/͹ƽ���б��ཻ�Բ��ԣ�List��
		@param ray ����
		@param plaeList һ������͹�ݻ�ƽ���б�
		@param normalIsOutside �����Ƿ����ⲿ
		*/
		static std::pair<bool, Real> intersects(
			const Ray& ray, const list<Plane>::type& planeList,
			bool normalIsOutside);

		/** ���� / ƽ���ཻ�Բ���
		@remarks NB ��һ��plane.getDistance(sphere.getCenter()) ��ȡ�ø����ϸ��
		*/
		static bool intersects(const Sphere& sphere, const Plane& plane);


		/** ������ڸ���λ�ü��ϸ�������������߿ռ�����*/
		static Vector3 calculateTangentSpaceVector(
			const Vector3& position1, const Vector3& position2, const Vector3& position3,
			Real u1, Real v1, Real u2, Real v2, Real u3, Real v3);

		/** ͨ������һ��ƽ�湹��һ��������� */
		static Matrix4 buildReflectionMatrix(const Plane& p);
		/** ����һ���淨�ߣ� ����W����������ԭ���ƫ����*/
		static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** ����һ���淨�ߣ� û�� w-��Ϣ  */
		static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** ����һ���淨�߶����ñ�׼��, ����W����������ԭ���ƫ���� */
		static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** ����һ���淨�߲��ñ�׼���� û��w-��Ϣ */
		static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** �ɸ�����ƫ�ƺ�����ֵ��������һ��ֵ���ڸ�˹(��̬)�ֲ����� 
		*/
		static Real gaussianDistribution(Real x, Real offset = 0.0f, Real scale = 1.0f);

		/**
		�����۲����
		*/
		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation,
			const Matrix4* reflectMatrix = 0);

		/** ��һ���߽����ȡ�ñ߽�뾶ֵ */
		static Real boundingRadiusFromAABB(const AxisAlignedBox& aabb);

		//�ж������������Ƿ����
		static inline bool equals(const Real a, const Real b, const Real tolerance = std::numeric_limits<Real>::epsilon())
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		static inline bool equals32(const FLOAT32 a, const FLOAT32 b, const FLOAT32 tolerance = std::numeric_limits<FLOAT32>::epsilon())
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		static inline bool equals64(const FLOAT64 a, const FLOAT64 b, const FLOAT64 tolerance = std::numeric_limits<FLOAT64>::epsilon())
		{
			return (a + tolerance >= b) && (a - tolerance <= b);
		}

		/**
		��������
		*/
		static SINT32 round2i(Real val)
		{
			return static_cast<SINT32>(val + 0.5);
		}
		template<class T>
		static const T _max(const T a, const T b)
		{
			return a > b ? a : b;
		}
		template<class T>
		static const T _min(const T a, const T b)
		{
			return a < b ? a : b;
		}
		template<class T>
		static const T& max_(const T& a, const T& b)
		{
			return a > b ? a : b;
		}
		template<class T>
		static const T& min_(const T& a, const T& b)
		{
			return a < b ? a : b;
		}
		template<class T>
		static const T& max3(const T& a, const T& b, const T& c)
		{
 			return a < b ? max_(b, c) : max_(a, c);
		}
		
		template<class T>
		static const T& min3(const T& a, const T& b, const T& c)
		{
			return a < b ? min_(a, c) : min_(b, c);
		}
		template<class T>
		static T lerp(const T& a, const T& b, const FLOAT32 t)
		{
			return (T)(a*(1.f - t)) + (b*t);
		}

	protected:
		//////////////////��������API�ڲ�����//////////////////////
		// ��APIʹ�õĽǶȵ�λ����
		static AngleUnit msAngleUnit;
		
		/// �ɹ��캯����ȷ�������Ǻ�����
		static int mTrigTableSize;

		/// Radian = ���Ǻ������Ȳ������� ( mTrigTableSize / 2 * PI )
		static Real mTrigTableFactor;
		static Real* mSinTable;
		static Real* mTanTable;

		/** ˽�к������������Ǻ�����
		*/
		void buildTrigTables();

		static Real SinTable(Real fValue);
		static Real TanTable(Real fValue);

	public:
		/** Ĭ�Ϲ��캯��
		@param
		���Ǻ����� ��ѡ����ȥ���ñ�Ĵ�С�����ڲ�ѯSin Cos Tan
		*/
		Math(unsigned int trigTableSize = 4096);

		~Math();

		
	};

	/////////////////////////////////////////////��������//////////////////////////////////////

	inline Real Radian::valueDegrees() const {
		return  Math::RadiansToDegrees(this->mRad);
	};

	inline Real Degree::valueRadians() const{
		return  Math::DegreesToRadians(this->mDeg);
	};

	inline Real Radian::valueAngleUnits() const
	{
		return Math::RadiansToAngleUnits(mRad);
	}

	

	inline Angle::operator Radian() const
	{
		return Radian(Math::AngleUnitsToRadians(mAngle));
	}

	inline Angle::operator Degree() const
	{
		return Degree(Math::AngleUnitsToDegrees(mAngle));
	}

	//! Constant for reciprocal of PI.
	const FLOAT32 RECIPROCAL_PI = 1.0f / PI;

	//! Constant for half of PI.
	const FLOAT32 HALF_PI = PI / 2.0f;

	//! Constant for 64bit PI.
	const FLOAT64 PI64 = 3.1415926535897932384626433832795028841971693993751;

	//! Constant for 64bit reciprocal of PI.
	const FLOAT64 RECIPROCAL_PI64 = 1.0 / PI64;

	//! 32bit Constant for converting from degrees to radians
	const FLOAT64 DEGTORAD = PI / 180.0f;

	//! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
	const FLOAT64 RADTODEG = 180.0f / PI;

	//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
	const FLOAT64 DEGTORAD64 = PI64 / 180.0;

	//! 64bit constant for converting from radians to degrees
	const FLOAT64 RADTODEG64 = 180.0 / PI64;
}
#endif