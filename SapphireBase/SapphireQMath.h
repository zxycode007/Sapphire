#ifndef _SAPPHIRE_Q_MATH_
#define _SAPPHIRE_Q_MATH_

#include "SapphirePrerequisites.h"


/**

  c的数学计算函数库

  */


//////////////////////////数学常量//////////////////////////////////
//PI相关
/**
@brief
定义圆周率宏
*/
#define PI         ((float)3.141592654f)
/**
@brief
定义 圆周率 × 2
*/
#define PI2        ((float)6.283185307f)
/**
@brief
定义圆周率/2
*/
#define PI_DIV_2   ((float)1.570796327f)
/**
@brief
定义圆周率/4
*/
#define PI_DIV_4   ((float)0.785398163f) 
/**
@brief
定义圆周率的倒数
*/
#define PI_INV     ((float)0.318309886f) 

//极小数ε
/**
@brief
ε到小数点后3位的
*/
#define EPSILON_E3 (float)(1E-3) 
/**
@brief
ε到小数点后4位的
*/
#define EPSILON_E4 (float)(1E-4)
/**
@brief
ε到小数点后5位的
*/
#define EPSILON_E5 (float)(1E-5)
/**
@brief
ε到小数点后6位的
*/
#define EPSILON_E6 (float)(1E-6)
/**
@brief
ε到小数点后7位的
*/
#define EPSILON_E7  (double)(1E-7)
/**
@brief
ε到小数点后8位的
*/
#define EPSILON_E8  (double)(1E-8)

//////////////////////////////////////////通用宏/////////////////////////////////////////
//判断大小,去较大或较小值
/**
@brief 取两者最小值
@param a 参数a
@param b 参数b
@return a，b中的最小值
*/
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
/**
@brief 取两者最大值
@param a 参数a
@param b 参数b
@return a，b中的最大值
*/
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
//交互变量的值
/**
@brief 互换变量的值
@param a 变量a
@param b 变量b
@param t 临时变量
*/
#define SWAP(a,b,t) {t=a; a=b; b=t;}
//角度弧度转换
/**
@brief 角度弧度转换
@param angle 角度值
@return angle对应的弧度值
*/
#define DEG_TO_RAD(angle) ((angle)*PI/180.0)
/**
@brief 弧度角度转换
@param radian 弧度值
@return radian对应的角度值
*/
#define RAD_TO_DEG(radian) ((radian)*180.0/PI)
//x到y范围随机
/**
@brief 返回一个x到y以内的范围随机整数
@param x 下限
@param y 上限
@return 随机数
*/
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
/**
@brief 浮点数比较宏
如果A<B值为1，否则为0
@param 浮点数A
@param 浮点数B
@return 比较结果
*/
#define FCMP(a,b) ((fabs(a-b) <EPSILON_E3)?1:0)
/**
@brief 查表法快速计算反余弦
因为数组下标不能为浮点型和负数，所以将索引域范围[-1,1] 平移1位 =  [0,2],并将扩大180倍，将[-1,1]分为360份,单位步进到0.01
@param  x取值在[-1,1]之间，|x| >= 0.01
*/
#define FAST_INV_COS(x) (dp_inverse_cos[(int)(((float)x+1)*(float)180)])



/*
float IEEE-754 bit 表示法
0      0x00000000
1.0    0x3f800000
0.5    0x3f000000
3      0x40400000
+inf   0x7f800000
-inf   0xff800000
+NaN   0x7fc00000 or 0x7ff00000
in 通用: number = (符号位 ? -1:1) * 2^(指数) * 1.(尾数位)
*/

typedef union{ unsigned int u; int s; float f; } inttofloat;


//! 浮点数表示一个整形值
#if (SAPPHIRE_FAST_MATH==0)
#define FR(x)                           ((FLOAT32&)(x))
#else
inline FLOAT32 FR(UINT32 x) { inttofloat tmp; tmp.u = x; return tmp.f; }
inline FLOAT32 FR(SINT32 x) { inttofloat tmp; tmp.s = x; return tmp.f; }
#endif


//! 整形表示为一个浮点型
#if (SAPPHIRE_FAST_MATH==0)
#define IR(x)                           ((UINT32&)(x))
#else
inline UINT32 IR(FLOAT32 x) { inttofloat tmp; tmp.f = x; return tmp.u; }
#endif













// 存放0~369度的正弦和余弦表
/**
@brief 余弦表
*/
extern float cos_look[361];
/**
@brief 正弦表
*/
extern float sin_look[361];
/**
@brief 反余弦表
反余弦值域为0~pi， 将[-1,1]的值映射到0~180度
为提高精度，将180度分为360等份，用0.5度为单位步进
之所以定义362是为了防止由于浮点数溢出导致数组访问越界
*/
extern float dp_inverse_cos[360 + 2];

/**
@brief 512内2为底的对数表
*/
extern unsigned char logbase2ofx[513];


float Fast_Sin(float theta);


