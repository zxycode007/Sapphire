#include "SapphireCSceneCollisionManager.h"
#include "SapphireISceneNode.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireITriangleSelector.h"
#include "SapphireSViewFrustum.h"

#include "SapphireOS.h"
#include "SapphireMath.h"


namespace Sapphire
{

	//! constructor
	CSceneCollisionManager::CSceneCollisionManager(ISceneManager* smanager, IVideoDriver* driver)
		: SceneManager(smanager), Driver(driver)
	{
#ifdef _DEBUG
		setDebugName("CSceneCollisionManager");
#endif

		if (Driver)
			Driver->grab();
	}


	//! destructor
	CSceneCollisionManager::~CSceneCollisionManager()
	{
		if (Driver)
			Driver->drop();
	}


	//! Returns the scene node, which is currently visible at the given
	//! screen coordinates, viewed from the currently active camera.
	ISceneNode* CSceneCollisionManager::getSceneNodeFromScreenCoordinatesBB(
		const Position2d& pos, SINT32 idBitMask, bool noDebugObjects, ISceneNode* root)
	{
		const line3d<Real> ln = getRayFromScreenCoordinates(pos, 0);

		if (ln.start == ln.end)
			return 0;

		return getSceneNodeFromRayBB(ln, idBitMask, noDebugObjects, root);
	}


	//! Returns the nearest scene node which collides with a 3d ray and
	//! which id matches a bitmask.
	ISceneNode* CSceneCollisionManager::getSceneNodeFromRayBB(
		const line3df& ray,
		SINT32 idBitMask, bool noDebugObjects, ISceneNode* root)
	{
		ISceneNode* best = 0;
		FLOAT32 dist = FLT_MAX;

		line3df truncatableRay(ray);

		getPickedNodeBB((root == 0) ? SceneManager->getRootSceneNode() : root, truncatableRay,
			idBitMask, noDebugObjects, dist, best);

		return best;
	}


