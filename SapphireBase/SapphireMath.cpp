#include "SapphireMath.h"
#include "SapphireQMath.h"
#include "SapphireMatrix3.h"
#include "SapphireMatrix4.h"
#include "SapphireRay.h"
#include "SapphireSphere.h"
#include "SapphirePlane.h"
#include "SapphirePlaneBoundedVolume.h"


namespace Sapphire
{

	const Real Math::POS_INFINITY = std::numeric_limits<Real>::infinity();
	const Real Math::NEG_INFINITY = -std::numeric_limits<Real>::infinity();
	const Real Math::_PI = Real(4.0 * atan(1.0));
	const Real Math::TWO_PI = Real(2.0 * PI);
	const Real Math::HALF_PI = Real(0.5 * PI);
	const Real Math::fDeg2Rad = PI / Real(180.0);
	const Real Math::fRad2Deg = Real(180.0) / PI;
	const Real Math::LOG2 = log(Real(2.0));

	//三角函数表
	int Math::mTrigTableSize;
	//角度单位
	Math::AngleUnit Math::msAngleUnit;
	//三角函数表步进因子
	Real  Math::mTrigTableFactor;
	Real *Math::mSinTable = NULL;
	Real *Math::mTanTable = NULL;

	Math::Math(unsigned int trigTableSize)
	{
	
		//角度类型设为角度
		msAngleUnit = AU_DEGREE;
		//三角函数的函数表
		mTrigTableSize = trigTableSize;
		//三角函数表因子  单位步进
		mTrigTableFactor = mTrigTableSize / Math::TWO_PI;
		//分配内存
		mSinTable = SAPPHIRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);
		mTanTable = SAPPHIRE_ALLOC_T(Real, mTrigTableSize, MEMCATEGORY_GENERAL);

