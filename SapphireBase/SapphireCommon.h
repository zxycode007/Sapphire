#ifndef _SAPPHIRE_COMMON_
#define _SAPPHIRE_COMMON_

#include "SapphireString.h"

#if defined ( SAPPHIRE_GCC_VISIBILITY )
#   pragma GCC visibility push(default)
#endif

#include <utility>
#include <sstream>

#if defined ( SAPPHIRE_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

namespace Sapphire
{

	///快速通用的哈希算法
	uint32 _SapphireExport FastHash(const char * data, int len, uint32 hashSoFar = 0);
	///用boost::hash_combine的相同的类型合并哈希
	template <typename T>
	uint32 HashCombine(uint32 hashSoFar, const T& data)
	{
		return FastHash((const char*)&data, sizeof(T), hashSoFar);
	}

	/** 
	比较函数，用以深度或模板缓冲区操作和其它
	*/
	enum CompareFunction
	{
		CMPF_ALWAYS_FAIL,
		CMPF_ALWAYS_PASS,
		CMPF_LESS,
		CMPF_LESS_EQUAL,
		CMPF_EQUAL,
		CMPF_NOT_EQUAL,
		CMPF_GREATER_EQUAL,
		CMPF_GREATER
	};

	/** 
	高级别的滤波模式快捷选项
	设置minification, magnification 和 mip 滤波器
	 */
	enum TextureFilterOptions
	{
		/// min=FO_POINT, mag=FO_POINT, mip=FO_NONE
		TFO_NONE,
		///min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
		TFO_BILINEAR,
		/// min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
		TFO_TRILINEAR,
		/// min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
		TFO_ANISOTROPIC
	};

	/**
	滤波器类型
	*/
	enum FilterType
	{
		/// 用以收缩纹理的滤波器
		FT_MIN,
		/// 用以放大纹理的滤波器
		FT_MAG,
		/// 决定使用mipmap的滤波器
		FT_MIP
	};
	/** 对于纹理/MIPMAP滤波选项 */
	enum FilterOptions
	{
		/// 没有滤波器 
		FO_NONE,
		/// 用于最近的像素
		FO_POINT,
		/// 求2X2像素区域的平均值，对于MIN和MAG表示为双线性滤波，对于MIP是三线性滤波
		FO_LINEAR,
		/// 类似于FO_LINEAR,但是对纹理平面的角度进行补偿
		FO_ANISOTROPIC
	};

	/** 光照模式 */
	enum ShadeOptions
	{
		SO_FLAT,
		SO_GOURAUD,
		SO_PHONG
	};

	/** 雾化模式. */
	enum FogMode
	{
		///没有雾
		FOG_NONE,
		/// 雾的密度由相机处开始成倍增加 (fog = 1/e^(distance * density))
		FOG_EXP,
		///  雾的密度由相机处开始成倍增加  (fog = 1/e^(distance * density)^2)
		FOG_EXP2,
		/// 雾的密度由开始和结束处线性增加
		FOG_LINEAR
	};

	/** 基于顶点环绕的硬件剔除模式
	这个设置应用关系到怎样调用硬件API剔除三角形的设置 */
	enum CullingMode
	{
		/// 硬件不会剔除三角形并且渲染它接受的所有东西
		CULL_NONE = 1,
		/// 硬件剔除观察来顶点顺序按顺时针排列的三角形（默认）
		CULL_CLOCKWISE = 2,
		///  硬件剔除观察来顶点顺序逆顺时针排列的三角形
		CULL_ANTICLOCKWISE = 3
	};

	/** 
	基于顶点法线的手动剔除模式
	这个设置是怎样软件剔除三角形后再发送给硬件API。这个剔除模式用于场景管理，它会选择实现它。
	通常这些会处理大量的固定世界几何体，通常是平面的。（软件剔除会有非常大开销）
 */
	enum ManualCullingMode
	{
		/// 不进行剔除，所有东西都发送给硬件
		MANUAL_CULL_NONE = 1,
		/// 剔除三角形，它的法线方向远离相机（默认）
		MANUAL_CULL_BACK = 2,
		/// 剔除三角形，它的法线朝向相机
		MANUAL_CULL_FRONT = 3
	};

