#include "SapphireCSceneNodeAnimatorRotation.h"

#include "SapphireISceneNode.h"

namespace Sapphire
{

	//! constructor
	CSceneNodeAnimatorRotation::CSceneNodeAnimatorRotation(UINT32 time, const Vector3& rotation)
		: Rotation(rotation), StartTime(time)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorRotation");
#endif
	}


	//! animates a scene node
	void CSceneNodeAnimatorRotation::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		if (node) // thanks to warui for this fix
		{
			const UINT32 diffTime = timeMs - StartTime;

			if (diffTime != 0)
			{
				// clip the rotation to small values, to avoid
				// precision problems with huge floats.
				Vector3 rot = node->getRotation() + Rotation*(diffTime*0.1f);
				if (rot.x>360.f)
					rot.x = fmodf(rot.x, 360.f);
				if (rot.y>360.f)
					rot.y = fmodf(rot.y, 360.f);
				if (rot.z>360.f)
					rot.z = fmodf(rot.z, 360.f);
				node->setRotation(rot);
				StartTime = timeMs;
			}
		}
	}


	//! Writes attributes of the scene node animator.
	void CSceneNodeAnimatorRotation::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Rotation", Rotation);
	}


	//! Reads attributes of the scene node animator.
	void CSceneNodeAnimatorRotation::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Rotation = in->getAttributeAsVector3d("Rotation");
	}


	ISceneNodeAnimator* CSceneNodeAnimatorRotation::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorRotation * newAnimator =
			new CSceneNodeAnimatorRotation(StartTime, Rotation);

		return newAnimator;
	}
}