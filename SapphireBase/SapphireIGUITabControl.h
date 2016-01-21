#ifndef _SAPPHIRE_I_GUI_TAB_CONTROL_
#define _SAPPHIRE_I_GUI_TAB_CONTROL_

#include "SapphireIGUIElement.h"
#include "SapphireColorValue.h"
#include "SapphireIGUISkin.h"

namespace Sapphire
{
	//! A tab-page, onto which other gui elements could be added.
	/** IGUITab refers to the page itself, not to the tab in the tabbar of an IGUITabControl. */
	class IGUITab : public IGUIElement
	{
	public:

		//! constructor
		IGUITab(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_TAB, environment, parent, id, rectangle) {}

		//! Returns zero based index of tab if in tabcontrol.
		/** Can be accessed later IGUITabControl::getTab() by this number.
		Note that this number can change when other tabs are inserted or removed .
		*/
		virtual SINT32 getNumber() const = 0;

		//! sets if the tab should draw its background
		virtual void setDrawBackground(bool draw = true) = 0;

		//! sets the color of the background, if it should be drawn.
		virtual void setBackgroundColor(ColourValue c) = 0;

		//! returns true if the tab is drawing its background, false if not
		virtual bool isDrawingBackground() const = 0;

		//! returns the color of the background
		virtual ColourValue getBackgroundColor() const = 0;

		//! sets the color of the text
		virtual void setTextColor(ColourValue c) = 0;

		//! gets the color of the text
		virtual ColourValue getTextColor() const = 0;
	};

	//! A standard tab control
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_TAB_CHANGED
	*/
	class IGUITabControl : public IGUIElement
	{
	public:

		//! constructor
		IGUITabControl(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_TAB_CONTROL, environment, parent, id, rectangle) {}

		//! Adds a tab
		virtual IGUITab* addTab(const wchar_t* caption, SINT32 id = -1) = 0;

		//! Insert the tab at the given index
		/** \return The tab on success or NULL on failure. */
		virtual IGUITab* insertTab(SINT32 idx, const wchar_t* caption, SINT32 id = -1) = 0;

		//! Removes a tab from the tabcontrol
		virtual void removeTab(SINT32 idx) = 0;

		//! Clears the tabcontrol removing all tabs
		virtual void clear() = 0;

		//! Returns amount of tabs in the tabcontrol
		virtual SINT32 getTabCount() const = 0;

		//! Returns a tab based on zero based index
		/** \param idx: zero based index of tab. Is a value betwenn 0 and getTabcount()-1;
		\return Returns pointer to the Tab. Returns 0 if no tab
		is corresponding to this tab. */
		virtual IGUITab* getTab(SINT32 idx) const = 0;

		//! Brings a tab to front.
		/** \param idx: number of the tab.
		\return Returns true if successful. */
		virtual bool setActiveTab(SINT32 idx) = 0;

		//! Brings a tab to front.
		/** \param tab: pointer to the tab.
		\return Returns true if successful. */
		virtual bool setActiveTab(IGUITab *tab) = 0;

		//! Returns which tab is currently active
		virtual SINT32 getActiveTab() const = 0;

		//! get the the id of the tab at the given absolute coordinates
		/** \return The id of the tab or -1 when no tab is at those coordinates*/
		virtual SINT32 getTabAt(SINT32 xpos, SINT32 ypos) const = 0;

		//! Set the height of the tabs
		virtual void setTabHeight(SINT32 height) = 0;

		//! Get the height of the tabs
		/** return Returns the height of the tabs */
		virtual SINT32 getTabHeight() const = 0;

		//! set the maximal width of a tab. Per default width is 0 which means "no width restriction".
		virtual void setTabMaxWidth(SINT32 width) = 0;

		//! get the maximal width of a tab
		virtual SINT32 getTabMaxWidth() const = 0;

		//! Set the alignment of the tabs
		/** Use EGUIA_UPPERLEFT or EGUIA_LOWERRIGHT */
		virtual void setTabVerticalAlignment(EGUI_ALIGNMENT alignment) = 0;

		//! Get the alignment of the tabs
		/** return Returns the alignment of the tabs */
		virtual EGUI_ALIGNMENT getTabVerticalAlignment() const = 0;

		//! Set the extra width added to tabs on each side of the text
		virtual void setTabExtraWidth(SINT32 extraWidth) = 0;

		//! Get the extra width added to tabs on each side of the text
		/** return Returns the extra width of the tabs */
		virtual SINT32 getTabExtraWidth() const = 0;
	};
}

#endif