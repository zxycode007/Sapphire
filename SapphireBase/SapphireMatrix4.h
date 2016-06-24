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
	封装了标准的4X4齐次矩阵类
	@remarks
	SAPPHIRE在矩阵乘法时使用列向量，这意味着一个向量被表述为一单个列，4行矩阵。
	这影响到矩阵变换顺序是由从右到左。如果向量V由这个矩阵M1，M2，M3变换，那么计算为M3*M2*M1*V
	注意矩阵乘法不能交换顺序，那样会得到不同的结果,是错误的
	@par
	列向量按从右到左顺序，同样这和OPengl一样。然而这和Direct3D相反，它是用行向量并且从左到右的矩阵乘法
	@par
	SAPPHIRE处理D3D和OpenGL之间的不同情况。
	当由不同的渲染系统内部操作时，SAPPHIRE只使用标准的数学转换。从右到左的矩阵乘法
	（SAPPHIRE转换矩阵，传递给D3D去补偿）
	@par
	j矩阵M * V 它们展示了矩阵项目元素布局如下
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
		/// 矩阵的元素，索引 [行][列].
		union {
			Real m[4][4];
			Real _m[16];
		};
	public:

		bool RowMajor;

		inline Matrix4()
		{
			*this = Matrix4::IDENTITY;
			RowMajor = false;
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
			RowMajor = false;
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


		/**
		@brief 获取矩阵第i个元素
		@param i: 元素在矩阵的序号
		@return 第i个元素
		*/
		inline  Real& getIndex(SINT32 iIndex)
		{
			assert(iIndex < 16);
			return _m[iIndex];
		}
		/**
		@brief 获取矩阵第i个元素   (常量)
		@param i: 元素在矩阵的序号
		@return 第i个元素
		*/
		inline const Real& getCIndex(SINT32 iIndex) const
		{
			assert(iIndex < 16);
			return _m[iIndex];
		}
		//获取第iRow行iColumn列的元素
		inline Real& operator () (size_t iRow, size_t iColumn)
		{
			assert(iRow < 4);
			assert(iColumn < 4);
			return m[iRow][iColumn];  
		}
		/*
		@brief 获取第iRow行数组
		@param iRow  第iRow行数
		Return 第iRow行数组
		*/
		inline Real* operator [] (size_t iRow)
		{
			assert(iRow < 4);
			return m[iRow];
		}
		/*
		@brief 获取第iRow行数组   (常量)
		@param iRow  第iRow行数
		Return 第iRow行数组
		*/
		inline const Real *operator [] (size_t iRow) const
		{
			assert(iRow < 4);
			return m[iRow];
		}

		//
		/**
		@brief  按列矩阵连接进行计算
		@param m2 另外一个矩阵
		*/
		inline Matrix4 concatenate(const Matrix4 &m2) const
		{

			Matrix4 mat = *this;
			Matrix4 mat2 = m2;
			//如果是行优先转置成列优先矩阵
			if (mat.RowMajor == true)
			{
				mat.transpose();
			}
			if (mat2.RowMajor == true)
			{
				mat2.transpose();
			}
			Matrix4 r;
			r.m[0][0] = mat.m[0][0] * mat2.m[0][0] + mat.m[0][1] * mat2.m[1][0] + mat.m[0][2] * mat2.m[2][0] + mat.m[0][3] * mat2.m[3][0];
			r.m[0][1] = mat.m[0][0] * mat2.m[0][1] + mat.m[0][1] * mat2.m[1][1] + mat.m[0][2] * mat2.m[2][1] + mat.m[0][3] * mat2.m[3][1];
			r.m[0][2] = mat.m[0][0] * mat2.m[0][2] + mat.m[0][1] * mat2.m[1][2] + mat.m[0][2] * mat2.m[2][2] + mat.m[0][3] * mat2.m[3][2];
			r.m[0][3] = mat.m[0][0] * mat2.m[0][3] + mat.m[0][1] * mat2.m[1][3] + mat.m[0][2] * mat2.m[2][3] + mat.m[0][3] * mat2.m[3][3];

			r.m[1][0] = mat.m[1][0] * mat2.m[0][0] + mat.m[1][1] * mat2.m[1][0] + mat.m[1][2] * mat2.m[2][0] + mat.m[1][3] * mat2.m[3][0];
			r.m[1][1] = mat.m[1][0] * mat2.m[0][1] + mat.m[1][1] * mat2.m[1][1] + mat.m[1][2] * mat2.m[2][1] + mat.m[1][3] * mat2.m[3][1];
			r.m[1][2] = mat.m[1][0] * mat2.m[0][2] + mat.m[1][1] * mat2.m[1][2] + mat.m[1][2] * mat2.m[2][2] + mat.m[1][3] * mat2.m[3][2];
			r.m[1][3] = mat.m[1][0] * mat2.m[0][3] + mat.m[1][1] * mat2.m[1][3] + mat.m[1][2] * mat2.m[2][3] + mat.m[1][3] * mat2.m[3][3];

			r.m[2][0] = mat.m[2][0] * mat2.m[0][0] + mat.m[2][1] * mat2.m[1][0] + mat.m[2][2] * mat2.m[2][0] + mat.m[2][3] * mat2.m[3][0];
			r.m[2][1] = mat.m[2][0] * mat2.m[0][1] + mat.m[2][1] * mat2.m[1][1] + mat.m[2][2] * mat2.m[2][1] + mat.m[2][3] * mat2.m[3][1];
			r.m[2][2] = mat.m[2][0] * mat2.m[0][2] + mat.m[2][1] * mat2.m[1][2] + mat.m[2][2] * mat2.m[2][2] + mat.m[2][3] * mat2.m[3][2];
			r.m[2][3] = mat.m[2][0] * mat2.m[0][3] + mat.m[2][1] * mat2.m[1][3] + mat.m[2][2] * mat2.m[2][3] + mat.m[2][3] * mat2.m[3][3];

			r.m[3][0] = mat.m[3][0] * mat2.m[0][0] + mat.m[3][1] * mat2.m[1][0] + mat.m[3][2] * mat2.m[2][0] + mat.m[3][3] * mat2.m[3][0];
			r.m[3][1] = mat.m[3][0] * mat2.m[0][1] + mat.m[3][1] * mat2.m[1][1] + mat.m[3][2] * mat2.m[2][1] + mat.m[3][3] * mat2.m[3][1];
			r.m[3][2] = mat.m[3][0] * mat2.m[0][2] + mat.m[3][1] * mat2.m[1][2] + mat.m[3][2] * mat2.m[2][2] + mat.m[3][3] * mat2.m[3][2];
			r.m[3][3] = mat.m[3][0] * mat2.m[0][3] + mat.m[3][1] * mat2.m[1][3] + mat.m[3][2] * mat2.m[2][3] + mat.m[3][3] * mat2.m[3][3];

			r.transpose();

			return r;
		}


		inline Matrix4 operator * (const Matrix4 &m2) const
		{
			return concatenate(m2);
		}


		inline Vector3 operator * (const Vector3 &v) const
		{
			Vector3 r;
			//w的倒数
			Real fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);
			Matrix4 mat = *this;
			//如果是行优先转置成列优先矩阵
			if (mat.RowMajor == true)
			{
				mat.transpose();
			}
			{
				
				r.x = (mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3]) * fInvW;
				r.y = (mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3]) * fInvW;
				r.z = (mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3]) * fInvW;
			}

			return r;
		}
		inline Vector4 operator * (const Vector4& v) const
		{
			Matrix4 mat = *this;
			if (mat.RowMajor == true)
			{
				mat.transpose();
			}
			return Vector4(
				mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * v.w,
				mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * v.w,
				mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * v.w,
				mat.m[3][0] * v.x + mat.m[3][1] * v.y + mat.m[3][2] * v.z + mat.m[3][3] * v.w
				);
		}

		inline Plane operator * (const Plane& p) 
		{
			Plane ret;
			Matrix4 invTrans = inverse();
			invTrans.transpose();
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
			return _m;
		}
		const Real* cpointer() const
		{
			return _m;
		}
		//返回转置矩阵 （本引擎是列优先矩阵，转置后可成行优先矩阵）
		void transpose(void)
		{

			std::swap(m[0][1], m[1][0]);
			std::swap(m[0][2], m[2][0]);
			std::swap(m[0][3], m[3][0]);
			std::swap(m[1][2], m[2][1]);
			std::swap(m[1][3], m[3][1]);
			std::swap(m[2][3], m[3][2]);

			RowMajor = !RowMajor;

			//return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
			//	m[0][1], m[1][1], m[2][1], m[3][1],
			//	m[0][2], m[1][2], m[2][2], m[3][2],
			//	m[0][3], m[1][3], m[2][3], m[3][3]);
			;
		}

		/*
		-----------------------------------------------------------------------
		变换
		-----------------------------------------------------------------------
		*/
		/** 用一个平移向量设置矩阵平移变换
		*/
		inline void setTrans(const Vector3& v)
		{
			if (RowMajor == true)
			{
				transpose();
			}
			m[0][3] = v.x;
			m[1][3] = v.y;
			m[2][3] = v.z;
		}

		/**
		提取矩阵的平移变换
		*/
		inline Vector3 getTrans() const
		{
			if (RowMajor == true)
			{
				return Vector3(m[3][0], m[3][1], m[3][2]);
			}
			else{
				return Vector3(m[0][3], m[1][3], m[2][3]);
			}
			
		}


		/**
		构建平移矩阵
		*/
		inline void makeTrans(const Vector3& v)
		{
			if (RowMajor == true)
			{
				transpose();
			}
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		inline void makeTrans(Real tx, Real ty, Real tz)
		{
			if (RowMajor == true)
			{
				transpose();
			}
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		/**
		获得平移矩阵
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

		// 返回旋转度数向量
		/** 
        注意它不总是返回setRotationDegrees（）设置相同的欧拉角, 但这个rotation用于旋转一个向量和节点
		有着相同的结果
		*/

		inline Vector3 getRotationDegrees()
		{
			Matrix4 mat = *this;
			//该算法用行矩阵
			if (mat.RowMajor == false)
			{
				mat.transpose();
			}			
			Vector3 scale = this->getScale();
			//缩放值不能为负数
			if (scale.y < 0 && scale.z < 0)
			{
				scale.y = -scale.y;
				scale.z = -scale.z;
			}
			else if (scale.x < 0 && scale.z < 0)
			{
				scale.x = -scale.x;
				scale.z = -scale.z;
			}
			else if (scale.x < 0 && scale.y < 0)
			{
				scale.x = -scale.x;
				scale.y = -scale.y;
			}
			const Vector3 invScale(reciprocal(scale.x), reciprocal(scale.y), reciprocal(scale.z));

			Real Y = -asin(Math::Clamp<Real>(mat.getIndex(2) * invScale.x, -1.0, 1.0));
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

		Matrix4& buildTextureTransform(Real rotateRad,
			const Vector2 &rotatecenter,
			const Vector2 &translate,
			const Vector2 &scale);


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

		inline void setRotationCenter(const Vector3& center, const Vector3& translation)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			
			getIndex(12) = -getIndex(0) * center.x - getIndex(4) * center.y - getIndex(8) * center.z + (center.x - translation.x);
			getIndex(13) = -getIndex(1) * center.x - getIndex(5) * center.y - getIndex(9) * center.z + (center.y - translation.y);
			getIndex(14) = -getIndex(2) * center.x - getIndex(6) * center.y - getIndex(10) * center.z + (center.z - translation.z);
			getIndex(15) = (Real) 1.0;
			transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
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

		inline Matrix4&  setRotationRadians(const Vector3& rotation, bool useTable = false)
		{
			if (RowMajor == false)
			{
				transpose();
			}

			const Real cr = Math::Cos(rotation.x, useTable);
			const Real sr = Math::Sin(rotation.x, useTable);
			const Real cp = Math::Cos(rotation.y, useTable);
			const Real sp = Math::Sin(rotation.y, useTable);
			const Real cy = Math::Cos(rotation.z, useTable);
			const Real sy = Math::Sin(rotation.z, useTable);

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

			transpose();

			return *this;
		}


		//! Builds a matrix which rotates a source vector to a look vector over an arbitrary axis
		/** \param camPos: viewer position in world coord
		\param center: object position in world-coord, rotation pivot
		\param translation: object final translation from center
		\param axis: axis to rotate about
		\param from: source vector to rotate from
		*/
		inline void buildAxisAlignedBillboard(
			const Vector3& camPos,
			const Vector3& center,
			const Vector3& translation,
			const Vector3& axis,
			const Vector3& from)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			// axis of rotation
			Vector3 up = axis;
			up.normalize();
			const Vector3 forward = (camPos - center).normalize();
			const Vector3 right = up.crossProduct(forward).normalize();

			// correct look vector
			const Vector3 look = right.crossProduct(up);

			// rotate from to
			// axis multiplication by sin
			const Vector3 vs = look.crossProduct(from);

			// cosinus angle
			const FLOAT32 ca = from.dotProduct(look);

			Vector3 vt(up * (1.f - ca));

			getIndex(0) = static_cast<Real>(vt.x * up.x + ca);
			getIndex(5) = static_cast<Real>(vt.y * up.y + ca);
			getIndex(10) = static_cast<Real>(vt.z * up.z + ca);

			vt.x *= up.y;
			vt.z *= up.x;
			vt.y *= up.z;

			getIndex(1) = static_cast<Real>(vt.x - vs.z);
			getIndex(2) = static_cast<Real>(vt.z + vs.y);
			getIndex(3) = 0;

			getIndex(4) = static_cast<Real>(vt.x + vs.z);
			getIndex(6) = static_cast<Real>(vt.y - vs.x);
			getIndex(7) = 0;

			getIndex(8) = static_cast<Real>(vt.z - vs.y);
			getIndex(9) = static_cast<Real>(vt.y + vs.x);
			getIndex(11) = 0;


			transpose();

			setRotationCenter(center, translation);
		}

		inline Matrix4&  setRotationDegrees(const Vector3& rotation, bool useTable = false)
		{
			//先转置为行矩阵
			if (RowMajor == false)
			{
				transpose();
			}

			const Real cr = Math::Cos(Math::DegreesToRadians(rotation.x), useTable); //roll 绕Z轴旋转角度的cos值
			const Real sr = Math::Sin(Math::DegreesToRadians(rotation.x), useTable); // roll绕Z轴旋转角度的sin值
			const Real cp = Math::Cos(Math::DegreesToRadians(rotation.y), useTable); // pitch  绕x轴旋转角度的cos值
			const Real sp = Math::Sin(Math::DegreesToRadians(rotation.y), useTable); // pitch  绕y轴旋转角度的sin值
			const Real cy = Math::Cos(Math::DegreesToRadians(rotation.z), useTable); // yaw 绕z轴旋转的cos值
			const Real sy = Math::Sin(Math::DegreesToRadians(rotation.z), useTable); //yaw 绕z轴旋转的sin值

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
			//然后转换回列矩阵
			transpose();

			return *this;
		}



		inline Matrix4& Matrix4::setTextureTranslate(FLOAT32 x, FLOAT32 y)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			//M[8] = (Real)x;
			m[0][3] = (Real)x;
			m[1][3] = (Real)y;
			//M[9] = (Real)y;

			transpose();

#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = definitelyIdentityMatrix && (x == 0.0f) && (y == 0.0f);
#endif
			return *this;
		}



		inline Matrix4& setTextureScale(FLOAT32 sx, FLOAT32 sy)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			m[0][0] = (Real)sx;
			m[1][1] = (Real)sy;
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = definitelyIdentityMatrix && (sx == 1.0f) && (sy == 1.0f);
#endif
			transpose();
			return *this;
		}




		inline Matrix4& setTextureRotationCenter(FLOAT32 rotateRad)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			const FLOAT32 c = cosf(rotateRad);
			const FLOAT32 s = sinf(rotateRad);
			getIndex(0) = (Real)s;
			//m[0][0] = (Real)c;
			getIndex(1) = (Real)s;
			//m[1][0] = (Real)s;

			getIndex(4) = (Real)-s;
			//m[0][1] = (Real)-s;
			getIndex(5) = (Real)c;
			//m[1][1] = (Real)c;

			getIndex(8) = (Real)(0.5f * (s - c) + 0.5f);
			//m[0][2] = (Real)(0.5f * (s - c) + 0.5f);

			getIndex(9) = (Real)(-0.5f * (s + c) + 0.5f);
			//m[1][2] = (Real)(-0.5f * (s + c) + 0.5f);

