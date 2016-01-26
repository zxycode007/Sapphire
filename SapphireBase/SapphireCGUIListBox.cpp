#include "SapphireCGUIListBox.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireCGUIListBox.h"
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireFont.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireCGUIScrollBar.h"
#include "SapphireOS.h"
namespace Sapphire
{

	//! constructor
	CGUIListBox::CGUIListBox(IGUIEnvironment* environment, IGUIElement* parent,
		SINT32 id, rect<SINT32> rectangle, bool clip,
		bool drawBack, bool moveOverSelect)
		: IGUIListBox(environment, parent, id, rectangle), Selected(-1),
		ItemHeight(0), ItemHeightOverride(0),
		TotalItemHeight(0), ItemsIconWidth(0), Font(0), IconBank(0),
		ScrollBar(0), selectTime(0), LastKeyTime(0), Selecting(false), DrawBack(drawBack),
		MoveOverSelect(moveOverSelect), AutoScroll(true), HighlightWhenNotFocused(true)
	{
#ifdef _DEBUG
		setDebugName("CGUIListBox");
#endif

		IGUISkin* skin = Environment->getSkin();
		const SINT32 s = skin->getSize(EGDS_SCROLLBAR_SIZE);

		ScrollBar = new CGUIScrollBar(false, Environment, this, -1,
			rect<SINT32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()),
			!clip);
		ScrollBar->setSubElement(true);
		ScrollBar->setTabStop(false);
		ScrollBar->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
		ScrollBar->setVisible(false);
		ScrollBar->setPos(0);

		setNotClipped(!clip);

		// this element can be tabbed to
		setTabStop(true);
		setTabOrder(-1);

