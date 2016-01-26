#include "SapphireCGUIComboBox.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireFont.h"
#include "SapphireIGUIButton.h"
#include "SapphireCGUIListBox.h"
#include "SapphireOS.h"

namespace Sapphire
{
	//! constructor
	CGUIComboBox::CGUIComboBox(IGUIEnvironment* environment, IGUIElement* parent,
		SINT32 id, rect<SINT32> rectangle)
		: IGUIComboBox(environment, parent, id, rectangle),
		ListButton(0), SelectedText(0), ListBox(0), LastFocus(0),
		Selected(-1), HAlign(EGUIA_UPPERLEFT), VAlign(EGUIA_CENTER), MaxSelectionRows(5), HasFocus(false)
	{
#ifdef _DEBUG
		setDebugName("CGUIComboBox");
#endif

		IGUISkin* skin = Environment->getSkin();

		SINT32 width = 15;
		if (skin)
			width = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);

		rect<SINT32> r;
		r.UpperLeftCorner.x = rectangle.getWidth() - width - 2;
		r.LowerRightCorner.x = rectangle.getWidth() - 2;

		r.UpperLeftCorner.y = 2;
		r.LowerRightCorner.y = rectangle.getHeight() - 2;

		ListButton = Environment->addButton(r, this, -1, L"");
		if (skin && skin->getSpriteBank())
		{
			ListButton->setSpriteBank(skin->getSpriteBank());
			ListButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_CURSOR_DOWN), skin->getColor(EGDC_WINDOW_SYMBOL));
			ListButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_DOWN), skin->getColor(EGDC_WINDOW_SYMBOL));
		}
		ListButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
		ListButton->setSubElement(true);
		ListButton->setTabStop(false);

		r.UpperLeftCorner.x = 2;
		r.UpperLeftCorner.y = 2;
		r.LowerRightCorner.x = RelativeRect.getWidth() - (ListButton->getAbsolutePosition().getWidth() + 2);
		r.LowerRightCorner.y = RelativeRect.getHeight() - 2;

		SelectedText = Environment->addStaticText(L"", r, false, false, this, -1, false);
		SelectedText->setSubElement(true);
		SelectedText->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
		SelectedText->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
		if (skin)
			SelectedText->setOverrideColor(skin->getColor(EGDC_BUTTON_TEXT));
		SelectedText->enableOverrideColor(true);

		// this element can be tabbed to
		setTabStop(true);
		setTabOrder(-1);
	}


	void CGUIComboBox::setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical)
	{
		HAlign = horizontal;
		VAlign = vertical;
		SelectedText->setTextAlignment(horizontal, vertical);
	}


	//! Set the maximal number of rows for the selection listbox
	void CGUIComboBox::setMaxSelectionRows(UINT32 max)
	{
		MaxSelectionRows = max;

		// force recalculation of open listbox
		if (ListBox)
		{
			openCloseMenu();
			openCloseMenu();
		}
	}

	//! Get the maximimal number of rows for the selection listbox
	UINT32 CGUIComboBox::getMaxSelectionRows() const
	{
		return MaxSelectionRows;
	}


	//! Returns amount of items in box
	UINT32 CGUIComboBox::getItemCount() const
	{
		return Items.size();
	}


	//! returns string of an item. the idx may be a value from 0 to itemCount-1
	const wchar_t* CGUIComboBox::getItem(UINT32 idx) const
	{
		if (idx >= Items.size())
			return 0;

		return Items[idx].Name.c_str();
	}

	//! returns string of an item. the idx may be a value from 0 to itemCount-1
	UINT32 CGUIComboBox::getItemData(UINT32 idx) const
	{
		if (idx >= Items.size())
			return 0;

		return Items[idx].Data;
	}

	//! Returns index based on item data
	SINT32 CGUIComboBox::getIndexForItemData(UINT32 data) const
	{
		for (UINT32 i = 0; i < Items.size(); ++i)
		{
			if (Items[i].Data == data)
				return i;
		}
		return -1;
	}


	//! Removes an item from the combo box.
	void CGUIComboBox::removeItem(UINT32 idx)
	{
		if (idx >= Items.size())
			return;

		if (Selected == (SINT32)idx)
			setSelected(-1);

		Items.erase(Items.begin() + idx);
	}


	//! Returns caption of this element.
	const wchar_t* CGUIComboBox::getText() const
	{
		return getItem(Selected);
	}


	//! adds an item and returns the index of it
	UINT32 CGUIComboBox::addItem(const wchar_t* text, UINT32 data)
	{
		Items.push_back(SComboData(text, data));

		if (Selected == -1)
			setSelected(0);

		return Items.size() - 1;
	}


	//! deletes all items in the combo box
	void CGUIComboBox::clear()
	{
		Items.clear();
		setSelected(-1);
	}


	//! returns id of selected item. returns -1 if no item is selected.
	SINT32 CGUIComboBox::getSelected() const
	{
		return Selected;
	}


	//! sets the selected item. Set this to -1 if no item should be selected
	void CGUIComboBox::setSelected(SINT32 idx)
	{
		if (idx < -1 || idx >= (SINT32)Items.size())
			return;

		Selected = idx;
		if (Selected == -1)
			SelectedText->setText(L"");
		else
			SelectedText->setText(Items[Selected].Name.c_str());
	}


	//! called if an event happened.
	bool CGUIComboBox::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{
			switch (event.EventType)
			{

			case EET_KEY_INPUT_EVENT:
				if (ListBox && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
				{
					// hide list box
					openCloseMenu();
					return true;
				}
				else
					if (event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE)
					{
						if (!event.KeyInput.PressedDown)
						{
							openCloseMenu();
						}

						ListButton->setPressed(ListBox == 0);

						return true;
					}
					else
						if (event.KeyInput.PressedDown)
						{
							SINT32 oldSelected = Selected;
							bool absorb = true;
							switch (event.KeyInput.Key)
							{
							case KEY_DOWN:
								setSelected(Selected + 1);
								break;
							case KEY_UP:
								setSelected(Selected - 1);
								break;
							case KEY_HOME:
							case KEY_PRIOR:
								setSelected(0);
								break;
							case KEY_END:
							case KEY_NEXT:
								setSelected((SINT32)Items.size() - 1);
								break;
							default:
								absorb = false;
							}

							if (Selected <0)
								setSelected(0);

							if (Selected >= (SINT32)Items.size())
								setSelected((SINT32)Items.size() - 1);

							if (Selected != oldSelected)
							{
								sendSelectionChangedEvent();
								return true;
							}

							if (absorb)
								return true;
						}
				break;

			case EET_GUI_EVENT:

				switch (event.GUIEvent.EventType)
				{
				case EGET_ELEMENT_FOCUS_LOST:
					if (ListBox &&
						(Environment->hasFocus(ListBox) || ListBox->isMyChild(event.GUIEvent.Caller)) &&
						event.GUIEvent.Element != this &&
						!isMyChild(event.GUIEvent.Element) &&
						!ListBox->isMyChild(event.GUIEvent.Element))
					{
						openCloseMenu();
					}
					break;
				case EGET_BUTTON_CLICKED:
					if (event.GUIEvent.Caller == ListButton)
					{
						openCloseMenu();
						return true;
					}
					break;
				case EGET_LISTBOX_SELECTED_AGAIN:
				case EGET_LISTBOX_CHANGED:
					if (event.GUIEvent.Caller == ListBox)
					{
						setSelected(ListBox->getSelected());
						if (Selected <0 || Selected >= (SINT32)Items.size())
							setSelected(-1);
						openCloseMenu();

						sendSelectionChangedEvent();
					}
					return true;
				default:
					break;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:

				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					Position2d p(event.MouseInput.X, event.MouseInput.Y);

					// send to list box
					if (ListBox && ListBox->isPointInside(p) && ListBox->OnEvent(event))
						return true;

					return true;
				}
				case EMIE_LMOUSE_LEFT_UP:
				{
					Position2d p(event.MouseInput.X, event.MouseInput.Y);

					// send to list box
					if (!(ListBox &&
						ListBox->getAbsolutePosition().isPointInside(p) &&
						ListBox->OnEvent(event)))
					{
						openCloseMenu();
					}
					return true;
				}
				case EMIE_MOUSE_WHEEL:
				{
					SINT32 oldSelected = Selected;
					setSelected(Selected + ((event.MouseInput.Wheel < 0) ? 1 : -1));

					if (Selected <0)
						setSelected(0);

					if (Selected >= (SINT32)Items.size())
						setSelected((SINT32)Items.size() - 1);

					if (Selected != oldSelected)
					{
						sendSelectionChangedEvent();
						return true;
					}
				}
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


	void CGUIComboBox::sendSelectionChangedEvent()
	{
		if (Parent)
		{
			SEvent event;

			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = this;
			event.GUIEvent.Element = 0;
			event.GUIEvent.EventType = EGET_COMBO_BOX_CHANGED;
			Parent->OnEvent(event);
		}
	}


	//! draws the element and its children
	void CGUIComboBox::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		IGUIElement *currentFocus = Environment->getFocus();
		if (currentFocus != LastFocus)
		{
			HasFocus = currentFocus == this || isMyChild(currentFocus);
			LastFocus = currentFocus;
		}

		// set colors each time as skin-colors can be changed
		SelectedText->setBackgroundColor(skin->getColor(EGDC_HIGH_LIGHT));
		if (isEnabled())
		{
			SelectedText->setDrawBackground(HasFocus);
			SelectedText->setOverrideColor(skin->getColor(HasFocus ? EGDC_HIGH_LIGHT_TEXT : EGDC_BUTTON_TEXT));
		}
		else
		{
			SelectedText->setDrawBackground(false);
			SelectedText->setOverrideColor(skin->getColor(EGDC_GRAY_TEXT));
		}
		ListButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_CURSOR_DOWN), skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL));
		ListButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_DOWN), skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL));


		rect<SINT32> frameRect(AbsoluteRect);

		// draw the border

		skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT),
			true, true, frameRect, &AbsoluteClippingRect);

		// draw children
		IGUIElement::draw();
	}


	void CGUIComboBox::openCloseMenu()
	{
		if (ListBox)
		{
			// close list box
			Environment->setFocus(this);
			ListBox->remove();
			ListBox = 0;
		}
		else
		{
			if (Parent)
				Parent->bringToFront(this);

			IGUISkin* skin = Environment->getSkin();
			UINT32 h = Items.size();

			if (h > getMaxSelectionRows())
				h = getMaxSelectionRows();
			if (h == 0)
				h = 1;

			IGUIFont* font = skin->getFont();
			if (font)
				h *= (font->getDimension(L"A").Height + 4);

			// open list box
			rect<SINT32> r(0, AbsoluteRect.getHeight(),
				AbsoluteRect.getWidth(), AbsoluteRect.getHeight() + h);

			ListBox = new CGUIListBox(Environment, this, -1, r, false, true, true);
			ListBox->setSubElement(true);
			ListBox->setNotClipped(true);
			ListBox->drop();

			// ensure that list box is always completely visible
			if (ListBox->getAbsolutePosition().LowerRightCorner.y > Environment->getRootGUIElement()->getAbsolutePosition().getHeight())
				ListBox->setRelativePosition(rect<SINT32>(0, -ListBox->getAbsolutePosition().getHeight(), AbsoluteRect.getWidth(), 0));

			for (SINT32 i = 0; i<(SINT32)Items.size(); ++i)
				ListBox->addItem(Items[i].Name.c_str());

			ListBox->setSelected(Selected);

			// set focus
			Environment->setFocus(ListBox);
		}
	}


	//! Writes attributes of the element.
	void CGUIComboBox::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUIComboBox::serializeAttributes(out, options);

		out->addEnum("HTextAlign", HAlign, GUIAlignmentNames);
		out->addEnum("VTextAlign", VAlign, GUIAlignmentNames);
		out->addInt("MaxSelectionRows", (SINT32)MaxSelectionRows);

		out->addInt("Selected", Selected);
		out->addInt("ItemCount", Items.size());
		for (UINT32 i = 0; i < Items.size(); ++i)
		{
			String s = "Item";
			s += i;
			s += "Text";
			out->addString(s.c_str(), Items[i].Name.c_str());
		}
	}


	//! Reads attributes of the element
	void CGUIComboBox::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		IGUIComboBox::deserializeAttributes(in, options);

		setTextAlignment((EGUI_ALIGNMENT)in->getAttributeAsEnumeration("HTextAlign", GUIAlignmentNames),
			(EGUI_ALIGNMENT)in->getAttributeAsEnumeration("VTextAlign", GUIAlignmentNames));
		setMaxSelectionRows((UINT32)(in->getAttributeAsInt("MaxSelectionRows")));

		// clear the list
		clear();
		// get item count
		UINT32 c = in->getAttributeAsInt("ItemCount");
		// add items
		for (UINT32 i = 0; i < c; ++i)
		{
			String s = "Item";
			s += i;
			s += "Text";
			addItem(in->getAttributeAsStringW(s.c_str()).c_str(), 0);
		}

		setSelected(in->getAttributeAsInt("Selected"));
	}
}

#endif