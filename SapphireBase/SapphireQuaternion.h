
#ifndef __SAPPHIRE_QUATERNION_H__
#define __SAPPHIRE_QUATERNION_H__

#include "SapphirePrerequisites.h"
#include "SapphireMath.h"

namespace Sapphire {

	class _SapphireExport Quaternion
	{
	public:
		/// Ĭ�Ϲ���������ʼ����λ��ת0��
		inline Quaternion()
			: w(1), x(0), y(0), z(0)
		{
		}
		//! ������ת��һ��ŷ���ǵ�һ����Ԫ��
		inline Quaternion(Real fx,Real fy, Real fz)
			:x(fx), y(fy), z(fz)
		{
		}
		//! ������ת��һ��ŷ����(����)��һ����Ԫ��
		Quaternion(const Vector3& vec);
		
		/// ����ȷ��ֵ������Ԫ��
	    inline Quaternion(
			Real fW,
			Real fX, Real fY, Real fZ)
			: w(fW), x(fX), y(fY), z(fZ)
		{
		}
		/// ��һ����ת��������Ԫ��
		inline Quaternion(const Matrix3& rot)
		{
			this->FromRotationMatrix(rot);
		}
		/// ��һ���Ƕ�/�ṹ��һ����Ԫ��
		inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
		{
			this->FromAngleAxis(rfAngle, rkAxis);
		}
		/// ���������������ṹ��һ����Ԫ��
		inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
		{
			this->FromAxes(xaxis, yaxis, zaxis);
		}
		///  ���������������ṹ��һ����Ԫ��
		inline Quaternion(const Vector3* akAxis)
		{
			this->FromAxes(akAxis);
		}
		/// ���ֶ�����4��W/X/Y/Zֵ������һ����Ԫ��
		inline Quaternion(Real* valptr)
		{
			memcpy(&w, valptr, sizeof(Real) * 4);
		}

