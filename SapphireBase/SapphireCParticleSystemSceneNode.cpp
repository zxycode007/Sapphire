#include "SapphireCParticleSystemSceneNode.h"
#include "SapphireOS.h"
#include "SapphireIScenesManager.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireIVideoDriver.h"

#include "SapphireCParticleAnimatedMeshSceneNodeEmitter.h"
#include "SapphireCParticleBoxEmitter.h"
#include "SapphireCParticleCylinderEmitter.h"
#include "SapphireCParticleMeshEmitter.h"
#include "SapphireCParticlePointEmitter.h"
#include "SapphireCParticleSphereEmitter.h"
#include "SapphireCParticlleRingEmitter.h"
#include "SapphireCParticleAttractionAffector.h"
#include "SapphireCParticleFadeOutAffector.h"
#include "SapphireCParticleGravityAffector.h"
#include "SapphireCParticleRotationAffector.h"
#include "SapphireCParticleScaleAffector.h"
#include "SapphireSViewFrustum.h"



namespace Sapphire
{

	//! constructor
	CParticleSystemSceneNode::CParticleSystemSceneNode(bool createDefaultEmitter,
		ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		const Vector3& position, const Vector3& rotation,
		const Vector3& scale)
		: IParticleSystemSceneNode(parent, mgr, id, position, rotation, scale),
		Emitter(0), ParticleSize(dimension2d<FLOAT32>(5.0f, 5.0f)), LastEmitTime(0),
		MaxParticles(0xffff), Buffer(0), ParticlesAreGlobal(true)
	{
#ifdef _DEBUG
		setDebugName("CParticleSystemSceneNode");
#endif

		Buffer = new SMeshBuffer();
		if (createDefaultEmitter)
		{
			IParticleEmitter* e = createBoxEmitter();
			setEmitter(e);
			e->drop();
		}
	}


	//! destructor
	CParticleSystemSceneNode::~CParticleSystemSceneNode()
	{
		if (Emitter)
			Emitter->drop();
		if (Buffer)
			Buffer->drop();

		removeAllAffectors();
	}


	//! Gets the particle emitter, which creates the particles.
	IParticleEmitter* CParticleSystemSceneNode::getEmitter()
	{
		return Emitter;
	}


	//! Sets the particle emitter, which creates the particles.
	void CParticleSystemSceneNode::setEmitter(IParticleEmitter* emitter)
	{
		if (emitter == Emitter)
			return;
		if (Emitter)
			Emitter->drop();

		Emitter = emitter;

		if (Emitter)
			Emitter->grab();
	}


	//! Adds new particle effector to the particle system.
	void CParticleSystemSceneNode::addAffector(IParticleAffector* affector)
	{
		affector->grab();
		AffectorList.push_back(affector);
	}

	//! Get a list of all particle affectors.
	const list<IParticleAffector*>::type& CParticleSystemSceneNode::getAffectors() const
	{
		return AffectorList;
	}

	//! Removes all particle affectors in the particle system.
	void CParticleSystemSceneNode::removeAllAffectors()
	{
		list<IParticleAffector*>::iterator it = AffectorList.begin();
		while (it != AffectorList.end())
		{
			(*it)->drop();
			it = AffectorList.erase(it);
		}
	}


	//! Returns the material based on the zero based index i.
	SMaterial& CParticleSystemSceneNode::getMaterial(UINT32 i)
	{
		return Buffer->Material;
	}


	//! Returns amount of materials used by this scene node.
	UINT32 CParticleSystemSceneNode::getMaterialCount() const
	{
		return 1;
	}


