#include "SapphireCGUIModalScreen.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEnvironment.h"
#include "SapphireOS.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIGUISkin.h"
namespace Sapphire
{
	//! constructor
	CGUIModalScreen::CGUIModalScreen(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id)
		: IGUIElement(EGUIET_MODAL_SCREEN, environment, parent, id, recti(0, 0, parent->getAbsolutePosition().getWidth(), parent->getAbsolutePosition().getHeight())),
		MouseDownTime(0)
	{
#ifdef _DEBUG
		setDebugName("CGUIModalScreen");
#endif
		setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);

		// this element is a tab group
		setTabGroup(true);
	}

	bool CGUIModalScreen::canTakeFocus(IGUIElement* target) const
	{
		return (target && ((const IGUIElement*)target == this // this element can take it
			|| isMyChild(target)    // own children also
			|| (target->getType() == EGUIET_MODAL_SCREEN)	// other modals also fine (is now on top or explicitely requested)
			|| (target->getParent() && target->getParent()->getType() == EGUIET_MODAL_SCREEN)))   // children of other modals will do
			;
	}

	bool CGUIModalScreen::isVisible() const
	{
		// any parent invisible?
		IGUIElement * parentElement = getParent();
		while (parentElement)
		{
			if (!parentElement->isVisible())
				return false;
			parentElement = parentElement->getParent();
		}

		// if we have no children then the modal is probably abused as a way to block input
		if (Children.empty())
		{
			return IGUIElement::isVisible();
		}

		// any child visible?
		bool visible = false;
		list<IGUIElement*>::const_iterator it = Children.begin();
		for (; it != Children.end(); ++it)
		{
			if ((*it)->isVisible())
			{
				visible = true;
				break;
			}
		}
		return visible;
	}

	bool CGUIModalScreen::isPointInside(const Position2d& point) const
	{
		return true;
	}

	//! called if an event happened.
	bool CGUIModalScreen::OnEvent(const SEvent& event)
	{
		if (!isEnabled() || !isVisible())
			return IGUIElement::OnEvent(event);

		switch (event.EventType)
		{
		case EET_GUI_EVENT:
			switch (event.GUIEvent.EventType)
			{
			case EGET_ELEMENT_FOCUSED:
				if (event.GUIEvent.Caller == this && isMyChild(event.GUIEvent.Element))
				{
					Environment->removeFocus(0);	// can't setFocus otherwise at it still has focus here
					Environment->setFocus(event.GUIEvent.Element);
					MouseDownTime = Timer::getTime();
					return true;
				}
				if (!canTakeFocus(event.GUIEvent.Caller))
				{
					if (!Children.empty())
						Environment->setFocus(*(Children.begin()));
					else
						Environment->setFocus(this);
				}
				IGUIElement::OnEvent(event);
				return false;
			case EGET_ELEMENT_FOCUS_LOST:
				if (!canTakeFocus(event.GUIEvent.Element))
				{
					if (isMyChild(event.GUIEvent.Caller))
					{
						if (!Children.empty())
							Environment->setFocus(*(Children.begin()));
						else
							Environment->setFocus(this);
					}
					else
					{
						MouseDownTime = Timer::getTime();
					}
					return true;
				}
				else
				{
					return IGUIElement::OnEvent(event);
				}
			case EGET_ELEMENT_CLOSED:
				// do not interfere with children being removed
				return IGUIElement::OnEvent(event);
			default:
				break;
			}
			break;
		case EET_MOUSE_INPUT_EVENT:
			if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				MouseDownTime = Timer::getTime();
			}
		default:
			break;
		}

		IGUIElement::OnEvent(event);	// anyone knows why events are passed on here? Causes p.e. problems when this is child of a CGUIWindow.

		return true; // absorb everything else
	}


	//! draws the element and its children
	void CGUIModalScreen::draw()
	{
		IGUISkin *skin = Environment->getSkin();

		if (!skin)
			return;

		UINT32 now = Timer::getTime();
		if (now - MouseDownTime < 300 && (now / 70) % 2)
		{
			list<IGUIElement*>::iterator it = Children.begin();
			rect<SINT32> r;
			ColourValue c = Environment->getSkin()->getColor(EGDC_3D_HIGH_LIGHT);

			for (; it != Children.end(); ++it)
			{
				if ((*it)->isVisible())
				{
					r = (*it)->getAbsolutePosition();
					r.LowerRightCorner.x += 1;
					r.LowerRightCorner.y += 1;
					r.UpperLeftCorner.x -= 1;
					r.UpperLeftCorner.y -= 1;

					skin->draw2DRectangle(this, c, r, &AbsoluteClippingRect);
				}
			}
		}

		IGUIElement::draw();
	}


	//! Removes a child.
	void CGUIModalScreen::removeChild(IGUIElement* child)
	{
		IGUIElement::removeChild(child);

		if (Children.empty())
		{
			remove();
		}
	}


	//! adds a child
	void CGUIModalScreen::addChild(IGUIElement* child)
	{
		IGUIElement::addChild(child);
		Environment->setFocus(child);
	}


	void CGUIModalScreen::updateAbsolutePosition()
	{
		rect<SINT32> parentRect(0, 0, 0, 0);

		if (Parent)
		{
			parentRect = Parent->getAbsolutePosition();
			RelativeRect.UpperLeftCorner.x = 0;
			RelativeRect.UpperLeftCorner.y = 0;
			RelativeRect.LowerRightCorner.x = parentRect.getWidth();
			RelativeRect.LowerRightCorner.y = parentRect.getHeight();
		}

		IGUIElement::updateAbsolutePosition();
	}


	//! Writes attributes of the element.
	void CGUIModalScreen::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUIElement::serializeAttributes(out, options);
	}

	//! Reads attributes of the element
	void CGUIModalScreen::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		IGUIElement::deserializeAttributes(in, options);
	}

}

#endif