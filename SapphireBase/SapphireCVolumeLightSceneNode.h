#ifndef _SAPPHIRE_C_VOLUME_LIGHT_SCENE_NODE_
#define _SAPPHIRE_C_VOLUME_LIGHT_SCENE_NODE_

#include "SapphireIVolumeLightSceneNode.h"
#include "SapphireIMesh.h"


namespace Sapphire
{
	class CVolumeLightSceneNode : public IVolumeLightSceneNode
	{
	public:

		//! constructor
		CVolumeLightSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const UINT32 subdivU = 32, const UINT32 subdivV = 32,
			const ColourValue foot = ColourValue::getColourValue(51, 0, 230, 180),
			const ColourValue tail = ColourValue::getColourValue(0, 0, 0, 0),
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		virtual ~CVolumeLightSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! returns the material based on the zero based index i.
		virtual SMaterial& getMaterial(UINT32 i);

		//! returns amount of materials used by this scene node.
		virtual UINT32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_VOLUME_LIGHT; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

		virtual void setSubDivideU(const UINT32 inU);
		virtual void setSubDivideV(const UINT32 inV);

		virtual UINT32 getSubDivideU() const { return SubdivideU; }
		virtual UINT32 getSubDivideV() const { return SubdivideV; }

		virtual void setFootColor(const ColourValue inColor);
		virtual void setTailColor(const ColourValue inColor);

		virtual ColourValue getFootColor() const { return FootColor; }
		virtual ColourValue getTailColor() const { return TailColor; }

	private:
		void constructLight();

		IMesh* Mesh;

		FLOAT32  LPDistance;		// Distance to hypothetical lightsource point -- affects fov angle

		UINT32  SubdivideU;		// Number of subdivisions in U and V space.
		UINT32  SubdivideV;		// Controls the number of "slices" in the volume.
		// NOTE : Total number of polygons = 2 + ((SubdivideU + 1) + (SubdivideV + 1)) * 2
		// Each slice being a quad plus the rectangular plane at the bottom.

		ColourValue FootColor;	// Color at the source
		ColourValue TailColor;	// Color at the end.

		Vector3 LightDimensions; // LightDimensions.Y Distance of shooting -- Length of beams
		// LightDimensions.X and LightDimensions.Z determine the size/dimension of the plane
	};
}

#endif