	//! recursive method for going through all scene nodes
	void CSceneCollisionManager::getPickedNodeBB(ISceneNode* root,
		line3d<Real>& ray, SINT32 bits, bool noDebugObjects,
		FLOAT32& outbestdistance, ISceneNode*& outbestnode)
	{
		const ISceneNodeList& children = root->getChildren();
		Vector3 v = ray.getVector();
		v.normalise();
		const Vector3 rayVector = v;
		 

		ISceneNodeList::const_iterator it = children.begin();
		for (; it != children.end(); ++it)
		{
			ISceneNode* current = *it;

			if (current->isVisible())
			{
				if ((noDebugObjects ? !current->isDebugObject() : true) &&
					(bits == 0 || (bits != 0 && (current->getID() & bits))))
				{
					// get world to object space transform
					Matrix4 worldToObject;
					//if (!current->getAbsoluteTransformation().getInverse(worldToObject))
					if (!current->getAbsoluteTransformation().getInverse(worldToObject))
						continue;

					// transform vector from world space to object space
					line3df objectRay(ray);
					//worldToObject.transformVect(objectRay.start);
					objectRay.start = worldToObject.transformAffine(objectRay.start);
					//worldToObject.transformVect(objectRay.end);
					objectRay.end = worldToObject.transformAffine(objectRay.end);

					const AxisAlignedBox & objectBox = current->getBoundingBox();

					// Do the initial intersection test in object space, since the
					// object space box test is more accurate.
					//if (objectBox.isPointInside(objectRay.start))
					if (objectBox.contains(objectRay.start))
					{
						// use fast bbox intersection to find distance to hitpoint
						// algorithm from Kay et al., code from gamedev.net
						const Vector3 dir = (objectRay.end - objectRay.start).normalize();
						//const Vector3 minDist = (objectBox.MinEdge - objectRay.start) / dir;
						const Vector3 minDist = (objectBox.getMinimum() - objectRay.start) / dir;
						//const Vector3 maxDist = (objectBox.MaxEdge - objectRay.start) / dir;
						const Vector3 maxDist = (objectBox.getMaximum() - objectRay.start) / dir;
						//const Vector3 realMin( min_(minDist.x, maxDist.x), Math::min_(minDist.y, maxDist.y), Math::min_(minDist.z, maxDist.z));
						const Vector3 realMin(Math::min_(minDist.x, maxDist.x), Math::min_(minDist.y, maxDist.y), Math::min_(minDist.z, maxDist.z));
						//const Vector3 realMax(max_(minDist.x, maxDist.x), max_(minDist.y, maxDist.y), max_(minDist.z, maxDist.z));
						const Vector3 realMax(Math::max_(minDist.x, maxDist.x), Math::max_(minDist.y, maxDist.y), Math::max_(minDist.z, maxDist.z));

						//const FLOAT32 minmax = min_(realMax.x, realMax.y, realMax.z);
						const FLOAT32 minmax = Math::min3(realMax.x, realMax.y, realMax.z);
						// nearest distance to intersection
						//const FLOAT32 maxmin = max_(realMin.x, realMin.y, realMin.z);
						const FLOAT32 maxmin = Math::max3(realMin.x, realMin.y, realMin.z);

						const FLOAT32 toIntersectionSq = (maxmin>0 ? maxmin*maxmin : minmax*minmax);
						if (toIntersectionSq < outbestdistance)
						{
							outbestdistance = toIntersectionSq;
							outbestnode = current;

							// And we can truncate the ray to stop us hitting further nodes.
							ray.end = ray.start + (rayVector * sqrtf(toIntersectionSq));
						}
					}
					else
						if (objectBox.intersectsWithLine(objectRay))
						{
							// Now transform into world space, since we need to use world space
							// scales and distances.
							AxisAlignedBox worldBox(objectBox);
							//current->getAbsoluteTransformation().transformBox(worldBox);
							worldBox.transform(current->getAbsoluteTransformation());

							//const Vector3 edges[8];
							//worldBox .getEdges(edges);
							const Vector3* edges = worldBox.getAllCorners();

							/* We need to check against each of 6 faces, composed of these corners:
							/3--------/7     
							/  |      / |
							/   |     /  |
							1---------5  |
							|   2- - -| -6
							|  /      |  /
							|/        | /
							0---------4/
							1-----2
							/|    /|
							/ |   / |
							5-----4  |
							|  0--|--3
							| /   | /
							|/    |/
							6-----7
							Note that we define them as opposite pairs of faces.
							*/
							/*
							static const SINT32 faceEdges[6][3] =
							{
								{ 0, 1, 5 }, // Front
								{ 6, 7, 3 }, // Back
								{ 2, 3, 1 }, // Left
								{ 4, 5, 7 }, // Right
								{ 1, 3, 7 }, // Top
								{ 2, 0, 4 }  // Bottom
							};
							*/
							static const SINT32 faceEdges[6][3] =
							{
								{ 6, 5, 4 }, // Front
								{ 3, 2, 1 }, // Back
								{ 0, 1, 5 }, // Left
								{ 7, 4, 2 }, // Right
								{ 5, 1, 2 }, // Top
								{ 0, 6, 7 }  // Bottom
							};
							Vector3 intersection;
							Plane facePlane;//plane3df facePlane;
							FLOAT32 bestDistToBoxBorder = FLT_MAX;
							FLOAT32 bestToIntersectionSq = FLT_MAX;

							for (SINT32 face = 0; face < 6; ++face)
							{
								 
								//facePlane.setPlane(edges[faceEdges[face][0]],
								//	edges[faceEdges[face][1]],
								//	edges[faceEdges[face][2]]);
								facePlane.redefine(edges[faceEdges[face][0]],
									edges[faceEdges[face][1]],
									edges[faceEdges[face][2]]);

								// Only consider lines that might be entering through this face, since we
								// already know that the start point is outside the box.
								if (facePlane.classifyPointRelation(ray.start) != ISREL3D_FRONT)
									continue;

								// Don't bother using a limited ray, since we already know that it should be long
								// enough to intersect with the box.
								//if (facePlane.getIntersectionWithLine(ray.start, rayVector, intersection))
								if (facePlane.getIntersectionWithLine(ray.start, rayVector, intersection))
								{
									//const FLOAT32 toIntersectionSq = ray.start.getDistanceFromSQ(intersection);
									const FLOAT32 toIntersectionSq = ray.start.squaredDistance(intersection);
									if (toIntersectionSq < outbestdistance)
									{
										// We have to check that the intersection with this plane is actually
										// on the box, so need to go back to object space again.
										//worldToObject.transformAffine(intersection);
										intersection = worldToObject.transformAffine(intersection);

										// find the closest point on the box borders. Have to do this as exact checks will fail due to floating point problems.
										//FLOAT32 distToBorder = max_(min_(abs_(objectBox.MinEdge.x - intersection.x), abs_(objectBox.MaxEdge.x - intersection.x)),
										//	min_(abs_(objectBox.MinEdge.y - intersection.y), abs_(objectBox.MaxEdge.y - intersection.y)),
										//	min_(abs_(objectBox.MinEdge.z - intersection.z), abs_(objectBox.MaxEdge.z - intersection.z)));
										FLOAT32 distToBorder = Math::max3(Math::min_(abs(objectBox.getMinimum().x - intersection.x), abs(objectBox.getMaximum().x - intersection.x)),
											Math::min_(abs(objectBox.getMinimum().y - intersection.y), abs(objectBox.getMaximum().y - intersection.y)),
											Math::min_(abs(objectBox.getMinimum().z - intersection.z), abs(objectBox.getMaximum().z - intersection.z)));
										if (distToBorder < bestDistToBoxBorder)
										{
											bestDistToBoxBorder = distToBorder;
											bestToIntersectionSq = toIntersectionSq;
										}
									}
								}

								// If the ray could be entering through the first face of a pair, then it can't
								// also be entering through the opposite face, and so we can skip that face.
								if (!(face & 0x01))
									++face;
							}

							if (bestDistToBoxBorder < FLT_MAX)
							{
								outbestdistance = bestToIntersectionSq;
								outbestnode = current;

								// If we got a hit, we can now truncate the ray to stop us hitting further nodes.
								ray.end = ray.start + (rayVector * sqrtf(outbestdistance));
							}
						}
				}

				// Only check the children if this node is visible.
				getPickedNodeBB(current, ray, bits, noDebugObjects, outbestdistance, outbestnode);
			}
		}
	}


