#include "SapphireCGUISkin.h"

#ifdef _SAPPHIRE_COMPILE_WITH_GUI_



#include "SapphireFont.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireIGUIElement.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIAttributes.h"

namespace Sapphire
{
	CGUISkin::CGUISkin(EGUI_SKIN_TYPE type, IVideoDriver* driver)
		: SpriteBank(0), Driver(driver), Type(type)
	{
#ifdef _DEBUG
		setDebugName("CGUISkin");
#endif

		if ((Type == EGST_WINDOWS_CLASSIC) || (Type == EGST_WINDOWS_METALLIC))
		{
			Colors[EGDC_3D_DARK_SHADOW] = ColourValue::getColourValue(101, 50, 50, 50);
			Colors[EGDC_3D_SHADOW] = ColourValue::getColourValue(101, 130, 130, 130);
			Colors[EGDC_3D_FACE] = ColourValue::getColourValue(101, 210, 210, 210);
			Colors[EGDC_3D_HIGH_LIGHT] = ColourValue::getColourValue(101, 255, 255, 255);
			Colors[EGDC_3D_LIGHT] = ColourValue::getColourValue(101, 210, 210, 210);
			Colors[EGDC_ACTIVE_BORDER] = ColourValue::getColourValue(101, 16, 14, 115);
			Colors[EGDC_ACTIVE_CAPTION] = ColourValue::getColourValue(255, 255, 255, 255);
			Colors[EGDC_APP_WORKSPACE] = ColourValue::getColourValue(101, 100, 100, 100);
			Colors[EGDC_BUTTON_TEXT] = ColourValue::getColourValue(240, 10, 10, 10);
			Colors[EGDC_GRAY_TEXT] = ColourValue::getColourValue(240, 130, 130, 130);
			Colors[EGDC_HIGH_LIGHT] = ColourValue::getColourValue(101, 8, 36, 107);
			Colors[EGDC_HIGH_LIGHT_TEXT] = ColourValue::getColourValue(240, 255, 255, 255);
			Colors[EGDC_INACTIVE_BORDER] = ColourValue::getColourValue(101, 165, 165, 165);
			Colors[EGDC_INACTIVE_CAPTION] = ColourValue::getColourValue(255, 30, 30, 30);
			Colors[EGDC_TOOLTIP] = ColourValue::getColourValue(200, 0, 0, 0);
			Colors[EGDC_TOOLTIP_BACKGROUND] = ColourValue::getColourValue(200, 255, 255, 225);
			Colors[EGDC_SCROLLBAR] = ColourValue::getColourValue(101, 230, 230, 230);
			Colors[EGDC_WINDOW] = ColourValue::getColourValue(101, 255, 255, 255);
			Colors[EGDC_WINDOW_SYMBOL] = ColourValue::getColourValue(200, 10, 10, 10);
			Colors[EGDC_ICON] = ColourValue::getColourValue(200, 255, 255, 255);
			Colors[EGDC_ICON_HIGH_LIGHT] = ColourValue::getColourValue(200, 8, 36, 107);
			Colors[EGDC_GRAY_WINDOW_SYMBOL] = ColourValue::getColourValue(240, 100, 100, 100);
			Colors[EGDC_EDITABLE] = ColourValue::getColourValue(255, 255, 255, 255);
			Colors[EGDC_GRAY_EDITABLE] = ColourValue::getColourValue(255, 120, 120, 120);
			Colors[EGDC_FOCUSED_EDITABLE] = ColourValue::getColourValue(255, 240, 240, 255);


			Sizes[EGDS_SCROLLBAR_SIZE] = 14;
			Sizes[EGDS_MENU_HEIGHT] = 30;
			Sizes[EGDS_WINDOW_BUTTON_WIDTH] = 15;
			Sizes[EGDS_CHECK_BOX_WIDTH] = 18;
			Sizes[EGDS_MESSAGE_BOX_WIDTH] = 500;
			Sizes[EGDS_MESSAGE_BOX_HEIGHT] = 200;
			Sizes[EGDS_BUTTON_WIDTH] = 80;
			Sizes[EGDS_BUTTON_HEIGHT] = 30;

			Sizes[EGDS_TEXT_DISTANCE_X] = 2;
			Sizes[EGDS_TEXT_DISTANCE_Y] = 0;

			Sizes[EGDS_TITLEBARTEXT_DISTANCE_X] = 2;
			Sizes[EGDS_TITLEBARTEXT_DISTANCE_Y] = 0;
		}
		else
		{
			//0x80a6a8af
			Colors[EGDC_3D_DARK_SHADOW].setAsARGB(0x60767982);
			//Colors[EGDC_3D_FACE]			=	0xc0c9ccd4;		// tab background
			Colors[EGDC_3D_FACE].setAsARGB(0xc0cbd2d9);		// tab background
			Colors[EGDC_3D_SHADOW].setAsARGB(0x50e4e8f1);		// tab background, and left-top highlight
			Colors[EGDC_3D_HIGH_LIGHT].setAsARGB(0x40c7ccdc);
			Colors[EGDC_3D_LIGHT].setAsARGB(0x802e313a);
			Colors[EGDC_ACTIVE_BORDER].setAsARGB(0x80404040);		// window title
			Colors[EGDC_ACTIVE_CAPTION].setAsARGB(0xffd0d0d0);
			Colors[EGDC_APP_WORKSPACE].setAsARGB(0xc0646464);		// unused
			Colors[EGDC_BUTTON_TEXT].setAsARGB(0xd0161616);
			Colors[EGDC_GRAY_TEXT].setAsARGB(0x3c141414);
			Colors[EGDC_HIGH_LIGHT].setAsARGB(0x6c606060);
			Colors[EGDC_HIGH_LIGHT_TEXT].setAsARGB(0xd0e0e0e0);
			Colors[EGDC_INACTIVE_BORDER].setAsARGB(0xf0a5a5a5);
			Colors[EGDC_INACTIVE_CAPTION].setAsARGB(0xffd2d2d2);
			Colors[EGDC_TOOLTIP].setAsARGB(0xf00f2033);
			Colors[EGDC_TOOLTIP_BACKGROUND].setAsARGB(0xc0cbd2d9);
			Colors[EGDC_SCROLLBAR].setAsARGB(0xf0e0e0e0);
			Colors[EGDC_WINDOW].setAsARGB(0xf0f0f0f0);
			Colors[EGDC_WINDOW_SYMBOL].setAsARGB(0xd0161616);
			Colors[EGDC_ICON].setAsARGB(0xd0161616);
			Colors[EGDC_ICON_HIGH_LIGHT].setAsARGB(0xd0606060);
			Colors[EGDC_GRAY_WINDOW_SYMBOL].setAsARGB(0x3c101010);
			Colors[EGDC_EDITABLE].setAsARGB(0xf0ffffff);
			Colors[EGDC_GRAY_EDITABLE].setAsARGB(0xf0cccccc);
			Colors[EGDC_FOCUSED_EDITABLE].setAsARGB(0xf0fffff0);

			Sizes[EGDS_SCROLLBAR_SIZE] = 14;
			Sizes[EGDS_MENU_HEIGHT] = 48;
			Sizes[EGDS_WINDOW_BUTTON_WIDTH] = 15;
			Sizes[EGDS_CHECK_BOX_WIDTH] = 18;
			Sizes[EGDS_MESSAGE_BOX_WIDTH] = 500;
			Sizes[EGDS_MESSAGE_BOX_HEIGHT] = 200;
			Sizes[EGDS_BUTTON_WIDTH] = 80;
			Sizes[EGDS_BUTTON_HEIGHT] = 30;

			Sizes[EGDS_TEXT_DISTANCE_X] = 3;
			Sizes[EGDS_TEXT_DISTANCE_Y] = 2;

			Sizes[EGDS_TITLEBARTEXT_DISTANCE_X] = 3;
			Sizes[EGDS_TITLEBARTEXT_DISTANCE_Y] = 2;
		}

		Sizes[EGDS_MESSAGE_BOX_GAP_SPACE] = 15;
		Sizes[EGDS_MESSAGE_BOX_MIN_TEXT_WIDTH] = 0;
		Sizes[EGDS_MESSAGE_BOX_MAX_TEXT_WIDTH] = 500;
		Sizes[EGDS_MESSAGE_BOX_MIN_TEXT_HEIGHT] = 0;
		Sizes[EGDS_MESSAGE_BOX_MAX_TEXT_HEIGHT] = 99999;

		Sizes[EGDS_BUTTON_PRESSED_IMAGE_OFFSET_X] = 1;
		Sizes[EGDS_BUTTON_PRESSED_IMAGE_OFFSET_Y] = 1;
		Sizes[EGDS_BUTTON_PRESSED_TEXT_OFFSET_X] = 0;
		Sizes[EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y] = 2;

		Texts[EGDT_MSG_BOX_OK] = L"OK";
		Texts[EGDT_MSG_BOX_CANCEL] = L"Cancel";
		Texts[EGDT_MSG_BOX_YES] = L"Yes";
		Texts[EGDT_MSG_BOX_NO] = L"No";
		Texts[EGDT_WINDOW_CLOSE] = L"Close";
		Texts[EGDT_WINDOW_RESTORE] = L"Restore";
		Texts[EGDT_WINDOW_MINIMIZE] = L"Minimize";
		Texts[EGDT_WINDOW_MAXIMIZE] = L"Maximize";

		Icons[EGDI_WINDOW_MAXIMIZE] = 225;
		Icons[EGDI_WINDOW_RESTORE] = 226;
		Icons[EGDI_WINDOW_CLOSE] = 227;
		Icons[EGDI_WINDOW_MINIMIZE] = 228;
		Icons[EGDI_CURSOR_UP] = 229;
		Icons[EGDI_CURSOR_DOWN] = 230;
		Icons[EGDI_CURSOR_LEFT] = 231;
		Icons[EGDI_CURSOR_RIGHT] = 232;
		Icons[EGDI_MENU_MORE] = 232;
		Icons[EGDI_CHECK_BOX_CHECKED] = 233;
		Icons[EGDI_DROP_DOWN] = 234;
		Icons[EGDI_SMALL_CURSOR_UP] = 235;
		Icons[EGDI_SMALL_CURSOR_DOWN] = 236;
		Icons[EGDI_RADIO_BUTTON_CHECKED] = 237;
		Icons[EGDI_MORE_LEFT] = 238;
		Icons[EGDI_MORE_RIGHT] = 239;
		Icons[EGDI_MORE_UP] = 240;
		Icons[EGDI_MORE_DOWN] = 241;
		Icons[EGDI_WINDOW_RESIZE] = 242;
		Icons[EGDI_EXPAND] = 243;
		Icons[EGDI_COLLAPSE] = 244;

		Icons[EGDI_FILE] = 245;
		Icons[EGDI_DIRECTORY] = 246;

		for (UINT32 i = 0; i<EGDF_COUNT; ++i)
			Fonts[i] = 0;

		UseGradient = (Type == EGST_WINDOWS_METALLIC) || (Type == EGST_BURNING_SKIN);
	}