float Fast_Cos(float theta);



int FastLog2(int x);



int Fast_Distance_2D(int x, int y);



float  Fast_Distance_3D(float fx, float fy, float fz);



int	Q_rand(int *seed);

float Q_random(int *seed);

float Q_crandom(int *seed);


// 返回一个随机数
float asm_rand();


// 返回随机数的最大值
float asm_rand_max();


// 返回 1 / a 对于 a * a = r
// -- Use this for Vector normalisation!!!
float asm_rsq(float r);



/**
@bire 截断char类型的范围
*/
signed char ClampChar(int i);
/**
@bire 截断short类型的范围
*/
signed short ClampShort(int i);

/*
快速求绝对值
*/
float Q_fabs(float f);

/*
 快速求平方根倒数
 */
float Q_rsqrt(float number);

//快速求平方根倒数（INTEL硬件版）
FORCEINLINE float Q_rsqrtHW(const float f)
{
#if defined(_MSC_VER)
	// SSE 平方根倒数估算法 精确到有符号的12位尾数
	float recsqrt;
	__asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
	__asm movss recsqrt, xmm0       // return xmm0
	return recsqrt;

	/*
	//来自于Nvidia
	u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
	f32 y = *(f32*)&tmp;
	return y * (1.47f - 0.47f * x * y * y);
	*/
#else
	return 1.f / sqrtf(f);
#endif
}


inline float fract(float x)
{
	return x - floorf(x);
}

// 内联函数 //////////////////////////////////////////

inline void Mem_Set_WORD(void *dest, unsigned short data, int count)
{
	//该函数一次填充16位字节内存

	_asm
	{
		mov edi, dest; //EDI指针指向目标内存地址 
		mov ecx, count; //将移动次数放入ECX 
		mov ax, data; //将16位数据放入AX
		rep stosw; //rep指令是重复上述操作,直到ECX为0，每执行一次ECX-1；STOS是将EAX值拷贝到ES:edi指向的地址; 如果设置了direction flag, 那么edi会在该指令执行后减小, 如果没有设置direction flag, 那么edi的值会增加.
	}

}

///////////////////////////////////////////////////////////

inline void Mem_Set_QUAD(void *dest, unsigned int data, int count)
{
	// 该函数一次填充32位字节内存


	_asm
	{
		mov edi, dest;
		mov ecx, count;
		mov eax, data;
		rep stosd;
	}

}




namespace Sapphire
{
	inline float32 round_(float32 x)
	{
		return floorf(x + 0.5f);
	}


	FORCEINLINE SINT32 floor32(float32 x)
	{
#if (SAPPHIRE_FAST_MATH)
		const float32 h = 0.5f;

		SINT32 t;

#if defined(_MSC_VER)
		__asm
		{
			fld	x
				fsub	h
				fistp	t
		}
#elif defined(__GNUC__)
		__asm__ __volatile__(
			"fsub %2 \n\t"
			"fistpl %0"
			: "=m" (t)
			: "t" (x), "f" (h)
			: "st"
			);
#else
		#  warn SAPPHIRE_FAST_MATH not supported.
		return (SINT32)floorf(x);
#endif
		return t;
#else // 非快速
		return (SINT32)floorf(x);
#endif
	}


	FORCEINLINE SINT32 round32(float32 x)
	{
#if (SAPPHIRE_FAST_MATH)
		SINT32 t;

#if defined(_MSC_VER)
		__asm
		{
			fld   x
				fistp t
		}
#elif defined(__GNUC__)
		__asm__ __volatile__(
			"fistpl %0 \n\t"
			: "=m"(t)
			: "t"(x)
			: "st"
			);
#else
		#  warn SAPPHIRE_FAST_MATH not supported.
		return (SINT32)round_(x);
#endif
		return t;
#else //非快速
		return (SINT32)round_(x);
#endif
	}



	FORCEINLINE SINT32 ceil32(float32 x)
	{
#if (SAPPHIRE_FAST_MATH)
		const float32 h = 0.5f;

		SINT32 t;

#if defined(_MSC_VER)
		__asm
		{
			fld	x
				fadd	h
				fistp	t
		}
#elif defined(__GNUC__)
		__asm__ __volatile__(
			"fadd %2 \n\t"
			"fistpl %0 \n\t"
			: "=m"(t)
			: "t"(x), "f"(h)
			: "st"
			);
#else
		#  warn SAPPHIRE_FAST_MATH not supported.
		return (SINT32)ceilf(x);
#endif
		return t;
#else  
		return (SINT32)ceilf(x);
#endif
	}


