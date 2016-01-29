#ifndef _SAPPHIRE_C_GUI_MODAL_SCREEN_
#define _SAPPHIRE_C_GUI_MODAL_SCREEN_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIElement.h"
namespace Sapphire
{
	class CGUIModalScreen : public IGUIElement
	{
	public:

		//! constructor
		CGUIModalScreen(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id);

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! Removes a child.
		virtual void removeChild(IGUIElement* child);

		//! Adds a child
		virtual void addChild(IGUIElement* child);


		//! draws the element and its children
		virtual void draw();

		//! Updates the absolute position.
		virtual void updateAbsolutePosition();

		//! Modalscreen is not a typical element, but rather acts like a state for it's children.
		//! isVisible is overriden to give this a useful behavior, so that a modal will no longer
		//! be active when its parent is invisible or all its children are invisible.
		virtual bool isVisible() const;

		//! Modals are infinite so every point is inside
		virtual bool isPointInside(const Position2d& point) const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	protected:
		virtual bool canTakeFocus(IGUIElement* target) const;

	private:

		UINT32 MouseDownTime;
	};
}
#endif

#endif