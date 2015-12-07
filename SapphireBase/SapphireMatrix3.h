#ifndef __SAPPHIRE_MATRIX3_H__
#define __SAPPHIRE_MATRIX3_H__

#include "SapphirePrerequisites.h"

#include "SapphireVector3.h"



// 注意.  这个 (x,y,z) 坐标系统假设右手坐标系
// 坐标轴旋转矩阵由下面, 
//绕x轴旋转
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// 当t>0 指定在yz平面逆时针旋转
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// 当t>0 指定在zx平面逆时针旋转
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// // 当t>0 指定在xy平面逆时针旋转

namespace Sapphire
{ 
	/** 一个3X3矩阵，可以表示绕轴旋转
	@note	 
	这个坐标系统假定为右手坐标系
	*/
	class _SapphireExport Matrix3
	{
	public:
		/**
		默认的构造函数
		@note
		它不能初始化高效的初始化矩阵
		*/
		inline Matrix3() {}
		//由一个3X3的二维数组来初始化这个3维矩阵
		inline explicit Matrix3(const Real arr[3][3])
		{
			memcpy(m, arr, 9 * sizeof(Real));
		}
		inline Matrix3(const Matrix3& rkMatrix)
		{
			memcpy(m, rkMatrix.m, 9 * sizeof(Real));
		}
		Matrix3(Real fEntry00, Real fEntry01, Real fEntry02,
			Real fEntry10, Real fEntry11, Real fEntry12,
			Real fEntry20, Real fEntry21, Real fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

		/** 交换这个矩阵和另外一个矩阵的内容
		*/
		inline void swap(Matrix3& other)
		{
			std::swap(m[0][0], other.m[0][0]);
			std::swap(m[0][1], other.m[0][1]);
			std::swap(m[0][2], other.m[0][2]);
			std::swap(m[1][0], other.m[1][0]);
			std::swap(m[1][1], other.m[1][1]);
			std::swap(m[1][2], other.m[1][2]);
			std::swap(m[2][0], other.m[2][0]);
			std::swap(m[2][1], other.m[2][1]);
			std::swap(m[2][2], other.m[2][2]);
		}

		// 重载内存访问操作符，允许使用mat[r][c]
		inline Real* operator[] (size_t iRow) const
		{
			return (Real*)m[iRow];
		}
		/*inline operator Real* ()
		{
		return (Real*)m[0];
		}*/
		/*
		 返回指定列向量
		*/
		Vector3 GetColumn(size_t iCol) const;
		/*
		设置指定列向量
		*/
		void SetColumn(size_t iCol, const Vector3& vec);

		/*
		用设定xyz轴的来设定列向量
		*/
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

		// 赋值操作
		inline Matrix3& operator= (const Matrix3& rkMatrix)
		{
			memcpy(m, rkMatrix.m, 9 * sizeof(Real));
			return *this;
		}

		/**
		测试两个矩阵是否相等
		*/
		bool operator== (const Matrix3& rkMatrix) const;

		/** 
		测试两个矩阵是否不相等
		*/
		inline bool operator!= (const Matrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

		// 
		//算术操作
		/** 矩阵相加
		*/
		Matrix3 operator+ (const Matrix3& rkMatrix) const;

		/**
		矩阵相减
		*/
		Matrix3 operator- (const Matrix3& rkMatrix) const;

		/** 
		矩阵相乘
		*/
		Matrix3 operator* (const Matrix3& rkMatrix) const;
		/**
		求负矩阵
		*/
		Matrix3 operator- () const;

		/// Matrix * vector [3x3 * 3x1 = 3x1]  矩阵*向量   （列矩阵）
		Vector3 operator* (const Vector3& rkVector) const;

		/// Vector * matrix [1x3 * 3x3 = 1x3]  向量*矩阵   （行矩阵）
		_SapphireExport friend Vector3 operator* (const Vector3& rkVector,
			const Matrix3& rkMatrix);

		/// 矩阵乘以标量
		Matrix3 operator* (Real fScalar) const;

		/// 标量*矩阵
		_SapphireExport friend Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix);

		// 实用函数
		/**
		矩阵转置
		*/
		Matrix3 Transpose() const;    
		/**
		求逆矩阵
		*/
		bool Inverse(Matrix3& rkInverse, Real fTolerance = 1e-06) const;
		/**
		@求逆矩阵
		*/
		Matrix3 Inverse(Real fTolerance = 1e-06) const;
		/**
		求矩阵行列式的值
		*/
		Real Determinant() const;

		/**
		单个值分解
		*/ 
		void SingularValueDecomposition(Matrix3& rkL, Vector3& rkS,
			Matrix3& rkR) const;
		/**
		单个值组合
		*/
		void SingularValueComposition(const Matrix3& rkL,
			const Vector3& rkS, const Matrix3& rkR);

		/// 格拉姆-施密特 正交化（用于旋转矩阵的列）
		void Orthonormalize();

		/// 正交Q  对角D,  上三角化的U存做（u01,u02,u12) QDU分解
		void QDUDecomposition(Matrix3& rkQ, Vector3& rkD,
			Vector3& rkU) const;

		// 求矩阵的谱范数
		Real SpectralNorm() const;

		// 矩阵必须被正交化
		//获取轴角度
		// 让(x,y,z)成为单位长度轴并且以A作为旋转角度
		// 这个旋转矩阵是R = I + sin(A)*P + (1-cos(A))*P^2 
		// I 是单位矩阵
		//
		//       +-        -+
		//   P = |  0 -z +y |
		//       | +z  0 -x |
		//       | -y +x  0 |
		//       +-        -+
		//
		// 如果A>0 ,R 表示一个在这个观察对于原点的一端的轴向量场景对于这个轴逆时针旋转
		// 代数式为：
		//
		//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
		//
		// 其中，A=Pi，R-R^t = 0 他们防止我们通过P的提取轴。 而注意的是当A=Pi时 R = I+2*P^2，
		// 所以P^2 = (R-I)/2。  P^2的对角元素为 x^2-1, y^2-1，和z^2-1。 我们可以通过它们解出轴(x,y,z)
		// 由于这个角度是pi，它与关于你选折的平方根没关系
		void ToAngleAxis(Vector3& rkAxis, Radian& rfAngle) const;
		inline void ToAngleAxis(Vector3& rkAxis, Degree& rfAngle) const {
			Radian r;
			ToAngleAxis(rkAxis, r);
			rfAngle = r;
		}
		//通过给定的轴和，角度来创建旋转矩阵
		void FromAngleAxis(const Vector3& rkAxis, const Radian& fRadians);

		//  分解yaw*pitch*roll，这个矩阵必须被正交化、 当yaw是绕up向量的旋转， pitch是绕右向量旋转， roll是绕方向向量的旋转
		bool ToEulerAnglesXYZ(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		bool ToEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		bool ToEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		bool ToEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		bool ToEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		bool ToEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle,
			Radian& rfRAngle) const;
		void FromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		/**
		对称矩阵的特征解
		矩阵必须是对称矩阵
		*/
		void EigenSolveSymmetric(Real afEigenvalue[3],
			Vector3 akEigenvector[3]) const;
		/**
		求矩阵的张量积
		*/
		static void TensorProduct(const Vector3& rkU, const Vector3& rkV,
			Matrix3& rkProduct);

		/** 判断是否被缩放*/
		inline bool hasScale() const
		{
			// 检测列向量值 == 本地轴
			Real t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
			if (!Math::RealEqual(t, 1.0, (Real)1e-04))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::RealEqual(t, 1.0, (Real)1e-04))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::RealEqual(t, 1.0, (Real)1e-04))
				return true;

