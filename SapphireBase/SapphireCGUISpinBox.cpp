#include "SapphireCGUISpinBox.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireCGUIEditBox.h"
#include "SapphireCGUIButton.h"
#include "SapphireIGUIEnvironment.h"
#include  "SapphireIEventRecevier.h"

namespace Sapphire
{
	//! constructor
	CGUISpinBox::CGUISpinBox(const wchar_t* text, bool border, IGUIEnvironment* environment,
		IGUIElement* parent, SINT32 id, const rect<SINT32>& rectangle)
		: IGUISpinBox(environment, parent, id, rectangle),
		EditBox(0), ButtonSpinUp(0), ButtonSpinDown(0), StepSize(1.f),
		RangeMin(-FLT_MAX), RangeMax(FLT_MAX), FormatString(L"%f"),
		DecimalPlaces(-1)
	{
#ifdef _DEBUG
		setDebugName("CGUISpinBox");
#endif

		CurrentIconColor = ColourValue::getColourValue(255, 255, 255, 255);
		SINT32 ButtonWidth = 16;

		ButtonSpinDown = Environment->addButton(
			rect<SINT32>(rectangle.getWidth() - ButtonWidth, rectangle.getHeight() / 2 + 1,
			rectangle.getWidth(), rectangle.getHeight()), this);
		ButtonSpinDown->grab();
		ButtonSpinDown->setSubElement(true);
		ButtonSpinDown->setTabStop(false);
		ButtonSpinDown->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_LOWERRIGHT);

