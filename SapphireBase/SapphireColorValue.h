#ifndef _SAPPHIRE_COLOURVALUE_H__
#define _SAPPHIRE_COLOURVALUE_H__

#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"

namespace Sapphire {



	//! һ���������ɫ��ʽ��ö��
	enum ECOLOR_FORMAT
	{
		//! 16λɫ�ʸ�ʽ�������������
		/** ���������Ϊ��������������Ƶ��������ѡ
		ÿ����ɫ������5λ��alpha��Ϣ�ɵ���λ��� */
		ECF_A1R5G5B5 = 0,

		//! ��׼��16λ��ɫ��ʽ
		ECF_R5G6B5,

		//! ��׼��24λ��ɫ��ʽ��û��alphaͨ������������8λ
		ECF_R8G8B8,

		//! Ĭ�ϵ�32λ��ɫ��ʽ��ÿ��������8λ
		ECF_A8R8G8B8,

		/** �����ʽ�����и�ʽֻ��������ȾĿ������ */

		//! 16λ�����ʽ��ʹ��16λ�ĺ�ɫͨ��
		ECF_R16F,

		//! 32λ�����ʽ����16λ��ɫͨ����16λ��ɫͨ��
		ECF_G16R16F,

		//! 64λ�����ʽ����16λ�죬�̣�����alphaͨ��
		ECF_A16B16G16R16F,

		//! 32λ�����ʽ����32λ�ĺ�ɫͨ��
		ECF_R32F,

		//! 64λ�����ʽ����32λ�ĺ�ɫ����ɫͨ��
		ECF_G32R32F,

		//! 128λ�����ʽ����32λ�ĺ�������͸����ʽ
		ECF_A32B32G32R32F,

		 
		ECF_UNKNOWN
	};


	//! ����һ��16λ��A1R5G5B5��ɫ
	inline UINT16 RGBA16(UINT32 r, UINT32 g, UINT32 b, UINT32 a = 0xFF)
	{
		return (UINT16)((a & 0x80) << 8 |
			(r & 0xF8) << 7 |
			(g & 0xF8) << 2 |
			(b & 0xF8) >> 3);
	}


	//! ����һ��16λ��A1R5G5B5��ɫ 
	inline UINT16 RGB16(UINT32 r, UINT32 g, UINT32 b)
	{
		return RGBA16(r, g, b);
	}


	//!  ����һ��16λ��A1R5G5B5��ɫ ,����16λ����ֵ
	inline UINT16 RGB16from16(UINT16 r, UINT16 g, UINT16 b)
	{
		return (0x8000 |
			(r & 0x1F) << 10 |
			(g & 0x1F) << 5 |
			(b & 0x1F));
	}


	//! ת��һ��32λ(X8R8G8B8)��ɫ��16λ��A1R5G5B5
	inline UINT16 X8R8G8B8toA1R5G5B5(UINT32 color)
	{
		return (UINT16)(0x8000 |
			(color & 0x00F80000) >> 9 |
			(color & 0x0000F800) >> 6 |
			(color & 0x000000F8) >> 3);
	}


	//! ת��һ��32λ(A8R8G8B8)��ɫ��16λ��A1R5G5B5
	inline UINT16 A8R8G8B8toA1R5G5B5(UINT32 color)
	{
		return (UINT16)((color & 0x80000000) >> 16 |
			(color & 0x00F80000) >> 9 |
			(color & 0x0000F800) >> 6 |
			(color & 0x000000F8) >> 3);
	}


	//! ת��һ��32λ(A8R8G8B8) ��ɫ��16λ��R5G6B5
	inline UINT16 A8R8G8B8toR5G6B5(UINT32 color)
	{
		return (UINT16)((color & 0x00F80000) >> 8 |
			(color & 0x0000FC00) >> 5 |
			(color & 0x000000F8) >> 3);
	}


	//! ��A1R5G5B5ת����A8R8G8B8
	inline UINT32 A1R5G5B5toA8R8G8B8(UINT16 color)
	{
		return (((-((SINT32)color & 0x00008000) >> (SINT32)31) & 0xFF000000) |
			((color & 0x00007C00) << 9) | ((color & 0x00007000) << 4) |
			((color & 0x000003E0) << 6) | ((color & 0x00000380) << 1) |
			((color & 0x0000001F) << 3) | ((color & 0x0000001C) >> 2)
			);
	}


	//! ����R5G6B5��A8R8G8B8����ɫ
	inline UINT32 R5G6B5toA8R8G8B8(UINT16 color)
	{
		return 0xFF000000 |
			((color & 0xF800) << 8) |
			((color & 0x07E0) << 5) |
			((color & 0x001F) << 3);
	}


	//! ���ش�A1R5G5B5��R5G6B5����ɫ 
	inline UINT16 R5G6B5toA1R5G5B5(UINT16 color)
	{
		return 0x8000 | (((color & 0xFFC0) >> 1) | (color & 0x1F));
	}


	//! ����A1R5G5B5��R5G6B5����ɫ  
	inline UINT16 A1R5G5B5toR5G6B5(UINT16 color)
	{
		return (((color & 0x7FE0) << 1) | (color & 0x1F));
	}



	//! ����A1R5G5B5��alpha����
 
	inline UINT32 getAlpha(UINT16 color)
	{
		return ((color >> 15) & 0x1);
	}


	//! ����A1R5G5B5�ĺ�ɫ����
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getRed(UINT16 color)
	{
		return ((color >> 10) & 0x1F);
	}


