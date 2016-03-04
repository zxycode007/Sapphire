#ifndef _SAPPHIRE_C_BILLBOARD_SCENE_NODE_
#define _SAPPHIRE_C_BILLBOARD_SCENE_NODE_


#include "SapphireIBillboardSceneNode.h"
#include "SapphireSVertex.h"

namespace Sapphire
{

	//! Scene node which is a billboard. A billboard is like a 3d sprite: A 2d element,
	//! which always looks to the camera. 
	class CBillboardSceneNode : virtual public IBillboardSceneNode
	{
	public:

		//! constructor
		CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position, const dimension2d<Real>& size,
			ColourValue colorTop = ColourValue(0xFFFFFFFF),
			ColourValue colorBottom = ColourValue(0xFFFFFFFF));

		//! pre render event
		virtual void OnRegisterSceneNode();

		//! render
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! sets the size of the billboard
		virtual void setSize(const dimension2d<Real>& size);

		//! Sets the widths of the top and bottom edges of the billboard independently.
		virtual void setSize(Real height, Real bottomEdgeWidth, Real topEdgeWidth);

		//! gets the size of the billboard
		virtual const dimension2df& getSize() const;

		//! Gets the widths of the top and bottom edges of the billboard.
		virtual void getSize(Real& height, Real& bottomEdgeWidth, Real& topEdgeWidth) const;

		virtual SMaterial& getMaterial(UINT32 i);

		//! returns amount of materials used by this scene node.
		virtual UINT32 getMaterialCount() const;

		//! Set the color of all vertices of the billboard
		//! \param overallColor: the color to set
		virtual void setColor(const ColourValue& overallColor);

		//! Set the color of the top and bottom vertices of the billboard
		//! \param topColor: the color to set the top vertices
		//! \param bottomColor: the color to set the bottom vertices
		virtual void setColor(const ColourValue& topColor,
			const ColourValue& bottomColor);

		//! Gets the color of the top and bottom vertices of the billboard
		//! \param[out] topColor: stores the color of the top vertices
		//! \param[out] bottomColor: stores the color of the bottom vertices
		virtual void getColor(ColourValue& topColor,
			ColourValue& bottomColor) const;

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_BILLBOARD; }

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);

	private:

		//! Size.Width is the bottom edge width
		dimension2df Size;
		Real TopEdgeWidth;
		AxisAlignedBox BBox;
		SMaterial Material;

		S3DVertex vertices[4];
		UINT16 indices[6];
	};
}

#endif