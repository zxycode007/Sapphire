#ifndef _SAPPHIRE_COLOURVALUE_H__
#define _SAPPHIRE_COLOURVALUE_H__

#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"
#include "SapphireMath.h"

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

		static ColourValue getColourValue(UINT32 alpha, UINT32 red, UINT32 green, UINT32 blue)
		{
			ColourValue c;
			c.setAlpha(alpha);
			c.setRed(red);
			c.setGreen(green);
			c.setBlue(blue);
			return c;
		}

		ColourValue(ARGB color)
		{
			uint32 val32 = color;

			// ת����32λģʽ
			// (ARGB = 8888)

			// Alpha
			a = ((val32 >> 24) & 0xFF) / 255.0f;

			// Red
			r = ((val32 >> 16) & 0xFF) / 255.0f;

			// Green
			g = ((val32 >> 8) & 0xFF) / 255.0f;

			// Blue
			b = (val32 & 0xFF) / 255.0f;
		}

		ColourValue(SINT32 color)
		{
			SINT32 val32 = color;

			// ת����32λģʽ
			// (ARGB = 8888)

			// Alpha
			a = ((val32 >> 24) & 0xFF) / 255.0f;

			// Red
			r = ((val32 >> 16) & 0xFF) / 255.0f;

			// Green
			g = ((val32 >> 8) & 0xFF) / 255.0f;

			// Blue
			b = (val32 & 0xFF) / 255.0f;
		}
		

		/* ColourValue(UINT32 alpha=255, UINT32 red=255, UINT32 green=255, UINT32 blue=255, bool ignore=true)
		{
			setAlpha(alpha);
			setRed(red);
			setGreen(green);
			setBlue(blue);
		}
		*/
		bool operator==(const ColourValue& rhs) const;
		 
		bool operator!=(const ColourValue& rhs) const;

		//ColourValue& operator=(const UINT32& rhs) const;

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
			g = static_cast<Real>(green) / 255;
		}

		void setBlue(UINT32 blue)
		{
			b = static_cast<Real>(blue) / 255;
		};

		UINT32 getAlpha() const
		{
			return static_cast<UINT32>(a * 255);
		}

		UINT32 getRed() const
		{
			return static_cast<UINT32>(r * 255);
		}

		UINT32 getGreen() const
		{
			return static_cast<UINT32>(g * 255);
		}

		UINT32 getBlue() const
		{
			return static_cast<UINT32>(b * 255);
		}

		UINT32 getAverage() 
		{
			return (getRed()+ getGreen() + getBlue()) / 3;
		}

		//! Converts color to OpenGL color format
		/** From ARGB to RGBA in 4 byte components for endian aware
		passing to OpenGL
		\param dest: address where the 4x8 bit OpenGL color is stored. */
		void toOpenGLColor(UINT8* dest) const
		{
			*dest = (UINT8)getRed();
			*++dest = (UINT8)getGreen();
			*++dest = (UINT8)getBlue();
			*++dest = (UINT8)getAlpha();
		}
		//----------------------------------------------------------
		


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

		//! set the color by expecting data in the given format
		/** \param data: must point to valid memory containing color information in the given format
		\param format: tells the format in which data is available
		*/
		void setData(const void *data, ECOLOR_FORMAT format)
		{
			UINT32 color;
			switch (format)
			{
			case ECF_A1R5G5B5:
				color = A1R5G5B5toA8R8G8B8(*(UINT16*)data);
				break;
			case ECF_R5G6B5:
				color = R5G6B5toA8R8G8B8(*(UINT16*)data);
				break;
			case ECF_A8R8G8B8:
				color = *(UINT32*)data;
				break;
			case ECF_R8G8B8:
			{
				UINT8* p = (UINT8*)data;
				//set(255, p[0], p[1], p[2]);
				color = ((255 & 0xff) << 24) | ((p[0] & 0xff) << 16) | ((p[1] & 0xff) << 8) | (p[2] & 0xff);
			}
			break;
			default:
				color = 0xffffffff;
				break;
			}
			setAsARGB(color);
		}

		//! ��ָ������ɫ��ʽд��һ����ɫ����
		/** \param data: Ŀ�����ɫ���ݡ���������㹻�������һ�����ָ����ʽ����ɫ����
		\param format: ����Ҫ��ʲô��ɫ����
		*/
		void getData(void *data, ECOLOR_FORMAT format)
		{
			UINT32 color = getAsARGB();
			switch (format)
			{
			case ECF_A1R5G5B5:
			{
				UINT16 * dest = (UINT16*)data;
				*dest = A8R8G8B8toA1R5G5B5(color);
			}
			break;

			case ECF_R5G6B5:
			{
				UINT16 * dest = (UINT16*)data;
				*dest = A8R8G8B8toR5G6B5(color);
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
				*dest = color;
			}
			break;

			default:
				break;
			}
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
		/*
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
		*/


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


	//! Class representing a color in HSL format
	/** The color values for hue, saturation, luminance
	are stored in 32bit floating point variables. Hue is in range [0,360],
	Luminance and Saturation are in percent [0,100]
	*/
	class SColorHSL
	{
	public:
		SColorHSL ( FLOAT32 h = 0.f, FLOAT32 s = 0.f, FLOAT32 l = 0.f )
			: Hue ( h ), Saturation ( s ), Luminance ( l ) {}

		void fromRGB(const ColourValue &color);
		void toRGB(ColourValue &color) const;

		FLOAT32 Hue;
		FLOAT32 Saturation;
		FLOAT32 Luminance;

	private:
		inline FLOAT32 toRGB1(FLOAT32 rm1, FLOAT32 rm2, FLOAT32 rh) const;

	};

	inline void SColorHSL::fromRGB(const ColourValue &color)
	{
		const FLOAT32 maxVal = Math::max3<FLOAT32>(color.r, color.g, color.b);
		const FLOAT32 minVal = Math::min3<FLOAT32>(color.r, color.g, color.b);
		Luminance = (maxVal + minVal) * 50;
		if (Math::equals(maxVal, minVal))
		{
			Hue = 0.f;
			Saturation = 0.f;
			return;
		}

		const FLOAT32 delta = maxVal - minVal;
		if (Luminance <= 50)
		{
			Saturation = (delta) / (maxVal + minVal);
		}
		else
		{
			Saturation = (delta) / (2 - maxVal - minVal);
		}
		Saturation *= 100;

		if (Math::equals (maxVal, color.r))
			Hue = (color.g - color.b) / delta;
		else if (Math::equals(maxVal, color.g))
			Hue = 2 + ((color.b - color.r) / delta);
		else // blue is max
			Hue = 4 + ((color.r - color.g) / delta);

		Hue *= 60.0f;
		while (Hue < 0.f)
			Hue += 360;
	}


	inline void SColorHSL::toRGB(ColourValue &color) const
	{
		const FLOAT32 l = Luminance / 100;
		if (Math::iszero(Saturation)) // grey
		{
			color.r = 1.0;
			color.g = 1.0;
			color.b = 1.0;
			return;
		}

		FLOAT32 rm2;

		if (Luminance <= 50)
		{
			rm2 = l + l * (Saturation / 100);
		}
		else
		{
			rm2 = l + (1 - l) * (Saturation / 100);
		}

		const FLOAT32 rm1 = 2.0f * l - rm2;

		const FLOAT32 h = Hue / 360.0f;
		color.r = toRGB1(rm1, rm2, h + 1.f / 3.f);
		color.g = toRGB1(rm1, rm2, h); 
		color.b = toRGB1(rm1, rm2, h - 1.f / 3.f);
		 
	}


	// algorithm from Foley/Van-Dam
	inline FLOAT32 SColorHSL::toRGB1(FLOAT32 rm1, FLOAT32 rm2, FLOAT32 rh) const
	{
		if (rh<0)
			rh += 1;
		if (rh>1)
			rh -= 1;

		if (rh < 1.f / 6.f)
			rm1 = rm1 + (rm2 - rm1) * rh*6.f;
		else if (rh < 0.5f)
			rm1 = rm2;
		else if (rh < 2.f / 3.f)
			rm1 = rm1 + (rm2 - rm1) * ((2.f / 3.f) - rh)*6.f;

		return rm1;
	}
}  

#endif
