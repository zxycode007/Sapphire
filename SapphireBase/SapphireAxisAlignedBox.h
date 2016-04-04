#ifndef _SAPPHIRE_AXISALIGNEDBOX_
#define _SAPPHIRE_AXISALIGNEDBOX_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireMatrix4.h"
#include "SapphireLine3D.h"


namespace Sapphire
{
	/**
	一个与X,Y,Z轴对齐的3d盒子
	@remarks
	这个类表示一个关于轴对称简单盒子。它内部只存放盒子极限两个点。一个是3个轴最小值，宁一个是3个轴最大值。
	这个类是使用轴对称碰撞盒子 axis-aligned bounding box (AABB)做碰撞和可视化测试
	*/
	class _SapphireExport AxisAlignedBox
	{
	public:
		enum Extent
		{
			EXTENT_NULL,
			EXTENT_FINITE,
			EXTENT_INFINITE
		};
	protected:

		Vector3 mMinimum;
		Vector3 mMaximum;
		Extent mExtent;
		mutable Vector3* mCorners;

	public:
		/*
		1-----2
		/|    /|
		/ |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		*/
		//对应顶点枚举
		typedef enum {
			FAR_LEFT_BOTTOM = 0,
			FAR_LEFT_TOP = 1,
			FAR_RIGHT_TOP = 2,
			FAR_RIGHT_BOTTOM = 3,
			NEAR_RIGHT_BOTTOM = 7,
			NEAR_LEFT_BOTTOM = 6,
			NEAR_LEFT_TOP = 5,
			NEAR_RIGHT_TOP = 4
		} CornerEnum;
		inline AxisAlignedBox() : mMinimum(Vector3::ZERO), mMaximum(Vector3::UNIT_SCALE), mCorners(0)
		{
			// 默认为空盒子
			setMinimum(-1.0, -1.0, -1.0);
			setMaximum(1.0, 1.0, 1.0);
			mExtent = EXTENT_NULL;
		}
		inline AxisAlignedBox(Extent e) : mMinimum(Vector3::ZERO), mMaximum(Vector3::UNIT_SCALE), mCorners(0)
		{
			setMinimum(-0.5, -0.5, -0.5);
			setMaximum(0.5, 0.5, 0.5);
			mExtent = e;
		}

		inline AxisAlignedBox(const AxisAlignedBox & rkBox) : mMinimum(Vector3::ZERO), mMaximum(Vector3::UNIT_SCALE), mCorners(0)

		{
			if (rkBox.isNull())
				setNull();
			else if (rkBox.isInfinite())
				setInfinite();
			else
				setExtents(rkBox.mMinimum, rkBox.mMaximum);
		} 

		inline AxisAlignedBox(const Vector3& min, const Vector3& max) : mMinimum(Vector3::ZERO), mMaximum(Vector3::UNIT_SCALE), mCorners(0)
		{
			setExtents(min, max);
		}

		inline AxisAlignedBox(const Vector3& init) : mMinimum(init), mMaximum(init), mCorners(0)
		{
			 
		}
		inline AxisAlignedBox(
			Real mx, Real my, Real mz,
			Real Mx, Real My, Real Mz) : mMinimum(Vector3::ZERO), mMaximum(Vector3::UNIT_SCALE), mCorners(0)
		{
			setExtents(mx, my, mz, Mx, My, Mz);
		}

		AxisAlignedBox& operator=(const AxisAlignedBox& rhs)
		{
			// 指定覆盖到避免复制mCorners
			if (rhs.isNull())
				setNull();
			else if (rhs.isInfinite())
				setInfinite();
			else
				setExtents(rhs.mMinimum, rhs.mMaximum);

			return *this;
		}

		~AxisAlignedBox()
		{
			if (mCorners)
				SAPPHIRE_FREE(mCorners, MEMCATEGORY_SCENE_CONTROL);
		}


		/** 
		获取盒子最小corner
		*/
		inline const Vector3& getMinimum(void) const
		{
			return mMinimum;
		}

		/** 
		获取盒子最小corner的修改版本
		*/
		inline Vector3& getMinimum(void)
		{
			return mMinimum;
		}

		/**
		获取盒子最大的corner
		*/
		inline const Vector3& getMaximum(void) const
		{
			return mMaximum;
		}

		/**获取盒子最大的corner的修改版本
		*/
		inline Vector3& getMaximum(void)
		{
			return mMaximum;
		}
		Vector3 getExtent() const
		{
			return mMaximum - mMinimum;
		}

