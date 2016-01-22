#ifndef _SAPPHIRE_C_GUI_SKIN_
#define _SAPPHIRE_C_GUI_SKIN_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"

namespace Sapphire
{
	class IVideoDriver;

	class CGUISkin : public IGUISkin
	{
	public:

		CGUISkin(EGUI_SKIN_TYPE type, IVideoDriver* driver);

		//! destructor
		virtual ~CGUISkin();

		//! returns default color
		virtual ColourValue getColor(EGUI_DEFAULT_COLOR color) const;

		//! sets a default color
		virtual void setColor(EGUI_DEFAULT_COLOR which, ColourValue newColor);

		//! returns size for the given size type
		virtual SINT32 getSize(EGUI_DEFAULT_SIZE size) const;

		//! sets a default size
		virtual void setSize(EGUI_DEFAULT_SIZE which, SINT32 size);

		//! returns the default font
		virtual IGUIFont* getFont(EGUI_DEFAULT_FONT which = EGDF_DEFAULT) const;

		//! sets a default font
		virtual void setFont(IGUIFont* font, EGUI_DEFAULT_FONT which = EGDF_DEFAULT);

		//! sets the sprite bank used for drawing icons
		virtual void setSpriteBank(IGUISpriteBank* bank);

		//! gets the sprite bank used for drawing icons
		virtual IGUISpriteBank* getSpriteBank() const;

		//! Returns a default icon
		/** Returns the sprite index within the sprite bank */
		virtual UINT32 getIcon(EGUI_DEFAULT_ICON icon) const;

		//! Sets a default icon
		/** Sets the sprite index used for drawing icons like arrows,
		close buttons and ticks in checkboxes
		\param icon: Enum specifying which icon to change
		\param index: The sprite index used to draw this icon */
		virtual void setIcon(EGUI_DEFAULT_ICON icon, UINT32 index);

		//! Returns a default text.
		/** For example for Message box button captions:
		"OK", "Cancel", "Yes", "No" and so on. */
		virtual const wchar_t* getDefaultText(EGUI_DEFAULT_TEXT text) const;

		//! Sets a default text.
		/** For example for Message box button captions:
		"OK", "Cancel", "Yes", "No" and so on. */
		virtual void setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText);

		//! draws a standard 3d button pane
		/** Used for drawing for example buttons in normal state.
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
		\param rect: Defining area where to draw.
		\param clip: Clip area.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly. */
		virtual void draw3DButtonPaneStandard(IGUIElement* element,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip = 0);

		//! draws a pressed 3d button pane
		/** Used for drawing for example buttons in pressed state.
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
		\param rect: Defining area where to draw.
		\param clip: Clip area.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly. */
		virtual void draw3DButtonPanePressed(IGUIElement* element,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip = 0);

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
		virtual void draw3DSunkenPane(IGUIElement* element,
			ColourValue bgcolor, bool flat,
			bool fillBackGround,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip = 0);

		//! draws a window background
		/** Used for drawing the background of dialogs and windows.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly.
		\param titleBarColor: Title color.
		\param drawTitleBar: True to enable title drawing.
		\param rect: Defining area where to draw.
		\param clip: Clip area.
		\param checkClientArea: When set to non-null the function will not draw anything,
		but will instead return the clientArea which can be used for drawing by the calling window.
		That is the area without borders and without titlebar.
		\return Returns rect where it would be good to draw title bar text. This will
		work even when checkClientArea is set to a non-null value.*/
		virtual rect<SINT32> draw3DWindowBackground(IGUIElement* element,
			bool drawTitleBar, ColourValue titleBarColor,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip,
			rect<SINT32>* checkClientArea);

		//! draws a standard 3d menu pane
		/** Used for drawing for menus and context menus.
		It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
		EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DMenuPane(IGUIElement* element,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip = 0);

		//! draws a standard 3d tool bar
		/** Used for drawing for toolbars and menus.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DToolBar(IGUIElement* element,
			const rect<SINT32>& rectangle,
			const rect<SINT32>* clip = 0);

		//! draws a tab button
		/** Used for drawing for tab buttons on top of tabs.
		\param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly.
		\param active: Specifies if the tab is currently active.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabButton(IGUIElement* element, bool active,
			const rect<SINT32>& rectangle, const  rect<SINT32>* clip = 0, EGUI_ALIGNMENT alignment = EGUIA_UPPERLEFT);

		//! draws a tab control body
		/** \param element: Pointer to the element which wishes to draw this. This parameter
		is usually not used by ISkin, but can be used for example by more complex
		implementations to find out how to draw the part exactly.
		\param border: Specifies if the border should be drawn.
		\param background: Specifies if the background should be drawn.
		\param rect: Defining area where to draw.
		\param clip: Clip area.	*/
		virtual void draw3DTabBody(IGUIElement* element, bool border, bool background,
			const rect<SINT32>& rectangle, const rect<SINT32>* clip, SINT32 tabHeight = -1, EGUI_ALIGNMENT alignment = EGUIA_UPPERLEFT);

		//! draws an icon, usually from the skin's sprite bank
		/** \param element: Pointer to the element which wishes to draw this icon.
		This parameter is usually not used by IGUISkin, but can be used for example
		by more complex implementations to find out how to draw the part exactly.
		\param icon: Specifies the icon to be drawn.
		\param position: The position to draw the icon
		\param starttime: The time at the start of the animation
		\param currenttime: The present time, used to calculate the frame number
		\param loop: Whether the animation should loop or not
		\param clip: Clip area.	*/
		virtual void drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
			const Position2d position,
			UINT32 starttime = 0, UINT32 currenttime = 0,
			bool loop = false, const rect<SINT32>* clip = 0);


		//! draws a 2d rectangle.
		/** \param element: Pointer to the element which wishes to draw this icon.
		This parameter is usually not used by IGUISkin, but can be used for example
		by more complex implementations to find out how to draw the part exactly.
		\param color: Color of the rectangle to draw. The alpha component specifies how
		transparent the rectangle will be.
		\param pos: Position of the rectangle.
		\param clip: Pointer to rectangle against which the rectangle will be clipped.
		If the pointer is null, no clipping will be performed. */
		virtual void draw2DRectangle(IGUIElement* element, const ColourValue &color,
			const rect<SINT32>& pos, const rect<SINT32>* clip =0);


		//! get the type of this skin
		virtual EGUI_SKIN_TYPE getType() const;

		//! Writes attributes of the object.
		//! Implement this to expose the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml serialization purposes.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the object.
		//! Implement this to set the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml deserialization purposes.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

	private:

		ColourValue Colors[EGDC_COUNT];
		SINT32 Sizes[EGDS_COUNT];
		UINT32 Icons[EGDI_COUNT];
		IGUIFont* Fonts[EGDF_COUNT];
		IGUISpriteBank* SpriteBank;
		StringW Texts[EGDT_COUNT];
		IVideoDriver* Driver;
		bool UseGradient;

		EGUI_SKIN_TYPE Type;
	};
}


#endif

#endif