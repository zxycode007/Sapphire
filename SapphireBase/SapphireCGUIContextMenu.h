#ifndef _SAPPHIRE_C_GUI_CONTEXT_MENU_
#define _SAPPHIRE_C_GUI_CONTEXT_MENU_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_

#include "SapphireIGUIContextMenu.h"
#include "SapphireFont.h"

#endif

namespace Sapphire
{
	//! GUI Context menu interface.
	class CGUIContextMenu : public IGUIContextMenu
	{
	public:

		//! constructor
		CGUIContextMenu(IGUIEnvironment* environment,
			IGUIElement* parent, SINT32 id, rect<SINT32> rectangle,
			bool getFocus = true, bool allowFocus = true);

		//! destructor
		virtual ~CGUIContextMenu();

		//! set behavior when menus are closed
		virtual void setCloseHandling(ECONTEXT_MENU_CLOSE onClose);

		//! get current behavior when the menue will be closed
		virtual ECONTEXT_MENU_CLOSE getCloseHandling() const;

		//! Returns amount of menu items
		virtual UINT32 getItemCount() const;

		//! Adds a menu item.
		virtual UINT32 addItem(const wchar_t* text, SINT32 commandid,
			bool enabled, bool hasSubMenu, bool checked, bool autoChecking);

		//! Insert a menu item at specified position.
		virtual UINT32 insertItem(UINT32 idx, const wchar_t* text, SINT32 commandId, bool enabled,
			bool hasSubMenu, bool checked, bool autoChecking);

		//! Find a item which has the given CommandId starting from given index
		virtual SINT32 findItemWithCommandId(SINT32 commandId, UINT32 idxStartSearch) const;

		//! Adds a separator item to the menu
		virtual void addSeparator();

		//! Returns text of the menu item.
		virtual const wchar_t* getItemText(UINT32 idx) const;

		//! Sets text of the menu item.
		virtual void setItemText(UINT32 idx, const wchar_t* text);

		//! Returns if a menu item is enabled
		virtual bool isItemEnabled(UINT32 idx) const;

		//! Sets if the menu item should be enabled.
		virtual void setItemEnabled(UINT32 idx, bool enabled);

		//! Returns if a menu item is checked
		virtual bool isItemChecked(UINT32 idx) const;

		//! Sets if the menu item should be checked.
		virtual void setItemChecked(UINT32 idx, bool enabled);

		//! Removes a menu item
		virtual void removeItem(UINT32 idx);

		//! Removes all menu items
		virtual void removeAllItems();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! Returns the selected item in the menu
		virtual SINT32 getSelectedItem() const;

		//! Returns a pointer to the submenu of an item.
		//! \return Pointer to the submenu of an item.
		virtual IGUIContextMenu* getSubMenu(UINT32 idx) const;

		//! Sets the visible state of this element.
		virtual void setVisible(bool visible);

		//! should the element change the checked status on clicking
		virtual void setItemAutoChecking(UINT32 idx, bool autoChecking);

		//! does the element change the checked status on clicking
		virtual bool getItemAutoChecking(UINT32 idx) const;

		//! Returns command id of a menu item
		virtual SINT32 getItemCommandId(UINT32 idx) const;

		//! Sets the command id of a menu item
		virtual void setItemCommandId(UINT32 idx, SINT32 id);

		//! Adds a sub menu from an element that already exists.
		virtual void setSubMenu(UINT32 index, CGUIContextMenu* menu);

		//! When an eventparent is set it receives events instead of the usual parent element
		virtual void setEventParent(IGUIElement *parent);

		//! Writes attributes of the element.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);

	protected:

		void closeAllSubMenus();
		bool hasOpenSubMenu() const;

		struct SItem
		{
			StringW Text;
			bool IsSeparator;
			bool Enabled;
			bool Checked;
			bool AutoChecking;
			dimension2d<UINT32> Dim;
			SINT32 PosY;
			CGUIContextMenu* SubMenu;
			SINT32 CommandId;
		};

		virtual void recalculateSize();

		//! returns true, if an element was highlighted
		virtual bool highlight(const Position2d& p, bool canOpenSubMenu);

		//! sends a click Returns:
		//! 0 if click went outside of the element,
		//! 1 if a valid button was clicked,
		//! 2 if a nonclickable element was clicked
		virtual UINT32 sendClick(const Position2d& p);

		//! returns the item highlight-area
		virtual rect<SINT32> getHRect(const SItem& i, const rect<SINT32>& absolute) const;

		//! Gets drawing rect of Item
		virtual rect<SINT32> getRect(const SItem& i, const rect<SINT32>& absolute) const;


		vector<SItem>::type Items;
		Position2d Pos;
		IGUIElement* EventParent;
		IGUIFont *LastFont;
		ECONTEXT_MENU_CLOSE CloseHandling;
		SINT32 HighLighted;
		UINT32 ChangeTime;
		bool AllowFocus;
	};

}

#endif