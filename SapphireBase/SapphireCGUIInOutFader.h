#ifndef _SAPPHIRE_C_GUI_IN_OUT_FADER_
#define _SAPPHIRE_C_GUI_IN_OUT_FADER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIInOutFader.h"


namespace Sapphire
{
	class CGUIInOutFader : public IGUIInOutFader
	{
	public:

		//! constructor
		CGUIInOutFader(IGUIEnvironment* environment, IGUIElement* parent,
			SINT32 id, rect<SINT32> rectangle);

		//! draws the element and its children
		virtual void draw();

		//! Gets the color to fade out to or to fade in from.
		virtual ColourValue getColor() const;

		//! Sets the color to fade out to or to fade in from.
		virtual void setColor(ColourValue color);
		virtual void setColor(ColourValue source, ColourValue dest);

		//! Starts the fade in process.
		virtual void fadeIn(UINT32 time);

		//! Starts the fade out process.
		virtual void fadeOut(UINT32 time);

		//! Returns if the fade in or out process is done.
		virtual bool isReady() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		enum EFadeAction
		{
			EFA_NOTHING = 0,
			EFA_FADE_IN,
			EFA_FADE_OUT
		};

		UINT32 StartTime;
		UINT32 EndTime;
		EFadeAction Action;

		ColourValue Color[2];
		ColourValue FullColor;
		ColourValue TransColor;
	};
}

#endif




#endif