	//! destructor
	CGUISkin::~CGUISkin()
	{
		for (UINT32 i = 0; i<EGDF_COUNT; ++i)
		{
			if (Fonts[i])
				Fonts[i]->drop();
		}

		if (SpriteBank)
			SpriteBank->drop();
	}


	//! returns default color
	ColourValue CGUISkin::getColor(EGUI_DEFAULT_COLOR color) const
	{
		if ((UINT32)color < EGDC_COUNT)
			return Colors[color];
		else
			return ColourValue();
	}


	//! sets a default color
	void CGUISkin::setColor(EGUI_DEFAULT_COLOR which, ColourValue newColor)
	{
		if ((UINT32)which < EGDC_COUNT)
			Colors[which] = newColor;
	}


	//! returns size for the given size type
	SINT32 CGUISkin::getSize(EGUI_DEFAULT_SIZE size) const
	{
		if ((UINT32)size < EGDS_COUNT)
			return Sizes[size];
		else
			return 0;
	}


	//! sets a default size
	void CGUISkin::setSize(EGUI_DEFAULT_SIZE which, SINT32 size)
	{
		if ((UINT32)which < EGDS_COUNT)
			Sizes[which] = size;
	}


	//! returns the default font
	IGUIFont* CGUISkin::getFont(EGUI_DEFAULT_FONT which) const
	{
		if (((UINT32)which < EGDF_COUNT) && Fonts[which])
			return Fonts[which];
		else
			return Fonts[EGDF_DEFAULT];
	}


