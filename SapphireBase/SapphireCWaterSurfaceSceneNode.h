#ifndef _SAPPHIRE_C_WATER_SURFACE_SCENE_NODE_
#define _SAPPHIRE_C_WATER_SURFACE_SCENE_NODE_

#include "SapphireCMeshSceneNode.h"

namespace Sapphire
{
	class CWaterSurfaceSceneNode : public CMeshSceneNode
	{
	public:

		//! constructor
		CWaterSurfaceSceneNode(FLOAT32 waveHeight, FLOAT32 waveSpeed, FLOAT32 waveLength,
			IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CWaterSurfaceSceneNode();

		//! frame registration
		virtual void OnRegisterSceneNode();

		//! animated update
		virtual void OnAnimate(UINT32 timeMs);

		//! Update mesh
		virtual void setMesh(IMesh* mesh);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_WATER_SURFACE; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		inline FLOAT32 addWave(const Vector3 &source, FLOAT32 time) const;

		FLOAT32 WaveLength;
		FLOAT32 WaveSpeed;
		FLOAT32 WaveHeight;
		IMesh* OriginalMesh;
	};
}

#endif