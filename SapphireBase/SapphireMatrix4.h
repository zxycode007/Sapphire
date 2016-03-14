#ifndef _SAPPHIRE_MATRIX4_
#define _SAPPHIRE_MATRIX4_

#include "SapphirePrerequisites.h"
#include "SapphireMatrix3.h"
#include "SapphireVector4.h"
#include "SapphirePlane.h"
#include "SapphireMath.h"

namespace Sapphire
{
	/** 
	��װ�˱�׼��4X4��ξ�����
	@remarks
	SAPPHIRE�ھ���˷�ʱʹ��������������ζ��һ������������Ϊһ�����У�4�о���
	��Ӱ�쵽����任˳�����ɴ��ҵ����������V���������M1��M2��M3�任����ô����ΪM3*M2*M1*V
	ע�����˷����ܽ���˳��������õ���ͬ�Ľ��,�Ǵ����
	@par
	�����������ҵ���˳��ͬ�����OPenglһ����Ȼ�����Direct3D�෴�����������������Ҵ����ҵľ���˷�
	@par
	SAPPHIRE����D3D��OpenGL֮��Ĳ�ͬ�����
	���ɲ�ͬ����Ⱦϵͳ�ڲ�����ʱ��SAPPHIREֻʹ�ñ�׼����ѧת�������ҵ���ľ���˷�
	��SAPPHIREת�����󣬴��ݸ�D3Dȥ������
	@par
	j����M * V ����չʾ�˾�����ĿԪ�ز�������
	<pre>
	[ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
	| m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
	| m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
	[ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
	</pre>
	*/
	class _SapphireExport Matrix4
	{
	protected:
		/// �����Ԫ�أ����� [��][��].
		union {
			Real m[4][4];
			Real _m[16];
		};
	public:
	 
		inline Matrix4()
		{
		}

