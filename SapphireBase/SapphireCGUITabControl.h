#ifndef _SAPPHIRE_C_GUI_TAB_CONTROL_
#define _SAPPHIRE_C_GUI_TAB_CONTROL_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUITabControl.h"
#include "SapphireIGUISkin.h"

namespace Sapphire
{
	class CGUITabControl;
	class IGUIButton;

	// A tab, onto which other gui elements could be added.
	class CGUITab : public IGUITab
	{
	public:

		//! constructor
		CGUITab(SINT32 number, IGUIEnvironment* environment,
			IGUIElement* parent, const rect<SINT32>& rectangle,
			SINT32 id);

		//! destructor
		//virtual ~CGUITab();

		//! Returns number of this tab in tabcontrol. Can be accessed
		//! later IGUITabControl::getTab() by this number.
		virtual SINT32 getNumber() const;

		//! Sets the number
		virtual void setNumber(SINT32 n);

		//! draws the element and its children
		virtual void draw();

		//! sets if the tab should draw its background
		virtual void setDrawBackground(bool draw = true);

		//! sets the color of the background, if it should be drawn.
		virtual void setBackgroundColor(ColourValue c);

		//! sets the color of the text
		virtual void setTextColor(ColourValue c);

		//! returns true if the tab is drawing its background, false if not
		virtual bool isDrawingBackground() const;

		//! returns the color of the background
		virtual ColourValue getBackgroundColor() const;

		virtual ColourValue getTextColor() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

		//! only for internal use by CGUITabControl
		void refreshSkinColors();

	private:

		SINT32 Number;
		ColourValue BackColor;
		bool OverrideTextColorEnabled;
		ColourValue TextColor;
		bool DrawBackground;
	};


	//! A standard tab control
	class CGUITabControl : public IGUITabControl
	{
	public:

		//! destructor
		CGUITabControl(IGUIEnvironment* environment,
			IGUIElement* parent, const rect<SINT32>& rectangle,
			bool fillbackground = true, bool border = true, SINT32 id = -1);

		//! destructor
		virtual ~CGUITabControl();

		//! Adds a tab
		virtual IGUITab* addTab(const wchar_t* caption, SINT32 id = -1);

		//! Adds a tab that has already been created
		virtual void addTab(CGUITab* tab);

		//! Insert the tab at the given index
		virtual IGUITab* insertTab(SINT32 idx, const wchar_t* caption, SINT32 id = -1);

		//! Removes a tab from the tabcontrol
		virtual void removeTab(SINT32 idx);

		//! Clears the tabcontrol removing all tabs
		virtual void clear();

		//! Returns amount of tabs in the tabcontrol
		virtual SINT32 getTabCount() const;

		//! Returns a tab based on zero based index
		virtual IGUITab* getTab(SINT32 idx) const;

		//! Brings a tab to front.
		virtual bool setActiveTab(SINT32 idx);

		//! Brings a tab to front.
		virtual bool setActiveTab(IGUITab *tab);

		//! Returns which tab is currently active
		virtual SINT32 getActiveTab() const;

		//! get the the id of the tab at the given absolute coordinates
		virtual SINT32 getTabAt(SINT32 xpos, SINT32 ypos) const;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Removes a child.
		virtual void removeChild(IGUIElement* child);

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
		//! Set the height of the tabs
		virtual void setTabHeight(SINT32 height);

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

		//! Get the height of the tabs
		virtual SINT32 getTabHeight() const;

		//! set the maximal width of a tab. Per default width is 0 which means "no width restriction".
		virtual void setTabMaxWidth(SINT32 width);

		//! get the maximal width of a tab
		virtual SINT32 getTabMaxWidth() const;

		//! Set the alignment of the tabs
		//! note: EGUIA_CENTER is not an option
		virtual void setTabVerticalAlignment(EGUI_ALIGNMENT alignment);

		//! Get the alignment of the tabs
		virtual EGUI_ALIGNMENT getTabVerticalAlignment() const;

		//! Set the extra width added to tabs on each side of the text
		virtual void setTabExtraWidth(SINT32 extraWidth);

		//! Get the extra width added to tabs on each side of the text
		virtual SINT32 getTabExtraWidth() const;

		//! Update the position of the element, decides scroll button status
		virtual void updateAbsolutePosition();

	private:

		void scrollLeft();
		void scrollRight();
		bool needScrollControl(SINT32 startIndex = 0, bool withScrollControl = false);
		SINT32 calcTabWidth(SINT32 pos, IGUIFont* font, const wchar_t* text, bool withScrollControl) const;
		rect<SINT32> calcTabPos();

		void recalculateScrollButtonPlacement();
		void recalculateScrollBar();
		void refreshSprites();

		vector<CGUITab*>::type Tabs;	// CGUITab* because we need setNumber (which is certainly not nice)
		SINT32 ActiveTab;
		bool Border;
		bool FillBackground;
		bool ScrollControl;
		SINT32 TabHeight;
		EGUI_ALIGNMENT VerticalAlignment;
		IGUIButton* UpButton;
		IGUIButton* DownButton;
		SINT32 TabMaxWidth;
		SINT32 CurrentScrollTabIndex;
		SINT32 TabExtraWidth;
	};
}

#endif

#endif