	/** 枚举可用的波类型 */
	enum WaveformType
	{
		/// 标准的正弦波，它平滑的从高到底的变化
		WFT_SINE,
		/// 用一个恒定速率增/减波峰的三角波
		WFT_TRIANGLE,
		/// 一半的时间在min，一半的世界在max的立即变换的方波
		WFT_SQUARE,
		/// 逐步稳步上升，从最短到最长期间和结束时的即时返回到最小的锯齿波
		WFT_SAWTOOTH,
		/// 反向锯齿波
		WFT_INVERSE_SAWTOOTH,
		//脉冲宽度调制。 就像WFT_SQUARE，除了高到低的转变是由周期
		/// 一个周期50%将于WFT_SQUARE方波
		WFT_PWM
	};

	/** 当光栅化时使用的多边形模式 */
	enum PolygonMode
	{
		/// 点模式
		PM_POINTS = 1,
		/// 线框模式
		PM_WIREFRAME = 2,
		/// 实体多边形模式
		PM_SOLID = 3
	};

	/** 阴影技术的枚举  */
	enum ShadowTechnique
	{
		/** 没有阴影 */
		SHADOWTYPE_NONE = 0x00,
		/** 
		阴影的附加的掩码(不直接使用，用 SHADOWTYPE_ enum替换)
		*/
		SHADOWDETAILTYPE_ADDITIVE = 0x01,
		/**
		阴影的调制的掩码(不直接使用，用 SHADOWTYPE_ enum替换)
		*/
		SHADOWDETAILTYPE_MODULATIVE = 0x02,
		/** 整合阴影的掩码 (不直接使用，用 SHADOWTYPE_ enum替换)
		*/
		SHADOWDETAILTYPE_INTEGRATED = 0x04,
		/** 模板阴影掩码 (不直接使用，用 SHADOWTYPE_ enum替换)
		*/
		SHADOWDETAILTYPE_STENCIL = 0x10,
		/** 纹理阴影的掩码 (不直接使用，用 SHADOWTYPE_ enum替换)
		*/
		SHADOWDETAILTYPE_TEXTURE = 0x20,

		/** 
		模板阴影技术，渲染所有的阴影体到在渲染后端所有非透明区域做一个调制。
		这个技术当多个光源时相对于附加模板阴影的方式少的多填充率。但是不是一个精确的模型
		*/
		SHADOWTYPE_STENCIL_MODULATIVE = 0x12,
		/** 
		模板阴影技术，渲染每个光源作为单独附加内容传递给场景。这个技术由于需要最少2次传递给整个场景所以需要非常大的
		附加值，如果有多个光源的话更多。然而，这和调制模板方式比较是非常精确模型，特别是在使用彩色光源和凹凸映射时特别明显
		*/
		SHADOWTYPE_STENCIL_ADDITIVE = 0x11,
		/** 
		基于纹理阴影技术，它包含一个单色渲染到纹理的阴影投射器和作为一个调制传递的阴影接收器的纹理的映射，
		这些阴影纹理的使用通过材质接收器完成。	
		*/
		SHADOWTYPE_TEXTURE_MODULATIVE = 0x22,

		/** 
		基于纹理阴影技术， 它包含一个单色渲染到纹理的阴影投射器和作为一个调制传递的阴影接收器的纹理的映射, 
		构建每个光源作为一个附加传递
	    这个技的需要非常大的填充率，因为它需要传递numLights + 2到整个场景。然而，它是比调制方式更精确
		特别是在使用彩色光源和凹凸映射时特别明显
		*/
		SHADOWTYPE_TEXTURE_ADDITIVE = 0x21,

