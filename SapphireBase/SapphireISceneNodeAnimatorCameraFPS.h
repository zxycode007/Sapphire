#ifndef _SAPPHIRE_I_SCENE_NODE_ANIMATOR_CAMERA_FPS_
#define _SAPPHIRE_I_SCENE_NODE_ANIMATOR_CAMERA_FPS_

#include "SapphireISceneNodeAnimator.h"
#include "SapphireIEventRecevier.h"

namespace Sapphire
{
	struct SKeyMap;

	//! Special scene node animator for FPS cameras
	/** This scene node animator can be attached to a camera to make it act
	like a first person shooter
	*/
	class ISceneNodeAnimatorCameraFPS : public ISceneNodeAnimator
	{
	public:

		//! Returns the speed of movement in units per millisecond
		virtual FLOAT32 getMoveSpeed() const = 0;

		//! Sets the speed of movement in units per millisecond
		virtual void setMoveSpeed(FLOAT32 moveSpeed) = 0;

		//! Returns the rotation speed in degrees
		/** The degrees are equivalent to a half screen movement of the mouse,
		i.e. if the mouse cursor had been moved to the border of the screen since
		the last animation. */
		virtual FLOAT32 getRotateSpeed() const = 0;

		//! Set the rotation speed in degrees
		virtual void setRotateSpeed(FLOAT32 rotateSpeed) = 0;

		//! Sets the keyboard mapping for this animator (old style)
		/** \param map Array of keyboard mappings, see irr::SKeyMap
		\param count Size of the keyboard map array. */
		virtual void setKeyMap(SKeyMap *map, UINT32 count) = 0;

		//! Sets the keyboard mapping for this animator
		//!	\param keymap The new keymap array 
		virtual void setKeyMap(const vector<SKeyMap>::type& keymap) = 0;

		//! Gets the keyboard mapping for this animator
		virtual const vector<SKeyMap>::type& getKeyMap() const = 0;

		//! Sets whether vertical movement should be allowed.
		/** If vertical movement is enabled then the camera may fight with
		gravity causing camera shake. Disable this if the camera has
		a collision animator with gravity enabled. */
		virtual void setVerticalMovement(bool allow) = 0;

		//! Sets whether the Y axis of the mouse should be inverted.
		/** If enabled then moving the mouse down will cause
		the camera to look up. It is disabled by default. */
		virtual void setInvertMouse(bool invert) = 0;
	};
}

#endif