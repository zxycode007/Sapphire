#ifndef _SAPPHIRE_I_GUI_SPIN_BOX_
#define _SAPPHIRE_I_GUI_SPIN_BOX_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	class IGUIEditBox;

	//! Single line edit box + spin buttons
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_SPINBOX_CHANGED
	*/
	class IGUISpinBox : public IGUIElement
	{
	public:

		//! constructor
		IGUISpinBox(IGUIEnvironment* environment, IGUIElement* parent,
			SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_SPIN_BOX, environment, parent, id, rectangle) {}

		//! Access the edit box used in the spin control
		virtual IGUIEditBox* getEditBox() const = 0;

		//! set the current value of the spinbox
		/** \param val: value to be set in the spinbox */
		virtual void setValue(FLOAT32 val) = 0;

		//! Get the current value of the spinbox
		virtual FLOAT32 getValue() const = 0;

		//! set the range of values which can be used in the spinbox
		/** \param min: minimum value
		\param max: maximum value */
		virtual void setRange(FLOAT32 min, FLOAT32 max) = 0;

		//! get the minimum value which can be used in the spinbox
		virtual FLOAT32 getMin() const = 0;

		//! get the maximum value which can be used in the spinbox
		virtual FLOAT32 getMax() const = 0;

		//! Step size by which values are changed when pressing the spinbuttons
		/** The step size also determines the number of decimal places to display
		\param step: stepsize used for value changes when pressing spinbuttons */
		virtual void setStepSize(FLOAT32 step = 1.f) = 0;

		//! Sets the number of decimal places to display.
		//! Note that this also rounds the range to the same number of decimal places.
		/** \param places: The number of decimal places to display, use -1 to reset */
		virtual void setDecimalPlaces(SINT32 places) = 0;

		//! get the current step size
		virtual FLOAT32 getStepSize() const = 0;
	};
}

#endif