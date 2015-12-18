#ifndef _SAPPHIRE_SPARTICLE_
#define _SAPPHIRE_SPARTICLE_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireDimension2D.h"
#include "SapphireColorValue.h"


namespace Sapphire
{
	 

	//! 保存粒子数据的结构体
	struct SParticle
	{
		//! 粒子的位置
		Vector3 pos;

		//! 粒子的方向和速度
		Vector3 vector;

		//! 粒子的生命开始时间
		UINT32 startTime;

		//! 粒子的生命结束时间
		UINT32 endTime;

		//! 粒子的当前颜色
		ColourValue color;

		//! 粒子的原颜色 
		/** 当粒子发射时的颜色*/
		ColourValue startColor;

		//! 粒子的原方向和速度
		/** 当粒子发射时的方向和速度 */
		Vector3 startVector;

		//! 粒子的缩放
		/** 粒子的当前缩放 */
		dimension2df size;

		//! 粒子的原缩放
		/** 粒子在发射时的缩 */
		dimension2df startSize;
	};
}

#endif