		/**
		设置盒子最小corner
		*/
		inline void setMinimum(const Vector3& vec)
		{
			mExtent = EXTENT_FINITE;
			mMinimum = vec;
		}

		inline void setMinimum(Real x, Real y, Real z)
		{
			mExtent = EXTENT_FINITE;
			mMinimum.x = x;
			mMinimum.y = y;
			mMinimum.z = z;
		}

		/**
		设置最小Corner的分量，用于从新设置盒子大小
		*/
		inline void setMinimumX(Real x)
		{
			mMinimum.x = x;
		}

		inline void setMinimumY(Real y)
		{
			mMinimum.y = y;
		}

		inline void setMinimumZ(Real z)
		{
			mMinimum.z = z;
		}

		/** 
		设置盒子最大Corner
		*/
		inline void setMaximum(const Vector3& vec)
		{
			mExtent = EXTENT_FINITE;
			mMaximum = vec;
		}

		inline void setMaximum(Real x, Real y, Real z)
		{
			mExtent = EXTENT_FINITE;
			mMaximum.x = x;
			mMaximum.y = y;
			mMaximum.z = z;
		}

		
		inline void setMaximumX(Real x)
		{
			mMaximum.x = x;
		}

		inline void setMaximumY(Real y)
		{
			mMaximum.y = y;
		}

		inline void setMaximumZ(Real z)
		{
			mMaximum.z = z;
		}

		/** 
		同时设置最大和最小内容
		*/
		inline void setExtents(const Vector3& min, const Vector3& max)
		{
			assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
				"The minimum corner of the box must be less than or equal to maximum corner");

			mExtent = EXTENT_FINITE;
			mMinimum = min;
			mMaximum = max;
		}

		inline void setExtents(
			Real mx, Real my, Real mz,
			Real Mx, Real My, Real Mz)
		{
			assert((mx <= Mx && my <= My && mz <= Mz) &&
				"The minimum corner of the box must be less than or equal to maximum corner");

			mExtent = EXTENT_FINITE;

			mMinimum.x = mx;
			mMinimum.y = my;
			mMinimum.z = mz;

			mMaximum.x = Mx;
			mMaximum.y = My;
			mMaximum.z = Mz;

		}

		/**
		返回一个指向8个顶点的指针数组，用于碰撞和非对齐的对象
		@remarks
		如果这些顶点顺序重要的话，他们遵循下面原则：
		4个顶点是在最小Z面（注意：由于Sapphire用右手坐标系，最小Z是在盒子的后面）
		中的最小值作为起点， 并且按逆时针环绕这个面（如果你在盒子外部观察这个面）。
		那最大Z值的4个顶点，起始它们中的最大值，通用是按逆时针环绕
		<pre>
		1-----2
		/|    /|
		/ |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		</pre>
		@remarks 作为使用一个静态成员的实现，确保用的是指的拷贝
		*/
		inline const Vector3* getAllCorners(void) const
		{
			assert((mExtent == EXTENT_FINITE) && "Can't get corners of a null or infinite AAB");

			// 它们的顺序按右手坐标系，最小Z面，起始与最小（所有），并按逆时针环绕面
			// 最大Z面，起始它们最大...
			// 只对于优化和兼容性
			if (!mCorners)
				mCorners = SAPPHIRE_ALLOC_T(Vector3, 8, MEMCATEGORY_SCENE_CONTROL);

			mCorners[0] = mMinimum;
			mCorners[1].x = mMinimum.x; mCorners[1].y = mMaximum.y; mCorners[1].z = mMinimum.z;
			mCorners[2].x = mMaximum.x; mCorners[2].y = mMaximum.y; mCorners[2].z = mMinimum.z;
			mCorners[3].x = mMaximum.x; mCorners[3].y = mMinimum.y; mCorners[3].z = mMinimum.z;

			mCorners[4] = mMaximum;
			mCorners[5].x = mMinimum.x; mCorners[5].y = mMaximum.y; mCorners[5].z = mMaximum.z;
			mCorners[6].x = mMinimum.x; mCorners[6].y = mMinimum.y; mCorners[6].z = mMaximum.z;
			mCorners[7].x = mMaximum.x; mCorners[7].y = mMinimum.y; mCorners[7].z = mMaximum.z;

			return mCorners;
		}

