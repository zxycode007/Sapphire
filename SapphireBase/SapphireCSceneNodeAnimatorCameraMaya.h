#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_CAMERA_MAYA_

#include "SapphireISceneNodeAnimatorCameraMaya.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireVector2.h"


namespace Sapphire
{
	class ICursorControl;

	//! Special scene node animator for FPS cameras
	/** This scene node animator can be attached to a camera to make it act
	like a 3d modelling tool camera
	*/
	class CSceneNodeAnimatorCameraMaya : public ISceneNodeAnimatorCameraMaya
	{
	public:
		//! Constructor
		CSceneNodeAnimatorCameraMaya(ICursorControl* cursor, FLOAT32 rotateSpeed = -1500.f,
			FLOAT32 zoomSpeed = 200.f, FLOAT32 translationSpeed = 1500.f, FLOAT32 distance = 70.f);

		//! Destructor
		virtual ~CSceneNodeAnimatorCameraMaya();

		//! Animates the scene node, currently only works on cameras
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Event receiver
		virtual bool OnEvent(const SEvent& event);

		//! Returns the speed of movement in units per millisecond
		virtual FLOAT32 getMoveSpeed() const;

		//! Sets the speed of movement in units per millisecond
		virtual void setMoveSpeed(FLOAT32 moveSpeed);

		//! Returns the rotation speed
		virtual FLOAT32 getRotateSpeed() const;

		//! Set the rotation speed
		virtual void setRotateSpeed(FLOAT32 rotateSpeed);

		//! Returns the zoom speed
		virtual FLOAT32 getZoomSpeed() const;

		//! Set the zoom speed
		virtual void setZoomSpeed(FLOAT32 zoomSpeed);

		//! Returns the current distance, i.e. orbit radius
		virtual FLOAT32 getDistance() const;

		//! Set the distance
		virtual void setDistance(FLOAT32 distance);

		//! This animator will receive events when attached to the active camera
		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		//! Returns type of the scene node
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
		{
			return ESNAT_CAMERA_MAYA;
		}

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:

		void allKeysUp();
		void animate();
		bool isMouseKeyDown(SINT32 key) const;

		bool MouseKeys[3];

		ICursorControl *CursorControl;
		ICameraSceneNode* OldCamera;
		Vector3 OldTarget;
		Vector3 LastCameraTarget;	// to find out if the camera target was moved outside this animator
		Position2d RotateStart;
		Position2d ZoomStart;
		Position2d TranslateStart;
		Position2d MousePos;
		FLOAT32 ZoomSpeed;
		FLOAT32 RotateSpeed;
		FLOAT32 TranslateSpeed;
		FLOAT32 CurrentZoom;
		FLOAT32 RotX, RotY;
		bool Zooming;
		bool Rotating;
		bool Moving;
		bool Translating;
	};
}

#endif