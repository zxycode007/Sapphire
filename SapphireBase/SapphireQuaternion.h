
#ifndef __SAPPHIRE_QUATERNION_H__
#define __SAPPHIRE_QUATERNION_H__

#include "SapphirePrerequisites.h"
#include "SapphireMath.h"

namespace Sapphire {

	class _SapphireExport Quaternion
	{
	public:
		/// 默认构造器，初始化单位旋转0度
		inline Quaternion()
			: w(1), x(0), y(0), z(0)
		{
		}
		//! 构造器转换一个欧拉角到一个四元数
		inline Quaternion(Real fx,Real fy, Real fz)
			:x(fx), y(fy), z(fz)
		{
		}
		//! 构造器转换一个欧拉角(弧度)到一个四元数
		Quaternion(const Vector3& vec);
		
		/// 用明确的值构造四元数
	    inline Quaternion(
			Real fW,
			Real fX, Real fY, Real fZ)
			: w(fW), x(fX), y(fY), z(fZ)
		{
		}
		/// 由一个旋转矩阵构造四元数
		inline Quaternion(const Matrix3& rot)
		{
			this->FromRotationMatrix(rot);
		}
		/// 由一个角度/轴构造一个四元数
		inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
		{
			this->FromAngleAxis(rfAngle, rkAxis);
		}
		/// 用三个正交本地轴构建一个四元数
		inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
		{
			this->FromAxes(xaxis, yaxis, zaxis);
		}
		///  用三个正交本地轴构建一个四元数
		inline Quaternion(const Vector3* akAxis)
		{
			this->FromAxes(akAxis);
		}
		/// 用手动输入4个W/X/Y/Z值来构建一个四元数
		inline Quaternion(Real* valptr)
		{
			memcpy(&w, valptr, sizeof(Real) * 4);
		}

