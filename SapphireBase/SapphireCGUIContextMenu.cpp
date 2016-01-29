#include "SapphireCGUIContextMenu.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireFont.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireOs.h"

namespace Sapphire
{

	//! constructor
	CGUIContextMenu::CGUIContextMenu(IGUIEnvironment* environment,
		IGUIElement* parent, SINT32 id,
		rect<SINT32> rectangle, bool getFocus, bool allowFocus)
		: IGUIContextMenu(environment, parent, id, rectangle), EventParent(0), LastFont(0),
		CloseHandling(ECMC_REMOVE), HighLighted(-1), ChangeTime(0), AllowFocus(allowFocus)
	{
#ifdef _DEBUG
		setDebugName("CGUIContextMenu");
#endif

		Pos = rectangle.UpperLeftCorner;
		recalculateSize();

		if (getFocus)
			Environment->setFocus(this);

		setNotClipped(true);
	}


	//! destructor
	CGUIContextMenu::~CGUIContextMenu()
	{
		for (UINT32 i = 0; i<Items.size(); ++i)
			if (Items[i].SubMenu)
				Items[i].SubMenu->drop();

		if (LastFont)
			LastFont->drop();
	}

	//! set behavior when menus are closed
	void CGUIContextMenu::setCloseHandling(ECONTEXT_MENU_CLOSE onClose)
	{
		CloseHandling = onClose;
	}

	//! get current behavior when the menue will be closed
	ECONTEXT_MENU_CLOSE CGUIContextMenu::getCloseHandling() const
	{
		return CloseHandling;
	}

	//! Returns amount of menu items
	UINT32 CGUIContextMenu::getItemCount() const
	{
		return Items.size();
	}


	//! Adds a menu item.
	UINT32 CGUIContextMenu::addItem(const wchar_t* text, SINT32 commandId, bool enabled, bool hasSubMenu, bool checked, bool autoChecking)
	{
		return insertItem(Items.size(), text, commandId, enabled, hasSubMenu, checked, autoChecking);
	}

	//! Insert a menu item at specified position.
	UINT32 CGUIContextMenu::insertItem(UINT32 idx, const wchar_t* text, SINT32 commandId, bool enabled,
		bool hasSubMenu, bool checked, bool autoChecking)
	{
		SItem s;
		s.Enabled = enabled;
		s.Checked = checked;
		s.AutoChecking = autoChecking;
		s.Text = text;
		s.IsSeparator = (text == 0);
		s.SubMenu = 0;
		s.CommandId = commandId;

		if (hasSubMenu)
		{
			s.SubMenu = new CGUIContextMenu(Environment, this, commandId,
				rect<SINT32>(0, 0, 100, 100), false, false);
			s.SubMenu->setVisible(false);
		}

		UINT32 result = idx;
		if (idx < Items.size())
		{
			Items.insert(Items.begin()+idx, s);
		}
		else
		{
			Items.push_back(s);
			result = Items.size() - 1;
		}

		recalculateSize();
		return result;
	}

	SINT32 CGUIContextMenu::findItemWithCommandId(SINT32 commandId, UINT32 idxStartSearch) const
	{
		for (UINT32 i = idxStartSearch; i<Items.size(); ++i)
		{
			if (Items[i].CommandId == commandId)
			{
				return (SINT32)i;
			}
		}
		return -1;
	}

	//! Adds a sub menu from an element that already exists.
	void CGUIContextMenu::setSubMenu(UINT32 index, CGUIContextMenu* menu)
	{
		if (index >= Items.size())
			return;

		if (menu)
			menu->grab();
		if (Items[index].SubMenu)
			Items[index].SubMenu->drop();

		Items[index].SubMenu = menu;
		menu->setVisible(false);

		if (Items[index].SubMenu)
		{
			menu->AllowFocus = false;
			if (Environment->getFocus() == menu)
			{
				Environment->setFocus(this);
			}
		}

		recalculateSize();
	}