	// 计算: 1 / x
	FORCEINLINE float32 reciprocal(const float32 f)
	{
#if (SAPPHIRE_FAST_MATH)

		// SSE Newton-Raphson 倒数估算法, 精确到尾数23位
		// 一个Newtown-Raphson 迭代:
		// f(i+1) = 2 * rcpss(f) - f * rcpss(f) * rcpss(f)
		float32 rec;
		__asm rcpss xmm0, f               // xmm0 = rcpss(f)
		__asm movss xmm1, f               // xmm1 = f
		__asm mulss xmm1, xmm0            // xmm1 = f * rcpss(f)
		__asm mulss xmm1, xmm0            // xmm2 = f * rcpss(f) * rcpss(f)
		__asm addss xmm0, xmm0            // xmm0 = 2 * rcpss(f)
		__asm subss xmm0, xmm1            // xmm0 = 2 * rcpss(f)
		//        - f * rcpss(f) * rcpss(f)
		__asm movss rec, xmm0             // return xmm0
		return rec;


		//! 不能够除0.. (fpu 异常)
		// 代替设置f到一个高值取获取一个接近0的返回值
		// 这儿必须测试（平面法线与其它的点积）<0.f
		//UINT32  x = (-(AIR(f) != 0 ) >> 31 ) & ( IR(f) ^ 0xd368d4a5 ) ^ 0xd368d4a5;
		//return 1.f / FR ( x );

#else  
		return 1.f / f;
#endif
	}

	// 计算: 1 / x
	FORCEINLINE Real reciprocal(const Real f)
	{
		return 1.0 / f;
	}

	inline SINT32 SINT32_min(SINT32 a, SINT32 b)
	{
		const SINT32 mask = (a - b) >> 31;
		return (a & mask) | (b & ~mask);
	}

	inline SINT32 SINT32_max(SINT32 a, SINT32 b)
	{
		const SINT32 mask = (a - b) >> 31;
		return (b & mask) | (a & ~mask);
	}

	inline SINT32 SINT32_clamp(SINT32 value, SINT32 low, SINT32 high)
	{
		return SINT32_min(SINT32_max(value, low), high);
	}


	//IEEE 754浮点数的2为底的整数对数
	static inline SINT32 s32_log2_f32(float32 f)
	{
		UINT32 x = IR(f);
		return ((x & 0x7F800000) >> 23) - 127;
	}



	static inline SINT32 s32_log2_s32(UINT32 x)
	{
		return s32_log2_f32((float32)x);
	}


#if 0
	//! 返回a是否等于b， 不使用任何进位误差
	inline bool equals(const SINT32 a, const SINT32 b)
	{
		return (a == b);
	}

	//! 返回a是否等于b， 不使用任何进位误差
	inline bool equals(const UINT32 a, const UINT32 b)
	{
		return (a == b);
	}
#endif
	//! 返回a是否等于b, 使用显示的进位误差
	inline bool equals(const SINT32 a, const SINT32 b, const SINT32 tolerance = std::numeric_limits<SINT32>::epsilon())
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! 返回a是否等于b, 使用显示的进位误差
	inline bool equals(const UINT32 a, const UINT32 b, const UINT32 tolerance = std::numeric_limits<UINT32>::epsilon())
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

#ifdef SAPPHIRE_SINT64_SUPPORT
	//! 返回a是否等于b, 使用显示的进位误差
	inline bool equals(const SINT64 a, const SINT64 b, const SINT64 tolerance = std::numeric_limits<SINT64>::epsilon())
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}
#endif

	/*
	使用偏差运河计数(地址偏差1字节)
	不允许拷贝0字节
	*/
	FORCEINLINE void memcpy32_small(void * dest, const void *source, UINT32 bytesize)
	{
		//计算出多少个32位双字
		UINT32 c = bytesize >> 2;

		do
		{
			((UINT32*)dest)[c - 1] = ((UINT32*)source)[c - 1];
		} while (--c);

	}

	//! 基于字的内存设置
	//  
	inline void memset16(void * dest, const UINT16 value, UINT32 bytesize)
	{
		UINT16 * d = (UINT16*)dest;

		UINT32 i;

		// 一次复制8*2=16个字节
		i = bytesize >> (1 + 3);
		while (i)
		{
			d[0] = value;
			d[1] = value;
			d[2] = value;
			d[3] = value;

			d[4] = value;
			d[5] = value;
			d[6] = value;
			d[7] = value;

			d += 8;
			--i;
		}

		i = (bytesize >> 1) & 7;
		while (i)
		{
			d[0] = value;
			++d;
			--i;
		}
	}

	inline void memset32(void * dest, const UINT32 value, UINT32 bytesize)
	{
		UINT32 * d = (UINT32*)dest;

		UINT32 i;

		i = bytesize >> (2 + 3);
		while (i)
		{
			d[0] = value;
			d[1] = value;
			d[2] = value;
			d[3] = value;

			d[4] = value;
			d[5] = value;
			d[6] = value;
			d[7] = value;

			d += 8;
			i -= 1;
		}

		i = (bytesize >> 2) & 7;
		while (i)
		{
			d[0] = value;
			d += 1;
			i -= 1;
		}
	}
}




void Build_Sin_Cos_Tables(void);

void Build_Inverse_Cos_Table(float *invcos, int  range_scale = 360);










#endif