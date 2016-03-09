#ifndef _SAPPHIRE_C_PARTICLE_SCALE_AFFECTOR_
#define _SAPPHIRE_C_PARTICLE_SCALE_AFFECTOR_

#include "SapphireIParticleAffector.h"

namespace Sapphire
{
	class CParticleScaleAffector : public IParticleAffector
	{
	public:
		CParticleScaleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f));

		virtual void affect(UINT32 now, SParticle *particlearray, UINT32 count);

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

		//! Get emitter type
		virtual E_PARTICLE_AFFECTOR_TYPE getType() const;

	protected:
		dimension2df ScaleTo;
	};
}

#endif