		/**
		基于纹理阴影技术，它包含一个单色渲染到纹理的阴影投射器和作为一个调制传递的阴影接收器的纹理的映射，
		这些阴影纹理的使用通过材质接收器完成。
		这技术是所有技术中最容易灵活的，因为材质作者完全控制阴影如何与普通渲染进行合并。可以通过SHADOWTYPE_TEXTURE_ADDITIVE
		精确的使用阴影，但是需要更多性能开销。然而，它使用上需要更专业，在几乎所有的例子中，shader 支持硬件都可以完整的使用它.
		注意：'addtive'这个模式意味这这个渲染阴影纹理的颜色模式是黑色。这并不意味这它能够自动添加到你的接收器上，如何使用取决于你
		*/
		SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED = 0x25,
		/** 
		基于纹理阴影技术，它包含一个单色渲染到纹理的阴影投射器和作为一个调制传递的阴影接收器的纹理的映射，
		这些阴影纹理的使用通过材质接收器完成。
		这技术是所有技术中最容易灵活的，因为材质作者完全控制阴影如何与普通渲染进行合并。。可以通过SHADOWTYPE_TEXTURE_ADDITIVE
		精确的使用阴影，但是需要更多性能开销。然而，它使用上需要更专业，在几乎所有的例子中，shader 支持硬件都可以完整的使用它.
		注意：'modulative'这个模式意味这这个渲染阴影纹理的颜色模式是‘shadow colour’。这并不意味这它能够自动调制到你的接收器上，如何使用取决于你
		*/
		SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED = 0x26
	};

	/** 描述材质属性应该跟踪顶点色彩的枚举 */
	typedef int TrackVertexColourType;
	enum TrackVertexColourEnum {
		TVC_NONE = 0x0,
		TVC_AMBIENT = 0x1,
		TVC_DIFFUSE = 0x2,
		TVC_SPECULAR = 0x4,
		TVC_EMISSIVE = 0x8
	};

	/** billboard和粒子系统的排序模式*/
	enum SortMode
	{
		/** 按相机的方向排序 */
		SM_DIRECTION,
		/** 按相机的距离排序 */
		SM_DISTANCE
	};

	/** 定义帧缓冲区类型*/
	enum FrameBufferType {
		FBT_COLOUR = 0x1,
		FBT_DEPTH = 0x2,
		FBT_STENCIL = 0x4
	};

	/** 当计算每个batch理想的实例数量，实例管理器的标志 */
	enum InstanceManagerFlags
	{
		/** 
		强制每个batch的一个实例数量足够低，像这样vertices * numInst < 65535
		由于通常提高性能，在硬件实例化技术，这个标志是无效的
		*/
		IM_USE16BIT = 0x0001,

		/** 
		这个实例数量调整每个顶点纹理浪费少量像素的问题
		*/
		IM_VTFBESTFIT = 0x0002,

		/** 
		用一个限制数量的骨骼动画共享几乎所有的实例
		更新只有有限数量的顶点纹理中的动画
		*/
		IM_VTFBONEMATRIXLOOKUP = 0x0004,

		IM_USEBONEDUALQUATERNIONS = 0x0008,

		/** 每个顶点用一个权重(i.e. VTF). */
		IM_USEONEWEIGHT = 0x0010,

		/** 所有技术都强制在每一个顶点一个权重*/
		IM_FORCEONEWEIGHT = 0x0020,

		IM_USEALL = IM_USE16BIT | IM_VTFBESTFIT | IM_USEONEWEIGHT
	};


	/** 一个哈希向量
	对STD::Vector的封装
	*/
	template <typename T>
	class HashedVector
	{
	public:
		typedef std::vector<T, STLAllocator<T, GeneralAllocPolicy> > VectorImpl;   
	protected:
		VectorImpl mList;
		mutable uint32 mListHash;
		mutable bool mListHashDirty;   //hash

		void addToHash(const T& newPtr) const 
		{
			mListHash = FastHash((const char*)&newPtr, sizeof(T), mListHash);  //计算出哈希值
		}
		void recalcHash() const
		{
			mListHash = 0;
			for (const_iterator i = mList.begin(); i != mList.end(); ++i)  //重新哈希
				addToHash(*i);
			mListHashDirty = false;

		}

	public:
		typedef typename VectorImpl::value_type value_type;
		typedef typename VectorImpl::pointer pointer;
		typedef typename VectorImpl::reference reference;
		typedef typename VectorImpl::const_reference const_reference;
		typedef typename VectorImpl::size_type size_type;
		typedef typename VectorImpl::difference_type difference_type;
		typedef typename VectorImpl::iterator iterator;
		typedef typename VectorImpl::const_iterator const_iterator;
		typedef typename VectorImpl::reverse_iterator reverse_iterator;
		typedef typename VectorImpl::const_reverse_iterator const_reverse_iterator;

