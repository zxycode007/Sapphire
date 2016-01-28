#include "SapphireCGUIMeshViewer.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireIMesh.h"
#include "SapphireOS.h"
#include "SapphireIGUISkin.h"

namespace Sapphire
{
	//! constructor
	CGUIMeshViewer::CGUIMeshViewer(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
		: IGUIMeshViewer(environment, parent, id, rectangle), Mesh(0)
	{
#ifdef _DEBUG
		setDebugName("CGUIMeshViewer");
#endif
	}


	//! destructor
	CGUIMeshViewer::~CGUIMeshViewer()
	{
		if (Mesh)
			Mesh->drop();
	}


	//! sets the mesh to be shown
	void CGUIMeshViewer::setMesh(IAnimatedMesh* mesh)
	{
		if (mesh)
			mesh->grab();
		if (Mesh)
			Mesh->drop();

		Mesh = mesh;

		/* This might be used for proper transformation etc.
		vector3df center(0.0f,0.0f,0.0f);
		aabbox3d<f32> box;

		box = Mesh->getMesh(0)->getBoundingBox();
		center = (box.MaxEdge + box.MinEdge) / 2;
		*/
	}


	//! Gets the displayed mesh
	IAnimatedMesh* CGUIMeshViewer::getMesh() const
	{
		return Mesh;
	}


	//! sets the material
	void CGUIMeshViewer::setMaterial(const SMaterial& material)
	{
		Material = material;
	}


	//! gets the material
	const SMaterial& CGUIMeshViewer::getMaterial() const
	{
		return Material;
	}


	//! called if an event happened.
	bool CGUIMeshViewer::OnEvent(const SEvent& event)
	{
		return IGUIElement::OnEvent(event);
	}


	//! draws the element and its children
	void CGUIMeshViewer::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		IVideoDriver* driver = Environment->getVideoDriver();
		rect<SINT32> viewPort = AbsoluteRect;
		viewPort.LowerRightCorner.x -= 1;
		viewPort.LowerRightCorner.y -= 1;
		viewPort.UpperLeftCorner.x += 1;
		viewPort.UpperLeftCorner.y += 1;

		viewPort.clipAgainst(AbsoluteClippingRect);

		// draw the frame

		rect<SINT32> frameRect(AbsoluteRect);
		frameRect.LowerRightCorner.y = frameRect.UpperLeftCorner.y + 1;
		skin->draw2DRectangle(this, skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

		frameRect.LowerRightCorner.y = AbsoluteRect.LowerRightCorner.y;
		frameRect.LowerRightCorner.x = frameRect.UpperLeftCorner.x + 1;
		skin->draw2DRectangle(this, skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.x = frameRect.LowerRightCorner.x - 1;
		skin->draw2DRectangle(this, skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.y = AbsoluteRect.LowerRightCorner.y - 1;
		skin->draw2DRectangle(this, skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

		// draw the mesh

		if (Mesh)
		{
			//TODO: if outside of screen, dont draw.
			// - why is the absolute clipping rect not already the screen?

			rect<SINT32> oldViewPort = driver->getViewPort();

			driver->setViewPort(viewPort);

			Matrix4 mat;

			//CameraControl->calculateProjectionMatrix(mat);
			//driver->setTransform(TS_PROJECTION, mat);

			mat = Matrix4::IDENTITY;
			mat.setTrans(Vector3(0, 0, 0));
			//mat.setTranslation(Vector3(0, 0, 0));
			driver->setTransform(ETS_WORLD, mat);

			//CameraControl->calculateViewMatrix(mat);
			//driver->setTransform(TS_VIEW, mat);

			driver->setMaterial(Material);

			UINT32 frame = 0;
			if (Mesh->getFrameCount())
				frame = (Timer::getTime() / 20) % Mesh->getFrameCount();
			const IMesh* const m = Mesh->getMesh(frame);
			for (UINT32 i = 0; i<m->getMeshBufferCount(); ++i)
			{
				IMeshBuffer* mb = m->getMeshBuffer(i);
				driver->drawVertexPrimitiveList(mb->getVertices(),
					mb->getVertexCount(), mb->getIndices(),
					mb->getIndexCount() / 3, mb->getVertexType(),
					EPT_TRIANGLES, mb->getIndexType());
			}

			driver->setViewPort(oldViewPort);
		}

		IGUIElement::draw();
	}
}

#endif