		/** 取得一个顶点的位置
		*/
		Vector3 getCorner(CornerEnum cornerToGet) const
		{
			switch (cornerToGet)
			{
			case FAR_LEFT_BOTTOM:
				return mMinimum;
			case FAR_LEFT_TOP:
				return Vector3(mMinimum.x, mMaximum.y, mMinimum.z);
			case FAR_RIGHT_TOP:
				return Vector3(mMaximum.x, mMaximum.y, mMinimum.z);
			case FAR_RIGHT_BOTTOM:
				return Vector3(mMaximum.x, mMinimum.y, mMinimum.z);
			case NEAR_RIGHT_BOTTOM:
				return Vector3(mMaximum.x, mMinimum.y, mMaximum.z);
			case NEAR_LEFT_BOTTOM:
				return Vector3(mMinimum.x, mMinimum.y, mMaximum.z);
			case NEAR_LEFT_TOP:
				return Vector3(mMinimum.x, mMaximum.y, mMaximum.z);
			case NEAR_RIGHT_TOP:
				return mMaximum;
			default:
				return Vector3();
			}
		}

		_SapphireExport friend std::ostream& operator<<(std::ostream& o, const AxisAlignedBox aab)
		{
			switch (aab.mExtent)
			{
			case EXTENT_NULL:
				o << "AxisAlignedBox(null)";
				return o;

			case EXTENT_FINITE:
				o << "AxisAlignedBox(min=" << aab.mMinimum << ", max=" << aab.mMaximum << ")";
				return o;

			case EXTENT_INFINITE:
				o << "AxisAlignedBox(infinite)";
				return o;

			default:  
				assert(false && "Never reached");
				return o;
			}
		}

		/** 
		合并传递的盒子到当前的盒子.
		*/
		void merge(const AxisAlignedBox& rhs)
		{
			// 判断是否为空，或无限大
			if ((rhs.mExtent == EXTENT_NULL) || (mExtent == EXTENT_INFINITE))
			{
				return;
			}
			// 无限大
			else if (rhs.mExtent == EXTENT_INFINITE)
			{
				mExtent = EXTENT_INFINITE;
			}
			// 除法当前为空
			else if (mExtent == EXTENT_NULL)
			{
				setExtents(rhs.mMinimum, rhs.mMaximum);
			}
			// 
			else
			{
				Vector3 min = mMinimum;
				Vector3 max = mMaximum;
				max.makeCeil(rhs.mMaximum);
				min.makeFloor(rhs.mMinimum);

				setExtents(min, max);
			}

		}

		//! 添加一个点到这个包围盒子
		/** 如果这个点在盒子外面，这个盒子会变大
		\param p: 添加到这个盒子的点 */
		void addInternalPoint(const Vector3& p)
		{
			addInternalPoint(p.x, p.y, p.z);
		}

		//! 添加一个另外的AABB盒子
		/** 这个盒子会成长更多，如果这个新盒子在这个盒子外面的话
		\param b: 要添加到这个盒子的另一个包围盒子*/
		void addInternalBox(const AxisAlignedBox& b)
		{
			addInternalPoint(b.getMaximum());
			addInternalPoint(b.getMinimum());
		}

		//! 添加一个点到这个包围盒子
		/** 如果这个点在盒子外面，这个盒子会变大
		\param x 要添加到这个盒子的点的x坐标
		\param y 要添加到这个盒子的点的y坐标
		\param z 要添加到这个盒子的点的z坐标 */
		void addInternalPoint(Real x, Real y, Real z)
		{
			if (x>mMaximum.x) mMaximum.x = x;
			if (y>mMaximum.y) mMaximum.y = y;
			if (z>mMaximum.z) mMaximum.z = z;

			if (x<mMinimum.x) mMinimum.x = x;
			if (y<mMinimum.y) mMinimum.y = y;
			if (z<mMinimum.z) mMinimum.z = z;
		}

		/** 扩展这个盒子到指定的点
		*/
		inline void merge(const Vector3& point)
		{
			switch (mExtent)
			{
			case EXTENT_NULL:  
				setExtents(point, point);
				return;

			case EXTENT_FINITE:
				mMaximum.makeCeil(point);
				mMinimum.makeFloor(point);
				return;

			case EXTENT_INFINITE: 
				return;
			}

			assert(false && "Never reached");
		}
       
		/**
		重置
		*/
		inline void reset(const Vector3& point)
		{
			setExtents(point, point);
		}

