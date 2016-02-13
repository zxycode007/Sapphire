#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_FLY_CIRCLE_

#include "SapphireISceneNode.h"

namespace Sapphire
{
	class CSceneNodeAnimatorFlyCircle : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CSceneNodeAnimatorFlyCircle(UINT32 time,
			const Vector3& center, FLOAT32 radius,
			FLOAT32 speed, const Vector3& direction,
			FLOAT32 radiusEllipsoid);

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_FLY_CIRCLE; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:
		// do some initial calculations
		void init();

		// circle center
		Vector3 Center;
		// up-vector, normal to the circle's plane
		Vector3 Direction;
		// Two helper vectors
		Vector3 VecU;
		Vector3 VecV;
		FLOAT32 Radius;
		FLOAT32 RadiusEllipsoid;
		FLOAT32 Speed;
		UINT32 StartTime;
	};
}
#endif