#include "SapphireCGUISpriteBank.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireITexture.h"

namespace Sapphire
{

	CGUISpriteBank::CGUISpriteBank(IGUIEnvironment* env) :
		Environment(env), Driver(0)
	{
#ifdef _DEBUG
		setDebugName("CGUISpriteBank");
#endif

		if (Environment)
		{
			Driver = Environment->getVideoDriver();
			if (Driver)
				Driver->grab();
		}
	}


	CGUISpriteBank::~CGUISpriteBank()
	{
		// drop textures
		for (UINT32 i = 0; i<Textures.size(); ++i)
			if (Textures[i])
				Textures[i]->drop();

		// drop video driver
		if (Driver)
			Driver->drop();
	}


	vector< rect<SINT32> >::type& CGUISpriteBank::getPositions()
	{
		return Rectangles;
	}


	vector< SGUISprite >::type& CGUISpriteBank::getSprites()
	{
		return Sprites;
	}


	UINT32 CGUISpriteBank::getTextureCount() const
	{
		return Textures.size();
	}


	ITexture* CGUISpriteBank::getTexture(UINT32 index) const
	{
		if (index < Textures.size())
			return Textures[index];
		else
			return 0;
	}


	void CGUISpriteBank::addTexture(ITexture* texture)
	{
		if (texture)
			texture->grab();

		Textures.push_back(texture);
	}


	void CGUISpriteBank::setTexture(UINT32 index, ITexture* texture)
	{
		while (index >= Textures.size())
			Textures.push_back(0);

		if (texture)
			texture->grab();

		if (Textures[index])
			Textures[index]->drop();

		Textures[index] = texture;
	}


	//! clear everything
	void CGUISpriteBank::clear()
	{
		// drop textures
		for (UINT32 i = 0; i<Textures.size(); ++i)
			if (Textures[i])
				Textures[i]->drop();
		Textures.clear();
		Sprites.clear();
		Rectangles.clear();
	}

	//! Add the texture and use it for a single non-animated sprite.
	SINT32 CGUISpriteBank::addTextureAsSprite(ITexture* texture)
	{
		if (!texture)
			return -1;

		addTexture(texture);
		UINT32 textureIndex = getTextureCount() - 1;

		UINT32 rectangleIndex = Rectangles.size();
		Rectangles.push_back(rect<SINT32>(0, 0, texture->getOriginalSize().Width, texture->getOriginalSize().Height));

		SGUISprite sprite;
		sprite.frameTime = 0;

		SGUISpriteFrame frame;
		frame.textureNumber = textureIndex;
		frame.rectNumber = rectangleIndex;
		sprite.Frames.push_back(frame);

		Sprites.push_back(sprite);

		return Sprites.size() - 1;
	}

	//! draws a sprite in 2d with scale and color
	void CGUISpriteBank::draw2DSprite(UINT32 index, const Position2d& pos,
		const rect<SINT32>* clip, const ColourValue& color,
		UINT32 starttime, UINT32 currenttime, bool loop, bool center)
	{
		if (index >= Sprites.size() || Sprites[index].Frames.empty())
			return;

		// work out frame number
		UINT32 frame = 0;
		if (Sprites[index].frameTime)
		{
			UINT32 f = ((currenttime - starttime) / Sprites[index].frameTime);
			if (loop)
				frame = f % Sprites[index].Frames.size();
			else
				frame = (f >= Sprites[index].Frames.size()) ? Sprites[index].Frames.size() - 1 : f;
		}

		const ITexture* tex = Textures[Sprites[index].Frames[frame].textureNumber];
		if (!tex)
			return;

		const UINT32 rn = Sprites[index].Frames[frame].rectNumber;
		if (rn >= Rectangles.size())
			return;

		const rect<SINT32>& r = Rectangles[rn];

		if (center)
		{
			Position2d p = pos;
			p = Vector2(p.x - (r.getSize() / 2).Width, p.y - (r.getSize() / 2).Height);
			Driver->draw2DImage(tex, p, r, clip, color, true);
		}
		else
		{
			Driver->draw2DImage(tex, pos, r, clip, color, true);
		}
	}


	void CGUISpriteBank::draw2DSpriteBatch(const vector<UINT32>::type& indices,
		const vector<Position2d>::type& pos,
		const rect<SINT32>* clip,
		const ColourValue& color,
		UINT32 starttime, UINT32 currenttime,
		bool loop, bool center)
	{
		const UINT32 drawCount = Math::_max<UINT32>(indices.size(), pos.size());

		if (Textures.empty())
			return;
		vector<SDrawBatch>::type drawBatches(Textures.size());
		for (UINT32 i = 0; i < Textures.size(); i++)
		{
			drawBatches.push_back(SDrawBatch());
			//drawBatches[i].positions.reallocate(drawCount);
			//drawBatches[i].sourceRects.reallocate(drawCount);
		}

		for (UINT32 i = 0; i < drawCount; i++)
		{
			const UINT32 index = indices[i];

			if (index >= Sprites.size() || Sprites[index].Frames.empty())
				continue;

			// work out frame number
			UINT32 frame = 0;
			if (Sprites[index].frameTime)
			{
				UINT32 f = ((currenttime - starttime) / Sprites[index].frameTime);
				if (loop)
					frame = f % Sprites[index].Frames.size();
				else
					frame = (f >= Sprites[index].Frames.size()) ? Sprites[index].Frames.size() - 1 : f;
			}

			const UINT32 texNum = Sprites[index].Frames[frame].textureNumber;

			SDrawBatch& currentBatch = drawBatches[texNum];

			const UINT32 rn = Sprites[index].Frames[frame].rectNumber;
			if (rn >= Rectangles.size())
				return;

			const rect<SINT32>& r = Rectangles[rn];

			if (center)
			{
				Position2d p = pos[i];
				//p -= r.getSize() / 2;
				p = Vector2(p.x - (r.getSize() / 2).Width, p.y - (r.getSize() / 2).Height);
				currentBatch.positions.push_back(p);
				currentBatch.sourceRects.push_back(r);
			}
			else
			{
				currentBatch.positions.push_back(pos[i]);
				currentBatch.sourceRects.push_back(r);
			}
		}

		for (UINT32 i = 0; i < drawBatches.size(); i++)
		{
			if (!drawBatches[i].positions.empty() && !drawBatches[i].sourceRects.empty())
				Driver->draw2DImageBatch(Textures[i], drawBatches[i].positions,
				drawBatches[i].sourceRects, clip, color, true);
		}
	}
}

#endif