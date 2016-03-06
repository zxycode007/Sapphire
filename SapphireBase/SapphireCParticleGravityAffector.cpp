#include "SapphireCParticleGravityAffector.h"
#include "SapphireOS.h"
#include "SapphireIAttributes.h"


namespace Sapphire
{
	//! constructor
	CParticleGravityAffector::CParticleGravityAffector(
		const Vector3& gravity, UINT32 timeForceLost)
		: IParticleGravityAffector(), TimeForceLost(static_cast<FLOAT32>(timeForceLost)), Gravity(gravity)
	{
#ifdef _DEBUG
		setDebugName("CParticleGravityAffector");
#endif
	}


	//! Affects an array of particles.
	void CParticleGravityAffector::affect(UINT32 now, SParticle* particlearray, UINT32 count)
	{
		if (!Enabled)
			return;
		FLOAT32 d;

		for (UINT32 i = 0; i<count; ++i)
		{
			d = (now - particlearray[i].startTime) / TimeForceLost;
			if (d > 1.0f)
				d = 1.0f;
			if (d < 0.0f)
				d = 0.0f;
			d = 1.0f - d;

			particlearray[i].vector = particlearray[i].startVector.getInterpolated(Gravity, d);
		}
	}

	//! Writes attributes of the object.
	void CParticleGravityAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Gravity", Gravity);
		out->addFloat("TimeForceLost", TimeForceLost);
	}


	//! Reads attributes of the object.
	void CParticleGravityAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Gravity = in->getAttributeAsVector3d("Gravity");
		TimeForceLost = in->getAttributeAsFloat("TimeForceLost");
	}

}