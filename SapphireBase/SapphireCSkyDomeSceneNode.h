#ifndef _SAPPHIRE_C_SKY_DOME_SCENE_NODE_
#define _SAPPHIRE_C_SKY_DOME_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireSMeshBuffer.h"

namespace Sapphire
{

	class CSkyDomeSceneNode : public ISceneNode
	{
	public:
		CSkyDomeSceneNode(ITexture* texture, UINT32 horiRes, UINT32 vertRes,
			FLOAT32 texturePercentage, FLOAT32 spherePercentage, FLOAT32 radius,
			ISceneNode* parent, ISceneManager* smgr, SINT32 id);
		virtual ~CSkyDomeSceneNode();
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const AxisAlignedBox& getBoundingBox() const;
		virtual SMaterial& getMaterial(UINT32 i);
		virtual UINT32 getMaterialCount() const;
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SKY_DOME; }

		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

	private:

		void generateMesh();

		SMeshBuffer* Buffer;

		UINT32 HorizontalResolution, VerticalResolution;
		FLOAT32 TexturePercentage, SpherePercentage, Radius;
	};

}


#endif