			return false;
		}

		inline Matrix3&  setRotationRadians(const Vector3& rotation, bool useTable=false)
		{
			const Real cr = Math::Cos(rotation.x, useTable);
			const Real sr = Math::Sin(rotation.x, useTable);
			const Real cp = Math::Cos(rotation.y, useTable);
			const Real sp = Math::Sin(rotation.y, useTable);
			const Real cy = Math::Cos(rotation.z, useTable);
			const Real sy = Math::Sin(rotation.z, useTable);

			m[0][0] = (cp*cy);
			m[1][0] = (cp*sy);
			m[2][0] = (-sp);

			const Real srsp = sr*sp;
			const Real crsp = cr*sp;

			m[0][1] = (srsp*cy - cr*sy);
			m[1][1] = (srsp*sy + cr*cy);
			m[2][1] = (sr*cp);

			m[0][2] = (crsp*cy + sr*sy);
			m[1][2] = (crsp*sy - sr*cy);
			m[1][2] = (cr*cp);

			return *this;
		}

		/** 
		将该矩阵输出到流
		*/
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Matrix3& mat)
		{
			o << "Matrix3(" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", "
				<< mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", "
				<< mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ")";
			return o;
		}

		static const Real EPSILON;      //无穷小
		static const Matrix3 ZERO;      //0矩阵
		static const Matrix3 IDENTITY;     //单位矩阵

	protected:
		// 对于特征解的支持
		void Tridiagonal(Real afDiag[3], Real afSubDiag[3]);  //三对角化
		bool QLAlgorithm(Real afDiag[3], Real afSubDiag[3]);  //QL算法

		// 对于矩阵奇异值分解SVD支持 
		static const Real msSvdEpsilon;       //svd极小值
		static const unsigned int msSvdMaxIterations;        //svd最大迭代次数
		//双对角线化
		static void Bidiagonalize(Matrix3& kA, Matrix3& kL,
			Matrix3& kR);         
		//GolubKahan 算法
		static void GolubKahanStep(Matrix3& kA, Matrix3& kL,
			Matrix3& kR);

		// 对于谱范数的支持
		static Real MaxCubicRoot(Real afCoeff[3]);  //最大立方体根

		Real m[3][3];

		// 对于快速访问
		friend class Matrix4;
	};
	/** @} */
	/** @} */
}
#endif
