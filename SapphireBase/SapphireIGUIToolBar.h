#ifndef _SAPPHIRE_I_GUI_TOOLBAR_
#define _SAPPHIRE_I_GUI_TOOLBAR_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	class ITexture;

	class IGUIButton;

	//! Stays at the top of its parent like the menu bar and contains tool buttons
	class IGUIToolBar : public IGUIElement
	{
	public:

		//! constructor
		IGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_TOOL_BAR, environment, parent, id, rectangle) {}

		//! Adds a button to the tool bar
		virtual IGUIButton* addButton(SINT32 id = -1, const wchar_t* text = 0, const wchar_t* tooltiptext = 0,
			ITexture* img = 0, ITexture* pressedimg = 0,
			bool isPushButton = false, bool useAlphaChannel = false) = 0;
	};
}


#endif