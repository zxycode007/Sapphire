#ifndef _SAPPHIRE_I_GUI_COMBO_BOX_
#define _SAPPHIRE_I_GUI_COMBO_BOX_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! Combobox widget
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_COMBO_BOX_CHANGED
	*/
	class IGUIComboBox : public IGUIElement
	{
	public:

		//! constructor
		IGUIComboBox(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_COMBO_BOX, environment, parent, id, rectangle) {}

		//! Returns amount of items in box
		virtual UINT32 getItemCount() const = 0;

		//! Returns string of an item. the idx may be a value from 0 to itemCount-1
		virtual const wchar_t* getItem(UINT32 idx) const = 0;

		//! Returns item data of an item. the idx may be a value from 0 to itemCount-1
		virtual UINT32 getItemData(UINT32 idx) const = 0;

		//! Returns index based on item data
		virtual SINT32 getIndexForItemData(UINT32 data) const = 0;

		//! Adds an item and returns the index of it
		virtual UINT32 addItem(const wchar_t* text, UINT32 data = 0) = 0;

		//! Removes an item from the combo box.
		/** Warning. This will change the index of all following items */
		virtual void removeItem(UINT32 idx) = 0;

		//! Deletes all items in the combo box
		virtual void clear() = 0;

		//! Returns id of selected item. returns -1 if no item is selected.
		virtual SINT32 getSelected() const = 0;

		//! Sets the selected item. Set this to -1 if no item should be selected
		virtual void setSelected(SINT32 idx) = 0;

		//! Sets text justification of the text area
		/** \param horizontal: EGUIA_UPPERLEFT for left justified (default),
		EGUIA_LOWEERRIGHT for right justified, or EGUIA_CENTER for centered text.
		\param vertical: EGUIA_UPPERLEFT to align with top edge,
		EGUIA_LOWEERRIGHT for bottom edge, or EGUIA_CENTER for centered text (default). */
		virtual void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical) = 0;

		//! Set the maximal number of rows for the selection listbox
		virtual void setMaxSelectionRows(UINT32 max) = 0;

		//! Get the maximimal number of rows for the selection listbox
		virtual UINT32 getMaxSelectionRows() const = 0;
	};
}

#endif