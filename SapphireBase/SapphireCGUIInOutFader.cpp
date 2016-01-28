#include "SapphireCGUIInOutFader.h"
#ifdef _SAPPHIRE_C_GUI_IN_OUT_FADER_
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireOS.h"

namespace Sapphire
{


	//! constructor
	CGUIInOutFader::CGUIInOutFader(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
		: IGUIInOutFader(environment, parent, id, rectangle)
	{
#ifdef _DEBUG
		setDebugName("CGUIInOutFader");
#endif

		Action = EFA_NOTHING;
		StartTime = 0;
		EndTime = 0;

		setColor(ColourValue(0, 0, 0, 0));
	}


	//! draws the element and its children
	void CGUIInOutFader::draw()
	{
		if (!IsVisible || !Action)
			return;

		UINT32 now =Timer::getTime();
		if (now > EndTime && Action == EFA_FADE_IN)
		{
			Action = EFA_NOTHING;
			return;
		}

		IVideoDriver* driver = Environment->getVideoDriver();

		if (driver)
		{
			FLOAT32 d;

			if (now > EndTime)
				d = 0.0f;
			else
				d = (EndTime - now) / (FLOAT32)(EndTime - StartTime);

			ColourValue newCol = FullColor.getInterpolated(TransColor, d);
			driver->draw2DRectangle(newCol, AbsoluteRect, &AbsoluteClippingRect);
		}

		IGUIElement::draw();
	}


	//! Gets the color to fade out to or to fade in from.
	ColourValue CGUIInOutFader::getColor() const
	{
		return Color[1];
	}


	//! Sets the color to fade out to or to fade in from.
	void CGUIInOutFader::setColor(ColourValue color)
	{
		ColourValue s = color;
		ColourValue d = color;

		s.setAlpha(255);
		d.setAlpha(0);
		setColor(s, d);

		/*
		Color[0] = color;

		FullColor = Color[0];
		TransColor = Color[0];

		if (Action == EFA_FADE_OUT)
		{
		FullColor.setAlpha(0);
		TransColor.setAlpha(255);
		}
		else
		if (Action == EFA_FADE_IN)
		{
		FullColor.setAlpha(255);
		TransColor.setAlpha(0);
		}
		*/
	}


	void CGUIInOutFader::setColor(ColourValue source, ColourValue dest)
	{
		Color[0] = source;
		Color[1] = dest;

		if (Action == EFA_FADE_OUT)
		{
			FullColor = Color[1];
			TransColor = Color[0];
		}
		else
			if (Action == EFA_FADE_IN)
			{
				FullColor = Color[0];
				TransColor = Color[1];
			}

	}


	//! Returns if the fade in or out process is done.
	bool CGUIInOutFader::isReady() const
	{
		UINT32 now =Timer::getTime();
		bool ret = (now > EndTime);
		
		return ret;
	}


	//! Starts the fade in process.
	void CGUIInOutFader::fadeIn(UINT32 time)
	{
		StartTime =Timer::getTime();
		EndTime = StartTime + time;
		Action = EFA_FADE_IN;
		setColor(Color[0], Color[1]);
	}


	//! Starts the fade out process.
	void CGUIInOutFader::fadeOut(UINT32 time)
	{
		StartTime =Timer::getTime();
		EndTime = StartTime + time;
		Action = EFA_FADE_OUT;
		setColor(Color[0], Color[1]);
	}


	//! Writes attributes of the element.
	void CGUIInOutFader::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
	{
		IGUIInOutFader::serializeAttributes(out, options);

		out->addColor("FullColor", FullColor);
		out->addColor("TransColor", TransColor);

	}


	//! Reads attributes of the element
	void CGUIInOutFader::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
	{
		IGUIInOutFader::deserializeAttributes(in, options);

		FullColor = in->getAttributeAsColor("FullColor");
		TransColor = in->getAttributeAsColor("TransColor");
	}
}

#endif