#include "SapphireCSceneNodeAnimatorFlyCircle.h"

namespace Sapphire
{

	//! constructor
	CSceneNodeAnimatorFlyCircle::CSceneNodeAnimatorFlyCircle(UINT32 time,
		const Vector3& center, FLOAT32 radius, FLOAT32 speed,
		const Vector3& direction, FLOAT32 radiusEllipsoid)
		: Center(center), Direction(direction), Radius(radius),
		RadiusEllipsoid(radiusEllipsoid), Speed(speed), StartTime(time)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorFlyCircle");
#endif
		init();
	}


	void CSceneNodeAnimatorFlyCircle::init()
	{
		Direction.normalise();

		if (Direction.y != 0)
			VecV = Vector3(50, 0, 0).crossProduct(Direction).normalise();
		else
			VecV = Vector3(0, 50, 0).crossProduct(Direction).normalise();
		VecU = VecV.crossProduct(Direction).normalise();
	}


	//! animates a scene node
	void CSceneNodeAnimatorFlyCircle::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		if (0 == node)
			return;

		FLOAT32 time;

		// Check for the condition where the StartTime is in the future.
		if (StartTime > timeMs)
			time = ((SINT32)timeMs - (SINT32)StartTime) * Speed;
		else
			time = (timeMs - StartTime) * Speed;

		//	node->setPosition(Center + Radius * ((VecU*cosf(time)) + (VecV*sinf(time))));
		FLOAT32 r2 = RadiusEllipsoid == 0.f ? Radius : RadiusEllipsoid;
		node->setPosition(Center + (Radius*cosf(time)*VecU) + (r2*sinf(time)*VecV));
	}


	//! Writes attributes of the scene node animator.
	void CSceneNodeAnimatorFlyCircle::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Center", Center);
		out->addFloat("Radius", Radius);
		out->addFloat("Speed", Speed);
		out->addVector3d("Direction", Direction);
		out->addFloat("RadiusEllipsoid", RadiusEllipsoid);
	}


	//! Reads attributes of the scene node animator.
	void CSceneNodeAnimatorFlyCircle::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Center = in->getAttributeAsVector3d("Center");
		Radius = in->getAttributeAsFloat("Radius");
		Speed = in->getAttributeAsFloat("Speed");
		Direction = in->getAttributeAsVector3d("Direction");
		StartTime = 0;

		if (Direction.equals(Vector3(0, 0, 0)))
			Direction.set(0, 1, 0); // irrlicht 1.1 backwards compatibility
		else
			Direction.normalise();

		RadiusEllipsoid = in->getAttributeAsFloat("RadiusEllipsoid");
		init();
	}


	ISceneNodeAnimator* CSceneNodeAnimatorFlyCircle::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorFlyCircle * newAnimator =
			new CSceneNodeAnimatorFlyCircle(StartTime, Center, Radius, Speed, Direction, RadiusEllipsoid);

		return newAnimator;
	}
}