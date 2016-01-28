#ifndef _SAPPHIRE_C_GUI_MENU_
#define _SAPPHIRE_C_GUI_MENU_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireCGUIContextMenu.h"
namespace Sapphire
{
	//! GUI menu interface.
	class CGUIMenu : public CGUIContextMenu
	{
	public:

		//! constructor
		CGUIMenu(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! draws the element and its children
		virtual void draw();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! Updates the absolute position.
		virtual void updateAbsolutePosition();

	protected:

		virtual void recalculateSize();

		//! returns the item highlight-area
		virtual rect<SINT32> getHRect(const SItem& i, const rect<SINT32>& absolute) const;

		//! Gets drawing rect of Item
		virtual rect<SINT32> getRect(const SItem& i, const rect<SINT32>& absolute) const;
	};
}


#endif


#endif