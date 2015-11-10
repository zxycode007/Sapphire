#include "SapphireQMath.h"

// 存放0~369度的正弦和余弦表
float cos_look[361];
float sin_look[361];
float dp_inverse_cos[360 + 2];

//这个数组快速计算以2位底0-512对数最近的整数
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

//构建正弦和余弦表
void Build_Sin_Cos_Tables(void)
{


	for (int ang = 0; ang <= 360; ang++)
	{

		//将角度转换为弧度
		float theta = (float)ang*PI / (float)180;


		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);

	}
}

void Build_Inverse_Cos_Table(float *invcos,
	int   range_scale)

{
	//这个函数创建一个反余弦函数表，用于点积计算夹角
	//它将[-1，1]映射到索引范围[0,range_scale]
	//然后将范围[-1,1]均匀地划分成range_scale+1份，并计算每个值的反余弦
	//再将结果存储在一个包含range_scale+1个元素的float数组中
	//查找表的精度为180/range_scale，例如，如果rangle_scale为360，则精度为0.5度, 如果rangle_scale是180，则精度为1度


	int  index = 0;

	//起始值
	float val = -1;

	//创建表,范围[0, rangle_scale]
	for (int index = 0; index <= range_scale; index++)
	{
		// 将下一个元素存储到反余弦查找表中
		// 如果val > 1 超过最大值，那么限制范围为1
		val = (val > 1) ? 1 : val;
		//求出val的反三角函数，c的math库函数 acos返回的是弧度，将其转换为角度,插入到表中
		invcos[index] = RAD_TO_DEG(acos(val));
		//Write_Error("\ninvcos[%d] = %f, val = %f", index, invcos[index], val);

		// val增加最小步进，以计算下一个val的值  
		val += ((float)1 / (float)(range_scale / 2));

	}
	// 增加一个元素，以免在访问时溢出一个单位，做限制不能超出范围

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
@bire 截断short类型的范围
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
@bire 截断char类型的范围
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


// 返回一个随机数
float asm_rand()
{

#if  SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC &&  SAPPHIRE_ARCH_TYPE == SAPPHIRE_ARCHITECTURE_32
#if 0
#if SAPPHIRE_COMP_VER >= 1300

	static unsigned __int64 q = time(NULL);

	_asm {
		movq mm0, q      //将64位int变量q值移动到mmx寄存器mm0中

			//操作码 0F 70 /r ib
			//PSHUFW mm1, mm2/m64, imm8
			//按照 imm8 中的编码对 mm2/m64 中的字执行乱序处理，结果存储到 mm1。
			pshufw mm1, mm0, 0x1E   //压缩字乱序  
			paddd mm0, mm1      //环绕打包双字节整数加法  按双字对齐,普通相加.与add指令类似.

			// 在移动整形内存地址并且释放MMX寄存器
			movq q, mm0
			emms   //浮点寄存器复位
	}

	return float(q);
#endif
#else
	// 不支持PSHUFW
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
	//取绝对值
	x = abs(x);
	y = abs(y);

	//求其中较小值
	int mn = MIN(x, y);

	return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));

}


int FastLog2(int x)
{
	float fx;
	unsigned long ix, exp;

	fx = (float)x;
	//提取单精度浮点型的完整IEEE浮点格式
	ix = *(unsigned long*)&fx;
	//右移23位去掉尾数域（23位），留下指数域8位
	exp = (ix >> 23) & 0xFF;
	//减去127偏差值，得真实指数域
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

	//取余数， 将theta转换到0~360度以内
	theta = fmodf(theta, 360);

	//如果theta小于0，转换为正值
	if (theta < 0)
	{
		theta += 360;
	}
	//提前theta的整数和小数部分，以便进行插值计算
	int theta_int = (int)theta;
	//小数部分用于插值计算误差
	float theta_frac = theta - theta_int;

	//使用查表法和根据小数部分进行插值来计算正弦
	return(sin_look[theta_int] +
		theta_frac*(sin_look[theta_int + 1] - sin_look[theta_int]));
}


// 返回 1 / a 对于 a * a = r
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