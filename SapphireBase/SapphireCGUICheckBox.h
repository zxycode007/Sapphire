#ifndef _SAPPHIRE_C_GUI_CHECK_BOX_
#define _SAPPHIRE_C_GUI_CHECK_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUICheckBox.h"

namespace Sapphire
{
	class CGUICheckBox : public IGUICheckBox
	{
	public:

		//! constructor
		CGUICheckBox(bool checked, IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! set if box is checked
		virtual void setChecked(bool checked);

		//! returns if box is checked
		virtual bool isChecked() const;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		UINT32 checkTime;
		bool Pressed;
		bool Checked;
	};
}

#endif



#endif