#ifndef _SAPPHIRE_C_PARTICLE_CYLINDER_EMITTER_
#define _SAPPHIRE_C_PARTICLE_CYLINDER_EMITTER_


#include "SapphireIParticleCylinderEmitter.h"

namespace Sapphire
{
	//! A default box emitter
	class CParticleCylinderEmitter : public IParticleCylinderEmitter
	{
	public:

		//! constructor
		CParticleCylinderEmitter(
			const Vector3& center, FLOAT32 radius,
			const Vector3& normal, FLOAT32 length,
			bool outlineOnly = false, const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 20,
			UINT32 maxParticlesPerSecond = 40,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000,
			UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)
			);

		//! Prepares an array with new particles to emitt into the system
		//! and returns how much new particles there are.
		virtual SINT32 emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray);

		//! Set the center of the radius for the cylinder, at one end of the cylinder
		virtual void setCenter(const Vector3& center) { Center = center; }

		//! Set the normal of the cylinder
		virtual void setNormal(const Vector3& normal) { Normal = normal; }

		//! Set the radius of the cylinder
		virtual void setRadius(FLOAT32 radius) { Radius = radius; }

		//! Set the length of the cylinder
		virtual void setLength(FLOAT32 length) { Length = length; }

		//! Set whether or not to draw points inside the cylinder
		virtual void setOutlineOnly(bool outlineOnly) { OutlineOnly = outlineOnly; }

		//! Set direction the emitter emits particles
		virtual void setDirection(const Vector3& newDirection) { Direction = newDirection; }

		//! Set direction the emitter emits particles
		virtual void setMinParticlesPerSecond(UINT32 minPPS) { MinParticlesPerSecond = minPPS; }

		//! Set direction the emitter emits particles
		virtual void setMaxParticlesPerSecond(UINT32 maxPPS) { MaxParticlesPerSecond = maxPPS; }

		//! Set direction the emitter emits particles
		virtual void setMinStartColor(const ColourValue& color) { MinStartColor = color; }

		//! Set direction the emitter emits particles
		virtual void setMaxStartColor(const ColourValue& color) { MaxStartColor = color; }

		//! Set the maximum starting size for particles
		virtual void setMaxStartSize(const dimension2df& size) { MaxStartSize = size; }

		//! Set the minimum starting size for particles
		virtual void setMinStartSize(const dimension2df& size) { MinStartSize = size; }

		//! Set the minimum particle life-time in milliseconds
		virtual void setMinLifeTime(UINT32 lifeTimeMin) { MinLifeTime = lifeTimeMin; }

		//! Set the maximum particle life-time in milliseconds
		virtual void setMaxLifeTime(UINT32 lifeTimeMax) { MaxLifeTime = lifeTimeMax; }

		//!	Maximal random derivation from the direction
		virtual void setMaxAngleDegrees(SINT32 maxAngleDegrees) { MaxAngleDegrees = maxAngleDegrees; }

		//! Get the center of the cylinder
		virtual const Vector3& getCenter() const { return Center; }

		//! Get the normal of the cylinder
		virtual const Vector3& getNormal() const { return Normal; }

		//! Get the radius of the cylinder
		virtual FLOAT32 getRadius() const { return Radius; }

		//! Get the center of the cylinder
		virtual FLOAT32 getLength() const { return Length; }

		//! Get whether or not to draw points inside the cylinder
		virtual bool getOutlineOnly() const { return OutlineOnly; }

		//! Gets direction the emitter emits particles
		virtual const Vector3& getDirection() const { return Direction; }

		//! Gets direction the emitter emits particles
		virtual UINT32 getMinParticlesPerSecond() const { return MinParticlesPerSecond; }

		//! Gets direction the emitter emits particles
		virtual UINT32 getMaxParticlesPerSecond() const { return MaxParticlesPerSecond; }

		//! Gets direction the emitter emits particles
		virtual const ColourValue& getMinStartColor() const { return MinStartColor; }

		//! Gets direction the emitter emits particles
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

		//! Writes attributes of the object.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the object.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		vector<SParticle>::type Particles;

		Vector3	Center;
		Vector3	Normal;
		Vector3 Direction;
		dimension2df MaxStartSize, MinStartSize;
		UINT32 MinParticlesPerSecond, MaxParticlesPerSecond;
		ColourValue MinStartColor, MaxStartColor;
		UINT32 MinLifeTime, MaxLifeTime;

		FLOAT32 Radius;
		FLOAT32 Length;

		UINT32 Time;
		UINT32 Emitted;
		SINT32 MaxAngleDegrees;

		bool OutlineOnly;
	};
}

#endif