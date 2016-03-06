#include "SapphireCParticleAnimatedMeshSceneNodeEmitter.h"
#include "SapphireIAnimatedMeshSceneNode.h"
#include "SapphireIMesh.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CParticleAnimatedMeshSceneNodeEmitter::CParticleAnimatedMeshSceneNodeEmitter(
		IAnimatedMeshSceneNode* node, bool useNormalDirection,
		const Vector3& direction, FLOAT32 normalDirectionModifier,
		SINT32 mbNumber, bool everyMeshVertex,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize, const dimension2df& maxStartSize)
		: Node(0), AnimatedMesh(0), BaseMesh(0), TotalVertices(0), MBCount(0), MBNumber(mbNumber),
		Direction(direction), NormalDirectionModifier(normalDirectionModifier),
		MinParticlesPerSecond(minParticlesPerSecond), MaxParticlesPerSecond(maxParticlesPerSecond),
		MinStartColor(minStartColor), MaxStartColor(maxStartColor),
		MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
		MaxStartSize(maxStartSize), MinStartSize(minStartSize),
		Time(0), Emitted(0), MaxAngleDegrees(maxAngleDegrees),
		EveryMeshVertex(everyMeshVertex), UseNormalDirection(useNormalDirection)
	{
#ifdef _DEBUG
		setDebugName("CParticleAnimatedMeshSceneNodeEmitter");
#endif
		setAnimatedMeshSceneNode(node);
	}


	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	SINT32 CParticleAnimatedMeshSceneNodeEmitter::emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray)
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

			// Get Mesh for this frame
			IMesh* frameMesh = AnimatedMesh->getMesh(floor32(Node->getFrameNr()),
				255, Node->getStartFrame(), Node->getEndFrame());
			for (UINT32 i = 0; i<amount; ++i)
			{
				if (EveryMeshVertex)
				{
					for (UINT32 j = 0; j<frameMesh->getMeshBufferCount(); ++j)
					{
						for (UINT32 k = 0; k<frameMesh->getMeshBuffer(j)->getVertexCount(); ++k)
						{
							p.pos = frameMesh->getMeshBuffer(j)->getPosition(k);
							if (UseNormalDirection)
								p.vector = frameMesh->getMeshBuffer(j)->getNormal(k) /
								NormalDirectionModifier;
							else
								p.vector = Direction;

							p.startTime = now;

							if (MaxAngleDegrees)
							{
								Vector3 tgt = p.vector;
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
					}
				}
				else
				{
					SINT32 randomMB = 0;
					if (MBNumber < 0)
						randomMB = Randomizer::rand() % MBCount;
					else
						randomMB = MBNumber;

					UINT32 vertexNumber = frameMesh->getMeshBuffer(randomMB)->getVertexCount();
					if (!vertexNumber)
						continue;
					vertexNumber = Randomizer::rand() % vertexNumber;

					p.pos = frameMesh->getMeshBuffer(randomMB)->getPosition(vertexNumber);
					if (UseNormalDirection)
						p.vector = frameMesh->getMeshBuffer(randomMB)->getNormal(vertexNumber) /
						NormalDirectionModifier;
					else
						p.vector = Direction;

					p.startTime = now;

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
			} 
			 



			outArray = Particles.begin()._Ptr;

			return Particles.size();
		}

		return 0;
	}


	//! Set Mesh to emit particles from
	void CParticleAnimatedMeshSceneNodeEmitter::setAnimatedMeshSceneNode(IAnimatedMeshSceneNode* node)
	{
		Node = node;
		AnimatedMesh = 0;
		BaseMesh = 0;
		TotalVertices = 0;
		VertexPerMeshBufferList.clear();
		if (!node)
		{
			return;
		}

		AnimatedMesh = node->getMesh();
		BaseMesh = AnimatedMesh->getMesh(0);

		MBCount = BaseMesh->getMeshBufferCount();
		VertexPerMeshBufferList.reserve(MBCount);
		for (UINT32 i = 0; i < MBCount; ++i)
		{
			VertexPerMeshBufferList.push_back(BaseMesh->getMeshBuffer(i)->getVertexCount());
			TotalVertices += BaseMesh->getMeshBuffer(i)->getVertexCount();
		}
	}
}