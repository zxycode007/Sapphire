#ifndef _SAPPHIRE_C_GUI_WINDOW_
#define _SAPPHIRE_C_GUI_WINDOW_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIWindow.h"

namespace Sapphire
{
	class IGUIButton;

	class CGUIWindow : public IGUIWindow
	{
	public:

		//! constructor
		CGUIWindow(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! destructor
		virtual ~CGUIWindow();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! update absolute position
		virtual void updateAbsolutePosition();

		//! draws the element and its children
		virtual void draw();

		//! Returns pointer to the close button
		virtual IGUIButton* getCloseButton() const;

		//! Returns pointer to the minimize button
		virtual IGUIButton* getMinimizeButton() const;

		//! Returns pointer to the maximize button
		virtual IGUIButton* getMaximizeButton() const;

		//! Returns true if the window is draggable, false if not
		virtual bool isDraggable() const;

		//! Sets whether the window is draggable
		virtual void setDraggable(bool draggable);

		//! Set if the window background will be drawn
		virtual void setDrawBackground(bool draw);

		//! Get if the window background will be drawn
		virtual bool getDrawBackground() const;

		//! Set if the window titlebar will be drawn
		//! Note: If the background is not drawn, then the titlebar is automatically also not drawn
		virtual void setDrawTitlebar(bool draw);

		//! Get if the window titlebar will be drawn
		virtual bool getDrawTitlebar() const;

		//! Returns the rectangle of the drawable area (without border and without titlebar)
		virtual rect<SINT32> getClientRect() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	protected:

		void updateClientRect();
		void refreshSprites();

		IGUIButton* CloseButton;
		IGUIButton* MinButton;
		IGUIButton* RestoreButton;
		rect<SINT32> ClientRect;
		ColourValue CurrentIconColor;

		Position2d DragStart;
		bool Dragging, IsDraggable;
		bool DrawBackground;
		bool DrawTitlebar;
		bool IsActive;
	};
}

#endif

#endif