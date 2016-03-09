#include "SapphireCParticleScaleAffector.h"
#include "SapphireIAttributes.h"


namespace Sapphire
{
	CParticleScaleAffector::CParticleScaleAffector(const dimension2df& scaleTo)
		: ScaleTo(scaleTo)
	{
#ifdef _DEBUG
		setDebugName("CParticleScaleAffector");
#endif
	}


	void CParticleScaleAffector::affect(UINT32 now, SParticle *particlearray, UINT32 count)
	{
		for (UINT32 i = 0; i<count; i++)
		{
			const UINT32 maxdiff = particlearray[i].endTime - particlearray[i].startTime;
			const UINT32 curdiff = now - particlearray[i].startTime;
			const FLOAT32 newscale = (FLOAT32)curdiff / maxdiff;
			particlearray[i].size = particlearray[i].startSize + ScaleTo*newscale;
		}
	}


	void CParticleScaleAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addFloat("ScaleToWidth", ScaleTo.Width);
		out->addFloat("ScaleToHeight", ScaleTo.Height);
	}


	void CParticleScaleAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		ScaleTo.Width = in->getAttributeAsFloat("ScaleToWidth");
		ScaleTo.Height = in->getAttributeAsFloat("ScaleToHeight");
	}


	E_PARTICLE_AFFECTOR_TYPE CParticleScaleAffector::getType() const
	{
		return EPAT_SCALE;
	}
}