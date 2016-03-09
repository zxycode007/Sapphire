#ifndef _SAPPHIRE_C_PARTICLE_SYSTEM_SCENE_NODE_
#define _SAPPHIRE_C_PARTICLE_SYSTEM_SCENE_NODE_

#include "SapphireIParticleSysetmSceneNode.h"
#include "SapphireSMeshBuffer.h"

namespace Sapphire
{
	//! A particle system scene node.
	/** A scene node controlling a particle system. The behavior of the particles
	can be controlled by setting the right particle emitters and affectors.
	*/
	class CParticleSystemSceneNode : public IParticleSystemSceneNode
	{
	public:

		//! constructor
		CParticleSystemSceneNode(bool createDefaultEmitter,
			ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position,
			const Vector3& rotation,
			const Vector3& scale);

		//! destructor
		virtual ~CParticleSystemSceneNode();

		//! Gets the particle emitter, which creates the particles.
		virtual IParticleEmitter* getEmitter();

		//! Sets the particle emitter, which creates the particles.
		virtual void setEmitter(IParticleEmitter* emitter);

		//! Adds new particle affector to the particle system.
		virtual void addAffector(IParticleAffector* affector);

		//! Get a list of all particle affectors.
		virtual const list<IParticleAffector*>::type& getAffectors() const;

		//! Removes all particle affectors in the particle system.
		virtual void removeAllAffectors();

		//! Returns the material based on the zero based index i.
		virtual SMaterial& getMaterial(UINT32 i);

		//! Returns amount of materials used by this scene node.
		virtual UINT32 getMaterialCount() const;

		//! pre render event
		virtual void OnRegisterSceneNode();

		//! render
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! Creates a particle emitter for an animated mesh scene node
		virtual IParticleAnimatedMeshSceneNodeEmitter* createAnimatedMeshSceneNodeEmitter(
			IAnimatedMeshSceneNode* node, bool useNormalDirection = true,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			FLOAT32 normalDirectionModifier = 100.0f, SINT32 mbNumber = -1,
			bool everyMeshVertex = false, UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a box particle emitter.
		virtual IParticleBoxEmitter* createBoxEmitter(
			const AxisAlignedBox& box = AxisAlignedBox(-10, 0, -10, 5, 30, 10),
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a particle emitter for emitting from a cylinder
		virtual IParticleCylinderEmitter* createCylinderEmitter(
			const Vector3& center, FLOAT32 radius,
			const Vector3& normal, FLOAT32 length,
			bool outlineOnly = false, const Vector3& direction = Vector3(0.0f, 0.5f, 0.0f),
			UINT32 minParticlesPerSecond = 5, UINT32 maxParticlesPersSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a mesh particle emitter.
		virtual IParticleMeshEmitter* createMeshEmitter(
			IMesh* mesh, bool useNormalDirection = true,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			FLOAT32 normalDirectionModifier = 100.0f, SINT32 mbNumber = -1,
			bool everyMeshVertex = false,
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a point particle emitter.
		virtual IParticlePointEmitter* createPointEmitter(
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a ring particle emitter.
		virtual IParticleRingEmitter* createRingEmitter(
			const Vector3& center, FLOAT32 radius, FLOAT32 ringThickness,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a sphere particle emitter.
		virtual IParticleSphereEmitter* createSphereEmitter(
			const Vector3& center, FLOAT32 radius,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));

		//! Creates a point attraction affector. This affector modifies the positions of the
		//! particles and attracts them to a specified point at a specified speed per second.
		virtual IParticleAttractionAffector* createAttractionAffector(
			const Vector3& point, FLOAT32 speed = 1.0f, bool attract = true,
			bool affectX = true, bool affectY = true, bool affectZ = true);

		//! Creates a scale particle affector.
		virtual IParticleAffector* createScaleParticleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f));

		//! Creates a fade out particle affector.
		virtual IParticleFadeOutAffector* createFadeOutParticleAffector(
			const ColourValue& targetColor = ColourValue(0, 0, 0, 0),
			UINT32 timeNeededToFadeOut = 1000);

		//! Creates a gravity affector.
		virtual IParticleGravityAffector* createGravityAffector(
			const Vector3& gravity = Vector3(0.0f, -0.03f, 0.0f),
			UINT32 timeForceLost = 1000);

		//! Creates a rotation affector. This affector rotates the particles
		//! around a specified pivot point. The speed is in Degrees per second.
		virtual IParticleRotationAffector* createRotationAffector(
			const Vector3& speed = Vector3(5.0f, 5.0f, 5.0f),
			const Vector3& pivotPoint = Vector3(0.0f, 0.0f, 0.0f));

		//! Sets the size of all particles.
		virtual void setParticleSize(
			const dimension2d<FLOAT32> &size = dimension2d<FLOAT32>(5.0f, 5.0f));

		//! Sets if the particles should be global. If they are, the particles are affected by
		//! the movement of the particle system scene node too, otherwise they completely
		//! ignore it. Default is true.
		virtual void setParticlesAreGlobal(bool global = true);

		//! Remove all currently visible particles
		virtual void clearParticles();

		//! Do manually update the particles.
		//! This should only be called when you want to render the node outside the scenegraph,
		//! as the node will care about this otherwise automatically.
		virtual void doParticleSystem(UINT32 time);

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_PARTICLE_SYSTEM; }

	private:

		void reallocateBuffers();

		list<IParticleAffector*>::type AffectorList;
		IParticleEmitter* Emitter;
		vector<SParticle>::type Particles;
		dimension2d<FLOAT32> ParticleSize;
		UINT32 LastEmitTime;
		SINT32 MaxParticles;

		SMeshBuffer* Buffer;

		enum E_PARTICLES_PRIMITIVE
		{
			EPP_POINT = 0,
			EPP_BILLBOARD,
			EPP_POINTSPRITE
		};
		E_PARTICLES_PRIMITIVE ParticlePrimitive;

		bool ParticlesAreGlobal;
	};
}

#endif