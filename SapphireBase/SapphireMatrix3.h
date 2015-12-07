#ifndef __SAPPHIRE_MATRIX3_H__
#define __SAPPHIRE_MATRIX3_H__

#include "SapphirePrerequisites.h"

#include "SapphireVector3.h"



// ע��.  ��� (x,y,z) ����ϵͳ������������ϵ
// ��������ת����������, 
//��x����ת
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// ��t>0 ָ����yzƽ����ʱ����ת
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// ��t>0 ָ����zxƽ����ʱ����ת
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// // ��t>0 ָ����xyƽ����ʱ����ת

namespace Sapphire
{ 
	/** һ��3X3���󣬿��Ա�ʾ������ת
	@note	 
	�������ϵͳ�ٶ�Ϊ��������ϵ
	*/
	class _SapphireExport Matrix3
	{
	public:
		/**
		Ĭ�ϵĹ��캯��
		@note
		�����ܳ�ʼ����Ч�ĳ�ʼ������
		*/
		inline Matrix3() {}
		//��һ��3X3�Ķ�ά��������ʼ�����3ά����
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

		/** ����������������һ�����������
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

		// �����ڴ���ʲ�����������ʹ��mat[r][c]
		inline Real* operator[] (size_t iRow) const
		{
			return (Real*)m[iRow];
		}
		/*inline operator Real* ()
		{
		return (Real*)m[0];
		}*/
		/*
		 ����ָ��������
		*/
		Vector3 GetColumn(size_t iCol) const;
		/*
		����ָ��������
		*/
		void SetColumn(size_t iCol, const Vector3& vec);

		/*
		���趨xyz������趨������
		*/
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

		// ��ֵ����
		inline Matrix3& operator= (const Matrix3& rkMatrix)
		{
			memcpy(m, rkMatrix.m, 9 * sizeof(Real));
			return *this;
		}

		/**
		�������������Ƿ����
		*/
		bool operator== (const Matrix3& rkMatrix) const;

		/** 
		�������������Ƿ����
		*/
		inline bool operator!= (const Matrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

		// 
		//��������
		/** �������
		*/
		Matrix3 operator+ (const Matrix3& rkMatrix) const;

		/**
		�������
		*/
		Matrix3 operator- (const Matrix3& rkMatrix) const;

		/** 
		�������
		*/
		Matrix3 operator* (const Matrix3& rkMatrix) const;
		/**
		�󸺾���
		*/
		Matrix3 operator- () const;

		/// Matrix * vector [3x3 * 3x1 = 3x1]  ����*����   ���о���
		Vector3 operator* (const Vector3& rkVector) const;

		/// Vector * matrix [1x3 * 3x3 = 1x3]  ����*����   ���о���
		_SapphireExport friend Vector3 operator* (const Vector3& rkVector,
			const Matrix3& rkMatrix);

		/// ������Ա���
		Matrix3 operator* (Real fScalar) const;

		/// ����*����
		_SapphireExport friend Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix);

		// ʵ�ú���
		/**
		����ת��
		*/
		Matrix3 Transpose() const;    
		/**
		�������
		*/
		bool Inverse(Matrix3& rkInverse, Real fTolerance = 1e-06) const;
		/**
		@�������
		*/
		Matrix3 Inverse(Real fTolerance = 1e-06) const;
		/**
		���������ʽ��ֵ
		*/
		Real Determinant() const;

		/**
		����ֵ�ֽ�
		*/ 
		void SingularValueDecomposition(Matrix3& rkL, Vector3& rkS,
			Matrix3& rkR) const;
		/**
		����ֵ���
		*/
		void SingularValueComposition(const Matrix3& rkL,
			const Vector3& rkS, const Matrix3& rkR);

		/// ����ķ-ʩ���� ��������������ת������У�
		void Orthonormalize();

		/// ����Q  �Խ�D,  �����ǻ���U������u01,u02,u12) QDU�ֽ�
		void QDUDecomposition(Matrix3& rkQ, Vector3& rkD,
			Vector3& rkU) const;

		// �������׷���
		Real SpectralNorm() const;

		// ������뱻������
		//��ȡ��Ƕ�
		// ��(x,y,z)��Ϊ��λ�����Ტ����A��Ϊ��ת�Ƕ�
		// �����ת������R = I + sin(A)*P + (1-cos(A))*P^2 
		// I �ǵ�λ����
		//
		//       +-        -+
		//   P = |  0 -z +y |
		//       | +z  0 -x |
		//       | -y +x  0 |
		//       +-        -+
		//
		// ���A>0 ,R ��ʾһ��������۲����ԭ���һ�˵����������������������ʱ����ת
		// ����ʽΪ��
		//
		//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
		//
		// ���У�A=Pi��R-R^t = 0 ���Ƿ�ֹ����ͨ��P����ȡ�ᡣ ��ע����ǵ�A=Piʱ R = I+2*P^2��
		// ����P^2 = (R-I)/2��  P^2�ĶԽ�Ԫ��Ϊ x^2-1, y^2-1����z^2-1�� ���ǿ���ͨ�����ǽ����(x,y,z)
		// ��������Ƕ���pi�����������ѡ�۵�ƽ����û��ϵ
		void ToAngleAxis(Vector3& rkAxis, Radian& rfAngle) const;
		inline void ToAngleAxis(Vector3& rkAxis, Degree& rfAngle) const {
			Radian r;
			ToAngleAxis(rkAxis, r);
			rfAngle = r;
		}
		//ͨ����������ͣ��Ƕ���������ת����
		void FromAngleAxis(const Vector3& rkAxis, const Radian& fRadians);

		//  �ֽ�yaw*pitch*roll�����������뱻�������� ��yaw����up��������ת�� pitch������������ת�� roll���Ʒ�����������ת
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
		�Գƾ����������
		��������ǶԳƾ���
		*/
		void EigenSolveSymmetric(Real afEigenvalue[3],
			Vector3 akEigenvector[3]) const;
		/**
		������������
		*/
		static void TensorProduct(const Vector3& rkU, const Vector3& rkV,
			Matrix3& rkProduct);

		/** �ж��Ƿ�����*/
		inline bool hasScale() const
		{
			// ���������ֵ == ������
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
		���þ����������
		*/
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Matrix3& mat)
		{
			o << "Matrix3(" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", "
				<< mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", "
				<< mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ")";
			return o;
		}

		static const Real EPSILON;      //����С
		static const Matrix3 ZERO;      //0����
		static const Matrix3 IDENTITY;     //��λ����

	protected:
		// �����������֧��
		void Tridiagonal(Real afDiag[3], Real afSubDiag[3]);  //���Խǻ�
		bool QLAlgorithm(Real afDiag[3], Real afSubDiag[3]);  //QL�㷨

		// ���ھ�������ֵ�ֽ�SVD֧�� 
		static const Real msSvdEpsilon;       //svd��Сֵ
		static const unsigned int msSvdMaxIterations;        //svd����������
		//˫�Խ��߻�
		static void Bidiagonalize(Matrix3& kA, Matrix3& kL,
			Matrix3& kR);         
		//GolubKahan �㷨
		static void GolubKahanStep(Matrix3& kA, Matrix3& kL,
			Matrix3& kR);

		// �����׷�����֧��
		static Real MaxCubicRoot(Real afCoeff[3]);  //����������

		Real m[3][3];

		// ���ڿ��ٷ���
		friend class Matrix4;
	};
	/** @} */
	/** @} */
}
#endif
