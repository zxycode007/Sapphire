#ifndef _SAPPHIRE_I_SCENE_NODE_COLLISION_RESPONSE_
#define _SAPPHIRE_I_SCENE_NODE_COLLISION_RESPONSE_

#include "SapphireISceneNode.h"

namespace Sapphire
{

	class ISceneNodeAnimatorCollisionResponse;

	//! Callback interface for catching events of collisions.
	/** Implement this interface and use
	ISceneNodeAnimatorCollisionResponse::setCollisionCallback to be able to
	be notified if a collision has occurred.
	**/
	class ICollisionCallback : public virtual IReferenceCounter
	{
	public:

		//! Will be called when a collision occurrs.
		/** See ISceneNodeAnimatorCollisionResponse::setCollisionCallback for more information.
		\param animator: Collision response animator in which the collision occurred. You can call
		this animator's methods to find the node, collisionPoint and/or collision triangle.
		\retval true if the collision was handled in the animator. The animator's target
		node will *not* be stopped at the collision point, but will instead move fully
		to the location that triggered the collision check.
		\retval false if the collision was not handled in the animator. The animator's
		target node will be moved to the collision position.
		*/
		virtual bool onCollision(const ISceneNodeAnimatorCollisionResponse& animator) = 0;
	};

	//! Special scene node animator for doing automatic collision detection and response.
	/** This scene node animator can be attached to any single scene node
	and will then prevent it from moving through specified collision geometry
	(e.g. walls and floors of the) world, as well as having it fall under gravity.
	This animator provides a simple implementation of first person shooter cameras.
	Attach it to a camera, and the camera will behave as the player control in a
	first person shooter game: The camera stops and slides at walls, walks up stairs,
	falls down if there is no floor under it, and so on.

	The animator will treat any change in the position of its target scene
	node as movement, including a setPosition(), as movement.  If you want to
	teleport the target scene node manually to a location without it being effected
	by collision geometry, then call setTargetNode(node) after calling node->setPosition().
	*/
	class ISceneNodeAnimatorCollisionResponse : public ISceneNodeAnimator
	{
	public:

		//! Destructor
		virtual ~ISceneNodeAnimatorCollisionResponse() {}

		//! Check if the attached scene node is falling.
		/** Falling means that there is no blocking wall from the scene
		node in the direction of the gravity. The implementation of
		this method is very fast, no collision detection is done when
		invoking it.
		\return True if the scene node is falling, false if not. */
		virtual bool isFalling() const = 0;

		//! Sets the radius of the ellipsoid for collision detection and response.
		/** If you have a scene node, and you are unsure about how big
		the radius should be, you could use the following code to
		determine it:
		\code
		core::aabbox<f32> box = yourSceneNode->getBoundingBox();
		Vector3 radius = box.MaxEdge - box.getCenter();
		\endcode
		\param radius: New radius of the ellipsoid. */
		virtual void setEllipsoidRadius(const Vector3& radius) = 0;

		//! Returns the radius of the ellipsoid for collision detection and response.
		/** \return Radius of the ellipsoid. */
		virtual Vector3 getEllipsoidRadius() const = 0;

		//! Sets the gravity of the environment.
		/** A good example value would be Vector3(0,-100.0f,0)
		for letting gravity affect all object to fall down. For bigger
		gravity, make increase the length of the vector. You can
		disable gravity by setting it to Vector3(0,0,0);
		\param gravity: New gravity vector. */
		virtual void setGravity(const Vector3& gravity) = 0;

		//! Get current vector of gravity.
		//! \return Gravity vector. */
		virtual Vector3 getGravity() const = 0;

		//! 'Jump' the animator, by adding a jump speed opposite to its gravity
		/** \param jumpSpeed The initial speed of the jump; the velocity will be opposite
		to this animator's gravity vector. */
		virtual void jump(FLOAT32 jumpSpeed) = 0;

		//! Should the Target react on collision ( default = true )
		virtual void setAnimateTarget(bool enable) = 0;
		virtual bool getAnimateTarget() const = 0;

		//! Set translation of the collision ellipsoid.
		/** By default, the ellipsoid for collision detection is
		created around the center of the scene node, which means that
		the ellipsoid surrounds it completely. If this is not what you
		want, you may specify a translation for the ellipsoid.
		\param translation: Translation of the ellipsoid relative
		to the position of the scene node. */
		virtual void setEllipsoidTranslation(const Vector3 &translation) = 0;

		//! Get the translation of the ellipsoid for collision detection.
		/** See
		ISceneNodeAnimatorCollisionResponse::setEllipsoidTranslation()
		for more details.
		\return Translation of the ellipsoid relative to the position
		of the scene node. */
		virtual Vector3 getEllipsoidTranslation() const = 0;

		//! Sets a triangle selector holding all triangles of the world with which the scene node may collide.
		/** \param newWorld: New triangle selector containing triangles
		to let the scene node collide with. */
		virtual void setWorld(ITriangleSelector* newWorld) = 0;

		//! Get the current triangle selector containing all triangles for collision detection.
		virtual ITriangleSelector* getWorld() const = 0;

		//! Set the single node that this animator will act on.
		/** \param node The new target node. Setting this will force the animator to update
		its last target position for the node, allowing setPosition() to teleport
		the node through collision geometry. */
		virtual void setTargetNode(ISceneNode * node) = 0;

		//! Gets the single node that this animator is acting on.
		/** \return The node that this animator is acting on. */
		virtual ISceneNode* getTargetNode(void) const = 0;

		//! Returns true if a collision occurred during the last animateNode()
		virtual bool collisionOccurred() const = 0;

		//! Returns the last point of collision.
		virtual const Vector3 & getCollisionPoint() const = 0;

		//! Returns the last triangle that caused a collision
		virtual const triangle3df & getCollisionTriangle() const = 0;

		//! Returns the position that the target node will be moved to, unless the collision is consumed in a callback.
		/**
		If you have a collision callback registered, and it consumes the collision, then the
		node will ignore the collision and will not stop at this position. Instead, it will
		move fully to the position that caused the collision to occur. */
		virtual const Vector3 & getCollisionResultPosition(void) const = 0;

		//! Returns the node that was collided with.
		virtual ISceneNode* getCollisionNode(void) const = 0;

		//! Sets a callback interface which will be called if a collision occurs.
		/** \param callback: collision callback handler that will be called when a collision
		occurs. Set this to 0 to disable the callback.
		*/
		virtual void setCollisionCallback(ICollisionCallback* callback) = 0;

	};

}

#endif