		void dirtyHash()
		{
			mListHashDirty = true;
		}
		bool isHashDirty() const
		{
			return mListHashDirty;
		}

		iterator begin()
		{
			// 需要假设哈希值需要重新计算量
			dirtyHash();
			return mList.begin();
		}
		iterator end() { return mList.end(); }
		const_iterator begin() const { return mList.begin(); }
		const_iterator end() const { return mList.end(); }
		reverse_iterator rbegin()
		{
			dirtyHash();
			return mList.rbegin();
		}
		reverse_iterator rend() { return mList.rend(); }
		const_reverse_iterator rbegin() const { return mList.rbegin(); }
		const_reverse_iterator rend() const { return mList.rend(); }
		size_type size() const { return mList.size(); }
		size_type max_size() const { return mList.max_size(); }
		size_type capacity() const { return mList.capacity(); }
		bool empty() const { return mList.empty(); }
		reference operator[](size_type n)
		{
			//假设哈希值需要重新计算
			dirtyHash();
			return mList[n];
		}
		const_reference operator[](size_type n) const { return mList[n]; }
		reference at(size_type n)
		{
			//假设哈希值需要重新计算
			dirtyHash();
			return mList.const_iterator(n);
		}
		const_reference at(size_type n) const { return mList.at(n); }
		HashedVector() : mListHash(0), mListHashDirty(false) {}
		HashedVector(size_type n) : mList(n), mListHash(0), mListHashDirty(n > 0) {}
		HashedVector(size_type n, const T& t) : mList(n, t), mListHash(0), mListHashDirty(n > 0) {}
		HashedVector(const HashedVector<T>& rhs)
			: mList(rhs.mList), mListHash(rhs.mListHash), mListHashDirty(rhs.mListHashDirty) {}

		template <class InputIterator>
		HashedVector(InputIterator a, InputIterator b)
			: mList(a, b), mListHashDirty(false)
		{
			dirtyHash();
		}

		~HashedVector() {}
		HashedVector<T>& operator=(const HashedVector<T>& rhs)
		{
			mList = rhs.mList;
			mListHash = rhs.mListHash;
			mListHashDirty = rhs.mListHashDirty;
			return *this;
		}

		void reserve(size_t t) { mList.reserve(t); }
		reference front()
		{
			//假设哈希值需要重新计算
			dirtyHash();
			return mList.front();
		}
		const_reference front() const { return mList.front(); }
		reference back()
		{
			//假设哈希值需要重新计算
			dirtyHash();
			return mList.back();
		}
		const_reference back() const { return mList.back(); }
		void push_back(const T& t)
		{
			mList.push_back(t);
			//快速处理哈希值添加
			if (!isHashDirty())
				addToHash(t);
		}
		void pop_back()
		{
			mList.pop_back();
			dirtyHash();
		}
		void swap(HashedVector<T>& rhs)
		{
			mList.swap(rhs.mList);
			dirtyHash();
		}
		iterator insert(iterator pos, const T& t)
		{
			bool recalc = (pos != end());
			iterator ret = mList.insert(pos, t);
			if (recalc)
				dirtyHash();
			else
				addToHash(t);
			return ret;
		}

		template <class InputIterator>
		void insert(iterator pos,
			InputIterator f, InputIterator l)
		{
			mList.insert(pos, f, l);
			dirtyHash();
		}

		void insert(iterator pos, size_type n, const T& x)
		{
			mList.insert(pos, n, x);
			dirtyHash();
		}

		iterator erase(iterator pos)
		{
			iterator ret = mList.erase(pos);
			dirtyHash();
			return ret;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator ret = mList.erase(first, last);
			dirtyHash();
			return ret;
		}
		void clear()
		{
			mList.clear();
			mListHash = 0;
			mListHashDirty = false;
		}

		void resize(size_type n, const T& t = T())
		{
			bool recalc = false;
			if (n != size())
				recalc = true;

			mList.resize(n, t);
			if (recalc)
				dirtyHash();
		}

		bool operator==(const HashedVector<T>& b)
		{
			return mListHash == b.mListHash;
		}

		bool operator<(const HashedVector<T>& b)
		{
			return mListHash < b.mListHash;
		}


		///获取哈希值
		uint32 getHash() const
		{
			if (isHashDirty())
				recalcHash();

			return mListHash;
		}
	public:



	};

