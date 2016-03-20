#ifndef _SAPPHIRE_C_GUI_ENVIRONMENT_
#define _SAPPHIRE_C_GUI_ENVIRONMENT_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIEnvironment.h"
#include "SapphireIGUIElement.h"
#include "SapphireIFileSystem.h"
#include "SapphireIOSOperator.h"


namespace Sapphire
{
	class IXMLWriter;



	class CGUIEnvironment : public IGUIEnvironment, public IGUIElement
	{
	public:

		//! constructor
		CGUIEnvironment(IFileSystem* fs, IVideoDriver* driver, IOSOperator* op);

		//! destructor
		virtual ~CGUIEnvironment();

		//! draws all gui elements
		virtual void drawAll();

		//! returns the current video driver
		virtual IVideoDriver* getVideoDriver() const;

		//! returns pointer to the filesystem
		virtual IFileSystem* getFileSystem() const;

		//! returns a pointer to the OS operator
		virtual IOSOperator* getOSOperator() const;

		//! posts an input event to the environment
		virtual bool postEventFromUser(const SEvent& event);

		//! This sets a new event receiver for gui events. Usually you do not have to
		//! use this method, it is used by the internal engine.
		virtual void setUserEventReceiver(IEventReceiver* evr);

		//! removes all elements from the environment
		virtual void clear();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! returns the current gui skin
		virtual IGUISkin* getSkin() const;

		//! Sets a new GUI Skin
		virtual void setSkin(IGUISkin* skin);

		//! Creates a new GUI Skin based on a template.
		/** \return Returns a pointer to the created skin.
		If you no longer need the skin, you should call IGUISkin::drop().
		See IReferenceCounted::drop() for more information. */
		virtual IGUISkin* createSkin(EGUI_SKIN_TYPE type);

		//! Creates the image list from the given texture.
		virtual IGUIImageList* createImageList(ITexture* texture,
			dimension2d<SINT32> imageSize, bool useAlphaChannel);

		//! returns the font
		virtual IGUIFont* getFont(const path& filename);

		//! add an externally loaded font
		virtual IGUIFont* addFont(const path& name, IGUIFont* font);

		//! remove loaded font
		virtual void removeFont(IGUIFont* font);

		//! returns default font
		virtual IGUIFont* getBuiltInFont() const;

		//! returns the sprite bank
		virtual IGUISpriteBank* getSpriteBank(const path& filename);

		//! returns the sprite bank
		virtual IGUISpriteBank* addEmptySpriteBank(const path& name);

		//! adds an button. The returned pointer must not be dropped.
		virtual IGUIButton* addButton(const rect<SINT32>& rectangle, IGUIElement* parent = 0, SINT32 id = -1, const wchar_t* text = 0, const wchar_t* tooltiptext = 0);

		//! adds a window. The returned pointer must not be dropped.
		virtual IGUIWindow* addWindow(const rect<SINT32>& rectangle, bool modal = false,
			const wchar_t* text = 0, IGUIElement* parent = 0, SINT32 id = -1);

		//! adds a modal screen. The returned pointer must not be dropped.
		virtual IGUIElement* addModalScreen(IGUIElement* parent);

		//! Adds a message box.
		virtual IGUIWindow* addMessageBox(const wchar_t* caption, const wchar_t* text = 0,
			bool modal = true, SINT32 flag = EMBF_OK, IGUIElement* parent = 0, SINT32 id = -1, ITexture* image = 0);

