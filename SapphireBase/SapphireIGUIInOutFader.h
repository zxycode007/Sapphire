#ifndef _SAPPHIRE_I_GUI_IN_OUT_FADER_
#define _SAPPHIRE_I_GUI_IN_OUT_FADER_

#include "SapphireIGUIElement.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	//! Element for fading out or in
	/** Here is a small example on how the class is used. In this example we fade
	in from a total red screen in the beginning. As you can see, the fader is not
	only useful for dramatic in and out fading, but also to show that the player
	is hit in a first person shooter game for example.
	\code
	gui::IGUIInOutFader* fader = device->getGUIEnvironment()->addInOutFader();
	fader->setColor(ColourValue(0,255,0,0));
	fader->fadeIn(4000);
	\endcode
	*/
	class IGUIInOutFader : public IGUIElement
	{
	public:

		//! constructor
		IGUIInOutFader(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_IN_OUT_FADER, environment, parent, id, rectangle) {}

		//! Gets the color to fade out to or to fade in from.
		virtual ColourValue getColor() const = 0;

		//! Sets the color to fade out to or to fade in from.
		/** \param color: Color to where it is faded out od from it is faded in. */
		virtual void setColor(ColourValue color) = 0;
		virtual void setColor(ColourValue source, ColourValue dest) = 0;

		//! Starts the fade in process.
		/** In the beginning the whole rect is drawn by the set color
		(black by default) and at the end of the overgiven time the
		color has faded out.
		\param time: Time specifying how long it should need to fade in,
		in milliseconds. */
		virtual void fadeIn(UINT32 time) = 0;

		//! Starts the fade out process.
		/** In the beginning everything is visible, and at the end of
		the time only the set color (black by the fault) will be drawn.
		\param time: Time specifying how long it should need to fade out,
		in milliseconds. */
		virtual void fadeOut(UINT32 time) = 0;

		//! Returns if the fade in or out process is done.
		virtual bool isReady() const = 0;
	};
}

#endif