		ButtonSpinUp = Environment->addButton(
			rect<SINT32>(rectangle.getWidth() - ButtonWidth, 0,
			rectangle.getWidth(), rectangle.getHeight() / 2), this);
		ButtonSpinUp->grab();
		ButtonSpinUp->setSubElement(true);
		ButtonSpinUp->setTabStop(false);
		ButtonSpinUp->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_CENTER);

		const rect<SINT32> rectEdit(0, 0, rectangle.getWidth() - ButtonWidth - 1, rectangle.getHeight());
		EditBox = Environment->addEditBox(text, rectEdit, border, this, -1);
		EditBox->grab();
		EditBox->setSubElement(true);
		EditBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);

		refreshSprites();
	}


	//! destructor
	CGUISpinBox::~CGUISpinBox()
	{
		if (ButtonSpinUp)
			ButtonSpinUp->drop();
		if (ButtonSpinDown)
			ButtonSpinDown->drop();
		if (EditBox)
			EditBox->drop();
	}

	void CGUISpinBox::refreshSprites()
	{
		IGUISpriteBank *sb = 0;
		if (Environment && Environment->getSkin())
		{
			sb = Environment->getSkin()->getSpriteBank();
		}

		if (sb)
		{
			IGUISkin * skin = Environment->getSkin();
			CurrentIconColor = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);
			ButtonSpinDown->setSpriteBank(sb);
			ButtonSpinDown->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_SMALL_CURSOR_DOWN), CurrentIconColor);
			ButtonSpinDown->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_SMALL_CURSOR_DOWN), CurrentIconColor);
			ButtonSpinUp->setSpriteBank(sb);
			ButtonSpinUp->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_SMALL_CURSOR_UP), CurrentIconColor);
			ButtonSpinUp->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_SMALL_CURSOR_UP), CurrentIconColor);
		}
		else
		{
			ButtonSpinDown->setText(L"-");
			ButtonSpinUp->setText(L"+");
		}
	}

	IGUIEditBox* CGUISpinBox::getEditBox() const
	{
		return EditBox;
	}


	void CGUISpinBox::setValue(FLOAT32 val)
	{
		wchar_t str[100];

		swprintf(str, 99, FormatString.c_str(), val);
		EditBox->setText(str);
		verifyValueRange();
	}


	FLOAT32 CGUISpinBox::getValue() const
	{
		const wchar_t* val = EditBox->getText();
		if (!val)
			return 0.f;
		String tmp = StringUtil::StringWToString((val));
		return fast_atof(tmp.c_str());
	}


	void CGUISpinBox::setRange(FLOAT32 min, FLOAT32 max)
	{
		if (max<min)
			swap(min, max);
		RangeMin = min;
		RangeMax = max;

		// we have to round the range - otherwise we can get into an infinte setValue/verifyValueRange cycle.
		wchar_t str[100];
		swprintf(str, 99, FormatString.c_str(), RangeMin);
		//RangeMin = core::fast_atof(core::stringc(str).c_str());
		RangeMin = fast_atof(StringUtil::StringWToString((str)).c_str());
		swprintf(str, 99, FormatString.c_str(), RangeMax);
		//RangeMax = core::fast_atof(core::stringc(str).c_str());
		RangeMax = fast_atof(StringUtil::StringWToString((str)).c_str());

		verifyValueRange();
	}


	FLOAT32 CGUISpinBox::getMin() const
	{
		return RangeMin;
	}


	FLOAT32 CGUISpinBox::getMax() const
	{
		return RangeMax;
	}


	FLOAT32 CGUISpinBox::getStepSize() const
	{
		return StepSize;
	}


	void CGUISpinBox::setStepSize(FLOAT32 step)
	{
		StepSize = step;
	}


	//! Sets the number of decimal places to display.
	void CGUISpinBox::setDecimalPlaces(SINT32 places)
	{
		DecimalPlaces = places;
		if (places == -1)
			FormatString = L"%f";
		else
		{
			FormatString = L"%.";
			FormatString += places;
			FormatString += L"f";
		}
		setRange(RangeMin, RangeMax);
		setValue(getValue());
	}


	bool CGUISpinBox::OnEvent(const SEvent& event)
	{
		if (IsEnabled)
		{
			bool changeEvent = false;
			switch (event.EventType)
			{
			case EET_MOUSE_INPUT_EVENT:
				switch (event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
				{
					FLOAT32 val = getValue() + (StepSize * (event.MouseInput.Wheel < 0 ? -1.f : 1.f));
					setValue(val);
					changeEvent = true;
				}
				break;
				default:
					break;
				}
				break;

			case EET_GUI_EVENT:
				if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
				{
					if (event.GUIEvent.Caller == ButtonSpinUp)
					{
						FLOAT32 val = getValue();
						val += StepSize;
						setValue(val);
						changeEvent = true;
					}
					else if (event.GUIEvent.Caller == ButtonSpinDown)
					{
						FLOAT32 val = getValue();
						val -= StepSize;
						setValue(val);
						changeEvent = true;
					}
				}
				if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED || event.GUIEvent.EventType == EGET_EDITBOX_ENTER)
				{
					if (event.GUIEvent.Caller == EditBox)
					{
						verifyValueRange();
						changeEvent = true;
					}
				}
				break;
			default:
				break;
			}

			if (changeEvent)
			{
				SEvent e;
				e.EventType = EET_GUI_EVENT;
				e.GUIEvent.Caller = this;
				e.GUIEvent.Element = 0;

				e.GUIEvent.EventType = EGET_SPINBOX_CHANGED;
				if (Parent)
					Parent->OnEvent(e);
				return true;
			}
		}

		return IGUIElement::OnEvent(event);
	}


	void CGUISpinBox::draw()
	{
		if (!isVisible())
			return;

		IGUISkin* skin = Environment->getSkin();
		if (!skin)
			return;

		ColourValue iconColor = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);
		if (iconColor != CurrentIconColor)
		{
			refreshSprites();
		}

		IGUISpinBox::draw();
	}

	void CGUISpinBox::verifyValueRange()
	{
		FLOAT32 val = getValue();
		if (val + EPSILON_E6 < RangeMin)//if (val + ROUNDING_ERROR_FLOAT32 < RangeMin)
			val = RangeMin;
		else if (val - EPSILON_E6 > RangeMax)
			val = RangeMax;
		else
			return;

		setValue(val);
	}


	//! Sets the new caption of the element
	void CGUISpinBox::setText(const wchar_t* text)
	{
		EditBox->setText(text);
		setValue(getValue());
		verifyValueRange();
	}


	//! Returns caption of this element.
	const wchar_t* CGUISpinBox::getText() const
	{
		return EditBox->getText();
	}


	//! Writes attributes of the element.
	void CGUISpinBox::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		IGUIElement::serializeAttributes(out, options);
		out->addFloat("Min", getMin());
		out->addFloat("Max", getMax());
		out->addFloat("Step", getStepSize());
		out->addInt("DecimalPlaces", DecimalPlaces);
	}


	//! Reads attributes of the element
	void CGUISpinBox::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		IGUIElement::deserializeAttributes(in, options);
		setRange(in->getAttributeAsFloat("Min"), in->getAttributeAsFloat("Max"));
		setStepSize(in->getAttributeAsFloat("Step"));
		setDecimalPlaces(in->getAttributeAsInt("DecimalPlaces"));
	}

}

#endif