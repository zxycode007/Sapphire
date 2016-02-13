#include "SapphireCSceneNodeAnimatorCameraMaya.h"
#include "SapphireICursorControl.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireSViewFrustum.h"
#include "SapphireIScenesManager.h"


namespace Sapphire
{
	//! constructor
	CSceneNodeAnimatorCameraMaya::CSceneNodeAnimatorCameraMaya(ICursorControl* cursor,
		FLOAT32 rotateSpeed, FLOAT32 zoomSpeed, FLOAT32 translateSpeed, FLOAT32 distance)
		: CursorControl(cursor), OldCamera(0), MousePos(0.5f, 0.5f),
		ZoomSpeed(zoomSpeed), RotateSpeed(rotateSpeed), TranslateSpeed(translateSpeed),
		CurrentZoom(distance), RotX(0.0f), RotY(0.0f),
		Zooming(false), Rotating(false), Moving(false), Translating(false)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorCameraMaya");
#endif

		if (CursorControl)
		{
			CursorControl->grab();
			MousePos = CursorControl->getRelativePosition();
		}

		allKeysUp();
	}


	//! destructor
	CSceneNodeAnimatorCameraMaya::~CSceneNodeAnimatorCameraMaya()
	{
		if (CursorControl)
			CursorControl->drop();
	}


	//! It is possible to send mouse and key events to the camera. Most cameras
	//! may ignore this input, but camera scene nodes which are created for
	//! example with ISceneManager::addMayaCameraSceneNode or
	//! ISceneManager::addMeshViewerCameraSceneNode, may want to get this input
	//! for changing their position, look at target or whatever.
	bool CSceneNodeAnimatorCameraMaya::OnEvent(const SEvent& event)
	{
		if (event.EventType != EET_MOUSE_INPUT_EVENT)
			return false;

		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			MouseKeys[0] = true;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			MouseKeys[2] = true;
			break;
		case EMIE_MMOUSE_PRESSED_DOWN:
			MouseKeys[1] = true;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			MouseKeys[0] = false;
			break;
		case EMIE_RMOUSE_LEFT_UP:
			MouseKeys[2] = false;
			break;
		case EMIE_MMOUSE_LEFT_UP:
			MouseKeys[1] = false;
			break;
		case EMIE_MOUSE_MOVED:
			MousePos = CursorControl->getRelativePosition();
			break;
		case EMIE_MOUSE_WHEEL:
		case EMIE_LMOUSE_DOUBLE_CLICK:
		case EMIE_RMOUSE_DOUBLE_CLICK:
		case EMIE_MMOUSE_DOUBLE_CLICK:
		case EMIE_LMOUSE_TRIPLE_CLICK:
		case EMIE_RMOUSE_TRIPLE_CLICK:
		case EMIE_MMOUSE_TRIPLE_CLICK:
		case EMIE_COUNT:
			return false;
		}
		return true;
	}


	//! OnAnimate() is called just before rendering the whole scene.
	void CSceneNodeAnimatorCameraMaya::animateNode(ISceneNode *node, UINT32 timeMs)
	{
		//Alt + LM = Rotate around camera pivot
		//Alt + LM + MM = Dolly forth/back in view direction (speed % distance camera pivot - max distance to pivot)
		//Alt + MM = Move on camera plane (Screen center is about the mouse pointer, depending on move speed)

		if (!node || node->getType() != ESNT_CAMERA)
			return;

		ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

		// If the camera isn't the active camera, and receiving input, then don't process it.
		if (!camera->isInputReceiverEnabled())
			return;

		ISceneManager * smgr = camera->getSceneManager();
		if (smgr && smgr->getActiveCamera() != camera)
			return;

		if (OldCamera != camera)
		{
			LastCameraTarget = OldTarget = camera->getTarget();
			OldCamera = camera;
		}
		else
		{
			OldTarget += camera->getTarget() - LastCameraTarget;
		}

		FLOAT32 nRotX = RotX;
		FLOAT32 nRotY = RotY;
		FLOAT32 nZoom = CurrentZoom;

		if ((isMouseKeyDown(0) && isMouseKeyDown(2)) || isMouseKeyDown(1))
		{
			if (!Zooming)
			{
				ZoomStart = MousePos;
				Zooming = true;
			}
			else
			{
				const FLOAT32 targetMinDistance = 0.1f;
				nZoom += (ZoomStart.x - MousePos.x) * ZoomSpeed;

				if (nZoom < targetMinDistance) // jox: fixed bug: bounce back when zooming to close
					nZoom = targetMinDistance;
			}
		}
		else if (Zooming)
		{
			const FLOAT32 old = CurrentZoom;
			CurrentZoom = CurrentZoom + (ZoomStart.x - MousePos.x) * ZoomSpeed;
			nZoom = CurrentZoom;

			if (nZoom < 0)
				nZoom = CurrentZoom = old;
			Zooming = false;
		}

		// Translation ---------------------------------

		Vector3 translate(OldTarget);
		const Vector3 upVector(camera->getUpVector());
		const Vector3 target = camera->getTarget();

		Vector3 pos = camera->getPosition();
		Vector3 tvectX = pos - target;
		tvectX = tvectX.crossProduct(upVector);
		tvectX.normalise();

		const SViewFrustum* const va = camera->getViewFrustum();
		Vector3 tvectY = (va->getFarLeftDown() - va->getFarRightDown());
		tvectY = tvectY.crossProduct(upVector.y > 0 ? pos - target : target - pos);
		tvectY.normalise();

		if (isMouseKeyDown(2) && !Zooming)
		{
			if (!Translating)
			{
				TranslateStart = MousePos;
				Translating = true;
			}
			else
			{
				translate += tvectX * (TranslateStart.x - MousePos.x)*TranslateSpeed +
					tvectY * (TranslateStart.y - MousePos.y)*TranslateSpeed;
			}
		}
		else if (Translating)
		{
			translate += tvectX * (TranslateStart.x - MousePos.x)*TranslateSpeed +
				tvectY * (TranslateStart.y - MousePos.y)*TranslateSpeed;
			OldTarget = translate;
			Translating = false;
		}

		// Rotation ------------------------------------

		if (isMouseKeyDown(0) && !Zooming)
		{
			if (!Rotating)
			{
				RotateStart = MousePos;
				Rotating = true;
				nRotX = RotX;
				nRotY = RotY;
			}
			else
			{
				nRotX += (RotateStart.x - MousePos.x) * RotateSpeed;
				nRotY += (RotateStart.y - MousePos.y) * RotateSpeed;
			}
		}
		else if (Rotating)
		{
			RotX += (RotateStart.x - MousePos.x) * RotateSpeed;
			RotY += (RotateStart.y - MousePos.y) * RotateSpeed;
			nRotX = RotX;
			nRotY = RotY;
			Rotating = false;
		}

		// Set pos ------------------------------------

		pos = translate;
		pos.x += nZoom;

		pos.rotateXYBy(nRotY, translate);
		pos.rotateXZBy(-nRotX, translate);

		camera->setPosition(pos);
		camera->setTarget(translate);

		// Rotation Error ----------------------------

		// jox: fixed bug: jitter when rotating to the top and bottom of y
		pos.set(0, 1, 0);
		pos.rotateXYBy(-nRotY);
		pos.rotateXZBy(-nRotX + 180.f);
		camera->setUpVector(pos);
		LastCameraTarget = camera->getTarget();
	}


	bool CSceneNodeAnimatorCameraMaya::isMouseKeyDown(SINT32 key) const
	{
		return MouseKeys[key];
	}


	void CSceneNodeAnimatorCameraMaya::allKeysUp()
	{
		for (SINT32 i = 0; i<3; ++i)
			MouseKeys[i] = false;
	}


	//! Sets the rotation speed
	void CSceneNodeAnimatorCameraMaya::setRotateSpeed(FLOAT32 speed)
	{
		RotateSpeed = speed;
	}


	//! Sets the movement speed
	void CSceneNodeAnimatorCameraMaya::setMoveSpeed(FLOAT32 speed)
	{
		TranslateSpeed = speed;
	}


	//! Sets the zoom speed
	void CSceneNodeAnimatorCameraMaya::setZoomSpeed(FLOAT32 speed)
	{
		ZoomSpeed = speed;
	}


	//! Set the distance
	void CSceneNodeAnimatorCameraMaya::setDistance(FLOAT32 distance)
	{
		CurrentZoom = distance;
	}


	//! Gets the rotation speed
	FLOAT32 CSceneNodeAnimatorCameraMaya::getRotateSpeed() const
	{
		return RotateSpeed;
	}


	// Gets the movement speed
	FLOAT32 CSceneNodeAnimatorCameraMaya::getMoveSpeed() const
	{
		return TranslateSpeed;
	}


	//! Gets the zoom speed
	FLOAT32 CSceneNodeAnimatorCameraMaya::getZoomSpeed() const
	{
		return ZoomSpeed;
	}


	//! Returns the current distance, i.e. orbit radius
	FLOAT32 CSceneNodeAnimatorCameraMaya::getDistance() const
	{
		return CurrentZoom;
	}


	ISceneNodeAnimator* CSceneNodeAnimatorCameraMaya::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorCameraMaya * newAnimator =
			new CSceneNodeAnimatorCameraMaya(CursorControl, RotateSpeed, ZoomSpeed, TranslateSpeed);
		return newAnimator;
	}
}