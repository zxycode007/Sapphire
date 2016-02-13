#ifndef _SAPHIRE_I_SCENE_NODE_ANIMATOR_CAMERA_MAYA_
#define _SAPHIRE_I_SCENE_NODE_ANIMATOR_CAMERA_MAYA_

#include "SapphireISceneNodeAnimator.h"

namespace Sapphire
{

	//! Special scene node animator for Maya-style cameras
	/** This scene node animator can be attached to a camera to make it act like a 3d
	modelling tool.
	The camera is moving relative to the target with the mouse, by pressing either
	of the three buttons.
	In order to move the camera, set a new target for the camera. The distance defines
	the current orbit radius the camera moves on. Distance can be changed via the setter
	or by mouse events.
	*/
	class ISceneNodeAnimatorCameraMaya : public ISceneNodeAnimator
	{
	public:

		//! Returns the speed of movement
		virtual FLOAT32 getMoveSpeed() const = 0;

		//! Sets the speed of movement
		virtual void setMoveSpeed(FLOAT32 moveSpeed) = 0;

		//! Returns the rotation speed
		virtual FLOAT32 getRotateSpeed() const = 0;

		//! Set the rotation speed
		virtual void setRotateSpeed(FLOAT32 rotateSpeed) = 0;

		//! Returns the zoom speed
		virtual FLOAT32 getZoomSpeed() const = 0;

		//! Set the zoom speed
		virtual void setZoomSpeed(FLOAT32 zoomSpeed) = 0;

		//! Returns the current distance, i.e. orbit radius
		virtual FLOAT32 getDistance() const = 0;

		//! Set the distance
		virtual void setDistance(FLOAT32 distance) = 0;
	};
}

#endif