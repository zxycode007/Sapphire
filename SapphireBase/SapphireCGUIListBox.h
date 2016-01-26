#ifndef _SAPPHIRE_C_GUI_LIST_BOX_
#define _SAPPHIRE_C_GUI_LIST_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIListBox.h"

namespace Sapphire
{
	class IGUIFont;
	class IGUIScrollBar;

	class CGUIListBox : public IGUIListBox
	{
	public:
		//! constructor
		CGUIListBox(IGUIEnvironment* environment, IGUIElement* parent,
			SINT32 id, rect<SINT32> rectangle, bool clip = true,
			bool drawBack = false, bool moveOverSelect = false);

		//! destructor
		virtual ~CGUIListBox();

		//! returns amount of list items
		virtual UINT32 getItemCount() const;

		//! returns string of a list item. the id may be a value from 0 to itemCount-1
		virtual const wchar_t* getListItem(UINT32 id) const;

		//! adds an list item, returns id of item
		virtual UINT32 addItem(const wchar_t* text);

		//! clears the list
		virtual void clear();

		//! returns id of selected item. returns -1 if no item is selected.
		virtual SINT32 getSelected() const;

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(SINT32 id);

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(const wchar_t *item);

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! adds an list item with an icon
		//! \param text Text of list entry
		//! \param icon Sprite index of the Icon within the current sprite bank. Set it to -1 if you want no icon
		//! \return
		//! returns the id of the new created item
		virtual UINT32 addItem(const wchar_t* text, SINT32 icon);

		//! Returns the icon of an item
		virtual SINT32 getIcon(UINT32 id) const;

		//! removes an item from the list
		virtual void removeItem(UINT32 id);

		//! get the the id of the item at the given absolute coordinates
		virtual SINT32 getItemAt(SINT32 xpos, SINT32 ypos) const;

		//! Sets the sprite bank which should be used to draw list icons. This font is set to the sprite bank of
		//! the built-in-font by default. A sprite can be displayed in front of every list item.
		//! An icon is an index within the icon sprite bank. Several default icons are available in the
		//! skin through getIcon
		virtual void setSpriteBank(IGUISpriteBank* bank);

		//! set whether the listbox should scroll to newly selected items
		virtual void setAutoScrollEnabled(bool scroll);

		//! returns true if automatic scrolling is enabled, false if not.
		virtual bool isAutoScrollEnabled() const;

		//! Update the position and size of the listbox, and update the scrollbar
		virtual void updateAbsolutePosition();

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

		//! set all item colors at given index to color
		virtual void setItemOverrideColor(UINT32 index, ColourValue color);

		//! set all item colors of specified type at given index to color
		virtual void setItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType, ColourValue color);

		//! clear all item colors at index
		virtual void clearItemOverrideColor(UINT32 index);

		//! clear item color at index for given colortype
		virtual void clearItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType);

		//! has the item at index its color overwritten?
		virtual bool hasItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const;

		//! return the overwrite color at given item index.
		virtual ColourValue getItemOverrideColor(UINT32 index, EGUI_LISTBOX_COLOR colorType) const;

		//! return the default color which is used for the given colorType
		virtual ColourValue getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const;

		//! set the item at the given index
		virtual void setItem(UINT32 index, const wchar_t* text, SINT32 icon);

		//! Insert the item at the given index
		//! Return the index on success or -1 on failure.
		virtual SINT32 insertItem(UINT32 index, const wchar_t* text, SINT32 icon);

		//! Swap the items at the given indices
		virtual void swapItems(UINT32 index1, UINT32 index2);

		//! set global itemHeight
		virtual void setItemHeight(SINT32 height);

		//! Sets whether to draw the background
		virtual void setDrawBackground(bool draw);


	private:

		struct ListItem
		{
			ListItem() : icon(-1)
			{}

			StringW text;
			SINT32 icon;

			// A multicolor extension
			struct ListItemOverrideColor
			{
				ListItemOverrideColor() : Use(false) {}
				bool Use;
				ColourValue Color;
			};
			ListItemOverrideColor OverrideColors[EGUI_LBC_COUNT];
		};

		void recalculateItemHeight();
		void selectNew(SINT32 ypos, bool onlyHover = false);
		void recalculateScrollPos();

		// extracted that function to avoid copy&paste code
		void recalculateItemWidth(SINT32 icon);

		// get labels used for serialization
		bool getSerializationLabels(EGUI_LISTBOX_COLOR colorType, String & useColorLabel, String & colorLabel) const;

		vector< ListItem >::type Items;
		SINT32 Selected;
		SINT32 ItemHeight;
		SINT32 ItemHeightOverride;
		SINT32 TotalItemHeight;
		SINT32 ItemsIconWidth;
		IGUIFont* Font;
		IGUISpriteBank* IconBank;
		IGUIScrollBar* ScrollBar;
		UINT32 selectTime;
		UINT32 LastKeyTime;
		StringW KeyBuffer;
		bool Selecting;
		bool DrawBack;
		bool MoveOverSelect;
		bool AutoScroll;
		bool HighlightWhenNotFocused;
	};
}

#endif

#endif