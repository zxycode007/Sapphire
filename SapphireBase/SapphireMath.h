#ifndef __SAPPHIRE_MATH__
#define __SAPPHIRE_MATH__


#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"









namespace Sapphire
{


	/**
	对指定角度的弧度制包装类
	@remarks
	Radian 的值可与Degree互换, 和转换 这些自动进行
	*/
	class Radian
	{

		Real mRad;

	public:
		explicit Radian(Real r = 0) : mRad(r) {};
		//注意Degree在头文件后面，需要在Prerequisites预先定义class Degree ，不然会编译器找不到该类定义
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
	对指定角度的角度制包装类
	@remarks
	Radian 的值可与Degree互换, 和转换 这些自动进行
	*/
	class Degree
	{
		Real mDeg;

	public:

		explicit Degree(Real d = 0) : mDeg(d) {}
		//注意Degree在头文件后面，需要在Prerequisites预先定义class Degree ，不然会编译器找不到该类定义
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

	////////////////////////////Radian重载函数的实现//////////////////////////////////////
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

		//这个数组快速计算以2位底0-512对数最近的整数
		//这个函数返回设置值的最大有效位数
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
		判断这个数是否是2的幂
		注意 0和1也会返回true，  n>1
		*/
		template<typename T>
		static FORCEINLINE bool isPO2(T n)
		{
			return (n & (n - 1)) == 0;
		}
	};

	/**
	  @brief 
	  该类定义了一个值作为默认的角度类型，由Math：：setAngleUnit
	  @remark
	  角度值可以在两种类型中自动转换
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

		//角度单位
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
		/// 存放常用的log(2)的值
		static const Real LOG2;


		/** 平方函数
		返回求fValue的平方
		*/
		static inline Real Sqr(Real fValue) { return fValue*fValue; }

		/** 平方根函数
		@param
		求fValue的平方根
		注意：这个函数CPU开销极大,如不需要精确值，用squredLength替代
		*/
		static inline Real Sqrt(Real fValue) { return Real(sqrt(fValue)); }


		/** 求平方根的倒数
		用于向量的标准化		 
		*/
		static Real InvSqrt(Real fValue);

		/** 创建一个单位长度的随机数
		@return
		一个范围在[0,1]的随机数
		*/
		static Real UnitRandom();

		static inline bool isNaN(Real f)
		{
			return f != f;
		}

		/**
		用指定的范围创建一个随机数
		@param
		fLow 范围的下界
		@param
		fHigh 范围的上界
		@return

		*/
		static Real RangeRandom(Real fLow, Real fHigh);


		/** 创建一个随机范围在[-1,1]的随机的数.
		*/
		static Real SymmetricRandom();

		/** 
		比较两个Real型，误差使用公差
		*/
		static bool RealEqual(Real a, Real b,
			Real tolerance = std::numeric_limits<Real>::epsilon());

		/*
		判断浮点数是否为0
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
		反余函数
		*/
		static Radian ACos(Real fValue);

		/** 
		反正弦函数
		*/
		static Radian ASin(Real fValue);

		/** 
		反正切函数
		*/
		static inline Radian ATan(Real fValue) { return Radian(atan(fValue)); }

		/** 两个值之间的反正切函数
		@param
		fY 要计算反正切的第一个值
		@param
		fX 要计算反正切的第二个值
		*/
		static inline Radian ATan2(Real fY, Real fX) { return Radian(atan2(fY, fX)); }

