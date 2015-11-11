#ifndef _SAPPHIRE_COLOURVALUE_H__
#define _SAPPHIRE_COLOURVALUE_H__

#include "SapphirePrerequisites.h"

namespace Sapphire {
	 
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

		explicit ColourValue(float red = 1.0f,
			float green = 1.0f,
			float blue = 1.0f,
			float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha)
		{ }

		bool operator==(const ColourValue& rhs) const;
		bool operator!=(const ColourValue& rhs) const;

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

}  

#endif
