#ifndef _SAPPHIRE_Q_MATH_
#define _SAPPHIRE_Q_MATH_

#include "SapphirePrerequisites.h"


/**

  c����ѧ���㺯����

  */


//////////////////////////��ѧ����//////////////////////////////////
//PI���
/**
@brief
����Բ���ʺ�
*/
#define PI         ((float)3.141592654f)
/**
@brief
���� Բ���� �� 2
*/
#define PI2        ((float)6.283185307f)
/**
@brief
����Բ����/2
*/
#define PI_DIV_2   ((float)1.570796327f)
/**
@brief
����Բ����/4
*/
#define PI_DIV_4   ((float)0.785398163f) 
/**
@brief
����Բ���ʵĵ���
*/
#define PI_INV     ((float)0.318309886f) 

//��С����
/**
@brief
�ŵ�С�����3λ��
*/
#define EPSILON_E3 (float)(1E-3) 
/**
@brief
�ŵ�С�����4λ��
*/
#define EPSILON_E4 (float)(1E-4)
/**
@brief
�ŵ�С�����5λ��
*/
#define EPSILON_E5 (float)(1E-5)
/**
@brief
�ŵ�С�����6λ��
*/
#define EPSILON_E6 (float)(1E-6)
/**
@brief
�ŵ�С�����7λ��
*/
#define EPSILON_E7  (double)(1E-7)
/**
@brief
�ŵ�С�����8λ��
*/
#define EPSILON_E8  (double)(1E-8)

//////////////////////////////////////////ͨ�ú�/////////////////////////////////////////
//�жϴ�С,ȥ�ϴ���Сֵ
/**
@brief ȡ������Сֵ
@param a ����a
@param b ����b
@return a��b�е���Сֵ
*/
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
/**
@brief ȡ�������ֵ
@param a ����a
@param b ����b
@return a��b�е����ֵ
*/
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
//����������ֵ
/**
@brief ����������ֵ
@param a ����a
@param b ����b
@param t ��ʱ����
*/
#define SWAP(a,b,t) {t=a; a=b; b=t;}
//�ǶȻ���ת��
/**
@brief �ǶȻ���ת��
@param angle �Ƕ�ֵ
@return angle��Ӧ�Ļ���ֵ
*/
#define DEG_TO_RAD(angle) ((angle)*PI/180.0)
/**
@brief ���ȽǶ�ת��
@param radian ����ֵ
@return radian��Ӧ�ĽǶ�ֵ
*/
#define RAD_TO_DEG(radian) ((radian)*180.0/PI)
//x��y��Χ���
/**
@brief ����һ��x��y���ڵķ�Χ�������
@param x ����
@param y ����
@return �����
*/
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
/**
@brief �������ȽϺ�
���A<BֵΪ1������Ϊ0
@param ������A
@param ������B
@return �ȽϽ��
*/
#define FCMP(a,b) ((fabs(a-b) <EPSILON_E3)?1:0)
/**
@brief ������ټ��㷴����
��Ϊ�����±겻��Ϊ�����ͺ͸��������Խ�������Χ[-1,1] ƽ��1λ =  [0,2],��������180������[-1,1]��Ϊ360��,��λ������0.01
@param  xȡֵ��[-1,1]֮�䣬|x| >= 0.01
*/
#define FAST_INV_COS(x) (dp_inverse_cos[(int)(((float)x+1)*(float)180)])



/*
float IEEE-754 bit ��ʾ��
0      0x00000000
1.0    0x3f800000
0.5    0x3f000000
3      0x40400000
+inf   0x7f800000
-inf   0xff800000
+NaN   0x7fc00000 or 0x7ff00000
in ͨ��: number = (����λ ? -1:1) * 2^(ָ��) * 1.(β��λ)
*/

typedef union{ unsigned int u; int s; float f; } inttofloat;


//! ��������ʾһ������ֵ
#if (SAPPHIRE_FAST_MATH==0)
#define FR(x)                           ((Real&)(x))
#else
inline Real FR(UINT32 x) { inttofloat tmp; tmp.u = x; return tmp.f; }
inline Real FR(SINT32 x) { inttofloat tmp; tmp.s = x; return tmp.f; }
#endif


//! ���α�ʾΪһ��������
#if (SAPPHIRE_FAST_MATH==0)
#define IR(x)                           ((Real&)(x))
#else
inline UINT32 IR(Real x) { inttofloat tmp; tmp.f = x; return tmp.u; }
#endif













// ���0~369�ȵ����Һ����ұ�
/**
@brief ���ұ�
*/
extern float cos_look[361];
/**
@brief ���ұ�
*/
extern float sin_look[361];
/**
@brief �����ұ�
������ֵ��Ϊ0~pi�� ��[-1,1]��ֵӳ�䵽0~180��
Ϊ��߾��ȣ���180�ȷ�Ϊ360�ȷݣ���0.5��Ϊ��λ����
֮���Զ���362��Ϊ�˷�ֹ���ڸ�������������������Խ��
*/
extern float dp_inverse_cos[360 + 2];

/**
@brief 512��2Ϊ�׵Ķ�����
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


// ����һ�������
 float asm_rand();


// ��������������ֵ
 float asm_rand_max();


 // ���� 1 / a ���� a * a = r
 // -- Use this for Vector normalisation!!!
 float asm_rsq(float r);



/**
@bire �ض�char���͵ķ�Χ
*/
 signed char ClampChar(int i);
/**
@bire �ض�short���͵ķ�Χ
*/
 signed short ClampShort(int i);

/*
���������ֵ
*/
 float Q_fabs(float f);

/*
 ������ƽ��������
 */
 float Q_rsqrt(float number);

 //������ƽ����������INTELӲ���棩
 FORCEINLINE float Q_rsqrtHW(const float f)
 {
#if defined(_MSC_VER)
	 // SSE ƽ�����������㷨 ��ȷ���з��ŵ�12λβ��
	 float recsqrt;
	 __asm rsqrtss xmm0, f           // xmm0 = rsqrtss(f)
	 __asm movss recsqrt, xmm0       // return xmm0
	 return recsqrt;

	 /*
	 //������Nvidia
	 u32 tmp = (u32(IEEE_1_0 << 1) + IEEE_1_0 - *(u32*)&x) >> 1;
	 f32 y = *(f32*)&tmp;
	 return y * (1.47f - 0.47f * x * y * y);
	 */
#else
	 return 1.f / sqrtf(f);
#endif
 }


 // �������� //////////////////////////////////////////

 inline void Mem_Set_WORD(void *dest, unsigned short data, int count)
 {
	 //�ú���һ�����16λ�ֽ��ڴ�

	 _asm
	 {
		 mov edi, dest; //EDIָ��ָ��Ŀ���ڴ��ַ 
		 mov ecx, count; //���ƶ���������ECX 
		 mov ax, data; //��16λ���ݷ���AX
		 rep stosw; //repָ�����ظ���������,ֱ��ECXΪ0��ÿִ��һ��ECX-1��STOS�ǽ�EAXֵ������ES:ediָ��ĵ�ַ; ���������direction flag, ��ôedi���ڸ�ָ��ִ�к��С, ���û������direction flag, ��ôedi��ֵ������.
	 }

 }

 ///////////////////////////////////////////////////////////

 inline void Mem_Set_QUAD(void *dest, unsigned int data, int count)
 {
	 // �ú���һ�����32λ�ֽ��ڴ�


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