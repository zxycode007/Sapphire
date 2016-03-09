#include "SapphireCParticleRotationAffector.h"
#include "SapphireIAttributes.h"

namespace Sapphire
{

	//! constructor
	CParticleRotationAffector::CParticleRotationAffector(const Vector3& speed, const Vector3& pivotPoint)
		: PivotPoint(pivotPoint), Speed(speed), LastTime(0)
	{
#ifdef _DEBUG
		setDebugName("CParticleRotationAffector");
#endif
	}


	//! Affects an array of particles.
	void CParticleRotationAffector::affect(UINT32 now, SParticle* particlearray, UINT32 count)
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
			if (Speed.x != 0.0f)
				particlearray[i].pos.rotateYZBy(timeDelta * Speed.x, PivotPoint);

			if (Speed.y != 0.0f)
				particlearray[i].pos.rotateXZBy(timeDelta * Speed.y, PivotPoint);

			if (Speed.z != 0.0f)
				particlearray[i].pos.rotateXYBy(timeDelta * Speed.z, PivotPoint);
		}
	}

	//! Writes attributes of the object.
	void CParticleRotationAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("PivotPoint", PivotPoint);
		out->addVector3d("Speed", Speed);
	}

	//! Reads attributes of the object.
	void CParticleRotationAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		PivotPoint = in->getAttributeAsVector3d("PivotPoint");
		Speed = in->getAttributeAsVector3d("Speed");
	}
}