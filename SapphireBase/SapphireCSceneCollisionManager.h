#ifndef _SAPPHIRE_C_SCENE_COLLISION_MANAGER_
#define _SAPPHIRE_C_SCENE_COLLISION_MANAGER_

#include "SapphireISceneCollisionManager.h"
#include "SapphireIScenesManager.h"
#include "SapphireIVideoDriver.h"

namespace Sapphire
{
	//! The Scene Collision Manager provides methods for performing collision tests and picking on scene nodes.
	class CSceneCollisionManager : public ISceneCollisionManager
	{
	public:

		//! constructor
		CSceneCollisionManager(ISceneManager* smanager, IVideoDriver* driver);

		//! destructor
		virtual ~CSceneCollisionManager();

		//! Returns the scene node, which is currently visible at the given
		//! screen coordinates, viewed from the currently active camera.
		virtual ISceneNode* getSceneNodeFromScreenCoordinatesBB(const Position2d& pos,
			SINT32 idBitMask = 0, bool bNoDebugObjects = false, ISceneNode* root = 0);

		//! Returns the nearest scene node which collides with a 3d ray and
		//! whose id matches a bitmask.
		virtual ISceneNode* getSceneNodeFromRayBB(const line3d<Real>& ray,
			SINT32 idBitMask = 0, bool bNoDebugObjects = false,
			ISceneNode* root = 0);

		//! Returns the scene node, at which the overgiven camera is looking at and
		//! which id matches the bitmask.
		virtual ISceneNode* getSceneNodeFromCameraBB(ICameraSceneNode* camera,
			SINT32 idBitMask = 0, bool bNoDebugObjects = false);

		//! Finds the collision point of a line and lots of triangles, if there is one.
		virtual bool getCollisionPoint(const line3df& ray,
			ITriangleSelector* selector, Vector3& outCollisionPoint,
			triangle3df& outTriangle,
			ISceneNode* & outNode);

		//! Collides a moving ellipsoid with a 3d world with gravity and returns
		//! the resulting new position of the ellipsoid.
		virtual Vector3 getCollisionResultPosition(
			ITriangleSelector* selector,
			const Vector3 &ellipsoidPosition,
			const Vector3& ellipsoidRadius,
			const Vector3& ellipsoidDirectionAndSpeed,
			triangle3df& triout,
			Vector3& hitPosition,
			bool& outFalling,
			ISceneNode*& outNode,
			FLOAT32 slidingSpeed,
			const Vector3& gravityDirectionAndSpeed);

		//! Returns a 3d ray which would go through the 2d screen coodinates.
		virtual line3d<Real> getRayFromScreenCoordinates(
			const Position2d & pos, ICameraSceneNode* camera = 0);

		//! Calculates 2d screen position from a 3d position.
		virtual Position2d getScreenCoordinatesFrom3DPosition(
			const Vector3 & pos, ICameraSceneNode* camera = 0, bool useViewPort = false);

		//! Gets the scene node and nearest collision point for a ray based on
		//! the nodes' id bitmasks, bounding boxes and triangle selectors.
		virtual ISceneNode* getSceneNodeAndCollisionPointFromRay(
			line3df ray,
			Vector3 & outCollisionPoint,
			triangle3df & outTriangle,
			SINT32 idBitMask = 0,
			ISceneNode * collisionRootNode = 0,
			bool noDebugObjects = false);


	private:

		//! recursive method for going through all scene nodes
		void getPickedNodeBB(ISceneNode* root, line3d<Real>& ray, SINT32 bits,
			bool bNoDebugObjects,
			FLOAT32& outbestdistance, ISceneNode*& outbestnode);

		//! recursive method for going through all scene nodes
		void getPickedNodeFromBBAndSelector(ISceneNode * root,
			line3df & ray,
			SINT32 bits,
			bool noDebugObjects,
			FLOAT32 & outBestDistanceSquared,
			ISceneNode * & outBestNode,
			Vector3 & outBestCollisionPoint,
			triangle3df & outBestTriangle);


		struct SCollisionData
		{
			Vector3 eRadius;

			Vector3 R3Velocity;
			Vector3 R3Position;

			Vector3 velocity;
			Vector3 normalizedVelocity;
			Vector3 basePoint;

			bool foundCollision;
			FLOAT32 nearestDistance;
			Vector3 intersectionPoint;

			triangle3df intersectionTriangle;
			SINT32 triangleIndex;
			SINT32 triangleHits;

			FLOAT32 slidingSpeed;

			ITriangleSelector* selector;
		};

		//! Tests the current collision data against an individual triangle.
		/**
		\param colData: the collision data.
		\param triangle: the triangle to test against.
		\return true if the triangle is hit (and is the closest hit), false otherwise */
		bool testTriangleIntersection(SCollisionData* colData,
			const triangle3df& triangle);

		//! recursive method for doing collision response
		Vector3 collideEllipsoidWithWorld(ITriangleSelector* selector,
			const Vector3 &position,
			const Vector3& radius, const Vector3& velocity,
			FLOAT32 slidingSpeed,
			const Vector3& gravity, triangle3df& triout,
			Vector3& hitPosition,
			bool& outFalling,
			ISceneNode*& outNode);

		Vector3 collideWithWorld(SINT32 recursionDepth, SCollisionData &colData,
			Vector3 pos, Vector3 vel);

		inline bool getLowestRoot(FLOAT32 a, FLOAT32 b, FLOAT32 c, FLOAT32 maxR, FLOAT32* root);

		ISceneManager* SceneManager;
		IVideoDriver* Driver;
		vector<triangle3df>::type Triangles; // triangle buffer
	};
}


#endif