	//! Adds a separator item to the menu
	void CGUIContextMenu::addSeparator()
	{
		addItem(0, -1, true, false, false, false);
	}


	//! Returns text of the menu item.
	const wchar_t* CGUIContextMenu::getItemText(UINT32 idx) const
	{
		if (idx >= Items.size())
			return 0;

		return Items[idx].Text.c_str();
	}


	//! Sets text of the menu item.
	void CGUIContextMenu::setItemText(UINT32 idx, const wchar_t* text)
	{
		if (idx >= Items.size())
			return;

		Items[idx].Text = text;
		recalculateSize();
	}

	//! should the element change the checked status on clicking
	void CGUIContextMenu::setItemAutoChecking(UINT32 idx, bool autoChecking)
	{
		if (idx >= Items.size())
			return;

		Items[idx].AutoChecking = autoChecking;
	}

	//! does the element change the checked status on clicking
	bool CGUIContextMenu::getItemAutoChecking(UINT32 idx) const
	{
		if (idx >= Items.size())
			return false;

		return Items[idx].AutoChecking;
	}


	//! Returns if a menu item is enabled
	bool CGUIContextMenu::isItemEnabled(UINT32 idx) const
	{
		if (idx >= Items.size())
		{
			;
			return false;
		}

		;
		return Items[idx].Enabled;
	}


	//! Returns if a menu item is checked
	bool CGUIContextMenu::isItemChecked(UINT32 idx) const
	{
		if (idx >= Items.size())
		{
			;
			return false;
		}

		;
		return Items[idx].Checked;
	}


	//! Sets if the menu item should be enabled.
	void CGUIContextMenu::setItemEnabled(UINT32 idx, bool enabled)
	{
		if (idx >= Items.size())
			return;

		Items[idx].Enabled = enabled;
	}


	//! Sets if the menu item should be checked.
	void CGUIContextMenu::setItemChecked(UINT32 idx, bool checked)
	{
		if (idx >= Items.size())
			return;

		Items[idx].Checked = checked;
	}


	//! Removes a menu item
	void CGUIContextMenu::removeItem(UINT32 idx)
	{
		if (idx >= Items.size())
			return;

		if (Items[idx].SubMenu)
		{
			Items[idx].SubMenu->drop();
			Items[idx].SubMenu = 0;
		}

		Items.erase(Items.begin()+idx);
		recalculateSize();
	}


	//! Removes all menu items
	void CGUIContextMenu::removeAllItems()
	{
		for (UINT32 i = 0; i<Items.size(); ++i)
			if (Items[i].SubMenu)
				Items[i].SubMenu->drop();

		Items.clear();
		recalculateSize();
	}


