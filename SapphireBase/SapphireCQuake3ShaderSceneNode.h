#ifndef _SAPPHIRE_C_QUAKE3_SHADER_SCREEN_NODE_
#define _SAPPHIRE_C_QUAKE3_SHADER_SCREEN_NODE_

#include "SapphireIMeshSceneNode.h"
#include "SapphireIQ3Shader.h"
#include "SapphireIFileSystem.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireSMeshBufferLightMap.h"
#include "SapphireSMesh.h"
#include "SapphireIScenesManager.h"

namespace Sapphire
{

	//! Scene node which is a Quake3 shader.
	class CQuake3ShaderSceneNode : public IMeshSceneNode
	{
	public:

		CQuake3ShaderSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			IFileSystem* fileSystem,
			const IMeshBuffer* original,
			const Quake3::IShader* shader
			);

		virtual ~CQuake3ShaderSceneNode();

		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual void OnAnimate(UINT32 timeMs);
		virtual const AxisAlignedBox& getBoundingBox() const;

		virtual UINT32 getMaterialCount() const;
		virtual SMaterial& getMaterial(UINT32 i);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_Q3SHADER_SCENE_NODE; }

		virtual void setMesh(IMesh* mesh){}
		virtual IMesh* getMesh() { return Mesh; }
		virtual void setReadOnlyMaterials(bool readonly) {}
		virtual bool isReadOnlyMaterials() const { return true; }

		//! Creates shadow volume scene node as child of this node
		//! and returns a pointer to it.
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
			SINT32 id, bool zfailmethod = true, FLOAT32 infinity = 10000.0f);

		//! Removes a child from this scene node.
		//! Implemented here, to be able to remove the shadow properly, if there is one,
		//! or to remove attached childs.
		virtual bool removeChild(ISceneNode* child);

	private:
		const Quake3::IShader* Shader;
		SMesh *Mesh;
		IShadowVolumeSceneNode* Shadow;
		const SMeshBufferLightMap* Original;
		SMeshBuffer* MeshBuffer;
		Vector3 MeshOffset;

		struct SQ3Texture
		{
			SQ3Texture() :
				TextureIndex(0),
				TextureFrequency(0.f),
				TextureAddressMode(ETC_REPEAT)
			{
				//Texture.setAllocStrategy(ALLOC_STRATEGY_SAFE);
			}

			Quake3::tTexArray Texture;

			UINT32 TextureIndex;
			FLOAT32 TextureFrequency;
			E_TEXTURE_CLAMP TextureAddressMode;	// Wrapping/Clamping
		};

		vector< SQ3Texture >::type Q3Texture;

		void loadTextures(IFileSystem * fileSystem);
		void addBuffer(SMeshBufferLightMap * buffer);
		void cloneBuffer(SMeshBuffer *dest, const SMeshBufferLightMap * buffer, bool translateCenter);

		void deformvertexes_wave(FLOAT32 dt, Quake3::SModifierFunction &function);
		void deformvertexes_move(FLOAT32 dt, Quake3::SModifierFunction &function);
		void deformvertexes_bulge(FLOAT32 dt, Quake3::SModifierFunction &function);
		void deformvertexes_autosprite(FLOAT32 dt, Quake3::SModifierFunction &function);
		void deformvertexes_autosprite2(FLOAT32 dt, Quake3::SModifierFunction &function);
		void deformvertexes_normal(FLOAT32 dt, Quake3::SModifierFunction &function);

		void vertextransform_tcgen(FLOAT32 dt, Quake3::SModifierFunction &function);
		void vertextransform_rgbgen(FLOAT32 dt, Quake3::SModifierFunction &function);
		void vertextransform_alphagen(FLOAT32 dt, Quake3::SModifierFunction &function);

		void transformtex(const Matrix4 &m, const UINT32 clamp);

		FLOAT32 TimeAbs;

		void animate(UINT32 stage, Matrix4 &texture);

		E_SCENE_NODE_RENDER_PASS getRenderStage() const;

	};
}

#endif