	ISceneNode* CSceneCollisionManager::getSceneNodeAndCollisionPointFromRay(
		line3df ray,
		Vector3 & outCollisionPoint,
		triangle3df & outTriangle,
		SINT32 idBitMask,
		ISceneNode * collisionRootNode,
		bool noDebugObjects)
	{
		ISceneNode* bestNode = 0;
		FLOAT32 bestDistanceSquared = FLT_MAX;

		if (0 == collisionRootNode)
			collisionRootNode = SceneManager->getRootSceneNode();

		// We don't try to do anything too clever, like sorting the candidate
		// nodes by distance to bounding-box. In the example below, we could do the
		// triangle collision check with node A first, but we'd have to check node B
		// anyway, as the actual collision point could be (and is) closer than the
		// collision point in node A.
		//
		//    ray end
		//       |
		//   AAAAAAAAAA
		//   A   |
		//   A   |  B
		//   A   |  B
		//   A  BBBBB
		//   A   |
		//   A   |
		//       |
		//       |
		//    ray start
		//
		// We therefore have to do a full BB and triangle collision on every scene
		// node in order to find the nearest collision point, so sorting them by
		// bounding box would be pointless.

		getPickedNodeFromBBAndSelector(collisionRootNode, ray, idBitMask,
			noDebugObjects, bestDistanceSquared, bestNode,
			outCollisionPoint, outTriangle);
		return bestNode;
	}


	void CSceneCollisionManager::getPickedNodeFromBBAndSelector(
		ISceneNode * root,
		line3df & ray,
		SINT32 bits,
		bool noDebugObjects,
		FLOAT32 & outBestDistanceSquared,
		ISceneNode * & outBestNode,
		Vector3 & outBestCollisionPoint,
		triangle3df & outBestTriangle)
	{
		const ISceneNodeList& children = root->getChildren();

		ISceneNodeList::const_iterator it = children.begin();
		for (; it != children.end(); ++it)
		{
			ISceneNode* current = *it;
			ITriangleSelector * selector = current->getTriangleSelector();

			if (selector && current->isVisible() &&
				(noDebugObjects ? !current->isDebugObject() : true) &&
				(bits == 0 || (bits != 0 && (current->getID() & bits))))
			{
				// get world to object space transform
				Matrix4 mat;
				if (!current->getAbsoluteTransformation().getInverse(mat))
					continue;

				// transform vector from world space to object space
				line3df line(ray);
				line.start = mat.transformAffine(line.start);
				line.end = mat.transformAffine(line.end);

				const AxisAlignedBox& box = current->getBoundingBox();

				Vector3 candidateCollisionPoint;
				triangle3df candidateTriangle;

				// do intersection test in object space
				ISceneNode * hitNode = 0;
				if (box.intersectsWithLine(line) &&
					getCollisionPoint(ray, selector, candidateCollisionPoint, candidateTriangle, hitNode))
				{
					const FLOAT32 distanceSquared = (candidateCollisionPoint - ray.start).squaredLength();

					if (distanceSquared < outBestDistanceSquared)
					{
						outBestDistanceSquared = distanceSquared;
						outBestNode = current;
						outBestCollisionPoint = candidateCollisionPoint;
						outBestTriangle = candidateTriangle;
						const Vector3 rayVector = ray.getVector().normalize();
						ray.end = ray.start + (rayVector * sqrtf(distanceSquared));
					}
				}
			}

			getPickedNodeFromBBAndSelector(current, ray, bits, noDebugObjects,
				outBestDistanceSquared, outBestNode,
				outBestCollisionPoint, outBestTriangle);
		}
	}


