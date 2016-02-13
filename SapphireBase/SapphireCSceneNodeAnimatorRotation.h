#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_RATATION_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_RATATION_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class CSceneNodeAnimatorRotation : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CSceneNodeAnimatorRotation(UINT32 time, const Vector3& rotation);

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_ROTATION; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:

		Vector3 Rotation;
		UINT32 StartTime;
	};
}

#endif