#include "SapphireCGUIFont.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireOS.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIXMLReader.h"
#include "SapphireIReadFile.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireIImage.h"


namespace Sapphire
{
	//! constructor
	CGUIFont::CGUIFont(IGUIEnvironment *env, const path& filename)
		: Driver(0), SpriteBank(0), Environment(env), WrongCharacter(0),
		MaxHeight(0), GlobalKerningWidth(0), GlobalKerningHeight(0)
	{
#ifdef _DEBUG
		setDebugName("CGUIFont");
#endif

		if (Environment)
		{
			// don't grab environment, to avoid circular references
			Driver = Environment->getVideoDriver();

			SpriteBank = Environment->getSpriteBank(filename);
			if (!SpriteBank)	// could be default-font which has no file
				SpriteBank = Environment->addEmptySpriteBank(filename);
			if (SpriteBank)
				SpriteBank->grab();
		}

		if (Driver)
			Driver->grab();

		setInvisibleCharacters(L" ");
	}


	//! destructor
	CGUIFont::~CGUIFont()
	{
		if (Driver)
			Driver->drop();

		if (SpriteBank)
		{
			SpriteBank->drop();
			// TODO: spritebank still exists in gui-environment and should be removed here when it's
			// reference-count is 1. Just can't do that from here at the moment.
			// But spritebank would not be able to drop textures anyway because those are in texture-cache
			// where they can't be removed unless materials start reference-couting 'em.
		}
	}


	//! loads a font file from xml
	bool CGUIFont::load(IXMLReader* xml)
	{
		if (!SpriteBank)
			return false;

		SpriteBank->clear();

		while (xml->read())
		{
			if (EXN_ELEMENT == xml->getNodeType())
			{
				if (StringW(L"Texture") == xml->getNodeName())
				{
					// add a texture
					String fn = StringUtil::StringWToString( xml->getAttributeValue(L"filename"));
					UINT32 i = (UINT32)xml->getAttributeValueAsInt(L"index");
					StringW alpha = xml->getAttributeValue(L"hasAlpha");

					while (i + 1 > SpriteBank->getTextureCount())
						SpriteBank->addTexture(0);

					// disable mipmaps+filtering
					bool mipmap = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
					Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

					// load texture
					SpriteBank->setTexture(i, Driver->getTexture(fn));

					// set previous mip-map+filter state
					Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, mipmap);

					// couldn't load texture, abort.
					if (!SpriteBank->getTexture(i))
					{
						Printer::log("Unable to load all textures in the font, aborting", LML_ERROR);
						;
						return false;
					}
					else
					{
						// colorkey texture rather than alpha channel?
						if (alpha == StringW(L"false"))
							Driver->makeColorKeyTexture(SpriteBank->getTexture(i), Position2d(0, 0));
					}
				}
				else if (StringW(L"c") == xml->getNodeName())
				{
					// adding a character to this font
					SFontArea a;
					SGUISpriteFrame f;
					SGUISprite s;
					rect<SINT32> rectangle;

					a.underhang = xml->getAttributeValueAsInt(L"u");
					a.overhang = xml->getAttributeValueAsInt(L"o");
					a.spriteno = SpriteBank->getSprites().size();
					SINT32 texno = xml->getAttributeValueAsInt(L"i");

					// parse rectangle
					String rectstr = StringUtil::StringWToString(xml->getAttributeValue(L"r"));
					wchar_t ch = xml->getAttributeValue(L"c")[0];

					const c8 *c = rectstr.c_str();
					SINT32 val;
					val = 0;
					while (*c >= '0' && *c <= '9')
					{
						val *= 10;
						val += *c - '0';
						c++;
					}
					rectangle.UpperLeftCorner.x = val;
					while (*c == L' ' || *c == L',') c++;

					val = 0;
					while (*c >= '0' && *c <= '9')
					{
						val *= 10;
						val += *c - '0';
						c++;
					}
					rectangle.UpperLeftCorner.y = val;
					while (*c == L' ' || *c == L',') c++;

					val = 0;
					while (*c >= '0' && *c <= '9')
					{
						val *= 10;
						val += *c - '0';
						c++;
					}
					rectangle.LowerRightCorner.x = val;
					while (*c == L' ' || *c == L',') c++;

					val = 0;
					while (*c >= '0' && *c <= '9')
					{
						val *= 10;
						val += *c - '0';
						c++;
					}
					rectangle.LowerRightCorner.y = val;

					//CharacterMap.insert(ch, Areas.size());
					CharacterMap[ch] = Areas.size();

					// make frame
					f.rectNumber = SpriteBank->getPositions().size();
					f.textureNumber = texno;

					// add frame to sprite
					s.Frames.push_back(f);
					s.frameTime = 0;

					// add rectangle to sprite bank
					SpriteBank->getPositions().push_back(rectangle);
					a.width = rectangle.getWidth();

					// add sprite to sprite bank
					SpriteBank->getSprites().push_back(s);

					// add character to font
					Areas.push_back(a);
				}
			}
		}

