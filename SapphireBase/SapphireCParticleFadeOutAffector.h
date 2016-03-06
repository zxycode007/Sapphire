#ifndef _SAPPHIRE_C_PARTICLIE_FADE_OUT_AFFECTOR_
#define _SAPPHIRE_C_PARTICLIE_FADE_OUT_AFFECTOR_

#include "SapphireIParticleFadeOutAffector.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	//! Particle Affector for fading out a color
	class CParticleFadeOutAffector : public IParticleFadeOutAffector
	{
	public:

		CParticleFadeOutAffector(const ColourValue& targetColor, UINT32 fadeOutTime);

		//! Affects a particle.
		virtual void affect(UINT32 now, SParticle* particlearray, UINT32 count);

		//! Sets the targetColor, i.e. the color the particles will interpolate
		//! to over time.
		virtual void setTargetColor(const ColourValue& targetColor) { TargetColor = targetColor; }

		//! Sets the amount of time it takes for each particle to fade out.
		virtual void setFadeOutTime(UINT32 fadeOutTime) { FadeOutTime = fadeOutTime ? static_cast<FLOAT32>(fadeOutTime) : 1.0f; }

		//! Sets the targetColor, i.e. the color the particles will interpolate
		//! to over time.
		virtual const ColourValue& getTargetColor() const { return TargetColor; }

		//! Sets the amount of time it takes for each particle to fade out.
		virtual UINT32 getFadeOutTime() const { return static_cast<UINT32>(FadeOutTime); }

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

		ColourValue TargetColor;
		FLOAT32 FadeOutTime;
	};
}

#endif