		inline Matrix4(
			Real m00, Real m01, Real m02, Real m03,
			Real m10, Real m11, Real m12, Real m13,
			Real m20, Real m21, Real m22, Real m23,
			Real m30, Real m31, Real m32, Real m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;
			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		 

		inline Matrix4(const Matrix3& m3x3)
		{
			operator=(IDENTITY);
			operator=(m3x3);
		}

		 

		inline Matrix4(const Quaternion& rot)
		{
			Matrix3 m3x3;
			rot.ToRotationMatrix(m3x3);
			operator=(IDENTITY);
			operator=(m3x3);
		}

		~Matrix4()
		{
			 
		};

		 
		inline void swap(Matrix4& other)
		{
			std::swap(m[0][0], other.m[0][0]);
			std::swap(m[0][1], other.m[0][1]);
			std::swap(m[0][2], other.m[0][2]);
			std::swap(m[0][3], other.m[0][3]);
			std::swap(m[1][0], other.m[1][0]);
			std::swap(m[1][1], other.m[1][1]);
			std::swap(m[1][2], other.m[1][2]);
			std::swap(m[1][3], other.m[1][3]);
			std::swap(m[2][0], other.m[2][0]);
			std::swap(m[2][1], other.m[2][1]);
			std::swap(m[2][2], other.m[2][2]);
			std::swap(m[2][3], other.m[2][3]);
			std::swap(m[3][0], other.m[3][0]);
			std::swap(m[3][1], other.m[3][1]);
			std::swap(m[3][2], other.m[3][2]);
			std::swap(m[3][3], other.m[3][3]);
		}

		

		inline Real& getIndex(SINT32 iIndex)
		{
		assert(iIndex < 16);
			return _m[iIndex];
		}

		inline Real& operator () (size_t iRow, size_t iColumn)
		{
			assert(iRow < 4);
			assert(iColumn < 4);
			return m[iColumn][iRow];
		}
		inline Real* operator [] (size_t iRow)
		{
			assert(iRow < 4);
			return m[iRow];
		}
		inline const Real *operator [] (size_t iRow) const
		{
			assert(iRow < 4);
			return m[iRow];
		}

		inline Matrix4 concatenate(const Matrix4 &m2) const
		{
			Matrix4 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
		}

		 
		inline Matrix4 operator * (const Matrix4 &m2) const
		{
			return concatenate(m2);
		}

		 
		inline Vector3 operator * (const Vector3 &v) const
		{
			Vector3 r;

			Real fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);

			r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
			r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
			r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;

			return r;
		}
		inline Vector4 operator * (const Vector4& v) const
		{
			return Vector4(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
				);
		}
		inline Plane operator * (const Plane& p) const
		{
			Plane ret;
			Matrix4 invTrans = inverse().transpose();
			Vector4 v4(p.normal.x, p.normal.y, p.normal.z, p.d);
			v4 = invTrans * v4;
			ret.normal.x = v4.x;
			ret.normal.y = v4.y;
			ret.normal.z = v4.z;
			ret.d = v4.w / ret.normal.normalise();

			return ret;
		}

 
		inline Matrix4 operator + (const Matrix4 &m2) const
		{
			Matrix4 r;

			r.m[0][0] = m[0][0] + m2.m[0][0];
			r.m[0][1] = m[0][1] + m2.m[0][1];
			r.m[0][2] = m[0][2] + m2.m[0][2];
			r.m[0][3] = m[0][3] + m2.m[0][3];

			r.m[1][0] = m[1][0] + m2.m[1][0];
			r.m[1][1] = m[1][1] + m2.m[1][1];
			r.m[1][2] = m[1][2] + m2.m[1][2];
			r.m[1][3] = m[1][3] + m2.m[1][3];

			r.m[2][0] = m[2][0] + m2.m[2][0];
			r.m[2][1] = m[2][1] + m2.m[2][1];
			r.m[2][2] = m[2][2] + m2.m[2][2];
			r.m[2][3] = m[2][3] + m2.m[2][3];

			r.m[3][0] = m[3][0] + m2.m[3][0];
			r.m[3][1] = m[3][1] + m2.m[3][1];
			r.m[3][2] = m[3][2] + m2.m[3][2];
			r.m[3][3] = m[3][3] + m2.m[3][3];

			return r;
		}

	 
		inline Matrix4 operator - (const Matrix4 &m2) const
		{
			Matrix4 r;
			r.m[0][0] = m[0][0] - m2.m[0][0];
			r.m[0][1] = m[0][1] - m2.m[0][1];
			r.m[0][2] = m[0][2] - m2.m[0][2];
			r.m[0][3] = m[0][3] - m2.m[0][3];

			r.m[1][0] = m[1][0] - m2.m[1][0];
			r.m[1][1] = m[1][1] - m2.m[1][1];
			r.m[1][2] = m[1][2] - m2.m[1][2];
			r.m[1][3] = m[1][3] - m2.m[1][3];

			r.m[2][0] = m[2][0] - m2.m[2][0];
			r.m[2][1] = m[2][1] - m2.m[2][1];
			r.m[2][2] = m[2][2] - m2.m[2][2];
			r.m[2][3] = m[2][3] - m2.m[2][3];

			r.m[3][0] = m[3][0] - m2.m[3][0];
			r.m[3][1] = m[3][1] - m2.m[3][1];
			r.m[3][2] = m[3][2] - m2.m[3][2];
			r.m[3][3] = m[3][3] - m2.m[3][3];

			return r;
		}

		 
		inline bool operator == (const Matrix4& m2) const
		{
			if (
				m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
				m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3])
				return false;
			return true;
		}

	 
		inline bool operator != (const Matrix4& m2) const
		{
			if (
				m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
				m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3])
				return true;
			return false;
		}

		 
		inline void operator = (const Matrix3& mat3)
		{
			m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
			m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
			m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
		}

		inline Real* pointer()
		{
			return this->_m;
		}
		//����ת�þ��� ���������������Ⱦ���ת�ú�ɳ������Ⱦ���
		inline Matrix4 transpose(void) const
		{
			return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		/*
		-----------------------------------------------------------------------
		�任
		-----------------------------------------------------------------------
		*/
		/** ��һ��ƽ���������þ���ƽ�Ʊ任
		*/
		inline void setTrans(const Vector3& v)
		{
			m[0][3] = v.x;
			m[1][3] = v.y;
			m[2][3] = v.z;
		}

		/** 
		��ȡ�����ƽ�Ʊ任
		*/
		inline Vector3 getTrans() const
		{
			return Vector3(m[0][3], m[1][3], m[2][3]);
		}


		/** 
		����ƽ�ƾ���
		*/
		inline void makeTrans(const Vector3& v)
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		inline void makeTrans(Real tx, Real ty, Real tz)
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		/**
		���ƽ�ƾ���
		*/
		inline static Matrix4 getTrans(const Vector3& v)
		{
			Matrix4 r;

			r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
			r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}
 
		inline static Matrix4 getTrans(Real t_x, Real t_y, Real t_z)
		{
			Matrix4 r;

			r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = t_x;
			r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = t_y;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = t_z;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		 
		inline void setScale(const Vector3& v)
		{
			m[0][0] = v.x;
			m[1][1] = v.y;
			m[2][2] = v.z;
		}

		inline Vector3 getScale()
		{
			return Vector3(m[0][0], m[1][1], m[2][2]);
		}

		 
		inline static Matrix4 getScale(const Vector3& v)
		{
			Matrix4 r;
			r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}
 

		 
		inline static Matrix4 getScale(Real s_x, Real s_y, Real s_z)
		{
			Matrix4 r;
			r.m[0][0] = s_x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = s_y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = s_z; r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		//! Returns a rotation that is equivalent to that set by setRotationDegrees().
		/** This code was sent in by Chev.  Note that it does not necessarily return
		the *same* Euler angles as those set by setRotationDegrees(), but the rotation will
		be equivalent, i.e. will have the same result when used to rotate a vector or node. */
		 
		inline Vector3 getRotationDegrees() 
		{
			Matrix4 mat = *this;
			mat = mat.transpose();
			Vector3 scale = this->getScale();
			// we need to check for negative scale on to axes, which would bring up wrong results
			if (scale.y<0 && scale.z<0)
			{
				scale.y = -scale.y;
				scale.z = -scale.z;
			}
			else if (scale.x<0 && scale.z<0)
			{
				scale.x = -scale.x;
				scale.z = -scale.z;
			}
			else if (scale.x<0 && scale.y<0)
			{
				scale.x = -scale.x;
				scale.y = -scale.y;
			}
			const Vector3 invScale(reciprocal(scale.x), reciprocal(scale.y), reciprocal(scale.z));

			Real Y = -asin(Math::Clamp(mat.getIndex(2) * invScale.x, -1.0, 1.0));
			const Real C = cos(Y);
			//Y *= RADTODEG64;

			Real rotx, roty, X, Z;

			if (!Math::iszero(C))
			{
				const Real invC = reciprocal(C);
				rotx = mat.getIndex(10) * invC * invScale.z;
				roty = mat.getIndex(6) * invC * invScale.y;
				X = atan2(roty, rotx) * RADTODEG64;
				rotx = mat.getIndex(0) * invC * invScale.x;
				roty = mat.getIndex(1) * invC * invScale.x;
				Z = atan2(roty, rotx) * RADTODEG64;
			}
			else
			{
				X = 0.0;
				rotx = mat.getIndex(5) * invScale.y;
				roty = -mat.getIndex(4) * invScale.y;
				Z = atan2(roty, rotx) * RADTODEG64;
			}

			// fix values that get below zero
			if (X < 0.0) X += 2 * PI;
			if (Y < 0.0) Y += 2 * PI;
			if (Z < 0.0) Z += 2 * PI;

			return Vector3(X, Y, Z);
		}

		inline void extract3x3Matrix(Matrix3& m3x3) const
		{
			m3x3.m[0][0] = m[0][0];
			m3x3.m[0][1] = m[0][1];
			m3x3.m[0][2] = m[0][2];
			m3x3.m[1][0] = m[1][0];
			m3x3.m[1][1] = m[1][1];
			m3x3.m[1][2] = m[1][2];
			m3x3.m[2][0] = m[2][0];
			m3x3.m[2][1] = m[2][1];
			m3x3.m[2][2] = m[2][2];

		}

	 
		inline bool hasScale() const
		{
			 
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


		 
		inline bool hasNegativeScale() const
		{
			return determinant() < 0;
		}
 
		inline Quaternion extractQuaternion() const
		{
			Matrix3 m3x3;
			extract3x3Matrix(m3x3);
			return Quaternion(m3x3);
		}

		inline Matrix4&  setRotationRadians(const Vector3& rotation, bool useTable=false)
		{
			*this = transpose();

			const Real cr = Math::Cos(rotation.x,useTable);
			const Real sr = Math::Sin(rotation.x, useTable);
			const Real cp = Math::Cos(rotation.y, useTable);
			const Real sp = Math::Sin(rotation.y, useTable);
			const Real cy = Math::Cos(rotation.z, useTable);
			const Real sy = Math::Sin(rotation.z, useTable);

			getIndex(0) =  (cp*cy);
			getIndex(1) = (cp*sy);
			getIndex(2) = (-sp);

			const Real srsp = sr*sp;
			const Real crsp = cr*sp;

			getIndex(4) = (srsp*cy - cr*sy);
			getIndex(5) = (srsp*sy + cr*cy);
			getIndex(6) = (sr*cp);

			getIndex(8) = (crsp*cy + sr*sy);
			getIndex(9) = (crsp*sy - sr*cy);
			getIndex(10) = (cr*cp);

			*this = transpose();

			return *this;
		}

		inline Matrix4&  setRotationDegrees(const Vector3& rotation, bool useTable = false)
		{
			//��ת��Ϊ�о���
			*this = transpose();

			const Real cr = Math::Cos(Math::DegreesToRadians(rotation.x), useTable); //roll ��Z����ת�Ƕȵ�cosֵ
			const Real sr = Math::Sin(Math::DegreesToRadians(rotation.x), useTable); // roll��Z����ת�Ƕȵ�sinֵ
			const Real cp = Math::Cos(Math::DegreesToRadians(rotation.y), useTable); // pitch  ��x����ת�Ƕȵ�cosֵ
			const Real sp = Math::Sin(Math::DegreesToRadians(rotation.y), useTable); // pitch  ��y����ת�Ƕȵ�sinֵ
			const Real cy = Math::Cos(Math::DegreesToRadians(rotation.z), useTable); // yaw ��z����ת��cosֵ
			const Real sy = Math::Sin(Math::DegreesToRadians(rotation.z), useTable); //yaw ��z����ת��sinֵ

			getIndex(0) = (cp*cy);
			getIndex(1) = (cp*sy);
			getIndex(2) = (-sp);

			const Real srsp = sr*sp;
			const Real crsp = cr*sp;

			getIndex(4) = (srsp*cy - cr*sy);
			getIndex(5) = (srsp*sy + cr*cy);
			getIndex(6) = (sr*cp);

			getIndex(8) = (crsp*cy + sr*sy);
			getIndex(9) = (crsp*sy - sr*cy);
			getIndex(10) = (cr*cp);
			//Ȼ��ת�����о���
			*this = transpose();

			return *this;
		}

		// Builds a left-handed perspective projection matrix based on a field of view
		inline Matrix4& buildProjectionMatrixPerspectiveFovLH(
			Real fieldOfViewRadians, Real aspectRatio, Real zNear, Real zFar)
		{
			const Real h = reciprocal(tan(fieldOfViewRadians*0.5));
			assert(aspectRatio == 0.f); //divide by zero
			const Real w = static_cast<Real>(h / aspectRatio);

			assert(zNear == zFar); //divide by zero
			*this = transpose();
			getIndex(0) = w;
			getIndex(1) = 0;
			getIndex(2) = 0;
			getIndex(3) = 0;

			getIndex(4) = 0;
			getIndex(5) = (Real)h;
			getIndex(6) = 0;
			getIndex(7) = 0;

			getIndex(8) = 0;
			getIndex(9) = 0;
			getIndex(10) = (Real)(zFar / (zFar - zNear));
			getIndex(11) = 1;

			getIndex(12) = 0;
			getIndex(13) = 0;
			getIndex(14) = (Real)(-zNear*zFar / (zFar - zNear));
			getIndex(15) = 0;

			*this = transpose();


#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}


		// Builds a right-handed perspective projection matrix based on a field of view
		inline Matrix4& buildProjectionMatrixPerspectiveFovRH(
			Real fieldOfViewRadians, Real aspectRatio, Real zNear, Real zFar)
		{
			const Real h = reciprocal(tan(fieldOfViewRadians*0.5));
			assert(aspectRatio == 0.f); //divide by zero
			const Real w = static_cast<Real>(h / aspectRatio);

			assert(zNear == zFar); //divide by zero

			*this = transpose();
			getIndex(0) = w;
			getIndex(1) = 0;
			getIndex(2) = 0;
			getIndex(3) = 0;

			getIndex(4) = 0;
			getIndex(5) = (Real)h;
			getIndex(6) = 0;
			getIndex(7) = 0;

			getIndex(8) = 0;
			getIndex(9) = 0;
			getIndex(10) = (Real)(zFar / (zNear - zFar)); // DirectX version
			//		M[10] = (T)(zFar+zNear/(zNear-zFar)); // OpenGL version
			getIndex(11) = -1;

			getIndex(12) = 0;
			getIndex(13) = 0;
			getIndex(14) = (Real)(zNear*zFar / (zNear - zFar)); // DirectX version
			//		M[14] = (T)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
			getIndex(15) = 0;

			*this = transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}


		// Builds a left-handed look-at matrix.
		inline Matrix4& buildCameraLookAtMatrixLH(
			const Vector3& position,
			const Vector3& target,
			const Vector3& upVector)
		{
			Vector3 zaxis = target - position;
			zaxis.normalize();

			Vector3 xaxis = upVector.crossProduct(zaxis);
			xaxis.normalize();

			Vector3 yaxis = zaxis.crossProduct(xaxis);
			//���㷽�������о��󣬶�Matrix4���о���洢�ģ���ת��
			*this = transpose(); 
			getIndex(0) = (Real)xaxis.x;
			getIndex(1) = (Real)yaxis.x;
			getIndex(2) = (Real)zaxis.x;
			getIndex(3) = 0;

			getIndex(4) = (Real)xaxis.y;
			getIndex(5) = (Real)yaxis.y;
			getIndex(6) = (Real)zaxis.y;
			getIndex(7) = 0;

			getIndex(8) = (Real)xaxis.z;
			getIndex(9) = (Real)yaxis.z;
			getIndex(10) = (Real)zaxis.z;
			getIndex(11) = 0;

			getIndex(12) = (Real)-xaxis.dotProduct(position);
			getIndex(13) = (Real)-yaxis.dotProduct(position);
			getIndex(14) = (Real)-zaxis.dotProduct(position);
			getIndex(15) = 1;
			//���㷽�������о��󣬶�Matrix4���о���洢�ģ���ת�û��о���
			*this = transpose(); 
#if defined ( USE_MARealRIX_RealESReal )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}

		inline Matrix4 setbyProduct(const Matrix4& other_a, const Matrix4& other_b)
		{
			
		    Matrix4 m1 = other_a;
			Matrix4 m2 = other_b;
			m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
			m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
			m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
			m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

			m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
			m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
			m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
			m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

			m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
			m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
			m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
			m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

			m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
			m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
			m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
			m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
			return *this;
		}

		//! multiply by another matrix
		// set this matrix to the product of two other matrices
		// goal is to reduce stack use and copy
		inline Matrix4 setbyproduct_nocheck(const Matrix4& other_a, const Matrix4& other_b)
		{
			 Matrix4 m1 = other_a;
			 Matrix4 m2 = other_b;
			 //�о���ת���о���
			 m1.transpose();
			 m2.transpose();

			 getIndex(0) = m1.getIndex(0) * m2.getIndex(0) + m1.getIndex(4) * m2.getIndex(1) + m1.getIndex(8) * m2.getIndex(2) + m1.getIndex(12) * m2.getIndex(3);
			 getIndex(1) = m1.getIndex(1) * m2.getIndex(0) + m1.getIndex(5) * m2.getIndex(1) + m1.getIndex(9) * m2.getIndex(2) + m1.getIndex(13) * m2.getIndex(3);
			 getIndex(2) = m1.getIndex(2) * m2.getIndex(0) + m1.getIndex(6) * m2.getIndex(1) + m1.getIndex(10) * m2.getIndex(2) + m1.getIndex(14) * m2.getIndex(3);
			 getIndex(3) = m1.getIndex(3) * m2.getIndex(0) + m1.getIndex(7) * m2.getIndex(1) + m1.getIndex(11) * m2.getIndex(2) + m1.getIndex(15) * m2.getIndex(3);

			 getIndex(4) = m1.getIndex(0) * m2.getIndex(4) + m1.getIndex(4) * m2.getIndex(5) + m1.getIndex(8) * m2.getIndex(6) + m1.getIndex(12) * m2.getIndex(7);
			 getIndex(5) = m1.getIndex(1) * m2.getIndex(4) + m1.getIndex(5) * m2.getIndex(5) + m1.getIndex(9) * m2.getIndex(6) + m1.getIndex(13) * m2.getIndex(7);
			 getIndex(6) = m1.getIndex(2) * m2.getIndex(4) + m1.getIndex(6) * m2.getIndex(5) + m1.getIndex(10) * m2.getIndex(6) + m1.getIndex(14) * m2.getIndex(7);
			 getIndex(7) = m1.getIndex(3) * m2.getIndex(4) + m1.getIndex(7) * m2.getIndex(5) + m1.getIndex(11) * m2.getIndex(6) + m1.getIndex(15) * m2.getIndex(7);

			 getIndex(8) = m1.getIndex(0) * m2.getIndex(8) + m1.getIndex(4) * m2.getIndex(9) + m1.getIndex(8) * m2.getIndex(10) + m1.getIndex(12) * m2.getIndex(11);
			 getIndex(9) = m1.getIndex(1) * m2.getIndex(8) + m1.getIndex(5) * m2.getIndex(9) + m1.getIndex(9) * m2.getIndex(10) + m1.getIndex(13) * m2.getIndex(11);
			 getIndex(10) = m1.getIndex(2) * m2.getIndex(8) + m1.getIndex(6) * m2.getIndex(9) + m1.getIndex(10) * m2.getIndex(10) + m1.getIndex(14) * m2.getIndex(11);
			 getIndex(11) = m1.getIndex(3) * m2.getIndex(8) + m1.getIndex(7) * m2.getIndex(9) + m1.getIndex(11) * m2.getIndex(10) + m1.getIndex(15) * m2.getIndex(11);

			 getIndex(12) = m1.getIndex(0) * m2.getIndex(12) + m1.getIndex(4) * m2.getIndex(13) + m1.getIndex(8) * m2.getIndex(14) + m1.getIndex(12) * m2.getIndex(15);
			 getIndex(13) = m1.getIndex(1) * m2.getIndex(12) + m1.getIndex(5) * m2.getIndex(13) + m1.getIndex(9) * m2.getIndex(14) + m1.getIndex(13) * m2.getIndex(15);
			 getIndex(14) = m1.getIndex(2) * m2.getIndex(12) + m1.getIndex(6) * m2.getIndex(13) + m1.getIndex(10) * m2.getIndex(14) + m1.getIndex(14) * m2.getIndex(15);
			 getIndex(15) = m1.getIndex(3) * m2.getIndex(12) + m1.getIndex(7) * m2.getIndex(13) + m1.getIndex(11) * m2.getIndex(14) + m1.getIndex(15) * m2.getIndex(15);
			 //���о����������ת�����о��󣨼��㷽���ǰ��о���Matrix4�ǰ��о���洢�ģ�
			 *this = transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}

		//�����
		static const Matrix4 ZERO;    
		//��������
		static const Matrix4 ZEROAFFINE;
		//��λ����
		static const Matrix4 IDENTITY;
		/** ʹ2d���ÿռ�{-1, 1} �� {0,1}�ͷ�תY��С����
		  */
		static const Matrix4 CLIPSPACE2DTOIMAGESPACE;

		inline Matrix4 operator*(Real scalar) const
		{
			return Matrix4(
				scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
				scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
				scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
				scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
		}

		
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const Matrix4& mat)
		{
			o << "Matrix4(";
			for (size_t i = 0; i < 4; ++i)
			{
				o << " row" << (unsigned)i << "{";
				for (size_t j = 0; j < 4; ++j)
				{
					o << mat[i][j] << " ";
				}
				o << "}";
			}
			o << ")";
			return o;
		}

		Matrix4 adjoint() const;
		Real determinant() const;
		Matrix4 inverse() const;
		inline bool Matrix4::getInverse(Matrix4& out) const;

		 
		void makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

		 
		void makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

		 
		void decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const;

		/** 
		�����������Ƿ��Ƿ������
		@remarks
		һ�����������4X4������3�е���(0,0,0,1)����
		e.g.��ͶӰϵ��
		*/
		inline bool isAffine(void) const
		{
			return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
		}


		/**
		�����������Ƿ��Ƿ������
		@remarks
		һ�����������4X4������3�е���(0,0,0,1)����
		e.g.��ͶӰϵ��
		*/
		inline bool isIdentify(void) const
		{
			return m[0][0] == 1 && m[1][1] == 1 && m[2][2] == 1 && m[3][3] == 1;
		}

		 
		Matrix4 inverseAffine(void) const;

		/** ���������������
		@note
		�����������Ƿ������ 
		*/
		inline Matrix4 concatenateAffine(const Matrix4 &m2) const
		{
			assert(isAffine() && m2.isAffine());

			return Matrix4(
				m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0],
				m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1],
				m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2],
				m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3],

				m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0],
				m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1],
				m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2],
				m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3],

				m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0],
				m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1],
				m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2],
				m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3],

				0, 0, 0, 1);
		}

		/** ��һ���������任3d����
		@remarks
		ͨ������ת��������3D������ͶӰ������w=1

		@note
		�����������Ƿ������
		*/
		inline Vector3 transformAffine(const Vector3& v) const
		{
			assert(isAffine());

			return Vector3(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
		}

		 
		inline Vector4 transformAffine(const Vector4& v) const
		{
			assert(isAffine());

			return Vector4(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				v.w);
		}

		inline Vector3 rotateVect(const Vector3& v) const
		{
			assert(isAffine());

			return Vector3(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
		}
		
		inline void inverseRotateVect(Vector3& vect) const
		{
			Vector3 tmp;
			//vect.x = m[0][0] * vect.x + m[0][1] * vect.y + m[0][2] * vect.z + m[0][3];
			//vect.y = m[1][0] * vect.x + m[1][1] * vect.y + m[1][2] * vect.z + m[1][3];
			//vect.z = m[2][0] * vect.x + m[2][1] * vect.y + m[2][2] * vect.z + m[2][3];
			tmp.x = m[0][0] * vect.x + m[1][0] * vect.y + m[2][0] * vect.z;
			tmp.y = m[0][1] * vect.x + m[1][1] * vect.y + m[2][1] * vect.z;
			tmp.z = m[0][2] * vect.x + m[1][2] * vect.y + m[2][2] * vect.z;

			vect.x = tmp.x;
			vect.y = tmp.y;
			vect.z = tmp.z;
		
		}
	};

	 
	inline Vector4 operator * (const Vector4& v, const Matrix4& mat)
	{
		return Vector4(
			v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
			v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
			v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
			v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
			);
	}
	 
	

}

#endif