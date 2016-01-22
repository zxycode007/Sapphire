#ifndef _SAPPHIRE_C_GUI_IMAGE_LIST_
#define _SAPPHIRE_C_GUI_IMAGE_LIST_

#include "SapphireIGUIImageList.h"
#include "SapphireIVideoDriver.h"

namespace Sapphire
{
	class CGUIImageList : public IGUIImageList
	{
	public:

		//! constructor
		CGUIImageList(IVideoDriver* Driver);

		//! destructor
		virtual ~CGUIImageList();

		//! Creates the image list from texture.
		//! \param texture: The texture to use
		//! \param imageSize: Size of a single image
		//! \param useAlphaChannel: true if the alpha channel from the texture should be used
		//! \return
		//! true if the image list was created
		bool createImageList(
			ITexture*			texture,
			dimension2d<SINT32>	imageSize,
			bool							useAlphaChannel);

		//! Draws an image and clips it to the specified rectangle if wanted
		//! \param index: Index of the image
		//! \param destPos: Position of the image to draw
		//! \param clip: Optional pointer to a rectalgle against which the text will be clipped.
		//! If the pointer is null, no clipping will be done.
		virtual void draw(SINT32 index, const Position2d& destPos,
			const rect<SINT32>* clip = 0);

		//! Returns the count of Images in the list.
		//! \return Returns the count of Images in the list.
		virtual SINT32 getImageCount() const
		{
			return ImageCount;
		}

		//! Returns the size of the images in the list.
		//! \return Returns the size of the images in the list.
		virtual dimension2d<SINT32> getImageSize() const
		{
			return ImageSize;
		}

	private:

		IVideoDriver*		Driver;
		ITexture*			Texture;
		SINT32							ImageCount;
		dimension2d<SINT32>	ImageSize;
		SINT32							ImagesPerRow;
		bool							UseAlphaChannel;
	};
}

#endif