	//! called if an event happened.
	bool CGUIContextMenu::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{

			switch (event.EventType)
			{
			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_ELEMENT_FOCUS_LOST:
					if (event.GUIEvent.Caller == this && !isMyChild(event.GUIEvent.Element) && AllowFocus)
					{
						// set event parent of submenus
						IGUIElement * p = EventParent ? EventParent : Parent;
						setEventParent(p);

						SEvent event;
						event.EventType = EET_GUI_EVENT;
						event.GUIEvent.Caller = this;
						event.GUIEvent.Element = 0;
						event.GUIEvent.EventType = EGET_ELEMENT_CLOSED;
						if (!p->OnEvent(event))
						{
							if (CloseHandling & ECMC_HIDE)
							{
								setVisible(false);
							}
							if (CloseHandling & ECMC_REMOVE)
							{
								remove();
							}
						}

						return false;
					}
					break;
				case EGET_ELEMENT_FOCUSED:
					if (event.GUIEvent.Caller == this && !AllowFocus)
					{
						return true;
					}
					break;
				default:
					break;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_LEFT_UP:
				{
					// menu might be removed if it loses focus in sendClick, so grab a reference
					grab();
					const UINT32 t = sendClick(Position2d(event.MouseInput.X, event.MouseInput.Y));
					if ((t == 0 || t == 1) && Environment->hasFocus(this))
						Environment->removeFocus(this);
					drop();
				}
				return true;
				case EMIE_LMOUSE_PRESSED_DOWN:
					return true;
				case EMIE_MOUSE_MOVED:
					if (Environment->hasFocus(this))
						highlight(Position2d(event.MouseInput.X, event.MouseInput.Y), true);
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


	//! Sets the visible state of this element.
	void CGUIContextMenu::setVisible(bool visible)
	{
		HighLighted = -1;
		ChangeTime = Timer::getTime();
		for (UINT32 j = 0; j<Items.size(); ++j)
			if (Items[j].SubMenu)
				Items[j].SubMenu->setVisible(false);

		IGUIElement::setVisible(visible);
	}


	//! sends a click Returns:
	//! 0 if click went outside of the element,
	//! 1 if a valid button was clicked,
	//! 2 if a nonclickable element was clicked
	UINT32 CGUIContextMenu::sendClick(const Position2d& p)
	{
		UINT32 t = 0;

		// get number of open submenu
		SINT32 openmenu = -1;
		SINT32 j;
		for (j = 0; j<(SINT32)Items.size(); ++j)
			if (Items[j].SubMenu && Items[j].SubMenu->isVisible())
			{
				openmenu = j;
				break;
			}

		// delegate click operation to submenu
		if (openmenu != -1)
		{
			t = Items[j].SubMenu->sendClick(p);
			if (t != 0)
				return t; // clicked something
		}

		// check click on myself
		if (isPointInside(p) &&
			(UINT32)HighLighted < Items.size())
		{
			if (!Items[HighLighted].Enabled ||
				Items[HighLighted].IsSeparator ||
				Items[HighLighted].SubMenu)
				return 2;

			if (Items[HighLighted].AutoChecking)
			{
				Items[HighLighted].Checked = Items[HighLighted].Checked ? false : true;
			}

			SEvent event;
			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = this;
			event.GUIEvent.Element = 0;
			event.GUIEvent.EventType = EGET_MENU_ITEM_SELECTED;
			if (EventParent)
				EventParent->OnEvent(event);
			else if (Parent)
				Parent->OnEvent(event);

			return 1;
		}

		return 0;
	}


	//! returns true, if an element was highligted
	bool CGUIContextMenu::highlight(const Position2d& p, bool canOpenSubMenu)
	{
		if (!isEnabled())
		{
			return false;
		}

		// get number of open submenu
		SINT32 openmenu = -1;
		SINT32 i;
		for (i = 0; i<(SINT32)Items.size(); ++i)
			if (Items[i].Enabled && Items[i].SubMenu && Items[i].SubMenu->isVisible())
			{
				openmenu = i;
				break;
			}

		// delegate highlight operation to submenu
		if (openmenu != -1)
		{
			if (Items[openmenu].Enabled && Items[openmenu].SubMenu->highlight(p, canOpenSubMenu))
			{
				HighLighted = openmenu;
				ChangeTime = Timer::getTime();
				return true;
			}
		}

		// highlight myself
		for (i = 0; i<(SINT32)Items.size(); ++i)
		{
			if (Items[i].Enabled && getHRect(Items[i], AbsoluteRect).isPointInside(p))
			{
				HighLighted = i;
				ChangeTime = Timer::getTime();

				// make submenus visible/invisible
				for (SINT32 j = 0; j<(SINT32)Items.size(); ++j)
					if (Items[j].SubMenu)
					{
						if (j == i && canOpenSubMenu && Items[j].Enabled)
							Items[j].SubMenu->setVisible(true);
						else if (j != i)
							Items[j].SubMenu->setVisible(false);
					}
				return true;
			}
		}

		HighLighted = openmenu;
		return false;
	}


	//! returns the item highlight-area
	rect<SINT32> CGUIContextMenu::getHRect(const SItem& i, const rect<SINT32>& absolute) const
	{
		rect<SINT32> r = absolute;
		r.UpperLeftCorner.y += i.PosY;
		r.LowerRightCorner.y = r.UpperLeftCorner.y + i.Dim.Height;
		return r;
	}


	//! Gets drawing rect of Item
	rect<SINT32> CGUIContextMenu::getRect(const SItem& i, const rect<SINT32>& absolute) const
	{
		rect<SINT32> r = absolute;
		r.UpperLeftCorner.y += i.PosY;
		r.LowerRightCorner.y = r.UpperLeftCorner.y + i.Dim.Height;
		r.UpperLeftCorner.x += 20;
		return r;
	}


	//! draws the element and its children
	void CGUIContextMenu::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();

		if (!skin)
			return;

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

		IGUISpriteBank* sprites = skin->getSpriteBank();

		Sapphire::rect<SINT32> rect = AbsoluteRect;
		Sapphire::rect<SINT32>* clip = 0;

		// draw frame
		skin->draw3DMenuPane(this, AbsoluteRect, clip);

		// loop through all menu items

		rect = AbsoluteRect;
		SINT32 y = AbsoluteRect.UpperLeftCorner.y;

		for (SINT32 i = 0; i<(SINT32)Items.size(); ++i)
		{
			if (Items[i].IsSeparator)
			{
				// draw separator
				rect = AbsoluteRect;
				rect.UpperLeftCorner.y += Items[i].PosY + 3;
				rect.LowerRightCorner.y = rect.UpperLeftCorner.y + 1;
				rect.UpperLeftCorner.x += 5;
				rect.LowerRightCorner.x -= 5;
				skin->draw2DRectangle(this, skin->getColor(EGDC_3D_SHADOW), rect, clip);

				rect.LowerRightCorner.y += 1;
				rect.UpperLeftCorner.y += 1;
				skin->draw2DRectangle(this, skin->getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

				y += 10;
			}
			else
			{
				rect = getRect(Items[i], AbsoluteRect);

				// draw highlighted

				if (i == HighLighted && Items[i].Enabled)
				{
					Sapphire::rect<SINT32> r = AbsoluteRect;
					r.LowerRightCorner.y = rect.LowerRightCorner.y;
					r.UpperLeftCorner.y = rect.UpperLeftCorner.y;
					r.LowerRightCorner.x -= 5;
					r.UpperLeftCorner.x += 5;
					skin->draw2DRectangle(this, skin->getColor(EGDC_HIGH_LIGHT), r, clip);
				}

				// draw text

				EGUI_DEFAULT_COLOR c = EGDC_BUTTON_TEXT;

				if (i == HighLighted)
					c = EGDC_HIGH_LIGHT_TEXT;

				if (!Items[i].Enabled)
					c = EGDC_GRAY_TEXT;

				if (font)
					font->draw(Items[i].Text, rect,
					skin->getColor(c), false, true, clip);

				// draw submenu symbol
				if (Items[i].SubMenu && sprites)
				{
					Sapphire::rect<SINT32> r = rect;
					r.UpperLeftCorner.x = r.LowerRightCorner.x - 15;

					sprites->draw2DSprite(skin->getIcon(EGDI_CURSOR_RIGHT),
						r.getCenter(), clip, skin->getColor(c),
						(i == HighLighted) ? ChangeTime : 0,
						(i == HighLighted) ? Timer::getTime() : 0,
						(i == HighLighted), true);
				}

				// draw checked symbol
				if (Items[i].Checked && sprites)
				{
					Sapphire::rect<SINT32> r = rect;
					r.LowerRightCorner.x = r.UpperLeftCorner.x - 15;
					r.UpperLeftCorner.x = r.LowerRightCorner.x + 15;
					sprites->draw2DSprite(skin->getIcon(EGDI_CHECK_BOX_CHECKED),
						r.getCenter(), clip, skin->getColor(c),
						(i == HighLighted) ? ChangeTime : 0,
						(i == HighLighted) ? Timer::getTime() : 0,
						(i == HighLighted), true);
				}
			}
		}

		IGUIElement::draw();
	}


	void CGUIContextMenu::recalculateSize()
	{
		IGUIFont* font = Environment->getSkin()->getFont(EGDF_MENU);

		if (!font)
			return;

		Sapphire::rect<SINT32> rect;
		rect.UpperLeftCorner = RelativeRect.UpperLeftCorner;
		UINT32 width = 100;
		UINT32 height = 3;

		UINT32 i;
		for (i = 0; i<Items.size(); ++i)
		{
			if (Items[i].IsSeparator)
			{
				Items[i].Dim.Width = 100;
				Items[i].Dim.Height = 10;
			}
			else
			{
				Items[i].Dim = font->getDimension(Items[i].Text.c_str());
				Items[i].Dim.Width += 40;

				if (Items[i].Dim.Width > width)
					width = Items[i].Dim.Width;
			}

			Items[i].PosY = height;
			height += Items[i].Dim.Height;
		}

		height += 5;

		if (height < 10)
			height = 10;

		rect.LowerRightCorner.x = RelativeRect.UpperLeftCorner.x + width;
		rect.LowerRightCorner.y = RelativeRect.UpperLeftCorner.y + height;

		setRelativePosition(rect);

		// recalculate submenus
		for (i = 0; i<Items.size(); ++i)
		{
			if (Items[i].SubMenu)
			{
				// move submenu
				const SINT32 w = Items[i].SubMenu->getAbsolutePosition().getWidth();
				const SINT32 h = Items[i].SubMenu->getAbsolutePosition().getHeight();

				Sapphire::rect<SINT32> subRect(width - 5, Items[i].PosY, width + w - 5, Items[i].PosY + h);

				// if it would be drawn beyond the right border, then add it to the left side
				IGUIElement * root = Environment->getRootGUIElement();
				if (root)
				{
					Sapphire::rect<SINT32> rectRoot(root->getAbsolutePosition());
					if (getAbsolutePosition().UpperLeftCorner.x + subRect.LowerRightCorner.x > rectRoot.LowerRightCorner.x)
					{
						subRect.UpperLeftCorner.x = -w;
						subRect.LowerRightCorner.x = 0;
					}
				}

				Items[i].SubMenu->setRelativePosition(subRect);
			}
		}
	}


	//! Returns the selected item in the menu
	SINT32 CGUIContextMenu::getSelectedItem() const
	{
		return HighLighted;
	}


	//! \return Returns a pointer to the submenu of an item.
	IGUIContextMenu* CGUIContextMenu::getSubMenu(UINT32 idx) const
	{
		if (idx >= Items.size())
			return 0;

		return Items[idx].SubMenu;
	}


	//! Returns command id of a menu item
	SINT32 CGUIContextMenu::getItemCommandId(UINT32 idx) const
	{
		if (idx >= Items.size())
			return -1;

		return Items[idx].CommandId;
	}


	//! Sets the command id of a menu item
	void CGUIContextMenu::setItemCommandId(UINT32 idx, SINT32 id)
	{
		if (idx >= Items.size())
			return;

		Items[idx].CommandId = id;
	}


	//! Writes attributes of the element.
	void CGUIContextMenu::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUIElement::serializeAttributes(out, options);
		out->addVector2d("Position", Pos);

		if (Parent->getType() == EGUIET_CONTEXT_MENU || Parent->getType() == EGUIET_MENU)
		{
			const IGUIContextMenu* const ptr = (const IGUIContextMenu*)Parent;
			// find the position of this item in its parent's list
			UINT32 i;
			// VC6 needs the cast for this
			for (i = 0; (i<ptr->getItemCount()) && (ptr->getSubMenu(i) != (const IGUIContextMenu*)this); ++i)
				; // do nothing

			out->addInt("ParentItem", i);
		}

		out->addInt("CloseHandling", (SINT32)CloseHandling);

		// write out the item list
		out->addInt("ItemCount", Items.size());

		String tmp;

		for (UINT32 i = 0; i < Items.size(); ++i)
		{
			tmp = "IsSeparator"; tmp += i;
			out->addBool(tmp.c_str(), Items[i].IsSeparator);

			if (!Items[i].IsSeparator)
			{
				tmp = "Text"; tmp += i;
				out->addString(tmp.c_str(), Items[i].Text.c_str());
				tmp = "CommandID"; tmp += i;
				out->addInt(tmp.c_str(), Items[i].CommandId);
				tmp = "Enabled"; tmp += i;
				out->addBool(tmp.c_str(), Items[i].Enabled);
				tmp = "Checked"; tmp += i;
				out->addBool(tmp.c_str(), Items[i].Checked);
				tmp = "AutoChecking"; tmp += i;
				out->addBool(tmp.c_str(), Items[i].AutoChecking);
			}
		}
	}


	//! Reads attributes of the element
	void CGUIContextMenu::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		IGUIElement::deserializeAttributes(in, options);

		Pos = in->getAttributeAsVector2d("Position");

		// link to this item's parent
		if (Parent && (Parent->getType() == EGUIET_CONTEXT_MENU || Parent->getType() == EGUIET_MENU))
			((CGUIContextMenu*)Parent)->setSubMenu(in->getAttributeAsInt("ParentItem"), this);

		CloseHandling = (ECONTEXT_MENU_CLOSE)in->getAttributeAsInt("CloseHandling");

		removeAllItems();

		// read the item list
		const SINT32 count = in->getAttributeAsInt("ItemCount");

		for (SINT32 i = 0; i<count; ++i)
		{
			String tmp;
			StringW txt;
			SINT32 commandid = -1;
			bool enabled = true;
			bool checked = false;
			bool autochecking = false;

			tmp = "IsSeparator"; tmp += i;
			if (in->existsAttribute(tmp.c_str()) && in->getAttributeAsBool(tmp.c_str()))
				addSeparator();
			else
			{
				tmp = "Text"; tmp += i;
				if (in->existsAttribute(tmp.c_str()))
					txt = in->getAttributeAsStringW(tmp.c_str());

				tmp = "CommandID"; tmp += i;
				if (in->existsAttribute(tmp.c_str()))
					commandid = in->getAttributeAsInt(tmp.c_str());

				tmp = "Enabled"; tmp += i;
				if (in->existsAttribute(tmp.c_str()))
					enabled = in->getAttributeAsBool(tmp.c_str());

				tmp = "Checked"; tmp += i;
				if (in->existsAttribute(tmp.c_str()))
					checked = in->getAttributeAsBool(tmp.c_str());

				tmp = "AutoChecking"; tmp += i;
				if (in->existsAttribute(tmp.c_str()))
					autochecking = in->getAttributeAsBool(tmp.c_str());

				addItem(StringW(txt.c_str()).c_str(), commandid, enabled, false, checked, autochecking);
			}
		}

		recalculateSize();
	}


	// because sometimes the element has no parent at click time
	void CGUIContextMenu::setEventParent(IGUIElement *parent)
	{
		EventParent = parent;

		for (UINT32 i = 0; i<Items.size(); ++i)
			if (Items[i].SubMenu)
				Items[i].SubMenu->setEventParent(parent);
	}


	bool CGUIContextMenu::hasOpenSubMenu() const
	{
		for (UINT32 i = 0; i<Items.size(); ++i)
			if (Items[i].SubMenu && Items[i].SubMenu->isVisible())
				return true;

		return false;
	}


	void CGUIContextMenu::closeAllSubMenus()
	{
		for (UINT32 i = 0; i<Items.size(); ++i)
			if (Items[i].SubMenu)
				Items[i].SubMenu->setVisible(false);

		//HighLighted = -1;
	}
}

#endif
