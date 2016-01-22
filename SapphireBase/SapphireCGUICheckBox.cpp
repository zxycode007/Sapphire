#include "SapphireCGUICheckBox.h"

#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireFont.h"
#include "SapphireOs.h"

namespace Sapphire
{
	//! constructor
	CGUICheckBox::CGUICheckBox(bool checked, IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
		: IGUICheckBox(environment, parent, id, rectangle), checkTime(0), Pressed(false), Checked(checked)
	{
#ifdef _DEBUG
		setDebugName("CGUICheckBox");
#endif

		// this element can be tabbed into
		setTabStop(true);
		setTabOrder(-1);
	}


	//! called if an event happened.
	bool CGUICheckBox::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{
			switch (event.EventType)
			{
			case EET_KEY_INPUT_EVENT:
				if (event.KeyInput.PressedDown &&
					(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
				{
					Pressed = true;
					return true;
				}
				else
					if (Pressed && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
					{
						Pressed = false;
						return true;
					}
					else
						if (!event.KeyInput.PressedDown && Pressed &&
							(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
						{
							Pressed = false;
							if (Parent)
							{
								SEvent newEvent;
								newEvent.EventType = EET_GUI_EVENT;
								newEvent.GUIEvent.Caller = this;
								newEvent.GUIEvent.Element = 0;
								Checked = !Checked;
								newEvent.GUIEvent.EventType = EGET_CHECKBOX_CHANGED;
								Parent->OnEvent(newEvent);
							}
							return true;
						}
				break;
			case EET_GUI_EVENT:
				if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
				{
					if (event.GUIEvent.Caller == this)
						Pressed = false;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
				if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
				{
					Pressed = true;
					checkTime = Timer::getTime();
					Environment->setFocus(this);
					return true;
				}
				else
					if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
					{
						bool wasPressed = Pressed;
						Environment->removeFocus(this);
						Pressed = false;

						if (wasPressed && Parent)
						{
							if (!AbsoluteClippingRect.isPointInside(Position2d(event.MouseInput.X, event.MouseInput.Y)))
							{
								Pressed = false;
								return true;
							}

							SEvent newEvent;
							newEvent.EventType = EET_GUI_EVENT;
							newEvent.GUIEvent.Caller = this;
							newEvent.GUIEvent.Element = 0;
							Checked = !Checked;
							newEvent.GUIEvent.EventType = EGET_CHECKBOX_CHANGED;
							Parent->OnEvent(newEvent);
						}

						return true;
					}
				break;
			default:
				break;
			}
		}

		return IGUIElement::OnEvent(event);
	}


	//! draws the element and its children
	void CGUICheckBox::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		if (skin)
		{
			const SINT32 height = skin->getSize(EGDS_CHECK_BOX_WIDTH);

			rect<SINT32> checkRect(AbsoluteRect.UpperLeftCorner.x,
				((AbsoluteRect.getHeight() - height) / 2) + AbsoluteRect.UpperLeftCorner.y,
				0, 0);

			checkRect.LowerRightCorner.x = checkRect.UpperLeftCorner.x + height;
			checkRect.LowerRightCorner.y = checkRect.UpperLeftCorner.y + height;

			EGUI_DEFAULT_COLOR col = EGDC_GRAY_EDITABLE;
			if (isEnabled())
				col = Pressed ? EGDC_FOCUSED_EDITABLE : EGDC_EDITABLE;
			skin->draw3DSunkenPane(this, skin->getColor(col),
				false, true, checkRect, &AbsoluteClippingRect);

			if (Checked)
			{
				skin->drawIcon(this, EGDI_CHECK_BOX_CHECKED, checkRect.getCenter(),
					checkTime, Timer::getTime(), false, &AbsoluteClippingRect);
			}
			if (Text.size())
			{
				checkRect = AbsoluteRect;
				checkRect.UpperLeftCorner.x += height + 5;

				IGUIFont* font = skin->getFont();
				if (font)
				{
					font->draw(StringUtil::StringWToString(Text), checkRect,
						skin->getColor(isEnabled() ? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT), false, true, &AbsoluteClippingRect);
				}
			}
		}
		IGUIElement::draw();
	}


	//! set if box is checked
	void CGUICheckBox::setChecked(bool checked)
	{
		Checked = checked;
	}


	//! returns if box is checked
	bool CGUICheckBox::isChecked() const
	{
		//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return Checked;
	}


	//! Writes attributes of the element.
	void CGUICheckBox::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUICheckBox::serializeAttributes(out, options);
		out->addBool("Checked", Checked);
	}


	//! Reads attributes of the element
	void CGUICheckBox::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		Checked = in->getAttributeAsBool("Checked");

		IGUICheckBox::deserializeAttributes(in, options);
	}
}
	
#endif