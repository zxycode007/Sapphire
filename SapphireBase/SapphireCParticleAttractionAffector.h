#ifndef _SAPPHIRE_C_PARTICLLE_AFFECTOR_
#define _SAPPHIRE_C_PARTICLLE_AFFECTOR_

#include "SapphireIParticleAttractionAffector.h"

namespace Sapphire
{
	//! Particle Affector for attracting particles to a point
	class CParticleAttractionAffector : public IParticleAttractionAffector
	{
	public:

		CParticleAttractionAffector(
			const Vector3& point = Vector3(), FLOAT32 speed = 1.0f,
			bool attract = true, bool affectX = true,
			bool affectY = true, bool affectZ = true);

		//! Affects a particle.
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count);

		//! Set the point that particles will attract to
		virtual void setPoint(const Vector3& point) { Point = point; }

		//! Set the speed, in game units per second that the particles will attract to
		//! the specified point
		virtual void setSpeed(FLOAT32 speed) { Speed = speed; }

		//! Set whether or not the particles are attracting or detracting
		virtual void setAttract(bool attract) { Attract = attract; }

		//! Set whether or not this will affect particles in the X direction
		virtual void setAffectX(bool affect) { AffectX = affect; }

		//! Set whether or not this will affect particles in the Y direction
		virtual void setAffectY(bool affect) { AffectY = affect; }

		//! Set whether or not this will affect particles in the Z direction
		virtual void setAffectZ(bool affect) { AffectZ = affect; }

		//! Get the point that particles are attracted to
		virtual const Vector3& getPoint() const { return Point; }

		//! Get the speed that points attract to the specified point
		virtual FLOAT32 getSpeed() const { return Speed; }

		//! Get whether or not the particles are attracting or detracting
		virtual bool getAttract() const { return Attract; }

		//! Get whether or not the particles X position are affected
		virtual bool getAffectX() const { return AffectX; }

		//! Get whether or not the particles Y position are affected
		virtual bool getAffectY() const { return AffectY; }

		//! Get whether or not the particles Z position are affected
		virtual bool getAffectZ() const { return AffectZ; }

		//! Writes attributes of the object.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the object.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		Vector3 Point;
		FLOAT32 Speed;
		bool AffectX;
		bool AffectY;
		bool AffectZ;
		bool Attract;
		UINT32 LastTime;
	};
}

#endif