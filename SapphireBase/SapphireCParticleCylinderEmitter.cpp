#include "SapphireCParticleCylinderEmitter.h"
#include "SapphireOS.h"
#include "SapphireIAttributes.h"


namespace Sapphire
{

	//! constructor
	CParticleCylinderEmitter::CParticleCylinderEmitter(
		const Vector3& center, FLOAT32 radius,
		const Vector3& normal, FLOAT32 length,
		bool outlineOnly, const Vector3& direction,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
		: Center(center), Normal(normal), Direction(direction),
		MaxStartSize(maxStartSize), MinStartSize(minStartSize),
		MinParticlesPerSecond(minParticlesPerSecond),
		MaxParticlesPerSecond(maxParticlesPerSecond),
		MinStartColor(minStartColor), MaxStartColor(maxStartColor),
		MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
		Radius(radius), Length(length), Time(0), Emitted(0),
		MaxAngleDegrees(maxAngleDegrees), OutlineOnly(outlineOnly)
	{
#ifdef _DEBUG
		setDebugName("CParticleCylinderEmitter");
#endif
	}


	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	SINT32 CParticleCylinderEmitter::emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray)
	{
		Time += timeSinceLastCall;

		const UINT32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
		const FLOAT32 perSecond = pps ? ((FLOAT32)MinParticlesPerSecond + Randomizer::frand() * pps) : MinParticlesPerSecond;
		const FLOAT32 everyWhatMillisecond = 1000.0f / perSecond;

		if (Time > everyWhatMillisecond)
		{
			Particles.clear();
			UINT32 amount = (UINT32)((Time / everyWhatMillisecond) + 0.5f);
			Time = 0;
			SParticle p;

			if (amount > MaxParticlesPerSecond * 2)
				amount = MaxParticlesPerSecond * 2;

			for (UINT32 i = 0; i<amount; ++i)
			{
				// Random distance from center if outline only is not true
				const FLOAT32 distance = (!OutlineOnly) ? (Randomizer::frand() * Radius) : Radius;

				// Random direction from center
				p.pos.set(Center.x + distance, Center.y, Center.z + distance);
				p.pos.rotateXZBy(Randomizer::frand() * 360, Center);

				// Random length
				const FLOAT32 length = Randomizer::frand() * Length;

				// Random point along the cylinders length
				p.pos += Normal * length;

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
	void CParticleCylinderEmitter::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Center", Center);
		out->addVector3d("Normal", Normal);
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
		out->addFloat("Radius", Radius);
		out->addFloat("Length", Length);
		out->addInt("MaxAngleDegrees", MaxAngleDegrees);
		out->addBool("OutlineOnly", OutlineOnly);
	}

	//! Reads attributes of the object.
	void CParticleCylinderEmitter::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Center = in->getAttributeAsVector3d("Center");
		Normal = in->getAttributeAsVector3d("Normal");
		if (Normal.length() == 0)
			Normal.set(0, 1.f, 0);
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

		Radius = in->getAttributeAsFloat("Radius");
		Length = in->getAttributeAsFloat("Length");
		MaxAngleDegrees = in->getAttributeAsInt("MaxAngleDegrees");
		OutlineOnly = in->getAttributeAsBool("OutlineOnly");
	}
}