	class Light;
	typedef HashedVector<Light*> LightList;

	//一元选项列表
	typedef map<String, bool>::type UnaryOptionList;  
	//二元选项列表
	typedef map<String, String>::type BinaryOptionList;
	
	/// 名字 / 值 参数对 (first = name, second = value)
	typedef map<String, String>::type NameValuePairList;

	/// Alias / Texture name pair (first = alias, second = texture name)
	typedef map<String, String>::type AliasTextureNamePairList;

	//T矩形区域，用来表示文本矩形区域 
	template< typename T > struct TRect
	{
		T left, top, right, bottom;
		TRect() : left(0), top(0), right(0), bottom(0) {}
		TRect(T const & l, T const & t, T const & r, T const & b)
			: left(l), top(t), right(r), bottom(b)
		{
		}
		TRect(TRect const & o)
			: left(o.left), top(o.top), right(o.right), bottom(o.bottom)
		{
		}
		TRect & operator=(TRect const & o)
		{
			left = o.left;
			top = o.top;
			right = o.right;
			bottom = o.bottom;
			return *this;
		}
		T width() const
		{
			return right - left;
		}
		T height() const
		{
			return bottom - top;
		}
		bool isNull() const
		{
			return width() == 0 || height() == 0;
		}
		void setNull()
		{
			left = right = top = bottom = 0;
		}
		TRect & merge(const TRect& rhs)
		{
			if (isNull())
			{
				*this = rhs;
			}
			else if (!rhs.isNull())
			{
				left = std::min(left, rhs.left);
				right = std::max(right, rhs.right);
				top = std::min(top, rhs.top);
				bottom = std::max(bottom, rhs.bottom);
			}

			return *this;

		}
		TRect intersect(const TRect& rhs) const
		{
			TRect ret;
			if (isNull() || rhs.isNull())
			{
				
				return ret;
			}
			else
			{
				ret.left = std::max(left, rhs.left);
				ret.right = std::min(right, rhs.right);
				ret.top = std::max(top, rhs.top);
				ret.bottom = std::min(bottom, rhs.bottom);
			}

			if (ret.left > ret.right || ret.top > ret.bottom)
			{
				// 不相交，返回空
				ret.left = ret.top = ret.right = ret.bottom = 0;
			}

			return ret;

		}

	};


	template<typename T>
	std::ostream& operator<<(std::ostream& o, const TRect<T>& r)
	{
		o << "TRect<>(l:" << r.left << ", t:" << r.top << ", r:" << r.right << ", b:" << r.bottom << ")";
		return o;
	}

	/** 
	用于定义一个2D单精度浮点型矩形空间
	*/
	typedef TRect<float> FloatRect;

	/** 
	用于定义一个2D Real（单/双精度）型矩形空间
	*/
	typedef TRect<Real> RealRect;
	/**
	用于定义一个2D整形矩形空间
	*/
	typedef TRect< long > Rect;

	/**
	用于定义一个3d 整形的盒子空间
	注意： 左，顶和前边已被包括了，但右，顶，后边没有
	*/
	struct Box
	{
		size_t left, top, right, bottom, front, back;
		///
		Box()
			: left(0), top(0), right(1), bottom(1), front(0), back(1)
		{
		}
		/** 定义盒子的左边，顶部，右边和底部坐标
		这个盒子将有深度(front=0 and back=1).
		@param	l	x 左边缘的值
		@param	t	y 上边缘的值
		@param	r	x 右边缘的值
		@param	b	y 下边缘的值
		@note注意： 左，顶和前边已被包括了，但右，顶，后边没有
		*/
		Box(size_t l, size_t t, size_t r, size_t b) :
			left(l),
			top(t),
			right(r),
			bottom(b),
			front(0),
			back(1)
		{
			assert(right >= left && bottom >= top && back >= front);
		}
		/** 定义盒子的左边，顶部，前面，右边和底部，后面坐标
		这个盒子将有深度(front=0 and back=1).
		@param	l	x 左边缘的值
		@param	t	y 上边缘的值
		@param  ff  z 前边缘的值
		@param	r	x 右边缘的值
		@param	b	y 下边缘的值
		@param  bb  z 后边缘的值
		@note注意： 左，顶和前边已被包括了，但右，顶，后边没有
		*/
		Box(size_t l, size_t t, size_t ff, size_t r, size_t b, size_t bb) :
			left(l),
			top(t),
			right(r),
			bottom(b),
			front(ff),
			back(bb)
		{
			assert(right >= left && bottom >= top && back >= front);
		}

