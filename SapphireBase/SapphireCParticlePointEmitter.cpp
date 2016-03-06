#include "SapphireCParticlePointEmitter.h"
#include "SapphireOS.h"
#include "SapphireIAttributes.h"


namespace Sapphire
{
	//! constructor
	CParticlePointEmitter::CParticlePointEmitter(
		const Vector3& direction, UINT32 minParticlesPerSecond,
		UINT32 maxParticlesPerSecond, ColourValue minStartColor,
		ColourValue maxStartColor, UINT32 lifeTimeMin, UINT32 lifeTimeMax,
		SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
		: Direction(direction),
		MinStartSize(minStartSize), MaxStartSize(maxStartSize),
		MinParticlesPerSecond(minParticlesPerSecond),
		MaxParticlesPerSecond(maxParticlesPerSecond),
		MinStartColor(minStartColor), MaxStartColor(maxStartColor),
		MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
		MaxAngleDegrees(maxAngleDegrees), Time(0), Emitted(0)
	{
#ifdef _DEBUG
		setDebugName("CParticlePointEmitter");
#endif
	}


	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	SINT32 CParticlePointEmitter::emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray)
	{
		Time += timeSinceLastCall;

		const UINT32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
		const FLOAT32 perSecond = pps ? ((FLOAT32)MinParticlesPerSecond + Randomizer::frand() * pps) : MinParticlesPerSecond;
		const FLOAT32 everyWhatMillisecond = 1000.0f / perSecond;

		if (Time > everyWhatMillisecond)
		{
			Time = 0;
			Particle.startTime = now;
			Particle.vector = Direction;

			if (MaxAngleDegrees)
			{
				Vector3 tgt = Direction;
				tgt.rotateXYBy(Randomizer::frand() * MaxAngleDegrees);
				tgt.rotateYZBy(Randomizer::frand() * MaxAngleDegrees);
				tgt.rotateXZBy(Randomizer::frand() * MaxAngleDegrees);
				Particle.vector = tgt;
			}

			Particle.endTime = now + MinLifeTime;
			if (MaxLifeTime != MinLifeTime)
				Particle.endTime += Randomizer::rand() % (MaxLifeTime - MinLifeTime);

			if (MinStartColor == MaxStartColor)
				Particle.color = MinStartColor;
			else
				Particle.color = MinStartColor.getInterpolated(MaxStartColor, Randomizer::frand());

			Particle.startColor = Particle.color;
			Particle.startVector = Particle.vector;

			if (MinStartSize == MaxStartSize)
				Particle.startSize = MinStartSize;
			else
				Particle.startSize = MinStartSize.getInterpolated(MaxStartSize, Randomizer::frand());
			Particle.size = Particle.startSize;

			outArray = &Particle;
			return 1;
		}

		return 0;
	}


	//! Writes attributes of the object.
	void CParticlePointEmitter::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Direction", Direction);
		out->addFloat("MinStartSizeWidth", MinStartSize.Width);
		out->addFloat("MinStartSizeHeight", MinStartSize.Height);
		out->addFloat("MaxStartSizeWidth", MaxStartSize.Width);
		out->addFloat("MaxStartSizeHeight", MaxStartSize.Height);
		out->addInt("MinParticlesPerSecond", MinParticlesPerSecond);
		out->addInt("MaxParticlesPerSecond", MaxParticlesPerSecond);
		out->addColor("MinStartColor", MinStartColor);
		out->addColor("MaxStartColor", MaxStartColor);
		out->addInt("MinLifeTime", MinLifeTime);
		out->addInt("MaxLifeTime", MaxLifeTime);
		out->addInt("MaxAngleDegrees", MaxAngleDegrees);
	}

	//! Reads attributes of the object.
	void CParticlePointEmitter::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Direction = in->getAttributeAsVector3d("Direction");
		if (Direction.length() == 0)
			Direction.set(0, 0.01f, 0);

		int idx = -1;
		idx = in->findAttribute("MinStartSizeWidth");
		if (idx >= 0)
			MinStartSize.Width = in->getAttributeAsFloat(idx);
		idx = in->findAttribute("MinStartSizeHeight");
		if (idx >= 0)
			MinStartSize.Height = in->getAttributeAsFloat(idx);
		idx = in->findAttribute("MaxStartSizeWidth");
		if (idx >= 0)
			MaxStartSize.Width = in->getAttributeAsFloat(idx);
		idx = in->findAttribute("MaxStartSizeHeight");
		if (idx >= 0)
			MaxStartSize.Height = in->getAttributeAsFloat(idx);

		MinParticlesPerSecond = in->getAttributeAsInt("MinParticlesPerSecond");
		MaxParticlesPerSecond = in->getAttributeAsInt("MaxParticlesPerSecond");

		MinParticlesPerSecond = Math::max_(1u, MinParticlesPerSecond);
		MaxParticlesPerSecond = Math::max_(MaxParticlesPerSecond, 1u);
		MaxParticlesPerSecond = Math::min_(MaxParticlesPerSecond, 200u);
		MinParticlesPerSecond = Math::min_(MinParticlesPerSecond, MaxParticlesPerSecond);

		MinStartColor = in->getAttributeAsColor("MinStartColor");
		MaxStartColor = in->getAttributeAsColor("MaxStartColor");
		MinLifeTime = in->getAttributeAsInt("MinLifeTime");
		MaxLifeTime = in->getAttributeAsInt("MaxLifeTime");
		MaxAngleDegrees = in->getAttributeAsInt("MaxAngleDegrees");

		MinLifeTime = Math::max_(0u, MinLifeTime);
		MaxLifeTime = Math::max_(MaxLifeTime, MinLifeTime);
		MinLifeTime = Math::min_(MinLifeTime, MaxLifeTime);
	}
}