	//! sets a default font
	void CGUISkin::setFont(IGUIFont* font, EGUI_DEFAULT_FONT which)
	{
		if ((UINT32)which >= EGDF_COUNT)
			return;

		if (font)
		{
			font->grab();
			if (Fonts[which])
				Fonts[which]->drop();

			Fonts[which] = font;
		}
	}


	//! gets the sprite bank stored
	IGUISpriteBank* CGUISkin::getSpriteBank() const
	{
		return SpriteBank;
	}


	//! set a new sprite bank or remove one by passing 0
	void CGUISkin::setSpriteBank(IGUISpriteBank* bank)
	{
		if (bank)
			bank->grab();

		if (SpriteBank)
			SpriteBank->drop();

		SpriteBank = bank;
	}


	//! Returns a default icon
	UINT32 CGUISkin::getIcon(EGUI_DEFAULT_ICON icon) const
	{
		if ((UINT32)icon < EGDI_COUNT)
			return Icons[icon];
		else
			return 0;
	}


	//! Sets a default icon
	void CGUISkin::setIcon(EGUI_DEFAULT_ICON icon, UINT32 index)
	{
		if ((UINT32)icon < EGDI_COUNT)
			Icons[icon] = index;
	}


	//! Returns a default text. For example for Message box button captions:
	//! "OK", "Cancel", "Yes", "No" and so on.
	const wchar_t* CGUISkin::getDefaultText(EGUI_DEFAULT_TEXT text) const
	{
		if ((UINT32)text < EGDT_COUNT)
			return Texts[text].c_str();
		else
			return Texts[0].c_str();
	}


