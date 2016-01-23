#ifndef _SAPPHIRE_C_GUI_COLOR_SELECT_DIALOG_
#define _SAPPHIRE_C_GUI_COLOR_SELECT_DIALOG_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIColorSelectDialog.h"
#include "SapphireIGUIButton.h"
#include "SapphireIGUISpinBox.h"
#include "SapphireIGUIImage.h"

namespace Sapphire
{
	class CGUIColorSelectDialog : public IGUIColorSelectDialog
	{
	public:

		//! constructor
		CGUIColorSelectDialog(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, SINT32 id);

		//! destructor
		virtual ~CGUIColorSelectDialog();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		virtual ColourValue getColor();
		virtual SColorHSL getColorHSL();

	private:

		//! sends the event that the file has been selected.
		void sendSelectedEvent();

		//! sends the event that the file choose process has been canceld
		void sendCancelEvent();

		Position2d DragStart;
		bool Dragging;
		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;

		vector<IGUISpinBox*>::type Battery;

		struct SColorCircle
		{
			IGUIImage * Control;
			ITexture * Texture;
		};
		SColorCircle ColorRing;

		void buildColorRing(const dimension2d<UINT32> & dim, SINT32 supersample, const ColourValue& borderColor);
	};
}

#endif

#endif