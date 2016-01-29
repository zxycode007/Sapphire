#include "SapphireCGUIMenu.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireFont.h"
#include "SapphireIGUIWindow.h"

#include "SapphireOS.h"
namespace Sapphire
{
	//! constructor
	CGUIMenu::CGUIMenu(IGUIEnvironment* environment, IGUIElement* parent,
		SINT32 id, rect<SINT32> rectangle)
		: CGUIContextMenu(environment, parent, id, rectangle, false, true)
	{
#ifdef _DEBUG
		setDebugName("CGUIMenu");
#endif

		Type = EGUIET_MENU;

		setNotClipped(false);

		recalculateSize();
	}


	//! draws the element and its children
	void CGUIMenu::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		IGUIFont* font = skin->getFont(EGDF_MENU);

		if (font != LastFont)
		{
			if (LastFont)
				LastFont->drop();
			LastFont = font;
			if (LastFont)
				LastFont->grab();

			recalculateSize();
		}

		rect<SINT32> rect = AbsoluteRect;

		// draw frame

		skin->draw3DToolBar(this, rect, &AbsoluteClippingRect);

		// loop through all menu items

		rect = AbsoluteRect;

		for (SINT32 i = 0; i<(SINT32)Items.size(); ++i)
		{
			if (!Items[i].IsSeparator)
			{
				rect = getRect(Items[i], AbsoluteRect);

				// draw highlighted
				if (i == HighLighted && Items[i].Enabled)
				{
					skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_DARK_SHADOW),
						true, true, rect, &AbsoluteClippingRect);
				}
				// draw text

				EGUI_DEFAULT_COLOR c = EGDC_BUTTON_TEXT;

				if (i == HighLighted)
					c = EGDC_HIGH_LIGHT_TEXT;

				if (!Items[i].Enabled)
					c = EGDC_GRAY_TEXT;

				if (font)
					font->draw(Items[i].Text, rect,
					skin->getColor(c), true, true, &AbsoluteClippingRect);
			}
		}

		IGUIElement::draw();
	}


	//! called if an event happened.
	bool CGUIMenu::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{

			switch (event.EventType)
			{
			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_ELEMENT_FOCUS_LOST:
					if (event.GUIEvent.Caller == this && !isMyChild(event.GUIEvent.Element))
					{
						closeAllSubMenus();
						HighLighted = -1;
					}
					break;
				case EGET_ELEMENT_FOCUSED:
					if (event.GUIEvent.Caller == this && Parent)
					{
						Parent->bringToFront(this);
					}
					break;
				default:
					break;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					if (!Environment->hasFocus(this))
					{
						Environment->setFocus(this);
					}

					if (Parent)
						Parent->bringToFront(this);

					Position2d p(event.MouseInput.X, event.MouseInput.Y);
					bool shouldCloseSubMenu = hasOpenSubMenu();
					if (!AbsoluteClippingRect.isPointInside(p))
					{
						shouldCloseSubMenu = false;
					}
					highlight(Position2d(event.MouseInput.X, event.MouseInput.Y), true);
					if (shouldCloseSubMenu)
					{
						Environment->removeFocus(this);
					}

					return true;
				}
				case EMIE_LMOUSE_LEFT_UP:
				{
					Position2d p(event.MouseInput.X, event.MouseInput.Y);
					if (!AbsoluteClippingRect.isPointInside(p))
					{
						SINT32 t = sendClick(p);
						if ((t == 0 || t == 1) && Environment->hasFocus(this))
							Environment->removeFocus(this);
					}

					return true;
				}
				case EMIE_MOUSE_MOVED:
					if (Environment->hasFocus(this) && HighLighted >= 0)
					{
						SINT32 oldHighLighted = HighLighted;
						highlight(Position2d(event.MouseInput.X, event.MouseInput.Y), true);
						if (HighLighted < 0)
							HighLighted = oldHighLighted;   // keep last hightlight active when moving outside the area
					}
					return true;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		return IGUIElement::OnEvent(event);
	}

	void CGUIMenu::recalculateSize()
	{
		rect<SINT32> clientRect; // client rect of parent
		if (Parent && Parent->hasType(EGUIET_WINDOW))
		{
			clientRect = static_cast<IGUIWindow*>(Parent)->getClientRect();
		}
		else if (Parent)
		{
			clientRect = rect<SINT32>(0, 0, Parent->getAbsolutePosition().getWidth(),
				Parent->getAbsolutePosition().getHeight());
		}
		else
		{
			clientRect = RelativeRect;
		}


		IGUISkin* skin = Environment->getSkin();
		IGUIFont* font = skin->getFont(EGDF_MENU);

		if (!font)
		{
			if (Parent && skin)
				RelativeRect = rect<SINT32>(clientRect.UpperLeftCorner.x, clientRect.UpperLeftCorner.y,
				clientRect.LowerRightCorner.x, clientRect.UpperLeftCorner.y + skin->getSize(EGDS_MENU_HEIGHT));
			return;
		}

		rect<SINT32> rect;
		rect.UpperLeftCorner = clientRect.UpperLeftCorner;
		SINT32 height = font->getDimension(L"A").Height + 5;
		//if (skin && height < skin->getSize ( EGDS_MENU_HEIGHT ))
		//	height = skin->getSize(EGDS_MENU_HEIGHT);
		SINT32 width = rect.UpperLeftCorner.x;
		SINT32 i;

		for (i = 0; i<(SINT32)Items.size(); ++i)
		{
			if (Items[i].IsSeparator)
			{
				Items[i].Dim.Width = 0;
				Items[i].Dim.Height = height;
			}
			else
			{
				Items[i].Dim = font->getDimension(Items[i].Text.c_str());
				Items[i].Dim.Width += 20;
			}

			Items[i].PosY = width;
			width += Items[i].Dim.Width;
		}

		width = clientRect.getWidth();

		rect.LowerRightCorner.x = rect.UpperLeftCorner.x + width;
		rect.LowerRightCorner.y = rect.UpperLeftCorner.y + height;

		setRelativePosition(rect);

		// recalculate submenus
		for (i = 0; i<(SINT32)Items.size(); ++i)
			if (Items[i].SubMenu)
			{
				// move submenu
				SINT32 w = Items[i].SubMenu->getAbsolutePosition().getWidth();
				SINT32 h = Items[i].SubMenu->getAbsolutePosition().getHeight();

				Items[i].SubMenu->setRelativePosition(
					Sapphire::rect<SINT32>(Items[i].PosY, height,
					Items[i].PosY + w - 5, height + h));
			}
	}


	//! returns the item highlight-area
	rect<SINT32> CGUIMenu::getHRect(const SItem& i, const rect<SINT32>& absolute) const
	{
		rect<SINT32> r = absolute;
		r.UpperLeftCorner.x += i.PosY;
		r.LowerRightCorner.x = r.UpperLeftCorner.x + i.Dim.Width;
		return r;
	}


	//! Gets drawing rect of Item
	rect<SINT32> CGUIMenu::getRect(const SItem& i, const rect<SINT32>& absolute) const
	{
		return getHRect(i, absolute);
	}


	void CGUIMenu::updateAbsolutePosition()
	{
		if (Parent)
			DesiredRect.LowerRightCorner.x = Parent->getAbsolutePosition().getWidth();

		IGUIElement::updateAbsolutePosition();
	}
}

#endif