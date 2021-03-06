#include "SapphirePrerequisites.h"
#include "SapphireMatrix4.h"
#include "SapphireVector2.h"

namespace Sapphire
{
	const Matrix4 Matrix4::ZERO(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);

	const Matrix4 Matrix4::ZEROAFFINE(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 1);

	const Matrix4 Matrix4::IDENTITY(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	const Matrix4 Matrix4::CLIPSPACE2DTOIMAGESPACE(
		0.5, 0, 0, 0.5,
		0, -0.5, 0, 0.5,
		0, 0, 1, 0,
		0, 0, 0, 1);

	//-----------------------------------------------------------------------
	inline static Real
		MINOR(const Matrix4& m, const size_t r0, const size_t r1, const size_t r2,
		const size_t c0, const size_t c1, const size_t c2)
	{
		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
			m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
	}
	//-----------------------------------------------------------------------
	Matrix4 Matrix4::adjoint() const
	{
		return Matrix4(MINOR(*this, 1, 2, 3, 1, 2, 3),
			-MINOR(*this, 0, 2, 3, 1, 2, 3),
			MINOR(*this, 0, 1, 3, 1, 2, 3),
			-MINOR(*this, 0, 1, 2, 1, 2, 3),

			-MINOR(*this, 1, 2, 3, 0, 2, 3),
			MINOR(*this, 0, 2, 3, 0, 2, 3),
			-MINOR(*this, 0, 1, 3, 0, 2, 3),
			MINOR(*this, 0, 1, 2, 0, 2, 3),

			MINOR(*this, 1, 2, 3, 0, 1, 3),
			-MINOR(*this, 0, 2, 3, 0, 1, 3),
			MINOR(*this, 0, 1, 3, 0, 1, 3),
			-MINOR(*this, 0, 1, 2, 0, 1, 3),

			-MINOR(*this, 1, 2, 3, 0, 1, 2),
			MINOR(*this, 0, 2, 3, 0, 1, 2),
			-MINOR(*this, 0, 1, 3, 0, 1, 2),
			MINOR(*this, 0, 1, 2, 0, 1, 2));
	}
	//-----------------------------------------------------------------------
	Real Matrix4::determinant() const
	{
		return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
			m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
			m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
			m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
	}
	//-----------------------------------------------------------------------
	Matrix4 Matrix4::inverse() 
	{
		if (RowMajor == true)
		{
			transpose();
		}

		Real m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
		Real m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
		Real m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
		Real m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		Real v0 = m20 * m31 - m21 * m30;
		Real v1 = m20 * m32 - m22 * m30;
		Real v2 = m20 * m33 - m23 * m30;
		Real v3 = m21 * m32 - m22 * m31;
		Real v4 = m21 * m33 - m23 * m31;
		Real v5 = m22 * m33 - m23 * m32;

		Real t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
		Real t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		Real t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
		Real t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

		Real invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		Real d00 = t00 * invDet;
		Real d10 = t10 * invDet;
		Real d20 = t20 * invDet;
		Real d30 = t30 * invDet;

		Real d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Real d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Real d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Real d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		Real d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Real d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Real d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Real d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		Real d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Real d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Real d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Real d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix4(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}
	//-----------------------------------------------------------------------
	Matrix4 Matrix4::inverseAffine(void) const
	{
		assert(isAffine());

		Real m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
		Real m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

		Real t00 = m22 * m11 - m21 * m12;
		Real t10 = m20 * m12 - m22 * m10;
		Real t20 = m21 * m10 - m20 * m11;

		Real m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];

		Real invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

		t00 *= invDet; t10 *= invDet; t20 *= invDet;

		m00 *= invDet; m01 *= invDet; m02 *= invDet;

		Real r00 = t00;
		Real r01 = m02 * m21 - m01 * m22;
		Real r02 = m01 * m12 - m02 * m11;

		Real r10 = t10;
		Real r11 = m00 * m22 - m02 * m20;
		Real r12 = m02 * m10 - m00 * m12;

		Real r20 = t20;
		Real r21 = m01 * m20 - m00 * m21;
		Real r22 = m00 * m11 - m01 * m10;

		Real m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];

		Real r03 = -(r00 * m03 + r01 * m13 + r02 * m23);
		Real r13 = -(r10 * m03 + r11 * m13 + r12 * m23);
		Real r23 = -(r20 * m03 + r21 * m13 + r22 * m23);

		return Matrix4(
			r00, r01, r02, r03,
			r10, r11, r12, r13,
			r20, r21, r22, r23,
			0, 0, 0, 1);
	}
	//-----------------------------------------------------------------------
	Vector3 Matrix4::transformAffine(const Vector3& v) const
	{
		Matrix4  mat = *this;
		//assert(!isAffine());
		if (RowMajor == true)
		{
			mat.transpose();
		}
		return Vector3(
			mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3],
			mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3],
			mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3]);
	}
	//-----------------------------------------------------------------------
	Vector4 Matrix4::transformAffine(const Vector4& v) const
	{
		Matrix4  mat = *this;
		//assert(!isAffine());
		if (RowMajor == true)
		{
			mat.transpose();
		}
		return Vector4(
			mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * v.w,
			mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * v.w,
			mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * v.w,
			v.w);
	}
	//-----------------------------------------------------------------------

	Matrix4& Matrix4::buildTextureTransform(Real rotateRad,
		const Vector2 &rotatecenter,
		const Vector2 &translate,
		const Vector2 &scale)
	{
		const Real c = cosf(rotateRad);
		const Real s = sinf(rotateRad);
		if (RowMajor == false)
		{
			transpose();
		}
		getIndex(0) = (Real)(c * scale.x);
		getIndex(1) = (Real)(s * scale.y);
		getIndex(2) = 0;
		getIndex(3) = 0;

		getIndex(4) = (Real)(-s * scale.x);
		getIndex(5) = (Real)(c * scale.y);
		getIndex(6) = 0;
		getIndex(7) = 0;

		getIndex(8) = (Real)(c * scale.x * rotatecenter.x + -s * rotatecenter.y + translate.x);
		getIndex(9) = (Real)(s * scale.y * rotatecenter.x + c * rotatecenter.y + translate.y);
		getIndex(10) = 1;
		getIndex(11) = 0;

		getIndex(12) = 0;
		getIndex(13) = 0;
		getIndex(14) = 0;
		getIndex(15) = 1;

		transpose();
#if defined ( USE_MATRIX_TEST )
		definitelyIdentityMatrix = false;
#endif
		return *this;
	}

	inline bool Matrix4::getInverse(Matrix4& out) const
	{
		/// Calculates the inverse of this Matrix
		/// The inverse is calculated using Cramers rule.
		/// If no inverse exists then 'false' is returned.

#if defined ( USE_MATRIX_TEST )
		if (this->isIdentity())
		{
			out = *this;
			return true;
		}
#endif
		Matrix4 m = *this;
		if (m.RowMajor == false)
		{
			m.transpose();
		}
		
		Real d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
			(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
			(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
			(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

		if (Math::iszero(d, FLT_MIN))
			return false;

		d = reciprocal(d);

		out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
			m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
			m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
		out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
			m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
			m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
		out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
			m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
			m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
		out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
			m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
			m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
		out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
			m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
			m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
		out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
			m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
			m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
		out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
			m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
			m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
		out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
			m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
			m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
		out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
			m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
		out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
			m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
			m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
		out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
			m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
			m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
		out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
			m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
			m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
		out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
			m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
		out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
			m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
			m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
		out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
			m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
			m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
		out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
			m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
			m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));

		out.transpose();

#if defined ( USE_MATRIX_TEST )
		out.definitelyIdentityMatrix = definitelyIdentityMatrix;
#endif
		return true;
	}

	void Matrix4::makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
	{
		// 顺序
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		Matrix3 rot3x3;
		orientation.ToRotationMatrix(rot3x3);

		if (RowMajor == true)
		{
			transpose();
		}
		 
		m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
		m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
		m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

		// 没有投影项
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	//-----------------------------------------------------------------------
	void Matrix4::makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
	{
		// 反转这些参数
		Vector3 invTranslate = -position;
		Vector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
		Quaternion invRot = orientation.Inverse();

		// 由于我们反转了，平移，旋转，缩放的顺序
		// 所以使平移相对于缩放和旋转
		invTranslate = invRot * invTranslate;  
		invTranslate *= invScale; 

		//构建3X3矩阵
		Matrix3 rot3x3;
		invRot.ToRotationMatrix(rot3x3);

		if (RowMajor == true)
		{
			transpose();
		}

		//  设置最终的缩放，旋转和平移矩阵
		m[0][0] = invScale.x * rot3x3[0][0]; m[0][1] = invScale.x * rot3x3[0][1]; m[0][2] = invScale.x * rot3x3[0][2]; m[0][3] = invTranslate.x;
		m[1][0] = invScale.y * rot3x3[1][0]; m[1][1] = invScale.y * rot3x3[1][1]; m[1][2] = invScale.y * rot3x3[1][2]; m[1][3] = invTranslate.y;
		m[2][0] = invScale.z * rot3x3[2][0]; m[2][1] = invScale.z * rot3x3[2][1]; m[2][2] = invScale.z * rot3x3[2][2]; m[2][3] = invTranslate.z;

		 
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	//-----------------------------------------------------------------------
	void Matrix4::decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const
	{
		assert(isAffine());

		Matrix3 m3x3;
		extract3x3Matrix(m3x3);

		Matrix3 matQ;
		Vector3 vecU;
		m3x3.QDUDecomposition(matQ, scale, vecU);

		orientation = Quaternion(matQ);

		if (RowMajor == false)
		{
			position = Vector3(m[0][3], m[1][3], m[2][3]);
		}
		else
		{
			position = Vector3(m[3][0], m[3][1], m[3][2]);
		}
		
	}

}