		//! adds a scrollbar. The returned pointer must not be dropped.
		virtual IGUIScrollBar* addScrollBar(bool horizontal, const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds an image element.
		virtual IGUIImage* addImage(ITexture* image,Position2d pos,
			bool useAlphaChannel = true, IGUIElement* parent = 0, SINT32 id = -1, const wchar_t* text = 0);

		//! adds an image. The returned pointer must not be dropped.
		virtual IGUIImage* addImage(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1, const wchar_t* text = 0, bool useAlphaChannel = true);

		//! adds a checkbox
		virtual IGUICheckBox* addCheckBox(bool checked, const rect<SINT32>& rectangle, IGUIElement* parent = 0, SINT32 id = -1, const wchar_t* text = 0);

		//! adds a list box
		virtual IGUIListBox* addListBox(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1, bool drawBackground = false);

		//! adds a tree view
		virtual IGUITreeView* addTreeView(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1, bool drawBackground = false,
			bool scrollBarVertical = true, bool scrollBarHorizontal = false);

		//! adds an mesh viewer. The returned pointer must not be dropped.
		virtual IGUIMeshViewer* addMeshViewer(const rect<SINT32>& rectangle, IGUIElement* parent = 0, SINT32 id = -1, const wchar_t* text = 0);

		//! Adds a file open dialog.
		virtual IGUIFileOpenDialog* addFileOpenDialog(const wchar_t* title = 0,
			bool modal = true, IGUIElement* parent = 0, SINT32 id = -1,
			bool restoreCWD = false, wchar_t* startDir = 0);

		//! Adds a color select dialog.
		virtual IGUIColorSelectDialog* addColorSelectDialog(const wchar_t* title = 0, bool modal = true, IGUIElement* parent = 0, SINT32 id = -1);

		//! adds a static text. The returned pointer must not be dropped.
		virtual IGUIStaticText* addStaticText(const wchar_t* text, const rect<SINT32>& rectangle,
			bool border = false, bool wordWrap = true, IGUIElement* parent = 0, SINT32 id = -1, bool drawBackground = false);

		//! Adds an edit box. The returned pointer must not be dropped.
		virtual IGUIEditBox* addEditBox(const wchar_t* text, const rect<SINT32>& rectangle,
			bool border = false, IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a spin box to the environment
		virtual IGUISpinBox* addSpinBox(const wchar_t* text, const rect<SINT32>& rectangle,
			bool border = false, IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a tab control to the environment.
		virtual IGUITabControl* addTabControl(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, bool fillbackground = false, bool border = true, SINT32 id = -1);

		//! Adds tab to the environment.
		virtual IGUITab* addTab(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a context menu to the environment.
		virtual IGUIContextMenu* addContextMenu(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a menu to the environment.
		virtual IGUIContextMenu* addMenu(IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a toolbar to the environment. It is like a menu is always placed on top
		//! in its parent, and contains buttons.
		virtual IGUIToolBar* addToolBar(IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a combo box to the environment.
		virtual IGUIComboBox* addComboBox(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1);

		//! Adds a table element.
		virtual IGUITable* addTable(const rect<SINT32>& rectangle,
			IGUIElement* parent = 0, SINT32 id = -1, bool drawBackground = false);

		//! sets the focus to an element
		virtual bool setFocus(IGUIElement* element);

		//! removes the focus from an element
		virtual bool removeFocus(IGUIElement* element);

		//! Returns if the element has focus
		virtual bool hasFocus(IGUIElement* element) const;

		//! Returns the element with the focus
		virtual IGUIElement* getFocus() const;

		//! Returns the element last known to be under the mouse
		virtual IGUIElement* getHovered() const;

		//! Adds an element for fading in or out.
		virtual IGUIInOutFader* addInOutFader(const rect<SINT32>* rectangle = 0, IGUIElement* parent = 0, SINT32 id = -1);

		//! Returns the root gui element.
		virtual IGUIElement* getRootGUIElement();

		virtual void OnPostRender(UINT32 time);

		//! Returns the default element factory which can create all built in elements
		virtual IGUIElementFactory* getDefaultGUIElementFactory() const;

		//! Adds an element factory to the gui environment.
		/** Use this to extend the gui environment with new element types which it should be
		able to create automaticly, for example when loading data from xml files. */
		virtual void registerGUIElementFactory(IGUIElementFactory* factoryToAdd);

		//! Returns amount of registered scene node factories.
		virtual UINT32 getRegisteredGUIElementFactoryCount() const;

		//! Returns a scene node factory by index
		virtual IGUIElementFactory* getGUIElementFactory(UINT32 index) const;

		//! Adds a GUI Element by its name
		virtual IGUIElement* addGUIElement(const c8* elementName, IGUIElement* parent = 0);

		//! Saves the current gui into a file.
		/** \param filename: Name of the file.
		\param start: The element to start saving from.
		if not specified, the root element will be used */
		virtual bool saveGUI(const path& filename, IGUIElement* start = 0);

		//! Saves the current gui into a file.
		/** \param file: The file to save the GUI to.
		\param start: The element to start saving from.
		if not specified, the root element will be used */
		virtual bool saveGUI(IWriteFile* file, IGUIElement* start = 0);

		//! Loads the gui. Note that the current gui is not cleared before.
		/** \param filename: Name of the file.
		\param parent: The parent of all loaded GUI elements,
		if not specified, the root element will be used */
		virtual bool loadGUI(const path& filename, IGUIElement* parent = 0);

		//! Loads the gui. Note that the current gui is not cleared before.
		/** \param file: IReadFile to load the GUI from
		\param parent: The parent of all loaded GUI elements,
		if not specified, the root element will be used */
		virtual bool loadGUI(IReadFile* file, IGUIElement* parent = 0);

		//! Writes attributes of the environment
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the environment.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! writes an element
		virtual void writeGUIElement(IXMLWriter* writer, IGUIElement* node);

		//! reads an element
		virtual void readGUIElement(IXMLReader* reader, IGUIElement* node);

	private:

		IGUIElement* getNextElement(bool reverse = false, bool group = false);

		void updateHoveredElement(Position2d mousePos);

		void loadBuiltInFont();

		struct SFont
		{
			SNamedPath NamedPath;
			IGUIFont* Font;

			bool operator < (const SFont& other) const
			{
				return (NamedPath < other.NamedPath);
			}

			bool operator ==(const SFont& other) const
			{
				return NamedPath == other.NamedPath;
				
			}
		};

		struct SSpriteBank
		{
			SNamedPath NamedPath;
			IGUISpriteBank* Bank;

			bool operator < (const SSpriteBank& other) const
			{
				return (NamedPath < other.NamedPath);
			}

			bool operator ==(const SSpriteBank& other) const
			{
				return NamedPath == other.NamedPath;
			}
		};

		struct SToolTip
		{
			IGUIStaticText* Element;
			UINT32 LastTime;
			UINT32 EnterTime;
			UINT32 LaunchTime;
			UINT32 RelaunchTime;
		};

		SToolTip ToolTip;

		vector<IGUIElementFactory*>::type GUIElementFactoryList;

		vector<SFont>::type Fonts;
		vector<SSpriteBank>::type Banks;
		IVideoDriver* Driver;
		IGUIElement* Hovered;
		IGUIElement* HoveredNoSubelement;	// subelements replaced by their parent, so you only have 'real' elements here
		IGUIElement* Focus;
		Position2d LastHoveredMousePos;
		IGUISkin* CurrentSkin;
		IFileSystem* FileSystem;
		IEventReceiver* UserReceiver;
		IOSOperator* Operator;
		static const path DefaultFontName;
	};
}

#endif
#endif