		buildTrigTables();
	}
	Math::~Math()
	{
		SAPPHIRE_FREE(mSinTable, MEMCATEGORY_GENERAL);
		SAPPHIRE_FREE(mTanTable, MEMCATEGORY_GENERAL);
	}

	/**
	@biref  构建三角函数表
	*/
	void Math::buildTrigTables(void)
	{
		// 构建三角函数查询表
		Real angle;
		for (int i = 0; i < mTrigTableSize; ++i)
		{
			//依次求出每个角度值的正弦和余弦值
			angle = Math::TWO_PI * i / mTrigTableSize;
			mSinTable[i] = sin(angle);
			mTanTable[i] = tan(angle);
		}
	}

	//通过查表来求正弦的值(弧度制)
	Real Math::SinTable(Real fValue)
	{
		// 转换范围到索引值，如果需要的进行包装
		int idx;
		if (fValue >= 0)
		{
			//计算出索引
			idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
		}
		else
		{
			idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
		}

		return mSinTable[idx];
	}
	//-----------------------------------------------------------------------
	Real Math::TanTable(Real fValue)
	{
		int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
		return mTanTable[idx];
	}


	Real Math::RadiansToAngleUnits(Real radians)
	{
		if (msAngleUnit == AU_DEGREE)
			return radians * fRad2Deg;
		else
			return radians;
	}

	//-----------------------------------------------------------------------
	Real Math::DegreesToAngleUnits(Real degrees)
	{
		if (msAngleUnit == AU_RADIAN)
			return degrees * fDeg2Rad;
		else
			return degrees;
	}

	Real Math::AngleUnitsToDegrees(Real angleunits)
	{
		if (msAngleUnit == AU_RADIAN)
			return angleunits * fRad2Deg;
		else
			return angleunits;
	}

	Real Math::AngleUnitsToRadians(Real angleunits)
	{
		if (msAngleUnit == AU_DEGREE)
			return angleunits * fDeg2Rad;
		else
			return angleunits;
	}

	Real Math::Sign(Real fValue)
	{
		if (fValue > 0.0)
			return 1.0;

		if (fValue < 0.0)
			return -1.0;

		return 0.0;
	}

	/** 
	@brief 单位随机
	*/
	Real Math::UnitRandom()
	{
		return asm_rand() / asm_rand_max();
	}

	/**
	@bire 求反余弦值函数(弧度制)
	*/
	Radian Math::ACos(Real fValue)
	{
		if (-1.0 < fValue)
		{
			if (fValue < 1.0)
				return Radian(acos(fValue));
			else
				return Radian(0.0);
		}
		else
		{
			return Radian(PI);
		}
	}

	Radian Math::ASin(Real fValue)
	{
		if (-1.0 < fValue)
		{
			if (fValue < 1.0)
				return Radian(asin(fValue));
			else
				return Radian(HALF_PI);
		}
		else
		{
			return Radian(-HALF_PI);
		}
	}

	/**
	求平方根的倒数
	*/
	Real Math::InvSqrt(Real fValue)
	{
		return Real(asm_rsq(fValue));
	}

	/** 
	用指定的范围创建一个随机数
	@param
	fLow 范围的下界
	@param
	fHigh 范围的上界
	@return
	
	*/
	Real Math::RangeRandom(Real fLow, Real fHigh)
	{
		return (fHigh - fLow)*UnitRandom() + fLow;
	}

	/** 
	创建一个随机范围在[-1,1]的随机的数.
	*/
	Real Math::SymmetricRandom()
	{
		return 2.0f * UnitRandom() - 1.0f;
	}

	/**
	比较两个Real型，误差使用公差
	*/
	bool Math::RealEqual(Real a, Real b, Real tolerance)
	{
		if (fabs(b - a) <= tolerance)
			return true;
		else
			return false;
	}

	/**
	@bire 角度值转弧度制
	*/
    Real Math::DegreesToRadians(Real degrees) { return degrees * Math::fDeg2Rad; }
	/**
	@bire 弧度值转角度制
	*/
	Real Math::RadiansToDegrees(Real radians) { return radians * Math::fRad2Deg; }



	//Radian::Radian(const Degree& d) : mRad(d.valueRadians()){};

	Degree::Degree(const Radian& r) : mDeg(r.valueDegrees()) {};


	//-----------------------------------------------------------------------
	//射线和平面相交性测试
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Plane& plane)
	{
		//平面方程N dot (P-P0) = 0
		//Nx*(x-x0)+Ny*(y-y0)+Nz(z-z0) = 0
		// Nx*x-Nx*x0 + Ny*y-Ny*y0 + Nz*z-Nz*z0  = 0
		// Nx*x+Ny*y+Nz*z - Nx*x0 - Ny*y0 -Nz*z0 = 0
		// //化成通用的Ax+Bx+Cx+D = 0 其中Nx=a Ny=b Nz =c   - Nx*x0 - Ny*y0 -Nz*z0=d;
		// 参数化直线方程P(x,y,z) = P0(x0,y0,z0)+Vt
		// x = x0 + Vx*t
		// y = y0 + Vy*t
		// z = z0 + Vz*t
		// 注意：平面x0,y0,z0和直线x0,y0,z0 上不是一个点，d需要单独表示
		//代入平面方程得：
		//Nx*(x0+Vx*t) + Ny*(y0+Vy*t) + Nz*(z0+Vz*t)  + d = 0
		// Nx*x0 + Nx*Vx*t + Ny*y0 + Ny*Vy*t + Nz*z0 + Nz*Vz*t + d = 0
		//t(Nx*Vx+Ny*Vy+Nz*Vz) + Nx*x0+Ny*y0+Nz*z0 +d = 0;
		// t = -(Nx*x0+Ny*y0+Nz*z0 +d) / (Nx*Vx+Ny*Vy+Nz*Vz)
		 
		//分母项： 平面法线与射线方向向量求点积
		Real denom = plane.normal.dotProduct(ray.getDirection());
		if (Math::Abs(denom) < std::numeric_limits<Real>::epsilon())
		{
			// 平行
			return std::pair<bool, Real>(false, 0);
		}
		else
		{
			
			Real nom = plane.normal.dotProduct(ray.getOrigin()) + plane.d;
			Real t = -(nom / denom);
			return std::pair<bool, Real>(t >= 0, t);
		}

	}
	//-----------------------------------------------------------------------
	//射线与平面列表的相交性判断
	std::pair<bool, Real> Math::intersects(const Ray& ray,
		const vector<Plane>::type& planes, bool normalIsOutside)
	{
		list<Plane>::type planesList;
		for (vector<Plane>::type::const_iterator i = planes.begin(); i != planes.end(); ++i)
		{
			planesList.push_back(*i);
		}
		return intersects(ray, planesList, normalIsOutside);
	}
	//-----------------------------------------------------------------------
	//射线与平面列表的相交性判断
	std::pair<bool, Real> Math::intersects(const Ray& ray,
		const list<Plane>::type& planes, bool normalIsOutside)
	{
		list<Plane>::type::const_iterator planeit, planeitend;
		planeitend = planes.end();
		bool allInside = true;
		std::pair<bool, Real> ret;
		std::pair<bool, Real> end;
		ret.first = false;
		ret.second = 0.0f;
		end.first = false;
		end.second = 0;


		// 我们不直接传递，需要Plane::Side的接口, 它的结果递归包含在Math功能中
		//指定平面判断标准，正面或反面
		Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

		for (planeit = planes.begin(); planeit != planeitend; ++planeit)
		{
			const Plane& plane = *planeit;
			// 判断射线原点是否在平面正/负半空间
			if (plane.getSide(ray.getOrigin()) == outside)
			{
				allInside = false;
				// 单个平面的相交性测试
				std::pair<bool, Real> planeRes =
					ray.intersects(plane);
				if (planeRes.first)
				{
					// 相交
					ret.first = true;
					// 用求得平面列表中和射线的最大距离的结果
					ret.second = std::max(ret.second, planeRes.second);
				}
				else
				{
					ret.first = false;
					ret.second = 0.0f;
					return ret;
				}
			}
			else
			{
				std::pair<bool, Real> planeRes =
					ray.intersects(plane);
				if (planeRes.first)
				{
					if (!end.first)
					{
						end.first = true;
						end.second = planeRes.second;
					}
					else
					{
						end.second = std::min(planeRes.second, end.second);
					}

				}

			}
		}
		//所有的都在内部
		if (allInside)
		{
			// 相交性测试在0距离，在容积内部
			ret.first = true;
			ret.second = 0.0f;
			return ret;
		}

		if (end.first)
		{
			if (end.second < ret.second)
			{
				ret.first = false;
				return ret;
			}
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Sphere& sphere,
		bool discardInside)
	{
		//获取射线的方向向量
		const Vector3& raydir = ray.getDirection();
		// 射线原点到球体中心向量
		const Vector3& rayorig = ray.getOrigin() - sphere.getCenter();
		//球体的半径
		Real radius = sphere.getRadius();

		// 检查射线是否在球体内部
		if (rayorig.squaredLength() <= radius*radius && discardInside)
		{
			return std::pair<bool, Real>(true, 0);
		}

		// 联解
		// 直线方程：P=P0+ V*T
		// 球方程：|X-C|^2 = r^2  其中X是球面坐标， C是球心坐标  r半径
		//  |P0+VT-C|^2 = r^2
		// |vt+(p0-c)|^2 = r^2
		// |vt|^2 + 2vt(p0-c) + |p0-c|^2 = r^2
		// |vt|^2 + 2vt(p0-c) + |p0-c|^2 -r^2 = 0
		// 二次方程根
		// 构建参与标准二次方程求解的系数
		//  t = (-b +/- sqrt(b*b + 4ac)) / 2a
		// a = v^2  b=2*v(p0-c)   c=|p0-c|^2 -r^2
		Real a = raydir.dotProduct(raydir);
		Real b = 2 * rayorig.dotProduct(raydir);
		Real c = rayorig.dotProduct(rayorig) - radius*radius;

		// 计算决定因素
		// 根的判别式delta <0 无实根
		Real d = (b*b) - (4 * a * c);
		if (d < 0)
		{
			// 不相交
			return std::pair<bool, Real>(false, 0);
		}
		else
		{
			// delta >=0 有实根
			// 顺便提及, 如果d=0，那么一个交点，容如果d>2有两个
			// 但是我们只要最接近的,那么就用解的'-'版本
			
			Real t = (-b - Math::Sqrt(d)) / (2 * a);
			if (t < 0)
				t = (-b + Math::Sqrt(d)) / (2 * a);
			return std::pair<bool, Real>(true, t);
		}


	}
	//-----------------------------------------------------------------------
	//射线与AABB盒子的相交性测试
	std::pair<bool, Real> Math::intersects(const Ray& ray, const AxisAlignedBox& box)
	{
		if (box.isNull()) return std::pair<bool, Real>(false, 0);
		if (box.isInfinite()) return std::pair<bool, Real>(true, 0);

		Real lowt = 0.0f;
		Real t;
		bool hit = false;
		Vector3 hitpoint;
		const Vector3& min = box.getMinimum();
		const Vector3& max = box.getMaximum();
		const Vector3& rayorig = ray.getOrigin();
		const Vector3& raydir = ray.getDirection();

		// 检测射线的原点是否在盒子最里面
		if (rayorig > min && rayorig < max)
		{
			return std::pair<bool, Real>(true, 0);
		}

		// 依次检查每个面，只检测最接近最小x的3个
		if (rayorig.x <= min.x && raydir.x > 0)
		{
			//求出当射线x=min.x时的t值
			//P = P0 + Vt
			// t = (P-P0)/V
			// t = (Px - P0x)/Vx = (Py - P0y)/Vy = (Pz - P0z)/Vz
			t = (min.x - rayorig.x) / raydir.x;
			if (t >= 0)
			{
				// 将t代入射线检测边界和距离
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// max的x
		if (rayorig.x >= max.x && raydir.x < 0)
		{
			t = (max.x - rayorig.x) / raydir.x;
			if (t >= 0)
			{
				 
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min y
		if (rayorig.y <= min.y && raydir.y > 0)
		{
			t = (min.y - rayorig.y) / raydir.y;
			if (t >= 0)
			{
				 
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max y
		if (rayorig.y >= max.y && raydir.y < 0)
		{
			t = (max.y - rayorig.y) / raydir.y;
			if (t >= 0)
			{
				 
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min z
		if (rayorig.z <= min.z && raydir.z > 0)
		{
			t = (min.z - rayorig.z) / raydir.z;
			if (t >= 0)
			{
				 
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max z
		if (rayorig.z >= max.z && raydir.z < 0)
		{
			t = (max.z - rayorig.z) / raydir.z;
			if (t >= 0)
			{
				 
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}

		return std::pair<bool, Real>(hit, lowt);

	}
	//-----------------------------------------------------------------------
	bool Math::intersects(const Ray& ray, const AxisAlignedBox& box,
		Real* d1, Real* d2)
	{
		if (box.isNull())
			return false;

		if (box.isInfinite())
		{
			if (d1) *d1 = 0;
			if (d2) *d2 = Math::POS_INFINITY;
			return true;
		}

		const Vector3& min = box.getMinimum();
		const Vector3& max = box.getMaximum();
		const Vector3& rayorig = ray.getOrigin();
		const Vector3& raydir = ray.getDirection();

		//方向的绝对值
		Vector3 absDir;
		absDir[0] = Math::Abs(raydir[0]);
		absDir[1] = Math::Abs(raydir[1]);
		absDir[2] = Math::Abs(raydir[2]);

		// 对轴进行排序，首先确保检测轴最小的浮点错误
		// 求出按大小排列的下标
		int imax = 0, imid = 1, imin = 2;
		if (absDir[0] < absDir[2])
		{
			imax = 2;
			imin = 0;
		}
		if (absDir[1] < absDir[imin])
		{
			imid = imin;
			imin = 1;
		}
		else if (absDir[1] > absDir[imax])
		{
			imid = imax;
			imax = 1;
		}

		Real start = 0, end = Math::POS_INFINITY;

//依次遍历每个轴
//求出每个射线原点到AABB盒子min和max点的每个轴相应的t值
//并对t值进行判断
#define _CALC_AXIS(i)                                       \
    do {                                                    \
        Real denom = 1 / raydir[i];                         \
        Real newstart = (min[i] - rayorig[i]) * denom;  /* 分别求出射线原点到AABB的max和min两点的t值 */    \
        Real newend = (max[i] - rayorig[i]) * denom;        \
        if (newstart > newend) std::swap(newstart, newend);   /* newstart t不应超过newend t   */                 \
        if (newstart > end || newend < start) return false;       /* 极值判断 */\
        if (newstart > start) start = newstart;             \
        if (newend < end) end = newend;                     \
	    } while(0)

		// 依次检测每个轴

		_CALC_AXIS(imax);

		if (absDir[imid] < std::numeric_limits<Real>::epsilon())
		{
			// 用中间和最小轴平行，检测边界
			if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
				rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}
		else
		{
			_CALC_AXIS(imid);

			if (absDir[imin] < std::numeric_limits<Real>::epsilon())
			{
				// 最小轴平行，检测边界
				if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
					return false;
			}
			else
			{
				_CALC_AXIS(imin);
			}
		}
#undef _CALC_AXIS

		if (d1) *d1 = start;
		if (d2) *d2 = end;

		return true;
	}
	//-----------------------------------------------------------------------
	//计算射线与三角形的相交性
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Vector3& a,
		const Vector3& b, const Vector3& c, const Vector3& normal,
		bool positiveSide, bool negativeSide)
	{
		//
		// 
		//
		Real t;
		{
			//射线与平面法线的点积
			Real denom = normal.dotProduct(ray.getDirection());

			// 检查相交面
			if (denom > +std::numeric_limits<Real>::epsilon())
			{
				if (!negativeSide)
					return std::pair<bool, Real>(false, 0);
			}
			else if (denom < -std::numeric_limits<Real>::epsilon())
			{
				if (!positiveSide)
					return std::pair<bool, Real>(false, 0);
			}
			else 
			{
				// 平行或者当平面法线没有标准化三角区域过于接近0
				return std::pair<bool, Real>(false, 0);
			}
			//求出射线到三角形屏幕的交点
			//t= -(n * p0 + d)/n * v = ( -(- n*a） - n*p0)/ n*v = （n*a - n*p0) /n*v
			t = normal.dotProduct(a - ray.getOrigin()) / denom;

			if (t < 0)
			{
				// 交点在原点后面
				return std::pair<bool, Real>(false, 0);
			}
		}

		//
		// 计算在X,Y,Z最大区域投影平面
		//
		size_t i0, i1;
		{
			Real n0 = Math::Abs(normal[0]);
			Real n1 = Math::Abs(normal[1]);
			Real n2 = Math::Abs(normal[2]);

			i0 = 1; i1 = 2;
			//按从小到大顺序排列法线的三个分量的绝对值
			//i0,i1去前面两个分量
			if (n1 > n2)
			{
				if (n1 > n0) i0 = 0;
			}
			else
			{
				if (n2 > n0) i1 = 0;
			}
		}

		//
		// 检测在三角形上的交点
		//
		{
			//设三角形三个点为A,B,C 则三角形平面内任意一点的参数方程为P=A+U(B-A)+V(C-A)
			//设三角形三个点权重为(0,0),(1,0),(0,1)  U是B-A向量的参数,V是C-A向量的参数
			Real u1 = b[i0] - a[i0];
			Real v1 = b[i1] - a[i1];
			Real u2 = c[i0] - a[i0];
			Real v2 = c[i1] - a[i1];
			Real u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];  //射线在三角平面上的交点坐标
			Real v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

			//
			Real alpha = u0 * v2 - u2 * v0;
			Real beta = u1 * v0 - u0 * v1;
			Real area = u1 * v2 - u2 * v1;

			// 检测最小值和浮点精度错误
			const Real EPSILON = 1e-6f;

			Real tolerance = -EPSILON * area;

			if (area > 0)
			{
				if (alpha < tolerance || beta < tolerance || alpha + beta > area - tolerance)
					return std::pair<bool, Real>(false, 0);
			}
			else
			{
				if (alpha > tolerance || beta > tolerance || alpha + beta < area - tolerance)
					return std::pair<bool, Real>(false, 0);
			}
		}

		return std::pair<bool, Real>(true, t);
	}
	//-----------------------------------------------------------------------
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Vector3& a,
		const Vector3& b, const Vector3& c,
		bool positiveSide, bool negativeSide)
	{
		Vector3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
		return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
	}
	//-----------------------------------------------------------------------
	//AABB盒子与球体相交性判断
	bool Math::intersects(const Sphere& sphere, const AxisAlignedBox& box)
	{
		if (box.isNull()) return false;
		if (box.isInfinite()) return true;

		//使用分离平面
		const Vector3& center = sphere.getCenter();
		Real radius = sphere.getRadius();
		const Vector3& min = box.getMinimum();
		const Vector3& max = box.getMaximum();

		// Arvo算法
		Real s, d = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (center.ptr()[i] < min.ptr()[i])
			{
				s = center.ptr()[i] - min.ptr()[i];
				d += s * s;
			}
			else if (center.ptr()[i] > max.ptr()[i])
			{
				s = center.ptr()[i] - max.ptr()[i];
				d += s * s;
			}
		}
		return d <= radius * radius;

	}
	//-----------------------------------------------------------------------
	//AABB盒子与平面相交
	bool Math::intersects(const Plane& plane, const AxisAlignedBox& box)
	{
		return (plane.getSide(box) == Plane::BOTH_SIDE);
	}
	//-----------------------------------------------------------------------
	bool Math::intersects(const Sphere& sphere, const Plane& plane)
	{
		return (
			Math::Abs(plane.getDistance(sphere.getCenter()))
			<= sphere.getRadius());
	}
	//-----------------------------------------------------------------------
	//计算切线空间向量
	Vector3 Math::calculateTangentSpaceVector(
		const Vector3& position1, const Vector3& position2, const Vector3& position3,
		Real u1, Real v1, Real u2, Real v2, Real u3, Real v3)
	{
		//三角形的边0 是沿着传入三角形顶点的一条边的向量，
		//而边1则沿着另外一条边的向量。使用叉积可求得法向量
		Vector3 side0 = position1 - position2;
		Vector3 side1 = position3 - position1;
		//计算面法线
		Vector3 normal = side1.crossProduct(side0);
		normal.normalise();  //标准化
		//现在使用一个公式去计算切向量
		Real deltaV0 = v1 - v2;       
		Real deltaV1 = v3 - v1;
		Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
		tangent.normalise();
		//计算次法线
		Real deltaU0 = u1 - u2;
		Real deltaU1 = u3 - u1;
		Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
		binormal.normalise();
		//现在，我们使用切量的叉积得到一个向量，它指向和我们上面法线计算相同的方向
		//如果，它指向负空间(与法线点积小于0)，那么必须反正副法线和切线
        //这是由于三角形在由切线空间到对象空间时会被镜像需要翻转切线
		Vector3 tangentCross = tangent.crossProduct(binormal);
		if (tangentCross.dotProduct(normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		return tangent;

	}
	//-----------------------------------------------------------------------
	//构建反射矩阵
	Matrix4 Math::buildReflectionMatrix(const Plane& p)
	{
		return Matrix4(
			-2 * p.normal.x * p.normal.x + 1, -2 * p.normal.x * p.normal.y, -2 * p.normal.x * p.normal.z, -2 * p.normal.x * p.d,
			-2 * p.normal.y * p.normal.x, -2 * p.normal.y * p.normal.y + 1, -2 * p.normal.y * p.normal.z, -2 * p.normal.y * p.d,
			-2 * p.normal.z * p.normal.x, -2 * p.normal.z * p.normal.y, -2 * p.normal.z * p.normal.z + 1, -2 * p.normal.z * p.d,
			0, 0, 0, 1);
	}
	//-----------------------------------------------------------------------
	Vector4 Math::calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = calculateBasicFaceNormal(v1, v2, v3);
		//现在设置w() 表示三个点到原点的距离
		return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
	}
	//-----------------------------------------------------------------------
	Vector3 Math::calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
		normal.normalise();
		return normal;
	}
	//-----------------------------------------------------------------------
	Vector4 Math::calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
		return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
	}
	//-----------------------------------------------------------------------
	Vector3 Math::calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
		return normal;
	}
	//-----------------------------------------------------------------------
	//高斯分布
	Real Math::gaussianDistribution(Real x, Real offset, Real scale)
	{
		Real nom = Math::Exp(
			-Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
		Real denom = scale * Math::Sqrt(2 * Math::_PI);

		return nom / denom;

	}
	//---------------------------------------------------------------------
	//构建观察矩阵
	Matrix4 Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation,
		const Matrix4* reflectMatrix)
	{
		Matrix4 viewMatrix;

		// 观察矩阵是
		//
		//  [ Lx  Uy  Dz  Tx  ]
		//  [ Lx  Uy  Dz  Ty  ]
		//  [ Lx  Uy  Dz  Tz  ]
		//  [ 0   0   0   1   ]
		//
		//  当 T = -(Transposed(Rot) * Pos)

		// 这个用3X3矩阵计算更有效率
		Matrix3 rot;
		orientation.ToRotationMatrix(rot);

		// 是转换关系到新的轴
		Matrix3 rotT = rot.Transpose();
		Vector3 trans = -rotT * position;

		// 构建最终矩阵
		viewMatrix = Matrix4::IDENTITY;
		viewMatrix = rotT; // 填充上三角3X3矩阵
		viewMatrix.setTrans(trans);
		//viewMatrix[0][3] = trans.x;
		//viewMatrix[1][3] = trans.y;
		//viewMatrix[2][3] = trans.z;

		// 处理反射矩阵
		if (reflectMatrix)
		{
			viewMatrix = viewMatrix * (*reflectMatrix);
		}

		return viewMatrix;

	}
	//---------------------------------------------------------------------
	Real Math::boundingRadiusFromAABB(const AxisAlignedBox& aabb)
	{
		Vector3 max = aabb.getMaximum();
		Vector3 min = aabb.getMinimum();

		Vector3 magnitude = max;
		magnitude.makeCeil(-max);
		magnitude.makeCeil(min);
		magnitude.makeCeil(-min);

		return magnitude.length();
	}

	
}