		//! 重设这个AABB盒子
		/** \param initValue 要设置新盒子的对象*/
		void reset(const AxisAlignedBox& initValue)
		{
			*this = initValue;
		}

		/** 
		通过矩阵对盒子进行变换
	 
		*/
		inline void transform(const Matrix4& matrix)
		{
			 
			if (mExtent != EXTENT_FINITE)
				return;

			Vector3 oldMin, oldMax, currentCorner;

			 
			oldMin = mMinimum;
			oldMax = mMaximum;

			 
			setNull();

			// 我们从而按下面顺序计算每个顶点
			// 0, 6, 5, 1, 2, 4 ,7 , 3
			// 这个序列允许我们改变在所有顶点一次改变一个成员

			// 对于每一个，都要用矩阵变换它
			// 会得到结果的顶点和合并结果顶点

			// First corner 
			// min min min
			currentCorner = oldMin;
			merge(matrix * currentCorner);

			// min,min,max
			currentCorner.z = oldMax.z;
			merge(matrix * currentCorner);

			// min max max
			currentCorner.y = oldMax.y;
			merge(matrix * currentCorner);

			// min max min
			currentCorner.z = oldMin.z;
			merge(matrix * currentCorner);

			// max max min
			currentCorner.x = oldMax.x;
			merge(matrix * currentCorner);

			// max max max
			currentCorner.z = oldMax.z;
			merge(matrix * currentCorner);

			// max min max
			currentCorner.y = oldMin.y;
			merge(matrix * currentCorner);

			// max min min
			currentCorner.z = oldMin.z;
			merge(matrix * currentCorner);
		}

		/** 
		通过仿射矩阵变换盒子
	    注意：矩阵必须是仿射矩阵
		*/
		void transformAffine(const Matrix4& m)
		{
			//assert(!m.isAffine());

			// 如果当前是无限的，什么都别做
			if (mExtent != EXTENT_FINITE)
				return;

			Vector3 centre = getCenter();
			Vector3 halfSize = getHalfSize();

			Vector3 newCentre = m.transformAffine(centre);
			Vector3 newHalfSize(
				Math::Abs(m[0][0]) * halfSize.x + Math::Abs(m[0][1]) * halfSize.y + Math::Abs(m[0][2]) * halfSize.z,
				Math::Abs(m[1][0]) * halfSize.x + Math::Abs(m[1][1]) * halfSize.y + Math::Abs(m[1][2]) * halfSize.z,
				Math::Abs(m[2][0]) * halfSize.x + Math::Abs(m[2][1]) * halfSize.y + Math::Abs(m[2][2]) * halfSize.z);

			setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
		}

		/** 
		设置box到null
		*/
		inline void setNull()
		{
			mExtent = EXTENT_NULL;
		}

		 
		inline bool isNull(void) const
		{
			return (mExtent == EXTENT_NULL);
		}
 
		bool isFinite(void) const
		{
			return (mExtent == EXTENT_FINITE);
		}

		 
		inline void setInfinite()
		{
			mExtent = EXTENT_INFINITE;
		}

		 
		bool isInfinite(void) const
		{
			return (mExtent == EXTENT_INFINITE);
		}

		/** 返回这个box使用与另一个相交 */
		inline bool intersects(const AxisAlignedBox& b2) const
		{
			 
			if (this->isNull() || b2.isNull())
				return false;

			 
			if (this->isInfinite() || b2.isInfinite())
				return true;

			// 判断面相交
			if (mMaximum.x < b2.mMinimum.x)
				return false;
			if (mMaximum.y < b2.mMinimum.y)
				return false;
			if (mMaximum.z < b2.mMinimum.z)
				return false;

			if (mMinimum.x > b2.mMaximum.x)
				return false;
			if (mMinimum.y > b2.mMaximum.y)
				return false;
			if (mMinimum.z > b2.mMaximum.z)
				return false;

			 
			return true;

		}

		

