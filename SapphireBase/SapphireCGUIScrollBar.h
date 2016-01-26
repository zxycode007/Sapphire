#ifndef _SAPPHIRE_C_GUI_SCROLL_BAR_
#define _SAPPHIRE_C_GUI_SCROLL_BAR_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIScrollBar.h"
#include "SapphireIGUIButton.h"

namespace Sapphire
{
	class CGUIScrollBar : public IGUIScrollBar
	{
	public:

		//! constructor
		CGUIScrollBar(bool horizontal, IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, rect<SINT32> rectangle,
			bool noclip = false);

		//! destructor
		virtual ~CGUIScrollBar();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		virtual void OnPostRender(UINT32 timeMs);


		//! gets the maximum value of the scrollbar.
		virtual SINT32 getMax() const;

		//! sets the maximum value of the scrollbar.
		virtual void setMax(SINT32 max);

		//! gets the minimum value of the scrollbar.
		virtual SINT32 getMin() const;

		//! sets the minimum value of the scrollbar.
		virtual void setMin(SINT32 min);

		//! gets the small step value
		virtual SINT32 getSmallStep() const;

		//! sets the small step value
		virtual void setSmallStep(SINT32 step);

		//! gets the large step value
		virtual SINT32 getLargeStep() const;

		//! sets the large step value
		virtual void setLargeStep(SINT32 step);

		//! gets the current position of the scrollbar
		virtual SINT32 getPos() const;

		//! sets the position of the scrollbar
		virtual void setPos(SINT32 pos);

		//! updates the rectangle
		virtual void updateAbsolutePosition();

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	private:

		void refreshControls();
		SINT32 getPosFromMousePos(const Position2d &p) const;

		IGUIButton* UpButton;
		IGUIButton* DownButton;

		rect<SINT32> SliderRect;

		bool Dragging;
		bool Horizontal;
		bool DraggedBySlider;
		bool TrayClick;
		SINT32 Pos;
		SINT32 DrawPos;
		SINT32 DrawHeight;
		SINT32 Min;
		SINT32 Max;
		SINT32 SmallStep;
		SINT32 LargeStep;
		SINT32 DesiredPos;
		UINT32 LastChange;
		ColourValue CurrentIconColor;

		FLOAT32 range() const { return (FLOAT32)(Max - Min); }
	};
}
 
#endif



#endif