	//! Returns the scene node, at which the overgiven camera is looking at and
	//! which id matches the bitmask.
	ISceneNode* CSceneCollisionManager::getSceneNodeFromCameraBB(
		ICameraSceneNode* camera, SINT32 idBitMask, bool noDebugObjects)
	{
		if (!camera)
			return 0;

		const Vector3 start = camera->getAbsolutePosition();
		Vector3 end = camera->getTarget();

		end = start + ((end - start).normalize() * camera->getFarValue());

		return getSceneNodeFromRayBB(line3df(start, end), idBitMask, noDebugObjects);
	}


	//! Finds the collision point of a line and lots of triangles, if there is one.
	bool CSceneCollisionManager::getCollisionPoint(const line3df& ray,
		ITriangleSelector* selector, Vector3& outIntersection,
		triangle3df& outTriangle,
		ISceneNode*& outNode)
	{
		if (!selector)
		{
			;
			return false;
		}

		SINT32 totalcnt = selector->getTriangleCount();
		if (totalcnt <= 0)
			return false;

		//Triangles.set_used(totalcnt);
		Triangles.reserve(totalcnt);

		SINT32 cnt = 0;
		selector->getTriangles(Triangles.begin()._Ptr, totalcnt, cnt, ray);

		const Vector3 linevect = ray.getVector().normalize();
		Vector3 intersection;
		FLOAT32 nearest = FLT_MAX;
		bool found = false;
		const FLOAT32 raylength = ray.getLengthSQ();

		const FLOAT32 minX = Math::min_(ray.start.x, ray.end.x);
		const FLOAT32 maxX = Math::max_(ray.start.x, ray.end.x);
		const FLOAT32 minY = Math::min_(ray.start.y, ray.end.y);
		const FLOAT32 maxY = Math::max_(ray.start.y, ray.end.y);
		const FLOAT32 minZ = Math::min_(ray.start.z, ray.end.z);
		const FLOAT32 maxZ = Math::max_(ray.start.z, ray.end.z);

		for (SINT32 i = 0; i<cnt; ++i)
		{
			const triangle3df & triangle = Triangles[i];

			if (minX > triangle.pointA.x && minX > triangle.pointB.x && minX > triangle.pointC.x)
				continue;
			if (maxX < triangle.pointA.x && maxX < triangle.pointB.x && maxX < triangle.pointC.x)
				continue;
			if (minY > triangle.pointA.y && minY > triangle.pointB.y && minY > triangle.pointC.y)
				continue;
			if (maxY < triangle.pointA.y && maxY < triangle.pointB.y && maxY < triangle.pointC.y)
				continue;
			if (minZ > triangle.pointA.z && minZ > triangle.pointB.z && minZ > triangle.pointC.z)
				continue;
			if (maxZ < triangle.pointA.z && maxZ < triangle.pointB.z && maxZ < triangle.pointC.z)
				continue;

			if (triangle.getIntersectionWithLine(ray.start, linevect, intersection))
			{
				//const FLOAT32 tmp = intersection.getDistanceFromSQ(ray.start);
				//const FLOAT32 tmp2 = intersection.getDistanceFromSQ(ray.end);
				const FLOAT32 tmp = intersection.squaredDistance(ray.start);
				const FLOAT32 tmp2 = intersection.squaredDistance(ray.end);

				if (tmp < raylength && tmp2 < raylength && tmp < nearest)
				{
					nearest = tmp;
					outTriangle = triangle;
					outIntersection = intersection;
					outNode = selector->getSceneNodeForTriangle(i);
					found = true;
				}
			}
		}

		;
		return found;
	}