		//! Tests if the box intersects with a line
		/** \param linemiddle Center of the line.
		\param linevect Vector of the line.
		\param halflength Half length of the line.
		\return True if there is an intersection, else false. */
		inline bool intersectsWithLine(const Vector3& linemiddle,
			const Vector3& linevect, Real halflength) const
		{
			const Vector3 e = getExtent() * (Real)0.5;
			const Vector3 t = getCenter() - linemiddle;

			if ((fabs(t.x) > e.x + halflength * fabs(linevect.x)) ||
				(fabs(t.y) > e.y + halflength * fabs(linevect.y)) ||
				(fabs(t.z) > e.z + halflength * fabs(linevect.z)))
				return false;

			Real r = e.y * (Real)fabs(linevect.z) + e.z * (Real)fabs(linevect.y);
			if (fabs(t.y*linevect.z - t.z*linevect.y) > r)
				return false;

			r = e.x * (Real)fabs(linevect.z) + e.z * (Real)fabs(linevect.x);
			if (fabs(t.z*linevect.x - t.x*linevect.z) > r)
				return false;

			r = e.x * (Real)fabs(linevect.y) + e.y * (Real)fabs(linevect.x);
			if (fabs(t.x*linevect.y - t.y*linevect.x) > r)
				return false;

			return true;
		}

		//! Tests if the box intersects with a line
		/** \param line: Line to test intersection with.
		\return True if there is an intersection , else false. */
		template<class K>
		inline bool intersectsWithLine(const line3d<K>& line) const
		{
			Vector3 v = line.getVector();
			v.normalise();
			return intersectsWithLine(line.getMiddle(), v, (Real)(line.getLength() * 0.5));
				
		}

	 
		inline AxisAlignedBox intersection(const AxisAlignedBox& b2) const
		{
			if (this->isNull() || b2.isNull())
			{
				return AxisAlignedBox();
			}
			else if (this->isInfinite())
			{
				return b2;
			}
			else if (b2.isInfinite())
			{
				return *this;
			}

			Vector3 intMin = mMinimum;
			Vector3 intMax = mMaximum;

			intMin.makeCeil(b2.getMinimum());
			intMax.makeFloor(b2.getMaximum());

			 
			if (intMin.x < intMax.x &&
				intMin.y < intMax.y &&
				intMin.z < intMax.z)
			{
				return AxisAlignedBox(intMin, intMax);
			}

			return AxisAlignedBox();
		}

