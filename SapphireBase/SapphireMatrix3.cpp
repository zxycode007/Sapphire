#include "SapphireMatrix3.h"
#include "SapphireMath.h"


namespace Sapphire
{
	//初始化常量
	const Real Matrix3::EPSILON = 1e-06;
	const Matrix3 Matrix3::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0);
	const Matrix3 Matrix3::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);
	const Real Matrix3::msSvdEpsilon = 1e-04;               
	const unsigned int Matrix3::msSvdMaxIterations = 32;

	//-----------------------------------------------------------------------
	Vector3 Matrix3::GetColumn(size_t iCol) const
	{
		assert(iCol < 3);
		return Vector3(m[0][iCol], m[1][iCol],
			m[2][iCol]);
	}
	//-----------------------------------------------------------------------
	void Matrix3::SetColumn(size_t iCol, const Vector3& vec)
	{
		assert(iCol < 3);
		m[0][iCol] = vec.x;
		m[1][iCol] = vec.y;
		m[2][iCol] = vec.z;

	}
	//-----------------------------------------------------------------------
	void Matrix3::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
	{
		SetColumn(0, xAxis);
		SetColumn(1, yAxis);
		SetColumn(2, zAxis);

	}

	//-----------------------------------------------------------------------
	bool Matrix3::operator== (const Matrix3& rkMatrix) const
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				if (m[iRow][iCol] != rkMatrix.m[iRow][iCol])
					return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator+ (const Matrix3& rkMatrix) const
	{
		Matrix3 kSum;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kSum.m[iRow][iCol] = m[iRow][iCol] +
					rkMatrix.m[iRow][iCol];
			}
		}
		return kSum;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator- (const Matrix3& rkMatrix) const
	{
		Matrix3 kDiff;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kDiff.m[iRow][iCol] = m[iRow][iCol] -
					rkMatrix.m[iRow][iCol];
			}
		}
		return kDiff;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator* (const Matrix3& rkMatrix) const
	{
		Matrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kProd.m[iRow][iCol] =
					m[iRow][0] * rkMatrix.m[0][iCol] +
					m[iRow][1] * rkMatrix.m[1][iCol] +
					m[iRow][2] * rkMatrix.m[2][iCol];
			}
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	Vector3 Matrix3::operator* (const Vector3& rkPoint) const
	{
		Vector3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				m[iRow][0] * rkPoint[0] +
				m[iRow][1] * rkPoint[1] +
				m[iRow][2] * rkPoint[2];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	Vector3 operator* (const Vector3& rkPoint, const Matrix3& rkMatrix)
	{
		Vector3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				rkPoint[0] * rkMatrix.m[0][iRow] +
				rkPoint[1] * rkMatrix.m[1][iRow] +
				rkPoint[2] * rkMatrix.m[2][iRow];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator- () const
	{
		Matrix3 kNeg;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kNeg[iRow][iCol] = -m[iRow][iCol];
		}
		return kNeg;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator* (Real fScalar) const
	{
		Matrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar*m[iRow][iCol];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix)
	{
		Matrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::Transpose() const
	{
		Matrix3 kTranspose;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kTranspose[iRow][iCol] = m[iCol][iRow];
		}
		return kTranspose;
	}
	//-----------------------------------------------------------------------
	bool Matrix3::Inverse(Matrix3& rkInverse, Real fTolerance) const
	{
		// 用代数余子式求逆矩阵  这有8次比用高斯消去法推要快 

		rkInverse[0][0] = m[1][1] * m[2][2] -
			m[1][2] * m[2][1];
		rkInverse[0][1] = m[0][2] * m[2][1] -
			m[0][1] * m[2][2];
		rkInverse[0][2] = m[0][1] * m[1][2] -
			m[0][2] * m[1][1];
		rkInverse[1][0] = m[1][2] * m[2][0] -
			m[1][0] * m[2][2];
		rkInverse[1][1] = m[0][0] * m[2][2] -
			m[0][2] * m[2][0];
		rkInverse[1][2] = m[0][2] * m[1][0] -
			m[0][0] * m[1][2];
		rkInverse[2][0] = m[1][0] * m[2][1] -
			m[1][1] * m[2][0];
		rkInverse[2][1] = m[0][1] * m[2][0] -
			m[0][0] * m[2][1];
		rkInverse[2][2] = m[0][0] * m[1][1] -
			m[0][1] * m[1][0];

		Real fDet =
			m[0][0] * rkInverse[0][0] +
			m[0][1] * rkInverse[1][0] +
			m[0][2] * rkInverse[2][0];

		if (Math::Abs(fDet) <= fTolerance)
			return false;

		Real fInvDet = 1.0f / fDet;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				rkInverse[iRow][iCol] *= fInvDet;
		}

		return true;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::Inverse(Real fTolerance) const
	{
		Matrix3 kInverse = Matrix3::ZERO;
		Inverse(kInverse, fTolerance);
		return kInverse;
	}
	//-----------------------------------------------------------------------
	Real Matrix3::Determinant() const
	{
		Real fCofactor00 = m[1][1] * m[2][2] -
			m[1][2] * m[2][1];
		Real fCofactor10 = m[1][2] * m[2][0] -
			m[1][0] * m[2][2];
		Real fCofactor20 = m[1][0] * m[2][1] -
			m[1][1] * m[2][0];

		Real fDet =
			m[0][0] * fCofactor00 +
			m[0][1] * fCofactor10 +
			m[0][2] * fCofactor20;

		return fDet;
	}
	//-----------------------------------------------------------------------
	void Matrix3::Bidiagonalize(Matrix3& kA, Matrix3& kL,
		Matrix3& kR)
	{
		Real afV[3], afW[3];
		Real fLength, fSign, fT1, fInvT1, fT2;
		bool bIdentity;

		// map first column to (*,0,0)
		fLength = Math::Sqrt(kA[0][0] * kA[0][0] + kA[1][0] * kA[1][0] +
			kA[2][0] * kA[2][0]);
		if (fLength > 0.0)
		{
			fSign = (kA[0][0] > 0.0f ? 1.0f : -1.0f);
			fT1 = kA[0][0] + fSign*fLength;
			fInvT1 = 1.0f / fT1;
			afV[1] = kA[1][0] * fInvT1;
			afV[2] = kA[2][0] * fInvT1;

			fT2 = -2.0f / (1.0f + afV[1] * afV[1] + afV[2] * afV[2]);
			afW[0] = fT2*(kA[0][0] + kA[1][0] * afV[1] + kA[2][0] * afV[2]);
			afW[1] = fT2*(kA[0][1] + kA[1][1] * afV[1] + kA[2][1] * afV[2]);
			afW[2] = fT2*(kA[0][2] + kA[1][2] * afV[1] + kA[2][2] * afV[2]);
			kA[0][0] += afW[0];
			kA[0][1] += afW[1];
			kA[0][2] += afW[2];
			kA[1][1] += afV[1] * afW[1];
			kA[1][2] += afV[1] * afW[2];
			kA[2][1] += afV[2] * afW[1];
			kA[2][2] += afV[2] * afW[2];

			kL[0][0] = 1.0f + fT2;
			kL[0][1] = kL[1][0] = fT2*afV[1];
			kL[0][2] = kL[2][0] = fT2*afV[2];
			kL[1][1] = 1.0f + fT2*afV[1] * afV[1];
			kL[1][2] = kL[2][1] = fT2*afV[1] * afV[2];
			kL[2][2] = 1.0f + fT2*afV[2] * afV[2];
			bIdentity = false;
		}
		else
		{
			kL = Matrix3::IDENTITY;
			bIdentity = true;
		}

		// map first row to (*,*,0)
		fLength = Math::Sqrt(kA[0][1] * kA[0][1] + kA[0][2] * kA[0][2]);
		if (fLength > 0.0)
		{
			fSign = (kA[0][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = kA[0][1] + fSign*fLength;
			afV[2] = kA[0][2] / fT1;

			fT2 = -2.0f / (1.0f + afV[2] * afV[2]);
			afW[0] = fT2*(kA[0][1] + kA[0][2] * afV[2]);
			afW[1] = fT2*(kA[1][1] + kA[1][2] * afV[2]);
			afW[2] = fT2*(kA[2][1] + kA[2][2] * afV[2]);
			kA[0][1] += afW[0];
			kA[1][1] += afW[1];
			kA[1][2] += afW[1] * afV[2];
			kA[2][1] += afW[2];
			kA[2][2] += afW[2] * afV[2];

			kR[0][0] = 1.0;
			kR[0][1] = kR[1][0] = 0.0;
			kR[0][2] = kR[2][0] = 0.0;
			kR[1][1] = 1.0f + fT2;
			kR[1][2] = kR[2][1] = fT2*afV[2];
			kR[2][2] = 1.0f + fT2*afV[2] * afV[2];
		}
		else
		{
			kR = Matrix3::IDENTITY;
		}

		// map second column to (*,*,0)
		fLength = Math::Sqrt(kA[1][1] * kA[1][1] + kA[2][1] * kA[2][1]);
		if (fLength > 0.0)
		{
			fSign = (kA[1][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = kA[1][1] + fSign*fLength;
			afV[2] = kA[2][1] / fT1;

			fT2 = -2.0f / (1.0f + afV[2] * afV[2]);
			afW[1] = fT2*(kA[1][1] + kA[2][1] * afV[2]);
			afW[2] = fT2*(kA[1][2] + kA[2][2] * afV[2]);
			kA[1][1] += afW[1];
			kA[1][2] += afW[2];
			kA[2][2] += afV[2] * afW[2];

			Real fA = 1.0f + fT2;
			Real fB = fT2*afV[2];
			Real fC = 1.0f + fB*afV[2];

			if (bIdentity)
			{
				kL[0][0] = 1.0;
				kL[0][1] = kL[1][0] = 0.0;
				kL[0][2] = kL[2][0] = 0.0;
				kL[1][1] = fA;
				kL[1][2] = kL[2][1] = fB;
				kL[2][2] = fC;
			}
			else
			{
				for (int iRow = 0; iRow < 3; iRow++)
				{
					Real fTmp0 = kL[iRow][1];
					Real fTmp1 = kL[iRow][2];
					kL[iRow][1] = fA*fTmp0 + fB*fTmp1;
					kL[iRow][2] = fB*fTmp0 + fC*fTmp1;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::GolubKahanStep(Matrix3& kA, Matrix3& kL,
		Matrix3& kR)
	{
		Real fT11 = kA[0][1] * kA[0][1] + kA[1][1] * kA[1][1];
		Real fT22 = kA[1][2] * kA[1][2] + kA[2][2] * kA[2][2];
		Real fT12 = kA[1][1] * kA[1][2];
		Real fTrace = fT11 + fT22;
		Real fDiff = fT11 - fT22;
		Real fDiscr = Math::Sqrt(fDiff*fDiff + 4.0f*fT12*fT12);
		Real fRoot1 = 0.5f*(fTrace + fDiscr);
		Real fRoot2 = 0.5f*(fTrace - fDiscr);

		// adjust right
		Real fY = kA[0][0] - (Math::Abs(fRoot1 - fT22) <=
			Math::Abs(fRoot2 - fT22) ? fRoot1 : fRoot2);
		Real fZ = kA[0][1];
		Real fInvLength = Math::InvSqrt(fY*fY + fZ*fZ);
		Real fSin = fZ*fInvLength;
		Real fCos = -fY*fInvLength;

		Real fTmp0 = kA[0][0];
		Real fTmp1 = kA[0][1];
		kA[0][0] = fCos*fTmp0 - fSin*fTmp1;
		kA[0][1] = fSin*fTmp0 + fCos*fTmp1;
		kA[1][0] = -fSin*kA[1][1];
		kA[1][1] *= fCos;

		size_t iRow;
		for (iRow = 0; iRow < 3; iRow++)
		{
			fTmp0 = kR[0][iRow];
			fTmp1 = kR[1][iRow];
			kR[0][iRow] = fCos*fTmp0 - fSin*fTmp1;
			kR[1][iRow] = fSin*fTmp0 + fCos*fTmp1;
		}

		// adjust left
		fY = kA[0][0];
		fZ = kA[1][0];
		fInvLength = Math::InvSqrt(fY*fY + fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[0][0] = fCos*kA[0][0] - fSin*kA[1][0];
		fTmp0 = kA[0][1];
		fTmp1 = kA[1][1];
		kA[0][1] = fCos*fTmp0 - fSin*fTmp1;
		kA[1][1] = fSin*fTmp0 + fCos*fTmp1;
		kA[0][2] = -fSin*kA[1][2];
		kA[1][2] *= fCos;

		size_t iCol;
		for (iCol = 0; iCol < 3; iCol++)
		{
			fTmp0 = kL[iCol][0];
			fTmp1 = kL[iCol][1];
			kL[iCol][0] = fCos*fTmp0 - fSin*fTmp1;
			kL[iCol][1] = fSin*fTmp0 + fCos*fTmp1;
		}

		// adjust right
		fY = kA[0][1];
		fZ = kA[0][2];
		fInvLength = Math::InvSqrt(fY*fY + fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[0][1] = fCos*kA[0][1] - fSin*kA[0][2];
		fTmp0 = kA[1][1];
		fTmp1 = kA[1][2];
		kA[1][1] = fCos*fTmp0 - fSin*fTmp1;
		kA[1][2] = fSin*fTmp0 + fCos*fTmp1;
		kA[2][1] = -fSin*kA[2][2];
		kA[2][2] *= fCos;

		for (iRow = 0; iRow < 3; iRow++)
		{
			fTmp0 = kR[1][iRow];
			fTmp1 = kR[2][iRow];
			kR[1][iRow] = fCos*fTmp0 - fSin*fTmp1;
			kR[2][iRow] = fSin*fTmp0 + fCos*fTmp1;
		}

		// adjust left
		fY = kA[1][1];
		fZ = kA[2][1];
		fInvLength = Math::InvSqrt(fY*fY + fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[1][1] = fCos*kA[1][1] - fSin*kA[2][1];
		fTmp0 = kA[1][2];
		fTmp1 = kA[2][2];
		kA[1][2] = fCos*fTmp0 - fSin*fTmp1;
		kA[2][2] = fSin*fTmp0 + fCos*fTmp1;

		for (iCol = 0; iCol < 3; iCol++)
		{
			fTmp0 = kL[iCol][1];
			fTmp1 = kL[iCol][2];
			kL[iCol][1] = fCos*fTmp0 - fSin*fTmp1;
			kL[iCol][2] = fSin*fTmp0 + fCos*fTmp1;
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::SingularValueDecomposition(Matrix3& kL, Vector3& kS,
		Matrix3& kR) const
	{
		// temas: currently unused
		//const int iMax = 16;
		size_t iRow, iCol;

		Matrix3 kA = *this;
		Bidiagonalize(kA, kL, kR);

		for (unsigned int i = 0; i < msSvdMaxIterations; i++)
		{
			Real fTmp, fTmp0, fTmp1;
			Real fSin0, fCos0, fTan0;
			Real fSin1, fCos1, fTan1;

			bool bTest1 = (Math::Abs(kA[0][1]) <=
				msSvdEpsilon*(Math::Abs(kA[0][0]) + Math::Abs(kA[1][1])));
			bool bTest2 = (Math::Abs(kA[1][2]) <=
				msSvdEpsilon*(Math::Abs(kA[1][1]) + Math::Abs(kA[2][2])));
			if (bTest1)
			{
				if (bTest2)
				{
					kS[0] = kA[0][0];
					kS[1] = kA[1][1];
					kS[2] = kA[2][2];
					break;
				}
				else
				{
					// 2x2 closed form factorization
					fTmp = (kA[1][1] * kA[1][1] - kA[2][2] * kA[2][2] +
						kA[1][2] * kA[1][2]) / (kA[1][2] * kA[2][2]);
					fTan0 = 0.5f*(fTmp + Math::Sqrt(fTmp*fTmp + 4.0f));
					fCos0 = Math::InvSqrt(1.0f + fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for (iCol = 0; iCol < 3; iCol++)
					{
						fTmp0 = kL[iCol][1];
						fTmp1 = kL[iCol][2];
						kL[iCol][1] = fCos0*fTmp0 - fSin0*fTmp1;
						kL[iCol][2] = fSin0*fTmp0 + fCos0*fTmp1;
					}

					fTan1 = (kA[1][2] - kA[2][2] * fTan0) / kA[1][1];
					fCos1 = Math::InvSqrt(1.0f + fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for (iRow = 0; iRow < 3; iRow++)
					{
						fTmp0 = kR[1][iRow];
						fTmp1 = kR[2][iRow];
						kR[1][iRow] = fCos1*fTmp0 - fSin1*fTmp1;
						kR[2][iRow] = fSin1*fTmp0 + fCos1*fTmp1;
					}

					kS[0] = kA[0][0];
					kS[1] = fCos0*fCos1*kA[1][1] -
						fSin1*(fCos0*kA[1][2] - fSin0*kA[2][2]);
					kS[2] = fSin0*fSin1*kA[1][1] +
						fCos1*(fSin0*kA[1][2] + fCos0*kA[2][2]);
					break;
				}
			}
			else
			{
				if (bTest2)
				{
					// 2x2 closed form factorization
					fTmp = (kA[0][0] * kA[0][0] + kA[1][1] * kA[1][1] -
						kA[0][1] * kA[0][1]) / (kA[0][1] * kA[1][1]);
					fTan0 = 0.5f*(-fTmp + Math::Sqrt(fTmp*fTmp + 4.0f));
					fCos0 = Math::InvSqrt(1.0f + fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for (iCol = 0; iCol < 3; iCol++)
					{
						fTmp0 = kL[iCol][0];
						fTmp1 = kL[iCol][1];
						kL[iCol][0] = fCos0*fTmp0 - fSin0*fTmp1;
						kL[iCol][1] = fSin0*fTmp0 + fCos0*fTmp1;
					}

					fTan1 = (kA[0][1] - kA[1][1] * fTan0) / kA[0][0];
					fCos1 = Math::InvSqrt(1.0f + fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for (iRow = 0; iRow < 3; iRow++)
					{
						fTmp0 = kR[0][iRow];
						fTmp1 = kR[1][iRow];
						kR[0][iRow] = fCos1*fTmp0 - fSin1*fTmp1;
						kR[1][iRow] = fSin1*fTmp0 + fCos1*fTmp1;
					}

					kS[0] = fCos0*fCos1*kA[0][0] -
						fSin1*(fCos0*kA[0][1] - fSin0*kA[1][1]);
					kS[1] = fSin0*fSin1*kA[0][0] +
						fCos1*(fSin0*kA[0][1] + fCos0*kA[1][1]);
					kS[2] = kA[2][2];
					break;
				}
				else
				{
					GolubKahanStep(kA, kL, kR);
				}
			}
		}

		// positize diagonal
		for (iRow = 0; iRow < 3; iRow++)
		{
			if (kS[iRow] < 0.0)
			{
				kS[iRow] = -kS[iRow];
				for (iCol = 0; iCol < 3; iCol++)
					kR[iRow][iCol] = -kR[iRow][iCol];
			}
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::SingularValueComposition(const Matrix3& kL,
		const Vector3& kS, const Matrix3& kR)
	{
		size_t iRow, iCol;
		Matrix3 kTmp;

		// product S*R
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
				kTmp[iRow][iCol] = kS[iRow] * kR[iRow][iCol];
		}

		// product L*S*R
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
			{
				m[iRow][iCol] = 0.0;
				for (int iMid = 0; iMid < 3; iMid++)
					m[iRow][iCol] += kL[iRow][iMid] * kTmp[iMid][iCol];
			}
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::Orthonormalize()
	{
		// 用Gram-Schmidth 算法来计算正交化
		// 如果这个矩阵是M = [m0|m1|m2],那么正交输出矩阵是Q = [q0|q1|q2],
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// 其 |V| 指定向量V的长度，A*B指定向量A与B的点积

		// 计算 q0
		Real fInvLength = Math::InvSqrt(m[0][0] * m[0][0]
			+ m[1][0] * m[1][0] +
			m[2][0] * m[2][0]);

		m[0][0] *= fInvLength;
		m[1][0] *= fInvLength;
		m[2][0] *= fInvLength;

		// 计算 q1
		Real fDot0 =
			m[0][0] * m[0][1] +
			m[1][0] * m[1][1] +
			m[2][0] * m[2][1];

		m[0][1] -= fDot0*m[0][0];
		m[1][1] -= fDot0*m[1][0];
		m[2][1] -= fDot0*m[2][0];

		fInvLength = Math::InvSqrt(m[0][1] * m[0][1] +
			m[1][1] * m[1][1] +
			m[2][1] * m[2][1]);

		m[0][1] *= fInvLength;
		m[1][1] *= fInvLength;
		m[2][1] *= fInvLength;

		// 计算 q2
		Real fDot1 =
			m[0][1] * m[0][2] +
			m[1][1] * m[1][2] +
			m[2][1] * m[2][2];

		fDot0 =
			m[0][0] * m[0][2] +
			m[1][0] * m[1][2] +
			m[2][0] * m[2][2];

		m[0][2] -= fDot0*m[0][0] + fDot1*m[0][1];
		m[1][2] -= fDot0*m[1][0] + fDot1*m[1][1];
		m[2][2] -= fDot0*m[2][0] + fDot1*m[2][1];

		fInvLength = Math::InvSqrt(m[0][2] * m[0][2] +
			m[1][2] * m[1][2] +
			m[2][2] * m[2][2]);

		m[0][2] *= fInvLength;
		m[1][2] *= fInvLength;
		m[2][2] *= fInvLength;
	}
	//-----------------------------------------------------------------------
	void Matrix3::QDUDecomposition(Matrix3& kQ,
		Vector3& kD, Vector3& kU) const
	{
		// 因子M = QR = QDU 其中Q是正交的，D是对角的，而U是对角线上的上三角矩阵
		// 算法用Gram-Schmidt 正交化算法 (QR 算法)
		//
		// 如果 M = [ m0 | m1 | m2 ] 而 Q = [ q0 | q1 | q2 ], 那么
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// 其中 |V| 指定了向量V长度，A*B是AB的点积。 
		// 那么矩阵R的元素为
		//
		//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
		//   r10 = 0      r11 = q1*m1  r12 = q1*m2
		//   r20 = 0      r21 = 0      r22 = q2*m2
		//
		// 所以 D = diag(r00,r11,r22) 并且 U 的元素 u01 = r01/r00,
		// u02 = r02/r00, and u12 = r12/r11.

		// Q = 旋转
		// D = 缩放
		// U = 剪切

		// D 保存着三个对角线元素r00, r11, r22
		// U 保存的元素 U[0] = u01, U[1] = u02, U[2] = u12

		//构建正交矩阵Q
		Real fInvLength = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
		if (!Sapphire::Math::RealEqual(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

		kQ[0][0] = m[0][0] * fInvLength;
		kQ[1][0] = m[1][0] * fInvLength;
		kQ[2][0] = m[2][0] * fInvLength;

		Real fDot = kQ[0][0] * m[0][1] + kQ[1][0] * m[1][1] +
			kQ[2][0] * m[2][1];
		kQ[0][1] = m[0][1] - fDot*kQ[0][0];
		kQ[1][1] = m[1][1] - fDot*kQ[1][0];
		kQ[2][1] = m[2][1] - fDot*kQ[2][0];
		fInvLength = kQ[0][1] * kQ[0][1] + kQ[1][1] * kQ[1][1] + kQ[2][1] * kQ[2][1];
		if (!Sapphire::Math::RealEqual(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

		kQ[0][1] *= fInvLength;
		kQ[1][1] *= fInvLength;
		kQ[2][1] *= fInvLength;

		fDot = kQ[0][0] * m[0][2] + kQ[1][0] * m[1][2] +
			kQ[2][0] * m[2][2];
		kQ[0][2] = m[0][2] - fDot*kQ[0][0];
		kQ[1][2] = m[1][2] - fDot*kQ[1][0];
		kQ[2][2] = m[2][2] - fDot*kQ[2][0];
		fDot = kQ[0][1] * m[0][2] + kQ[1][1] * m[1][2] +
			kQ[2][1] * m[2][2];
		kQ[0][2] -= fDot*kQ[0][1];
		kQ[1][2] -= fDot*kQ[1][1];
		kQ[2][2] -= fDot*kQ[2][1];
		fInvLength = kQ[0][2] * kQ[0][2] + kQ[1][2] * kQ[1][2] + kQ[2][2] * kQ[2][2];
		if (!Sapphire::Math::RealEqual(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

		kQ[0][2] *= fInvLength;
		kQ[1][2] *= fInvLength;
		kQ[2][2] *= fInvLength;

		// 保证这个正交矩阵有行列式 1 
		Real fDet = kQ[0][0] * kQ[1][1] * kQ[2][2] + kQ[0][1] * kQ[1][2] * kQ[2][0] +
			kQ[0][2] * kQ[1][0] * kQ[2][1] - kQ[0][2] * kQ[1][1] * kQ[2][0] -
			kQ[0][1] * kQ[1][0] * kQ[2][2] - kQ[0][0] * kQ[1][2] * kQ[2][1];

		if (fDet < 0.0)
		{
			for (size_t iRow = 0; iRow < 3; iRow++)
				for (size_t iCol = 0; iCol < 3; iCol++)
					kQ[iRow][iCol] = -kQ[iRow][iCol];
		}

		// 构建右 矩阵R
		Matrix3 kR;
		kR[0][0] = kQ[0][0] * m[0][0] + kQ[1][0] * m[1][0] +
			kQ[2][0] * m[2][0];
		kR[0][1] = kQ[0][0] * m[0][1] + kQ[1][0] * m[1][1] +
			kQ[2][0] * m[2][1];
		kR[1][1] = kQ[0][1] * m[0][1] + kQ[1][1] * m[1][1] +
			kQ[2][1] * m[2][1];
		kR[0][2] = kQ[0][0] * m[0][2] + kQ[1][0] * m[1][2] +
			kQ[2][0] * m[2][2];
		kR[1][2] = kQ[0][1] * m[0][2] + kQ[1][1] * m[1][2] +
			kQ[2][1] * m[2][2];
		kR[2][2] = kQ[0][2] * m[0][2] + kQ[1][2] * m[1][2] +
			kQ[2][2] * m[2][2];

		// 放大分量
		kD[0] = kR[0][0];
		kD[1] = kR[1][1];
		kD[2] = kR[2][2];

		// 横切分量
		Real fInvD0 = 1.0f / kD[0];
		kU[0] = kR[0][1] * fInvD0;
		kU[1] = kR[0][2] * fInvD0;
		kU[2] = kR[1][2] / kD[1];
	}
	//-----------------------------------------------------------------------
	Real Matrix3::MaxCubicRoot(Real afCoeff[3])
	{
		// 谱范数是对于A^T * A, 所以特征多项式
		// P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 有三个正实根
		// 这产生了断言 c[0] < 0 和 c[2]*c[2] >= 3*c[1].

		const Real fOneThird = 1.0 / 3.0;
		const Real fEpsilon = 1e-06;
		Real fDiscr = afCoeff[2] * afCoeff[2] - 3.0f*afCoeff[1];
		if (fDiscr <= fEpsilon)
			return -fOneThird*afCoeff[2];

		// 计算一个P(x)的上界，这假定 A^T*A
		// 已经被通过它的最大元素缩放过
		Real fX = 1.0;
		Real fPoly = afCoeff[0] + fX*(afCoeff[1] + fX*(afCoeff[2] + fX));
		if (fPoly < 0.0)
		{
			// 用一个矩阵范数来找到再最大根的上边界
			fX = Math::Abs(afCoeff[0]);
			Real fTmp = 1.0f + Math::Abs(afCoeff[1]);
			if (fTmp > fX)
				fX = fTmp;
			fTmp = 1.0f + Math::Abs(afCoeff[2]);
			if (fTmp > fX)
				fX = fTmp;
		}

		// 牛顿法找根
		Real fTwoC2 = 2.0f*afCoeff[2];
		for (int i = 0; i < 16; i++)
		{
			fPoly = afCoeff[0] + fX*(afCoeff[1] + fX*(afCoeff[2] + fX));
			if (Math::Abs(fPoly) <= fEpsilon)
				return fX;

			Real fDeriv = afCoeff[1] + fX*(fTwoC2 + 3.0f*fX);
			fX -= fPoly / fDeriv;
		}

		return fX;
	}
	//-----------------------------------------------------------------------
	Real Matrix3::SpectralNorm() const
	{
		Matrix3 kP;
		size_t iRow, iCol;
		Real fPmax = 0.0;
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
			{
				kP[iRow][iCol] = 0.0;
				for (int iMid = 0; iMid < 3; iMid++)
				{
					kP[iRow][iCol] +=
						m[iMid][iRow] * m[iMid][iCol];
				}
				if (kP[iRow][iCol] > fPmax)
					fPmax = kP[iRow][iCol];
			}
		}

		Real fInvPmax = 1.0f / fPmax;
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
				kP[iRow][iCol] *= fInvPmax;
		}

		Real afCoeff[3];
		afCoeff[0] = -(kP[0][0] * (kP[1][1] * kP[2][2] - kP[1][2] * kP[2][1]) +
			kP[0][1] * (kP[2][0] * kP[1][2] - kP[1][0] * kP[2][2]) +
			kP[0][2] * (kP[1][0] * kP[2][1] - kP[2][0] * kP[1][1]));
		afCoeff[1] = kP[0][0] * kP[1][1] - kP[0][1] * kP[1][0] +
			kP[0][0] * kP[2][2] - kP[0][2] * kP[2][0] +
			kP[1][1] * kP[2][2] - kP[1][2] * kP[2][1];
		afCoeff[2] = -(kP[0][0] + kP[1][1] + kP[2][2]);

		Real fRoot = MaxCubicRoot(afCoeff);
		Real fNorm = Math::Sqrt(fPmax*fRoot);
		return fNorm;
	}
	//-----------------------------------------------------------------------
	void Matrix3::ToAngleAxis(Vector3& rkAxis, Radian& rfRadians) const
	{

		Real fTrace = m[0][0] + m[1][1] + m[2][2];
		Real fCos = 0.5f*(fTrace - 1.0f);
		rfRadians = Math::ACos(fCos);  // 在 [0,PI]

		if (rfRadians > Radian(0.0))
		{
			if (rfRadians < Radian(Math::_PI))
			{
				rkAxis.x = m[2][1] - m[1][2];
				rkAxis.y = m[0][2] - m[2][0];
				rkAxis.z = m[1][0] - m[0][1];
				rkAxis.normalise();
			}
			else
			{
				// 角度是 PI
				float fHalfInverse;
				if (m[0][0] >= m[1][1])
				{
					// r00 >= r11
					if (m[0][0] >= m[2][2])
					{
						// r00 是最大对角项
						rkAxis.x = 0.5f*Math::Sqrt(m[0][0] -
							m[1][1] - m[2][2] + 1.0f);
						fHalfInverse = 0.5f / rkAxis.x;
						rkAxis.y = fHalfInverse*m[0][1];
						rkAxis.z = fHalfInverse*m[0][2];
					}
					else
					{
						// r22 是最大对角项
						rkAxis.z = 0.5f*Math::Sqrt(m[2][2] -
							m[0][0] - m[1][1] + 1.0f);
						fHalfInverse = 0.5f / rkAxis.z;
						rkAxis.x = fHalfInverse*m[0][2];
						rkAxis.y = fHalfInverse*m[1][2];
					}
				}
				else
				{
					// r11 > r00
					if (m[1][1] >= m[2][2])
					{
						// r11 是最大对角项
						rkAxis.y = 0.5f*Math::Sqrt(m[1][1] -
							m[0][0] - m[2][2] + 1.0f);
						fHalfInverse = 0.5f / rkAxis.y;
						rkAxis.x = fHalfInverse*m[0][1];
						rkAxis.z = fHalfInverse*m[1][2];
					}
					else
					{
						// r22 是最大对角项
						rkAxis.z = 0.5f*Math::Sqrt(m[2][2] -
							m[0][0] - m[1][1] + 1.0f);
						fHalfInverse = 0.5f / rkAxis.z;
						rkAxis.x = fHalfInverse*m[0][2];
						rkAxis.y = fHalfInverse*m[1][2];
					}
				}
			}
		}
		else
		{
			// 这个角度是0并且矩阵是单位矩阵，任意轴都可以工作，所以就用x轴
			rkAxis.x = 1.0;
			rkAxis.y = 0.0;
			rkAxis.z = 0.0;
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromAngleAxis(const Vector3& rkAxis, const Radian& fRadians)
	{
		Real fCos = Math::Cos(fRadians);
		Real fSin = Math::Sin(fRadians);
		Real fOneMinusCos = 1.0f - fCos;
		Real fX2 = rkAxis.x*rkAxis.x;
		Real fY2 = rkAxis.y*rkAxis.y;
		Real fZ2 = rkAxis.z*rkAxis.z;
		Real fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
		Real fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
		Real fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
		Real fXSin = rkAxis.x*fSin;
		Real fYSin = rkAxis.y*fSin;
		Real fZSin = rkAxis.z*fSin;

		m[0][0] = fX2*fOneMinusCos + fCos;
		m[0][1] = fXYM - fZSin;
		m[0][2] = fXZM + fYSin;
		m[1][0] = fXYM + fZSin;
		m[1][1] = fY2*fOneMinusCos + fCos;
		m[1][2] = fYZM - fXSin;
		m[2][0] = fXZM - fYSin;
		m[2][1] = fYZM + fXSin;
		m[2][2] = fZ2*fOneMinusCos + fCos;
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesXYZ(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz          -cy*sz           sy
		//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
		//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

		rfPAngle = Radian(Math::ASin(m[0][2]));
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(-m[1][2], m[2][2]);
				rfRAngle = Math::ATan2(-m[0][1], m[0][0]);
				return true;
			}
			else
			{
				// WARNING.  没有唯一解
				Radian fRmY = Math::ATan2(m[1][0], m[1][1]);
				rfRAngle = Radian(0.0);  // 
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING. 没有唯一解
			Radian fRpY = Math::ATan2(m[1][0], m[1][1]);
			rfRAngle = Radian(0.0);  //  
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz          -sz              cz*sy
		//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
		//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

		rfPAngle = Math::ASin(-m[0][1]);
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(m[2][1], m[1][1]);
				rfRAngle = Math::ATan2(m[0][2], m[0][0]);
				return true;
			}
			else
			{
				// WARNING.  没有唯一解
				Radian fRmY = Math::ATan2(-m[2][0], m[2][2]);
				rfRAngle = Radian(0.0);  
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING. 没有唯一解
			Radian fRpY = Math::ATan2(-m[2][0], m[2][2]);
			rfRAngle = Radian(0.0);  
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
		//        cx*sz           cx*cz          -sx
		//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

		rfPAngle = Math::ASin(-m[1][2]);
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(m[0][2], m[2][2]);
				rfRAngle = Math::ATan2(m[1][0], m[1][1]);
				return true;
			}
			else
			{
				// WARNING.  没有唯一解
				Radian fRmY = Math::ATan2(-m[0][1], m[0][0]);
				rfRAngle = Radian(0.0);   
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  没有唯一解
			Radian fRpY = Math::ATan2(-m[0][1], m[0][0]);
			rfRAngle = Radian(0.0);  
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
		//        sz              cx*cz          -cz*sx
		//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

		rfPAngle = Math::ASin(m[1][0]);
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(-m[2][0], m[0][0]);
				rfRAngle = Math::ATan2(-m[1][2], m[1][1]);
				return true;
			}
			else
			{
				// WARNING. 没有唯一解
				Radian fRmY = Math::ATan2(m[2][1], m[2][2]);
				rfRAngle = Radian(0.0);  
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  没有唯一解
			Radian fRpY = Math::ATan2(m[2][1], m[2][2]);
			rfRAngle = Radian(0.0);   
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
		//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
		//       -cx*sy           sx              cx*cy

		rfPAngle = Math::ASin(m[2][1]);
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(-m[0][1], m[1][1]);
				rfRAngle = Math::ATan2(-m[2][0], m[2][2]);
				return true;
			}
			else
			{
				// WARNING.  没有唯一解
				Radian fRmY = Math::ATan2(m[0][2], m[0][0]);
				rfRAngle = Radian(0.0);  
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  没有唯一解
			Radian fRpY = Math::ATan2(m[0][2], m[0][0]);
			rfRAngle = Radian(0.0); 
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::ToEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle,
		Radian& rfRAngle) const
	{
		// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
		//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
		//       -sy              cy*sx           cx*cy

		rfPAngle = Math::ASin(-m[2][0]);
		if (rfPAngle < Radian(Math::HALF_PI))
		{
			if (rfPAngle > Radian(-Math::HALF_PI))
			{
				rfYAngle = Math::ATan2(m[1][0], m[0][0]);
				rfRAngle = Math::ATan2(m[2][1], m[2][2]);
				return true;
			}
			else
			{
				// WARNING.  没有唯一解
				Radian fRmY = Math::ATan2(-m[0][1], m[0][2]);
				rfRAngle = Radian(0.0); 
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  没有唯一解
			Radian fRpY = Math::ATan2(-m[0][1], m[0][2]);
			rfRAngle = Radian(0.0); 
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		*this = kXMat*(kYMat*kZMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		*this = kXMat*(kZMat*kYMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		*this = kYMat*(kXMat*kZMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		*this = kYMat*(kZMat*kXMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		*this = kZMat*(kXMat*kYMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::FromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle,
		const Radian& fRAngle)
	{
		Real fCos, fSin;

		fCos = Math::Cos(fYAngle);
		fSin = Math::Sin(fYAngle);
		Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

		fCos = Math::Cos(fPAngle);
		fSin = Math::Sin(fPAngle);
		Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

		fCos = Math::Cos(fRAngle);
		fSin = Math::Sin(fRAngle);
		Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

		*this = kZMat*(kYMat*kXMat);
	}
	//-----------------------------------------------------------------------
	void Matrix3::Tridiagonal(Real afDiag[3], Real afSubDiag[3])
	{
		// Householder 变换 T = Q^t M Q
		//   Input:
		//     mat, 对称 3x3 矩阵 M
		//   Output:
		//     mat, 正交矩阵 Q
		//     diag, T的对角线元素
		//     subd, T的子对角线元素 (T是对称的)

		Real fA = m[0][0];
		Real fB = m[0][1];
		Real fC = m[0][2];
		Real fD = m[1][1];
		Real fE = m[1][2];
		Real fF = m[2][2];

		afDiag[0] = fA;
		afSubDiag[2] = 0.0;
		if (Math::Abs(fC) >= EPSILON)
		{
			Real fLength = Math::Sqrt(fB*fB + fC*fC);
			Real fInvLength = 1.0f / fLength;
			fB *= fInvLength;
			fC *= fInvLength;
			Real fQ = 2.0f*fB*fE + fC*(fF - fD);
			afDiag[1] = fD + fC*fQ;
			afDiag[2] = fF - fC*fQ;
			afSubDiag[0] = fLength;
			afSubDiag[1] = fE - fB*fQ;
			m[0][0] = 1.0;
			m[0][1] = 0.0;
			m[0][2] = 0.0;
			m[1][0] = 0.0;
			m[1][1] = fB;
			m[1][2] = fC;
			m[2][0] = 0.0;
			m[2][1] = fC;
			m[2][2] = -fB;
		}
		else
		{
			afDiag[1] = fD;
			afDiag[2] = fF;
			afSubDiag[0] = fB;
			afSubDiag[1] = fE;
			m[0][0] = 1.0;
			m[0][1] = 0.0;
			m[0][2] = 0.0;
			m[1][0] = 0.0;
			m[1][1] = 1.0;
			m[1][2] = 0.0;
			m[2][0] = 0.0;
			m[2][1] = 0.0;
			m[2][2] = 1.0;
		}
	}
	//-----------------------------------------------------------------------
	bool Matrix3::QLAlgorithm(Real afDiag[3], Real afSubDiag[3])
	{
		// QL 迭代

		for (int i0 = 0; i0 < 3; i0++)
		{
			const unsigned int iMaxIter = 32;
			unsigned int iIter;
			for (iIter = 0; iIter < iMaxIter; iIter++)
			{
				int i1;
				for (i1 = i0; i1 <= 1; i1++)
				{
					Real fSum = Math::Abs(afDiag[i1]) +
						Math::Abs(afDiag[i1 + 1]);
					if (Math::Abs(afSubDiag[i1]) + fSum == fSum)
						break;
				}
				if (i1 == i0)
					break;

				Real fTmp0 = (afDiag[i0 + 1] - afDiag[i0]) / (2.0f*afSubDiag[i0]);
				Real fTmp1 = Math::Sqrt(fTmp0*fTmp0 + 1.0f);
				if (fTmp0 < 0.0)
					fTmp0 = afDiag[i1] - afDiag[i0] + afSubDiag[i0] / (fTmp0 - fTmp1);
				else
					fTmp0 = afDiag[i1] - afDiag[i0] + afSubDiag[i0] / (fTmp0 + fTmp1);
				Real fSin = 1.0;
				Real fCos = 1.0;
				Real fTmp2 = 0.0;
				for (int i2 = i1 - 1; i2 >= i0; i2--)
				{
					Real fTmp3 = fSin*afSubDiag[i2];
					Real fTmp4 = fCos*afSubDiag[i2];
					if (Math::Abs(fTmp3) >= Math::Abs(fTmp0))
					{
						fCos = fTmp0 / fTmp3;
						fTmp1 = Math::Sqrt(fCos*fCos + 1.0f);
						afSubDiag[i2 + 1] = fTmp3*fTmp1;
						fSin = 1.0f / fTmp1;
						fCos *= fSin;
					}
					else
					{
						fSin = fTmp3 / fTmp0;
						fTmp1 = Math::Sqrt(fSin*fSin + 1.0f);
						afSubDiag[i2 + 1] = fTmp0*fTmp1;
						fCos = 1.0f / fTmp1;
						fSin *= fCos;
					}
					fTmp0 = afDiag[i2 + 1] - fTmp2;
					fTmp1 = (afDiag[i2] - fTmp0)*fSin + 2.0f*fTmp4*fCos;
					fTmp2 = fSin*fTmp1;
					afDiag[i2 + 1] = fTmp0 + fTmp2;
					fTmp0 = fCos*fTmp1 - fTmp4;

					for (int iRow = 0; iRow < 3; iRow++)
					{
						fTmp3 = m[iRow][i2 + 1];
						m[iRow][i2 + 1] = fSin*m[iRow][i2] +
							fCos*fTmp3;
						m[iRow][i2] = fCos*m[iRow][i2] -
							fSin*fTmp3;
					}
				}
				afDiag[i0] -= fTmp2;
				afSubDiag[i0] = fTmp0;
				afSubDiag[i1] = 0.0;
			}

			if (iIter == iMaxIter)
			{
			 
				return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	void Matrix3::EigenSolveSymmetric(Real afEigenvalue[3],
		Vector3 akEigenvector[3]) const
	{
		Matrix3 kMatrix = *this;
		Real afSubDiag[3];
		kMatrix.Tridiagonal(afEigenvalue, afSubDiag);
		kMatrix.QLAlgorithm(afEigenvalue, afSubDiag);

		for (size_t i = 0; i < 3; i++)
		{
			akEigenvector[i][0] = kMatrix[0][i];
			akEigenvector[i][1] = kMatrix[1][i];
			akEigenvector[i][2] = kMatrix[2][i];
		}

		// 从右手坐标系构造特征向量
		Vector3 kCross = akEigenvector[1].crossProduct(akEigenvector[2]);
		Real fDet = akEigenvector[0].dotProduct(kCross);
		if (fDet < 0.0)
		{
			akEigenvector[2][0] = -akEigenvector[2][0];
			akEigenvector[2][1] = -akEigenvector[2][1];
			akEigenvector[2][2] = -akEigenvector[2][2];
		}
	}
	//-----------------------------------------------------------------------
	//张量积
	void Matrix3::TensorProduct(const Vector3& rkU, const Vector3& rkV,
		Matrix3& rkProduct)
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				rkProduct[iRow][iCol] = rkU[iRow] * rkV[iCol];
		}
	}
	//-----------------------------------------------------------------------
}