		updateAbsolutePosition();
	}


	//! destructor
	CGUIListBox::~CGUIListBox()
	{
		if (ScrollBar)
			ScrollBar->drop();

		if (Font)
			Font->drop();

		if (IconBank)
			IconBank->drop();
	}


	//! returns amount of list items
	UINT32 CGUIListBox::getItemCount() const
	{
		return Items.size();
	}


	//! returns string of a list item. the may be a value from 0 to itemCount-1
	const wchar_t* CGUIListBox::getListItem(UINT32 id) const
	{
		if (id >= Items.size())
			return 0;

		return Items[id].text.c_str();
	}


	//! Returns the icon of an item
	SINT32 CGUIListBox::getIcon(UINT32 id) const
	{
		if (id >= Items.size())
			return -1;

		return Items[id].icon;
	}


	//! adds a list item, returns id of item
	UINT32 CGUIListBox::addItem(const wchar_t* text)
	{
		return addItem(text, -1);
	}


	//! adds a list item, returns id of item
	void CGUIListBox::removeItem(UINT32 id)
	{
		if (id >= Items.size())
			return;

		if ((UINT32)Selected == id)
		{
			Selected = -1;
		}
		else if ((UINT32)Selected > id)
		{
			Selected -= 1;
			selectTime = Timer::getTime();
		}

		Items.erase(Items.begin() + id);

		recalculateItemHeight();
	}


	SINT32 CGUIListBox::getItemAt(SINT32 xpos, SINT32 ypos) const
	{
		if (xpos < AbsoluteRect.UpperLeftCorner.x || xpos >= AbsoluteRect.LowerRightCorner.x
			|| ypos < AbsoluteRect.UpperLeftCorner.y || ypos >= AbsoluteRect.LowerRightCorner.y
			)
			return -1;

		if (ItemHeight == 0)
			return -1;

		SINT32 item = ((ypos - AbsoluteRect.UpperLeftCorner.y - 1) + ScrollBar->getPos()) / ItemHeight;
		if (item < 0 || item >= (SINT32)Items.size())
			return -1;

		return item;
	}

	//! clears the list
	void CGUIListBox::clear()
	{
		Items.clear();
		ItemsIconWidth = 0;
		Selected = -1;

		if (ScrollBar)
			ScrollBar->setPos(0);

		recalculateItemHeight();
	}


	void CGUIListBox::recalculateItemHeight()
	{
		IGUISkin* skin = Environment->getSkin();

		if (Font != skin->getFont())
		{
			if (Font)
				Font->drop();

			Font = skin->getFont();
			if (0 == ItemHeightOverride)
				ItemHeight = 0;

			if (Font)
			{
				if (0 == ItemHeightOverride)
					ItemHeight = Font->getDimension(L"A").Height + 4;

				Font->grab();
			}
		}

		TotalItemHeight = ItemHeight * Items.size();
		ScrollBar->setMax(Math::_max(0, TotalItemHeight - AbsoluteRect.getHeight()));
		SINT32 minItemHeight = ItemHeight > 0 ? ItemHeight : 1;
		ScrollBar->setSmallStep(minItemHeight);
		ScrollBar->setLargeStep(2 * minItemHeight);

		if (TotalItemHeight <= AbsoluteRect.getHeight())
			ScrollBar->setVisible(false);
		else
			ScrollBar->setVisible(true);
	}


	//! returns id of selected item. returns -1 if no item is selected.
	SINT32 CGUIListBox::getSelected() const
	{
		return Selected;
	}


	//! sets the selected item. Set this to -1 if no item should be selected
	void CGUIListBox::setSelected(SINT32 id)
	{
		if ((UINT32)id >= Items.size())
			Selected = -1;
		else
			Selected = id;

		selectTime = Timer::getTime();

		recalculateScrollPos();
	}

	//! sets the selected item. Set this to -1 if no item should be selected
	void CGUIListBox::setSelected(const wchar_t *item)
	{
		SINT32 index = -1;

		if (item)
		{
			for (index = 0; index < (SINT32)Items.size(); ++index)
			{
				if (Items[index].text == item)
					break;
			}
		}
		setSelected(index);
	}

	//! called if an event happened.
	bool CGUIListBox::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{
			switch (event.EventType)
			{
			case EET_KEY_INPUT_EVENT:
				if (event.KeyInput.PressedDown &&
					(event.KeyInput.Key == KEY_DOWN ||
					event.KeyInput.Key == KEY_UP ||
					event.KeyInput.Key == KEY_HOME ||
					event.KeyInput.Key == KEY_END ||
					event.KeyInput.Key == KEY_NEXT ||
					event.KeyInput.Key == KEY_PRIOR))
				{
					SINT32 oldSelected = Selected;
					switch (event.KeyInput.Key)
					{
					case KEY_DOWN:
						Selected += 1;
						break;
					case KEY_UP:
						Selected -= 1;
						break;
					case KEY_HOME:
						Selected = 0;
						break;
					case KEY_END:
						Selected = (SINT32)Items.size() - 1;
						break;
					case KEY_NEXT:
						Selected += AbsoluteRect.getHeight() / ItemHeight;
						break;
					case KEY_PRIOR:
						Selected -= AbsoluteRect.getHeight() / ItemHeight;
						break;
					default:
						break;
					}
					if (Selected<0)
						Selected = 0;
					if (Selected >= (SINT32)Items.size())
						Selected = Items.size() - 1;	// will set Selected to -1 for empty listboxes which is correct


					recalculateScrollPos();

					// post the news

					if (oldSelected != Selected && Parent && !Selecting && !MoveOverSelect)
					{
						SEvent e;
						e.EventType = EET_GUI_EVENT;
						e.GUIEvent.Caller = this;
						e.GUIEvent.Element = 0;
						e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
						Parent->OnEvent(e);
					}

					return true;
				}
				else
					if (!event.KeyInput.PressedDown && (event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
					{
						if (Parent)
						{
							SEvent e;
							e.EventType = EET_GUI_EVENT;
							e.GUIEvent.Caller = this;
							e.GUIEvent.Element = 0;
							e.GUIEvent.EventType = EGET_LISTBOX_SELECTED_AGAIN;
							Parent->OnEvent(e);
						}
						return true;
					}
					else if (event.KeyInput.PressedDown && event.KeyInput.Char)
					{
						// change selection based on text as it is typed.
						UINT32 now = Timer::getTime();

						if (now - LastKeyTime < 500)
						{
							// add to key buffer if it isn't a key repeat
							if (!(KeyBuffer.size() == 1 && KeyBuffer[0] == event.KeyInput.Char))
							{
								KeyBuffer += L" ";
								KeyBuffer[KeyBuffer.size() - 1] = event.KeyInput.Char;
							}
						}
						else
						{
							KeyBuffer = L" ";
							KeyBuffer[0] = event.KeyInput.Char;
						}
						LastKeyTime = now;

						// find the selected item, starting at the current selection
						SINT32 start = Selected;
						// dont change selection if the key buffer matches the current item
						if (Selected > -1 && KeyBuffer.size() > 1)
						{
							if (Items[Selected].text.size() >= KeyBuffer.size() &&
								//KeyBuffer.equals_ignore_case(Items[Selected].text.substr(0, KeyBuffer.size())))
								StringUtil::equalsW(KeyBuffer, Items[Selected].text.substr(0, KeyBuffer.size()),false)
								)
								return true;
						}

						SINT32 current;
						for (current = start + 1; current < (SINT32)Items.size(); ++current)
						{
							if (Items[current].text.size() >= KeyBuffer.size())
							{
								if (StringUtil::equalsW(KeyBuffer, Items[current].text.substr(0, KeyBuffer.size()), false))//KeyBuffer.equals_ignore_case(Items[current].text.substr(0, KeyBuffer.size())))
								{
									if (Parent && Selected != current && !Selecting && !MoveOverSelect)
									{
										SEvent e;
										e.EventType = EET_GUI_EVENT;
										e.GUIEvent.Caller = this;
										e.GUIEvent.Element = 0;
										e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
										Parent->OnEvent(e);
									}
									setSelected(current);
									return true;
								}
							}
						}
						for (current = 0; current <= start; ++current)
						{
							if (Items[current].text.size() >= KeyBuffer.size())
							{
								if (StringUtil::equalsW(KeyBuffer, Items[current].text.substr(0, KeyBuffer.size()), false))//KeyBuffer.equals_ignore_case(Items[current].text.substr(0, KeyBuffer.size())))
								{
									if (Parent && Selected != current && !Selecting && !MoveOverSelect)
									{
										Selected = current;
										SEvent e;
										e.EventType = EET_GUI_EVENT;
										e.GUIEvent.Caller = this;
										e.GUIEvent.Element = 0;
										e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
										Parent->OnEvent(e);
									}
									setSelected(current);
									return true;
								}
							}
						}

						return true;
					}
				break;

			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_SCROLL_BAR_CHANGED:
					if (event.GUIEvent.Caller == ScrollBar)
						return true;
					break;
				case EGET_ELEMENT_FOCUS_LOST:
				{
					if (event.GUIEvent.Caller == this)
						Selecting = false;
				}
				default:
					break;
				}
				break;

			case EET_MOUSE_INPUT_EVENT:
			{
				Position2d p(event.MouseInput.X, event.MouseInput.Y);

				switch (event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
					ScrollBar->setPos(ScrollBar->getPos() + (event.MouseInput.Wheel < 0 ? -1 : 1)*-ItemHeight / 2);
					return true;

				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					Selecting = true;
					return true;
				}

				case EMIE_LMOUSE_LEFT_UP:
				{
					Selecting = false;

					if (isPointInside(p))
						selectNew(event.MouseInput.Y);

					return true;
				}

				case EMIE_MOUSE_MOVED:
					if (Selecting || MoveOverSelect)
					{
						if (isPointInside(p))
						{
							selectNew(event.MouseInput.Y, true);
							return true;
						}
					}
				default:
					break;
				}
			}
			break;
			case EET_LOG_TEXT_EVENT:
			case EET_USER_EVENT:
			case EET_JOYSTICK_INPUT_EVENT:
			//case EGUIET_FORCE_32_BIT:
				break;
			}
		}

		return IGUIElement::OnEvent(event);
	}


	void CGUIListBox::selectNew(SINT32 ypos, bool onlyHover)
	{
		UINT32 now = Timer::getTime();
		SINT32 oldSelected = Selected;

		Selected = getItemAt(AbsoluteRect.UpperLeftCorner.x, ypos);
		if (Selected<0 && !Items.empty())
			Selected = 0;

		recalculateScrollPos();

		EGUI_EVENT_TYPE eventType = (Selected == oldSelected && now < selectTime + 500) ? EGET_LISTBOX_SELECTED_AGAIN : EGET_LISTBOX_CHANGED;
		selectTime = now;
		// post the news
		if (Parent && !onlyHover)
		{
			SEvent event;
			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = this;
			event.GUIEvent.Element = 0;
			event.GUIEvent.EventType = eventType;
			Parent->OnEvent(event);
		}
	}


	//! Update the position and size of the listbox, and update the scrollbar
	void CGUIListBox::updateAbsolutePosition()
	{
		IGUIElement::updateAbsolutePosition();

		recalculateItemHeight();
	}


	//! draws the element and its children
	void CGUIListBox::draw()
	{
		if (!IsVisible)
			return;

		recalculateItemHeight(); // if the font changed

		IGUISkin* skin = Environment->getSkin();

		rect<SINT32>* clipRect = 0;

		// draw background
		rect<SINT32> frameRect(AbsoluteRect);

		// draw items

		rect<SINT32> clientClip(AbsoluteRect);
		clientClip.UpperLeftCorner.y += 1;
		clientClip.UpperLeftCorner.x += 1;
		if (ScrollBar->isVisible())
			clientClip.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x - skin->getSize(EGDS_SCROLLBAR_SIZE);
		clientClip.LowerRightCorner.y -= 1;
		clientClip.clipAgainst(AbsoluteClippingRect);

		skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true,
			DrawBack, frameRect, &AbsoluteClippingRect);

		if (clipRect)
			clientClip.clipAgainst(*clipRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.x += 1;
		if (ScrollBar->isVisible())
			frameRect.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x - skin->getSize(EGDS_SCROLLBAR_SIZE);

		frameRect.LowerRightCorner.y = AbsoluteRect.UpperLeftCorner.y + ItemHeight;

		frameRect.UpperLeftCorner.y -= ScrollBar->getPos();
		frameRect.LowerRightCorner.y -= ScrollBar->getPos();

		bool hl = (HighlightWhenNotFocused || Environment->hasFocus(this) || Environment->hasFocus(ScrollBar));

		for (SINT32 i = 0; i<(SINT32)Items.size(); ++i)
		{
			if (frameRect.LowerRightCorner.y >= AbsoluteRect.UpperLeftCorner.y &&
				frameRect.UpperLeftCorner.y <= AbsoluteRect.LowerRightCorner.y)
			{
				if (i == Selected && hl)
					skin->draw2DRectangle(this, skin->getColor(EGDC_HIGH_LIGHT), frameRect, &clientClip);

				rect<SINT32> textRect = frameRect;
				textRect.UpperLeftCorner.x += 3;

				if (Font)
				{
					if (IconBank && (Items[i].icon > -1))
					{
						Position2d iconPos = textRect.UpperLeftCorner;
						iconPos.y += textRect.getHeight() / 2;
						iconPos.x += ItemsIconWidth / 2;

						if (i == Selected && hl)
						{
							IconBank->draw2DSprite((UINT32)Items[i].icon, iconPos, &clientClip,
								hasItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) ?
								getItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) : getItemDefaultColor(EGUI_LBC_ICON_HIGHLIGHT),
								selectTime, Timer::getTime(), false, true);
						}
						else
						{
							IconBank->draw2DSprite((UINT32)Items[i].icon, iconPos, &clientClip,
								hasItemOverrideColor(i, EGUI_LBC_ICON) ? getItemOverrideColor(i, EGUI_LBC_ICON) : getItemDefaultColor(EGUI_LBC_ICON),
								0, (i == Selected) ? Timer::getTime() : 0, false, true);
						}
					}

					textRect.UpperLeftCorner.x += ItemsIconWidth + 3;

					if (i == Selected && hl)
					{
						Font->draw(StringUtil::StringWToString(Items[i].text), textRect,
							hasItemOverrideColor(i, EGUI_LBC_TEXT_HIGHLIGHT) ?
							getItemOverrideColor(i, EGUI_LBC_TEXT_HIGHLIGHT) : getItemDefaultColor(EGUI_LBC_TEXT_HIGHLIGHT),
							false, true, &clientClip);
					}
					else
					{
						Font->draw(StringUtil::StringWToString(Items[i].text), textRect,
							hasItemOverrideColor(i, EGUI_LBC_TEXT) ? getItemOverrideColor(i, EGUI_LBC_TEXT) : getItemDefaultColor(EGUI_LBC_TEXT),
							false, true, &clientClip);
					}

					textRect.UpperLeftCorner.x -= ItemsIconWidth + 3;
				}
			}

			frameRect.UpperLeftCorner.y += ItemHeight;
			frameRect.LowerRightCorner.y += ItemHeight;
		}

		IGUIElement::draw();
	}


	//! adds an list item with an icon
	UINT32 CGUIListBox::addItem(const wchar_t* text, SINT32 icon)
	{
		ListItem i;
		i.text = text;
		i.icon = icon;

		Items.push_back(i);
		recalculateItemHeight();
		recalculateItemWidth(icon);

		return Items.size() - 1;
	}


	void CGUIListBox::setSpriteBank(IGUISpriteBank* bank)
	{
		if (bank == IconBank)
			return;
		if (IconBank)
			IconBank->drop();

		IconBank = bank;
		if (IconBank)
			IconBank->grab();
	}


	void CGUIListBox::recalculateScrollPos()
	{
		if (!AutoScroll)
			return;

		const SINT32 selPos = (Selected == -1 ? TotalItemHeight : Selected * ItemHeight) - ScrollBar->getPos();

		if (selPos < 0)
		{
			ScrollBar->setPos(ScrollBar->getPos() + selPos);
		}
		else
			if (selPos > AbsoluteRect.getHeight() - ItemHeight)
			{
				ScrollBar->setPos(ScrollBar->getPos() + selPos - AbsoluteRect.getHeight() + ItemHeight);
			}
	}


	void CGUIListBox::setAutoScrollEnabled(bool scroll)
	{
		AutoScroll = scroll;
	}


	bool CGUIListBox::isAutoScrollEnabled() const
	{
		
		return AutoScroll;
	}


	bool CGUIListBox::getSerializationLabels(EGUI_LISTBOX_COLOR colorType, String & useColorLabel, String & colorLabel) const
	{
		switch (colorType)
		{
		case EGUI_LBC_TEXT:
			useColorLabel = "UseColText";
			colorLabel = "ColText";
			break;
		case EGUI_LBC_TEXT_HIGHLIGHT:
			useColorLabel = "UseColTextHl";
			colorLabel = "ColTextHl";
			break;
		case EGUI_LBC_ICON:
			useColorLabel = "UseColIcon";
			colorLabel = "ColIcon";
			break;
		case EGUI_LBC_ICON_HIGHLIGHT:
			useColorLabel = "UseColIconHl";
			colorLabel = "ColIconHl";
			break;
		default:
			return false;
		}
		return true;
	}


	//! Writes attributes of the element.
	void CGUIListBox::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUIListBox::serializeAttributes(out, options);

		// todo: out->addString	("IconBank",		IconBank->getName?);
		out->addBool("DrawBack", DrawBack);
		out->addBool("MoveOverSelect", MoveOverSelect);
		out->addBool("AutoScroll", AutoScroll);

		out->addInt("ItemCount", Items.size());
		for (UINT32 i = 0; i<Items.size(); ++i)
		{
			String label("text");
			label += i;
			out->addString(label.c_str(), Items[i].text.c_str());

			for (SINT32 c = 0; c < (SINT32)EGUI_LBC_COUNT; ++c)
			{
				String useColorLabel, colorLabel;
				if (!getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel))
					return;
				label = useColorLabel; label += i;
				if (Items[i].OverrideColors[c].Use)
				{
					out->addBool(label.c_str(), true);
					label = colorLabel; label += i;
					out->addColor(label.c_str(), Items[i].OverrideColors[c].Color);
				}
				else
				{
					out->addBool(label.c_str(), false);
				}
			}
		}
	}


	//! Reads attributes of the element
	void CGUIListBox::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		clear();

		DrawBack = in->getAttributeAsBool("DrawBack");
		MoveOverSelect = in->getAttributeAsBool("MoveOverSelect");
		AutoScroll = in->getAttributeAsBool("AutoScroll");

		IGUIListBox::deserializeAttributes(in, options);

		const SINT32 count = in->getAttributeAsInt("ItemCount");
		for (SINT32 i = 0; i<count; ++i)
		{
			String label("text");
			ListItem item;

			label += i;
			item.text = in->getAttributeAsStringW(label.c_str());

			addItem(item.text.c_str(), item.icon);

			for (UINT32 c = 0; c < EGUI_LBC_COUNT; ++c)
			{
				String useColorLabel, colorLabel;
				if (!getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel))
					return;
				label = useColorLabel; label += i;
				Items[i].OverrideColors[c].Use = in->getAttributeAsBool(label.c_str());
				if (Items[i].OverrideColors[c].Use)
				{
					label = colorLabel; label += i;
					Items[i].OverrideColors[c].Color = in->getAttributeAsColor(label.c_str());
				}
			}
		}
	}


	void CGUIListBox::recalculateItemWidth(SINT32 icon)
	{
		if (IconBank && icon > -1 &&
			IconBank->getSprites().size() > (UINT32)icon &&
			IconBank->getSprites()[(UINT32)icon].Frames.size())
		{
			UINT32 rno = IconBank->getSprites()[(UINT32)icon].Frames[0].rectNumber;
			if (IconBank->getPositions().size() > rno)
			{
				const SINT32 w = IconBank->getPositions()[rno].getWidth();
				if (w > ItemsIconWidth)
					ItemsIconWidth = w;
			}
		}
	}


	void CGUIListBox::setItem(UINT32 index, const wchar_t* text, SINT32 icon)
	{
		if (index >= Items.size())
			return;

		Items[index].text = text;
		Items[index].icon = icon;

		recalculateItemHeight();
		recalculateItemWidth(icon);
	}


	//! Insert the item at the given index
	//! Return the index on success or -1 on failure.
	SINT32 CGUIListBox::insertItem(UINT32 index, const wchar_t* text, SINT32 icon)
	{
		ListItem i;
		i.text = text;
		i.icon = icon;

		Items.insert(Items.begin() + index, i);
		recalculateItemHeight();
		recalculateItemWidth(icon);

		return index;
	}


	void CGUIListBox::swapItems(UINT32 index1, UINT32 index2)
	{
		if (index1 >= Items.size() || index2 >= Items.size())
			return;

		ListItem dummmy = Items[index1];
		Items[index1] = Items[index2];
		Items[index2] = dummmy;
	}


	void CGUIListBox::setItemOverrideColor(UINT32 index, ColourValue color)
	{
		for (UINT32 c = 0; c < EGUI_LBC_COUNT; ++c)
		{
			Items[index].OverrideColors[c].Use = true;
			Items[index].OverrideColors[c].Color = color;
		}
	}


	void CGUIListBox::setItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType, ColourValue color)
	{
		if (index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT)
			return;

		Items[index].OverrideColors[colorType].Use = true;
		Items[index].OverrideColors[colorType].Color = color;
	}


	void CGUIListBox::clearItemOverrideColor(UINT32 index)
	{
		for (UINT32 c = 0; c < (UINT32)EGUI_LBC_COUNT; ++c)
		{
			Items[index].OverrideColors[c].Use = false;
		}
	}


	void CGUIListBox::clearItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType)
	{
		if (index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT)
			return;

		Items[index].OverrideColors[colorType].Use = false;
	}


	bool CGUIListBox::hasItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const
	{
		if (index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT)
			return false;

		return Items[index].OverrideColors[colorType].Use;
	}


	ColourValue CGUIListBox::getItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const
	{
		if ((UINT32)index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT)
			return ColourValue();

		return Items[index].OverrideColors[colorType].Color;
	}


	ColourValue CGUIListBox::getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const
	{
		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return ColourValue();

		switch (colorType)
		{
		case EGUI_LBC_TEXT:
			return skin->getColor(EGDC_BUTTON_TEXT);
		case EGUI_LBC_TEXT_HIGHLIGHT:
			return skin->getColor(EGDC_HIGH_LIGHT_TEXT);
		case EGUI_LBC_ICON:
			return skin->getColor(EGDC_ICON);
		case EGUI_LBC_ICON_HIGHLIGHT:
			return skin->getColor(EGDC_ICON_HIGH_LIGHT);
		default:
			return ColourValue();
		}
	}

	//! set global itemHeight
	void CGUIListBox::setItemHeight(SINT32 height)
	{
		ItemHeight = height;
		ItemHeightOverride = 1;
	}


	//! Sets whether to draw the background
	void CGUIListBox::setDrawBackground(bool draw)
	{
		DrawBack = draw;
	}

}
#endif