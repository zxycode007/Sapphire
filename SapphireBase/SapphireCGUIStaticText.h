#ifndef _SAPPHIRE_C_GUI_STATIC_TEXT_
#define _SAPPHIRE_C_GUI_STATIC_TEXT_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIStaticText.h"

namespace Sapphire
{
	class CGUIStaticText : public IGUIStaticText
	{
	public:

		//! constructor
		CGUIStaticText(const wchar_t* text, bool border, IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, const rect<SINT32>& rectangle,
			bool background = false);

		//! destructor
		virtual ~CGUIStaticText();

		//! draws the element and its children
		virtual void draw();

		//! Sets another skin independent font.
		virtual void setOverrideFont(IGUIFont* font = 0);

		//! Gets the override font (if any)
		virtual IGUIFont* getOverrideFont() const;

		//! Get the font which is used right now for drawing
		virtual IGUIFont* getActiveFont() const;

		//! Sets another color for the text.
		virtual void setOverrideColor(ColourValue color);

		//! Sets another color for the background.
		virtual void setBackgroundColor(ColourValue color);

		//! Sets whether to draw the background
		virtual void setDrawBackground(bool draw);

		//! Gets the background color
		virtual ColourValue getBackgroundColor() const;

		//! Checks if background drawing is enabled
		virtual bool isDrawBackgroundEnabled() const;

		//! Sets whether to draw the border
		virtual void setDrawBorder(bool draw);

		//! Checks if border drawing is enabled
		virtual bool isDrawBorderEnabled() const;

		//! Sets alignment mode for text
		virtual void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);

		//! Gets the override color
		virtual ColourValue getOverrideColor() const;

		//! Sets if the static text should use the overide color or the
		//! color in the gui skin.
		virtual void enableOverrideColor(bool enable);

		//! Checks if an override color is enabled
		virtual bool isOverrideColorEnabled() const;

		//! Set whether the text in this label should be clipped if it goes outside bounds
		virtual void setTextRestrainedInside(bool restrainedInside);

		//! Checks if the text in this label should be clipped if it goes outside bounds
		virtual bool isTextRestrainedInside() const;

		//! Enables or disables word wrap for using the static text as
		//! multiline text control.
		virtual void setWordWrap(bool enable);

		//! Checks if word wrap is enabled
		virtual bool isWordWrapEnabled() const;

		//! Sets the new caption of this element.
		virtual void setText(const wchar_t* text);

		//! Returns the height of the text in pixels when it is drawn.
		virtual SINT32 getTextHeight() const;

		//! Returns the width of the current text, in the current font
		virtual SINT32 getTextWidth() const;

		//! Updates the absolute position, splits text if word wrap is enabled
		virtual void updateAbsolutePosition();

		//! Set whether the string should be interpreted as right-to-left (RTL) text
		/** \note This component does not implement the Unicode bidi standard, the
		text of the component should be already RTL if you call this. The
		main difference when RTL is enabled is that the linebreaks for multiline
		elements are performed starting from the end.
		*/
		virtual void setRightToLeft(bool rtl);

		//! Checks if the text should be interpreted as right-to-left text
		virtual bool isRightToLeft() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		//! Breaks the single text line.
		void breakText();

		EGUI_ALIGNMENT HAlign, VAlign;
		bool Border;
		bool OverrideColorEnabled;
		bool OverrideBGColorEnabled;
		bool WordWrap;
		bool Background;
		bool RestrainTextInside;
		bool RightToLeft;

		ColourValue OverrideColor, BGColor;
		IGUIFont* OverrideFont;
		IGUIFont* LastBreakFont; // stored because: if skin changes, line break must be recalculated.

		vector< StringW >::type BrokenText;
	};
}

#endif

#endif