	//! ����A1R5G5B5����ɫ����
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getGreen(UINT16 color)
	{
		return ((color >> 5) & 0x1F);
	}


	//! ����A1R5G5B5����ɫ����
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getBlue(UINT16 color)
	{
		return (color & 0x1F);
	}


	//! ����16λA1R5G5B5��ƽ��ֵ
	inline SINT32 getAverage(SINT32 color)
	{
		return ((getRed(color) << 3) + (getGreen(color) << 3) + (getBlue(color) << 3)) / 3;
	}
	 
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
		//static const Real inv;

		explicit ColourValue(float red = 1.0f,
			float green = 1.0f,
			float blue = 1.0f,
			float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha)
		{ }

		bool operator==(const ColourValue& rhs) const;
		 
		bool operator!=(const ColourValue& rhs) const;

		ColourValue& operator=(const UINT32& rhs) const;

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


		void setAlpha(UINT32 alpha)
		{
			a = static_cast<Real>(alpha) / 255;
		};

		void setRed(UINT32 red)
		{
			r = static_cast<Real>(red) / 255;
		};

		void setGreen(UINT32 green)
		{
			static_cast<Real>(green) / 255;
		}

		void setBlue(UINT32 blue)
		{
			static_cast<Real>(blue) / 255;
		};

		UINT32 getAlpha()
		{
			return static_cast<UINT32>(a * 255);
		}

		UINT32 getRed()
		{
			return static_cast<UINT32>(r * 255);
		}

		UINT32 getGreen()
		{
			return static_cast<UINT32>(g * 255);
		}

		UINT32 getBlue()
		{
			return static_cast<UINT32>(b * 255);
		}

		UINT32 getAverage()
		{
			return (getRed()+ getGreen() + getBlue()) / 3;
		}

		//! ��ȡ����ɫ��Χ[0,255]�еĹ���ֵ
		Real getLightness() 
		{
			return 0.5f*(Math::_max<UINT32>(Math::_max<UINT32>(getRed(), getGreen()), getBlue()) + Math::_min<UINT32>(Math::_min<UINT32>(getRed(), getGreen()), getBlue()));
		}
		//��ȡ����
		Real getLuminance() 
		{
			return 0.3f*getRed() + 0.59f*getGreen() + 0.11f*getBlue();
		}

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

		bool operator<(const ColourValue& other) const { return (getAsARGB() < other.getAsARGB()); }

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

		
		

		//! ��ָ������ɫ��ʽд��һ����ɫ����
		/** \param data: Ŀ�����ɫ���ݡ���������㹻�������һ�����ָ����ʽ����ɫ����
		\param format: ����Ҫ��ʲô��ɫ����
		*/
		void getData(void *data, ECOLOR_FORMAT format)
		{
			switch (format)
			{
			case ECF_A1R5G5B5:
			{
				UINT16 * dest = (UINT16*)data;
				*dest =  A8R8G8B8toA1R5G5B5(getAsARGB());
			}
			break;

			case ECF_R5G6B5:
			{
				UINT16 * dest = (UINT16*)data;
				*dest = A8R8G8B8toR5G6B5(getAsARGB());
			}
			break;

			case ECF_R8G8B8:
			{
				UINT8* dest = (UINT8*)data;
				dest[0] = (UINT8)getRed();
				dest[1] = (UINT8)getGreen();
				dest[2] = (UINT8)getBlue();
			}
			break;

			case ECF_A8R8G8B8:
			{
				UINT32 * dest = (UINT32*)data;
				*dest = getAsARGB();
			}
			break;

			default:
				break;
			}
		}

		//! ��Real��ֵ�����ɫ������һ����ɫ
		/** \param other: �������ɫ
		\param d: ��0.0f �� 1.0f֮���ֵ
		\return Interpolated color. */
		ColourValue getInterpolated(const ColourValue &other, Real d) const
		{
			d = Math::Clamp<Real>(d, 0.f, 1.f);
			const Real inv = 1.0f - d;
			return ColourValue(
				 (other.r*inv + r*d),
				 (other.g*inv + g*d),
				 (other.b*inv + b*d),
				 (other.a*inv + a*d)
				 );
		}

		//! ���ز�ֵ����ɫ�����εģ� 
		/** \param c1: ��һ��Ҫ��֮��ֵ����ɫ
		\param c2: �ڶ���Ҫ��֮��ֵ����ɫ
		\param d: ��0.0f��1.0f֮����� */
		ColourValue getInterpolated_quadratic(const ColourValue& c1, const ColourValue& c2, Real d) const
		{
			// this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
			d = Math::Clamp<Real>(d, 0.f, 1.f);
			const Real inv = 1.f - d;
			const Real mul0 = inv * inv;
			const Real mul1 = 2.f * d * inv;
			const Real mul2 = d * d;

			return ColourValue(
				Math::Clamp<Real>((
				r   * mul0 + c1.r   * mul1 + c2.r   * mul2), 0.0, 1.0f),
				Math::Clamp<Real>( (
				g * mul0 + c1.g * mul1 + c2.g * mul2), 0.0, 1.0f),
				Math::Clamp<Real>( (
				b  * mul0 + c1.b  * mul1 + c2.b  * mul2), 0.0, 1.0f),
				Math::Clamp<Real>((
				a * mul0 + c1.a * mul1 + c2.a * mul2), 0.0, 1.0f)
				);
		}

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
