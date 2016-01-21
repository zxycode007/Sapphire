#ifndef _SAPPHIRE_I_GUI_IMAGE_LIST_
#define _SAPPHIRE_I_GUI_IMAGE_LIST_

#include "SapphireIGUIElement.h"
#include "SapphireRectangle.h"

namespace Sapphire
{

	//! Font interface.
	class IGUIImageList : public virtual IReferenceCounter
	{
	public:

		//! Destructor
		virtual ~IGUIImageList() {};

		//! Draws an image and clips it to the specified rectangle if wanted
		//! \param index: Index of the image
		//! \param destPos: Position of the image to draw
		//! \param clip: Optional pointer to a rectalgle against which the text will be clipped.
		//! If the pointer is null, no clipping will be done.
		virtual void draw(SINT32 index, const Position2d& destPos,
			const rect<SINT32>* clip = 0) = 0;

		//! Returns the count of Images in the list.
		//! \return Returns the count of Images in the list.
		virtual SINT32 getImageCount() const = 0;

		//! Returns the size of the images in the list.
		//! \return Returns the size of the images in the list.
		virtual  dimension2d<SINT32> getImageSize() const = 0;
	};
}

#endif