#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = definitelyIdentityMatrix && (rotateRad == 0.0f);
#endif
			transpose();
			return *this;
		}


		//! Builds a matrix that rotates from one vector to another
		/** \param from: vector to rotate from
		\param to: vector to rotate to

		http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
		*/
		inline Matrix4& buildRotateFromTo(const Vector3& from, const Vector3& to)
		{
			// unit vectors
			Vector3 f(from);
			Vector3 t(to);
			f.normalize();
			t.normalize();

			// axis multiplication by sin
			Vector3 vs(t.crossProduct(f));

			// axis of rotation
			Vector3 v(vs);
			v.normalize();

			// cosinus angle
			Real ca = f.dotProduct(t);

			Vector3 vt(v * (1 - ca));
			if (RowMajor == false)
			{
				transpose();
			}
			getIndex(0) = vt.x * v.x + ca;
			getIndex(5) = vt.y * v.y + ca;
			getIndex(10) = vt.z * v.z + ca;

			vt.x *= v.y;
			vt.z *= v.x;
			vt.y *= v.z;

			getIndex(1) = vt.x - vs.z;
			getIndex(2) = vt.z + vs.y;
			getIndex(3) = 0;

			getIndex(4) = vt.x + vs.z;
			getIndex(6) = vt.y - vs.x;
			getIndex(7) = 0;

			getIndex(8) = vt.z - vs.y;
			getIndex(9) = vt.y + vs.x;
			getIndex(11) = 0;

			getIndex(12) = 0;
			getIndex(13) = 0;
			getIndex(14) = 0;
			getIndex(15) = 1;

			transpose();
			return *this;
		}



		inline Matrix4& buildProjectionMatrixOrthoLH(
			FLOAT32 widthOfViewVolume, FLOAT32 heightOfViewVolume, FLOAT32 zNear, FLOAT32 zFar)
		{
			assert(widthOfViewVolume != 0.f); //divide by zero
			assert(heightOfViewVolume != 0.f); //divide by zero
			assert(zNear != zFar); //divide by zero
			if (RowMajor == false)
			{
				transpose();
			}
			getIndex(0) = (Real)(2 / widthOfViewVolume);
			getIndex(1) = 0;
			getIndex(2) = 0;
			getIndex(3) = 0;

			getIndex(4) = 0;
			getIndex(5) = (Real)(2 / heightOfViewVolume);
			getIndex(6) = 0;
			getIndex(7) = 0;

			getIndex(8) = 0;
			getIndex(9) = 0;
			getIndex(10) = (Real)(1 / (zFar - zNear));
			getIndex(11) = 0;

			getIndex(12) = 0;
			getIndex(13) = 0;
			getIndex(14) = (Real)(zNear / (zNear - zFar));
			getIndex(15) = 1;
			transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}



		inline Matrix4& buildProjectionMatrixOrthoRH(
			FLOAT32 widthOfViewVolume, FLOAT32 heightOfViewVolume, FLOAT32 zNear, FLOAT32 zFar)
		{
			assert(widthOfViewVolume == 0.f); //divide by zero
			assert(heightOfViewVolume == 0.f); //divide by zero
			assert(zNear == zFar); //divide by zero
			if (RowMajor == false)
			{
				transpose();
			}
			getIndex(0) = (Real)(2 / widthOfViewVolume);
			getIndex(1) = 0;
			getIndex(2) = 0;
			getIndex(3) = 0;

			getIndex(4) = 0;
			getIndex(5) = (Real)(2 / heightOfViewVolume);
			getIndex(6) = 0;
			getIndex(7) = 0;

			getIndex(8) = 0;
			getIndex(9) = 0;
			getIndex(10) = (Real)(1 / (zNear - zFar));
			getIndex(11) = 0;

			getIndex(12) = 0;
			getIndex(13) = 0;
			getIndex(14) = (Real)(zNear / (zNear - zFar));
			getIndex(15) = 1;
			transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}

		// Builds a left-handed perspective projection matrix based on a field of view
		inline Matrix4& buildProjectionMatrixPerspectiveFovLH(
			Real fieldOfViewRadians, Real aspectRatio, Real zNear, Real zFar)
		{
			const Real h = reciprocal(tan(fieldOfViewRadians*0.5));
			assert(aspectRatio != 0.f); //divide by zero
			const Real w = static_cast<Real>(h / aspectRatio);

			assert(zNear != zFar); //divide by zero
			if (RowMajor == false)
			{
				transpose();
			}
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

			transpose();


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

			if (RowMajor == false)
			{
				transpose();
			}
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

			transpose();
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
			zaxis.normalise();

			Vector3 xaxis = upVector.crossProduct(zaxis);
			xaxis.normalise();

			Vector3 yaxis = zaxis.crossProduct(xaxis);
			//计算方法是以行矩阵，而Matrix4是列矩阵存储的，先转置
			if (RowMajor == false)
			{
				transpose();
			}
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
			//计算方法是以行矩阵，而Matrix4是列矩阵存储的，再转置回列矩阵
			 transpose(); 
#if defined ( USE_MARealRIX_RealESReal )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}



		inline Matrix4& setTextureScaleCenter(FLOAT32 sx, FLOAT32 sy)
		{
			if (RowMajor == false)
			{
				transpose();
			}
			getIndex(0) = (Real)sx;
			//m[0][0] = (Real)sx;
			getIndex(5) = (Real)sy;
			//m[1][1] = (Real)sy;
			getIndex(8) = (Real)(0.5f - 0.5f * sx);
		//	m[0][2] = (Real)(0.5f - 0.5f * sx);
			getIndex(9) = (Real)(0.5f - 0.5f * sy);
		//	m[1][2] = (Real)(0.5f - 0.5f * sy);

#if defined ( USE_MARealRIX_RealESReal )
			definitelyIdentityMatrix = definitelyIdentityMatrix && (sx == 1.0f) && (sy == 1.0f);
#endif
			transpose();
			return *this;
		}


		inline Matrix4& setM(const Real *data)
		{
			memcpy(_m, data, 16 * sizeof(Real));

#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}


		inline Matrix4 setbyProduct(const Matrix4& other_a, const Matrix4& other_b)
		{

			Matrix4 m1 = other_a;
			Matrix4 m2 = other_b;

			if (RowMajor == true)
			{
				transpose();
			}
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
			//列矩阵转成行矩阵
			Matrix4 m1 = other_a;
			Matrix4 m2 = other_b;

			if (m1.RowMajor == false)
			{
				m1.transpose();
			}
			if (m2.RowMajor == false)
			{
				m2.transpose();
			}


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

			RowMajor = true;
			//将行矩阵计算结果再转换会列矩阵（计算方法是按行矩阵，Matrix4是按列矩阵存储的）
			transpose();
#if defined ( USE_MATRIX_TEST )
			definitelyIdentityMatrix = false;
#endif
			return *this;
		}

		//零矩阵
		static const Matrix4 ZERO;
		//零仿射矩阵
		static const Matrix4 ZEROAFFINE;
		//单位矩阵
		static const Matrix4 IDENTITY;
		/** 使2d剪裁空间{-1, 1} 到 {0,1}和翻转Y的小矩阵
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
		Matrix4 inverse();
		inline bool Matrix4::getInverse(Matrix4& out) const;


		void makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);


		void makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);


		void decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const;

		/**
		检测这个矩阵是否是仿射矩阵
		@remarks
		一个仿射矩阵是4X4矩阵由3行等于(0,0,0,1)构成
		e.g.非投影系数
		*/
		inline bool isAffine(void) const
		{
			if (RowMajor == true)
			{
				return m[0][3] == 0 && m[1][3] == 0 && m[2][3] == 0 && m[3][3] == 1;
			}
			else{
				return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
			}
			
		}


		/**
		检测这个矩阵是否是仿射矩阵
		@remarks
		一个仿射矩阵是4X4矩阵由3行等于(0,0,0,1)构成
		e.g.非投影系数
		*/
		inline bool isIdentify(void) const
		{
			return m[0][0] == 1 && m[1][1] == 1 && m[2][2] == 1 && m[3][3] == 1;
		}


		Matrix4 inverseAffine(void) const;

		/** 链接两个仿射矩阵
		@note
		这个矩阵必须是仿射矩阵
		*/
		inline Matrix4 concatenateAffine(const Matrix4 &m2) 
		{
			assert(isAffine() && m2.isAffine());

			if (RowMajor == true )
			{
				transpose();
			}
			
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

		/** 用一个仿射矩阵变换3d向量
		@remarks
		通过矩阵转换给定的3D向量，投影这个结果w=1

		@note
		这个矩阵必须是仿射矩阵
		*/
		Vector3 transformAffine(const Vector3& v) const;
		


		Vector4 transformAffine(const Vector4& v) const;
		

		inline Vector3 rotateVect(const Vector3& v)  
		{
			//assert(!isAffine());
			if (RowMajor == true)
			{
				transpose();
			}
			return Vector3(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
		}

		inline void inverseRotateVect(Vector3& vect) 
		{
			Vector3 tmp;
			if (RowMajor == true)
			{
				transpose();
			}
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
		if (mat.RowMajor == false)
		{
			return Vector4(
				v.x*mat[0][0] + v.y*mat[0][1] + v.z*mat[0][2] + v.w*mat[0][3],
				v.x*mat[1][0] + v.y*mat[1][1] + v.z*mat[1][2] + v.w*mat[1][3],
				v.x*mat[2][0] + v.y*mat[2][1] + v.z*mat[2][2] + v.w*mat[2][3],
				v.x*mat[3][0] + v.y*mat[3][1] + v.z*mat[3][2] + v.w*mat[3][3]
				);
		}
		else{
			return Vector4(
				v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
				v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
				v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
				v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
				);
		}
		
	}



}

#endif