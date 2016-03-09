#ifndef _SAPPHIRE_C_PARTICLE_ROTATION_AFFECTOR_
#define _SAPPHIRE_C_PARTICLE_ROTATION_AFFECTOR_

#include "SapphireIParticleRotationAffector.h"

namespace Sapphire
{

	//! Particle Affector for rotating particles about a point
	class CParticleRotationAffector : public IParticleRotationAffector
	{
	public:

		CParticleRotationAffector(const Vector3& speed = Vector3(5.0f, 5.0f, 5.0f),
			const Vector3& point = Vector3());

		//! Affects a particle.
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count);

		//! Set the point that particles will attract to
		virtual void setPivotPoint(const Vector3& point) { PivotPoint = point; }

		//! Set the speed in degrees per second
		virtual void setSpeed(const Vector3& speed) { Speed = speed; }

		//! Get the point that particles are attracted to
		virtual const Vector3& getPivotPoint() const { return PivotPoint; }

		//! Get the speed in degrees per second
		virtual const Vector3& getSpeed() const { return Speed; }

		//! Writes attributes of the object.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the object.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		Vector3 PivotPoint;
		Vector3 Speed;
		UINT32 LastTime;
	};
}

#endif