#ifndef _SAPPHIRE_RECTANGLE__
#define _SAPPHIRE_RECTANGLE__

#include "SapphirePrerequisites.h"

namespace Sapphire {

	

	struct _SapphireExport Rectangle
	{
		Real left;
		Real top;
		Real right;
		Real bottom;

		inline bool inside(Real x, Real y) const { return x >= left && x <= right && y >= top && y <= bottom; }
	};

	/** 两个矩形区域的相交性测试 
	*/
	inline Rectangle intersect(const Rectangle& lhs, const Rectangle& rhs)
	{
		Rectangle r;

		r.left = lhs.left   > rhs.left ? lhs.left : rhs.left;
		r.top = lhs.top    > rhs.top ? lhs.top : rhs.top;
		r.right = lhs.right  < rhs.right ? lhs.right : rhs.right;
		r.bottom = lhs.bottom < rhs.bottom ? lhs.bottom : rhs.bottom;

		return r;
	}
	

}

#endif 
