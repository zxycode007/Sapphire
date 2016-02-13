#ifndef _SAPPHIRE_C_SCENE_ANIMATOR_CAMERA_NODE_
#define _SAPPHIRE_C_SCENE_ANIMATOR_CAMERA_NODE_

#include "SapphireISceneNodeAnimatorCameraFPS.h"
#include "SapphireVector2.h"
#include "SapphireVector2.h"
#include "SapphireKeyMap.h"

namespace Sapphire
{
	class ICursorControl;

	//! Special scene node animator for FPS cameras
	class CSceneNodeAnimatorCameraFPS : public ISceneNodeAnimatorCameraFPS
	{
	public:

		//! Constructor
		CSceneNodeAnimatorCameraFPS(ICursorControl* cursorControl,
			FLOAT32 rotateSpeed = 100.0f, FLOAT32 moveSpeed = .5f, FLOAT32 jumpSpeed = 0.f,
			SKeyMap* keyMapArray = 0, UINT32 keyMapSize = 0, bool noVerticalMovement = false,
			bool invertY = false);

		//! Destructor
		virtual ~CSceneNodeAnimatorCameraFPS();

		//! Animates the scene node, currently only works on cameras
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Event receiver
		virtual bool OnEvent(const SEvent& event);

		//! Returns the speed of movement in units per second
		virtual FLOAT32 getMoveSpeed() const;

		//! Sets the speed of movement in units per second
		virtual void setMoveSpeed(FLOAT32 moveSpeed);

		//! Returns the rotation speed
		virtual FLOAT32 getRotateSpeed() const;

		//! Set the rotation speed
		virtual void setRotateSpeed(FLOAT32 rotateSpeed);

		//! Sets the keyboard mapping for this animator (old style)
		//! \param keymap: an array of keyboard mappings, see SKeyMap
		//! \param count: the size of the keyboard map array
		virtual void setKeyMap(SKeyMap *map, UINT32 count);

		//! Sets the keyboard mapping for this animator
		//!	\param keymap The new keymap array 
		virtual void setKeyMap(const vector<SKeyMap>::type& keymap);

		//! Gets the keyboard mapping for this animator
		virtual const vector<SKeyMap>::type& getKeyMap() const;

		//! Sets whether vertical movement should be allowed.
		virtual void setVerticalMovement(bool allow);

		//! Sets whether the Y axis of the mouse should be inverted.
		/** If enabled then moving the mouse down will cause
		the camera to look up. It is disabled by default. */
		virtual void setInvertMouse(bool invert);

		//! This animator will receive events when attached to the active camera
		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		//! Returns the type of this animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_CAMERA_FPS;
		}

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer once you're
		done with it. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:
		void allKeysUp();

		ICursorControl *CursorControl;

		FLOAT32 MaxVerticalAngle;

		FLOAT32 MoveSpeed;
		FLOAT32 RotateSpeed;
		FLOAT32 JumpSpeed;
		// -1.0f for inverted mouse, defaults to 1.0f
		FLOAT32 MouseYDirection;

		SINT32 LastAnimationTime;

		vector<SKeyMap>::type KeyMap;
		Position2d CenterCursor, CursorPos;

		bool CursorKeys[EKA_COUNT];

		bool firstUpdate;
		bool firstInput;
		bool NoVerticalMovement;
	};

}

#endif