		/// 计算盒子容积
		Real volume(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return 0.0f;

			case EXTENT_FINITE:
			{
				Vector3 diff = mMaximum - mMinimum;
				return diff.x * diff.y * diff.z;
			}

			case EXTENT_INFINITE:
				return Math::POS_INFINITY;

			default:  
				assert(false && "Never reached");
				return 0.0f;
			}
		}

		/** 通过给定的向量缩放AABB*/
		inline void scale(const Vector3& s)
		{
			 
			if (mExtent != EXTENT_FINITE)
				return;

			 
			Vector3 min = mMinimum * s;
			Vector3 max = mMaximum * s;
			setExtents(min, max);
		}

		/** 测试盒子是否与球体相交*/
		bool intersects(const Sphere& s) const
		{
			return Math::intersects(s, *this);
		}
		/** 测试盒子与平面相交 */
		bool intersects(const Plane& p) const
		{
			return Math::intersects(p, *this);
		}
		/** 测试这个顶点是否在盒子之内 */
		bool intersects(const Vector3& v) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return false;

			case EXTENT_FINITE:
				return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  &&
					v.y >= mMinimum.y  &&  v.y <= mMaximum.y  &&
					v.z >= mMinimum.z  &&  v.z <= mMaximum.z);

			case EXTENT_INFINITE:
				return true;

			default:  
				assert(false && "Never reached");
				return false;
			}
		}
		/// 取得盒子中心点
		Vector3 getCenter(void) const
		{
			assert((mExtent == EXTENT_FINITE) && "Can't get center of a null or infinite AAB");

			return Vector3(
				(mMaximum.x + mMinimum.x) * 0.5f,
				(mMaximum.y + mMinimum.y) * 0.5f,
				(mMaximum.z + mMinimum.z) * 0.5f);
		}
		/// 取得盒子大小
		Vector3 getSize(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return Vector3::ZERO;

			case EXTENT_FINITE:
				return mMaximum - mMinimum;

			case EXTENT_INFINITE:
				return Vector3(
					Math::POS_INFINITY,
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default:  
				assert(false && "Never reached");
				return Vector3::ZERO;
			}
		}
		/// 取得盒子半大小
		Vector3 getHalfSize(void) const
		{
			switch (mExtent)
			{
			case EXTENT_NULL:
				return Vector3::ZERO;

			case EXTENT_FINITE:
				return (mMaximum - mMinimum) * 0.5;

			case EXTENT_INFINITE:
				return Vector3(
					Math::POS_INFINITY,
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default: // 关闭编译器
				assert(false && "Never reached");
				return Vector3::ZERO;
			}
		}

		/** 测试给定点是否在盒子内部
		*/
		bool contains(const Vector3& v) const
		{
			if (isNull())
				return false;
			if (isInfinite())
				return true;

			return mMinimum.x <= v.x && v.x <= mMaximum.x &&
				mMinimum.y <= v.y && v.y <= mMaximum.y &&
				mMinimum.z <= v.z && v.z <= mMaximum.z;
		}

		/** 返回一个给定点和盒子任意部分的距离*/
		Real distance(const Vector3& v) const
		{

			if (this->contains(v))
				return 0;
			else
			{
				Real maxDist = 0;
#ifdef min(a,b)
#undef min(a,b)
				maxDist = std::numeric_limits<Real>::min();
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

				if (v.x < mMinimum.x)
					maxDist = Math::_max(maxDist, mMinimum.x - v.x);
				if (v.y < mMinimum.y)
					maxDist = Math::_max(maxDist, mMinimum.y - v.y);
				if (v.z < mMinimum.z)
					maxDist = Math::_max(maxDist, mMinimum.z - v.z);

				if (v.x > mMaximum.x)
					maxDist = Math::_max(maxDist, v.x - mMaximum.x);
				if (v.y > mMaximum.y)
					maxDist = Math::_max(maxDist, v.y - mMaximum.y);
				if (v.z > mMaximum.z)
					maxDist = Math::_max(maxDist, v.z - mMaximum.z);

				return maxDist;
			}
		}

		//! Classifies a relation with a plane.
		/** \param plane Plane to classify relation to.
		\return Returns ISREL3D_FRONT if the box is in front of the plane,
		ISREL3D_BACK if the box is behind the plane, and
		ISREL3D_CLIPPED if it is on both sides of the plane. */
		EIntersectionRelation3D classifyPlaneRelation(const Plane& plane) const
		{
			Vector3 nearPoint(getMaximum());
			Vector3 farPoint(getMinimum());

			if (plane.normal.x > 0)
			{
				nearPoint.x = getMaximum().x;
				farPoint.x = getMinimum().x;
			}

			if (plane.normal.y >  0)
			{
				nearPoint.y = getMinimum().y;
				farPoint.y = getMaximum().y;
			}

			if (plane.normal.z > 0)
			{
				nearPoint.z = getMinimum().z;
				farPoint.z = getMaximum().z;
			}

			if (plane.normal.dotProduct(nearPoint) + plane.d > 0)
				return ISREL3D_FRONT;

			if (plane.normal.dotProduct(farPoint) + plane.d > 0)
				return ISREL3D_CLIPPED;

			return ISREL3D_BACK;
		}

		//! Calculates a new interpolated bounding box.
		/** d=0 returns other, d=1 returns this, all other values blend between
		the two boxes.
		\param other Other box to interpolate between
		\param d Value between 0.0f and 1.0f.
		\return Interpolated box. */
		AxisAlignedBox getInterpolated(const AxisAlignedBox& other, FLOAT32 d) const
		{
			FLOAT32 inv = 1.0f - d;
			return AxisAlignedBox((other.getMinimum()*inv) + (getMinimum()*d),
				(other.getMaximum()*inv) + (getMaximum()*d));
		}

		/** 测试另一个盒子是否被这个盒子包含
		*/
		bool contains(const AxisAlignedBox& other) const
		{
			if (other.isNull() || this->isInfinite())
				return true;

			if (this->isNull() || other.isInfinite())
				return false;

			return this->mMinimum.x <= other.mMinimum.x &&
				this->mMinimum.y <= other.mMinimum.y &&
				this->mMinimum.z <= other.mMinimum.z &&
				other.mMaximum.x <= this->mMaximum.x &&
				other.mMaximum.y <= this->mMaximum.y &&
				other.mMaximum.z <= this->mMaximum.z;
		}

		 
		bool operator== (const AxisAlignedBox& rhs) const
		{
			if (this->mExtent != rhs.mExtent)
				return false;

			if (!this->isFinite())
				return true;

			return this->mMinimum == rhs.mMinimum &&
				this->mMaximum == rhs.mMaximum;
		}

		 
		bool operator!= (const AxisAlignedBox& rhs) const
		{
			return !(*this == rhs);
		}

		 
		static const AxisAlignedBox BOX_NULL;
		static const AxisAlignedBox BOX_INFINITE;


	};

	/** @} */
	/** @} */

}

#endif