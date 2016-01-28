#ifndef _SAPPHIRE_C_GUI_MESSAGE_BOX_
#define _SAPPHIRE_C_GUI_MESSAGE_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireCGUIWindow.h"
#include "SapphireIGUIStaticText.h"
#include "SapphireIGUIImage.h"

namespace Sapphire
{
	class CGUIMessageBox : public CGUIWindow
	{
	public:

		//! constructor
		CGUIMessageBox(IGUIEnvironment* environment, const wchar_t* caption,
			const wchar_t* text, SINT32 flag,
			IGUIElement* parent, SINT32 id, rect<SINT32> rectangle, ITexture* image = 0);

		//! destructor
		virtual ~CGUIMessageBox();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		void refreshControls();
		void setButton(IGUIButton*& button, bool isAvailable, const rect<SINT32> & btnRect, const wchar_t * text, IGUIElement*& focusMe);

		IGUIButton* OkButton;
		IGUIButton* CancelButton;
		IGUIButton* YesButton;
		IGUIButton* NoButton;
		IGUIStaticText* StaticText;
		IGUIImage * Icon;
		ITexture * IconTexture;

		SINT32 Flags;
		StringW MessageText;
		bool Pressed;
	};
}

#endif

#endif