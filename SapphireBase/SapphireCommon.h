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

	///����ͨ�õĹ�ϣ�㷨
	uint32 _SapphireExport FastHash(const char * data, int len, uint32 hashSoFar = 0);
	///��boost::hash_combine����ͬ�����ͺϲ���ϣ
	template <typename T>
	uint32 HashCombine(uint32 hashSoFar, const T& data)
	{
		return FastHash((const char*)&data, sizeof(T), hashSoFar);
	}

	/** 
	�ȽϺ�����������Ȼ�ģ�建��������������
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
	�߼�����˲�ģʽ���ѡ��
	����minification, magnification �� mip �˲���
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
	�˲�������
	*/
	enum FilterType
	{
		/// ��������������˲���
		FT_MIN,
		/// ���ԷŴ�������˲���
		FT_MAG,
		/// ����ʹ��mipmap���˲���
		FT_MIP
	};
	/** ��������/MIPMAP�˲�ѡ�� */
	enum FilterOptions
	{
		/// û���˲��� 
		FO_NONE,
		/// �������������
		FO_POINT,
		/// ��2X2���������ƽ��ֵ������MIN��MAG��ʾΪ˫�����˲�������MIP���������˲�
		FO_LINEAR,
		/// ������FO_LINEAR,���Ƕ�����ƽ��ĽǶȽ��в���
		FO_ANISOTROPIC
	};

	/** ����ģʽ */
	enum ShadeOptions
	{
		SO_FLAT,
		SO_GOURAUD,
		SO_PHONG
	};

	/** ��ģʽ. */
	enum FogMode
	{
		///û����
		FOG_NONE,
		/// ����ܶ����������ʼ�ɱ����� (fog = 1/e^(distance * density))
		FOG_EXP,
		///  ����ܶ����������ʼ�ɱ�����  (fog = 1/e^(distance * density)^2)
		FOG_EXP2,
		/// ����ܶ��ɿ�ʼ�ͽ�������������
		FOG_LINEAR
	};

	/** ���ڶ��㻷�Ƶ�Ӳ���޳�ģʽ
	�������Ӧ�ù�ϵ����������Ӳ��API�޳������ε����� */
	enum CullingMode
	{
		/// Ӳ�������޳������β�����Ⱦ�����ܵ����ж���
		CULL_NONE = 1,
		/// Ӳ���޳��۲�������˳��˳ʱ�����е������Σ�Ĭ�ϣ�
		CULL_CLOCKWISE = 2,
		///  Ӳ���޳��۲�������˳����˳ʱ�����е�������
		CULL_ANTICLOCKWISE = 3
	};

	/** 
	���ڶ��㷨�ߵ��ֶ��޳�ģʽ
	�����������������޳������κ��ٷ��͸�Ӳ��API������޳�ģʽ���ڳ�����������ѡ��ʵ������
	ͨ����Щ�ᴦ������Ĺ̶����缸���壬ͨ����ƽ��ġ�������޳����зǳ�������
 */
	enum ManualCullingMode
	{
		/// �������޳������ж��������͸�Ӳ��
		MANUAL_CULL_NONE = 1,
		/// �޳������Σ����ķ��߷���Զ�������Ĭ�ϣ�
		MANUAL_CULL_BACK = 2,
		/// �޳������Σ����ķ��߳������
		MANUAL_CULL_FRONT = 3
	};

	/** ö�ٿ��õĲ����� */
	enum WaveformType
	{
		/// ��׼�����Ҳ�����ƽ���ĴӸߵ��׵ı仯
		WFT_SINE,
		/// ��һ���㶨������/����������ǲ�
		WFT_TRIANGLE,
		/// һ���ʱ����min��һ���������max�������任�ķ���
		WFT_SQUARE,
		/// ���Ȳ�����������̵���ڼ�ͽ���ʱ�ļ�ʱ���ص���С�ľ�ݲ�
		WFT_SAWTOOTH,
		/// �����ݲ�
		WFT_INVERSE_SAWTOOTH,
		//�����ȵ��ơ� ����WFT_SQUARE�����˸ߵ��͵�ת����������
		/// һ������50%����WFT_SQUARE����
		WFT_PWM
	};

	/** ����դ��ʱʹ�õĶ����ģʽ */
	enum PolygonMode
	{
		/// ��ģʽ
		PM_POINTS = 1,
		/// �߿�ģʽ
		PM_WIREFRAME = 2,
		/// ʵ������ģʽ
		PM_SOLID = 3
	};

	/** ��Ӱ������ö��  */
	enum ShadowTechnique
	{
		/** û����Ӱ */
		SHADOWTYPE_NONE = 0x00,
		/** 
		��Ӱ�ĸ��ӵ�����(��ֱ��ʹ�ã��� SHADOWTYPE_ enum�滻)
		*/
		SHADOWDETAILTYPE_ADDITIVE = 0x01,
		/**
		��Ӱ�ĵ��Ƶ�����(��ֱ��ʹ�ã��� SHADOWTYPE_ enum�滻)
		*/
		SHADOWDETAILTYPE_MODULATIVE = 0x02,
		/** ������Ӱ������ (��ֱ��ʹ�ã��� SHADOWTYPE_ enum�滻)
		*/
		SHADOWDETAILTYPE_INTEGRATED = 0x04,
		/** ģ����Ӱ���� (��ֱ��ʹ�ã��� SHADOWTYPE_ enum�滻)
		*/
		SHADOWDETAILTYPE_STENCIL = 0x10,
		/** ������Ӱ������ (��ֱ��ʹ�ã��� SHADOWTYPE_ enum�滻)
		*/
		SHADOWDETAILTYPE_TEXTURE = 0x20,

		/** 
		ģ����Ӱ��������Ⱦ���е���Ӱ�嵽����Ⱦ������з�͸��������һ�����ơ�
		��������������Դʱ����ڸ���ģ����Ӱ�ķ�ʽ�ٵĶ�����ʡ����ǲ���һ����ȷ��ģ��
		*/
		SHADOWTYPE_STENCIL_MODULATIVE = 0x12,
		/** 
		ģ����Ӱ��������Ⱦÿ����Դ��Ϊ�����������ݴ��ݸ��������������������Ҫ����2�δ��ݸ���������������Ҫ�ǳ����
		����ֵ������ж����Դ�Ļ����ࡣȻ������͵���ģ�巽ʽ�Ƚ��Ƿǳ���ȷģ�ͣ��ر�����ʹ�ò�ɫ��Դ�Ͱ�͹ӳ��ʱ�ر�����
		*/
		SHADOWTYPE_STENCIL_ADDITIVE = 0x11,
		/** 
		����������Ӱ������������һ����ɫ��Ⱦ���������ӰͶ��������Ϊһ�����ƴ��ݵ���Ӱ�������������ӳ�䣬
		��Щ��Ӱ�����ʹ��ͨ�����ʽ�������ɡ�	
		*/
		SHADOWTYPE_TEXTURE_MODULATIVE = 0x22,

		/** 
		����������Ӱ������ ������һ����ɫ��Ⱦ���������ӰͶ��������Ϊһ�����ƴ��ݵ���Ӱ�������������ӳ��, 
		����ÿ����Դ��Ϊһ�����Ӵ���
	    ���������Ҫ�ǳ��������ʣ���Ϊ����Ҫ����numLights + 2������������Ȼ�������Ǳȵ��Ʒ�ʽ����ȷ
		�ر�����ʹ�ò�ɫ��Դ�Ͱ�͹ӳ��ʱ�ر�����
		*/
		SHADOWTYPE_TEXTURE_ADDITIVE = 0x21,

		/**
		����������Ӱ������������һ����ɫ��Ⱦ���������ӰͶ��������Ϊһ�����ƴ��ݵ���Ӱ�������������ӳ�䣬
		��Щ��Ӱ�����ʹ��ͨ�����ʽ�������ɡ�
		�⼼�������м��������������ģ���Ϊ����������ȫ������Ӱ�������ͨ��Ⱦ���кϲ�������ͨ��SHADOWTYPE_TEXTURE_ADDITIVE
		��ȷ��ʹ����Ӱ��������Ҫ�������ܿ�����Ȼ������ʹ������Ҫ��רҵ���ڼ������е������У�shader ֧��Ӳ��������������ʹ����.
		ע�⣺'addtive'���ģʽ��ζ�������Ⱦ��Ӱ�������ɫģʽ�Ǻ�ɫ���Ⲣ����ζ�����ܹ��Զ���ӵ���Ľ������ϣ����ʹ��ȡ������
		*/
		SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED = 0x25,
		/** 
		����������Ӱ������������һ����ɫ��Ⱦ���������ӰͶ��������Ϊһ�����ƴ��ݵ���Ӱ�������������ӳ�䣬
		��Щ��Ӱ�����ʹ��ͨ�����ʽ�������ɡ�
		�⼼�������м��������������ģ���Ϊ����������ȫ������Ӱ�������ͨ��Ⱦ���кϲ���������ͨ��SHADOWTYPE_TEXTURE_ADDITIVE
		��ȷ��ʹ����Ӱ��������Ҫ�������ܿ�����Ȼ������ʹ������Ҫ��רҵ���ڼ������е������У�shader ֧��Ӳ��������������ʹ����.
		ע�⣺'modulative'���ģʽ��ζ�������Ⱦ��Ӱ�������ɫģʽ�ǡ�shadow colour�����Ⲣ����ζ�����ܹ��Զ����Ƶ���Ľ������ϣ����ʹ��ȡ������
		*/
		SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED = 0x26
	};

	/** ������������Ӧ�ø��ٶ���ɫ�ʵ�ö�� */
	typedef int TrackVertexColourType;
	enum TrackVertexColourEnum {
		TVC_NONE = 0x0,
		TVC_AMBIENT = 0x1,
		TVC_DIFFUSE = 0x2,
		TVC_SPECULAR = 0x4,
		TVC_EMISSIVE = 0x8
	};

	/** billboard������ϵͳ������ģʽ*/
	enum SortMode
	{
		/** ������ķ������� */
		SM_DIRECTION,
		/** ������ľ������� */
		SM_DISTANCE
	};

	/** ����֡����������*/
	enum FrameBufferType {
		FBT_COLOUR = 0x1,
		FBT_DEPTH = 0x2,
		FBT_STENCIL = 0x4
	};

	/** ������ÿ��batch�����ʵ��������ʵ���������ı�־ */
	enum InstanceManagerFlags
	{
		/** 
		ǿ��ÿ��batch��һ��ʵ�������㹻�ͣ�������vertices * numInst < 65535
		����ͨ��������ܣ���Ӳ��ʵ���������������־����Ч��
		*/
		IM_USE16BIT = 0x0001,

		/** 
		���ʵ����������ÿ�����������˷��������ص�����
		*/
		IM_VTFBESTFIT = 0x0002,

		/** 
		��һ�����������Ĺ����������������е�ʵ��
		����ֻ�����������Ķ��������еĶ���
		*/
		IM_VTFBONEMATRIXLOOKUP = 0x0004,

		IM_USEBONEDUALQUATERNIONS = 0x0008,

		/** ÿ��������һ��Ȩ��(i.e. VTF). */
		IM_USEONEWEIGHT = 0x0010,

		/** ���м�����ǿ����ÿһ������һ��Ȩ��*/
		IM_FORCEONEWEIGHT = 0x0020,

		IM_USEALL = IM_USE16BIT | IM_VTFBESTFIT | IM_USEONEWEIGHT
	};


	/** һ����ϣ����
	��STD::Vector�ķ�װ
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
			mListHash = FastHash((const char*)&newPtr, sizeof(T), mListHash);  //�������ϣֵ
		}
		void recalcHash() const
		{
			mListHash = 0;
			for (const_iterator i = mList.begin(); i != mList.end(); ++i)  //���¹�ϣ
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
			// ��Ҫ�����ϣֵ��Ҫ���¼�����
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
			//�����ϣֵ��Ҫ���¼���
			dirtyHash();
			return mList[n];
		}
		const_reference operator[](size_type n) const { return mList[n]; }
		reference at(size_type n)
		{
			//�����ϣֵ��Ҫ���¼���
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
			//�����ϣֵ��Ҫ���¼���
			dirtyHash();
			return mList.front();
		}
		const_reference front() const { return mList.front(); }
		reference back()
		{
			//�����ϣֵ��Ҫ���¼���
			dirtyHash();
			return mList.back();
		}
		const_reference back() const { return mList.back(); }
		void push_back(const T& t)
		{
			mList.push_back(t);
			//���ٴ����ϣֵ���
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


		///��ȡ��ϣֵ
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

	//һԪѡ���б�
	typedef map<String, bool>::type UnaryOptionList;  
	//��Ԫѡ���б�
	typedef map<String, String>::type BinaryOptionList;
	
	/// ���� / ֵ ������ (first = name, second = value)
	typedef map<String, String>::type NameValuePairList;

	/// Alias / Texture name pair (first = alias, second = texture name)
	typedef map<String, String>::type AliasTextureNamePairList;

	//T��������������ʾ�ı��������� 
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
				// ���ཻ�����ؿ�
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
	���ڶ���һ��2D�����ȸ����;��οռ�
	*/
	typedef TRect<float> FloatRect;

	/** 
	���ڶ���һ��2D Real����/˫���ȣ��;��οռ�
	*/
	typedef TRect<Real> RealRect;
	/**
	���ڶ���һ��2D���ξ��οռ�
	*/
	typedef TRect< long > Rect;

	/**
	���ڶ���һ��3d ���εĺ��ӿռ�
	ע�⣺ �󣬶���ǰ���ѱ������ˣ����ң��������û��
	*/
	struct Box
	{
		size_t left, top, right, bottom, front, back;
		///
		Box()
			: left(0), top(0), right(1), bottom(1), front(0), back(1)
		{
		}
		/** ������ӵ���ߣ��������ұߺ͵ײ�����
		������ӽ������(front=0 and back=1).
		@param	l	x ���Ե��ֵ
		@param	t	y �ϱ�Ե��ֵ
		@param	r	x �ұ�Ե��ֵ
		@param	b	y �±�Ե��ֵ
		@noteע�⣺ �󣬶���ǰ���ѱ������ˣ����ң��������û��
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
		/** ������ӵ���ߣ�������ǰ�棬�ұߺ͵ײ�����������
		������ӽ������(front=0 and back=1).
		@param	l	x ���Ե��ֵ
		@param	t	y �ϱ�Ե��ֵ
		@param  ff  z ǰ��Ե��ֵ
		@param	r	x �ұ�Ե��ֵ
		@param	b	y �±�Ե��ֵ
		@param  bb  z ���Ե��ֵ
		@noteע�⣺ �󣬶���ǰ���ѱ������ˣ����ң��������û��
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

		/// ������һ�������Ƿ��������һ����
		bool contains(const Box &def) const
		{
			return (def.left >= left && def.top >= top && def.front >= front &&
				def.right <= right && def.bottom <= bottom && def.back <= back);
		}

		/// ��ȡ���ӿ��
		size_t getWidth() const { return right - left; }
		///��ȡ���Ӹ߶�
		size_t getHeight() const { return bottom - top; }
		/// ��ȡ�������
		size_t getDepth() const { return back - front; }
	};


	/** 
	������һԪ '-blah'�Ͷ�Ԫ'-blah foo'��������ѡ����ݵ������ѡ�������
	@param numargs, argv ��׼�Ĵ���main�����Ĳ���
	@param unaryOptList һԪѡ���Map�б� (i.e. ��Щ����Ҫ����).ֻ��key��������Ҫvalue ����'-e'
	@param binOptList ��Ԫѡ���Map�б� (i.e. ��Щ��Ҫ����) ����'-e afile.txt'
	Ӧ��Ԥ�ȷ��䣬����'-e'��Ĭ������
	*/
	int _SapphireExport findCommandLineOpts(int numargs, char** argv, UnaryOptionList& unaryOptList,
		BinaryOptionList& binOptList);

	///ͨ�õļ��ý��
	enum ClipResult
	{
		/// ������
		CLIPPED_NONE = 0,
		/// ���ּ���
		CLIPPED_SOME = 1,
		/// ��ȫ������
		CLIPPED_ALL = 2
	};

	/// ��Ⱦ���ڴ�������
	struct RenderWindowDescription
	{
		String				name;  //������
		unsigned int		width;  //���
		unsigned int		height;  //�߶�
		bool				useFullScreen;  //�Ƿ�ȫ��
		NameValuePairList	miscParams;     //����
	};



	/// ��Ⱦ���ڴ��������б�����
	typedef vector<RenderWindowDescription>::type RenderWindowDescriptionList;

	/// ��Ⱦ��������
	//typedef vector<RenderWindow*>::type RenderWindowList;

	/// ����һ������ֵ˳�����ֵĹ����� 
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

		/// ����һ��������
		String generate()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
			//	std::ostringstream s;
			//s << mPrefix << mNext++;
			StringUtil::StrStreamType ss;
			ss << mPrefix << mNext++;
			return ss.str();
		}

		/// �����ڲ�������
		void reset()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mNext = 1ULL;
		}

		/// �ֶ������ڲ�������(Ҫ����)
		void setNext(unsigned long long int val)
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mNext = val;
		}

		/// ȡ���ڲ�������
		unsigned long long int getNext() const
		{
			// ȡ��ʱ��ס����Ϊ64λ��֧��ԭ�Ӳ���
			SAPPHIRE_LOCK_AUTO_MUTEX
				return mNext;
		}




	};


	/** 
	����һ���򵥶���ص�ģ����
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
		�ӳ��л�ȡ��һ������
		@return ����һ��ָʾ�Ƿ�Ҫ�ͷ���
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
		���һ������Ŀ������
		*/
		virtual void addItem(const T& i)
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mItems.push_front(i);
		}
		/// ��ճ�
		virtual void clear()
		{
			SAPPHIRE_LOCK_AUTO_MUTEX
				mItems.clear();
		}



	};

}

#endif