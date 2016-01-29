#include "SapphireCGUIToolBar.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIGUIButton.h"
#include "SapphireFont.h"
#include "SapphireCGUIButton.h"

namespace Sapphire
{
	//! constructor
	CGUIToolBar::CGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
		:IGUIToolBar(environment, parent, id, rectangle), ButtonX(5)
	{
#ifdef _DEBUG
		setDebugName("CGUIToolBar");
#endif

		// calculate position and find other menubars
		SINT32 y = 0;
		SINT32 parentwidth = 100;

		if (parent)
		{
			parentwidth = Parent->getAbsolutePosition().getWidth();

			const list<IGUIElement*>::type& children = parent->getChildren();
			list<IGUIElement*>::const_iterator it = children.begin();
			for (; it != children.end(); ++it)
			{
				rect<SINT32> r = (*it)->getAbsolutePosition();
				if (r.UpperLeftCorner.x == 0 && r.UpperLeftCorner.y <= y &&
					r.LowerRightCorner.x == parentwidth)
					y = r.LowerRightCorner.y;
			}
		}

		rect<SINT32> rr;
		rr.UpperLeftCorner.x = 0;
		rr.UpperLeftCorner.y = y;
		SINT32 height = Environment->getSkin()->getSize(EGDS_MENU_HEIGHT);

		/*IGUISkin* skin = Environment->getSkin();
		IGUIFont* font = skin->getFont();
		if (font)
		{
		SINT32 t = font->getDimension(L"A").Height + 5;
		if (t > height)
		height = t;
		}*/

		rr.LowerRightCorner.x = parentwidth;
		rr.LowerRightCorner.y = rr.UpperLeftCorner.y + height;
		setRelativePosition(rr);
	}


	//! called if an event happened.
	bool CGUIToolBar::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{
			if (event.EventType == EET_MOUSE_INPUT_EVENT &&
				event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				if (AbsoluteClippingRect.isPointInside(Position2d(event.MouseInput.X, event.MouseInput.Y)))
					return true;
			}
		}

		return IGUIElement::OnEvent(event);
	}


	//! draws the element and its children
	void CGUIToolBar::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return;

		rect<SINT32> rect = AbsoluteRect;
		Sapphire::rect<SINT32>* clip = &AbsoluteClippingRect;

		// draw frame
		skin->draw3DToolBar(this, rect, clip);

		IGUIElement::draw();
	}


	//! Updates the absolute position.
	void CGUIToolBar::updateAbsolutePosition()
	{
		if (Parent)
		{
			DesiredRect.UpperLeftCorner.x = 0;
			DesiredRect.LowerRightCorner.x = Parent->getAbsolutePosition().getWidth();
		}

		IGUIElement::updateAbsolutePosition();
	}


	//! Adds a button to the tool bar
	IGUIButton* CGUIToolBar::addButton(SINT32 id, const wchar_t* text, const wchar_t* tooltiptext,
		ITexture* img, ITexture* pressed, bool isPushButton,
		bool useAlphaChannel)
	{
		ButtonX += 3;

		rect<SINT32> rectangle(ButtonX, 2, ButtonX + 1, 3);
		if (img)
		{
			const dimension2du &size = img->getOriginalSize();
			rectangle.LowerRightCorner.x = rectangle.UpperLeftCorner.x + size.Width + 8;
			rectangle.LowerRightCorner.y = rectangle.UpperLeftCorner.y + size.Height + 6;
		}

		if (text)
		{
			IGUISkin* skin = Environment->getSkin();
			IGUIFont * font = skin->getFont(EGDF_BUTTON);
			if (font)
			{
				dimension2d<UINT32> dim = font->getDimension(text);
				if ((SINT32)dim.Width > rectangle.getWidth())
					rectangle.LowerRightCorner.x = rectangle.UpperLeftCorner.x + dim.Width + 8;
				if ((SINT32)dim.Height > rectangle.getHeight())
					rectangle.LowerRightCorner.y = rectangle.UpperLeftCorner.y + dim.Height + 6;
			}
		}

		ButtonX += rectangle.getWidth();

		IGUIButton* button = new CGUIButton(Environment, this, id, rectangle);
		button->drop();

		if (text)
			button->setText(text);

		if (tooltiptext)
			button->setToolTipText(tooltiptext);

		if (img)
			button->setImage(img);

		if (pressed)
			button->setPressedImage(pressed);

		if (isPushButton)
			button->setIsPushButton(isPushButton);

		if (useAlphaChannel)
			button->setUseAlphaChannel(useAlphaChannel);

		return button;
	}
}

#endif