	//! Collides a moving ellipsoid with a 3d world with gravity and returns
	//! the resulting new position of the ellipsoid.
	Vector3 CSceneCollisionManager::getCollisionResultPosition(
		ITriangleSelector* selector,
		const Vector3 &position, const Vector3& radius,
		const Vector3& direction,
		triangle3df& triout,
		Vector3& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode,
		FLOAT32 slidingSpeed,
		const Vector3& gravity)
	{
		return collideEllipsoidWithWorld(selector, position,
			radius, direction, slidingSpeed, gravity, triout, hitPosition, outFalling, outNode);
	}


	bool CSceneCollisionManager::testTriangleIntersection(SCollisionData* colData,
		const triangle3df& triangle)
	{
		const Plane trianglePlane = triangle.getPlane();

		// only check front facing polygons
		if (!trianglePlane.isFrontFacing(colData->normalizedVelocity))
			return false;

		// get interval of plane intersection

		FLOAT32 t1, t0;
		bool embeddedInPlane = false;

		// calculate signed distance from sphere position to triangle plane
		//FLOAT32 signedDistToTrianglePlane = trianglePlane.getDistanceTo(
		//	colData->basePoint);
		FLOAT32 signedDistToTrianglePlane = trianglePlane.getDistance(
			colData->basePoint);

		FLOAT32 normalDotVelocity =
			trianglePlane.normal.dotProduct(colData->velocity);

		if (Math::iszero(normalDotVelocity))
		{
			// sphere is traveling parallel to plane

			if (fabs(signedDistToTrianglePlane) >= 1.0f)
				return false; // no collision possible
			else
			{
				// sphere is embedded in plane
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			normalDotVelocity = reciprocal(normalDotVelocity);

			// N.D is not 0. Calculate intersection interval
			t0 = (-1.f - signedDistToTrianglePlane) * normalDotVelocity;
			t1 = (1.f - signedDistToTrianglePlane) * normalDotVelocity;

			// Swap so t0 < t1
			if (t0 > t1) { FLOAT32 tmp = t1; t1 = t0; t0 = tmp; }

			// check if at least one value is within the range
			if (t0 > 1.0f || t1 < 0.0f)
				return false; // both t values are outside 1 and 0, no collision possible

			// clamp to 0 and 1
			t0 = Math::Clamp(t0, 0.f, 1.f);
			t1 = Math::Clamp(t1, 0.f, 1.f);
		}

		// at this point we have t0 and t1, if there is any intersection, it
		// is between this interval
		Vector3 collisionPoint;
		bool foundCollision = false;
		FLOAT32 t = 1.0f;

		// first check the easy case: Collision within the triangle;
		// if this happens, it must be at t0 and this is when the sphere
		// rests on the front side of the triangle plane. This can only happen
		// if the sphere is not embedded in the triangle plane.

		if (!embeddedInPlane)
		{
			Vector3 planeIntersectionPoint =
				(colData->basePoint - trianglePlane.normal )
				+ (colData->velocity * t0);

			if (triangle.isPointInside(planeIntersectionPoint))
			{
				foundCollision = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}

		// if we havent found a collision already we will have to sweep
		// the sphere against points and edges of the triangle. Note: A
		// collision inside the triangle will always happen before a
		// vertex or edge collision.

		if (!foundCollision)
		{
			Vector3 velocity = colData->velocity;
			Vector3 base = colData->basePoint;

			//FLOAT32 velocitySqaredLength = velocity.getLengthSQ();
			FLOAT32 velocitySqaredLength = velocity.squaredLength();
			FLOAT32 a, b, c;
			FLOAT32 newT;

			// for each edge or vertex a quadratic equation has to be solved:
			// a*t^2 + b*t + c = 0. We calculate a,b, and c for each test.

			// check against points
			a = velocitySqaredLength;

			// p1
			b = 2.0f * (velocity.dotProduct(base - triangle.pointA));
			//c = (triangle.pointA - base).getLengthSQ() - 1.f;
			c = (triangle.pointA - base).squaredLength() - 1.f;
			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointA;
			}

			// p2
			if (!foundCollision)
			{
				b = 2.0f * (velocity.dotProduct(base - triangle.pointB));
				//c = (triangle.pointB - base).getLengthSQ() - 1.f;
				c = (triangle.pointB - base).squaredLength() - 1.f;
				if (getLowestRoot(a, b, c, t, &newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.pointB;
				}
			}

			// p3
			if (!foundCollision)
			{
				b = 2.0f * (velocity.dotProduct(base - triangle.pointC));
				//c = (triangle.pointC - base).getLengthSQ() - 1.f;
				c = (triangle.pointC - base).squaredLength() - 1.f;
				if (getLowestRoot(a, b, c, t, &newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.pointC;
				}
			}

			// check against edges:

			// p1 --- p2
			Vector3 edge = triangle.pointB - triangle.pointA;
			Vector3 baseToVertex = triangle.pointA - base;
			//FLOAT32 edgeSqaredLength = edge.getLengthSQ();
			FLOAT32 edgeSqaredLength = edge.squaredLength();
			FLOAT32 edgeDotVelocity = edge.dotProduct(velocity);
			FLOAT32 edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

			// calculate parameters for equation
			a = edgeSqaredLength* -velocitySqaredLength +
				edgeDotVelocity*edgeDotVelocity;
			b = edgeSqaredLength* (2.f *velocity.dotProduct(baseToVertex)) -
				2.0f*edgeDotVelocity*edgeDotBaseToVertex;
			//c = edgeSqaredLength* (1.f - baseToVertex.getLengthSQ()) +
			//	edgeDotBaseToVertex*edgeDotBaseToVertex;
			c = edgeSqaredLength* (1.f - baseToVertex.squaredLength()) +
				edgeDotBaseToVertex*edgeDotBaseToVertex;

			// does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT))
			{
				FLOAT32 f = (edgeDotVelocity*newT - edgeDotBaseToVertex) / edgeSqaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// intersection took place within segment
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.pointA + (edge*f);
				}
			}

			// p2 --- p3
			edge = triangle.pointC - triangle.pointB;
			baseToVertex = triangle.pointB - base;
			//edgeSqaredLength = edge.getLengthSQ();
			edgeSqaredLength = edge.squaredLength();
			edgeDotVelocity = edge.dotProduct(velocity);
			edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

			// calculate parameters for equation
			a = edgeSqaredLength* -velocitySqaredLength +
				edgeDotVelocity*edgeDotVelocity;
			b = edgeSqaredLength* (2 * velocity.dotProduct(baseToVertex)) -
				2.0f*edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSqaredLength* (1 - baseToVertex.squaredLength()) +
				edgeDotBaseToVertex*edgeDotBaseToVertex;

			// does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT))
			{
				FLOAT32 f = (edgeDotVelocity*newT - edgeDotBaseToVertex) /
					edgeSqaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// intersection took place within segment
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.pointB + (edge*f);
				}
			}


			// p3 --- p1
			edge = triangle.pointA - triangle.pointC;
			baseToVertex = triangle.pointC - base;
			edgeSqaredLength = edge.squaredLength();
			edgeDotVelocity = edge.dotProduct(velocity);
			edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

			// calculate parameters for equation
			a = edgeSqaredLength* -velocitySqaredLength +
				edgeDotVelocity*edgeDotVelocity;
			b = edgeSqaredLength* (2 * velocity.dotProduct(baseToVertex)) -
				2.0f*edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSqaredLength* (1 - baseToVertex.squaredLength()) +
				edgeDotBaseToVertex*edgeDotBaseToVertex;

			// does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT))
			{
				FLOAT32 f = (edgeDotVelocity*newT - edgeDotBaseToVertex) /
					edgeSqaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// intersection took place within segment
					t = newT;
					foundCollision = true;
					collisionPoint = triangle.pointC + (edge*f);
				}
			}
		}// end no collision found

		// set result:
		if (foundCollision)
		{
			// distance to collision is t
			FLOAT32 distToCollision = t*colData->velocity.length();

			// does this triangle qualify for closest hit?
			if (!colData->foundCollision ||
				distToCollision	< colData->nearestDistance)
			{
				colData->nearestDistance = distToCollision;
				colData->intersectionPoint = collisionPoint;
				colData->foundCollision = true;
				colData->intersectionTriangle = triangle;
				++colData->triangleHits;
				return true;
			}
		}// end found collision

		return false;
	}


