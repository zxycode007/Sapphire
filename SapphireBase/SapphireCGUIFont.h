#ifndef _SAPPHIRE_C_GUI_FONT_
#define _SAPPHIRE_C_GUI_FONT_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIFontBitmap.h"
#include "SapphireIXMLReader.h"
#include "SapphireIReadFile.h"


namespace Sapphire
{
	class IVideoDriver;
	class IImage;
	class IGUIEnvironment;

	class CGUIFont : public IGUIFontBitmap
	{
	public:

		//! constructor
		CGUIFont(IGUIEnvironment* env, const path& filename);

		//! destructor
		virtual ~CGUIFont();

		//! loads a font from a texture file
		bool load(const path& filename);

		//! loads a font from a texture file
		bool load(IReadFile* file);

		//! loads a font from an XML file
		bool load(IXMLReader* xml);

		//! draws an text and clips it to the specified rectangle if wanted
		virtual void draw(const StringW& text, const rect<SINT32>& position,
			ColourValue color, bool hcenter = false,
			bool vcenter = false, const rect<SINT32>* clip = 0);

		//! returns the dimension of a text
		virtual dimension2d<UINT32> getDimension(const wchar_t* text) const;

		//! Calculates the index of the character in the text which is on a specific position.
		virtual SINT32 getCharacterFromPos(const wchar_t* text, SINT32 pixel_x) const;

		//! Returns the type of this font
		virtual EGUI_FONT_TYPE getType() const { return EGFT_BITMAP; }

		//! set an Pixel Offset on Drawing ( scale position on width )
		virtual void setKerningWidth(SINT32 kerning);
		virtual void setKerningHeight(SINT32 kerning);

		//! set an Pixel Offset on Drawing ( scale position on width )
		virtual SINT32 getKerningWidth(const wchar_t* thisLetter = 0, const wchar_t* previousLetter = 0) const;
		virtual SINT32 getKerningHeight() const;

		//! gets the sprite bank
		virtual IGUISpriteBank* getSpriteBank() const;

		//! returns the sprite number from a given character
		virtual UINT32 getSpriteNoFromChar(const wchar_t *c) const;

		virtual void setInvisibleCharacters(const wchar_t *s);

	private:

		struct SFontArea
		{
			SFontArea() : underhang(0), overhang(0), width(0), spriteno(0) {}
			SINT32				underhang;
			SINT32				overhang;
			SINT32				width;
			UINT32				spriteno;
		};

		//! load & prepare font from ITexture
		bool loadTexture(IImage * image, const path& name);

		void readPositions(IImage* texture, SINT32& lowerRightPositions);

		SINT32 getAreaFromCharacter(const wchar_t c) const;
		void setMaxHeight();

		vector<SFontArea>::type		Areas;
		map<wchar_t, SINT32>::type		CharacterMap;
		IVideoDriver*		Driver;
		IGUISpriteBank*			SpriteBank;
		IGUIEnvironment*		Environment;
		UINT32				WrongCharacter;
		SINT32				MaxHeight;
		SINT32				GlobalKerningWidth, GlobalKerningHeight;

		StringW Invisible;
	};
}


#endif


#endif