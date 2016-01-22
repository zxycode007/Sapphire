#include "SapphireCGUIImageList.h"

namespace Sapphire
{
	//! constructor
	CGUIImageList::CGUIImageList(IVideoDriver* driver)
		: Driver(driver),
		Texture(0),
		ImageCount(0),
		ImageSize(0, 0),
		ImagesPerRow(0),
		UseAlphaChannel(false)
	{
#ifdef _DEBUG
		setDebugName("CGUIImageList");
#endif

		if (Driver)
		{
			Driver->grab();
		}
	}



	//! destructor
	CGUIImageList::~CGUIImageList()
	{
		if (Driver)
		{
			Driver->drop();
		}

		if (Texture)
		{
			Texture->drop();
		}
	}


	//! Creates the image list from texture.
	bool CGUIImageList::createImageList(ITexture* texture,
		dimension2d<SINT32> imageSize,
		bool useAlphaChannel)
	{
		if (!texture)
		{
			return false;
		}

		Texture = texture;
		Texture->grab();

		ImageSize = imageSize;

		ImagesPerRow = Texture->getSize().Width / ImageSize.Width;
		ImageCount = ImagesPerRow * Texture->getSize().Height / ImageSize.Height;

		UseAlphaChannel = useAlphaChannel;

		return true;
	}

	//! Draws an image and clips it to the specified rectangle if wanted
	void CGUIImageList::draw(SINT32 index, const Position2d& destPos,
		const rect<SINT32>* clip /*= 0*/)
	{
		rect<SINT32> sourceRect;

		if (!Driver || index < 0 || index >= ImageCount)
		{
			return;
		}

		sourceRect.UpperLeftCorner.x = (index % ImagesPerRow) * ImageSize.Width;
		sourceRect.UpperLeftCorner.y = (index / ImagesPerRow) * ImageSize.Height;
		sourceRect.LowerRightCorner.x = sourceRect.UpperLeftCorner.x + ImageSize.Width;
		sourceRect.LowerRightCorner.y = sourceRect.UpperLeftCorner.y + ImageSize.Height;

		Driver->draw2DImage(Texture, destPos, sourceRect, clip,
			ColourValue(255, 255, 255, 255), UseAlphaChannel);
	}
}