	//! Collides a moving ellipsoid with a 3d world with gravity and returns
	//! the resulting new position of the ellipsoid.
	Vector3 CSceneCollisionManager::collideEllipsoidWithWorld(
		ITriangleSelector* selector, const Vector3 &position,
		const Vector3& radius, const Vector3& velocity,
		FLOAT32 slidingSpeed,
		const Vector3& gravity,
		triangle3df& triout,
		Vector3& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode)
	{
		if (!selector || radius.x == 0.0f || radius.y == 0.0f || radius.z == 0.0f)
			return position;

		// This code is based on the paper "Improved Collision detection and Response"
		// by Kasper Fauerby, but some parts are modified.

		SCollisionData colData;
		colData.R3Position = position;
		colData.R3Velocity = velocity;
		colData.eRadius = radius;
		colData.nearestDistance = FLT_MAX;
		colData.selector = selector;
		colData.slidingSpeed = slidingSpeed;
		colData.triangleHits = 0;
		colData.triangleIndex = -1;

		Vector3 eSpacePosition = colData.R3Position / colData.eRadius;
		Vector3 eSpaceVelocity = colData.R3Velocity / colData.eRadius;

		// iterate until we have our final position

		Vector3 finalPos = collideWithWorld(
			0, colData, eSpacePosition, eSpaceVelocity);

		outFalling = false;

		// add gravity

		if (gravity != Vector3(0, 0, 0))
		{
			colData.R3Position = finalPos * colData.eRadius;
			colData.R3Velocity = gravity;
			colData.triangleHits = 0;

			eSpaceVelocity = gravity / colData.eRadius;

			finalPos = collideWithWorld(0, colData,
				finalPos, eSpaceVelocity);

			outFalling = (colData.triangleHits == 0);
		}

		if (colData.triangleHits)
		{
			triout = colData.intersectionTriangle;
			triout.pointA *= colData.eRadius;
			triout.pointB *= colData.eRadius;
			triout.pointC *= colData.eRadius;
			outNode = selector->getSceneNodeForTriangle(colData.triangleIndex);
		}

		finalPos *= colData.eRadius;
		hitPosition = colData.intersectionPoint * colData.eRadius;
		return finalPos;
	}


