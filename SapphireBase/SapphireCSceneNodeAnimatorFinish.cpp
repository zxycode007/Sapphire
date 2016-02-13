#include "SapphireCSceneNodeAnimatorFinish.h"
#include "SapphireIScenesManager.h"
#include "SapphireSceneParameters.h"

namespace Sapphire
{


	//! constructor
	CSceneNodeAnimatorDelete::CSceneNodeAnimatorDelete(ISceneManager* manager, UINT32 time)
		: ISceneNodeAnimatorFinishing(time), SceneManager(manager)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorDelete");
#endif
	}


	//! animates a scene node
	void CSceneNodeAnimatorDelete::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		if (timeMs > FinishTime)
		{
			HasFinished = true;
			if (node && SceneManager)
			{
				// don't delete if scene manager is attached to an editor
				if (!SceneManager->getParameters()->getAttributeAsBool(SAPPHIRE_SCENE_MANAGER_IS_EDITOR))
					SceneManager->addToDeletionQueue(node);
			}
		}
	}


	ISceneNodeAnimator* CSceneNodeAnimatorDelete::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorDelete * newAnimator =
			new CSceneNodeAnimatorDelete(SceneManager, FinishTime);

		return newAnimator;
	}
}