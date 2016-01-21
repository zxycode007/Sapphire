#ifndef _SAPPHIRE_I_GUI_IMAGE_
#define _SAPPHIRE_I_GUI_IMAGE_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	//! GUI element displaying an image.
	class IGUIImage : public IGUIElement
	{
	public:

		//! constructor
		IGUIImage(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_IMAGE, environment, parent, id, rectangle) {}

		//! Sets an image texture
		virtual void setImage(ITexture* image) = 0;

		//! Gets the image texture
		virtual ITexture* getImage() const = 0;

		//! Sets the color of the image
		virtual void setColor(ColourValue color) = 0;

		//! Sets if the image should scale to fit the element
		virtual void setScaleImage(bool scale) = 0;

		//! Sets if the image should use its alpha channel to draw itself
		virtual void setUseAlphaChannel(bool use) = 0;

		//! Gets the color of the image
		virtual ColourValue getColor() const = 0;

		//! Returns true if the image is scaled to fit, false if not
		virtual bool isImageScaled() const = 0;

		//! Returns true if the image is using the alpha channel, false if not
		virtual bool isAlphaChannelUsed() const = 0;
	};
}


#endif