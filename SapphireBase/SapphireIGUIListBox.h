#ifndef _SAPPHIRE_I_GUI_LIST_BOX_
#define _SAPPHIRE_I_GUI_LIST_BOX_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	class IGUISpriteBank;

	//! Enumeration for listbox colors
	enum EGUI_LISTBOX_COLOR
	{
		//! Color of text
		EGUI_LBC_TEXT = 0,
		//! Color of selected text
		EGUI_LBC_TEXT_HIGHLIGHT,
		//! Color of icon
		EGUI_LBC_ICON,
		//! Color of selected icon
		EGUI_LBC_ICON_HIGHLIGHT,
		//! Not used, just counts the number of available colors
		EGUI_LBC_COUNT
	};


	//! Default list box GUI element.
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_LISTBOX_CHANGED
	\li EGET_LISTBOX_SELECTED_AGAIN
	*/
	class IGUIListBox : public IGUIElement
	{
	public:
		//! constructor
		IGUIListBox(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_LIST_BOX, environment, parent, id, rectangle) {}

		//! returns amount of list items
		virtual UINT32 getItemCount() const = 0;

		//! returns string of a list item. the may id be a value from 0 to itemCount-1
		virtual const wchar_t* getListItem(UINT32 id) const = 0;

		//! adds an list item, returns id of item
		virtual UINT32 addItem(const wchar_t* text) = 0;

		//! adds an list item with an icon
		/** \param text Text of list entry
		\param icon Sprite index of the Icon within the current sprite bank. Set it to -1 if you want no icon
		\return The id of the new created item */
		virtual UINT32 addItem(const wchar_t* text, SINT32 icon) = 0;

		//! Removes an item from the list
		virtual void removeItem(UINT32 index) = 0;

		//! get the the id of the item at the given absolute coordinates
		/** \return The id of the listitem or -1 when no item is at those coordinates*/
		virtual SINT32 getItemAt(SINT32 xpos, SINT32 ypos) const = 0;

		//! Returns the icon of an item
		virtual SINT32 getIcon(UINT32 index) const = 0;

		//! Sets the sprite bank which should be used to draw list icons.
		/** This font is set to the sprite bank of the built-in-font by
		default. A sprite can be displayed in front of every list item.
		An icon is an index within the icon sprite bank. Several
		default icons are available in the skin through getIcon. */
		virtual void setSpriteBank(IGUISpriteBank* bank) = 0;

		//! clears the list, deletes all items in the listbox
		virtual void clear() = 0;

		//! returns id of selected item. returns -1 if no item is selected.
		virtual SINT32 getSelected() const = 0;

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(SINT32 index) = 0;

		//! sets the selected item. Set this to 0 if no item should be selected
		virtual void setSelected(const wchar_t *item) = 0;

		//! set whether the listbox should scroll to newly selected items
		virtual void setAutoScrollEnabled(bool scroll) = 0;

		//! returns true if automatic scrolling is enabled, false if not.
		virtual bool isAutoScrollEnabled() const = 0;

		//! set all item colors at given index to color
		virtual void setItemOverrideColor(UINT32 index, ColourValue color) = 0;

		//! set all item colors of specified type at given index to color
		virtual void setItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType, ColourValue color) = 0;

		//! clear all item colors at index
		virtual void clearItemOverrideColor(UINT32 index) = 0;

		//! clear item color at index for given colortype
		virtual void clearItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) = 0;

		//! has the item at index its color overwritten?
		virtual bool hasItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const = 0;

		//! return the overwrite color at given item index.
		virtual ColourValue getItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const = 0;

		//! return the default color which is used for the given colorType
		virtual ColourValue getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const = 0;

		//! set the item at the given index
		virtual void setItem(UINT32 index, const wchar_t* text, SINT32 icon) = 0;

		//! Insert the item at the given index
		/** \return The index on success or -1 on failure. */
		virtual SINT32 insertItem(UINT32 index, const wchar_t* text, SINT32 icon) = 0;

		//! Swap the items at the given indices
		virtual void swapItems(UINT32 index1, UINT32 index2) = 0;

		//! set global itemHeight
		virtual void setItemHeight(SINT32 height) = 0;

		//! Sets whether to draw the background
		virtual void setDrawBackground(bool draw) = 0;
	};
}

#endif