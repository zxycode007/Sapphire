#ifndef _SAPPHIRE_I_GUI_SPRITE_BANK_
#define _SAPPHIRE_I_GUI_SPRITE_BANK_

#include "SapphireIReferenceCounter.h"
#include "SapphireColorValue.h"
#include "SapphireRectangle.h"

namespace Sapphire
{
	class ITexture;

	//! A single sprite frame.
	struct SGUISpriteFrame
	{
		UINT32 textureNumber;
		UINT32 rectNumber;
	};

	//! A sprite composed of several frames.
	struct SGUISprite
	{
		SGUISprite() : Frames(), frameTime(0) {}

		vector<SGUISpriteFrame>::type Frames;
		UINT32 frameTime;
	};


	//! Sprite bank interface.
	/** See http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=25742&highlight=spritebank
	* for more information how to use the spritebank.
	*/
	class IGUISpriteBank : public virtual IReferenceCounter
	{
	public:

		//! Returns the list of rectangles held by the sprite bank
		virtual vector< rect<SINT32> >::type& getPositions() = 0;

		//! Returns the array of animated sprites within the sprite bank
		virtual vector< SGUISprite >::type& getSprites() = 0;

		//! Returns the number of textures held by the sprite bank
		virtual UINT32 getTextureCount() const = 0;

		//! Gets the texture with the specified index
		virtual ITexture* getTexture(UINT32 index) const = 0;

		//! Adds a texture to the sprite bank
		virtual void addTexture(ITexture* texture) = 0;

		//! Changes one of the textures in the sprite bank
		virtual void setTexture(UINT32 index, ITexture* texture) = 0;

		//! Add the texture and use it for a single non-animated sprite.
		//! The texture and the corresponding rectangle and sprite will all be added to the end of each array.
		//! returns the index of the sprite or -1 on failure
		virtual SINT32 addTextureAsSprite(ITexture* texture) = 0;

		//! clears sprites, rectangles and textures
		virtual void clear() = 0;

		//! Draws a sprite in 2d with position and color
		virtual void draw2DSprite(UINT32 index, const Position2d& pos,
			const rect<SINT32>* clip = 0,
			const ColourValue& color = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 starttime = 0, UINT32 currenttime = 0,
			bool loop = true, bool center = false) = 0;

		//! Draws a sprite batch in 2d using an array of positions and a color
		virtual void draw2DSpriteBatch(const vector<UINT32>::type& indices, const vector<Position2d>::type& pos,
			const rect<SINT32>* clip = 0,
			const ColourValue& color = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 starttime = 0, UINT32 currenttime = 0,
			bool loop = true, bool center = false) = 0;
	};
}

#endif