		/*
		 角度转弧度
		 返回的Real
		*/
		static  Real DegreesToRadians(Real degrees);
		/*
		 弧度转角度
		 返回Real
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

		/** 正弦函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Sin(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}
		/** 正弦函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Sin(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue)) : SinTable(fValue);
		}
		/** 余弦函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Cos(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}
		/** 余弦函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Cos(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		/** 正切函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Tan(const Radian& fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}
		/** 正切函数
		@param
		fValue 弧度角
		@param
		useTables 如果为true，使用三角函数查找表
		*/
		static inline Real Tan(Real fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue)) : TanTable(fValue);
		}

		/**
		指数函数
		*/
		static inline Real Exp(Real fValue) { return Real(exp(fValue)); }


		/**
		对数函数
		*/
		static inline Real Log(Real fValue) { return Real(log(fValue)); }

		/**
		2为底对数函数
		*/
		static inline Real Log2(Real fValue) { return Real(log(fValue) / LOG2); }

		/**
		N为底对数函数
		*/
		static inline Real LogN(Real base, Real fValue) { return Real(log(fValue) / log(base)); }

		/**
		幂函数
		*/
		static inline Real Pow(Real fBase, Real fExponent) { return Real(pow(fBase, fExponent)); }


		/** 
		绝对值函数
		*/
		static inline Real Abs(Real fValue) { return Real(fabs(fValue)); }

		/** 
		绝对值函数
		@param
		fValue 角度值， 求一个角度的绝对值
		*/
		static inline Degree Abs(const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }

		/** 
		绝对值函数
		@param
		fValue 弧度值， 求一个弧度的绝对值
		*/
		static inline Radian Abs(const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }


		/** 射线 / 平面相交, 返回bool结果和距离 */
		static std::pair<bool, Real> intersects(const Ray& ray, const Plane& plane);

		/** 射线 / 球体相交, 返回bool结果和距离 */
		static std::pair<bool, Real> intersects(const Ray& ray, const Sphere& sphere,
			bool discardInside = true);

		/** 射线 / 盒子相交, 返回bool结果并距离 */
		static std::pair<bool, Real> intersects(const Ray& ray, const AxisAlignedBox& box);

		/** 
		射线/盒子  相交， 返回布尔结果和两个相交处的距离
		@param
		ray 射线
		@param
		box 射线
		@param
		d1 一个Real型指针去取回由射线原点到近相交处的距离，可以为NULL,它意味着不用关心交互处的距离
		@param
		d2 一个Real型指针去取回由射线原点到远近机处的距离，可以为NULL,它意味着不用关心交互处的距离
		@return
		如果射线与盒子相机，返回true，并且将近相交处距离返回d1,远相交处距离返回d2，保证0<=d1<=d2
		@par
		如果射线不能盒子相交， false，并且d1,d2不能被修改 
		*/
		static bool intersects(const Ray& ray, const AxisAlignedBox& box,
			Real* d1, Real* d2);

		/** 射线/ 三角形相交,返回布尔和距离
		@param
		ray 射线
		@param
		a 三角形的第一个顶点
		@param
		b 三角形的第二个顶点
		@param
		c 三角形的第三个顶点
		@param
		normal 这个三角形平面的法线(传递而不是计算，调用者必须依据先求出法线),不需要标准化
		@param
		positiveSide 与三角形正面相交
		@param
		negativeSide 与三角形反面相交
		@return
		如果射线与三角形相交，pair返回true和交点与射线原点的距离
		@par
		如果射线没有和三角形相交，pair返回false和0
		*/
		static std::pair<bool, Real> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c, const Vector3& normal,
			bool positiveSide = true, bool negativeSide = true);

		
		
		
		/** 射线/ 三角形相交,返回布尔和距离（不带法线）
		@param
		ray 射线
		@param
		a 三角形的第一个顶点
		@param
		b 三角形的第二个顶点
		@param
		c 三角形的第三个顶点
		@param
		positiveSide 与三角形正面相交
		@param
		negativeSide 与三角形反面相交
		@return
		如果射线与三角形相交，pair返回true和交点与射线原点的距离
		@par
		如果射线没有和三角形相交，pair返回false和0
		*/
		static std::pair<bool, Real> intersects(const Ray& ray, const Vector3& a,
			const Vector3& b, const Vector3& c,
			bool positiveSide = true, bool negativeSide = true);

		/** 球体/盒子的相交测试 */
		static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

		/** 平面/盒子相交测试 */
		static bool intersects(const Plane& plane, const AxisAlignedBox& box);

		/** 射线/凸平面列表相交性测试（vector）
		@param ray 射线
		@param planeList 一个构成凸容积平面列表
		@param normalIsOutside 法线是否在外部
		*/
		static std::pair<bool, Real> intersects(
			const Ray& ray, const vector<Plane>::type& planeList,
			bool normalIsOutside);

		/** 射线/凸平面列表相交性测试（List）
		@param ray 射线
		@param plaeList 一个构成凸容积平面列表
		@param normalIsOutside 法线是否在外部
		*/
		static std::pair<bool, Real> intersects(
			const Ray& ray, const list<Plane>::type& planeList,
			bool normalIsOutside);

		/** 球体 / 平面相交性测试
		@remarks NB 用一个plane.getDistance(sphere.getCenter()) 来取得更多的细节
		*/
		static bool intersects(const Sphere& sphere, const Plane& plane);


		/** 计算对于给定位置集合个纹理坐标的切线空间向量*/
		static Vector3 calculateTangentSpaceVector(
			const Vector3& position1, const Vector3& position2, const Vector3& position3,
			Real u1, Real v1, Real u2, Real v2, Real u3, Real v3);

		/** 通过传递一个平面构建一个反射矩阵 */
		static Matrix4 buildReflectionMatrix(const Plane& p);
		/** 计算一个面法线， 包含W分量它是自原点的偏移量*/
		static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** 计算一个面法线， 没有 w-信息  */
		static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** 计算一个面法线而不用标准化, 包含W分量它是自原点的偏移量 */
		static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** 计算一个面法线不用标准化， 没有w-信息 */
		static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** 由给定的偏移和缩放值参数创建一个值基于高斯(正态)分布函数 
		*/
		static Real gaussianDistribution(Real x, Real offset = 0.0f, Real scale = 1.0f);

		/**
		构建观察矩阵
		*/
		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation,
			const Matrix4* reflectMatrix = 0);

		/** 从一个边界盒子取得边界半径值 */
		static Real boundingRadiusFromAABB(const AxisAlignedBox& aabb);

		//判断两个浮点数是否相等
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
		四舍五入
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
		//////////////////不对外由API内部调用//////////////////////
		// 由API使用的角度单位类型
		static AngleUnit msAngleUnit;
		
		/// 由构造函数来确定的三角函数表
		static int mTrigTableSize;

		/// Radian = 三角函数表弧度步进因子 ( mTrigTableSize / 2 * PI )
		static Real mTrigTableFactor;
		static Real* mSinTable;
		static Real* mTanTable;

		/** 私有函数：创建三角函数表
		*/
		void buildTrigTables();

		static Real SinTable(Real fValue);
		static Real TanTable(Real fValue);

	public:
		/** 默认构造函数
		@param
		三角函数表 可选参数去设置表的大小，用于查询Sin Cos Tan
		*/
		Math(unsigned int trigTableSize = 4096);

		~Math();

		
	};

	/////////////////////////////////////////////内联函数//////////////////////////////////////

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