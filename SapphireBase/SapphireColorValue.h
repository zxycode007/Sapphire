#ifndef _SAPPHIRE_COLOURVALUE_H__
#define _SAPPHIRE_COLOURVALUE_H__

#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"
#include "SapphireMath.h"

namespace Sapphire {



	//! 一个纹理的颜色格式的枚举
	enum ECOLOR_FORMAT
	{
		//! 16位色彩格式：用于软件驱动
		/** 因此它可以为引擎所有其它视频驱动的首选
		每个颜色分量有5位，alpha信息由单个位组成 */
		ECF_A1R5G5B5 = 0,

		//! 标准的16位颜色格式
		ECF_R5G6B5,

		//! 标准的24位颜色格式，没有alpha通道，红绿蓝均8位
		ECF_R8G8B8,

		//! 默认的32位颜色格式，每个分量均8位
		ECF_A8R8G8B8,

		/** 浮点格式。下列格式只能用于渲染目标纹理 */

		//! 16位浮点格式，使用16位的红色通道
		ECF_R16F,

		//! 32位浮点格式，用16位红色通道和16位绿色通道
		ECF_G16R16F,

		//! 64位浮点格式，用16位红，绿，蓝和alpha通道
		ECF_A16B16G16R16F,

		//! 32位浮点格式，用32位的红色通道
		ECF_R32F,

		//! 64位浮点格式，用32位的红色和绿色通道
		ECF_G32R32F,

		//! 128位浮点格式，用32位的红绿蓝和透明格式
		ECF_A32B32G32R32F,

		 
		ECF_UNKNOWN
	};


	//! 创建一个16位的A1R5G5B5颜色
	inline UINT16 RGBA16(UINT32 r, UINT32 g, UINT32 b, UINT32 a = 0xFF)
	{
		return (UINT16)((a & 0x80) << 8 |
			(r & 0xF8) << 7 |
			(g & 0xF8) << 2 |
			(b & 0xF8) >> 3);
	}


	//! 创建一个16位的A1R5G5B5颜色 
	inline UINT16 RGB16(UINT32 r, UINT32 g, UINT32 b)
	{
		return RGBA16(r, g, b);
	}


	//!  创建一个16位的A1R5G5B5颜色 ,基于16位输入值
	inline UINT16 RGB16from16(UINT16 r, UINT16 g, UINT16 b)
	{
		return (0x8000 |
			(r & 0x1F) << 10 |
			(g & 0x1F) << 5 |
			(b & 0x1F));
	}


	//! 转换一个32位(X8R8G8B8)颜色到16位的A1R5G5B5
	inline UINT16 X8R8G8B8toA1R5G5B5(UINT32 color)
	{
		return (UINT16)(0x8000 |
			(color & 0x00F80000) >> 9 |
			(color & 0x0000F800) >> 6 |
			(color & 0x000000F8) >> 3);
	}


	//! 转换一个32位(A8R8G8B8)颜色到16位的A1R5G5B5
	inline UINT16 A8R8G8B8toA1R5G5B5(UINT32 color)
	{
		return (UINT16)((color & 0x80000000) >> 16 |
			(color & 0x00F80000) >> 9 |
			(color & 0x0000F800) >> 6 |
			(color & 0x000000F8) >> 3);
	}


	//! 转换一个32位(A8R8G8B8) 颜色到16位的R5G6B5
	inline UINT16 A8R8G8B8toR5G6B5(UINT32 color)
	{
		return (UINT16)((color & 0x00F80000) >> 8 |
			(color & 0x0000FC00) >> 5 |
			(color & 0x000000F8) >> 3);
	}


	//! 从A1R5G5B5转换到A8R8G8B8
	inline UINT32 A1R5G5B5toA8R8G8B8(UINT16 color)
	{
		return (((-((SINT32)color & 0x00008000) >> (SINT32)31) & 0xFF000000) |
			((color & 0x00007C00) << 9) | ((color & 0x00007000) << 4) |
			((color & 0x000003E0) << 6) | ((color & 0x00000380) << 1) |
			((color & 0x0000001F) << 3) | ((color & 0x0000001C) >> 2)
			);
	}


	//! 返回R5G6B5的A8R8G8B8的颜色
	inline UINT32 R5G6B5toA8R8G8B8(UINT16 color)
	{
		return 0xFF000000 |
			((color & 0xF800) << 8) |
			((color & 0x07E0) << 5) |
			((color & 0x001F) << 3);
	}


	//! 返回从A1R5G5B5的R5G6B5的颜色 
	inline UINT16 R5G6B5toA1R5G5B5(UINT16 color)
	{
		return 0x8000 | (((color & 0xFFC0) >> 1) | (color & 0x1F));
	}


	//! 返回A1R5G5B5到R5G6B5的颜色  
	inline UINT16 A1R5G5B5toR5G6B5(UINT16 color)
	{
		return (((color & 0x7FE0) << 1) | (color & 0x1F));
	}



	//! 返回A1R5G5B5的alpha分量
 
	inline UINT32 getAlpha(UINT16 color)
	{
		return ((color >> 15) & 0x1);
	}


