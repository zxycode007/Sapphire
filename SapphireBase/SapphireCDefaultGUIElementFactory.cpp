#ifndef _SAPPHIRE_C_DEFAULT_ELEMENT_FACTORY_
#define _SAPPHIRE_C_DEFAULT_ELEMENT_FACTORY_

#include "SapphireCDefaultGUIElementFactory.h"

#include "SapphireIGUIEnvironment.h"

#include "SapphireIGUIButton.h"
#include "SapphireIGUICheckBox.h"
#include "SapphireIGUIComboBox.h"
#include "SapphireIGUIContextMenu.h"
#include "SapphireIGUIEditBox.h"
#include "SapphireIGUIFileOpenDialog.h"
#include "SapphireIGUIImage.h"
#include "SapphireIGUIListBox.h"
#include "SapphireIGUIMeshViewer.h"
#include "SapphireIGUIScrollBar.h"
#include "SapphireIGUISpinBox.h"
#include "SapphireIGUIStaticText.h"
#include "SapphireIGUITabControl.h"
#include "SapphireIGUITable.h"
#include "SapphireIGUIToolBar.h"
#include "SapphireIGUIWindow.h"
#include "SapphireIGUITreeView.h"
#include "SapphireIGUIInOutFader.h"
#include "SapphireIGUIColorSelectDialog.h"


namespace Sapphire
{

	CDefaultGUIElementFactory::CDefaultGUIElementFactory(IGUIEnvironment* env)
		: Environment(env)
	{

#ifdef _DEBUG
		setDebugName("CDefaultGUIElementFactory");
#endif

		// don't grab the gui environment here to prevent cyclic references
	}


	//! adds an element to the env based on its type id
	IGUIElement* CDefaultGUIElementFactory::addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent)
	{
		switch (type)
		{
		case EGUIET_BUTTON:
			return Environment->addButton(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_CHECK_BOX:
			return Environment->addCheckBox(false, rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_COLOR_SELECT_DIALOG:
			return Environment->addColorSelectDialog(0, true, parent);
		case EGUIET_COMBO_BOX:
			return Environment->addComboBox(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_CONTEXT_MENU:
			return Environment->addContextMenu(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_MENU:
			return Environment->addMenu(parent);
		case EGUIET_EDIT_BOX:
			return Environment->addEditBox(0, rect<SINT32>(0, 0, 100, 100), true, parent);
		case EGUIET_FILE_OPEN_DIALOG:
			return Environment->addFileOpenDialog(0, true, parent);
		case EGUIET_IMAGE:
			return Environment->addImage(0, Position2d(0, 0), true, parent);
		case EGUIET_IN_OUT_FADER:
			return Environment->addInOutFader(0, parent);
		case EGUIET_LIST_BOX:
			return Environment->addListBox(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_MESH_VIEWER:
			return Environment->addMeshViewer(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_MODAL_SCREEN:
			return Environment->addModalScreen(parent);
		case EGUIET_MESSAGE_BOX:
			return Environment->addMessageBox(0, 0, false, 0, parent);
		case EGUIET_SCROLL_BAR:
			return Environment->addScrollBar(false, rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_STATIC_TEXT:
			return Environment->addStaticText(0, rect<SINT32>(0, 0, 100, 100), false, true, parent);
		case EGUIET_TAB:
			return Environment->addTab(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_TAB_CONTROL:
			return Environment->addTabControl(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_TABLE:
			return Environment->addTable(rect<SINT32>(0, 0, 100, 100), parent);
		case EGUIET_TOOL_BAR:
			return Environment->addToolBar(parent);
		case EGUIET_WINDOW:
			return Environment->addWindow(rect<SINT32>(0, 0, 100, 100), false, 0, parent);
		case EGUIET_SPIN_BOX:
			return Environment->addSpinBox(L"0.0", rect<SINT32>(0, 0, 100, 100), true, parent);
		case EGUIET_TREE_VIEW:
			return Environment->addTreeView(rect<SINT32>(0, 0, 100, 100), parent);
		default:
			return 0;
		}
	}


	//! adds an element to the environment based on its type name
	IGUIElement* CDefaultGUIElementFactory::addGUIElement(const c8* typeName, IGUIElement* parent)
	{
		return addGUIElement(getTypeFromName(typeName), parent);
	}


	//! Returns the amount of element types this factory is able to create.
	SINT32 CDefaultGUIElementFactory::getCreatableGUIElementTypeCount() const
	{
		return EGUIET_COUNT;
	}


	//! Returns the type of a createable element type.
	EGUI_ELEMENT_TYPE CDefaultGUIElementFactory::getCreateableGUIElementType(SINT32 idx) const
	{
		if (idx >= 0 && idx<EGUIET_COUNT)
			return (EGUI_ELEMENT_TYPE)idx;

		return EGUIET_ELEMENT;
	}


	//! Returns the type name of a createable element type.
	const c8* CDefaultGUIElementFactory::getCreateableGUIElementTypeName(SINT32 idx) const
	{
		if (idx >= 0 && idx<EGUIET_COUNT)
			return GUIElementTypeNames[idx];

		return 0;
	}


	//! Returns the type name of a createable element type.
	const c8* CDefaultGUIElementFactory::getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const
	{
		// for this factory, type == index

		if (type >= 0 && type<EGUIET_COUNT)
			return GUIElementTypeNames[type];

		return 0;
	}

	EGUI_ELEMENT_TYPE CDefaultGUIElementFactory::getTypeFromName(const c8* name) const
	{
		for (UINT32 i = 0; GUIElementTypeNames[i]; ++i)
			if (!strcmp(name, GUIElementTypeNames[i]))
				return (EGUI_ELEMENT_TYPE)i;

		return EGUIET_ELEMENT;
	}
}

#endif