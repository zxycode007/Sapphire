#include "SapphireCGUIColorSelectDialog.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIGUIButton.h"
#include "SapphireIGUIStaticText.h"
#include "SapphireFont.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireIFileList.h"
#include "SapphireIImage.h"
#include "SapphireOs.h"
#include "SapphireQMath.h"

namespace Sapphire
{
	const SINT32 CSD_WIDTH = 350;
	const SINT32 CSD_HEIGHT = 300;

	struct subElementPredefines
	{
		const wchar_t *pre;
		const wchar_t *init;
		const wchar_t *post;
		int x, y;
		int range_down, range_up;
	};

	static const subElementPredefines Template[] =
	{
		{ L"A:", L"0", 0, 50, 165, 0, 255 },
		{ L"R:", L"0", 0, 20, 205, 0, 255 },
		{ L"G:", L"0", 0, 20, 230, 0, 255 },
		{ L"B:", L"0", 0, 20, 255, 0, 255 },
		{ L"H:", L"0", L"бу", 80, 205, 0, 360 },
		{ L"S:", L"0", L"%", 80, 230, 0, 100 },
		{ L"L:", L"0", L"%", 80, 255, 0, 100 },
	};


	//! constructor
	CGUIColorSelectDialog::CGUIColorSelectDialog(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, SINT32 id)
		: IGUIColorSelectDialog(environment, parent, id,
		rect<SINT32>((parent->getAbsolutePosition().getWidth() - CSD_WIDTH) / 2,
		(parent->getAbsolutePosition().getHeight() - CSD_HEIGHT) / 2,
		(parent->getAbsolutePosition().getWidth() - CSD_WIDTH) / 2 + CSD_WIDTH,
		(parent->getAbsolutePosition().getHeight() - CSD_HEIGHT) / 2 + CSD_HEIGHT)),
		Dragging(false)
	{
#ifdef _DEBUG
		IGUIElement::setDebugName("CGUIColorSelectDialog");
#endif

		Text = title;

		IGUISkin* skin = Environment->getSkin();

		const SINT32 buttonw = environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
		const SINT32 posx = RelativeRect.getWidth() - buttonw - 4;

		CloseButton = Environment->addButton(rect<SINT32>(posx, 3, posx + buttonw, 3 + buttonw),
			this, -1, L"", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close");
		if (skin && skin->getSpriteBank())
		{
			CloseButton->setSpriteBank(skin->getSpriteBank());
			CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), skin->getColor(EGDC_WINDOW_SYMBOL));
			CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), skin->getColor(EGDC_WINDOW_SYMBOL));
		}
		CloseButton->setSubElement(true);
		CloseButton->setTabStop(false);
		CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
		CloseButton->grab();

		OKButton = Environment->addButton(
			rect<SINT32>(RelativeRect.getWidth() - 80, 30, RelativeRect.getWidth() - 10, 50),
			this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_OK) : L"OK");
		OKButton->setSubElement(true);
		OKButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
		OKButton->grab();

		CancelButton = Environment->addButton(
			rect<SINT32>(RelativeRect.getWidth() - 80, 55, RelativeRect.getWidth() - 10, 75),
			this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_CANCEL) : L"Cancel");
		CancelButton->setSubElement(true);
		CancelButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
		CancelButton->grab();

		IVideoDriver* driver = Environment->getVideoDriver();
		ColorRing.Texture = driver->getTexture("#colorring");
		if (0 == ColorRing.Texture)
		{
			buildColorRing(dimension2d<UINT32>(128, 128), 1,
				Environment->getSkin()->getColor(EGDC_3D_SHADOW));
		}

		rect<SINT32> r(20, 20, 0, 0);

		ColorRing.Control = Environment->addImage(ColorRing.Texture, r.UpperLeftCorner, true, this);
		ColorRing.Control->setSubElement(true);
		ColorRing.Control->grab();

		for (UINT32 i = 0; i != sizeof(Template) / sizeof(subElementPredefines); ++i)
		{
			if (Template[i].pre)
			{
				r.UpperLeftCorner.x = Template[i].x;
				r.UpperLeftCorner.y = Template[i].y;
				r.LowerRightCorner.x = r.UpperLeftCorner.x + 15;
				r.LowerRightCorner.y = r.UpperLeftCorner.y + 20;
				IGUIElement *t = Environment->addStaticText(Template[i].pre, r, false, false, this);
				t->setSubElement(true);
			}

			if (Template[i].post)
			{
				r.UpperLeftCorner.x = Template[i].x + 56;
				r.UpperLeftCorner.y = Template[i].y;
				r.LowerRightCorner.x = r.UpperLeftCorner.x + 15;
				r.LowerRightCorner.y = r.UpperLeftCorner.y + 20;
				IGUIElement *t = Environment->addStaticText(Template[i].post, r, false, false, this);
				t->setSubElement(true);
			}

			r.UpperLeftCorner.x = Template[i].x + 15;
			r.UpperLeftCorner.y = Template[i].y - 2;
			r.LowerRightCorner.x = r.UpperLeftCorner.x + 40;
			r.LowerRightCorner.y = r.UpperLeftCorner.y + 20;

			IGUISpinBox* spin = Environment->addSpinBox(Template[i].init, r, true, this);
			spin->setSubElement(true);
			spin->setDecimalPlaces(0);
			spin->setRange((FLOAT32)Template[i].range_down, (FLOAT32)Template[i].range_up);
			spin->grab();

			Battery.push_back(spin);
		}

		bringToFront(CancelButton);
		bringToFront(OKButton);
	}


	//! destructor
	CGUIColorSelectDialog::~CGUIColorSelectDialog()
	{
		if (CloseButton)
			CloseButton->drop();

		if (OKButton)
			OKButton->drop();

		if (CancelButton)
			CancelButton->drop();

		for (UINT32 i = 0; i != Battery.size(); ++i)
			Battery[i]->drop();

		if (ColorRing.Control)
			ColorRing.Control->drop();
	}


	//! renders a antialiased, colored ring
	void CGUIColorSelectDialog::buildColorRing(const dimension2d<UINT32> & dim, SINT32 supersample, const ColourValue& borderColor)
	{
		const dimension2d<UINT32> d(dim.Width * supersample, dim.Height * supersample);
		IVideoDriver* driver = Environment->getVideoDriver();

		IImage *RawTexture = driver->createImage(ECF_A8R8G8B8, d);
		ARGB c = 0x00808080;
		RawTexture->fill(ColourValue(c));

		const SINT32 radiusOut = (d.Width / 2) - 4;
		const SINT32 fullR2 = radiusOut * radiusOut;

		ColourValue rgb(0, 0, 0);
		SColorHSL hsl;
		hsl.Luminance = 50;
		hsl.Saturation = 100;

		Position2d p;
		for (p.y = -radiusOut; p.y <= radiusOut; p.y += 1)
		{
			SINT32 y2 = p.y * p.y;

			for (p.x = -radiusOut; p.x <= radiusOut; p.x += 1)
			{
				SINT32 r2 = y2 + (p.x * p.x);

				// test point in circle
				SINT32 testa = r2 - fullR2;

				if (testa < 0)
				{
					// dotproduct u ( x,y ) * v ( 1, 0 ) = cosinus(a)

					const FLOAT32 r = sqrtf((FLOAT32)r2);

					// normalize, dotproduct = xnorm
					const FLOAT32 xn = r == 0.f ? 0.f : -p.x * reciprocal(r);

					hsl.Hue = RAD_TO_DEG(acosf(xn));
					if (p.y > 0)
						hsl.Hue = 360 - hsl.Hue;
					hsl.Hue -= 90;

					const FLOAT32 rTest = r / radiusOut;
#if 0
					if (rTest < 0.33f)
					{
						// luminance from 0 to 50
						hsl.Luminance = 50 * (rTest / 0.33);
						hsl.Saturation = 0.f;
						hsl.toRGB(rgb);
					}
					else
						if (rTest < 0.66f)
						{
							// saturation from 0 to 100
							hsl.Saturation = 100 * ((rTest - 0.33f) / 0.33f);
							hsl.Luminance = 50;
							hsl.toRGB(rgb);
						}
						else
						{
							// luminance from 50 to 100
							hsl.Luminance = 100 * (0.5f + ((rTest - 0.66f) / .66f));
							hsl.Saturation = 100;
							hsl.toRGB(rgb);
						}
					// borders should be slightly transparent
					if (rTest >= 0.95f)
						rgb.a = (1.f - rTest) * 20;
					else
						rgb.a = 1.f;
#else
					if (rTest > 0.5f)
					{
						hsl.Saturation = 100;
						hsl.Luminance = 50;
						hsl.toRGB(rgb);
					}
					// borders should be slightly transparent
					if (rTest < 0.5f)
						rgb.a = 0;
					else if (rTest >= 0.95f)
						rgb.a = (1.f - rTest) * 20;
					else if (rTest <= 0.55f)
						rgb.a = (rTest - 0.5f) * 20;
					else
						rgb.a = 1.f;
#endif
					RawTexture->setPixel(4 + p.x + radiusOut, 4 + p.y + radiusOut, rgb);
				}
			}
		}

		RawTexture->unlock();

		if (supersample > 1)
		{
			IImage * filter = driver->createImage(ECF_A8R8G8B8, dim);
			RawTexture->copyToScalingBoxFilter(filter);
			RawTexture->drop();
			RawTexture = filter;
		}

		bool generateMipLevels = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

		ColorRing.Texture = driver->addTexture("#colorring", RawTexture);
		RawTexture->drop();

		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, generateMipLevels);
	}


	//! called if an event happened.
	bool CGUIColorSelectDialog::OnEvent(const SEvent& event)
	{
		if (isEnabled())
		{
			switch (event.EventType)
			{
			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_SPINBOX_CHANGED:
				{
					for (UINT32 i = 0; i != Battery.size(); ++i)
					{
						if (event.GUIEvent.Caller == Battery[i])
						{
							if (i<4)
							{
								ColourValue rgb((UINT32)Battery[0]->getValue(), (UINT32)Battery[1]->getValue(),
									(UINT32)Battery[2]->getValue(), (UINT32)Battery[3]->getValue());
								SColorHSL hsl;
								ColourValue rgb2(rgb);
								hsl.fromRGB(rgb2);
								Battery[4]->setValue(hsl.Hue);
								Battery[5]->setValue(hsl.Saturation);
								Battery[6]->setValue(hsl.Luminance);
							}
							else
							{
								SColorHSL hsl(Battery[4]->getValue(), Battery[5]->getValue(),
									Battery[6]->getValue());
								ColourValue rgb2;
								hsl.toRGB(rgb2);
								ColourValue rgb = rgb2;
								Battery[1]->setValue((FLOAT32)rgb.getRed());
								Battery[2]->setValue((FLOAT32)rgb.getGreen());
								Battery[3]->setValue((FLOAT32)rgb.getBlue());
							}
						}
					}
					return true;
				}

				case EGET_ELEMENT_FOCUS_LOST:
					Dragging = false;
					break;
				case EGET_BUTTON_CLICKED:
					if (event.GUIEvent.Caller == CloseButton ||
						event.GUIEvent.Caller == CancelButton)
					{
						sendCancelEvent();
						remove();
						return true;
					}
					else
						if (event.GUIEvent.Caller == OKButton)
						{
							sendSelectedEvent();
							remove();
							return true;
						}
					break;

				case EGET_LISTBOX_CHANGED:
				case EGET_LISTBOX_SELECTED_AGAIN:
				default:
					break;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN:
					DragStart.x = event.MouseInput.X;
					DragStart.y = event.MouseInput.Y;
					Dragging = true;
					Environment->setFocus(this);
					return true;
				case EMIE_LMOUSE_LEFT_UP:
					Dragging = false;
					Environment->removeFocus(this);
					return true;
				case EMIE_MOUSE_MOVED:
					if (Dragging)
					{
						// gui window should not be dragged outside its parent
						if (Parent)
							if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.x + 1 ||
								event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.y + 1 ||
								event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.x - 1 ||
								event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.y - 1)

								return true;

						move(Position2d(event.MouseInput.X - DragStart.x, event.MouseInput.Y - DragStart.y));
						DragStart.x = event.MouseInput.X;
						DragStart.y = event.MouseInput.Y;
						return true;
					}
				default:
					break;
				}
			default:
				break;
			}
		}

		return IGUIElement::OnEvent(event);
	}


	//! draws the element and its children
	void CGUIColorSelectDialog::draw()
	{
		if (!IsVisible)
			return;

		IGUISkin* skin = Environment->getSkin();
		rect<SINT32> rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
			AbsoluteRect, &AbsoluteClippingRect);

		if (Text.size())
		{
			rect.UpperLeftCorner.x += 2;
			rect.LowerRightCorner.x -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

			IGUIFont* font = skin->getFont(EGDF_WINDOW);
			if (font)
				font->draw(StringUtil::StringWToString(Text), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true,
				&AbsoluteClippingRect);
		}

		IGUIElement::draw();

		// draw color selector after the window elements
		Vector2 pos(ColorRing.Control->getAbsolutePosition().UpperLeftCorner);
		pos.x += ColorRing.Texture->getOriginalSize().Width / 2;
		pos.y += ColorRing.Texture->getOriginalSize().Height / 2;