	//! 返回A1R5G5B5的红色分量
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getRed(UINT16 color)
	{
		return ((color >> 10) & 0x1F);
	}


	//! 返回A1R5G5B5的绿色分量
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getGreen(UINT16 color)
	{
		return ((color >> 5) & 0x1F);
	}


	//! 返回A1R5G5B5的蓝色分量
	/** Shift left by 3 to get 8 bit value. */
	inline UINT32 getBlue(UINT16 color)
	{
		return (color & 0x1F);
	}


	//! 返回16位A1R5G5B5的平均值
	inline SINT32 getAverage(SINT32 color)
	{
		return ((getRed(color) << 3) + (getGreen(color) << 3) + (getBlue(color) << 3)) / 3;
	}
	 
	////32位RGBA
	typedef uint32 RGBA; 
	////32位ARGB
	typedef uint32 ARGB;
	////32位ABGR
	typedef uint32 ABGR;
	////32位BGRA
	typedef uint32 BGRA;

	/** 表示颜色的类
	@remarks
	颜色由4个分量所表示，每个分量为一个0.0到1.0的浮点值
	@par
	这3个标准的颜色分量是红，绿，蓝， 一个更大数指定这个颜色中的这个分量有更大的份量。
	第四个分量是‘alpha’值，它表示透明度。 0.0完全透明，1.0完全不透明。
	*/
	class _SapphireExport ColourValue
	{
	public:
		//定义常用颜色
		static const ColourValue ZERO;
		static const ColourValue Black;   //黑   
		static const ColourValue White;    //白
		static const ColourValue Red;      //红
		static const ColourValue Green;     //绿
		static const ColourValue Blue;        //蓝
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

			// 转换到32位模式
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

			// 转换到32位模式
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

		/** 返回RGBA
		*/
		RGBA getAsRGBA(void) const;

		/** 返回ARGB
		*/
		ARGB getAsARGB(void) const;

		/**返回BGRA
		*/
		BGRA getAsBGRA(void) const;

		/** 返回ABGR */
		ABGR getAsABGR(void) const;

		/** 设置 RGBA.
		*/
		void setAsRGBA(const RGBA val);

		/** 设置 ARGB.
		*/
		void setAsARGB(const ARGB val);

		/**设置 BGRA.
		*/
		void setAsBGRA(const BGRA val);

		/** 设置 ABGR.
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
		


		//! 获取在颜色范围[0,255]中的光照值
		Real getLightness() 
		{
			return 0.5f*(Math::_max<UINT32>(Math::_max<UINT32>(getRed(), getGreen()), getBlue()) + Math::_min<UINT32>(Math::_min<UINT32>(getRed(), getGreen()), getBlue()));
		}
		//获取亮度
		Real getLuminance() 
		{
			return 0.3f*getRed() + 0.59f*getGreen() + 0.11f*getBlue();
		}

		/** 截断颜色值到范围[0,1]中
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
		对于一个饱和度，As saturate,除这个颜色值不受影响之外和饱和颜色值作为一个拷贝返回
		  */
		ColourValue saturateCopy(void) const
		{
			ColourValue ret = *this;
			ret.saturate();
			return ret;
		}

		/// 数组访问器
		inline float operator [] (const size_t i) const
		{
			assert(i < 4);

			return *(&r + i);
		}

		///数组访问器
		inline float& operator [] (const size_t i)
		{
			assert(i < 4);

			return *(&r + i);
		}

		/// 对于直接拷贝的指针访问器
		inline float* ptr()
		{
			return &r;
		}
		///对于直接拷贝的指针访问器
		inline const float* ptr() const
		{
			return &r;
		}


		// 数学操作
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

		//! 以指定的颜色格式写入一个颜色数据
		/** \param data: 目标的颜色数据。必须包含足够大的内容一般接受指定格式的颜色数据
		\param format: 告诉要用什么颜色数据
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

		/** 设置色相Hue，饱和度Saturation，和明亮度Brithtness
		@param hue 色相值, 将0-360缩放到 [0,1]范围
		@param saturation 饱和度级别, [0,1]
		@param brightness 明亮度级别, [0,1]
		*/
		void setHSB(Real hue, Real saturation, Real brightness);

		/** 转换当前颜色到设置色相Hue，饱和度Saturation，和明亮度Brithtness值
		@param hue 输出色相值, 将0-360缩放到 [0,1]范围
		@param saturation 输出饱和度级别, [0,1]
		@param brightness 输出明亮度级别, [0,1]
		*/
		void getHSB(Real* hue, Real* saturation, Real* brightness) const;

		
		

		//! 以指定的颜色格式写入一个颜色数据
		/** \param data: 目标的颜色数据。必须包含足够大的内容一般接受指定格式的颜色数据
		\param format: 告诉要用什么颜色数据
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


		//! 用Real插值这个颜色到另外一个颜色
		/** \param other: 另外的颜色
		\param d: 在0.0f 和 1.0f之间的值
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

		//! 返回插值的颜色（二次的） 
		/** \param c1: 第一个要与之插值的颜色
		\param c2: 第二个要与之插值的颜色
		\param d: 在0.0f到1.0f之间的数 */
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

		/** 输出到一个流
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
