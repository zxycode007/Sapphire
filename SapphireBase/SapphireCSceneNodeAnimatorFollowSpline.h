#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_FOLLOW_SPLINE_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_FOLLOW_SPLINE_

#include "SapphireISceneNode.h"
#include "SapphireISceneNodeAnimatorFinishing.h"

namespace Sapphire
{
	//! Scene node animator based free code Matthias Gall wrote and sent in. (Most of
	//! this code is written by him, I only modified bits.)
	class CSceneNodeAnimatorFollowSpline : public ISceneNodeAnimatorFinishing
	{
	public:

		//! constructor
		CSceneNodeAnimatorFollowSpline(UINT32 startTime,
			const vector< Vector3 >::type& points,
			FLOAT32 speed = 1.0f, FLOAT32 tightness = 0.5f, bool loop = true, bool pingpong = false);

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_FOLLOW_SPLINE; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	protected:

		//! clamps a the value idx to fit into range 0..size-1
		SINT32 clamp(SINT32 idx, SINT32 size);

		vector< Vector3 >::type Points;
		FLOAT32 Speed;
		FLOAT32 Tightness;
		UINT32 StartTime;
		bool Loop;
		bool PingPong;
	};
}

#endif