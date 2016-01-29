#ifndef _SAPPHIRE_C_GUI_TOOL_BAR_
#define _SAPPHIRE_C_GUI_TOOL_BAR_


#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIToolbar.h"
namespace Sapphire
{
	//! Stays at the top of its parent like the menu bar and contains tool buttons
	class CGUIToolBar : public IGUIToolBar
	{
	public:

		//! constructor
		CGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Updates the absolute position.
		virtual void updateAbsolutePosition();

		//! Adds a button to the tool bar
		virtual IGUIButton* addButton(SINT32 id = -1, const wchar_t* text = 0, const wchar_t* tooltiptext = 0,
			ITexture* img = 0, ITexture* pressed = 0,
			bool isPushButton = false, bool useAlphaChannel = false);

	private:

		SINT32 ButtonX;
	};
}

#endif

#endif