#ifndef _SAPPHIRE_ANIMABLE_
#define _SAPPHIRE_ANIMABLE_
#include "SapphirePrerequisites.h"



namespace Sapphire
{
	/** Defines an object property which is animable, i.e. may be keyframed.
	@remarks
	Animable properties are those which can be altered over time by a
	predefined keyframe sequence. They may be set directly, or they may
	be modified from their existing state (common if multiple animations
	are expected to apply at once). Implementors of this interface are
	expected to override the 'setValue', 'setCurrentStateAsBaseValue' and
	'applyDeltaValue' methods appropriate to the type in question, and to
	initialise the type.
	@par
	AnimableValue instances are accessible through any class which extends
	AnimableObject in order to expose it's animable properties.
	@note
	This class is an instance of the Adapter pattern, since it generalises
	access to a particular property. Whilst it could have been templated
	such that the type which was being referenced was compiled in, this would
	make it more difficult to aggregated generically, and since animations
	are often comprised of multiple properties it helps to be able to deal
	with all values through a single class.
	*/
	class _SapphireExport AnimableValue : public AnimableAlloc
	{


	};
}



#endif 