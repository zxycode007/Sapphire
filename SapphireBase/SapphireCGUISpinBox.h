#ifndef _SAPPHIRE_C_GUI_SPIN_BOX_
#define _SAPPHIRE_C_GUI_SPIN_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISpinBox.h"
namespace Sapphire
{
	class IGUIEditBox;
	class IGUIButton;

	class CGUISpinBox : public IGUISpinBox
	{
	public:

		//! constructor
		CGUISpinBox(const wchar_t* text, bool border, IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, const rect<SINT32>& rectangle);

		//! destructor
		virtual ~CGUISpinBox();

		//! Access the edit box used in the spin control
		/** \param enable: If set to true, the override color, which can be set
		with IGUIEditBox::setOverrideColor is used, otherwise the
		EGDC_BUTTON_TEXT color of the skin. */
		virtual IGUIEditBox* getEditBox() const;

		//! set the current value of the spinbox
		/** \param val: value to be set in the spinbox */
		virtual void setValue(FLOAT32 val);

		//! Get the current value of the spinbox
		virtual FLOAT32 getValue() const;

		//! set the range of values which can be used in the spinbox
		/** \param min: minimum value
		\param max: maximum value */
		virtual void setRange(FLOAT32 min, FLOAT32 max);

		//! get the minimum value which can be used in the spinbox
		virtual FLOAT32 getMin() const;

		//! get the maximum value which can be used in the spinbox
		virtual FLOAT32 getMax() const;

		//! step size by which values are changed when pressing the spin buttons
		/** \param step: stepsize used for value changes when pressing spin buttons */
		virtual void setStepSize(FLOAT32 step = 1.f);

		//! returns the step size
		virtual FLOAT32 getStepSize() const;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! Draws the element and its children.
		virtual void draw();

		//! Sets the new caption of the element
		virtual void setText(const wchar_t* text);

		//! Returns caption of this element.
		virtual const wchar_t* getText() const;

		//! Sets the number of decimal places to display.
		//! Note that this also rounds the range to the same number of decimal places.
		/** \param places: The number of decimal places to display, use -1 to reset */
		virtual void setDecimalPlaces(SINT32 places);

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	protected:
		virtual void verifyValueRange();
		void refreshSprites();

		IGUIEditBox * EditBox;
		IGUIButton * ButtonSpinUp;
		IGUIButton * ButtonSpinDown;
		ColourValue CurrentIconColor;
		FLOAT32 StepSize;
		FLOAT32 RangeMin;
		FLOAT32 RangeMax;

		StringW FormatString;
		SINT32 DecimalPlaces;
	};
}

#endif

#endif