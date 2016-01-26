#ifndef _SAPPHIRE_C_GUI_EDIT_BOX_
#define _SAPPHIRE_C_GUI_EDIT_BOX_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEditBox.h"
#include "SapphireIOSOperator.h"


namespace Sapphire
{
	class CGUIEditBox : public IGUIEditBox
	{
	public:

		//! constructor
		CGUIEditBox(const wchar_t* text, bool border, IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, const rect<SINT32>& rectangle);

		//! destructor
		virtual ~CGUIEditBox();

		//! Sets another skin independent font.
		virtual void setOverrideFont(IGUIFont* font = 0);

		//! Gets the override font (if any)
		/** \return The override font (may be 0) */
		virtual IGUIFont* getOverrideFont() const;

		//! Get the font which is used right now for drawing
		/** Currently this is the override font when one is set and the
		font of the active skin otherwise */
		virtual IGUIFont* getActiveFont() const;

		//! Sets another color for the text.
		virtual void setOverrideColor(ColourValue color);

		//! Gets the override color
		virtual ColourValue getOverrideColor() const;

		//! Sets if the text should use the overide color or the
		//! color in the gui skin.
		virtual void enableOverrideColor(bool enable);

		//! Checks if an override color is enabled
		/** \return true if the override color is enabled, false otherwise */
		virtual bool isOverrideColorEnabled(void) const;

		//! Sets whether to draw the background
		virtual void setDrawBackground(bool draw);

		//! Turns the border on or off
		virtual void setDrawBorder(bool border);

		//! Enables or disables word wrap for using the edit box as multiline text editor.
		virtual void setWordWrap(bool enable);

		//! Checks if word wrap is enabled
		//! \return true if word wrap is enabled, false otherwise
		virtual bool isWordWrapEnabled() const;

		//! Enables or disables newlines.
		/** \param enable: If set to true, the EGET_EDITBOX_ENTER event will not be fired,
		instead a newline character will be inserted. */
		virtual void setMultiLine(bool enable);

		//! Checks if multi line editing is enabled
		//! \return true if mult-line is enabled, false otherwise
		virtual bool isMultiLineEnabled() const;

		//! Enables or disables automatic scrolling with cursor position
		//! \param enable: If set to true, the text will move around with the cursor position
		virtual void setAutoScroll(bool enable);

		//! Checks to see if automatic scrolling is enabled
		//! \return true if automatic scrolling is enabled, false if not
		virtual bool isAutoScrollEnabled() const;

		//! Gets the size area of the text in the edit box
		//! \return Returns the size in pixels of the text
		virtual dimension2du getTextDimension();

		//! Sets text justification
		virtual void setTextAlignment(EGUI_ALIGNMENT horizontal, EGUI_ALIGNMENT vertical);

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Sets the new caption of this element.
		virtual void setText(const wchar_t* text);

		//! Sets the maximum amount of characters which may be entered in the box.
		//! \param max: Maximum amount of characters. If 0, the character amount is
		//! infinity.
		virtual void setMax(UINT32 max);

		//! Returns maximum amount of characters, previously set by setMax();
		virtual UINT32 getMax() const;

		//! Sets whether the edit box is a password box. Setting this to true will
		/** disable MultiLine, WordWrap and the ability to copy with ctrl+c or ctrl+x
		\param passwordBox: true to enable password, false to disable
		\param passwordChar: the character that is displayed instead of letters */
		virtual void setPasswordBox(bool passwordBox, wchar_t passwordChar = L'*');

		//! Returns true if the edit box is currently a password box.
		virtual bool isPasswordBox() const;

		//! Updates the absolute position, splits text if required
		virtual void updateAbsolutePosition();

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	protected:
		//! Breaks the single text line.
		void breakText();
		//! sets the area of the given line
		void setTextRect(SINT32 line);
		//! returns the line number that the cursor is on
		SINT32 getLineFromPos(SINT32 pos);
		//! adds a letter to the edit box
		void inputChar(wchar_t c);
		//! calculates the current scroll position
		void calculateScrollPos();
		//! calculated the FrameRect
		void calculateFrameRect();
		//! send some gui event to parent
		void sendGuiEvent(EGUI_EVENT_TYPE type);
		//! set text markers
		void setTextMarkers(SINT32 begin, SINT32 end);

		bool processKey(const SEvent& event);
		bool processMouse(const SEvent& event);
		SINT32 getCursorPos(SINT32 x, SINT32 y);

		bool MouseMarking;
		bool Border;
		bool Background;
		bool OverrideColorEnabled;
		SINT32 MarkBegin;
		SINT32 MarkEnd;

		ColourValue OverrideColor;
		IGUIFont *OverrideFont, *LastBreakFont;
		IOSOperator* Operator;

		UINT32 BlinkStartTime;
		SINT32 CursorPos;
		SINT32 HScrollPos, VScrollPos; // scroll position in characters
		UINT32 Max;

		bool WordWrap, MultiLine, AutoScroll, PasswordBox;
		wchar_t PasswordChar;
		EGUI_ALIGNMENT HAlign, VAlign;

		vector< StringW >::type BrokenText;
		vector< SINT32 >::type BrokenTextPositions;

		rect<SINT32> CurrentTextRect, FrameRect; // temporary values
	};
}


#endif


#endif