	Vector3 CSceneCollisionManager::collideWithWorld(SINT32 recursionDepth,
		SCollisionData &colData, Vector3 pos, Vector3 vel)
	{
		FLOAT32 veryCloseDistance = colData.slidingSpeed;

		if (recursionDepth > 5)
			return pos;

		colData.velocity = vel;
		colData.normalizedVelocity = vel;
		colData.normalizedVelocity.normalize();
		colData.basePoint = pos;
		colData.foundCollision = false;
		colData.nearestDistance = FLT_MAX;

		//------------------ collide with world

		// get all triangles with which we might collide
		AxisAlignedBox box(colData.R3Position);
		box.addInternalPoint(colData.R3Position + colData.R3Velocity);
		//box.MinEdge -= colData.eRadius;
		//box.MaxEdge += colData.eRadius;
		box.setMinimum(box.getMinimum() - colData.eRadius);
		box.setMaximum(box.getMaximum() + colData.eRadius);

		SINT32 totalTriangleCnt = colData.selector->getTriangleCount();
		//Triangles.set_used(totalTriangleCnt);
		Triangles.reserve(totalTriangleCnt);

		Matrix4 scaleMatrix;
		scaleMatrix.setScale(
			Vector3(1.0f / colData.eRadius.x,
			1.0f / colData.eRadius.y,
			1.0f / colData.eRadius.z));

		SINT32 triangleCnt = 0;
		//colData.selector->getTriangles(Triangles.pointer(), totalTriangleCnt, triangleCnt, box, &scaleMatrix);
		colData.selector->getTriangles(Triangles.begin()._Ptr, totalTriangleCnt, triangleCnt, box, &scaleMatrix);

		for (SINT32 i = 0; i<triangleCnt; ++i)
			if (testTriangleIntersection(&colData, Triangles[i]))
				colData.triangleIndex = i;

		//---------------- end collide with world

		if (!colData.foundCollision)
			return pos + vel;

		// original destination point
		const Vector3 destinationPoint = pos + vel;
		Vector3 newBasePoint = pos;

		// only update if we are not already very close
		// and if so only move very close to intersection, not to the
		// exact point
		if (colData.nearestDistance >= veryCloseDistance)
		{
			Vector3 v = vel;
			//v.setLength(colData.nearestDistance - veryCloseDistance);
			v.setLength(colData.nearestDistance - veryCloseDistance);
			newBasePoint = colData.basePoint + v;

			v.normalize();
			colData.intersectionPoint -= (v * veryCloseDistance);
		}

		// calculate sliding plane

		const Vector3 slidePlaneOrigin = colData.intersectionPoint;
		const Vector3 slidePlaneNormal = (newBasePoint - colData.intersectionPoint).normalize();
		Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

		Vector3 newDestinationPoint =
			destinationPoint -
			(slidePlaneNormal * slidingPlane.getDistance(destinationPoint));

		// generate slide vector

		const Vector3 newVelocityVector = newDestinationPoint -
			colData.intersectionPoint;

		if (newVelocityVector.length() < veryCloseDistance)
			return newBasePoint;

		return collideWithWorld(recursionDepth + 1, colData,
			newBasePoint, newVelocityVector);
	}


