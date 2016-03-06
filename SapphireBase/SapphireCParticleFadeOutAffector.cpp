#include "SapphireCParticleFadeOutAffector.h"
#include "SapphireIAttributes.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CParticleFadeOutAffector::CParticleFadeOutAffector(
		const ColourValue& targetColor, UINT32 fadeOutTime)
		: IParticleFadeOutAffector(), TargetColor(targetColor)
	{

#ifdef _DEBUG
		setDebugName("CParticleFadeOutAffector");
#endif

		FadeOutTime = fadeOutTime ? static_cast<FLOAT32>(fadeOutTime) : 1.0f;
	}


	//! Affects an array of particles.
	void CParticleFadeOutAffector::affect(UINT32 now, SParticle* particlearray, UINT32 count)
	{
		if (!Enabled)
			return;
		FLOAT32 d;

		for (UINT32 i = 0; i<count; ++i)
		{
			if (particlearray[i].endTime - now < FadeOutTime)
			{
				d = (particlearray[i].endTime - now) / FadeOutTime;	// FadeOutTime probably FLOAT32 to save casts here (just guessing)
				particlearray[i].color = particlearray[i].startColor.getInterpolated(
					TargetColor, d);
			}
		}
	}


	//! Writes attributes of the object.
	//! Implement this to expose the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml serialization purposes.
	void CParticleFadeOutAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addColor("TargetColor", TargetColor);
		out->addFloat("FadeOutTime", FadeOutTime);
	}

	//! Reads attributes of the object.
	//! Implement this to set the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml deserialization purposes.
	//! \param startIndex: start index where to start reading attributes.
	//! \return: returns last index of an attribute read by this affector
	void CParticleFadeOutAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		TargetColor = in->getAttributeAsColor("TargetColor");
		FadeOutTime = in->getAttributeAsFloat("FadeOutTime");
	}
}