		// set bad character
		WrongCharacter = getAreaFromCharacter(L' ');

		setMaxHeight();

		return true;
	}


	void CGUIFont::setMaxHeight()
	{
		if (!SpriteBank)
			return;

		MaxHeight = 0;
		SINT32 t;

		vector< rect<SINT32> >::type& p = SpriteBank->getPositions();

		for (UINT32 i = 0; i<p.size(); ++i)
		{
			t = p[i].getHeight();
			if (t>MaxHeight)
				MaxHeight = t;
		}

	}


	//! loads a font file, native file needed, for texture parsing
	bool CGUIFont::load(IReadFile* file)
	{
		if (!Driver)
			return false;

		return loadTexture(Driver->createImageFromFile(file),
			file->getFileName());
	}


	//! loads a font file, native file needed, for texture parsing
	bool CGUIFont::load(const path& filename)
	{
		if (!Driver)
			return false;

		return loadTexture(Driver->createImageFromFile(filename),
			filename);
	}


	//! load & prepare font from ITexture
	bool CGUIFont::loadTexture(IImage* image, const path& name)
	{
		if (!image || !SpriteBank)
			return false;

		SINT32 lowerRightPositions = 0;

		IImage* tmpImage = image;
		bool deleteTmpImage = false;
		switch (image->getColorFormat())
		{
		case ECF_R5G6B5:
			tmpImage = Driver->createImage(ECF_A1R5G5B5, image->getDimension());
			image->copyTo(tmpImage);
			deleteTmpImage = true;
			break;
		case ECF_A1R5G5B5:
		case ECF_A8R8G8B8:
			break;
		case ECF_R8G8B8:
			tmpImage = Driver->createImage(ECF_A8R8G8B8, image->getDimension());
			image->copyTo(tmpImage);
			deleteTmpImage = true;
			break;
		default:
			Printer::log("Unknown texture format provided for CGUIFont::loadTexture", LML_ERROR);
			return false;
		}
		readPositions(tmpImage, lowerRightPositions);

		WrongCharacter = getAreaFromCharacter(L' ');

		// output warnings
		if (!lowerRightPositions || !SpriteBank->getSprites().size())
			Printer::log("Either no upper or lower corner pixels in the font file. If this font was made using the new font tool, please load the XML file instead. If not, the font may be corrupted.", LML_ERROR);
		else
			if (lowerRightPositions != (SINT32)SpriteBank->getPositions().size())
				Printer::log("The amount of upper corner pixels and the lower corner pixels is not equal, font file may be corrupted.", LML_ERROR);

		bool ret = (!SpriteBank->getSprites().empty() && lowerRightPositions);

		if (ret)
		{
			bool flag[2];
			flag[0] = Driver->getTextureCreationFlag(ETCF_ALLOW_NON_POWER_2);
			flag[1] = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);

			Driver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2, true);
			Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

			SpriteBank->addTexture(Driver->addTexture(name, tmpImage));

			Driver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2, flag[0]);
			Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, flag[1]);
		}
		if (deleteTmpImage)
			tmpImage->drop();
		image->drop();

		setMaxHeight();

		return ret;
	}


	void CGUIFont::readPositions(IImage* image, SINT32& lowerRightPositions)
	{
		if (!SpriteBank)
			return;

		const dimension2d<UINT32> size = image->getDimension();

		ColourValue colorTopLeft = image->getPixel(0, 0);
		colorTopLeft.setAlpha(255);
		image->setPixel(0, 0, colorTopLeft);
		ColourValue colorLowerRight = image->getPixel(1, 0);
		ColourValue colorBackGround = image->getPixel(2, 0);
		ColourValue colorBackGroundTransparent = 0;

		image->setPixel(1, 0, colorBackGround);

		// start parsing

		Position2d pos(0, 0);
		for (pos.y = 0; pos.y<(SINT32)size.Height; ++pos.y)
		{
			for (pos.x = 0; pos.x<(SINT32)size.Width; ++pos.x)
			{
				const ColourValue c = image->getPixel(pos.x, pos.y);
				if (c == colorTopLeft)
				{
					image->setPixel(pos.x, pos.y, colorBackGroundTransparent);
					SpriteBank->getPositions().push_back(rect<SINT32>(pos, pos));
				}
				else
					if (c == colorLowerRight)
					{
						// too many lower right points
						if (SpriteBank->getPositions().size() <= (UINT32)lowerRightPositions)
						{
							lowerRightPositions = 0;
							return;
						}

						image->setPixel(pos.x, pos.y, colorBackGroundTransparent);
						SpriteBank->getPositions()[lowerRightPositions].LowerRightCorner = pos;
						// add frame to sprite bank
						SGUISpriteFrame f;
						f.rectNumber = lowerRightPositions;
						f.textureNumber = 0;
						SGUISprite s;
						s.Frames.push_back(f);
						s.frameTime = 0;
						SpriteBank->getSprites().push_back(s);
						// add character to font
						SFontArea a;
						a.overhang = 0;
						a.underhang = 0;
						a.spriteno = lowerRightPositions;
						a.width = SpriteBank->getPositions()[lowerRightPositions].getWidth();
						Areas.push_back(a);
						// map letter to character
						wchar_t ch = (wchar_t)(lowerRightPositions + 32);
						//CharacterMap.set(ch, lowerRightPositions);
						CharacterMap[ch] = lowerRightPositions;
						++lowerRightPositions;
					}
					else
						if (c == colorBackGround)
							image->setPixel(pos.x, pos.y, colorBackGroundTransparent);
			}
		}
	}


	//! set an Pixel Offset on Drawing ( scale position on width )
	void CGUIFont::setKerningWidth(SINT32 kerning)
	{
		GlobalKerningWidth = kerning;
	}


	//! set an Pixel Offset on Drawing ( scale position on width )
	SINT32 CGUIFont::getKerningWidth(const wchar_t* thisLetter, const wchar_t* previousLetter) const
	{
		SINT32 ret = GlobalKerningWidth;

		if (thisLetter)
		{
			ret += Areas[getAreaFromCharacter(*thisLetter)].overhang;

			if (previousLetter)
			{
				ret += Areas[getAreaFromCharacter(*previousLetter)].underhang;
			}
		}

		return ret;
	}


	//! set an Pixel Offset on Drawing ( scale position on height )
	void CGUIFont::setKerningHeight(SINT32 kerning)
	{
		GlobalKerningHeight = kerning;
	}


	//! set an Pixel Offset on Drawing ( scale position on height )
	SINT32 CGUIFont::getKerningHeight() const
	{
		return GlobalKerningHeight;
	}


	//! returns the sprite number from a given character
	UINT32 CGUIFont::getSpriteNoFromChar(const wchar_t *c) const
	{
		return Areas[getAreaFromCharacter(*c)].spriteno;
	}


	SINT32 CGUIFont::getAreaFromCharacter(const wchar_t c) const
	{
		map<wchar_t, SINT32>::const_iterator it = CharacterMap.find(c);
		if (it != CharacterMap.end())
		{
			SINT32 ret = it->second;
			return ret;
		}	
		else
			return WrongCharacter;
	}

	void CGUIFont::setInvisibleCharacters(const wchar_t *s)
	{
		Invisible = s;
	}


	//! returns the dimension of text
	dimension2d<UINT32> CGUIFont::getDimension(const wchar_t* text) const
	{
		dimension2d<UINT32> dim(0, 0);
		dimension2d<UINT32> thisLine(0, MaxHeight);

		for (const wchar_t* p = text; *p; ++p)
		{
			bool lineBreak = false;
			if (*p == L'\r') // Mac or Windows breaks
			{
				lineBreak = true;
				if (p[1] == L'\n') // Windows breaks
					++p;
			}
			else if (*p == L'\n') // Unix breaks
			{
				lineBreak = true;
			}
			if (lineBreak)
			{
				dim.Height += thisLine.Height;
				if (dim.Width < thisLine.Width)
					dim.Width = thisLine.Width;
				thisLine.Width = 0;
				continue;
			}

			const SFontArea &area = Areas[getAreaFromCharacter(*p)];

			thisLine.Width += area.underhang;
			thisLine.Width += area.width + area.overhang + GlobalKerningWidth;
		}

		dim.Height += thisLine.Height;
		if (dim.Width < thisLine.Width)
			dim.Width = thisLine.Width;

		return dim;
	}

	//! draws some text and clips it to the specified rectangle if wanted
	void CGUIFont::draw(const StringW& text, const rect<SINT32>& position,
		ColourValue color,
		bool hcenter, bool vcenter, const rect<SINT32>* clip
		)
	{
		if (!Driver || !SpriteBank)
			return;

		dimension2d<SINT32> textDimension;	// NOTE: don't make this UINT32 or the >> later on can fail when the dimension width is < position width
		Position2d offset = position.UpperLeftCorner;

		if (hcenter || vcenter || clip)
			textDimension = getDimension(text.c_str());

		if (hcenter)
			offset.x += (position.getWidth() - textDimension.Width) >> 1;

		if (vcenter)
			offset.y += (position.getHeight() - textDimension.Height) >> 1;

		if (clip)
		{
			rect<SINT32> clippedRect(offset, textDimension);
			clippedRect.clipAgainst(*clip);
			if (!clippedRect.isValid())
				return;
		}

		vector<UINT32>::type indices(text.size());
		vector<Position2d>::type offsets(text.size());

		for (UINT32 i = 0; i < text.size(); i++)
		{
			wchar_t c = text[i];

			bool lineBreak = false;
			if (c == L'\r') // Mac or Windows breaks
			{
				lineBreak = true;
				if (text[i + 1] == L'\n') // Windows breaks
					c = text[++i];
			}
			else if (c == L'\n') // Unix breaks
			{
				lineBreak = true;
			}

			if (lineBreak)
			{
				offset.y += MaxHeight;
				offset.x = position.UpperLeftCorner.x;

				if (hcenter)
				{
					offset.x += (position.getWidth() - textDimension.Width) >> 1;
				}
				continue;
			}

			SFontArea& area = Areas[getAreaFromCharacter(c)];

			offset.x += area.underhang;
			if (Invisible.at(c) < 0)//if (Invisible.findFirst(c) < 0)
			{
				indices.push_back(area.spriteno);
				offsets.push_back(offset);
			}

			offset.x += area.width + area.overhang + GlobalKerningWidth;
		}

		SpriteBank->draw2DSpriteBatch(indices, offsets, clip, color);
	}





	//! Calculates the index of the character in the text which is on a specific position.
	SINT32 CGUIFont::getCharacterFromPos(const wchar_t* text, SINT32 pixel_x) const
	{
		SINT32 x = 0;
		SINT32 idx = 0;

		while (text[idx])
		{
			const SFontArea& a = Areas[getAreaFromCharacter(text[idx])];

			x += a.width + a.overhang + a.underhang + GlobalKerningWidth;

			if (x >= pixel_x)
				return idx;

			++idx;
		}

		return -1;
	}


	IGUISpriteBank* CGUIFont::getSpriteBank() const
	{
		return SpriteBank;
	}
}

#endif