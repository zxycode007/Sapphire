#ifndef _SAPPHIRE_C_GUI_TABLE_
#define _SAPPHIRE_C_GUI_TABLE_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUITable.h"

namespace Sapphire
{
	class IGUIFont;
	class IGUIScrollBar;

	class CGUITable : public IGUITable
	{
	public:
		//! constructor
		CGUITable(IGUIEnvironment* environment, IGUIElement* parent,
			SINT32 id, const rect<SINT32>& rectangle, bool clip = true,
			bool drawBack = false, bool moveOverSelect = true);

		//! destructor
		~CGUITable();

		//! Adds a column
		//! If columnIndex is outside the current range, do push new colum at the end
		virtual void addColumn(const wchar_t* caption, SINT32 columnIndex = -1);

		//! remove a column from the table
		virtual void removeColumn(UINT32 columnIndex);

		//! Returns the number of columns in the table control
		virtual SINT32 getColumnCount() const;

		//! Makes a column active. This will trigger an ordering process.
		/** \param idx: The id of the column to make active.
		\return True if successful. */
		virtual bool setActiveColumn(SINT32 columnIndex, bool doOrder = false);

		//! Returns which header is currently active
		virtual SINT32 getActiveColumn() const;

		//! Returns the ordering used by the currently active column
		virtual EGUI_ORDERING_MODE getActiveColumnOrdering() const;

		//! set a column width
		virtual void setColumnWidth(UINT32 columnIndex, UINT32 width);

		//! Get the width of a column
		virtual UINT32 getColumnWidth(UINT32 columnIndex) const;

		//! columns can be resized by drag 'n drop
		virtual void setResizableColumns(bool resizable);

		//! can columns be resized by dran 'n drop?
		virtual bool hasResizableColumns() const;

		//! This tells the table control which ordering mode should be used when
		//! a column header is clicked.
		/** \param columnIndex: The index of the column header.
		\param state: If true, a EGET_TABLE_HEADER_CHANGED message will be sent and you can order the table data as you whish.*/
		//! \param mode: One of the modes defined in EGUI_COLUMN_ORDERING
		virtual void setColumnOrdering(UINT32 columnIndex, EGUI_COLUMN_ORDERING mode);

		//! Returns which row is currently selected
		virtual SINT32 getSelected() const;

		//! set wich row is currently selected
		virtual void setSelected(SINT32 index);

		//! Returns amount of rows in the tabcontrol
		virtual SINT32 getRowCount() const;

		//! adds a row to the table
		/** \param rowIndex: zero based index of rows. The row will be
		inserted at this position. If a row already exists
		there, it will be placed after it. If the row is larger
		than the actual number of rows by more than one, it
		won't be created. Note that if you create a row that is
		not at the end, there might be performance issues*/
		virtual UINT32 addRow(UINT32 rowIndex);

		//! Remove a row from the table
		virtual void removeRow(UINT32 rowIndex);

		//! clear the table rows, but keep the columns intact
		virtual void clearRows();

		//! Swap two row positions. This is useful for a custom ordering algo.
		virtual void swapRows(UINT32 rowIndexA, UINT32 rowIndexB);

		//! This tells the table to start ordering all the rows. You
		//! need to explicitly tell the table to reorder the rows when
		//! a new row is added or the cells data is changed. This makes
		//! the system more flexible and doesn't make you pay the cost
		//! of ordering when adding a lot of rows.
		//! \param columnIndex: When set to -1 the active column is used.
		virtual void orderRows(SINT32 columnIndex = -1, EGUI_ORDERING_MODE mode = EGOM_NONE);


		//! Set the text of a cell
		virtual void setCellText(UINT32 rowIndex, UINT32 columnIndex, const StringW& text);

		//! Set the text of a cell, and set a color of this cell.
		virtual void setCellText(UINT32 rowIndex, UINT32 columnIndex, const StringW& text, ColourValue color);

		//! Set the data of a cell
		//! data will not be serialized.
		virtual void setCellData(UINT32 rowIndex, UINT32 columnIndex, void *data);

		//! Set the color of a cell text
		virtual void setCellColor(UINT32 rowIndex, UINT32 columnIndex, ColourValue color);

		//! Get the text of a cell
		virtual const wchar_t* getCellText(UINT32 rowIndex, UINT32 columnIndex) const;

		//! Get the data of a cell
		virtual void* getCellData(UINT32 rowIndex, UINT32 columnIndex) const;

		//! clears the table, deletes all items in the table
		virtual void clear();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent &event);

		//! draws the element and its children
		virtual void draw();

		//! Set flags, as defined in EGUI_TABLE_DRAW_FLAGS, which influence the layout
		virtual void setDrawFlags(SINT32 flags);

		//! Get the flags, as defined in EGUI_TABLE_DRAW_FLAGS, which influence the layout
		virtual SINT32 getDrawFlags() const;

		//! Writes attributes of the object.
		//! Implement this to expose the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml serialization purposes.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the object.
		//! Implement this to set the attributes of your scene node animator for
		//! scripting languages, editors, debuggers or xml deserialization purposes.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

	protected:
		virtual void refreshControls();
		virtual void checkScrollbars();

	private:

		struct Cell
		{
			Cell() : IsOverrideColor(false), Data(0)  {}
			StringW Text;
			StringW BrokenText;
			bool IsOverrideColor;
			ColourValue Color;
			void *Data;
		};

		struct Row
		{
			Row() {}
			vector<Cell>::type Items;
		};

		struct Column
		{
			Column() : Width(0), OrderingMode(EGCO_NONE) {}
			StringW Name;
			UINT32 Width;
			EGUI_COLUMN_ORDERING OrderingMode;
		};

		void breakText(const StringW &text, StringW & brokenText, UINT32 cellWidth);
		void selectNew(SINT32 ypos, bool onlyHover = false);
		bool selectColumnHeader(SINT32 xpos, SINT32 ypos);
		bool dragColumnStart(SINT32 xpos, SINT32 ypos);
		bool dragColumnUpdate(SINT32 xpos);
		void recalculateHeights();
		void recalculateWidths();

		vector< Column >::type Columns;
		vector< Row >::type Rows;
		IGUIFont* Font;
		IGUIScrollBar* VerticalScrollBar;
		IGUIScrollBar* HorizontalScrollBar;
		bool Clip;
		bool DrawBack;
		bool MoveOverSelect;
		bool Selecting;
		SINT32  CurrentResizedColumn;
		SINT32  ResizeStart;
		bool ResizableColumns;

		SINT32 ItemHeight;
		SINT32 TotalItemHeight;
		SINT32 TotalItemWidth;
		SINT32 Selected;
		SINT32 CellHeightPadding;
		SINT32 CellWidthPadding;
		SINT32 ActiveTab;
		EGUI_ORDERING_MODE CurrentOrdering;
		SINT32 DrawFlags;
	};
}


#endif
#endif