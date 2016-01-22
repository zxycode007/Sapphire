#ifndef _SAPPHIRE_I_GUI_COLOR_SELECT_DIALOG_
#define _SAPPHIRE_I_GUI_COLOR_SELECT_DIALOG_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! Standard color chooser dialog.
	class IGUIColorSelectDialog : public IGUIElement
	{
	public:

		//! constructor
		IGUIColorSelectDialog(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_COLOR_SELECT_DIALOG, environment, parent, id, rectangle) {}
	};

}

#endif