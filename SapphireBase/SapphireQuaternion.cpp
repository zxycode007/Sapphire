#include "SapphirePrerequisites.h"
#include "SapphireMath.h"
#include "SapphireQuaternion.h"
#include "SapphireVector3.h"
#include "SapphireMatrix3.h"
#include "SapphireMatrix4.h"

namespace Sapphire
{
	const Real Quaternion::msEpsilon = 1e-03;
	const Quaternion Quaternion::ZERO(0, 0, 0, 0);
	const Quaternion Quaternion::IDENTITY(1, 0, 0, 0);

	//-----------------------------------------------------------------------

	Quaternion::Quaternion(const Vector3& vec) 		 
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	//-----------------------------------------------------------------------
	void Quaternion::set(Real x, Real y, Real z, Real w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}



	//-----------------------------------------------------------------------
	void Quaternion::FromRotationMatrix(const Matrix3& kRot)
	{
		// Ken Shoemake 1987 在SIGGRAPH 的算法：四元数积分和快速动画
		// "Quaternion Calculus and Fast Animation". 

		Real fTrace = kRot[0][0] + kRot[1][1] + kRot[2][2];
		Real fRoot;

		if (fTrace > 0.0)
		{
			// |w| > 1/2, 可能同样选择 w > 1/2
			fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
			w = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;  // 1/(4w)
			x = (kRot[2][1] - kRot[1][2])*fRoot;
			y = (kRot[0][2] - kRot[2][0])*fRoot;
			z = (kRot[1][0] - kRot[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if (kRot[1][1] > kRot[0][0])
				i = 1;
			if (kRot[2][2] > kRot[i][i])
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = Math::Sqrt(kRot[i][i] - kRot[j][j] - kRot[k][k] + 1.0f);
			Real* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			w = (kRot[k][j] - kRot[j][k])*fRoot;
			*apkQuat[j] = (kRot[j][i] + kRot[i][j])*fRoot;
			*apkQuat[k] = (kRot[k][i] + kRot[i][k])*fRoot;
		}
	}
	//-----------------------------------------------------------------------
	void Quaternion::ToRotationMatrix(Matrix3& kRot) const
	{
		Real fTx = x + x;
		Real fTy = y + y;
		Real fTz = z + z;
		Real fTwx = fTx*w;
		Real fTwy = fTy*w;
		Real fTwz = fTz*w;
		Real fTxx = fTx*x;
		Real fTxy = fTy*x;
		Real fTxz = fTz*x;
		Real fTyy = fTy*y;
		Real fTyz = fTz*y;
		Real fTzz = fTz*z;

		kRot[0][0] = 1.0f - (fTyy + fTzz);
		kRot[0][1] = fTxy - fTwz;
		kRot[0][2] = fTxz + fTwy;
		kRot[1][0] = fTxy + fTwz;
		kRot[1][1] = 1.0f - (fTxx + fTzz);
		kRot[1][2] = fTyz - fTwx;
		kRot[2][0] = fTxz - fTwy;
		kRot[2][1] = fTyz + fTwx;
		kRot[2][2] = 1.0f - (fTxx + fTyy);
	}
	//-----------------------------------------------------------------------
	void Quaternion::FromAngleAxis(const Radian& rfAngle,
		const Vector3& rkAxis)
	{
		// 断言:  axis[] 是单位长度
		//
		// 这个四元数表示的旋转是
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		Radian fHalfAngle(0.5*rfAngle.valueRadians());
		Real fSin = Math::Sin(fHalfAngle);
		w = Math::Cos(fHalfAngle);
		x = fSin*rkAxis.x;
		y = fSin*rkAxis.y;
		z = fSin*rkAxis.z;
	}
	//-----------------------------------------------------------------------
	void Quaternion::ToAngleAxis(Radian& rfAngle, Vector3& rkAxis) const
	{
		// 这个四元数表示的旋转是
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		Real fSqrLength = x*x + y*y + z*z;
		if (fSqrLength > 0.0)
		{
			rfAngle = 2.0*Math::ACos(w).valueRadians();
			Real fInvLength = Math::InvSqrt(fSqrLength);
			rkAxis.x = x*fInvLength;
			rkAxis.y = y*fInvLength;
			rkAxis.z = z*fInvLength;
		}
		else
		{
			// 角度是 0 (mod 2*pi), 所以任何轴都会做
			rfAngle = Radian(0.0);
			rkAxis.x = 1.0;
			rkAxis.y = 0.0;
			rkAxis.z = 0.0;
		}
	}
	//-----------------------------------------------------------------------
	void Quaternion::FromAxes(const Vector3* akAxis)
	{
		Matrix3 kRot;

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			kRot[0][iCol] = akAxis[iCol].x;
			kRot[1][iCol] = akAxis[iCol].y;
			kRot[2][iCol] = akAxis[iCol].z;
		}

		FromRotationMatrix(kRot);
	}
	 

	//-----------------------------------------------------------------------
	void Quaternion::FromAxes(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
	{
		Matrix3 kRot;

		kRot[0][0] = xaxis.x;
		kRot[1][0] = xaxis.y;
		kRot[2][0] = xaxis.z;

		kRot[0][1] = yaxis.x;
		kRot[1][1] = yaxis.y;
		kRot[2][1] = yaxis.z;

		kRot[0][2] = zaxis.x;
		kRot[1][2] = zaxis.y;
		kRot[2][2] = zaxis.z;

		FromRotationMatrix(kRot);

	}
	//-----------------------------------------------------------------------
	void Quaternion::ToAxes(Vector3* akAxis) const
	{
		Matrix3 kRot;

		ToRotationMatrix(kRot);

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			akAxis[iCol].x = kRot[0][iCol];
			akAxis[iCol].y = kRot[1][iCol];
			akAxis[iCol].z = kRot[2][iCol];
		}
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::xAxis(void) const
	{
		//Real fTx  = 2.0*x;
		Real fTy = 2.0f*y;
		Real fTz = 2.0f*z;
		Real fTwy = fTy*w;
		Real fTwz = fTz*w;
		Real fTxy = fTy*x;
		Real fTxz = fTz*x;
		Real fTyy = fTy*y;
		Real fTzz = fTz*z;

		return Vector3(1.0f - (fTyy + fTzz), fTxy + fTwz, fTxz - fTwy);
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::yAxis(void) const
	{
		Real fTx = 2.0f*x;
		Real fTy = 2.0f*y;
		Real fTz = 2.0f*z;
		Real fTwx = fTx*w;
		Real fTwz = fTz*w;
		Real fTxx = fTx*x;
		Real fTxy = fTy*x;
		Real fTyz = fTz*y;
		Real fTzz = fTz*z;

		return Vector3(fTxy - fTwz, 1.0f - (fTxx + fTzz), fTyz + fTwx);
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::zAxis(void) const
	{
		Real fTx = 2.0f*x;
		Real fTy = 2.0f*y;
		Real fTz = 2.0f*z;
		Real fTwx = fTx*w;
		Real fTwy = fTy*w;
		Real fTxx = fTx*x;
		Real fTxz = fTz*x;
		Real fTyy = fTy*y;
		Real fTyz = fTz*y;

		return Vector3(fTxz + fTwy, fTyz - fTwx, 1.0f - (fTxx + fTyy));
	}
	//-----------------------------------------------------------------------
	void Quaternion::ToAxes(Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const
	{
		Matrix3 kRot;

		ToRotationMatrix(kRot);

		xaxis.x = kRot[0][0];
		xaxis.y = kRot[1][0];
		xaxis.z = kRot[2][0];

		yaxis.x = kRot[0][1];
		yaxis.y = kRot[1][1];
		yaxis.z = kRot[2][1];

		zaxis.x = kRot[0][2];
		zaxis.y = kRot[1][2];
		zaxis.z = kRot[2][2];
	}

	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator+ (const Quaternion& rkQ) const
	{
		return Quaternion(w + rkQ.w, x + rkQ.x, y + rkQ.y, z + rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator- (const Quaternion& rkQ) const
	{
		return Quaternion(w - rkQ.w, x - rkQ.x, y - rkQ.y, z - rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator* (const Quaternion& rkQ) const
	{
		// NOTE:  多个情况四元数位置不能交换
		// p*q != q*p.

		return Quaternion
			(
			w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
			w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
			w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
			w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
			);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator* (Real fScalar) const
	{
		return Quaternion(fScalar*w, fScalar*x, fScalar*y, fScalar*z);
	}
	//-----------------------------------------------------------------------
	Quaternion operator* (Real fScalar, const Quaternion& rkQ)
	{
		return Quaternion(fScalar*rkQ.w, fScalar*rkQ.x, fScalar*rkQ.y,
			fScalar*rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator- () const
	{
		return Quaternion(-w, -x, -y, -z);
	}
	//-----------------------------------------------------------------------
	Real Quaternion::Dot(const Quaternion& rkQ) const
	{
		return w*rkQ.w + x*rkQ.x + y*rkQ.y + z*rkQ.z;
	}
	//-----------------------------------------------------------------------
	Real Quaternion::Norm() const
	{
		return w*w + x*x + y*y + z*z;
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::Inverse() const
	{
		Real fNorm = w*w + x*x + y*y + z*z;
		if (fNorm > 0.0)
		{
			Real fInvNorm = 1.0f / fNorm;
			return Quaternion(w*fInvNorm, -x*fInvNorm, -y*fInvNorm, -z*fInvNorm);
		}
		else
		{
			// 返回一个无效的结果标志这个错误
			return ZERO;
		}
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::UnitInverse() const
	{
		// 断言：'this' 是单位长度
		return Quaternion(w, -x, -y, -z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::Exp() const
	{
		// 如果 q = A*(x*i+y*j+z*k) 而 (x,y,z) 是单位长度, 那么
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  如 sin(A) 接近0
		// 用 exp(q) = cos(A)+A*(x*i+y*j+z*k) 自 A/sin(A) 极限为 1.

		Radian fAngle(Math::Sqrt(x*x + y*y + z*z));
		Real fSin = Math::Sin(fAngle);

		Quaternion kResult;
		kResult.w = Math::Cos(fAngle);

		if (Math::Abs(fSin) >= msEpsilon)
		{
			Real fCoeff = fSin / (fAngle.valueRadians());
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
		}
		else
		{
			kResult.x = x;
			kResult.y = y;
			kResult.z = z;
		}

		return kResult;
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::Log() const
	{
		// 如果 q = cos(A)+sin(A)*(x*i+y*j+z*k) 而 (x,y,z) 是单位长度, 那么
		// log(q) = A*(x*i+y*j+z*k).  如果 sin(A) 接近0, 用 log(q) =
		// sin(A)*(x*i+y*j+z*k) 自 sin(A)/A 极限为 1.

		Quaternion kResult;
		kResult.w = 0.0;

		if (Math::Abs(w) < 1.0)
		{
			Radian fAngle(Math::ACos(w));
			Real fSin = Math::Sin(fAngle);
			if (Math::Abs(fSin) >= msEpsilon)
			{
				Real fCoeff = fAngle.valueRadians() / fSin;
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
				return kResult;
			}
		}

		kResult.x = x;
		kResult.y = y;
		kResult.z = z;

		return kResult;
	}
	//-----------------------------------------------------------------------
	Vector3 Quaternion::operator* (const Vector3& v) const
	{
		// nVidia SDK的实现
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.crossProduct(v);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;

	}
	//-----------------------------------------------------------------------
	bool Quaternion::equals(const Quaternion& rhs, const Radian& tolerance) const
	{
		Real fCos = Dot(rhs);
		Radian angle = Math::ACos(fCos);

		return (Math::Abs(angle.valueRadians()) <= tolerance.valueRadians())
			|| Math::RealEqual(angle.valueRadians(), Math::_PI, tolerance.valueRadians());


	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::Slerp(Real fT, const Quaternion& rkP,
		const Quaternion& rkQ, bool shortestPath)
	{
		Real fCos = rkP.Dot(rkQ);
		Quaternion rkT;

		// 需要翻转旋转？
		if (fCos < 0.0f && shortestPath)
		{
			fCos = -fCos;
			rkT = -rkQ;
		}
		else
		{
			rkT = rkQ;
		}

		if (Math::Abs(fCos) < 1 - msEpsilon)
		{
			// 正常情况 (slerp)
			Real fSin = Math::Sqrt(1 - Math::Sqr(fCos));
			Radian fAngle = Math::ATan2(fSin, fCos);
			Real fInvSin = 1.0f / fSin;
			Real fCoeff0 = Math::Sin((1.0f - fT) * fAngle.valueRadians()) * fInvSin;
			Real fCoeff1 = Math::Sin(fT * fAngle.valueRadians()) * fInvSin;
			return fCoeff0 * rkP + fCoeff1 * rkT;
		}
		else
		{
			// 这有两种情况
			// 1. "rkP" 和 "rkQ" 是非常接近(fCos ~= +1), 所以我们能做安全的做线性插值
			// 2. "rkP" 和 "rkQ" 几乎是每一个 (fCos ~= -1)的反转，这就有无数种插值的可能性。但是我们不可能有修正这个问题的方法，
			// 所有就在这里用线性插值
			Quaternion t = (1.0f - fT) * rkP + fT * rkT;
			// 使这个分量重新标准化
			t.normalise();
			return t;
		}
	}

	//-----------------------------------------------------------------------
	Quaternion Quaternion::Slerp(const Quaternion& rkP,
		const Quaternion& rkQ, Real fT)
	{
		return Slerp(fT, rkP, rkQ, false);
	}


	//-----------------------------------------------------------------------
	Quaternion Quaternion::SlerpExtraSpins(Real fT,
		const Quaternion& rkP, const Quaternion& rkQ, int iExtraSpins)
	{
		Real fCos = rkP.Dot(rkQ);
		Radian fAngle(Math::ACos(fCos));

		if (Math::Abs(fAngle.valueRadians()) < msEpsilon)
			return rkP;

		Real fSin = Math::Sin(fAngle);
		Radian fPhase(Math::_PI*iExtraSpins*fT);
		Real fInvSin = 1.0f / fSin;
		Real fCoeff0 = Math::Sin((1.0f - fT)*fAngle.valueRadians() - fPhase.valueRadians())*fInvSin;
		Real fCoeff1 = Math::Sin(fT*fAngle.valueRadians() + fPhase.valueRadians())*fInvSin;
		return fCoeff0*rkP + fCoeff1*rkQ;
	}
	//-----------------------------------------------------------------------
	void Quaternion::Intermediate(const Quaternion& rkQ0,
		const Quaternion& rkQ1, const Quaternion& rkQ2,
		Quaternion& rkA, Quaternion& rkB)
	{
		// 断言:  q0, q1, q2 是单位向量

		Quaternion kQ0inv = rkQ0.UnitInverse();
		Quaternion kQ1inv = rkQ1.UnitInverse();
		Quaternion rkP0 = kQ0inv*rkQ1;
		Quaternion rkP1 = kQ1inv*rkQ2;
		Quaternion kArg = 0.25*(rkP0.Log() - rkP1.Log());
		Quaternion kMinusArg = -kArg;

		rkA = rkQ1*kArg.Exp();
		rkB = rkQ1*kMinusArg.Exp();
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::Squad(Real fT,
		const Quaternion& rkP, const Quaternion& rkA,
		const Quaternion& rkB, const Quaternion& rkQ, bool shortestPath)
	{
		Real fSlerpT = 2.0f*fT*(1.0f - fT);
		Quaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
		Quaternion kSlerpQ = Slerp(fT, rkA, rkB);
		return Slerp(fSlerpT, kSlerpP, kSlerpQ);
	}
	//-----------------------------------------------------------------------
	Real Quaternion::normalise(void)
	{
		Real len = Norm();
		Real factor = 1.0f / Math::Sqrt(len);
		*this = *this * factor;
		return len;
	}
	//-----------------------------------------------------------------------
	Radian Quaternion::getRoll(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// roll = atan2(localx.y, localx.x)
			// 拾取部分xAxis() 实现我们需要
			//			Real fTx  = 2.0*x;
			Real fTy = 2.0f*y;
			Real fTz = 2.0f*z;
			Real fTwz = fTz*w;
			Real fTxy = fTy*x;
			Real fTyy = fTy*y;
			Real fTzz = fTz*z;

			// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

			return Radian(Math::ATan2(fTxy + fTwz, 1.0f - (fTyy + fTzz)));

		}
		else
		{
			return Radian(Math::ATan2(2 * (x*y + w*z), w*w + x*x - y*y - z*z));
		}
	}
	//-----------------------------------------------------------------------
	Radian Quaternion::getPitch(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// pitch = atan2(localy.z, localy.y)
			// 拾取部分 yAxis() 实现我们需要
			Real fTx = 2.0f*x;
			//			Real fTy  = 2.0f*y;
			Real fTz = 2.0f*z;
			Real fTwx = fTx*w;
			Real fTxx = fTx*x;
			Real fTyz = fTz*y;
			Real fTzz = fTz*z;

			// Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
			return Radian(Math::ATan2(fTyz + fTwx, 1.0f - (fTxx + fTzz)));
		}
		else
		{
			// 内部版本
			return Radian(Math::ATan2(2 * (y*z + w*x), w*w - x*x - y*y + z*z));
		}
	}
	//-----------------------------------------------------------------------
	Radian Quaternion::getYaw(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// yaw = atan2(localz.x, localz.z)
			//拾取部分 zAxis() 实现我们需要
			Real fTx = 2.0f*x;
			Real fTy = 2.0f*y;
			Real fTz = 2.0f*z;
			Real fTwy = fTy*w;
			Real fTxx = fTx*x;
			Real fTxz = fTz*x;
			Real fTyy = fTy*y;

			// Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

			return Radian(Math::ATan2(fTxz + fTwy, 1.0f - (fTxx + fTyy)));

		}
		else
		{
			// 实现我们需要
			return Radian(Math::ASin(-2 * (x*z - w*y)));
		}
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::nlerp(Real fT, const Quaternion& rkP,
		const Quaternion& rkQ, bool shortestPath)
	{
		Quaternion result;
		Real fCos = rkP.Dot(rkQ);
		if (fCos < 0.0f && shortestPath)
		{
			result = rkP + fT * ((-rkQ) - rkP);
		}
		else
		{
			result = rkP + fT * (rkQ - rkP);
		}
		result.normalise();
		return result;
	}


	inline Quaternion& Quaternion::makeIdentity()
	{
		w = 1.f;
		x = 0.f;
		y = 0.f;
		z = 0.f;
		return *this;
	}

	Quaternion& Quaternion::rotationFromTo(const Vector3& from, const Vector3& to)
	{
		// Based on Stan Melax's article in Game Programming Gems
		// Copy, since cannot modify local
		Vector3 v0 = from;
		Vector3 v1 = to;
		v0.normalize();
		v1.normalize();

		const FLOAT32 d = v0.dotProduct(v1);
		if (d >= 1.0f) // If dot == 1, vectors are the same
		{
			return makeIdentity();
		}
		else if (d <= -1.0f) // exactly opposite
		{
			Vector3 axis(1.0f, 0.f, 0.f);
			axis = axis.crossProduct(v0);
			if (axis.length() == 0)
			{
				axis.set(0.f, 1.f, 0.f);
				axis = axis.crossProduct(v0);
			}
			// same as fromAngleAxis(PI, axis).normalize();
			set(axis.x, axis.y, axis.z, 0);
			normalise();
			return *this;
		}

		const FLOAT32 s = sqrtf((1 + d) * 2); // optimize inv_sqrt
		const FLOAT32 invs = 1.f / s;
		const Vector3 c = v0.crossProduct(v1)*invs;
		set(c.x, c.y, c.z, s * 0.5f);
		normalise();
		return *this;
	}

	inline void Quaternion::getMatrixCenter(Matrix4 &dest,
		const Vector3 &center,
		const Vector3 &translation) const
	{
		dest = dest.transpose(); 
		dest.getIndex(0) = 1.0f - 2.0f*y*y - 2.0f*z*z;
		dest.getIndex(1) = 2.0f*x*y + 2.0f*z*w;
		dest.getIndex(2) = 2.0f*x*z - 2.0f*y*w;
		dest.getIndex(3) = 0.0f;

		dest.getIndex(4) = 2.0f*x*y - 2.0f*z*w;
		dest.getIndex(5) = 1.0f - 2.0f*x*x - 2.0f*z*z;
		dest.getIndex(6) = 2.0f*z*y + 2.0f*x*w;
		dest.getIndex(7) = 0.0f;

		dest.getIndex(8) = 2.0f*x*z + 2.0f*y*w;
		dest.getIndex(9) = 2.0f*z*y - 2.0f*x*w;
		dest.getIndex(10) = 1.0f - 2.0f*x*x - 2.0f*y*y;
		dest.getIndex(11) = 0.0f;
		dest = dest.transpose();
		dest.setRotationCenter(center, translation);
	}

	void Quaternion::getMatrix(Matrix4 &dest, const Vector3 &translation) const
	{
		Matrix3 mat3;
		ToRotationMatrix(mat3);
		dest = Matrix4(mat3);
		dest.setTrans(translation);

	}

	void Quaternion::getMatrix(Matrix4 &dest) const
	{
		Matrix3 mat3;
		ToRotationMatrix(mat3);
		dest = Matrix4(mat3);
		//dest.setTrans(translation);

	}
}