		/** 
		交换这个四元数和另外一个的内容
		*/
		inline void swap(Quaternion& other)
		{
			std::swap(w, other.w);
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		/// 数组访问操作符
		inline Real operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// 数组访问操作符
		inline Real& operator [] (const size_t i)
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// 为了直接拷贝的指针访问器
		inline Real* ptr()
		{
			return &w;
		}

		/// 为了直接拷贝的指针访问器
		inline const Real* ptr() const
		{
			return &w;
		}

		void FromRotationMatrix(const Matrix3& kRot);
		void ToRotationMatrix(Matrix3& kRot) const;
		/** 
		由四元数创建一个矩阵
		*/
		void getMatrix(Matrix4 &dest, const Vector3 &translation) const;

		void getMatrix(Matrix4 &dest) const;
		/** 
		用支持的向量设置四元数，通过指定角度来“roll”向量
		*/
		void FromAngleAxis(const Radian& rfAngle, const Vector3& rkAxis);
		void ToAngleAxis(Radian& rfAngle, Vector3& rkAxis) const;
		inline void ToAngleAxis(Degree& dAngle, Vector3& rkAxis) const {
			Radian rAngle;
			ToAngleAxis(rAngle, rkAxis);
			dAngle = rAngle;
		}
		/** 
		用3轴构造这个四元数， 这些轴假设已经正交化 
		@看 FromAxes
		*/
		void FromAxes(const Vector3* akAxis);
		//void FromAxes(const Vector3& akAxis) const;
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		/** 取回3个正交化的轴定义这个四元数 
		@看 FromAxes */
		void ToAxes(Vector3* akAxis) const;
		
		void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

		/** 
		返回这个四元数定义这个X正交轴。 就像做xAxis = Vector3::UNIT_X * this
		同样调用这个本地X轴
		*/
		Vector3 xAxis(void) const;

		/** 
		返回这个四元数定义这个Y正交轴。 就像做yAxis = Vector3::UNIT_Y * this
		同样调用这个本地Y轴
		*/
		Vector3 yAxis(void) const;

		 
		Vector3 zAxis(void) const;

		inline Quaternion& operator= (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
		Quaternion operator+ (const Quaternion& rkQ) const;
		Quaternion operator- (const Quaternion& rkQ) const;
		Quaternion operator* (const Quaternion& rkQ) const;
		Quaternion operator* (Real fScalar) const;

		_SapphireExport friend Quaternion operator* (Real fScalar,
			const Quaternion& rkQ);

		Quaternion operator- () const;

		inline bool operator== (const Quaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) &&
				(rhs.z == z) && (rhs.w == w);
		}
		inline bool operator!= (const Quaternion& rhs) const
		{
			return !operator==(rhs);
		}
		// 一个四元数的函数
		/// 返回四元数的点积
		Real Dot(const Quaternion& rkQ) const;
		/* 返回这个四元数的法线长度
		*/
		Real Norm() const;
		/// 标准化四元数，并且返回之前的长度
		Real normalise(void);
		Quaternion Inverse() const;  // 应用非0四元数
		Quaternion UnitInverse() const;  // 应用单位长度四元数
		Quaternion Exp() const;
		Quaternion Log() const;

		/// 通过一个四元数旋转一个向量
		Vector3 operator* (const Vector3& rkVector) const;

		/** 
		计算这个四元数的本地滚转roll元素
		@param reprojectAxis 默认这个方法返回 'intuitive' 结果
		如果你投影四元数本地y到x和y轴，这个他们之间的角度将被返回。如果设置为false，这个结果
		是实际是yaw偏转，这将由四元数实现，它是去获取同样的方向的最短可能路径和可能包含更少的轴旋转
		这返回值的上域是从-180到180度
		*/
		Radian getRoll(bool reprojectAxis = true) const;
		/** 
		计算这个四元数的本地倾斜pitch元素
		@param reprojectAxis 默认这个方法返回 'intuitive' 结果
		如果你投影四元数本地Z到X和Y轴，这个他们之间的角度将被返回。如果设置为true，这个结果
		是实际是yaw偏转，这将由四元数实现，它是去获取同样的方向的最短可能路径和可能包含更少的轴旋转
		这返回值的上域是从-180到180度
		*/
		Radian getPitch(bool reprojectAxis = true) const;
		/** 
		计算这个四元数的本地偏转yaw元素
		@param reprojectAxis 默认这个方法返回 'intuitive' 结果
		如果你投影四元数本地y到X和Z轴，这个他们之间的角度将被返回。如果设置为true，这个结果
		是实际是yaw偏转，这将由四元数实现，它是去获取同样的方向的最短可能路径和可能包含更少的轴旋转
		这返回值的上域是从-180到180度
		 
		*/
		Radian getYaw(bool reprojectAxis = true) const;
		/// 用公差判断是否相等
		bool equals(const Quaternion& rhs, const Radian& tolerance) const;

		/** 
		应用在两个四元数间球面线性插值，并且返回结果
		Slerp ( 0.0f, A, B ) = A
		Slerp ( 1.0f, A, B ) = B
		@return 插值后的四元数
		@remarks
		插值有以用恒定速率插值的特性， 并且是最小的转矩（除非shortestPath=false）
		无论如何，它是不可交换的，意味着Slerp(0.75f,A,B) != Slerp(0.25f,B,A)
		因此小心操作。这对于IK动画特别重要
		*/
		static Quaternion Slerp(Real fT, const Quaternion& rkP,
			const Quaternion& rkQ, bool shortestPath = false);


		static Quaternion Slerp(const Quaternion& rkP,
			const Quaternion& rkQ, Real fT=0.5);

		/** 
		@见Slerp. 它额外增加了自旋“spins”（旋转多次），通过参数 'iExtraSpins'。
		来指定，当插值到达了最终值后的处理
		 
		*/
		static Quaternion SlerpExtraSpins(Real fT,
			const Quaternion& rkP, const Quaternion& rkQ,
			int iExtraSpins);

		// 设置球面二次插值
		static void Intermediate(const Quaternion& rkQ0,
			const Quaternion& rkQ1, const Quaternion& rkQ2,
			Quaternion& rka, Quaternion& rkB);

		// 球面二次插值
		static Quaternion Squad(Real fT, const Quaternion& rkP,
			const Quaternion& rkA, const Quaternion& rkB,
			const Quaternion& rkQ, bool shortestPath = false);

		/** 
		在两个四元数直接应用标准化线性插值，并且返回结果
		nlerp ( 0.0f, A, B ) = A
		nlerp ( 1.0f, A, B ) = B
		@remarks
		Nlerp 比Slerp更快
		Nlerp 可以交换(见Slerp；某些特定地方需要交换性，如IK动画),并且可以最小转矩（除法shortestPath=false）
		然而，它可以用于非恒定速率的插值；有时会用到，有时不会。 有一个非恒定速率可以提供更自然的旋转而无需调整其权重；
		如果你的场景相信定时旋转或假设他将指出一个特别的角度在一个特定的权重值，那么Slerp是更好的选择
		*/
		static Quaternion nlerp(Real fT, const Quaternion& rkP,
			const Quaternion& rkQ, bool shortestPath = false);

		/// 截断正弦0值
		static const Real msEpsilon;

		 
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

		Real w, x, y, z;

		 
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

		 
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Quaternion& q)
		{
			o << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
			return o;
		}

	};
	/** @} */
	/** @} */

}




#endif 
