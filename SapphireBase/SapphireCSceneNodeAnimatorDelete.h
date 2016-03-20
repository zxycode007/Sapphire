#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_DELETE_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_DELETE_

#include "SapphireISceneNodeAnimatorFinishing.h"



namespace Sapphire
{
	class CSceneNodeAnimatorDelete : public ISceneNodeAnimatorFinishing
	{
	public:

		//! constructor
		CSceneNodeAnimatorDelete(ISceneManager* manager, UINT32 when);

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_DELETION;
		}

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:

		ISceneManager* SceneManager;
	};
}


#endif