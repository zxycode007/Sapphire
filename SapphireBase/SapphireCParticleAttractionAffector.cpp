#include "SapphireCParticleAttractionAffector.h"
#include "SapphireIAttributes.h"

namespace Sapphire
{

	//! constructor
	CParticleAttractionAffector::CParticleAttractionAffector(
		const Vector3& point, FLOAT32 speed, bool attract,
		bool affectX, bool affectY, bool affectZ)
		: Point(point), Speed(speed), AffectX(affectX), AffectY(affectY),
		AffectZ(affectZ), Attract(attract), LastTime(0)
	{
#ifdef _DEBUG
		setDebugName("CParticleAttractionAffector");
#endif
	}


	//! Affects an array of particles.
	void CParticleAttractionAffector::affect(UINT32 now, SParticle* particlearray, UINT32 count)
	{
		if (LastTime == 0)
		{
			LastTime = now;
			return;
		}

		FLOAT32 timeDelta = (now - LastTime) / 1000.0f;
		LastTime = now;

		if (!Enabled)
			return;

		for (UINT32 i = 0; i<count; ++i)
		{
			Vector3 direction = (Point - particlearray[i].pos).normalize();
			direction *= Speed * timeDelta;

			if (!Attract)
				direction *= -1.0f;

			if (AffectX)
				particlearray[i].pos.x += direction.x;

			if (AffectY)
				particlearray[i].pos.y += direction.y;

			if (AffectZ)
				particlearray[i].pos.z += direction.z;
		}
	}

	//! Writes attributes of the object.
	void CParticleAttractionAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Point", Point);
		out->addFloat("Speed", Speed);
		out->addBool("AffectX", AffectX);
		out->addBool("AffectY", AffectY);
		out->addBool("AffectZ", AffectZ);
		out->addBool("Attract", Attract);
	}

	//! Reads attributes of the object.
	void CParticleAttractionAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Point = in->getAttributeAsVector3d("Point");
		Speed = in->getAttributeAsFloat("Speed");
		AffectX = in->getAttributeAsBool("AffectX");
		AffectY = in->getAttributeAsBool("AffectY");
		AffectZ = in->getAttributeAsBool("AffectZ");
		Attract = in->getAttributeAsBool("Attract");
	}
}