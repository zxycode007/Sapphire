#ifndef _SAPPHIRE_I_GUI_SRCOLL_BAR_
#define _SAPPHIRE_I_GUI_SRCOLL_BAR_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! Default scroll bar GUI element.
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_SCROLL_BAR_CHANGED
	*/
	class IGUIScrollBar : public IGUIElement
	{
	public:

		//! constructor
		IGUIScrollBar(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_SCROLL_BAR, environment, parent, id, rectangle) {}

		//! sets the maximum value of the scrollbar.
		virtual void setMax(SINT32 max) = 0;
		//! gets the maximum value of the scrollbar.
		virtual SINT32 getMax() const = 0;

		//! sets the minimum value of the scrollbar.
		virtual void setMin(SINT32 min) = 0;
		//! gets the minimum value of the scrollbar.
		virtual SINT32 getMin() const = 0;

		//! gets the small step value
		virtual SINT32 getSmallStep() const = 0;

		//! Sets the small step
		/** That is the amount that the value changes by when clicking
		on the buttons or using the cursor keys. */
		virtual void setSmallStep(SINT32 step) = 0;

		//! gets the large step value
		virtual SINT32 getLargeStep() const = 0;

		//! Sets the large step
		/** That is the amount that the value changes by when clicking
		in the tray, or using the page up and page down keys. */
		virtual void setLargeStep(SINT32 step) = 0;

		//! gets the current position of the scrollbar
		virtual SINT32 getPos() const = 0;

		//! sets the current position of the scrollbar
		virtual void setPos(SINT32 pos) = 0;
	};
}

#endif