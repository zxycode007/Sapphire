#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_COLLISION_RESPONSE_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_COLLISION_RESPONSE_

#include "SapphireISceneNodeAnimatorCollisionResponse.h"

namespace Sapphire
{
	//! Special scene node animator for doing automatic collision detection and response.
	/** This scene node animator can be attached to any scene node modifying it in that
	way, that it cannot move through walls of the world, is influenced by gravity and
	acceleration. This animator is useful for example for first person shooter
	games. Attach it for example to a first person shooter camera, and the camera will
	behave as the player control in a first person shooter game: The camera stops and
	slides at walls, walks up stairs, falls down if there is no floor under it, and so on.
	*/
	class CSceneNodeAnimatorCollisionResponse : public ISceneNodeAnimatorCollisionResponse
	{
	public:

		//! constructor
		CSceneNodeAnimatorCollisionResponse(ISceneManager* scenemanager,
			ITriangleSelector* world, ISceneNode* object,
			const Vector3& ellipsoidRadius = Vector3(30, 60, 30),
			const Vector3& gravityPerSecond = Vector3(0, -100.0f, 0),
			const Vector3& ellipsoidTranslation = Vector3(0, 0, 0),
			FLOAT32 slidingSpeed = 0.0005f);

		//! destructor
		virtual ~CSceneNodeAnimatorCollisionResponse();

		//! Returns if the attached scene node is falling, which means that
		//! there is no blocking wall from the scene node in the direction of
		//! the gravity.
		virtual bool isFalling() const;

		//! Sets the radius of the ellipsoid with which collision detection and
		//! response is done.
		virtual void setEllipsoidRadius(const Vector3& radius);

		//! Returns the radius of the ellipsoid with which the collision detection and
		//! response is done.
		virtual Vector3 getEllipsoidRadius() const;

		//! Sets the gravity of the environment.
		virtual void setGravity(const Vector3& gravity);

		//! 'Jump' the animator, by adding a jump speed opposite to its gravity
		virtual void jump(FLOAT32 jumpSpeed);

		//! Should the Target react on collision ( default = true )
		virtual void setAnimateTarget(bool enable);
		virtual bool getAnimateTarget() const;

		//! Returns current vector of gravity.
		virtual Vector3 getGravity() const;

		//! Sets the translation of the ellipsoid for collision detection.
		virtual void setEllipsoidTranslation(const Vector3 &translation);

		//! Returns the translation of the ellipsoid for collision detection.
		virtual Vector3 getEllipsoidTranslation() const;

		//! Sets a triangle selector holding all triangles of the world with which
		//! the scene node may collide.
		virtual void setWorld(ITriangleSelector* newWorld);

		//! Returns the current triangle selector containing all triangles for
		//! collision detection.
		virtual ITriangleSelector* getWorld() const;

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_COLLISION_RESPONSE; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

		//! Set the single node that this animator will act on.
		virtual void setTargetNode(ISceneNode * node) { setNode(node); }

		//! Gets the single node that this animator is acting on.
		virtual ISceneNode* getTargetNode(void) const { return Object; }

		//! Returns true if a collision occurred during the last animateNode()
		virtual bool collisionOccurred() const { return CollisionOccurred; }

		//! Returns the last point of collision.
		virtual const Vector3 & getCollisionPoint() const { return CollisionPoint; }

		//! Returns the last triangle that caused a collision.
		virtual const triangle3df & getCollisionTriangle() const { return CollisionTriangle; }

		virtual const Vector3 & getCollisionResultPosition(void) const { return CollisionResultPosition; }

		virtual ISceneNode* getCollisionNode(void) const { return CollisionNode; }


		//! Sets a callback interface which will be called if a collision occurs.
		/** \param callback: collision callback handler that will be called when a collision
		occurs. Set this to 0 to disable the callback.
		*/
		virtual void setCollisionCallback(ICollisionCallback* callback);

	private:

		void setNode(ISceneNode* node);

		Vector3 Radius;
		Vector3 Gravity;
		Vector3 Translation;
		Vector3 FallingVelocity; // In the direction of Gravity.

		Vector3 LastPosition;
		triangle3df RefTriangle;

		ITriangleSelector* World;
		ISceneNode* Object;
		ISceneManager* SceneManager;
		UINT32 LastTime;
		FLOAT32 SlidingSpeed;

		Vector3 CollisionPoint;
		triangle3df CollisionTriangle;
		Vector3 CollisionResultPosition;
		ISceneNode * CollisionNode;
		ICollisionCallback* CollisionCallback;

		bool Falling;
		bool IsCamera;
		bool AnimateCameraTarget;
		bool CollisionOccurred;
		bool FirstUpdate;
	};
}

#endif