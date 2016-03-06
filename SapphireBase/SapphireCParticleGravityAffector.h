#ifndef _SAPPHIRE_C_PARTICLE_GRAVITY_AFFECTOR_
#define _SAPPHIRE_C_PARTICLE_GRAVITY_AFFECTOR_

#include "SapphireIParticleGravityAffector.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	//! Particle Affector for affecting direction of particle
	class CParticleGravityAffector : public IParticleGravityAffector
	{
	public:

		CParticleGravityAffector(
			const Vector3& gravity = Vector3(0.0f, -0.03f, 0.0f),
			UINT32 timeForceLost = 1000);

		//! Affects a particle.
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count);

		//! Set the time in milliseconds when the gravity force is totally
		//! lost and the particle does not move any more.
		virtual void setTimeForceLost(FLOAT32 timeForceLost) { TimeForceLost = timeForceLost; }

		//! Set the direction and force of gravity.
		virtual void setGravity(const Vector3& gravity) { Gravity = gravity; }

		//! Set the time in milliseconds when the gravity force is totally
		//! lost and the particle does not move any more.
		virtual FLOAT32 getTimeForceLost() const { return TimeForceLost; }

		//! Set the direction and force of gravity.
		virtual const Vector3& getGravity() const { return Gravity; }

		//! Writes attributes of the object.
		//! Implement this to expose the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml serialization purposes.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the object.
		//! Implement this to set the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml deserialization purposes.
		//! \param startIndex: start index where to start reading attributes.
		//! \return: returns last index of an attribute read by this affector
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:
		FLOAT32 TimeForceLost;
		Vector3 Gravity;
	};
}

#endif