#ifndef _SAPPHIRE_C_GUI_SPRITE_BANK_
#define _SAPPHIRE_C_GUI_SPRITE_BANK_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISpriteBank.h"

namespace Sapphire
{
	class IVideoDriver;
	class ITexture;
	class IGUIEnvironment;

	//! Sprite bank interface.
	class CGUISpriteBank : public IGUISpriteBank
	{
	public:

		CGUISpriteBank(IGUIEnvironment* env);
		virtual ~CGUISpriteBank();

		virtual vector< rect<SINT32> >::type& getPositions();
		virtual vector< SGUISprite >::type& getSprites();

		virtual UINT32 getTextureCount() const;
		virtual ITexture* getTexture(UINT32 index) const;
		virtual void addTexture(ITexture* texture);
		virtual void setTexture(UINT32 index, ITexture* texture);

		//! Add the texture and use it for a single non-animated sprite.
		virtual SINT32 addTextureAsSprite(ITexture* texture);

		//! clears sprites, rectangles and textures
		virtual void clear();

		//! Draws a sprite in 2d with position and color
		virtual void draw2DSprite(UINT32 index, const Position2d& pos, const rect<SINT32>* clip = 0,
			const ColourValue& color = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 starttime = 0, UINT32 currenttime = 0, bool loop = true, bool center = false);

		//! Draws a sprite batch in 2d using an array of positions and a color
		virtual void draw2DSpriteBatch(const vector<UINT32>::type& indices, const vector<Position2d>::type& pos,
			const rect<SINT32>* clip = 0,
			const ColourValue& color = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 starttime = 0, UINT32 currenttime = 0,
			bool loop = true, bool center = false);

	protected:

		struct SDrawBatch
		{
			vector<Position2d>::type positions;
			vector<recti>::type sourceRects;
			UINT32 textureNumber;
		};

		vector<SGUISprite>::type Sprites;
		vector< rect<SINT32> >::type Rectangles;
		vector<ITexture*>::type Textures;
		IGUIEnvironment* Environment;
		IVideoDriver* Driver;

	};
}

#endif

#endif