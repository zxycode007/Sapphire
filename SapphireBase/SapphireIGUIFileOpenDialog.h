#ifndef _SAPPHIRE_I_GUI_FILE_OPEN_DIALOG_
#define _SAPPHIRE_I_GUI_FILE_OPEN_DIALOG_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! Standard file chooser dialog.
	/** \warning When the user selects a folder this does change the current working directory

	\par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_DIRECTORY_SELECTED
	\li EGET_FILE_SELECTED
	\li EGET_FILE_CHOOSE_DIALOG_CANCELLED
	*/
	class IGUIFileOpenDialog : public IGUIElement
	{
	public:

		//! constructor
		IGUIFileOpenDialog(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_FILE_OPEN_DIALOG, environment, parent, id, rectangle) {}

		//! Returns the filename of the selected file. Returns NULL, if no file was selected.
		virtual const wchar_t* getFileName() const = 0;

		//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
		virtual const path& getDirectoryName() = 0;
	};
}

#endif