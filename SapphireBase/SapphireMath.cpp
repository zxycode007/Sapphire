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

	//���Ǻ�����
	int Math::mTrigTableSize;
	//�Ƕȵ�λ
	Math::AngleUnit Math::msAngleUnit;
	//���Ǻ�����������
	Real  Math::mTrigTableFactor;
	Real *Math::mSinTable = NULL;
	Real *Math::mTanTable = NULL;

	Math::Math(unsigned int trigTableSize)
	{
	
		//�Ƕ�������Ϊ�Ƕ�
		msAngleUnit = AU_DEGREE;
		//���Ǻ����ĺ�����
		mTrigTableSize = trigTableSize;
		//���Ǻ���������  ��λ����
		mTrigTableFactor = mTrigTableSize / Math::TWO_PI;
		//�����ڴ�
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
	@biref  �������Ǻ�����
	*/
	void Math::buildTrigTables(void)
	{
		// �������Ǻ�����ѯ��
		Real angle;
		for (int i = 0; i < mTrigTableSize; ++i)
		{
			//�������ÿ���Ƕ�ֵ�����Һ�����ֵ
			angle = Math::TWO_PI * i / mTrigTableSize;
			mSinTable[i] = sin(angle);
			mTanTable[i] = tan(angle);
		}
	}

	//ͨ������������ҵ�ֵ(������)
	Real Math::SinTable(Real fValue)
	{
		// ת����Χ������ֵ�������Ҫ�Ľ��а�װ
		int idx;
		if (fValue >= 0)
		{
			//���������
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
	@brief ��λ���
	*/
	Real Math::UnitRandom()
	{
		return asm_rand() / asm_rand_max();
	}

	/**
	@bire ������ֵ����(������)
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
	��ƽ�����ĵ���
	*/
	Real Math::InvSqrt(Real fValue)
	{
		return Real(asm_rsq(fValue));
	}

	/** 
	��ָ���ķ�Χ����һ�������
	@param
	fLow ��Χ���½�
	@param
	fHigh ��Χ���Ͻ�
	@return
	
	*/
	Real Math::RangeRandom(Real fLow, Real fHigh)
	{
		return (fHigh - fLow)*UnitRandom() + fLow;
	}

	/** 
	����һ�������Χ��[-1,1]���������.
	*/
	Real Math::SymmetricRandom()
	{
		return 2.0f * UnitRandom() - 1.0f;
	}

	/**
	�Ƚ�����Real�ͣ����ʹ�ù���
	*/
	bool Math::RealEqual(Real a, Real b, Real tolerance)
	{
		if (fabs(b - a) <= tolerance)
			return true;
		else
			return false;
	}

	/**
	@bire �Ƕ�ֵת������
	*/
    Real Math::DegreesToRadians(Real degrees) { return degrees * Math::fDeg2Rad; }
	/**
	@bire ����ֵת�Ƕ���
	*/
	Real Math::RadiansToDegrees(Real radians) { return radians * Math::fRad2Deg; }



	//Radian::Radian(const Degree& d) : mRad(d.valueRadians()){};

	Degree::Degree(const Radian& r) : mDeg(r.valueDegrees()) {};


	//-----------------------------------------------------------------------
	//���ߺ�ƽ���ཻ�Բ���
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Plane& plane)
	{
		//ƽ�淽��N dot (P-P0) = 0
		//Nx*(x-x0)+Ny*(y-y0)+Nz(z-z0) = 0
		// Nx*x-Nx*x0 + Ny*y-Ny*y0 + Nz*z-Nz*z0  = 0
		// Nx*x+Ny*y+Nz*z - Nx*x0 - Ny*y0 -Nz*z0 = 0
		// //����ͨ�õ�Ax+Bx+Cx+D = 0 ����Nx=a Ny=b Nz =c   - Nx*x0 - Ny*y0 -Nz*z0=d;
		// ������ֱ�߷���P(x,y,z) = P0(x0,y0,z0)+Vt
		// x = x0 + Vx*t
		// y = y0 + Vy*t
		// z = z0 + Vz*t
		// ע�⣺ƽ��x0,y0,z0��ֱ��x0,y0,z0 �ϲ���һ���㣬d��Ҫ������ʾ
		//����ƽ�淽�̵ã�
		//Nx*(x0+Vx*t) + Ny*(y0+Vy*t) + Nz*(z0+Vz*t)  + d = 0
		// Nx*x0 + Nx*Vx*t + Ny*y0 + Ny*Vy*t + Nz*z0 + Nz*Vz*t + d = 0
		//t(Nx*Vx+Ny*Vy+Nz*Vz) + Nx*x0+Ny*y0+Nz*z0 +d = 0;
		// t = -(Nx*x0+Ny*y0+Nz*z0 +d) / (Nx*Vx+Ny*Vy+Nz*Vz)
		 
		//��ĸ� ƽ�淨�������߷�����������
		Real denom = plane.normal.dotProduct(ray.getDirection());
		if (Math::Abs(denom) < std::numeric_limits<Real>::epsilon())
		{
			// ƽ��
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
	//������ƽ���б���ཻ���ж�
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
	//������ƽ���б���ཻ���ж�
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


		// ���ǲ�ֱ�Ӵ��ݣ���ҪPlane::Side�Ľӿ�, ���Ľ���ݹ������Math������
		//ָ��ƽ���жϱ�׼���������
		Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

		for (planeit = planes.begin(); planeit != planeitend; ++planeit)
		{
			const Plane& plane = *planeit;
			// �ж�����ԭ���Ƿ���ƽ����/����ռ�
			if (plane.getSide(ray.getOrigin()) == outside)
			{
				allInside = false;
				// ����ƽ����ཻ�Բ���
				std::pair<bool, Real> planeRes =
					ray.intersects(plane);
				if (planeRes.first)
				{
					// �ཻ
					ret.first = true;
					// �����ƽ���б��к����ߵ�������Ľ��
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
		//���еĶ����ڲ�
		if (allInside)
		{
			// �ཻ�Բ�����0���룬���ݻ��ڲ�
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
		//��ȡ���ߵķ�������
		const Vector3& raydir = ray.getDirection();
		// ����ԭ�㵽������������
		const Vector3& rayorig = ray.getOrigin() - sphere.getCenter();
		//����İ뾶
		Real radius = sphere.getRadius();

		// ��������Ƿ��������ڲ�
		if (rayorig.squaredLength() <= radius*radius && discardInside)
		{
			return std::pair<bool, Real>(true, 0);
		}

		// ����
		// ֱ�߷��̣�P=P0+ V*T
		// �򷽳̣�|X-C|^2 = r^2  ����X���������꣬ C����������  r�뾶
		//  |P0+VT-C|^2 = r^2
		// |vt+(p0-c)|^2 = r^2
		// |vt|^2 + 2vt(p0-c) + |p0-c|^2 = r^2
		// |vt|^2 + 2vt(p0-c) + |p0-c|^2 -r^2 = 0
		// ���η��̸�
		// ���������׼���η�������ϵ��
		//  t = (-b +/- sqrt(b*b + 4ac)) / 2a
		// a = v^2  b=2*v(p0-c)   c=|p0-c|^2 -r^2
		Real a = raydir.dotProduct(raydir);
		Real b = 2 * rayorig.dotProduct(raydir);
		Real c = rayorig.dotProduct(rayorig) - radius*radius;

		// �����������
		// �����б�ʽdelta <0 ��ʵ��
		Real d = (b*b) - (4 * a * c);
		if (d < 0)
		{
			// ���ཻ
			return std::pair<bool, Real>(false, 0);
		}
		else
		{
			// delta >=0 ��ʵ��
			// ˳���ἰ, ���d=0����ôһ�����㣬�����d>2������
			// ��������ֻҪ��ӽ���,��ô���ý��'-'�汾
			
			Real t = (-b - Math::Sqrt(d)) / (2 * a);
			if (t < 0)
				t = (-b + Math::Sqrt(d)) / (2 * a);
			return std::pair<bool, Real>(true, t);
		}


	}
	//-----------------------------------------------------------------------
	//������AABB���ӵ��ཻ�Բ���
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

		// ������ߵ�ԭ���Ƿ��ں���������
		if (rayorig > min && rayorig < max)
		{
			return std::pair<bool, Real>(true, 0);
		}

		// ���μ��ÿ���棬ֻ�����ӽ���Сx��3��
		if (rayorig.x <= min.x && raydir.x > 0)
		{
			//���������x=min.xʱ��tֵ
			//P = P0 + Vt
			// t = (P-P0)/V
			// t = (Px - P0x)/Vx = (Py - P0y)/Vy = (Pz - P0z)/Vz
			t = (min.x - rayorig.x) / raydir.x;
			if (t >= 0)
			{
				// ��t�������߼��߽�;���
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
		// max��x
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

		//����ľ���ֵ
		Vector3 absDir;
		absDir[0] = Math::Abs(raydir[0]);
		absDir[1] = Math::Abs(raydir[1]);
		absDir[2] = Math::Abs(raydir[2]);

		// ���������������ȷ���������С�ĸ������
		// �������С���е��±�
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

//���α���ÿ����
//���ÿ������ԭ�㵽AABB����min��max���ÿ������Ӧ��tֵ
//����tֵ�����ж�
#define _CALC_AXIS(i)                                       \
    do {                                                    \
        Real denom = 1 / raydir[i];                         \
        Real newstart = (min[i] - rayorig[i]) * denom;  /* �ֱ��������ԭ�㵽AABB��max��min�����tֵ */    \
        Real newend = (max[i] - rayorig[i]) * denom;        \
        if (newstart > newend) std::swap(newstart, newend);   /* newstart t��Ӧ����newend t   */                 \
        if (newstart > end || newend < start) return false;       /* ��ֵ�ж� */\
        if (newstart > start) start = newstart;             \
        if (newend < end) end = newend;                     \
	    } while(0)

		// ���μ��ÿ����

		_CALC_AXIS(imax);

		if (absDir[imid] < std::numeric_limits<Real>::epsilon())
		{
			// ���м����С��ƽ�У����߽�
			if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
				rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}
		else
		{
			_CALC_AXIS(imid);

			if (absDir[imin] < std::numeric_limits<Real>::epsilon())
			{
				// ��С��ƽ�У����߽�
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
	//���������������ε��ཻ��
	std::pair<bool, Real> Math::intersects(const Ray& ray, const Vector3& a,
		const Vector3& b, const Vector3& c, const Vector3& normal,
		bool positiveSide, bool negativeSide)
	{
		//
		// 
		//
		Real t;
		{
			//������ƽ�淨�ߵĵ��
			Real denom = normal.dotProduct(ray.getDirection());

			// ����ཻ��
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
				// ƽ�л��ߵ�ƽ�淨��û�б�׼������������ڽӽ�0
				return std::pair<bool, Real>(false, 0);
			}
			//������ߵ���������Ļ�Ľ���
			//t= -(n * p0 + d)/n * v = ( -(- n*a�� - n*p0)/ n*v = ��n*a - n*p0) /n*v
			t = normal.dotProduct(a - ray.getOrigin()) / denom;

			if (t < 0)
			{
				// ������ԭ�����
				return std::pair<bool, Real>(false, 0);
			}
		}

		//
		// ������X,Y,Z�������ͶӰƽ��
		//
		size_t i0, i1;
		{
			Real n0 = Math::Abs(normal[0]);
			Real n1 = Math::Abs(normal[1]);
			Real n2 = Math::Abs(normal[2]);

			i0 = 1; i1 = 2;
			//����С����˳�����з��ߵ����������ľ���ֵ
			//i0,i1ȥǰ����������
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
		// ������������ϵĽ���
		//
		{
			//��������������ΪA,B,C ��������ƽ��������һ��Ĳ�������ΪP=A+U(B-A)+V(C-A)
			//��������������Ȩ��Ϊ(0,0),(1,0),(0,1)  U��B-A�����Ĳ���,V��C-A�����Ĳ���
			Real u1 = b[i0] - a[i0];
			Real v1 = b[i1] - a[i1];
			Real u2 = c[i0] - a[i0];
			Real v2 = c[i1] - a[i1];
			Real u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];  //����������ƽ���ϵĽ�������
			Real v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

			//
			Real alpha = u0 * v2 - u2 * v0;
			Real beta = u1 * v0 - u0 * v1;
			Real area = u1 * v2 - u2 * v1;

			// �����Сֵ�͸��㾫�ȴ���
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
	//AABB�����������ཻ���ж�
	bool Math::intersects(const Sphere& sphere, const AxisAlignedBox& box)
	{
		if (box.isNull()) return false;
		if (box.isInfinite()) return true;

		//ʹ�÷���ƽ��
		const Vector3& center = sphere.getCenter();
		Real radius = sphere.getRadius();
		const Vector3& min = box.getMinimum();
		const Vector3& max = box.getMaximum();

		// Arvo�㷨
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
	//AABB������ƽ���ཻ
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
	//�������߿ռ�����
	Vector3 Math::calculateTangentSpaceVector(
		const Vector3& position1, const Vector3& position2, const Vector3& position3,
		Real u1, Real v1, Real u2, Real v2, Real u3, Real v3)
	{
		//�����εı�0 �����Ŵ��������ζ����һ���ߵ�������
		//����1����������һ���ߵ�������ʹ�ò������÷�����
		Vector3 side0 = position1 - position2;
		Vector3 side1 = position3 - position1;
		//�����淨��
		Vector3 normal = side1.crossProduct(side0);
		normal.normalise();  //��׼��
		//����ʹ��һ����ʽȥ����������
		Real deltaV0 = v1 - v2;       
		Real deltaV1 = v3 - v1;
		Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
		tangent.normalise();
		//����η���
		Real deltaU0 = u1 - u2;
		Real deltaU1 = u3 - u1;
		Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
		binormal.normalise();
		//���ڣ�����ʹ�������Ĳ���õ�һ����������ָ����������淨�߼�����ͬ�ķ���
		//�������ָ�򸺿ռ�(�뷨�ߵ��С��0)����ô���뷴�������ߺ�����
        //���������������������߿ռ䵽����ռ�ʱ�ᱻ������Ҫ��ת����
		Vector3 tangentCross = tangent.crossProduct(binormal);
		if (tangentCross.dotProduct(normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		return tangent;

	}
	//-----------------------------------------------------------------------
	//�����������
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
		//��������w() ��ʾ�����㵽ԭ��ľ���
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
	//��˹�ֲ�
	Real Math::gaussianDistribution(Real x, Real offset, Real scale)
	{
		Real nom = Math::Exp(
			-Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
		Real denom = scale * Math::Sqrt(2 * Math::_PI);

		return nom / denom;

	}
	//---------------------------------------------------------------------
	//�����۲����
	Matrix4 Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation,
		const Matrix4* reflectMatrix)
	{
		Matrix4 viewMatrix;

		// �۲������
		//
		//  [ Lx  Uy  Dz  Tx  ]
		//  [ Lx  Uy  Dz  Ty  ]
		//  [ Lx  Uy  Dz  Tz  ]
		//  [ 0   0   0   1   ]
		//
		//  �� T = -(Transposed(Rot) * Pos)

		// �����3X3����������Ч��
		Matrix3 rot;
		orientation.ToRotationMatrix(rot);

		// ��ת����ϵ���µ���
		Matrix3 rotT = rot.Transpose();
		Vector3 trans = -rotT * position;

		// �������վ���
		viewMatrix = Matrix4::IDENTITY;
		viewMatrix = rotT; // ���������3X3����
		viewMatrix.setTrans(trans);
		//viewMatrix[0][3] = trans.x;
		//viewMatrix[1][3] = trans.y;
		//viewMatrix[2][3] = trans.z;

		// ���������
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