		/** 
		���������Ԫ��������һ��������
		*/
		inline void swap(Quaternion& other)
		{
			std::swap(w, other.w);
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		/// ������ʲ�����
		inline Real operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// ������ʲ�����
		inline Real& operator [] (const size_t i)
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// Ϊ��ֱ�ӿ�����ָ�������
		inline Real* ptr()
		{
			return &w;
		}

		/// Ϊ��ֱ�ӿ�����ָ�������
		inline const Real* ptr() const
		{
			return &w;
		}

		void FromRotationMatrix(const Matrix3& kRot);
		void ToRotationMatrix(Matrix3& kRot) const;
		/** 
		����Ԫ������һ������
		*/
		void getMatrix(Matrix4 &dest, const Vector3 &translation) const;

		void getMatrix(Matrix4 &dest) const;
		/** 
		��֧�ֵ�����������Ԫ����ͨ��ָ���Ƕ�����roll������
		*/
		void FromAngleAxis(const Radian& rfAngle, const Vector3& rkAxis);
		void ToAngleAxis(Radian& rfAngle, Vector3& rkAxis) const;
		inline void ToAngleAxis(Degree& dAngle, Vector3& rkAxis) const {
			Radian rAngle;
			ToAngleAxis(rAngle, rkAxis);
			dAngle = rAngle;
		}
		/** 
		��3�ṹ�������Ԫ���� ��Щ������Ѿ������� 
		@�� FromAxes
		*/
		void FromAxes(const Vector3* akAxis);
		//void FromAxes(const Vector3& akAxis) const;
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		/** ȡ��3�����������ᶨ�������Ԫ�� 
		@�� FromAxes */
		void ToAxes(Vector3* akAxis) const;
		
		void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

		/** 
		���������Ԫ���������X�����ᡣ ������xAxis = Vector3::UNIT_X * this
		ͬ�������������X��
		*/
		Vector3 xAxis(void) const;

		/** 
		���������Ԫ���������Y�����ᡣ ������yAxis = Vector3::UNIT_Y * this
		ͬ�������������Y��
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
		// һ����Ԫ���ĺ���
		/// ������Ԫ���ĵ��
		Real Dot(const Quaternion& rkQ) const;
		/* ���������Ԫ���ķ��߳���
		*/
		Real Norm() const;
		/// ��׼����Ԫ�������ҷ���֮ǰ�ĳ���
		Real normalise(void);
		Quaternion Inverse() const;  // Ӧ�÷�0��Ԫ��
		Quaternion UnitInverse() const;  // Ӧ�õ�λ������Ԫ��
		Quaternion Exp() const;
		Quaternion Log() const;

		/// ͨ��һ����Ԫ����תһ������
		Vector3 operator* (const Vector3& rkVector) const;

		/** 
		���������Ԫ���ı��ع�תrollԪ��
		@param reprojectAxis Ĭ������������� 'intuitive' ���
		�����ͶӰ��Ԫ������y��x��y�ᣬ�������֮��ĽǶȽ������ء��������Ϊfalse��������
		��ʵ����yawƫת���⽫����Ԫ��ʵ�֣�����ȥ��ȡͬ���ķ������̿���·���Ϳ��ܰ������ٵ�����ת
		�ⷵ��ֵ�������Ǵ�-180��180��
		*/
		Radian getRoll(bool reprojectAxis = true) const;
		/** 
		���������Ԫ���ı�����бpitchԪ��
		@param reprojectAxis Ĭ������������� 'intuitive' ���
		�����ͶӰ��Ԫ������Z��X��Y�ᣬ�������֮��ĽǶȽ������ء��������Ϊtrue��������
		��ʵ����yawƫת���⽫����Ԫ��ʵ�֣�����ȥ��ȡͬ���ķ������̿���·���Ϳ��ܰ������ٵ�����ת
		�ⷵ��ֵ�������Ǵ�-180��180��
		*/
		Radian getPitch(bool reprojectAxis = true) const;
		/** 
		���������Ԫ���ı���ƫתyawԪ��
		@param reprojectAxis Ĭ������������� 'intuitive' ���
		�����ͶӰ��Ԫ������y��X��Z�ᣬ�������֮��ĽǶȽ������ء��������Ϊtrue��������
		��ʵ����yawƫת���⽫����Ԫ��ʵ�֣�����ȥ��ȡͬ���ķ������̿���·���Ϳ��ܰ������ٵ�����ת
		�ⷵ��ֵ�������Ǵ�-180��180��
		 
		*/
		Radian getYaw(bool reprojectAxis = true) const;
		/// �ù����ж��Ƿ����
		bool equals(const Quaternion& rhs, const Radian& tolerance) const;

		/** 
		Ӧ����������Ԫ�����������Բ�ֵ�����ҷ��ؽ��
		Slerp ( 0.0f, A, B ) = A
		Slerp ( 1.0f, A, B ) = B
		@return ��ֵ�����Ԫ��
		@remarks
		��ֵ�����ú㶨���ʲ�ֵ�����ԣ� ��������С��ת�أ�����shortestPath=false��
		������Σ����ǲ��ɽ����ģ���ζ��Slerp(0.75f,A,B) != Slerp(0.25f,B,A)
		���С�Ĳ����������IK�����ر���Ҫ
		*/
		static Quaternion Slerp(Real fT, const Quaternion& rkP,
			const Quaternion& rkQ, bool shortestPath = false);


		static Quaternion Slerp(const Quaternion& rkP,
			const Quaternion& rkQ, Real fT=0.5);

		/** 
		@��Slerp. ������������������spins������ת��Σ���ͨ������ 'iExtraSpins'��
		��ָ��������ֵ����������ֵ��Ĵ���
		 
		*/
		static Quaternion SlerpExtraSpins(Real fT,
			const Quaternion& rkP, const Quaternion& rkQ,
			int iExtraSpins);

		// ����������β�ֵ
		static void Intermediate(const Quaternion& rkQ0,
			const Quaternion& rkQ1, const Quaternion& rkQ2,
			Quaternion& rka, Quaternion& rkB);

		// ������β�ֵ
		static Quaternion Squad(Real fT, const Quaternion& rkP,
			const Quaternion& rkA, const Quaternion& rkB,
			const Quaternion& rkQ, bool shortestPath = false);

		/** 
		��������Ԫ��ֱ��Ӧ�ñ�׼�����Բ�ֵ�����ҷ��ؽ��
		nlerp ( 0.0f, A, B ) = A
		nlerp ( 1.0f, A, B ) = B
		@remarks
		Nlerp ��Slerp����
		Nlerp ���Խ���(��Slerp��ĳЩ�ض��ط���Ҫ�����ԣ���IK����),���ҿ�����Сת�أ�����shortestPath=false��
		Ȼ�������������ڷǺ㶨���ʵĲ�ֵ����ʱ���õ�����ʱ���ᡣ ��һ���Ǻ㶨���ʿ����ṩ����Ȼ����ת�����������Ȩ�أ�
		�����ĳ������Ŷ�ʱ��ת���������ָ��һ���ر�ĽǶ���һ���ض���Ȩ��ֵ����ôSlerp�Ǹ��õ�ѡ��
		*/
		static Quaternion nlerp(Real fT, const Quaternion& rkP,
			const Quaternion& rkQ, bool shortestPath = false);

		/// �ض�����0ֵ
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
