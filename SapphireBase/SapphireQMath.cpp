#include "SapphireQMath.h"

// ���0~369�ȵ����Һ����ұ�
float cos_look[361];
float sin_look[361];
float dp_inverse_cos[360 + 2];

//���������ټ�����2λ��0-512�������������
unsigned char logbase2ofx[513] =
{
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,

	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

};

//�������Һ����ұ�
void Build_Sin_Cos_Tables(void)
{


	for (int ang = 0; ang <= 360; ang++)
	{

		//���Ƕ�ת��Ϊ����
		float theta = (float)ang*PI / (float)180;


		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);

	}
}

void Build_Inverse_Cos_Table(float *invcos,
	int   range_scale)

{
	//�����������һ�������Һ��������ڵ������н�
	//����[-1��1]ӳ�䵽������Χ[0,range_scale]
	//Ȼ�󽫷�Χ[-1,1]���ȵػ��ֳ�range_scale+1�ݣ�������ÿ��ֵ�ķ�����
	//�ٽ�����洢��һ������range_scale+1��Ԫ�ص�float������
	//���ұ�ľ���Ϊ180/range_scale�����磬���rangle_scaleΪ360���򾫶�Ϊ0.5��, ���rangle_scale��180���򾫶�Ϊ1��


	int  index = 0;

	//��ʼֵ
	float val = -1;

	//������,��Χ[0, rangle_scale]
	for (int index = 0; index <= range_scale; index++)
	{
		// ����һ��Ԫ�ش洢�������Ҳ��ұ���
		// ���val > 1 �������ֵ����ô���Ʒ�ΧΪ1
		val = (val > 1) ? 1 : val;
		//���val�ķ����Ǻ�����c��math�⺯�� acos���ص��ǻ��ȣ�����ת��Ϊ�Ƕ�,���뵽����
		invcos[index] = RAD_TO_DEG(acos(val));
		//Write_Error("\ninvcos[%d] = %f, val = %f", index, invcos[index], val);

		// val������С�������Լ�����һ��val��ֵ  
		val += ((float)1 / (float)(range_scale / 2));

	}
	// ����һ��Ԫ�أ������ڷ���ʱ���һ����λ�������Ʋ��ܳ�����Χ

	invcos[index] = invcos[index - 1];

}


float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   

	return y;
}

float Q_fabs(float f) {
	int tmp = *(int *)&f;
	tmp &= 0x7FFFFFFF;
	return *(float *)&tmp;
}


/**
@bire �ض�short���͵ķ�Χ
*/
signed short ClampShort(int i) {
	if (i < -32768) {
		return -32768;
	}
	if (i > 0x7fff) {
		return 0x7fff;
	}
	return i;
}


/**
@bire �ض�char���͵ķ�Χ
*/
signed char ClampChar(int i) {
	if (i < -128) {
		return -128;
	}
	if (i > 127) {
		return 127;
	}
	return i;
}


float asm_rand_max()
{

#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32
#if 0
#if SAPPHIRE_COMP_VER >= 1300

	return (std::numeric_limits< unsigned __int64 >::max)();
	return 9223372036854775807.0f;
#endif
#else
	//vc6
	return float(RAND_MAX);
#endif

#else
	// GCC etc
	return float(RAND_MAX);

#endif
}


// ����һ�������
float asm_rand()
{

#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32
#if 0
#if SAPPHIRE_COMP_VER >= 1300

	static unsigned __int64 q = time(NULL);

	_asm {
		movq mm0, q      //��64λint����qֵ�ƶ���mmx�Ĵ���mm0��

			//������ 0F 70 /r ib
			//PSHUFW mm1, mm2/m64, imm8
			//���� imm8 �еı���� mm2/m64 �е���ִ������������洢�� mm1��
			pshufw mm1, mm0, 0x1E   //ѹ��������  
			paddd mm0, mm1      //���ƴ��˫�ֽ������ӷ�  ��˫�ֶ���,��ͨ���.��addָ������.

			// ���ƶ������ڴ��ַ�����ͷ�MMX�Ĵ���
			movq q, mm0
			emms   //����Ĵ�����λ
	}

	return float(q);
#endif
#else
	// ��֧��PSHUFW
	return float(rand());
#endif
#else
	// GCC 

	return float(rand());

#endif
}


float Q_crandom(int *seed) {
	return 2.0 * (Q_random(seed) - 0.5);
}

float Q_random(int *seed) {
	return (Q_rand(seed) & 0xffff) / (float)0x10000;
}

int	Q_rand(int *seed) {
	*seed = (69069 * *seed + 1);
	return *seed;
}


float  Fast_Distance_3D(float fx, float fy, float fz)
{


	int temp;
	int x, y, z;


	x = fabs(fx) * 1024;
	y = fabs(fy) * 1024;
	z = fabs(fz) * 1024;


	if (y < x) SWAP(x, y, temp)

		if (z < y) SWAP(y, z, temp)

			if (y < x) SWAP(x, y, temp)

				int dist = (z + 11 * (y >> 5) + (x >> 2));

	return((float)(dist >> 10));

}



int Fast_Distance_2D(int x, int y)
{
	//ȡ����ֵ
	x = abs(x);
	y = abs(y);

	//�����н�Сֵ
	int mn = MIN(x, y);

	return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));

}


int FastLog2(int x)
{
	float fx;
	unsigned long ix, exp;

	fx = (float)x;
	//��ȡ�����ȸ����͵�����IEEE�����ʽ
	ix = *(unsigned long*)&fx;
	//����23λȥ��β����23λ��������ָ����8λ
	exp = (ix >> 23) & 0xFF;
	//��ȥ127ƫ��ֵ������ʵָ����
	return exp - 127;
}


float Fast_Cos(float theta)
{
	theta = fmodf(theta, 360);

	if (theta < 0) theta += 360.0;

	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	return(cos_look[theta_int] +
		theta_frac*(cos_look[theta_int + 1] - cos_look[theta_int]));

}


float Fast_Sin(float theta)
{

	//ȡ������ ��thetaת����0~360������
	theta = fmodf(theta, 360);

	//���thetaС��0��ת��Ϊ��ֵ
	if (theta < 0)
	{
		theta += 360;
	}
	//��ǰtheta��������С�����֣��Ա���в�ֵ����
	int theta_int = (int)theta;
	//С���������ڲ�ֵ�������
	float theta_frac = theta - theta_int;

	//ʹ�ò���͸���С�����ֽ��в�ֵ����������
	return(sin_look[theta_int] +
		theta_frac*(sin_look[theta_int + 1] - sin_look[theta_int]));
}


// ���� 1 / a ���� a * a = r
// -- Use this for Vector normalisation!!!
float asm_rsq(float r)
{
#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32

	__asm {
		fld1 // r0 = 1.f
			fld r // r1 = r0, r0 = r
			fsqrt // r0 = sqrtf( r0 )
			fdiv // r0 = r1 / r0
	} // returns r0

#else

	return 1. / sqrt(r);

#endif
}