	//! Returns a 3d ray which would go through the 2d screen coodinates.
	line3df CSceneCollisionManager::getRayFromScreenCoordinates(
		const Position2d & pos, ICameraSceneNode* camera)
	{
		line3df ln(0, 0, 0, 0, 0, 0);

		if (!SceneManager)
			return ln;

		if (!camera)
			camera = SceneManager->getActiveCamera();

		if (!camera)
			return ln;

		const SViewFrustum* f = camera->getViewFrustum();

		Vector3 farLeftUp = f->getFarLeftUp();
		Vector3 lefttoright = f->getFarRightUp() - farLeftUp;
		Vector3 uptodown = f->getFarLeftDown() - farLeftUp;

		const rect<SINT32>& viewPort = Driver->getViewPort();
		dimension2d<UINT32> screenSize(viewPort.getWidth(), viewPort.getHeight());

		FLOAT32 dx = pos.x / (FLOAT32)screenSize.Width;
		FLOAT32 dy = pos.y / (FLOAT32)screenSize.Height;

		if (camera->isOrthogonal())
			ln.start = f->cameraPosition + (lefttoright * (dx - 0.5f)) + (uptodown * (dy - 0.5f));
		else
			ln.start = f->cameraPosition;

		ln.end = farLeftUp + (lefttoright * dx) + (uptodown * dy);

		return ln;
	}


	//! Calculates 2d screen position from a 3d position.
	Position2d CSceneCollisionManager::getScreenCoordinatesFrom3DPosition(
		const Vector3 & pos3d, ICameraSceneNode* camera, bool useViewPort)
	{
		if (!SceneManager || !Driver)
			return Position2d(-1000, -1000);

		if (!camera)
			camera = SceneManager->getActiveCamera();

		if (!camera)
			return Position2d(-1000, -1000);

		dimension2d<UINT32> dim;
		if (useViewPort)
			dim.set(Driver->getViewPort().getWidth(), Driver->getViewPort().getHeight());
		else
			dim = (Driver->getCurrentRenderTargetSize());

		dim.Width /= 2;
		dim.Height /= 2;

		Matrix4 trans = camera->getProjectionMatrix();
		trans = camera->getViewMatrix() * trans;

		//FLOAT32 transformedPos[4] = { pos3d.x, pos3d.y, pos3d.z, 1.0f };
		Vector4 transformedPos = { pos3d.x, pos3d.y, pos3d.z, 1.0f };

		//trans.multiplyWith1x4Matrix(transformedPos);
		transformedPos = trans * transformedPos;

		if (transformedPos[3] < 0)
			return Position2d(-10000, -10000);

		const FLOAT32 zDiv = transformedPos[3] == 0.0f ? 1.0f :
			reciprocal(transformedPos[3]);

		return Position2d(
			dim.Width + round32(dim.Width * (transformedPos[0] * zDiv)),
			dim.Height - round32(dim.Height * (transformedPos[1] * zDiv)));
	}


	inline bool CSceneCollisionManager::getLowestRoot(FLOAT32 a, FLOAT32 b, FLOAT32 c, FLOAT32 maxR, FLOAT32* root)
	{
		// check if solution exists
		const FLOAT32 determinant = b*b - 4.0f*a*c;

		// if determinant is negative, no solution
		if (determinant < 0.0f || a == 0.f)
			return false;

		// calculate two roots: (if det==0 then x1==x2
		// but lets disregard that slight optimization)

		const FLOAT32 sqrtD = sqrtf(determinant);
		const FLOAT32 invDA = reciprocal(2 * a);
		FLOAT32 r1 = (-b - sqrtD) * invDA;
		FLOAT32 r2 = (-b + sqrtD) * invDA;

		// sort so x1 <= x2
		if (r1 > r2)
			swap(r1, r2);

		// get lowest root
		if (r1 > 0 && r1 < maxR)
		{
			*root = r1;
			return true;
		}

		// its possible that we want x2, this can happen if x1 < 0
		if (r2 > 0 && r2 < maxR)
		{
			*root = r2;
			return true;
		}

		return false;
	}
}