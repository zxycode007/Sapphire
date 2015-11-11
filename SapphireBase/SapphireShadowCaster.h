#ifndef _SAPPHIRE_SHADOW_CASTER_
#define _SAPPHIRE_SHADOW_CASTER_
#include "SapphirePrerequisites.h"

namespace Sapphire
{
	/**
	这个类定义了阴影投射器必须实现的接口
	*/
	class _SapphireExport ShadowCaster
	{
	public:
		virtual ~ShadowCaster() { }
		/** Returns whether or not this object currently casts a shadow. */
		virtual bool getCastShadows(void) const = 0;

		
	};

}

#endif 