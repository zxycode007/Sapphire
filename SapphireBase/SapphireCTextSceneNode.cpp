#include "SapphireCTextSceneNode.h"
#include "SapphireIScenesManager.h"
#include "SapphireIVideoDriver.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireOS.h"


namespace Sapphire
{

	//! constructor
	CTextSceneNode::CTextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		IGUIFont* font, ISceneCollisionManager* coll,
		const Vector3& position, const wchar_t* text,
		ColourValue color)
		: ITextSceneNode(parent, mgr, id, position), Text(text), Color(color),
		Font(font), Coll(coll)

	{
#ifdef _DEBUG
		setDebugName("CTextSceneNode");
#endif

		if (Font)
			Font->grab();

		setAutomaticCulling(EAC_OFF);
	}

	//! destructor
	CTextSceneNode::~CTextSceneNode()
	{
		if (Font)
			Font->drop();
	}

	void CTextSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

		ISceneNode::OnRegisterSceneNode();
	}

	//! renders the node.
	void CTextSceneNode::render()
	{
		if (!Font || !Coll)
			return;

		Position2d pos = Coll->getScreenCoordinatesFrom3DPosition(getAbsolutePosition(),
			SceneManager->getActiveCamera());

		rect<SINT32> r(pos, dimension2d<SINT32>(1, 1));
		Font->draw(Text.c_str(), r, Color, true, true);
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CTextSceneNode::getBoundingBox() const
	{
		return Box;
	}

	//! sets the text string
	void CTextSceneNode::setText(const wchar_t* text)
	{
		Text = text;
	}


	//! sets the color of the text
	void CTextSceneNode::setTextColor(ColourValue color)
	{
		Color = color;
	}


	//!--------------------------------- CBillboardTextSceneNode ----------------------------------------------


	//! constructor
	CBillboardTextSceneNode::CBillboardTextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		IGUIFont* font, const wchar_t* text,
		const Vector3& position, const dimension2df& size,
		ColourValue colorTop, ColourValue shade_bottom)
		: IBillboardTextSceneNode(parent, mgr, id, position),
		Font(0), ColorTop(colorTop), ColorBottom(shade_bottom), Mesh(0)
	{
#ifdef _DEBUG
		setDebugName("CBillboardTextSceneNode");
#endif

		Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
		Material.MaterialTypeParam = 1.f / 255.f;
		Material.BackfaceCulling = false;
		Material.Lighting = false;
		Material.ZBuffer = ECFN_LESSEQUAL;
		Material.ZWriteEnable = false;

		if (font)
		{
			// doesn't support other font types
			if (font->getType() == EGFT_BITMAP)
			{
				Font = (IGUIFontBitmap*)font;
				Font->grab();

				// mesh with one buffer per texture
				Mesh = new SMesh();
				for (UINT32 i = 0; i<Font->getSpriteBank()->getTextureCount(); ++i)
				{
					SMeshBuffer *mb = new SMeshBuffer();
					mb->Material = Material;
					mb->Material.setTexture(0, Font->getSpriteBank()->getTexture(i));
					Mesh->addMeshBuffer(mb);
					mb->drop();
				}
			}
			else
			{
				Printer::log("Sorry, CBillboardTextSceneNode does not support this font type", LML_NORMAL);
			}
		}

		setText(text);
		setSize(size);

		setAutomaticCulling(EAC_BOX);
	}



	CBillboardTextSceneNode::~CBillboardTextSceneNode()
	{
		if (Font)
			Font->drop();

		if (Mesh)
			Mesh->drop();

	}


	//! sets the text string
	void CBillboardTextSceneNode::setText(const wchar_t* text)
	{
		if (!Mesh)
			return;

		Text = text;

		Symbol.clear();

		// clear mesh
		for (UINT32 j = 0; j < Mesh->getMeshBufferCount(); ++j)
		{
			((SMeshBuffer*)Mesh->getMeshBuffer(j))->Indices.clear();
			((SMeshBuffer*)Mesh->getMeshBuffer(j))->Vertices.clear();
		}

		if (!Font)
			return;

		const vector< rect<SINT32> >::type &sourceRects = Font->getSpriteBank()->getPositions();
		const vector< SGUISprite >::type &sprites = Font->getSpriteBank()->getSprites();

		FLOAT32 dim[2];
		FLOAT32 tex[4];

		UINT32 i;
		for (i = 0; i != Text.size(); ++i)
		{
			SSymbolInfo info;

			UINT32 spriteno = Font->getSpriteNoFromChar(&text[i]);
			UINT32 rectno = sprites[spriteno].Frames[0].rectNumber;
			UINT32 texno = sprites[spriteno].Frames[0].textureNumber;

			dim[0] = reciprocal((FLOAT32)Font->getSpriteBank()->getTexture(texno)->getSize().Width);
			dim[1] = reciprocal((FLOAT32)Font->getSpriteBank()->getTexture(texno)->getSize().Height);

			const rect<SINT32>& s = sourceRects[rectno];

			// add space for letter to buffer
			SMeshBuffer* buf = (SMeshBuffer*)Mesh->getMeshBuffer(texno);
			UINT32 firstInd = buf->Indices.size();
			UINT32 firstVert = buf->Vertices.size();
			//buf->Indices.set_used(firstInd + 6);
			//buf->Vertices.set_used(firstVert + 4);

			tex[0] = (s.LowerRightCorner.x * dim[0]) + 0.5f*dim[0]; // half pixel
			tex[1] = (s.LowerRightCorner.y * dim[1]) + 0.5f*dim[1];
			tex[2] = (s.UpperLeftCorner.y  * dim[1]) - 0.5f*dim[1];
			tex[3] = (s.UpperLeftCorner.x  * dim[0]) - 0.5f*dim[0];

			buf->Vertices[firstVert + 0].TCoords.set(tex[0], tex[1]);
			buf->Vertices[firstVert + 1].TCoords.set(tex[0], tex[2]);
			buf->Vertices[firstVert + 2].TCoords.set(tex[3], tex[2]);
			buf->Vertices[firstVert + 3].TCoords.set(tex[3], tex[1]);

			buf->Vertices[firstVert + 0].Color = ColorBottom;
			buf->Vertices[firstVert + 3].Color = ColorBottom;
			buf->Vertices[firstVert + 1].Color = ColorTop;
			buf->Vertices[firstVert + 2].Color = ColorTop;

			buf->Indices[firstInd + 0] = (UINT16)firstVert + 0;
			buf->Indices[firstInd + 1] = (UINT16)firstVert + 2;
			buf->Indices[firstInd + 2] = (UINT16)firstVert + 1;
			buf->Indices[firstInd + 3] = (UINT16)firstVert + 0;
			buf->Indices[firstInd + 4] = (UINT16)firstVert + 3;
			buf->Indices[firstInd + 5] = (UINT16)firstVert + 2;

			wchar_t *tp = 0;
			if (i>0)
				tp = &Text[i - 1];

			info.Width = (FLOAT32)s.getWidth();
			info.bufNo = texno;
			info.Kerning = (FLOAT32)Font->getKerningWidth(&Text[i], tp);
			info.firstInd = firstInd;
			info.firstVert = firstVert;

			Symbol.push_back(info);
		}
	}


	//! pre render event
	void CBillboardTextSceneNode::OnAnimate(UINT32 timeMs)
	{
		ISceneNode::OnAnimate(timeMs);

		if (!IsVisible || !Font || !Mesh)
			return;

		ICameraSceneNode* camera = SceneManager->getActiveCamera();
		if (!camera)
			return;

		// get text width
		FLOAT32 textLength = 0.f;
		UINT32 i;
		for (i = 0; i != Symbol.size(); ++i)
		{
			SSymbolInfo &info = Symbol[i];
			textLength += info.Kerning + info.Width;
		}
		if (textLength<0.0f)
			textLength = 1.0f;

		//const Matrix4 &m = camera->getViewFrustum()->Matrices[ ETS_VIEW ];

		// make billboard look to camera
		Vector3 pos = getAbsolutePosition();

		Vector3 campos = camera->getAbsolutePosition();
		Vector3 target = camera->getTarget();
		Vector3 up = camera->getUpVector();
		Vector3 view = target - campos;
		view.normalize();

		Vector3 horizontal = up.crossProduct(view);
		if (horizontal.length() == 0)
		{
			horizontal.set(up.y, up.x, up.z);
		}

		horizontal.normalize();
		Vector3 space = horizontal;

		horizontal *= 0.5f * Size.Width;

		Vector3 vertical = horizontal.crossProduct(view);
		vertical.normalize();
		vertical *= 0.5f * Size.Height;

		view *= -1.0f;

		// center text
		pos += space * (Size.Width * -0.5f);

		for (i = 0; i != Symbol.size(); ++i)
		{
			SSymbolInfo &info = Symbol[i];
			FLOAT32 infw = info.Width / textLength;
			FLOAT32 infk = info.Kerning / textLength;
			FLOAT32 w = (Size.Width * infw * 0.5f);
			pos += space * w;

			SMeshBuffer* buf = (SMeshBuffer*)Mesh->getMeshBuffer(info.bufNo);

			buf->Vertices[info.firstVert + 0].Normal = view;
			buf->Vertices[info.firstVert + 1].Normal = view;
			buf->Vertices[info.firstVert + 2].Normal = view;
			buf->Vertices[info.firstVert + 3].Normal = view;

			buf->Vertices[info.firstVert + 0].Pos = pos + (space * w) + vertical;
			buf->Vertices[info.firstVert + 1].Pos = pos + (space * w) - vertical;
			buf->Vertices[info.firstVert + 2].Pos = pos - (space * w) - vertical;
			buf->Vertices[info.firstVert + 3].Pos = pos - (space * w) + vertical;

			pos += space * (Size.Width*infk + w);
		}

		// make bounding box

		for (i = 0; i< Mesh->getMeshBufferCount(); ++i)
			Mesh->getMeshBuffer(i)->recalculateBoundingBox();
		Mesh->recalculateBoundingBox();

		BBox = Mesh->getBoundingBox();
		//Matrix4 mat (getAbsoluteTransformation(), Matrix4::EM4CONST_INVERSE);
		Matrix4 mat = getAbsoluteTransformation();
		mat.inverse();
		//mat.transformBoxEx(BBox);
		BBox.transform(mat);
	}

	void CBillboardTextSceneNode::OnRegisterSceneNode()
	{
		SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
		ISceneNode::OnRegisterSceneNode();
	}


	//! render
	void CBillboardTextSceneNode::render()
	{
		if (!Mesh)
			return;

		IVideoDriver* driver = SceneManager->getVideoDriver();

		// draw
		Matrix4 mat;
		driver->setTransform(ETS_WORLD, mat);

		for (UINT32 i = 0; i < Mesh->getMeshBufferCount(); ++i)
		{
			driver->setMaterial(Mesh->getMeshBuffer(i)->getMaterial());
			driver->drawMeshBuffer(Mesh->getMeshBuffer(i));
		}

		if (DebugDataVisible & EDS_BBOX)
		{
			driver->setTransform(ETS_WORLD, AbsoluteTransformation);
			SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			driver->draw3DBox(BBox, ColourValue(0, 208, 195, 152));
		}
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CBillboardTextSceneNode::getBoundingBox() const
	{
		return BBox;
	}


	//! sets the size of the billboard
	void CBillboardTextSceneNode::setSize(const dimension2df& size)
	{
		Size = size;

		if (Size.Width == 0.0f)
			Size.Width = 1.0f;

		if (Size.Height == 0.0f)
			Size.Height = 1.0f;

		//FLOAT32 avg = (size.Width + size.Height)/6;
		//BBox.MinEdge.set(-avg,-avg,-avg);
		//BBox.MaxEdge.set(avg,avg,avg);
	}


	SMaterial& CBillboardTextSceneNode::getMaterial(UINT32 i)
	{
		if (Mesh && Mesh->getMeshBufferCount() > i)
			return Mesh->getMeshBuffer(i)->getMaterial();
		else
			return Material;
	}


	//! returns amount of materials used by this scene node.
	UINT32 CBillboardTextSceneNode::getMaterialCount() const
	{
		if (Mesh)
			return Mesh->getMeshBufferCount();
		else
			return 0;
	}


	//! gets the size of the billboard
	const dimension2df& CBillboardTextSceneNode::getSize() const
	{
		return Size;
	}


	//! sets the color of the text
	void CBillboardTextSceneNode::setTextColor(ColourValue color)
	{
		Color = color;
	}

	//! Set the color of all vertices of the billboard
	//! \param overallColor: the color to set
	void CBillboardTextSceneNode::setColor(const ColourValue & overallColor)
	{
		if (!Mesh)
			return;

		for (UINT32 i = 0; i != Text.size(); ++i)
		{
			const SSymbolInfo &info = Symbol[i];
			SMeshBuffer* buf = (SMeshBuffer*)Mesh->getMeshBuffer(info.bufNo);
			buf->Vertices[info.firstVert + 0].Color = overallColor;
			buf->Vertices[info.firstVert + 1].Color = overallColor;
			buf->Vertices[info.firstVert + 2].Color = overallColor;
			buf->Vertices[info.firstVert + 3].Color = overallColor;
		}
	}


	//! Set the color of the top and bottom vertices of the billboard
	//! \param topColor: the color to set the top vertices
	//! \param bottomColor: the color to set the bottom vertices
	void CBillboardTextSceneNode::setColor(const ColourValue & topColor, const ColourValue & bottomColor)
	{
		if (!Mesh)
			return;

		ColorBottom = bottomColor;
		ColorTop = topColor;
		for (UINT32 i = 0; i != Text.size(); ++i)
		{
			const SSymbolInfo &info = Symbol[i];
			SMeshBuffer* buf = (SMeshBuffer*)Mesh->getMeshBuffer(info.bufNo);
			buf->Vertices[info.firstVert + 0].Color = ColorBottom;
			buf->Vertices[info.firstVert + 3].Color = ColorBottom;
			buf->Vertices[info.firstVert + 1].Color = ColorTop;
			buf->Vertices[info.firstVert + 2].Color = ColorTop;
		}
	}


	//! Gets the color of the top and bottom vertices of the billboard
	//! \param topColor: stores the color of the top vertices
	//! \param bottomColor: stores the color of the bottom vertices
	void CBillboardTextSceneNode::getColor(ColourValue & topColor, ColourValue & bottomColor) const
	{
		topColor = ColorTop;
		bottomColor = ColorBottom;
	}

}