#include "SapphireCWaterSurfaceSceneNode.h"
#include "SapphireIScenesManager.h"
#include "SapphireIMeshManipulator.h"
#include "SapphireIMeshCache.h"
#include "SapphireSVertex.h"
#include "SapphireSMesh.h"
#include "SapphireOS.h"


namespace Sapphire
{

	//! constructor
	CWaterSurfaceSceneNode::CWaterSurfaceSceneNode(FLOAT32 waveHeight, FLOAT32 waveSpeed, FLOAT32 waveLength,
		IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		const Vector3& position, const Vector3& rotation,
		const Vector3& scale)
		: CMeshSceneNode(mesh, parent, mgr, id, position, rotation, scale),
		WaveLength(waveLength), WaveSpeed(waveSpeed), WaveHeight(waveHeight),
		OriginalMesh(0)
	{
#ifdef _DEBUG
		setDebugName("CWaterSurfaceSceneNode");
#endif

		setMesh(mesh);
	}


	//! destructor
	CWaterSurfaceSceneNode::~CWaterSurfaceSceneNode()
	{
		// Mesh is dropped in CMeshSceneNode destructor
		if (OriginalMesh)
			OriginalMesh->drop();
	}


	//! frame
	void CWaterSurfaceSceneNode::OnRegisterSceneNode()
	{
		CMeshSceneNode::OnRegisterSceneNode();
	}


	void CWaterSurfaceSceneNode::OnAnimate(UINT32 timeMs)
	{
		if (Mesh && IsVisible)
		{
			const UINT32 meshBufferCount = Mesh->getMeshBufferCount();
			const FLOAT32 time = timeMs / WaveSpeed;

			for (UINT32 b = 0; b<meshBufferCount; ++b)
			{
				const UINT32 vtxCnt = Mesh->getMeshBuffer(b)->getVertexCount();

				for (UINT32 i = 0; i<vtxCnt; ++i)
					Mesh->getMeshBuffer(b)->getPosition(i).y = addWave(
					OriginalMesh->getMeshBuffer(b)->getPosition(i),
					time);
			}// end for all mesh buffers
			Mesh->setDirty(EBT_VERTEX);

			SceneManager->getMeshManipulator()->recalculateNormals(Mesh);
		}
		CMeshSceneNode::OnAnimate(timeMs);
	}


	void CWaterSurfaceSceneNode::setMesh(IMesh* mesh)
	{
		CMeshSceneNode::setMesh(mesh);
		if (!mesh)
			return;
		if (OriginalMesh)
			OriginalMesh->drop();
		IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(mesh);
		OriginalMesh = mesh;
		Mesh = clone;
		Mesh->setHardwareMappingHint(EHM_STATIC, EBT_INDEX);
		//	Mesh->setHardwareMappingHint(EHM_STREAM, EBT_VERTEX);
	}


	//! Writes attributes of the scene node.
	void CWaterSurfaceSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addFloat("WaveLength", WaveLength);
		out->addFloat("WaveSpeed", WaveSpeed);
		out->addFloat("WaveHeight", WaveHeight);

		CMeshSceneNode::serializeAttributes(out, options);
		// serialize original mesh
		out->setAttribute("Mesh", SceneManager->getMeshCache()->getMeshName(OriginalMesh).getPath().c_str());
	}


	//! Reads attributes of the scene node.
	void CWaterSurfaceSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		WaveLength = in->getAttributeAsFloat("WaveLength");
		WaveSpeed = in->getAttributeAsFloat("WaveSpeed");
		WaveHeight = in->getAttributeAsFloat("WaveHeight");

		if (Mesh)
		{
			Mesh->drop();
			Mesh = OriginalMesh;
			OriginalMesh = 0;
		}
		// deserialize original mesh
		CMeshSceneNode::deserializeAttributes(in, options);

		if (Mesh)
		{
			IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(Mesh);
			OriginalMesh = Mesh;
			Mesh = clone;
		}
	}


	FLOAT32 CWaterSurfaceSceneNode::addWave(const Vector3 &source, FLOAT32 time) const
	{
		return source.y +
			(sinf(((source.x / WaveLength) + time)) * WaveHeight) +
			(cosf(((source.z / WaveLength) + time)) * WaveHeight);
	}
}