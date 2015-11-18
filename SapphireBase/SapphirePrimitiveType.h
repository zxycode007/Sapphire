#ifndef _SAPPHIRE_PRIMITIVE_TYPE_
#define _SAPPHIRE_PRIMITIVE_TYPE_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	//! 枚举图元类型
	enum E_PRIMITIVE_TYPE
	{
		//单个顶点
		EPT_POINTS = 0,

		//所有顶点链接成一条单独线段
		EPT_LINE_STRIP,

		//!同上，但是首位相接
		EPT_LINE_LOOP,

		//! 每两个顶点链接为N/2条线段
		EPT_LINES,

		//!  自前两个顶点后的每个顶点构成一个新三角形N-2
		EPT_TRIANGLE_STRIP,

		//!  自前两个顶点后的每个顶点构成一个新三角形N-2
		//! 所有的三角形都环绕第一个顶点
		EPT_TRIANGLE_FAN,

		//! 设置所有顶点到每一个三角形
		EPT_TRIANGLES,

		//! 对于前两个顶点After the first two vertices each further tw vetices create a quad with the preceding two.
		EPT_QUAD_STRIP,

		//! 每四个顶点构成一个四边形
		EPT_QUADS,

		//! 就是一个LINE_LOOP,但填充
		EPT_POLYGON,

		//! 在GPU由一个顶点扩展到一个四边形的公告牌
		EPT_POINT_SPRITES
	};
}

#endif