#include "SapphireCGUITabControl.h"

#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireCGUIButton.h"
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireFont.h"
#include "SapphireIVideoDriver.h"
#include "SapphireRectangle.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CGUITab::CGUITab(SINT32 number, IGUIEnvironment* environment,
		IGUIElement* parent, const rect<SINT32>& rectangle,
		SINT32 id)
		: IGUITab(environment, parent, id, rectangle), Number(number),
		BackColor(0, 0, 0, 0), OverrideTextColorEnabled(false), TextColor(255, 0, 0, 0),
		DrawBackground(false)
	{
#ifdef _DEBUG
		setDebugName("CGUITab");
#endif

		const IGUISkin* const skin = environment->getSkin();
		if (skin)
			TextColor = skin->getColor(EGDC_BUTTON_TEXT);
	}


	//! Returns number of tab in tabcontrol. Can be accessed
	//! later IGUITabControl::getTab() by this number.
	SINT32 CGUITab::getNumber() const
	{
		return Number;
	}


	//! Sets the number
	void CGUITab::setNumber(SINT32 n)
	{
		Number = n;
	}

	void CGUITab::refreshSkinColors()
	{
		if (!OverrideTextColorEnabled)
		{
			TextColor = Environment->getSkin()->getColor(EGDC_BUTTON_TEXT);
		}
	}

	//! draws the element and its children
	void CGUITab::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin *skin = Environment->getSkin();

		if (skin && DrawBackground)
			skin->draw2DRectangle(this, BackColor, AbsoluteRect, &AbsoluteClippingRect);

		IGUIElement::draw();
	}


	//! sets if the tab should draw its background
	void CGUITab::setDrawBackground(bool draw)
	{
		DrawBackground = draw;
	}


	//! sets the color of the background, if it should be drawn.
	void CGUITab::setBackgroundColor(ColourValue c)
	{
		BackColor = c;
	}


	//! sets the color of the text
	void CGUITab::setTextColor(ColourValue c)
	{
		OverrideTextColorEnabled = true;
		TextColor = c;
	}


	ColourValue CGUITab::getTextColor() const
	{
		return TextColor;
	}


	//! returns true if the tab is drawing its background, false if not
	bool CGUITab::isDrawingBackground() const
	{
		
		return DrawBackground;
	}


	//! returns the color of the background
	ColourValue CGUITab::getBackgroundColor() const
	{
		return BackColor;
	}


	//! Writes attributes of the element.
	void CGUITab::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUITab::serializeAttributes(out, options);

		out->addInt("TabNumber", Number);
		out->addBool("DrawBackground", DrawBackground);
		out->addColor("BackColor", BackColor);
		out->addBool("OverrideTextColorEnabled", OverrideTextColorEnabled);
		out->addColor("TextColor", TextColor);

	}


	//! Reads attributes of the element
	void CGUITab::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		IGUITab::deserializeAttributes(in, options);

		setNumber(in->getAttributeAsInt("TabNumber"));
		setDrawBackground(in->getAttributeAsBool("DrawBackground"));
		setBackgroundColor(in->getAttributeAsColor("BackColor"));
		bool override = in->getAttributeAsBool("OverrideTextColorEnabled");
		setTextColor(in->getAttributeAsColor("TextColor"));
		if (!override)
		{
			OverrideTextColorEnabled = false;
		}

		if (Parent && Parent->getType() == EGUIET_TAB_CONTROL)
		{
			((CGUITabControl*)Parent)->addTab(this);
			if (isVisible())
				((CGUITabControl*)Parent)->setActiveTab(this);
		}
	}


	// ------------------------------------------------------------------
	// Tabcontrol
	// ------------------------------------------------------------------

	//! constructor
	CGUITabControl::CGUITabControl(IGUIEnvironment* environment,
		IGUIElement* parent, const rect<SINT32>& rectangle,
		bool fillbackground, bool border, SINT32 id)
		: IGUITabControl(environment, parent, id, rectangle), ActiveTab(-1),
		Border(border), FillBackground(fillbackground), ScrollControl(false), TabHeight(0), VerticalAlignment(EGUIA_UPPERLEFT),
		UpButton(0), DownButton(0), TabMaxWidth(0), CurrentScrollTabIndex(0), TabExtraWidth(20)
	{
#ifdef _DEBUG
		setDebugName("CGUITabControl");
#endif

		IGUISkin* skin = Environment->getSkin();
		IGUISpriteBank* sprites = 0;

		TabHeight = 32;

		if (skin)
		{
			sprites = skin->getSpriteBank();
			TabHeight = skin->getSize(EGDS_BUTTON_HEIGHT) + 2;
		}

		UpButton = Environment->addButton(rect<SINT32>(0, 0, 10, 10), this);

		if (UpButton)
		{
			UpButton->setSpriteBank(sprites);
			UpButton->setVisible(false);
			UpButton->setSubElement(true);
			UpButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
			UpButton->setOverrideFont(Environment->getBuiltInFont());
			UpButton->grab();
		}

		DownButton = Environment->addButton(rect<SINT32>(0, 0, 10, 10), this);

		if (DownButton)
		{
			DownButton->setSpriteBank(sprites);
			DownButton->setVisible(false);
			DownButton->setSubElement(true);
			DownButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
			DownButton->setOverrideFont(Environment->getBuiltInFont());
			DownButton->grab();
		}

		setTabVerticalAlignment(EGUIA_UPPERLEFT);
		refreshSprites();
	}

	//! destructor
	CGUITabControl::~CGUITabControl()
	{
		for (UINT32 i = 0; i<Tabs.size(); ++i)
		{
			if (Tabs[i])
				Tabs[i]->drop();
		}

		if (UpButton)
			UpButton->drop();

		if (DownButton)
			DownButton->drop();
	}

	void CGUITabControl::refreshSprites()
	{
		ColourValue color(255, 255, 255, 255);
		IGUISkin* skin = Environment->getSkin();
		if (skin)
		{
			color = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);
		}

		if (UpButton)
		{
			UpButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_CURSOR_LEFT), color);
			UpButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_LEFT), color);
		}

		if (DownButton)
		{
			DownButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_CURSOR_RIGHT), color);
			DownButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_RIGHT), color);
		}
	}

	//! Adds a tab
	IGUITab* CGUITabControl::addTab(const wchar_t* caption, SINT32 id)
	{
		CGUITab* tab = new CGUITab(Tabs.size(), Environment, this, calcTabPos(), id);

		tab->setText(caption);
		tab->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
		tab->setVisible(false);
		Tabs.push_back(tab);

		if (ActiveTab == -1)
		{
			ActiveTab = 0;
			tab->setVisible(true);
		}

		recalculateScrollBar();

		return tab;
	}


	//! adds a tab which has been created elsewhere
	void CGUITabControl::addTab(CGUITab* tab)
	{
		if (!tab)
			return;

		// check if its already added
		for (UINT32 i = 0; i < Tabs.size(); ++i)
		{
			if (Tabs[i] == tab)
				return;
		}

		tab->grab();

		if (tab->getNumber() == -1)
			tab->setNumber((SINT32)Tabs.size());

		while (tab->getNumber() >= (SINT32)Tabs.size())
			Tabs.push_back(0);

		if (Tabs[tab->getNumber()])
		{
			Tabs.push_back(Tabs[tab->getNumber()]);
			Tabs[Tabs.size() - 1]->setNumber(Tabs.size());
		}
		Tabs[tab->getNumber()] = tab;

		if (ActiveTab == -1)
			ActiveTab = tab->getNumber();


		if (tab->getNumber() == ActiveTab)
		{
			setActiveTab(ActiveTab);
		}
	}

	//! Insert the tab at the given index
	IGUITab* CGUITabControl::insertTab(SINT32 idx, const wchar_t* caption, SINT32 id)
	{
		if (idx < 0 || idx >(SINT32)Tabs.size())	// idx == Tabs.size() is indeed ok here as array can handle that
			return NULL;

		CGUITab* tab = new CGUITab(idx, Environment, this, calcTabPos(), id);

		tab->setText(caption);
		tab->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
		tab->setVisible(false);
		//Tabs.insert(tab, (UINT32)idx);
		Tabs.insert(Tabs.begin() + (UINT32)idx, tab);
		if (ActiveTab == -1)
		{
			ActiveTab = 0;
			tab->setVisible(true);
		}

		for (UINT32 i = (UINT32)idx + 1; i < Tabs.size(); ++i)
		{
			Tabs[i]->setNumber(i);
		}

		recalculateScrollBar();

		return tab;
	}

	//! Removes a tab from the tabcontrol
	void CGUITabControl::removeTab(SINT32 idx)
	{
		if (idx < 0 || idx >= (SINT32)Tabs.size())
			return;

		Tabs[(UINT32)idx]->drop();
		Tabs.erase(Tabs.begin() + (UINT32)idx);
		for (UINT32 i = (UINT32)idx; i < Tabs.size(); ++i)
		{
			Tabs[i]->setNumber(i);
		}
	}

	//! Clears the tabcontrol removing all tabs
	void CGUITabControl::clear()
	{
		for (UINT32 i = 0; i<Tabs.size(); ++i)
		{
			if (Tabs[i])
				Tabs[i]->drop();
		}
		Tabs.clear();
	}

	//! Returns amount of tabs in the tabcontrol
	SINT32 CGUITabControl::getTabCount() const
	{
		return Tabs.size();
	}


	//! Returns a tab based on zero based index
	IGUITab* CGUITabControl::getTab(SINT32 idx) const
	{
		if ((UINT32)idx >= Tabs.size())
			return 0;

		return Tabs[idx];
	}


	//! called if an event happened.
	bool CGUITabControl::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{

			switch (event.EventType)
			{
			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_BUTTON_CLICKED:
					if (event.GUIEvent.Caller == UpButton)
					{
						scrollLeft();
						return true;
					}
					else if (event.GUIEvent.Caller == DownButton)
					{
						scrollRight();
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
				case EMIE_LMOUSE_PRESSED_DOWN:
					// todo: dragging tabs around
					return true;
				case EMIE_LMOUSE_LEFT_UP:
				{
					SINT32 idx = getTabAt(event.MouseInput.X, event.MouseInput.Y);
					if (idx >= 0)
					{
						setActiveTab(idx);
						return true;
					}
					break;
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


	void CGUITabControl::scrollLeft()
	{
		if (CurrentScrollTabIndex > 0)
			--CurrentScrollTabIndex;
		recalculateScrollBar();
	}


	void CGUITabControl::scrollRight()
	{
		if (CurrentScrollTabIndex < (SINT32)(Tabs.size()) - 1)
		{
			if (needScrollControl(CurrentScrollTabIndex, true))
				++CurrentScrollTabIndex;
		}
		recalculateScrollBar();
	}

	SINT32 CGUITabControl::calcTabWidth(SINT32 pos, IGUIFont* font, const wchar_t* text, bool withScrollControl) const
	{
		if (!font)
			return 0;

		SINT32 len = font->getDimension(text).Width + TabExtraWidth;
		if (TabMaxWidth > 0 && len > TabMaxWidth)
			len = TabMaxWidth;

		// check if we miss the place to draw the tab-button
		if (withScrollControl && ScrollControl && pos + len > UpButton->getAbsolutePosition().UpperLeftCorner.x - 2)
		{
			SINT32 tabMinWidth = font->getDimension(L"A").Width;
			if (TabExtraWidth > 0 && TabExtraWidth > tabMinWidth)
				tabMinWidth = TabExtraWidth;

			if (ScrollControl && pos + tabMinWidth <= UpButton->getAbsolutePosition().UpperLeftCorner.x - 2)
			{
				len = UpButton->getAbsolutePosition().UpperLeftCorner.x - 2 - pos;
			}
		}
		return len;
	}

	bool CGUITabControl::needScrollControl(SINT32 startIndex, bool withScrollControl)
	{
		if (startIndex >= (SINT32)Tabs.size())
			startIndex -= 1;

		if (startIndex < 0)
			startIndex = 0;

		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return false;

		IGUIFont* font = skin->getFont();

		rect<SINT32> frameRect(AbsoluteRect);

		if (Tabs.empty())
			return false;

		if (!font)
			return false;

		SINT32 pos = frameRect.UpperLeftCorner.x + 2;

		for (SINT32 i = startIndex; i<(SINT32)Tabs.size(); ++i)
		{
			// get Text
			const wchar_t* text = 0;
			if (Tabs[i])
				text = Tabs[i]->getText();

			// get text length
			SINT32 len = calcTabWidth(pos, font, text, false);	// always without withScrollControl here or len would be shortened

			frameRect.LowerRightCorner.x += len;

			frameRect.UpperLeftCorner.x = pos;
			frameRect.LowerRightCorner.x = frameRect.UpperLeftCorner.x + len;
			pos += len;

			if (withScrollControl && pos > UpButton->getAbsolutePosition().UpperLeftCorner.x - 2)
				return true;

			if (!withScrollControl && pos > AbsoluteRect.LowerRightCorner.x)
				return true;
		}

		return false;
	}


	rect<SINT32> CGUITabControl::calcTabPos()
	{
		rect<SINT32> r;
		r.UpperLeftCorner.x = 0;
		r.LowerRightCorner.x = AbsoluteRect.getWidth();
		if (Border)
		{
			++r.UpperLeftCorner.x;
			--r.LowerRightCorner.x;
		}

		if (VerticalAlignment == EGUIA_UPPERLEFT)
		{
			r.UpperLeftCorner.y = TabHeight + 2;
			r.LowerRightCorner.y = AbsoluteRect.getHeight() - 1; 
			if (Border)
			{
				--r.LowerRightCorner.y;
			}
		}
		else
		{
			r.UpperLeftCorner.y = 0;
			r.LowerRightCorner.y = AbsoluteRect.getHeight() - (TabHeight + 2);
			if (Border)
			{
				++r.UpperLeftCorner.y;
			}
		}

		return r;
	}


	//! draws the element and its children
	void CGUITabControl::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return;

		IGUIFont* font = skin->getFont();
		IVideoDriver* driver = Environment->getVideoDriver();

		rect<SINT32> frameRect(AbsoluteRect);

		if (Tabs.empty())
			driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

		if (!font)
			return;

		if (VerticalAlignment == EGUIA_UPPERLEFT)
		{
			frameRect.UpperLeftCorner.y += 2;
			frameRect.LowerRightCorner.y = frameRect.UpperLeftCorner.y + TabHeight;
		}
		else
		{
			frameRect.UpperLeftCorner.y = frameRect.LowerRightCorner.y - TabHeight - 1;
			frameRect.LowerRightCorner.y -= 2;
		}

		rect<SINT32> tr;
		SINT32 pos = frameRect.UpperLeftCorner.x + 2;

		bool needLeftScroll = CurrentScrollTabIndex > 0;
		bool needRightScroll = false;

		// left and right pos of the active tab
		SINT32 left = 0;
		SINT32 right = 0;

		//const wchar_t* activetext = 0;
		CGUITab *activeTab = 0;

		for (UINT32 i = CurrentScrollTabIndex; i<Tabs.size(); ++i)
		{
			// get Text
			const wchar_t* text = 0;
			if (Tabs[i])
				text = Tabs[i]->getText();

			// get text length
			SINT32 len = calcTabWidth(pos, font, text, true);
			if (ScrollControl && pos + len > UpButton->getAbsolutePosition().UpperLeftCorner.x - 2)
			{
				needRightScroll = true;
				break;
			}

			frameRect.LowerRightCorner.x += len;
			frameRect.UpperLeftCorner.x = pos;
			frameRect.LowerRightCorner.x = frameRect.UpperLeftCorner.x + len;

			pos += len;

			if (text)
				Tabs[i]->refreshSkinColors();

			if ((SINT32)i == ActiveTab)
			{
				left = frameRect.UpperLeftCorner.x;
				right = frameRect.LowerRightCorner.x;
				//activetext = text;
				activeTab = Tabs[i];
			}
			else
			{
				skin->draw3DTabButton(this, false, frameRect, &AbsoluteClippingRect, VerticalAlignment);

				// draw text
				rect<SINT32> textClipRect(frameRect);	// TODO: exact size depends on borders in draw3DTabButton which we don't get with current interface
				textClipRect.clipAgainst(AbsoluteClippingRect);
				font->draw(text, frameRect, Tabs[i]->getTextColor(),
					true, true, &textClipRect);
			}
		}

		// draw active tab
		if (left != 0 && right != 0 && activeTab != 0)
		{
			// draw upper highlight frame
			if (VerticalAlignment == EGUIA_UPPERLEFT)
			{
				frameRect.UpperLeftCorner.x = left - 2;
				frameRect.LowerRightCorner.x = right + 2;
				frameRect.UpperLeftCorner.y -= 2;

				skin->draw3DTabButton(this, true, frameRect, &AbsoluteClippingRect, VerticalAlignment);

				// draw text
				rect<SINT32> textClipRect(frameRect);	// TODO: exact size depends on borders in draw3DTabButton which we don't get with current interface
				textClipRect.clipAgainst(AbsoluteClippingRect);
				font->draw(activeTab->getText(), frameRect, activeTab->getTextColor(),
					true, true, &textClipRect);

				tr.UpperLeftCorner.x = AbsoluteRect.UpperLeftCorner.x;
				tr.LowerRightCorner.x = left - 1;
				tr.UpperLeftCorner.y = frameRect.LowerRightCorner.y - 1;
				tr.LowerRightCorner.y = frameRect.LowerRightCorner.y;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), tr, &AbsoluteClippingRect);

				tr.UpperLeftCorner.x = right;
				tr.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), tr, &AbsoluteClippingRect);
			}
			else
			{

				frameRect.UpperLeftCorner.x = left - 2;
				frameRect.LowerRightCorner.x = right + 2;
				frameRect.LowerRightCorner.y += 2;

				skin->draw3DTabButton(this, true, frameRect, &AbsoluteClippingRect, VerticalAlignment);

				// draw text
				font->draw(activeTab->getText(), frameRect, activeTab->getTextColor(),
					true, true, &frameRect);

				tr.UpperLeftCorner.x = AbsoluteRect.UpperLeftCorner.x;
				tr.LowerRightCorner.x = left - 1;
				tr.UpperLeftCorner.y = frameRect.UpperLeftCorner.y - 1;
				tr.LowerRightCorner.y = frameRect.UpperLeftCorner.y;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), tr, &AbsoluteClippingRect);

				tr.UpperLeftCorner.x = right;
				tr.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), tr, &AbsoluteClippingRect);
			}
		}
		else
		{
			if (VerticalAlignment == EGUIA_UPPERLEFT)
			{
				tr.UpperLeftCorner.x = AbsoluteRect.UpperLeftCorner.x;
				tr.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x;
				tr.UpperLeftCorner.y = frameRect.LowerRightCorner.y - 1;
				tr.LowerRightCorner.y = frameRect.LowerRightCorner.y;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), tr, &AbsoluteClippingRect);
			}
			else
			{
				tr.UpperLeftCorner.x = AbsoluteRect.UpperLeftCorner.x;
				tr.LowerRightCorner.x = 1000;
				tr.UpperLeftCorner.y = frameRect.UpperLeftCorner.y - 1;
				tr.LowerRightCorner.y = frameRect.UpperLeftCorner.y;
				driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), tr, &AbsoluteClippingRect);
			}
		}

		skin->draw3DTabBody(this, Border, FillBackground, AbsoluteRect, &AbsoluteClippingRect, TabHeight, VerticalAlignment);

		// enable scrollcontrols on need
		if (UpButton)
			UpButton->setEnabled(needLeftScroll);
		if (DownButton)
			DownButton->setEnabled(needRightScroll);
		refreshSprites();

		IGUIElement::draw();
	}


	//! Set the height of the tabs
	void CGUITabControl::setTabHeight(SINT32 height)
	{
		if (height < 0)
			height = 0;

		TabHeight = height;

		recalculateScrollButtonPlacement();
		recalculateScrollBar();
	}


	//! Get the height of the tabs
	SINT32 CGUITabControl::getTabHeight() const
	{
		return TabHeight;
	}

	//! set the maximal width of a tab. Per default width is 0 which means "no width restriction".
	void CGUITabControl::setTabMaxWidth(SINT32 width)
	{
		TabMaxWidth = width;
	}

	//! get the maximal width of a tab
	SINT32 CGUITabControl::getTabMaxWidth() const
	{
		return TabMaxWidth;
	}


	//! Set the extra width added to tabs on each side of the text
	void CGUITabControl::setTabExtraWidth(SINT32 extraWidth)
	{
		if (extraWidth < 0)
			extraWidth = 0;

		TabExtraWidth = extraWidth;

		recalculateScrollBar();
	}


	//! Get the extra width added to tabs on each side of the text
	SINT32 CGUITabControl::getTabExtraWidth() const
	{
		return TabExtraWidth;
	}


	void CGUITabControl::recalculateScrollBar()
	{
		if (!UpButton || !DownButton)
			return;

		ScrollControl = needScrollControl() || CurrentScrollTabIndex > 0;

		if (ScrollControl)
		{
			UpButton->setVisible(true);
			DownButton->setVisible(true);
		}
		else
		{
			UpButton->setVisible(false);
			DownButton->setVisible(false);
		}

		bringToFront(UpButton);
		bringToFront(DownButton);
	}

	//! Set the alignment of the tabs
	void CGUITabControl::setTabVerticalAlignment(EGUI_ALIGNMENT alignment)
	{
		VerticalAlignment = alignment;

		recalculateScrollButtonPlacement();
		recalculateScrollBar();

		rect<SINT32> r(calcTabPos());
		for (UINT32 i = 0; i<Tabs.size(); ++i)
		{
			Tabs[i]->setRelativePosition(r);
		}
	}

	void CGUITabControl::recalculateScrollButtonPlacement()
	{
		IGUISkin* skin = Environment->getSkin();
		SINT32 ButtonSize = 16;
		SINT32 ButtonHeight = TabHeight - 2;
		if (ButtonHeight < 0)
			ButtonHeight = TabHeight;
		if (skin)
		{
			ButtonSize = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
			if (ButtonSize > TabHeight)
				ButtonSize = TabHeight;
		}

		SINT32 ButtonX = RelativeRect.getWidth() - (SINT32)(2.5f*(FLOAT32)ButtonSize) - 1;
		SINT32 ButtonY = 0;

		if (VerticalAlignment == EGUIA_UPPERLEFT)
		{
			ButtonY = 2 + (TabHeight / 2) - (ButtonHeight / 2);
			UpButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
			DownButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
		}
		else
		{
			ButtonY = RelativeRect.getHeight() - (TabHeight / 2) - (ButtonHeight / 2) - 2;
			UpButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
			DownButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
		}

		UpButton->setRelativePosition(rect<SINT32>(ButtonX, ButtonY, ButtonX + ButtonSize, ButtonY + ButtonHeight));
		ButtonX += ButtonSize + 1;
		DownButton->setRelativePosition(rect<SINT32>(ButtonX, ButtonY, ButtonX + ButtonSize, ButtonY + ButtonHeight));
	}

	//! Get the alignment of the tabs
	EGUI_ALIGNMENT CGUITabControl::getTabVerticalAlignment() const
	{
		return VerticalAlignment;
	}


	SINT32 CGUITabControl::getTabAt(SINT32 xpos, SINT32 ypos) const
	{
		Position2d p(xpos, ypos);
		IGUISkin* skin = Environment->getSkin();
		IGUIFont* font = skin->getFont();

		rect<SINT32> frameRect(AbsoluteRect);

		if (VerticalAlignment == EGUIA_UPPERLEFT)
		{
			frameRect.UpperLeftCorner.y += 2;
			frameRect.LowerRightCorner.y = frameRect.UpperLeftCorner.y + TabHeight;
		}
		else
		{
			frameRect.UpperLeftCorner.y = frameRect.LowerRightCorner.y - TabHeight;
		}

		SINT32 pos = frameRect.UpperLeftCorner.x + 2;

		if (!frameRect.isPointInside(p))
			return -1;

		for (SINT32 i = CurrentScrollTabIndex; i<(SINT32)Tabs.size(); ++i)
		{
			// get Text
			const wchar_t* text = 0;
			if (Tabs[i])
				text = Tabs[i]->getText();

			// get text length
			SINT32 len = calcTabWidth(pos, font, text, true);
			if (ScrollControl && pos + len > UpButton->getAbsolutePosition().UpperLeftCorner.x - 2)
				return -1;

			frameRect.UpperLeftCorner.x = pos;
			frameRect.LowerRightCorner.x = frameRect.UpperLeftCorner.x + len;

			pos += len;

			if (frameRect.isPointInside(p))
			{
				return i;
			}
		}
		return -1;
	}

	//! Returns which tab is currently active
	SINT32 CGUITabControl::getActiveTab() const
	{
		return ActiveTab;
	}


	//! Brings a tab to front.
	bool CGUITabControl::setActiveTab(SINT32 idx)
	{
		if ((UINT32)idx >= Tabs.size())
			return false;

		bool changed = (ActiveTab != idx);

		ActiveTab = idx;

		for (SINT32 i = 0; i<(SINT32)Tabs.size(); ++i)
			if (Tabs[i])
				Tabs[i]->setVisible(i == ActiveTab);

		if (changed)
		{
			SEvent event;
			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = this;
			event.GUIEvent.Element = 0;
			event.GUIEvent.EventType = EGET_TAB_CHANGED;
			Parent->OnEvent(event);
		}

		return true;
	}


	bool CGUITabControl::setActiveTab(IGUITab *tab)
	{
		for (SINT32 i = 0; i<(SINT32)Tabs.size(); ++i)
			if (Tabs[i] == tab)
				return setActiveTab(i);
		return false;
	}


	//! Removes a child.
	void CGUITabControl::removeChild(IGUIElement* child)
	{
		bool isTab = false;

		UINT32 i = 0;
		// check if it is a tab
		while (i<Tabs.size())
		{
			if (Tabs[i] == child)
			{
				Tabs[i]->drop();
				Tabs.erase(Tabs.begin() + i);
				isTab = true;
			}
			else
				++i;
		}

		// reassign numbers
		if (isTab)
		{
			for (i = 0; i<Tabs.size(); ++i)
				if (Tabs[i])
					Tabs[i]->setNumber(i);
		}

		// remove real element
		IGUIElement::removeChild(child);

		recalculateScrollBar();
	}


	//! Update the position of the element, decides scroll button status
	void CGUITabControl::updateAbsolutePosition()
	{
		IGUIElement::updateAbsolutePosition();
		recalculateScrollBar();
	}


	//! Writes attributes of the element.
	void CGUITabControl::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUITabControl::serializeAttributes(out, options);

		out->addInt("ActiveTab", ActiveTab);
		out->addBool("Border", Border);
		out->addBool("FillBackground", FillBackground);
		out->addInt("TabHeight", TabHeight);
		out->addInt("TabMaxWidth", TabMaxWidth);
		out->addEnum("TabVerticalAlignment", SINT32(VerticalAlignment), GUIAlignmentNames);
	}


	//! Reads attributes of the element
	void CGUITabControl::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		Border = in->getAttributeAsBool("Border");
		FillBackground = in->getAttributeAsBool("FillBackground");

		ActiveTab = -1;

		setTabHeight(in->getAttributeAsInt("TabHeight"));
		TabMaxWidth = in->getAttributeAsInt("TabMaxWidth");

		IGUITabControl::deserializeAttributes(in, options);

		setActiveTab(in->getAttributeAsInt("ActiveTab"));
		setTabVerticalAlignment(static_cast<EGUI_ALIGNMENT>(in->getAttributeAsEnumeration("TabVerticalAlignment", GUIAlignmentNames)));
	}
}

#endif