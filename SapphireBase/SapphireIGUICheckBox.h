#ifndef _SAPPHIRE_I_GUI_CHECK_BOX_
#define _SAPPHIRE_I_GUI_CHECK_BOX_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! GUI Check box interface.
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_CHECKBOX_CHANGED
	*/
	class IGUICheckBox : public IGUIElement
	{
	public:

		//! constructor
		IGUICheckBox(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_CHECK_BOX, environment, parent, id, rectangle) {}

		//! Set if box is checked.
		virtual void setChecked(bool checked) = 0;

		//! Returns true if box is checked.
		virtual bool isChecked() const = 0;
	};
}

#endif