	//! Creates a particle emitter for an animated mesh scene node
	IParticleAnimatedMeshSceneNodeEmitter*
		CParticleSystemSceneNode::createAnimatedMeshSceneNodeEmitter(
		IAnimatedMeshSceneNode* node, bool useNormalDirection,
		const Vector3& direction, FLOAT32 normalDirectionModifier,
		SINT32 mbNumber, bool everyMeshVertex,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticleAnimatedMeshSceneNodeEmitter(node,
			useNormalDirection, direction, normalDirectionModifier,
			mbNumber, everyMeshVertex,
			minParticlesPerSecond, maxParticlesPerSecond,
			minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a box particle emitter.
	IParticleBoxEmitter* CParticleSystemSceneNode::createBoxEmitter(
		const AxisAlignedBox& box, const Vector3& direction,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax,
		SINT32 maxAngleDegrees, const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticleBoxEmitter(box, direction, minParticlesPerSecond,
			maxParticlesPerSecond, minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a particle emitter for emitting from a cylinder
	IParticleCylinderEmitter* CParticleSystemSceneNode::createCylinderEmitter(
		const Vector3& center, FLOAT32 radius,
		const Vector3& normal, FLOAT32 length,
		bool outlineOnly, const Vector3& direction,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticleCylinderEmitter(center, radius, normal, length,
			outlineOnly, direction,
			minParticlesPerSecond, maxParticlesPerSecond,
			minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a mesh particle emitter.
	IParticleMeshEmitter* CParticleSystemSceneNode::createMeshEmitter(
		IMesh* mesh, bool useNormalDirection,
		const Vector3& direction, FLOAT32 normalDirectionModifier,
		SINT32 mbNumber, bool everyMeshVertex,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticleMeshEmitter(mesh, useNormalDirection, direction,
			normalDirectionModifier, mbNumber, everyMeshVertex,
			minParticlesPerSecond, maxParticlesPerSecond,
			minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a point particle emitter.
	IParticlePointEmitter* CParticleSystemSceneNode::createPointEmitter(
		const Vector3& direction, UINT32 minParticlesPerSecond,
		UINT32 maxParticlesPerSecond, const ColourValue& minStartColor,
		const ColourValue& maxStartColor, UINT32 lifeTimeMin, UINT32 lifeTimeMax,
		SINT32 maxAngleDegrees, const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticlePointEmitter(direction, minParticlesPerSecond,
			maxParticlesPerSecond, minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a ring particle emitter.
	IParticleRingEmitter* CParticleSystemSceneNode::createRingEmitter(
		const Vector3& center, FLOAT32 radius, FLOAT32 ringThickness,
		const Vector3& direction,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax, SINT32 maxAngleDegrees,
		const dimension2df& minStartSize, const dimension2df& maxStartSize)
	{
		return new CParticleRingEmitter(center, radius, ringThickness, direction,
			minParticlesPerSecond, maxParticlesPerSecond, minStartColor,
			maxStartColor, lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a sphere particle emitter.
	IParticleSphereEmitter* CParticleSystemSceneNode::createSphereEmitter(
		const Vector3& center, FLOAT32 radius, const Vector3& direction,
		UINT32 minParticlesPerSecond, UINT32 maxParticlesPerSecond,
		const ColourValue& minStartColor, const ColourValue& maxStartColor,
		UINT32 lifeTimeMin, UINT32 lifeTimeMax,
		SINT32 maxAngleDegrees, const dimension2df& minStartSize,
		const dimension2df& maxStartSize)
	{
		return new CParticleSphereEmitter(center, radius, direction,
			minParticlesPerSecond, maxParticlesPerSecond,
			minStartColor, maxStartColor,
			lifeTimeMin, lifeTimeMax, maxAngleDegrees,
			minStartSize, maxStartSize);
	}


	//! Creates a point attraction affector. This affector modifies the positions of the
	//! particles and attracts them to a specified point at a specified speed per second.
	IParticleAttractionAffector* CParticleSystemSceneNode::createAttractionAffector(
		const Vector3& point, FLOAT32 speed, bool attract,
		bool affectX, bool affectY, bool affectZ)
	{
		return new CParticleAttractionAffector(point, speed, attract, affectX, affectY, affectZ);
	}

	//! Creates a scale particle affector.
	IParticleAffector* CParticleSystemSceneNode::createScaleParticleAffector(const dimension2df& scaleTo)
	{
		return new CParticleScaleAffector(scaleTo);
	}


	//! Creates a fade out particle affector.
	IParticleFadeOutAffector* CParticleSystemSceneNode::createFadeOutParticleAffector(
		const ColourValue& targetColor, UINT32 timeNeededToFadeOut)
	{
		return new CParticleFadeOutAffector(targetColor, timeNeededToFadeOut);
	}


	//! Creates a gravity affector.
	IParticleGravityAffector* CParticleSystemSceneNode::createGravityAffector(
		const Vector3& gravity, UINT32 timeForceLost)
	{
		return new CParticleGravityAffector(gravity, timeForceLost);
	}


	//! Creates a rotation affector. This affector rotates the particles around a specified pivot
	//! point.  The speed represents Degrees of rotation per second.
	IParticleRotationAffector* CParticleSystemSceneNode::createRotationAffector(
		const Vector3& speed, const Vector3& pivotPoint)
	{
		return new CParticleRotationAffector(speed, pivotPoint);
	}


	//! pre render event
	void CParticleSystemSceneNode::OnRegisterSceneNode()
	{
		doParticleSystem(Timer::getTime());

		if (IsVisible && (Particles.size() != 0))
		{
			SceneManager->registerNodeForRendering(this);
			ISceneNode::OnRegisterSceneNode();
		}
	}


	//! render
	void CParticleSystemSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();
		ICameraSceneNode* camera = SceneManager->getActiveCamera();

		if (!camera || !driver)
			return;


#if 0
		// calculate vectors for letting particles look to camera
		Vector3 view(camera->getTarget() - camera->getAbsolutePosition());
		view.normalize();

		view *= -1.0f;

#else

		const Matrix4 &m = camera->getViewFrustum()->getTransform(ETS_VIEW);

		
		//const Vector3 view(-m[2], -m[6], -m[10]);
		const Vector3 view = Vector3(-m.getCIndex(8), -m.getCIndex(9), -m.getCIndex(10));

#endif

		// reallocate arrays, if they are too small
		reallocateBuffers();

		// create particle vertex data
		SINT32 idx = 0;
		for (UINT32 i = 0; i<Particles.size(); ++i)
		{
			const SParticle& particle = Particles[i];

#if 0
			Vector3 horizontal = camera->getUpVector().crossProduct(view);
			horizontal.normalize();
			horizontal *= 0.5f * particle.size.Width;

			Vector3 vertical = horizontal.crossProduct(view);
			vertical.normalize();
			vertical *= 0.5f * particle.size.Height;

#else
			FLOAT32 f;

			f = 0.5f * particle.size.Width;
			//const Vector3 horizontal(m[0] * f, m[4] * f, m[8] * f);
			const Vector3 horizontal(m.getCIndex(0) * f, m.getCIndex(1) * f, m.getCIndex(2) * f);

			f = -0.5f * particle.size.Height;
			//const Vector3 vertical(m[1] * f, m[5] * f, m[9] * f);
			const Vector3 vertical(m.getCIndex(4) * f, m.getCIndex(5) * f, m.getCIndex(6) * f);
#endif

			Buffer->Vertices[0 + idx].Pos = particle.pos + horizontal + vertical;
			Buffer->Vertices[0 + idx].Color = particle.color;
			Buffer->Vertices[0 + idx].Normal = view;

			Buffer->Vertices[1 + idx].Pos = particle.pos + horizontal - vertical;
			Buffer->Vertices[1 + idx].Color = particle.color;
			Buffer->Vertices[1 + idx].Normal = view;

			Buffer->Vertices[2 + idx].Pos = particle.pos - horizontal - vertical;
			Buffer->Vertices[2 + idx].Color = particle.color;
			Buffer->Vertices[2 + idx].Normal = view;

			Buffer->Vertices[3 + idx].Pos = particle.pos - horizontal + vertical;
			Buffer->Vertices[3 + idx].Color = particle.color; 
			Buffer->Vertices[3 + idx].Normal = view;

			idx += 4;
		}

		// render all
		Matrix4 mat;
		if (!ParticlesAreGlobal)
			mat.setTrans(AbsoluteTransformation.getTrans());
		driver->setTransform(ETS_WORLD, mat);

		driver->setMaterial(Buffer->Material);

		driver->drawVertexPrimitiveList(Buffer->getVertices(), Particles.size() * 4,
			Buffer->getIndices(), Particles.size() * 2, EVT_STANDARD, EPT_TRIANGLES, Buffer->getIndexType());

		// for debug purposes only:
		if (DebugDataVisible & EDS_BBOX)
		{
			driver->setTransform(ETS_WORLD, AbsoluteTransformation);
			SMaterial deb_m;
			deb_m.Lighting = false;
			driver->setMaterial(deb_m);
			driver->draw3DBox(Buffer->BoundingBox, ColourValue(0, 255, 255, 255));
		}
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CParticleSystemSceneNode::getBoundingBox() const
	{
		return Buffer->getBoundingBox();
	}


	void CParticleSystemSceneNode::doParticleSystem(UINT32 time)
	{
		if (LastEmitTime == 0)
		{
			LastEmitTime = time;
			return;
		}

		UINT32 now = time;
		UINT32 timediff = time - LastEmitTime;
		LastEmitTime = time;

		// run emitter

		if (Emitter && IsVisible)
		{
			SParticle* array = 0;
			SINT32 newParticles = Emitter->emitt(now, timediff, array);

			if (newParticles && array)
			{
				SINT32 j = Particles.size();
				if (newParticles > 16250 - j)
					newParticles = 16250 - j;
				//Particles.set_used(j + newParticles);
				Particles.resize(j + newParticles);
				for (SINT32 i = j; i<j + newParticles; ++i)
				{
					Particles[i] = array[i - j];
					AbsoluteTransformation.rotateVect(Particles[i].startVector);
					if (ParticlesAreGlobal)
						//AbsoluteTransformation.transformVect(Particles[i].pos);
						AbsoluteTransformation.transformAffine(Particles[i].pos);
				}
			}
		}

		// run affectors
		list<IParticleAffector*>::iterator ait = AffectorList.begin();
		for (; ait != AffectorList.end(); ++ait)
			(*ait)->affect(now, Particles.begin()._Ptr, Particles.size());

		if (ParticlesAreGlobal)
			Buffer->BoundingBox.reset(AbsoluteTransformation.getTrans());
		else
			Buffer->BoundingBox.reset(Vector3(0, 0, 0));

		// animate all particles
		FLOAT32 scale = (FLOAT32)timediff;

		for (UINT32 i = 0; i<Particles.size();)
		{
			// erase is pretty expensive!
			if (now > Particles[i].endTime)
			{
				// Particle order does not seem to matter.
				// So we can delete by switching with last particle and deleting that one.
				// This is a lot faster and speed is very important here as the erase otherwise
				// can cause noticable freezes.
				Particles[i] = Particles[Particles.size() - 1];
				//Particles.erase(Particles.size() - 1);
				Particles.erase(Particles.end() - 1);
			}
			else
			{
				Particles[i].pos += (Particles[i].vector * scale);
				Buffer->BoundingBox.addInternalPoint(Particles[i].pos);
				++i;
			}
		}

		const FLOAT32 m = (ParticleSize.Width > ParticleSize.Height ? ParticleSize.Width : ParticleSize.Height) * 0.5f;
		Buffer->BoundingBox.getMaximum().x += m;
		Buffer->BoundingBox.getMaximum().y += m;
		Buffer->BoundingBox.getMaximum().z += m;

		Buffer->BoundingBox.getMinimum().x -= m;
		Buffer->BoundingBox.getMinimum().y -= m;
		Buffer->BoundingBox.getMinimum().z -= m;

		if (ParticlesAreGlobal)
		{
			//Matrix4 absinv(AbsoluteTransformation, Matrix4::EM4CONST_INVERSE);
			Matrix4 absinv = AbsoluteTransformation.inverse();
			//absinv.transformBoxEx(Buffer->BoundingBox);
			Buffer->BoundingBox.transformAffine(absinv);
		}
	}


	//! Sets if the particles should be global. If it is, the particles are affected by
	//! the movement of the particle system scene node too, otherwise they completely
	//! ignore it. Default is true.
	void CParticleSystemSceneNode::setParticlesAreGlobal(bool global)
	{
		ParticlesAreGlobal = global;
	}

	//! Remove all currently visible particles
	void CParticleSystemSceneNode::clearParticles()
	{
		//Particles.set_used(0);
		Particles.clear();
	}

	//! Sets the size of all particles.
	void CParticleSystemSceneNode::setParticleSize(const dimension2d<FLOAT32> &size)
	{
		Printer::log("setParticleSize is deprecated, use setMinStartSize/setMaxStartSize in emitter.", LML_WARNING);
		//A bit of a hack, but better here than in the particle code
		if (Emitter)
		{
			Emitter->setMinStartSize(size);
			Emitter->setMaxStartSize(size);
		}
		ParticleSize = size;
	}


	void CParticleSystemSceneNode::reallocateBuffers()
	{
		if (Particles.size() * 4 > Buffer->getVertexCount() ||
			Particles.size() * 6 > Buffer->getIndexCount())
		{
			UINT32 oldSize = Buffer->getVertexCount();
			//Buffer->Vertices.set_used(Particles.size() * 4);
			Buffer->Vertices.resize(Particles.size() * 4);

			UINT32 i;

			// fill remaining vertices
			for (i = oldSize; i<Buffer->Vertices.size(); i += 4)
			{
				Buffer->Vertices[0 + i].TCoords.set(0.0f, 0.0f);
				Buffer->Vertices[1 + i].TCoords.set(0.0f, 1.0f);
				Buffer->Vertices[2 + i].TCoords.set(1.0f, 1.0f);
				Buffer->Vertices[3 + i].TCoords.set(1.0f, 0.0f);
			}

			// fill remaining indices
			UINT32 oldIdxSize = Buffer->getIndexCount();
			UINT32 oldvertices = oldSize;
			//Buffer->Indices.set_used(Particles.size() * 6);
			Buffer->Indices.resize(Particles.size() * 6);

			for (i = oldIdxSize; i<Buffer->Indices.size(); i += 6)
			{
				Buffer->Indices[0 + i] = (UINT16)0 + oldvertices;
				Buffer->Indices[1 + i] = (UINT16)2 + oldvertices;
				Buffer->Indices[2 + i] = (UINT16)1 + oldvertices;
				Buffer->Indices[3 + i] = (UINT16)0 + oldvertices;
				Buffer->Indices[4 + i] = (UINT16)3 + oldvertices;
				Buffer->Indices[5 + i] = (UINT16)2 + oldvertices;
				oldvertices += 4;
			}
		}
	}


	//! Writes attributes of the scene node.
	void CParticleSystemSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		IParticleSystemSceneNode::serializeAttributes(out, options);

		out->addBool("GlobalParticles", ParticlesAreGlobal);
		out->addFloat("ParticleWidth", ParticleSize.Width);
		out->addFloat("ParticleHeight", ParticleSize.Height);

		// write emitter

		E_PARTICLE_EMITTER_TYPE type = EPET_COUNT;
		if (Emitter)
			type = Emitter->getType();

		out->addEnum("Emitter", (SINT32)type, ParticleEmitterTypeNames);

		if (Emitter)
			Emitter->serializeAttributes(out, options);

		// write affectors

		E_PARTICLE_AFFECTOR_TYPE atype = EPAT_NONE;

		for (list<IParticleAffector*>::const_iterator it = AffectorList.begin();
			it != AffectorList.end(); ++it)
		{
			atype = (*it)->getType();

			out->addEnum("Affector", (SINT32)atype, ParticleAffectorTypeNames);

			(*it)->serializeAttributes(out);
		}

		// add empty affector to make it possible to add further affectors

		if (options && options->Flags & EARWF_FOR_EDITOR)
			out->addEnum("Affector", EPAT_NONE, ParticleAffectorTypeNames);
	}


	//! Reads attributes of the scene node.
	void CParticleSystemSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		IParticleSystemSceneNode::deserializeAttributes(in, options);

		ParticlesAreGlobal = in->getAttributeAsBool("GlobalParticles");
		ParticleSize.Width = in->getAttributeAsFloat("ParticleWidth");
		ParticleSize.Height = in->getAttributeAsFloat("ParticleHeight");

		// read emitter

		int emitterIdx = in->findAttribute("Emitter");
		if (emitterIdx == -1)
			return;

		if (Emitter)
			Emitter->drop();
		Emitter = 0;

		E_PARTICLE_EMITTER_TYPE type = (E_PARTICLE_EMITTER_TYPE)
			in->getAttributeAsEnumeration("Emitter", ParticleEmitterTypeNames);

		switch (type)
		{
		case EPET_POINT:
			Emitter = createPointEmitter();
			break;
		case EPET_ANIMATED_MESH:
			Emitter = createAnimatedMeshSceneNodeEmitter(NULL); // we can't set the node - the user will have to do this
			break;
		case EPET_BOX:
			Emitter = createBoxEmitter();
			break;
		case EPET_CYLINDER:
			Emitter = createCylinderEmitter(Vector3(0, 0, 0), 10.f, Vector3(0, 1, 0), 10.f);	// (values here don't matter)
			break;
		case EPET_MESH:
			Emitter = createMeshEmitter(NULL);	// we can't set the mesh - the user will have to do this
			break;
		case EPET_RING:
			Emitter = createRingEmitter(Vector3(0, 0, 0), 10.f, 10.f);	// (values here don't matter)
			break;
		case EPET_SPHERE:
			Emitter = createSphereEmitter(Vector3(0, 0, 0), 10.f);	// (values here don't matter)
			break;
		default:
			break;
		}

		UINT32 idx = 0;

#if 0
		if (Emitter)
			idx = Emitter->deserializeAttributes(idx, in);

		++idx;
#else
		if (Emitter)
			Emitter->deserializeAttributes(in);
#endif

		// read affectors

		removeAllAffectors();
		UINT32 cnt = in->getAttributeCount();

		while (idx < cnt)
		{
			const char* name = in->getAttributeName(idx);

			if (!name || strcmp("Affector", name))
				return;

			E_PARTICLE_AFFECTOR_TYPE atype =
				(E_PARTICLE_AFFECTOR_TYPE)in->getAttributeAsEnumeration(idx, ParticleAffectorTypeNames);

			IParticleAffector* aff = 0;

			switch (atype)
			{
			case EPAT_ATTRACT:
				aff = createAttractionAffector(Vector3(0, 0, 0));
				break;
			case EPAT_FADE_OUT:
				aff = createFadeOutParticleAffector();
				break;
			case EPAT_GRAVITY:
				aff = createGravityAffector();
				break;
			case EPAT_ROTATE:
				aff = createRotationAffector();
				break;
			case EPAT_SCALE:
				aff = createScaleParticleAffector();
				break;
			case EPAT_NONE:
			default:
				break;
			}

			++idx;

			if (aff)
			{
#if 0
				idx = aff->deserializeAttributes(idx, in, options);
				++idx;
#else
				aff->deserializeAttributes(in, options);
#endif

				addAffector(aff);
				aff->drop();
			}
		}
	}
}