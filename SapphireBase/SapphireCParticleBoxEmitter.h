#ifndef _SAPPHIRE_C_PARTICLE_BOX_EMITTER_
#define _SAPPHIRE_C_PARTICLE_BOX_EMITTER_

#include "SapphireIParticleBoxEmitter.h"
#include "SapphireAxisAlignedBox.h"

namespace Sapphire
{

	//! A default box emitter
	class CParticleBoxEmitter : public IParticleBoxEmitter
	{
	public:

		//! constructor
		CParticleBoxEmitter(
			const AxisAlignedBox& box,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 20,
			UINT32 maxParticlesPerSecond = 40,
			ColourValue minStartColor = ColourValue(255, 0, 0, 0),
			ColourValue maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000,
			UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)
			);

		//! Prepares an array with new particles to emitt into the system
		//! and returns how much new particles there are.
		virtual SINT32 emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray);

		//! Set direction the emitter emits particles.
		virtual void setDirection(const Vector3& newDirection) { Direction = newDirection; }

		//! Set minimum number of particles emitted per second.
		virtual void setMinParticlesPerSecond(UINT32 minPPS) { MinParticlesPerSecond = minPPS; }

		//! Set maximum number of particles emitted per second.
		virtual void setMaxParticlesPerSecond(UINT32 maxPPS) { MaxParticlesPerSecond = maxPPS; }

		//! Set minimum start color.
		virtual void setMinStartColor(const ColourValue& color) { MinStartColor = color; }

		//! Set maximum start color.
		virtual void setMaxStartColor(const ColourValue& color) { MaxStartColor = color; }

		//! Set the maximum starting size for particles
		virtual void setMaxStartSize(const dimension2df& size) { MaxStartSize = size; };

		//! Set the minimum starting size for particles
		virtual void setMinStartSize(const dimension2df& size) { MinStartSize = size; };

		//! Set the minimum particle life-time in milliseconds
		virtual void setMinLifeTime(UINT32 lifeTimeMin) { MinLifeTime = lifeTimeMin; }

		//! Set the maximum particle life-time in milliseconds
		virtual void setMaxLifeTime(UINT32 lifeTimeMax) { MaxLifeTime = lifeTimeMax; }

		//!	Maximal random derivation from the direction
		virtual void setMaxAngleDegrees(SINT32 maxAngleDegrees) { MaxAngleDegrees = maxAngleDegrees; }

		//! Set box from which the particles are emitted.
		virtual void setBox(const AxisAlignedBox& box) { Box = box; }

		//! Gets direction the emitter emits particles.
		virtual const Vector3& getDirection() const { return Direction; }

		//! Gets minimum number of particles emitted per second.
		virtual UINT32 getMinParticlesPerSecond() const { return MinParticlesPerSecond; }

		//! Gets maximum number of particles emitted per second.
		virtual UINT32 getMaxParticlesPerSecond() const { return MaxParticlesPerSecond; }

		//! Gets minimum start color.
		virtual const ColourValue& getMinStartColor() const { return MinStartColor; }

		//! Gets maximum start color.
		virtual const ColourValue& getMaxStartColor() const { return MaxStartColor; }

		//! Gets the maximum starting size for particles
		virtual const dimension2df& getMaxStartSize() const { return MaxStartSize; }

		//! Gets the minimum starting size for particles
		virtual const dimension2df& getMinStartSize() const { return MinStartSize; }

		//! Get the minimum particle life-time in milliseconds
		virtual UINT32 getMinLifeTime() const { return MinLifeTime; }

		//! Get the maximum particle life-time in milliseconds
		virtual UINT32 getMaxLifeTime() const { return MaxLifeTime; }

		//!	Maximal random derivation from the direction
		virtual SINT32 getMaxAngleDegrees() const { return MaxAngleDegrees; }

		//! Get box from which the particles are emitted.
		virtual const AxisAlignedBox& getBox() const { return Box; }

		//! Writes attributes of the object.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the object.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		vector<SParticle>::type Particles;
		AxisAlignedBox Box;
		Vector3 Direction;
		dimension2df MaxStartSize, MinStartSize;
		UINT32 MinParticlesPerSecond, MaxParticlesPerSecond;
		ColourValue MinStartColor, MaxStartColor;
		UINT32 MinLifeTime, MaxLifeTime;

		UINT32 Time;
		UINT32 Emitted;
		SINT32 MaxAngleDegrees;
	};
}

#endif