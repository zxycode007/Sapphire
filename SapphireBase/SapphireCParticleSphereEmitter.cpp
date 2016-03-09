#include "SapphireCompileConifg.h"
#include "SapphireCParticleSphereEmitter.h"
#include "SapphireOS.h"
#include "SapphireIAttributes.h"

namespace Sapphire
{

	//! constructor
	CParticleSphereEmitter::CParticleSphereEmitter(
		const Vector3& center, FLOAT32 radius,
		const Vector3& direction, UINT32 minParticlesPerSecond,
		UINT32 maxParticlesPerSecond, const ColourValue& minStartColor,
		const ColourValue& maxStartColor, UINT32 lifeTimeMin, UINT32 lifeTimeMax,
		SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
		: Center(center), Radius(radius), Direction(direction),
		MinStartSize(minStartSize), MaxStartSize(maxStartSize),
		MinParticlesPerSecond(minParticlesPerSecond),
		MaxParticlesPerSecond(maxParticlesPerSecond),
		MinStartColor(minStartColor), MaxStartColor(maxStartColor),
		MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
		Time(0), Emitted(0), MaxAngleDegrees(maxAngleDegrees)
	{
#ifdef _DEBUG
		setDebugName("CParticleSphereEmitter");
#endif

	}


	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	SINT32 CParticleSphereEmitter::emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray)
	{
		Time += timeSinceLastCall;

		const UINT32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
		const FLOAT32 perSecond = pps ? ((FLOAT32)MinParticlesPerSecond + Randomizer::frand() * pps) : MinParticlesPerSecond;
		const FLOAT32 everyWhatMillisecond = 1000.0f / perSecond;

		if (Time > everyWhatMillisecond)
		{
			//Particles.set_used(0);
			Particles.clear();
			UINT32 amount = (UINT32)((Time / everyWhatMillisecond) + 0.5f);
			Time = 0;
			SParticle p;

			if (amount > MaxParticlesPerSecond * 2)
				amount = MaxParticlesPerSecond * 2;

			for (UINT32 i = 0; i<amount; ++i)
			{
				// Random distance from center
				const FLOAT32 distance = Randomizer::frand() * Radius;

				// Random direction from center
				p.pos.set(Center + distance);
				p.pos.rotateXYBy(Randomizer::frand() * 360.f, Center);
				p.pos.rotateYZBy(Randomizer::frand() * 360.f, Center);
				p.pos.rotateXZBy(Randomizer::frand() * 360.f, Center);

				p.startTime = now;
				p.vector = Direction;

				if (MaxAngleDegrees)
				{
					Vector3 tgt = Direction;
					tgt.rotateXYBy(Randomizer::frand() * MaxAngleDegrees);
					tgt.rotateYZBy(Randomizer::frand() * MaxAngleDegrees);
					tgt.rotateXZBy(Randomizer::frand() * MaxAngleDegrees);
					p.vector = tgt;
				}

				p.endTime = now + MinLifeTime;
				if (MaxLifeTime != MinLifeTime)
					p.endTime += Randomizer::rand() % (MaxLifeTime - MinLifeTime);

				if (MinStartColor == MaxStartColor)
					p.color = MinStartColor;
				else
					p.color = MinStartColor.getInterpolated(MaxStartColor, Randomizer::frand());

				p.startColor = p.color;
				p.startVector = p.vector;

				if (MinStartSize == MaxStartSize)
					p.startSize = MinStartSize;
				else
					p.startSize = MinStartSize.getInterpolated(MaxStartSize, Randomizer::frand());
				p.size = p.startSize;

				Particles.push_back(p);
			}

			outArray = Particles.begin()._Ptr;

			return Particles.size();
		}

		return 0;
	}

	//! Writes attributes of the object.
	void CParticleSphereEmitter::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Center", Direction);
		out->addFloat("Radius", Radius);

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
	void CParticleSphereEmitter::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Center = in->getAttributeAsVector3d("Center");
		Radius = in->getAttributeAsFloat("Radius");

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
		MinLifeTime = Math::max_(0u, MinLifeTime);
		MaxLifeTime = Math::max_(MaxLifeTime, MinLifeTime);
		MinLifeTime = Math::min_(MinLifeTime, MaxLifeTime);

		MaxAngleDegrees = in->getAttributeAsInt("MaxAngleDegrees");
	}
}