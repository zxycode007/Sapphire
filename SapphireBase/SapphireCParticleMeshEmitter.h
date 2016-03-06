#ifndef _SAPPHIRE_C_PARTICLE_MESH_EMITTER_
#define _SAPPHIRE_C_PARTICLE_MESH_EMITTER_

#include "SapphireIParticleMeshEmitter.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireIMeshBuffer.h"

namespace Sapphire
{
	//! A default box emitter
	class CParticleMeshEmitter : public IParticleMeshEmitter
	{
	public:

		//! constructor
		CParticleMeshEmitter(
			IMesh* mesh, bool useNormalDirection = true,
			const Vector3& direction = Vector3(0.0f, 0.0f, 0.0f),
			FLOAT32 normalDirectionModifier = 100.0f,
			SINT32 mbNumber = -1,
			bool everyMeshVertex = false,
			UINT32 minParticlesPerSecond = 20,
			UINT32 maxParticlesPerSecond = 40,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000,
			UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)
			);

		//! Prepares an array with new particles to emitt into the system
		//! and returns how much new particles there are.
		virtual SINT32 emitt(UINT32 now, UINT32 timeSinceLastCall, SParticle*& outArray);

		//! Set Mesh to emit particles from
		virtual void setMesh(IMesh* mesh);

		//! Set whether to use vertex normal for direction, or direction specified
		virtual void setUseNormalDirection(bool useNormalDirection) { UseNormalDirection = useNormalDirection; }

		//! Set direction the emitter emits particles
		virtual void setDirection(const Vector3& newDirection) { Direction = newDirection; }

		//! Set the amount that the normal is divided by for getting a particles direction
		virtual void setNormalDirectionModifier(FLOAT32 normalDirectionModifier) { NormalDirectionModifier = normalDirectionModifier; }

		//! Sets whether to emit min<->max particles for every vertex per second, or to pick
		//! min<->max vertices every second
		virtual void setEveryMeshVertex(bool everyMeshVertex) { EveryMeshVertex = everyMeshVertex; }

		//! Set minimum number of particles the emitter emits per second
		virtual void setMinParticlesPerSecond(UINT32 minPPS) { MinParticlesPerSecond = minPPS; }

		//! Set maximum number of particles the emitter emits per second
		virtual void setMaxParticlesPerSecond(UINT32 maxPPS) { MaxParticlesPerSecond = maxPPS; }

		//! Set minimum starting color for particles
		virtual void setMinStartColor(const ColourValue& color) { MinStartColor = color; }

		//! Set maximum starting color for particles
		virtual void setMaxStartColor(const ColourValue& color) { MaxStartColor = color; }

		//! Set the maximum starting size for particles
		virtual void setMaxStartSize(const dimension2df& size) { MaxStartSize = size; }

		//! Set the minimum starting size for particles
		virtual void setMinStartSize(const dimension2df& size) { MinStartSize = size; }

		//! Set the minimum particle life-time in milliseconds
		virtual void setMinLifeTime(UINT32 lifeTimeMin) { MinLifeTime = lifeTimeMin; }

		//! Set the maximum particle life-time in milliseconds
		virtual void setMaxLifeTime(UINT32 lifeTimeMax) { MaxLifeTime = lifeTimeMax; }

		//!	Set maximal random derivation from the direction
		virtual void setMaxAngleDegrees(SINT32 maxAngleDegrees) { MaxAngleDegrees = maxAngleDegrees; }

		//! Get Mesh we're emitting particles from
		virtual const IMesh* getMesh() const { return Mesh; }

		//! Get whether to use vertex normal for direciton, or direction specified
		virtual bool isUsingNormalDirection() const { return UseNormalDirection; }

		//! Get direction the emitter emits particles
		virtual const Vector3& getDirection() const { return Direction; }

		//! Get the amount that the normal is divided by for getting a particles direction
		virtual FLOAT32 getNormalDirectionModifier() const { return NormalDirectionModifier; }

		//! Gets whether to emit min<->max particles for every vertex per second, or to pick
		//! min<->max vertices every second
		virtual bool getEveryMeshVertex() const { return EveryMeshVertex; }

		//! Get the minimum number of particles the emitter emits per second
		virtual UINT32 getMinParticlesPerSecond() const { return MinParticlesPerSecond; }

		//! Get the maximum number of particles the emitter emits per second
		virtual UINT32 getMaxParticlesPerSecond() const { return MaxParticlesPerSecond; }

		//! Get the minimum starting color for particles
		virtual const ColourValue& getMinStartColor() const { return MinStartColor; }

		//! Get the maximum starting color for particles
		virtual const ColourValue& getMaxStartColor() const { return MaxStartColor; }

		//! Gets the maximum starting size for particles
		virtual const dimension2df& getMaxStartSize() const { return MaxStartSize; }

		//! Gets the minimum starting size for particles
		virtual const dimension2df& getMinStartSize() const { return MinStartSize; }

		//! Get the minimum particle life-time in milliseconds
		virtual UINT32 getMinLifeTime() const { return MinLifeTime; }

		//! Get the maximum particle life-time in milliseconds
		virtual UINT32 getMaxLifeTime() const { return MaxLifeTime; }

		//!	Get maximal random derivation from the direction
		virtual SINT32 getMaxAngleDegrees() const { return MaxAngleDegrees; }

	private:

		const IMesh* Mesh;
		vector<SINT32>::type	VertexPerMeshBufferList;
		SINT32 TotalVertices;
		UINT32 MBCount;
		SINT32 MBNumber;

		FLOAT32 NormalDirectionModifier;
		vector<SParticle>::type Particles;
		Vector3 Direction;
		dimension2df MaxStartSize, MinStartSize;
		UINT32 MinParticlesPerSecond, MaxParticlesPerSecond;
		ColourValue MinStartColor, MaxStartColor;
		UINT32 MinLifeTime, MaxLifeTime;

		UINT32 Time;
		UINT32 Emitted;
		SINT32 MaxAngleDegrees;

		bool EveryMeshVertex;
		bool UseNormalDirection;
	};
}

#endif