		/// 返回另一个盒子是否是这个的一部分
		bool contains(const Box &def) const
		{
			return (def.left >= left && def.top >= top && def.front >= front &&
				def.right <= right && def.bottom <= bottom && def.back <= back);
		}

		/// 获取盒子宽度
		size_t getWidth() const { return right - left; }
		///获取盒子高度
		size_t getHeight() const { return bottom - top; }
		/// 获取盒子深度
		size_t getDepth() const { return back - front; }
	};


	/** 
	本地由一元 '-blah'和二元'-blah foo'的命令行选项，传递到后面非选项的索引
	@param numargs, argv 标准的传入main函数的参数
	@param unaryOptList 一元选项的Map列表 (i.e. 这些不需要参数).只需key，而不需要value 例如'-e'
	@param binOptList 二元选项的Map列表 (i.e. 这些需要参数) 例如'-e afile.txt'
	应该预先分配，例如'-e'和默认设置
	*/
	int _SapphireExport findCommandLineOpts(int numargs, char** argv, UnaryOptionList& unaryOptList,
		BinaryOptionList& binOptList);

	///通用的剪裁结果
	enum ClipResult
	{
		/// 不剪裁
		CLIPPED_NONE = 0,
		/// 部分剪裁
		CLIPPED_SOME = 1,
		/// 对全部剪裁
		CLIPPED_ALL = 2
	};

	/// 渲染窗口创建参数
	struct RenderWindowDescription
	{
		String				name;  //窗口名
		unsigned int		width;  //宽度
		unsigned int		height;  //高度
		bool				useFullScreen;  //是否全屏
		NameValuePairList	miscParams;     //杂项
	};



	/// 渲染窗口创建参数列表容器
	typedef vector<RenderWindowDescription>::type RenderWindowDescriptionList;

	/// 渲染窗口容器
	//typedef vector<RenderWindow*>::type RenderWindowList;

	/// 创建一个按数值顺序化名字的工具类 
	class _SapphireExport NameGenerator
	{
	protected:
		String mPrefix;
		unsigned long long int mNext;
		SAPPHIRE_AUTO_MUTEX
	public:
		NameGenerator(const NameGenerator& rhs)
			: mPrefix(rhs.mPrefix), mNext(rhs.mNext) {}

		NameGenerator(const String& prefix) : mPrefix(prefix), mNext(1) {}

		/// 创建一个新名字
		String generate()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
			//	std::ostringstream s;
			//s << mPrefix << mNext++;
			StringUtil::StrStreamType ss;
			ss << mPrefix << mNext++;
			return ss.str();
		}

		/// 重置内部计数器
		void reset()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mNext = 1ULL;
		}

		/// 手动设置内部计数器(要谨慎)
		void setNext(unsigned long long int val)
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mNext = val;
		}

		/// 取得内部计数器
		unsigned long long int getNext() const
		{
			// 取得时锁住，因为64位不支持原子操作
			SAPPHIRE_LOCK_AUTO_MUTEX
				return mNext;
		}




	};


	/** 
	描述一个简单对象池的模板类
	*/
	template <typename T>
	class Pool
	{
	protected:
		typedef typename list<T>::type ItemList;
		ItemList mItems;
		SAPPHIRE_AUTO_MUTEX
	public:
		Pool() {}
		virtual ~Pool() {}

		/** 
		从池中获取下一个对象
		@return 返回一个指示是否要释放它
		*/
		virtual std::pair<bool, T> removeItem()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				std::pair<bool, T> ret;
			if (mItems.empty())
			{
				ret.first = false;
			}
			else
			{
				ret.first = true;
				ret.second = mItems.front();
				mItems.pop_front();
			}
			return ret;
		}

		/** 
		添加一个新项目到池中
		*/
		virtual void addItem(const T& i)
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mItems.push_front(i);
		}
		/// 清空池
		virtual void clear()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mItems.clear();
		}



	};

}

#endif