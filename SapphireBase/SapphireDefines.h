#ifndef _SAPPHIRE_DEFINES_
#define _SAPPHIRE_DEFINES_


namespace Sapphire
{
	//! 创建4个字符ASCII码用于简单id
#define MAKE_IRR_ID(c0, c1, c2, c3) \
		((UINT32)(UINT8)(c0) | ((UINT32)(UINT8)(c1) << 8) | \
		((UINT32)(UINT8)(c2) << 16) | ((UINT32)(UINT8)(c3) << 24 ))
}



#endif 