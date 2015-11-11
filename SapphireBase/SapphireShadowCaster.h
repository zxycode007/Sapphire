#ifndef _SAPPHIRE_SHADOW_CASTER_
#define _SAPPHIRE_SHADOW_CASTER_
#include "SapphirePrerequisites.h"

namespace Sapphire
{
	/**
	����ඨ������ӰͶ��������ʵ�ֵĽӿ�
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