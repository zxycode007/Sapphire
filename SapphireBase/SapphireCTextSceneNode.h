#ifndef _SAPPHIRE_C_TEXT_SCENE_NODE_
#define _SAPPHIRE_C_TEXT_SCENE_NODE_

#include "SapphireITextSceneNode.h"
#include "SapphireIBillboardTextSceneNode.h"
#include "SapphireFont.h"
#include "SapphireIGUIFontBitmap.h"
#include "SapphireISceneCollisionManager.h"
#include "SapphireSMesh.h"

namespace Sapphire
{
	class CTextSceneNode : public ITextSceneNode
	{
	public:

		//! constructor
		CTextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			IGUIFont* font, ISceneCollisionManager* coll,
			const Vector3& position = Vector3(0, 0, 0), const wchar_t* text = 0,
			ColourValue color = ColourValue::getColourValue(100, 0, 0, 0));

		//! destructor
		virtual ~CTextSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! sets the text string
		virtual void setText(const wchar_t* text);

		//! sets the color of the text
		virtual void setTextColor(ColourValue color);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_TEXT; }

	private:

		StringW Text;
		ColourValue Color;
		IGUIFont* Font;
		ISceneCollisionManager* Coll;
		AxisAlignedBox Box;
	};

	class CBillboardTextSceneNode : public IBillboardTextSceneNode
	{
	public:

		CBillboardTextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			IGUIFont* font, const wchar_t* text,
			const Vector3& position, const dimension2df& size,
			ColourValue colorTop, ColourValue shade_bottom);

		//! destructor
		virtual ~CBillboardTextSceneNode();

		//! sets the vertex positions etc
		virtual void OnAnimate(UINT32 timeMs);

		//! registers the node into the transparent pass
		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! sets the text string
		virtual void setText(const wchar_t* text);

		//! sets the color of the text
		virtual void setTextColor(ColourValue color);

		//! sets the size of the billboard
		virtual void setSize(const dimension2df& size);

		//! gets the size of the billboard
		virtual const dimension2df& getSize() const;

		virtual SMaterial& getMaterial(UINT32 i);

		//! returns amount of materials used by this scene node.
		virtual UINT32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_TEXT; }

		//! Set the color of all vertices of the billboard
		//! \param overallColor: the color to set
		virtual void setColor(const ColourValue & overallColor);

		//! Set the color of the top and bottom vertices of the billboard
		//! \param topColor: the color to set the top vertices
		//! \param bottomColor: the color to set the bottom vertices
		virtual void setColor(const ColourValue & topColor, const ColourValue & bottomColor);

		//! Gets the color of the top and bottom vertices of the billboard
		//! \param topColor: stores the color of the top vertices
		//! \param bottomColor: stores the color of the bottom vertices
		virtual void getColor(ColourValue & topColor, ColourValue & bottomColor) const;

		virtual void setSize(FLOAT32 height, FLOAT32 bottomEdgeWidth, FLOAT32 topEdgeWidth)
		{
			setSize(dimension2df(bottomEdgeWidth, height));
		}

		virtual void getSize(FLOAT32& height, FLOAT32& bottomEdgeWidth, FLOAT32& topEdgeWidth) const
		{
			height = Size.Height;
			bottomEdgeWidth = Size.Width;
			topEdgeWidth = Size.Width;
		}

	private:

		StringW Text;
		ColourValue Color;
		IGUIFontBitmap* Font;

		dimension2df Size;
		AxisAlignedBox BBox;
		SMaterial Material;

		ColourValue ColorTop;
		ColourValue ColorBottom;
		struct SSymbolInfo
		{
			UINT32 bufNo;
			FLOAT32 Width;
			FLOAT32 Kerning;
			UINT32 firstInd;
			UINT32 firstVert;
		};

		vector < SSymbolInfo >::type Symbol;

		SMesh *Mesh;
	};
}
#endif