#if 0
		const FLOAT32 h = Battery[4]->getValue();
		const FLOAT32 s = Battery[5]->getValue();
		const FLOAT32 l = Battery[6]->getValue();
		const FLOAT32 factor = 58.f*(((s == 0) && (l<50)) ? (l*0.33f / 50) : (
			(s<100) ? ((.33f + (s*0.33f / 100))) : ((0.66f + (l - 50)*0.33f / 50))));

#else
		const FLOAT32 factor = 44;
#endif
		pos.x += round32(sinf(DEG_TO_RAD(Battery[4]->getValue()))*factor);
		pos.y -= round32(cosf(DEG_TO_RAD(Battery[4]->getValue()))*factor);
		Environment->getVideoDriver()->draw2DPolygon(pos, 4, 0xffffffff, 4);
	}


	ColourValue CGUIColorSelectDialog::getColor()
	{
		return ColourValue((UINT32)Battery[0]->getValue(), (UINT32)Battery[1]->getValue(),
			(UINT32)Battery[2]->getValue(), (UINT32)Battery[3]->getValue());
	}

	SColorHSL CGUIColorSelectDialog::getColorHSL()
	{
		return SColorHSL(Battery[4]->getValue(), Battery[5]->getValue(),
			Battery[6]->getValue());
	}

	//! sends the event that the file has been selected.
	void CGUIColorSelectDialog::sendSelectedEvent()
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.Element = 0;
		event.GUIEvent.EventType = EGET_FILE_SELECTED;
		Parent->OnEvent(event);
	}


	//! sends the event that the file choose process has been canceld
	void CGUIColorSelectDialog::sendCancelEvent()
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.Element = 0;
		event.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
		Parent->OnEvent(event);
	}

}




#endif