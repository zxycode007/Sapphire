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
#define FR(x)                           ((Real&)(x))
#else
inline Real FR(UINT32 x) { inttofloat tmp; tmp.u = x; return tmp.f; }
inline Real FR(SINT32 x) { inttofloat tmp; tmp.s = x; return tmp.f; }
#endif


//! 整形表示为一个浮点型
#if (SAPPHIRE_FAST_MATH==0)
#define IR(x)                           ((Real&)(x))
#else
inline UINT32 IR(Real x) { inttofloat tmp; tmp.f = x; return tmp.u; }
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



void Build_Sin_Cos_Tables(void);

void Build_Inverse_Cos_Table(float *invcos, int  range_scale = 360);










#endif