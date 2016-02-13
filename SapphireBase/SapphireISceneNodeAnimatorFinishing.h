#ifndef _SAPPHIRE_I_SCENE_NODE_ANIMATOR_FINISH_
#define _SAPPHIRE_I_SCENE_NODE_ANIMATOR_FINISH_

#include "SapphireISceneNode.h"


namespace Sapphire
{
	//! This is an abstract base class for animators that have a discrete end time.
	class ISceneNodeAnimatorFinishing : public ISceneNodeAnimator
	{
	public:

		//! constructor
		ISceneNodeAnimatorFinishing(UINT32 finishTime)
			: FinishTime(finishTime), HasFinished(false) { }

		virtual bool hasFinished(void) const { return HasFinished; }

	protected:

		UINT32 FinishTime;
		bool HasFinished;
	};
}

#endif