	//! Sets a default text. For example for Message box button captions:
	//! "OK", "Cancel", "Yes", "No" and so on.
	void CGUISkin::setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText)
	{
		if ((UINT32)which < EGDT_COUNT)
			Texts[which] = newText;
	}


	//! draws a standard 3d button pane
	/**	Used for drawing for example buttons in normal state.
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
	\param rect: Defining area where to draw.
	\param clip: Clip area.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly. */
	void CGUISkin::draw3DButtonPaneStandard(IGUIElement* element,
		const rect<SINT32>& r,
		const rect<SINT32>* clip)
	{
		if (!Driver)
			return;

		rect<SINT32> rect = r;

		if (Type == EGST_BURNING_SKIN)
		{
			rect.UpperLeftCorner.x -= 1;
			rect.UpperLeftCorner.y -= 1;
			rect.LowerRightCorner.x += 1;
			rect.LowerRightCorner.y += 1;
			draw3DSunkenPane(element,
				getColor(EGDC_WINDOW).getInterpolated(0xFFFFFFFF, 0.9f)
				, false, true, rect, clip);
			return;
		}

		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

		rect.LowerRightCorner.x -= 1;
		rect.LowerRightCorner.y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect.UpperLeftCorner.x += 1;
		rect.UpperLeftCorner.y += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect.LowerRightCorner.x -= 1;
		rect.LowerRightCorner.y -= 1;

		if (!UseGradient)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
		}
		else
		{
			const ColourValue c1 = getColor(EGDC_3D_FACE);
			const ColourValue c2 = c1.getInterpolated(getColor(EGDC_3D_DARK_SHADOW), 0.4f);
			Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
		}
	}


	//! draws a pressed 3d button pane
	/**	Used for drawing for example buttons in pressed state.
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
	\param rect: Defining area where to draw.
	\param clip: Clip area.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly. */
	void CGUISkin::draw3DButtonPanePressed(IGUIElement* element,
		const rect<SINT32>& r,
		const rect<SINT32>* clip)
	{
		if (!Driver)
			return;

		rect<SINT32> rect = r;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect.LowerRightCorner.x -= 1;
		rect.LowerRightCorner.y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

		rect.UpperLeftCorner.x += 1;
		rect.UpperLeftCorner.y += 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect.UpperLeftCorner.x += 1;
		rect.UpperLeftCorner.y += 1;

		if (!UseGradient)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
		}
		else
		{
			const ColourValue c1 = getColor(EGDC_3D_FACE);
			const ColourValue c2 = c1.getInterpolated(getColor(EGDC_3D_DARK_SHADOW), 0.4f);
			Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
		}
	}


	//! draws a sunken 3d pane
	/** Used for drawing the background of edit, combo or check boxes.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly.
	\param bgcolor: Background color.
	\param flat: Specifies if the sunken pane should be flat or displayed as sunken
	deep into the ground.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	void CGUISkin::draw3DSunkenPane(IGUIElement* element, ColourValue bgcolor,
		bool flat, bool fillBackGround,
		const rect<SINT32>& r,
		const rect<SINT32>* clip)
	{
		if (!Driver)
			return;

		rect<SINT32> rect = r;

		if (fillBackGround)
			Driver->draw2DRectangle(bgcolor, rect, clip);

		if (flat)
		{
			// draw flat sunken pane

			rect.LowerRightCorner.y = rect.UpperLeftCorner.y + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);	// top

			++rect.UpperLeftCorner.y;
			rect.LowerRightCorner.y = r.LowerRightCorner.y;
			rect.LowerRightCorner.x = rect.UpperLeftCorner.x + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);	// left

			rect = r;
			++rect.UpperLeftCorner.y;
			rect.UpperLeftCorner.x = rect.LowerRightCorner.x - 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);	// right

			rect = r;
			++rect.UpperLeftCorner.x;
			rect.UpperLeftCorner.y = r.LowerRightCorner.y - 1;
			--rect.LowerRightCorner.x;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);	// bottom
		}
		else
		{
			// draw deep sunken pane
			rect.LowerRightCorner.y = rect.UpperLeftCorner.y + 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);	// top
			++rect.UpperLeftCorner.x;
			++rect.UpperLeftCorner.y;
			--rect.LowerRightCorner.x;
			++rect.LowerRightCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

			rect.UpperLeftCorner.x = r.UpperLeftCorner.x;
			rect.UpperLeftCorner.y = r.UpperLeftCorner.y + 1;
			rect.LowerRightCorner.x = rect.UpperLeftCorner.x + 1;
			rect.LowerRightCorner.y = r.LowerRightCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);	// left
			++rect.UpperLeftCorner.x;
			++rect.UpperLeftCorner.y;
			++rect.LowerRightCorner.x;
			--rect.LowerRightCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

			rect = r;
			rect.UpperLeftCorner.x = rect.LowerRightCorner.x - 1;
			++rect.UpperLeftCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);	// right
			--rect.UpperLeftCorner.x;
			++rect.UpperLeftCorner.y;
			--rect.LowerRightCorner.x;
			--rect.LowerRightCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_LIGHT), rect, clip);

			rect = r;
			++rect.UpperLeftCorner.x;
			rect.UpperLeftCorner.y = r.LowerRightCorner.y - 1;
			--rect.LowerRightCorner.x;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);	// bottom
			++rect.UpperLeftCorner.x;
			--rect.UpperLeftCorner.y;
			--rect.LowerRightCorner.x;
			--rect.LowerRightCorner.y;
			Driver->draw2DRectangle(getColor(EGDC_3D_LIGHT), rect, clip);
		}
	}


	//! draws a window background
	// return where to draw title bar text.
	rect<SINT32> CGUISkin::draw3DWindowBackground(IGUIElement* element,
		bool drawTitleBar, ColourValue titleBarColor,
		const rect<SINT32>& r,
		const rect<SINT32>* clip,
		rect<SINT32>* checkClientArea)
	{
		if (!Driver)
		{
			if (checkClientArea)
			{
				*checkClientArea = r;
			}
			return r;
		}

		rect<SINT32> rect = r;

		// top border
		rect.LowerRightCorner.y = rect.UpperLeftCorner.y + 1;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);
		}

		// left border
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		rect.LowerRightCorner.x = rect.UpperLeftCorner.x + 1;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);
		}

		// right border dark outer line
		rect.UpperLeftCorner.x = r.LowerRightCorner.x - 1;
		rect.LowerRightCorner.x = r.LowerRightCorner.x;
		rect.UpperLeftCorner.y = r.UpperLeftCorner.y;
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);
		}

		// right border bright innner line
		rect.UpperLeftCorner.x -= 1;
		rect.LowerRightCorner.x -= 1;
		rect.UpperLeftCorner.y += 1;
		rect.LowerRightCorner.y -= 1;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);
		}

		// bottom border dark outer line
		rect.UpperLeftCorner.x = r.UpperLeftCorner.x;
		rect.UpperLeftCorner.y = r.LowerRightCorner.y - 1;
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		rect.LowerRightCorner.x = r.LowerRightCorner.x;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);
		}

		// bottom border bright inner line
		rect.UpperLeftCorner.x += 1;
		rect.LowerRightCorner.x -= 1;
		rect.UpperLeftCorner.y -= 1;
		rect.LowerRightCorner.y -= 1;
		if (!checkClientArea)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);
		}

		// client area for background
		rect = r;
		rect.UpperLeftCorner.x += 1;
		rect.UpperLeftCorner.y += 1;
		rect.LowerRightCorner.x -= 2;
		rect.LowerRightCorner.y -= 2;
		if (checkClientArea)
		{
			*checkClientArea = rect;
		}

		if (!checkClientArea)
		{
			if (!UseGradient)
			{
				Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
			}
			else if (Type == EGST_BURNING_SKIN)
			{
				const ColourValue c1 = getColor(EGDC_WINDOW).getInterpolated(0xFFFFFFFF, 0.9f);
				const ColourValue c2 = getColor(EGDC_WINDOW).getInterpolated(0xFFFFFFFF, 0.8f);

				Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
			}
			else
			{
				const ColourValue c2 = getColor(EGDC_3D_SHADOW);
				const ColourValue c1 = getColor(EGDC_3D_FACE);
				Driver->draw2DRectangle(rect, c1, c1, c1, c2, clip);
			}
		}

		// title bar
		rect = r;
		rect.UpperLeftCorner.x += 2;
		rect.UpperLeftCorner.y += 2;
		rect.LowerRightCorner.x -= 2;
		rect.LowerRightCorner.y = rect.UpperLeftCorner.y + getSize(EGDS_WINDOW_BUTTON_WIDTH) + 2;

		if (drawTitleBar)
		{
			if (checkClientArea)
			{
				(*checkClientArea).UpperLeftCorner.y = rect.LowerRightCorner.y;
			}
			else
			{
				// draw title bar
				//if (!UseGradient)
				//	Driver->draw2DRectangle(titleBarColor, rect, clip);
				//else
				if (Type == EGST_BURNING_SKIN)
				{
					const ColourValue c = titleBarColor.getInterpolated(ColourValue(titleBarColor.getAlpha(), 255, 255, 255), 0.8f);
					Driver->draw2DRectangle(rect, titleBarColor, titleBarColor, c, c, clip);
				}
				else
				{
					const ColourValue c = titleBarColor.getInterpolated(ColourValue(titleBarColor.getAlpha(), 0, 0, 0), 0.2f);
					Driver->draw2DRectangle(rect, titleBarColor, c, titleBarColor, c, clip);
				}
			}
		}

		return rect;
	}


	//! draws a standard 3d menu pane
	/**	Used for drawing for menus and context menus.
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	void CGUISkin::draw3DMenuPane(IGUIElement* element,
		const rect<SINT32>& r, const rect<SINT32>* clip)
	{
		if (!Driver)
			return;

		rect<SINT32> rect = r;

		if (Type == EGST_BURNING_SKIN)
		{
			rect.UpperLeftCorner.y -= 3;
			draw3DButtonPaneStandard(element, rect, clip);
			return;
		}

		// in this skin, this is exactly what non pressed buttons look like,
		// so we could simply call
		// draw3DButtonPaneStandard(element, rect, clip);
		// here.
		// but if the skin is transparent, this doesn't look that nice. So
		// We draw it a little bit better, with some more draw2DRectangle calls,
		// but there aren't that much menus visible anyway.

		rect.LowerRightCorner.y = rect.UpperLeftCorner.y + 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		rect.LowerRightCorner.x = rect.UpperLeftCorner.x + 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), rect, clip);

		rect.UpperLeftCorner.x = r.LowerRightCorner.x - 1;
		rect.LowerRightCorner.x = r.LowerRightCorner.x;
		rect.UpperLeftCorner.y = r.UpperLeftCorner.y;
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

		rect.UpperLeftCorner.x -= 1;
		rect.LowerRightCorner.x -= 1;
		rect.UpperLeftCorner.y += 1;
		rect.LowerRightCorner.y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect.UpperLeftCorner.x = r.UpperLeftCorner.x;
		rect.UpperLeftCorner.y = r.LowerRightCorner.y - 1;
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		rect.LowerRightCorner.x = r.LowerRightCorner.x;
		Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), rect, clip);

		rect.UpperLeftCorner.x += 1;
		rect.LowerRightCorner.x -= 1;
		rect.UpperLeftCorner.y -= 1;
		rect.LowerRightCorner.y -= 1;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect = r;
		rect.UpperLeftCorner.x += 1;
		rect.UpperLeftCorner.y += 1;
		rect.LowerRightCorner.x -= 2;
		rect.LowerRightCorner.y -= 2;

		if (!UseGradient)
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
		else
		{
			const ColourValue c1 = getColor(EGDC_3D_FACE);
			const ColourValue c2 = getColor(EGDC_3D_SHADOW);
			Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
		}
	}


	//! draws a standard 3d tool bar
	/**	Used for drawing for toolbars and menus.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	void CGUISkin::draw3DToolBar(IGUIElement* element,
		const rect<SINT32>& r,
		const rect<SINT32>* clip)
	{
		if (!Driver)
			return;

		rect<SINT32> rect = r;

		rect.UpperLeftCorner.x = r.UpperLeftCorner.x;
		rect.UpperLeftCorner.y = r.LowerRightCorner.y - 1;
		rect.LowerRightCorner.y = r.LowerRightCorner.y;
		rect.LowerRightCorner.x = r.LowerRightCorner.x;
		Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), rect, clip);

		rect = r;
		rect.LowerRightCorner.y -= 1;

		if (!UseGradient)
		{
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), rect, clip);
		}
		else
			if (Type == EGST_BURNING_SKIN)
			{
				const ColourValue c1 = 0xF0000000 | getColor(EGDC_3D_FACE).getAsARGB();
				const ColourValue c2 = 0xF0000000 | getColor(EGDC_3D_SHADOW).getAsARGB();

				rect.LowerRightCorner.y += 1;
				Driver->draw2DRectangle(rect, c1, c2, c1, c2, clip);
			}
			else
			{
				const ColourValue c1 = getColor(EGDC_3D_FACE);
				const ColourValue c2 = getColor(EGDC_3D_SHADOW);
				Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
			}
	}


	//! draws a tab button
	/**	Used for drawing for tab buttons on top of tabs.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly.
	\param active: Specifies if the tab is currently active.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	void CGUISkin::draw3DTabButton(IGUIElement* element, bool active,
		const rect<SINT32>& frameRect, const rect<SINT32>* clip, EGUI_ALIGNMENT alignment)
	{
		if (!Driver)
			return;

		rect<SINT32> tr = frameRect;

		if (alignment == EGUIA_UPPERLEFT)
		{
			tr.LowerRightCorner.x -= 2;
			tr.LowerRightCorner.y = tr.UpperLeftCorner.y + 1;
			tr.UpperLeftCorner.x += 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

			// draw left highlight
			tr = frameRect;
			tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
			tr.UpperLeftCorner.y += 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

			// draw grey background
			tr = frameRect;
			tr.UpperLeftCorner.x += 1;
			tr.UpperLeftCorner.y += 1;
			tr.LowerRightCorner.x -= 2;
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), tr, clip);

			// draw right middle gray shadow
			tr.LowerRightCorner.x += 1;
			tr.UpperLeftCorner.x = tr.LowerRightCorner.x - 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

			tr.LowerRightCorner.x += 1;
			tr.UpperLeftCorner.x += 1;
			tr.UpperLeftCorner.y += 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), tr, clip);
		}
		else
		{
			tr.LowerRightCorner.x -= 2;
			tr.UpperLeftCorner.y = tr.LowerRightCorner.y - 1;
			tr.UpperLeftCorner.x += 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

			// draw left highlight
			tr = frameRect;
			tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
			tr.LowerRightCorner.y -= 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

			// draw grey background
			tr = frameRect;
			tr.UpperLeftCorner.x += 1;
			tr.UpperLeftCorner.y -= 1;
			tr.LowerRightCorner.x -= 2;
			tr.LowerRightCorner.y -= 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_FACE), tr, clip);

			// draw right middle gray shadow
			tr.LowerRightCorner.x += 1;
			tr.UpperLeftCorner.x = tr.LowerRightCorner.x - 1;
			//tr.LowerRightCorner.y -= 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

			tr.LowerRightCorner.x += 1;
			tr.UpperLeftCorner.x += 1;
			tr.LowerRightCorner.y -= 1;
			Driver->draw2DRectangle(getColor(EGDC_3D_DARK_SHADOW), tr, clip);
		}
	}


	//! draws a tab control body
	/**	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex
	implementations to find out how to draw the part exactly.
	\param border: Specifies if the border should be drawn.
	\param background: Specifies if the background should be drawn.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	void CGUISkin::draw3DTabBody(IGUIElement* element, bool border, bool background,
		const rect<SINT32>& rectangle, const rect<SINT32>* clip, SINT32 tabHeight, EGUI_ALIGNMENT alignment)
	{
		if (!Driver)
			return;

		Sapphire::rect<SINT32> tr = rectangle;

		if (tabHeight == -1)
			tabHeight = getSize(EGDS_BUTTON_HEIGHT);

		// draw border.
		if (border)
		{
			if (alignment == EGUIA_UPPERLEFT)
			{
				// draw left hightlight
				tr.UpperLeftCorner.y += tabHeight + 2;
				tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

				// draw right shadow
				tr.UpperLeftCorner.x = rectangle.LowerRightCorner.x - 1;
				tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

				// draw lower shadow
				tr = rectangle;
				tr.UpperLeftCorner.y = tr.LowerRightCorner.y - 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);
			}
			else
			{
				// draw left hightlight
				tr.LowerRightCorner.y -= tabHeight + 2;
				tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

				// draw right shadow
				tr.UpperLeftCorner.x = rectangle.LowerRightCorner.x - 1;
				tr.LowerRightCorner.x = tr.UpperLeftCorner.x + 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_SHADOW), tr, clip);

				// draw lower shadow
				tr = rectangle;
				tr.LowerRightCorner.y = tr.UpperLeftCorner.y + 1;
				Driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);
			}
		}

		if (background)
		{
			if (alignment == EGUIA_UPPERLEFT)
			{
				tr = rectangle;
				tr.UpperLeftCorner.y += tabHeight + 2;
				tr.LowerRightCorner.x -= 1;
				tr.UpperLeftCorner.x += 1;
				tr.LowerRightCorner.y -= 1;
			}
			else
			{
				tr = rectangle;
				tr.UpperLeftCorner.x += 1;
				tr.UpperLeftCorner.y -= 1;
				tr.LowerRightCorner.x -= 1;
				tr.LowerRightCorner.y -= tabHeight + 2;
				//tr.UpperLeftCorner.x += 1;
			}

			if (!UseGradient)
				Driver->draw2DRectangle(getColor(EGDC_3D_FACE), tr, clip);
			else
			{
				ColourValue c1 = getColor(EGDC_3D_FACE);
				ColourValue c2 = getColor(EGDC_3D_SHADOW);
				Driver->draw2DRectangle(tr, c1, c1, c2, c2, clip);
			}
		}
	}


	//! draws an icon, usually from the skin's sprite bank
	/**	\param parent: Pointer to the element which wishes to draw this icon.
	This parameter is usually not used by IGUISkin, but can be used for example
	by more complex implementations to find out how to draw the part exactly.
	\param icon: Specifies the icon to be drawn.
	\param position: The position to draw the icon
	\param starttime: The time at the start of the animation
	\param currenttime: The present time, used to calculate the frame number
	\param loop: Whether the animation should loop or not
	\param clip: Clip area.	*/
	void CGUISkin::drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
		const Position2d position,
		UINT32 starttime, UINT32 currenttime,
		bool loop, const rect<SINT32>* clip)
	{
		if (!SpriteBank)
			return;

		bool gray = element && !element->isEnabled();
		SpriteBank->draw2DSprite(Icons[icon], position, clip,
			Colors[gray ? EGDC_GRAY_WINDOW_SYMBOL : EGDC_WINDOW_SYMBOL], starttime, currenttime, loop, true);
	}


	EGUI_SKIN_TYPE CGUISkin::getType() const
	{
		return Type;
	}


	//! draws a 2d rectangle.
	void CGUISkin::draw2DRectangle(IGUIElement* element,
		const ColourValue &color, const rect<SINT32>& pos,
		const rect<SINT32>* clip)
	{
		Driver->draw2DRectangle(color, pos, clip);
	}


	//! Writes attributes of the object.
	//! Implement this to expose the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml serialization purposes.
	void CGUISkin::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		UINT32 i;
		for (i = 0; i<EGDC_COUNT; ++i)
			out->addColor(GUISkinColorNames[i], Colors[i]);

		for (i = 0; i<EGDS_COUNT; ++i)
			out->addInt(GUISkinSizeNames[i], Sizes[i]);

		for (i = 0; i<EGDT_COUNT; ++i)
			out->addString(GUISkinTextNames[i], Texts[i].c_str());

		for (i = 0; i<EGDI_COUNT; ++i)
			out->addInt(GUISkinIconNames[i], Icons[i]);
	}


	//! Reads attributes of the object.
	//! Implement this to set the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml deserialization purposes.
	void CGUISkin::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		// TODO: This is not nice code for downward compatibility, whenever new values are added and users
		// load an old skin the corresponding values will be set to 0.
		UINT32 i;
		for (i = 0; i<EGDC_COUNT; ++i)
			Colors[i] = in->getAttributeAsColor(GUISkinColorNames[i]);

		for (i = 0; i<EGDS_COUNT; ++i)
			Sizes[i] = in->getAttributeAsInt(GUISkinSizeNames[i]);

		for (i = 0; i<EGDT_COUNT; ++i)
			Texts[i] = in->getAttributeAsStringW(GUISkinTextNames[i]);

		for (i = 0; i<EGDI_COUNT; ++i)
			Icons[i] = in->getAttributeAsInt(GUISkinIconNames[i]);
	}
}
#endif
