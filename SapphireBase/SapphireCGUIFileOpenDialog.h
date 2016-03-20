#ifndef _SAPPHIRE_C_GUI_FILE_OPEN_DIALOG_
#define _SAPPHIRE_C_GUI_FILE_OPEN_DIALOG_


#include "SapphireCompileConifg.h"



#ifdef  _SAPPHIRE_COMPILE_WITH_GUI_

#include "SapphireIGUIFileOpenDialog.h"
#include "SapphireIGUIButton.h"
#include "SapphireIGUIListBox.h"
#include "SapphireIGUIEditBox.h"
#include "SapphireIFileSystem.h"

namespace Sapphire
{
	class CGUIFileOpenDialog : public IGUIFileOpenDialog
	{
	public:

		//! constructor
		CGUIFileOpenDialog(const wchar_t* title, IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, bool restoreCWD = false,
			StringW startDir = 0);

		//! destructor
		virtual ~CGUIFileOpenDialog();

		//! returns the filename of the selected file. Returns NULL, if no file was selected.
		virtual const wchar_t* getFileName() const;

		//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
		virtual const path& getDirectoryName();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

	protected:

		//! fills the listbox with files.
		void fillListBox();

		//! sends the event that the file has been selected.
		void sendSelectedEvent(EGUI_EVENT_TYPE type);

		//! sends the event that the file choose process has been canceld
		void sendCancelEvent();

		Position2d DragStart;
		StringW FileName;
		path FileDirectory;
		path RestoreDirectory;
		path StartDirectory;

		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;
		IGUIListBox* FileBox;
		IGUIEditBox* FileNameText;
		IGUIElement* EventParent;
		IFileSystem* FileSystem;
		IFileList* FileList;
		bool Dragging;
	};
}

#endif

#endif