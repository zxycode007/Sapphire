#ifndef _SAPPHIRE_C_GUI_COMBO_BOX_
#define _SAPPHIRE_C_GUI_COMBO_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_

#include "SapphireIGUIComboBox.h"
#include "SapphireIGUIStaticText.h"



namespace Sapphire
{
	class IGUIButton;
	class IGUIListBox;

	//! Single line edit box for editing simple text.
	class CGUIComboBox : public IGUIComboBox
	{
	public:

		//! constructor
		CGUIComboBox(IGUIEnvironment* environment, IGUIElement* parent,
			SINT32 id, rect<SINT32> rectangle);

		//! Returns amount of items in box
		virtual UINT32 getItemCount() const;

		//! returns string of an item. the idx may be a value from 0 to itemCount-1
		virtual const wchar_t* getItem(UINT32 idx) const;

		//! Returns item data of an item. the idx may be a value from 0 to itemCount-1
		virtual UINT32 getItemData(UINT32 idx) const;

		//! Returns index based on item data
		virtual SINT32 getIndexForItemData(UINT32 data) const;

		//! adds an item and returns the index of it
		virtual UINT32 addItem(const wchar_t* text, UINT32 data);

		//! Removes an item from the combo box.
		virtual void removeItem(UINT32 id);

		//! deletes all items in the combo box
		virtual void clear();

		//! returns the text of the currently selected item
		virtual const wchar_t* getText() const;

		//! returns id of selected item. returns -1 if no item is selected.
		virtual SINT32 getSelected() const;

		//! sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(SINT32 idx);

		//! sets the text alignment of the text part
		virtual void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);

		//! Set the maximal number of rows for the selection listbox
		virtual void setMaxSelectionRows(UINT32 max);

		//! Get the maximimal number of rows for the selection listbox
		virtual UINT32 getMaxSelectionRows() const;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		void openCloseMenu();
		void sendSelectionChangedEvent();

		IGUIButton* ListButton;
		IGUIStaticText* SelectedText;
		IGUIListBox* ListBox;
		IGUIElement *LastFocus;


		struct SComboData
		{
			SComboData(const wchar_t * text, UINT32 data)
				: Name(text), Data(data) {}

			StringW Name;
			UINT32 Data;
		};
		vector< SComboData >::type Items;

		SINT32 Selected;
		EGUI_ALIGNMENT HAlign, VAlign;
		UINT32 MaxSelectionRows;
		bool HasFocus;
	};
}

#endif
#endif