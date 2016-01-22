#ifndef _SAPPHIRE_C_GUI_IMAGE_
#define _SAPPHIRE_C_GUI_IMAGE_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_

#include "SapphireIGUIImage.h"

namespace Sapphire
{
	class CGUIImage : public IGUIImage
	{
	public:

		//! constructor
		CGUIImage(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! destructor
		virtual ~CGUIImage();

		//! sets an image
		virtual void setImage(ITexture* image);

		//! Gets the image texture
		virtual ITexture* getImage() const;

		//! sets the color of the image
		virtual void setColor(ColourValue color);

		//! sets if the image should scale to fit the element
		virtual void setScaleImage(bool scale);

		//! draws the element and its children
		virtual void draw();

		//! sets if the image should use its alpha channel to draw itself
		virtual void setUseAlphaChannel(bool use);

		//! Gets the color of the image
		virtual ColourValue getColor() const;

		//! Returns true if the image is scaled to fit, false if not
		virtual bool isImageScaled() const;

		//! Returns true if the image is using the alpha channel, false if not
		virtual bool isAlphaChannelUsed() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:
		ITexture* Texture;
		ColourValue Color;
		bool UseAlphaChannel;
		bool ScaleImage;

	};

}
#endif
#endif