#ifndef _SAPPHIRE_DIMENSION2D_
#define _SAPPHIRE_DIMENSION2D_



namespace Sapphire
{
	//template <class T>
	//class vector2d;

	 
	template <class T>
	class dimension2d
	{
	public:
		 
		dimension2d() : Width(0), Height(0) {}
	 
		dimension2d(const T& width, const T& height)
			: Width(width), Height(height) {}

		dimension2d(const Vector2& other);  

		 
		template <class U>
		explicit dimension2d(const dimension2d<U>& other) :
			Width((T)other.Width), Height((T)other.Height) { }

		template <class U>
		dimension2d<T>& operator=(const dimension2d<U>& other)
		{
			Width = (T)other.Width;
			Height = (T)other.Height;
			return *this;
		}


	
		bool operator==(const dimension2d<T>& other) const
		{
			return core::equals(Width, other.Width) &&
				core::equals(Height, other.Height);
		}

	 
		bool operator!=(const dimension2d<T>& other) const
		{
			return !(*this == other);
		}

		bool operator==(const Vector2& other) const; 

		bool operator!=(const Vector2& other) const
		{
			return !(*this == other);
		}

		 
		dimension2d<T>& set(const T& width, const T& height)
		{
			Width = width;
			Height = height;
			return *this;
		}
 
		dimension2d<T>& operator/=(const T& scale)
		{
			Width /= scale;
			Height /= scale;
			return *this;
		}

 
		dimension2d<T> operator/(const T& scale) const
		{
			return dimension2d<T>(Width / scale, Height / scale);
		}

	 
		dimension2d<T>& operator*=(const T& scale)
		{
			Width *= scale;
			Height *= scale;
			return *this;
		}

	 
		dimension2d<T> operator*(const T& scale) const
		{
			return dimension2d<T>(Width*scale, Height*scale);
		}

	 
		dimension2d<T>& operator+=(const dimension2d<T>& other)
		{
			Width += other.Width;
			Height += other.Height;
			return *this;
		}

 
		dimension2d<T> operator+(const dimension2d<T>& other) const
		{
			return dimension2d<T>(Width + other.Width, Height + other.Height);
		}

		 
		dimension2d<T>& operator-=(const dimension2d<T>& other)
		{
			Width -= other.Width;
			Height -= other.Height;
			return *this;
		}

	 
		dimension2d<T> operator-(const dimension2d<T>& other) const
		{
			return dimension2d<T>(Width - other.Width, Height - other.Height);
		}

		//! Get area
		T getArea() const
		{
			return Width*Height;
		}

		//! 通过一些属性获取优化大小
		/** 这个函数通常用于纹理矩形计算。这个函数返回一个更大伙更小的2的指数次幂的面积矩形
		\param requirePowerOfTwo 强制结果的值只用2次幂
		\param requireSquare 使结果长=宽
		\param larger 选择结果比当前是更大还是更小 
		\param maxValue Maximum 纹理尺寸，如果大于0面积被maxValue截断
		\return 在给定约束下的优化过矩形*/
		dimension2d<T> getOptimalSize(
			bool requirePowerOfTwo = true,
			bool requireSquare = false,
			bool larger = true,
			UINT32 maxValue = 0) const
		{
			UINT32 i = 1;
			UINT32 j = 1;
			if (requirePowerOfTwo)
			{
				while (i<(UINT32)Width)
					i <<= 1;
				if (!larger && i != 1 && i != (UINT32)Width)
					i >>= 1;
				while (j<(UINT32)Height)
					j <<= 1;
				if (!larger && j != 1 && j != (UINT32)Height)
					j >>= 1;
			}
			else
			{
				i = (UINT32)Width;
				j = (UINT32)Height;
			}

			if (requireSquare)
			{
				if ((larger && (i>j)) || (!larger && (i<j)))
					j = i;
				else
					i = j;
			}

			if (maxValue > 0 && i > maxValue)
				i = maxValue;

			if (maxValue > 0 && j > maxValue)
				j = maxValue;

			return dimension2d<T>((T)i, (T)j);
		}

		//! 获取插值的矩形
		/** \param 要插值的矩形
		\param d 值在0.0f 和 1.0f之间
		\return 插值的矩形 */
		dimension2d<T> getInterpolated(const dimension2d<T>& other, Real d) const
		{
			Real inv = (1.0f - d);
			return dimension2d<T>((T)(other.Width*inv + Width*d), (T)(other.Height*inv + Height*d));
		}


		 
		T Width;
	 
		T Height;
	};

	//!Real型dimension
	typedef dimension2d<Real> dimension2df;
	//! UINT32型dimension
	typedef dimension2d<UINT32> dimension2du;

	//! sint32型dimension
	typedef dimension2d<SINT32> dimension2di;
}

#endif
