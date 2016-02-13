#include "SapphireCSceneNodeAnimatorCollisionResponse.h"
#include "SapphireISceneCollisionManager.h"
#include "SapphireIScenesManager.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireOS.h"

namespace Sapphire
{
	//! constructor
	CSceneNodeAnimatorCollisionResponse::CSceneNodeAnimatorCollisionResponse(
		ISceneManager* scenemanager,
		ITriangleSelector* world, ISceneNode* object,
		const Vector3& ellipsoidRadius,
		const Vector3& gravityPerSecond,
		const Vector3& ellipsoidTranslation,
		FLOAT32 slidingSpeed)
		: Radius(ellipsoidRadius), Gravity(gravityPerSecond), Translation(ellipsoidTranslation),
		World(world), Object(object), SceneManager(scenemanager), LastTime(0),
		SlidingSpeed(slidingSpeed), CollisionNode(0), CollisionCallback(0),
		Falling(false), IsCamera(false), AnimateCameraTarget(true), CollisionOccurred(false),
		FirstUpdate(true)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorCollisionResponse");
#endif

		if (World)
			World->grab();

		setNode(Object);
	}


	//! destructor
	CSceneNodeAnimatorCollisionResponse::~CSceneNodeAnimatorCollisionResponse()
	{
		if (World)
			World->drop();

		if (CollisionCallback)
			CollisionCallback->drop();
	}


	//! Returns if the attached scene node is falling, which means that
	//! there is no blocking wall from the scene node in the direction of
	//! the gravity.
	bool CSceneNodeAnimatorCollisionResponse::isFalling() const
	{
		//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return Falling;
	}


	//! Sets the radius of the ellipsoid with which collision detection and
	//! response is done.
	void CSceneNodeAnimatorCollisionResponse::setEllipsoidRadius(
		const Vector3& radius)
	{
		Radius = radius;
		FirstUpdate = true;
	}


	//! Returns the radius of the ellipsoid with wich the collision detection and
	//! response is done.
	Vector3 CSceneNodeAnimatorCollisionResponse::getEllipsoidRadius() const
	{
		return Radius;
	}


	//! Sets the gravity of the environment.
	void CSceneNodeAnimatorCollisionResponse::setGravity(const Vector3& gravity)
	{
		Gravity = gravity;
		FirstUpdate = true;
	}


	//! Returns current vector of gravity.
	Vector3 CSceneNodeAnimatorCollisionResponse::getGravity() const
	{
		return Gravity;
	}


	//! 'Jump' the animator, by adding a jump speed opposite to its gravity
	void CSceneNodeAnimatorCollisionResponse::jump(FLOAT32 jumpSpeed)
	{
		FallingVelocity -= (Vector3(Gravity).normalise()) * jumpSpeed;
		Falling = true;
	}


	//! Sets the translation of the ellipsoid for collision detection.
	void CSceneNodeAnimatorCollisionResponse::setEllipsoidTranslation(const Vector3 &translation)
	{
		Translation = translation;
	}


	//! Returns the translation of the ellipsoid for collision detection.
	Vector3 CSceneNodeAnimatorCollisionResponse::getEllipsoidTranslation() const
	{
		return Translation;
	}


	//! Sets a triangle selector holding all triangles of the world with which
	//! the scene node may collide.
	void CSceneNodeAnimatorCollisionResponse::setWorld(ITriangleSelector* newWorld)
	{
		if (newWorld)
			newWorld->grab();

		if (World)
			World->drop();

		World = newWorld;

		FirstUpdate = true;
	}


	//! Returns the current triangle selector containing all triangles for
	//! collision detection.
	ITriangleSelector* CSceneNodeAnimatorCollisionResponse::getWorld() const
	{
		return World;
	}


	void CSceneNodeAnimatorCollisionResponse::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		CollisionOccurred = false;

		if (node != Object)
			setNode(node);

		if (!Object || !World)
			return;

		// trigger reset
		if (timeMs == 0)
		{
			FirstUpdate = true;
			timeMs = LastTime;
		}

		if (FirstUpdate)
		{
			LastPosition = Object->getPosition();
			Falling = false;
			LastTime = timeMs;
			FallingVelocity.set(0, 0, 0);

			FirstUpdate = false;
		}

		const UINT32 diff = timeMs - LastTime;
		LastTime = timeMs;

		CollisionResultPosition = Object->getPosition();
		Vector3 vel = CollisionResultPosition - LastPosition;

		FallingVelocity += Gravity * (FLOAT32)diff * 0.001f;

		CollisionTriangle = RefTriangle;
		CollisionPoint = Vector3();
		CollisionResultPosition = Vector3();
		CollisionNode = 0;

		// Vector3 force = vel + FallingVelocity;

		if (AnimateCameraTarget)
		{
			// TODO: divide SlidingSpeed by frame time

			bool f = false;
			CollisionResultPosition
				= SceneManager->getSceneCollisionManager()->getCollisionResultPosition(
				World, LastPosition - Translation,
				Radius, vel, CollisionTriangle, CollisionPoint, f,
				CollisionNode, SlidingSpeed, FallingVelocity);

			CollisionOccurred = (CollisionTriangle != RefTriangle);

			CollisionResultPosition += Translation;

			if (f)//CollisionTriangle == RefTriangle)
			{
				Falling = true;
			}
			else
			{
				Falling = false;
				FallingVelocity.set(0, 0, 0);
			}

			bool collisionConsumed = false;

			if (CollisionOccurred && CollisionCallback)
				collisionConsumed = CollisionCallback->onCollision(*this);

			if (!collisionConsumed)
				Object->setPosition(CollisionResultPosition);
		}

		// move camera target
		if (AnimateCameraTarget && IsCamera)
		{
			const Vector3 pdiff = Object->getPosition() - LastPosition - vel;
			ICameraSceneNode* cam = (ICameraSceneNode*)Object;
			cam->setTarget(cam->getTarget() + pdiff);
		}

		LastPosition = Object->getPosition();
	}


	void CSceneNodeAnimatorCollisionResponse::setNode(ISceneNode* node)
	{
		Object = node;

		if (Object)
		{
			LastPosition = Object->getPosition();
			IsCamera = (Object->getType() == ESNT_CAMERA);
		}

		LastTime = Timer::getTime();
	}


	//! Writes attributes of the scene node animator.
	void CSceneNodeAnimatorCollisionResponse::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Radius", Radius);
		out->addVector3d("Gravity", Gravity);
		out->addVector3d("Translation", Translation);
		out->addBool("AnimateCameraTarget", AnimateCameraTarget);
	}


	//! Reads attributes of the scene node animator.
	void CSceneNodeAnimatorCollisionResponse::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Radius = in->getAttributeAsVector3d("Radius");
		Gravity = in->getAttributeAsVector3d("Gravity");
		Translation = in->getAttributeAsVector3d("Translation");
		AnimateCameraTarget = in->getAttributeAsBool("AnimateCameraTarget");
	}


	ISceneNodeAnimator* CSceneNodeAnimatorCollisionResponse::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		if (!newManager) newManager = SceneManager;

		CSceneNodeAnimatorCollisionResponse * newAnimator =
			new CSceneNodeAnimatorCollisionResponse(newManager, World, Object, Radius,
			(Gravity * 1000.0f), Translation, SlidingSpeed);

		return newAnimator;
	}

	void CSceneNodeAnimatorCollisionResponse::setCollisionCallback(ICollisionCallback* callback)
	{
		if (CollisionCallback == callback)
			return;

		if (CollisionCallback)
			CollisionCallback->drop();

		CollisionCallback = callback;

		if (CollisionCallback)
			CollisionCallback->grab();
	}

	//! Should the Target react on collision ( default = true )
	void CSceneNodeAnimatorCollisionResponse::setAnimateTarget(bool enable)
	{
		AnimateCameraTarget = enable;
		FirstUpdate = true;
	}

	//! Should the Target react on collision ( default = true )
	bool CSceneNodeAnimatorCollisionResponse::getAnimateTarget() const
	{
		return AnimateCameraTarget;
	}
}