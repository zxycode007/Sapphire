#ifndef _SAPPHIRE_COLOURVALUE_H__
#define _SAPPHIRE_COLOURVALUE_H__

#include "SapphirePrerequisites.h"

namespace Sapphire {
	 
	////32λRGBA
	typedef uint32 RGBA; 
	////32λARGB
	typedef uint32 ARGB;
	////32λABGR
	typedef uint32 ABGR;
	////32λBGRA
	typedef uint32 BGRA;

	/** ��ʾ��ɫ����
	@remarks
	��ɫ��4����������ʾ��ÿ������Ϊһ��0.0��1.0�ĸ���ֵ
	@par
	��3����׼����ɫ�����Ǻ죬�̣����� һ��������ָ�������ɫ�е���������и���ķ�����
	���ĸ������ǡ�alpha��ֵ������ʾ͸���ȡ� 0.0��ȫ͸����1.0��ȫ��͸����
	*/
	class _SapphireExport ColourValue
	{
	public:
		//���峣����ɫ
		static const ColourValue ZERO;
		static const ColourValue Black;   //��   
		static const ColourValue White;    //��
		static const ColourValue Red;      //��
		static const ColourValue Green;     //��
		static const ColourValue Blue;        //��

		explicit ColourValue(float red = 1.0f,
			float green = 1.0f,
			float blue = 1.0f,
			float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha)
		{ }

		bool operator==(const ColourValue& rhs) const;
		bool operator!=(const ColourValue& rhs) const;

		float r, g, b, a;

		/** ����RGBA
		*/
		RGBA getAsRGBA(void) const;

		/** ����ARGB
		*/
		ARGB getAsARGB(void) const;

		/**����BGRA
		*/
		BGRA getAsBGRA(void) const;

		/** ����ABGR */
		ABGR getAsABGR(void) const;

		/** ���� RGBA.
		*/
		void setAsRGBA(const RGBA val);

		/** ���� ARGB.
		*/
		void setAsARGB(const ARGB val);

		/**���� BGRA.
		*/
		void setAsBGRA(const BGRA val);

		/** ���� ABGR.
		*/
		void setAsABGR(const ABGR val);

		/** �ض���ɫֵ����Χ[0,1]��
		*/
		void saturate(void)
		{
			if (r < 0)
				r = 0;
			else if (r > 1)
				r = 1;

			if (g < 0)
				g = 0;
			else if (g > 1)
				g = 1;

			if (b < 0)
				b = 0;
			else if (b > 1)
				b = 1;

			if (a < 0)
				a = 0;
			else if (a > 1)
				a = 1;
		}

		/** 
		����һ�����Ͷȣ�As saturate,�������ɫֵ����Ӱ��֮��ͱ�����ɫֵ��Ϊһ����������
		  */
		ColourValue saturateCopy(void) const
		{
			ColourValue ret = *this;
			ret.saturate();
			return ret;
		}

		/// ���������
		inline float operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&r + i);
		}

		///���������
		inline float& operator [] (const size_t i)
		{
			assert(i < 4);

			return *(&r + i);
		}

		/// ����ֱ�ӿ�����ָ�������
		inline float* ptr()
		{
			return &r;
		}
		///����ֱ�ӿ�����ָ�������
		inline const float* ptr() const
		{
			return &r;
		}


		// ��ѧ����
		inline ColourValue operator + (const ColourValue& rkVector) const
		{
			ColourValue kSum;

			kSum.r = r + rkVector.r;
			kSum.g = g + rkVector.g;
			kSum.b = b + rkVector.b;
			kSum.a = a + rkVector.a;

			return kSum;
		}

		inline ColourValue operator - (const ColourValue& rkVector) const
		{
			ColourValue kDiff;

			kDiff.r = r - rkVector.r;
			kDiff.g = g - rkVector.g;
			kDiff.b = b - rkVector.b;
			kDiff.a = a - rkVector.a;

			return kDiff;
		}

		inline ColourValue operator * (const float fScalar) const
		{
			ColourValue kProd;

			kProd.r = fScalar*r;
			kProd.g = fScalar*g;
			kProd.b = fScalar*b;
			kProd.a = fScalar*a;

			return kProd;
		}

		inline ColourValue operator * (const ColourValue& rhs) const
		{
			ColourValue kProd;

			kProd.r = rhs.r * r;
			kProd.g = rhs.g * g;
			kProd.b = rhs.b * b;
			kProd.a = rhs.a * a;

			return kProd;
		}

		inline ColourValue operator / (const ColourValue& rhs) const
		{
			ColourValue kProd;

			kProd.r = r / rhs.r;
			kProd.g = g / rhs.g;
			kProd.b = b / rhs.b;
			kProd.a = a / rhs.a;

			return kProd;
		}

		inline ColourValue operator / (const float fScalar) const
		{
			assert(fScalar != 0.0);

			ColourValue kDiv;

			float fInv = 1.0f / fScalar;
			kDiv.r = r * fInv;
			kDiv.g = g * fInv;
			kDiv.b = b * fInv;
			kDiv.a = a * fInv;

			return kDiv;
		}

		inline friend ColourValue operator * (const float fScalar, const ColourValue& rkVector)
		{
			ColourValue kProd;

			kProd.r = fScalar * rkVector.r;
			kProd.g = fScalar * rkVector.g;
			kProd.b = fScalar * rkVector.b;
			kProd.a = fScalar * rkVector.a;

			return kProd;
		}

		 
		inline ColourValue& operator += (const ColourValue& rkVector)
		{
			r += rkVector.r;
			g += rkVector.g;
			b += rkVector.b;
			a += rkVector.a;

			return *this;
		}

		inline ColourValue& operator -= (const ColourValue& rkVector)
		{
			r -= rkVector.r;
			g -= rkVector.g;
			b -= rkVector.b;
			a -= rkVector.a;

			return *this;
		}

		inline ColourValue& operator *= (const float fScalar)
		{
			r *= fScalar;
			g *= fScalar;
			b *= fScalar;
			a *= fScalar;
			return *this;
		}

		inline ColourValue& operator /= (const float fScalar)
		{
			assert(fScalar != 0.0);

			float fInv = 1.0f / fScalar;

			r *= fInv;
			g *= fInv;
			b *= fInv;
			a *= fInv;

			return *this;
		}

		/** ����ɫ��Hue�����Ͷ�Saturation����������Brithtness
		@param hue ɫ��ֵ, ��0-360���ŵ� [0,1]��Χ
		@param saturation ���Ͷȼ���, [0,1]
		@param brightness �����ȼ���, [0,1]
		*/
		void setHSB(Real hue, Real saturation, Real brightness);

		/** ת����ǰ��ɫ������ɫ��Hue�����Ͷ�Saturation����������Brithtnessֵ
		@param hue ���ɫ��ֵ, ��0-360���ŵ� [0,1]��Χ
		@param saturation ������Ͷȼ���, [0,1]
		@param brightness ��������ȼ���, [0,1]
		*/
		void getHSB(Real* hue, Real* saturation, Real* brightness) const;



		/** �����һ����
		*/
		inline _SapphireExport friend std::ostream& operator <<
			(std::ostream& o, const ColourValue& c)
		{
			o << "ColourValue(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
			return o;
		}

	};
	/** @} */
	/** @} */

}  

#endif
