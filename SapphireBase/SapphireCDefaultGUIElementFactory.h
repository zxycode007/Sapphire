#ifndef _SAPPHIRE_C_DEFAULT_GUI_ELEMENT_FACTORY_
#define _SAPPHIRE_C_DEFAULT_GUI_ELEMENT_FACTORY_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIElementFactory.h"

namespace Sapphire
{
	class IGUIElement;
	class IGUIEnvironment;

	//! This interface makes it possible to dynamically create gui elements.
	class CDefaultGUIElementFactory : public IGUIElementFactory
	{
	public:

		CDefaultGUIElementFactory(IGUIEnvironment* env);

		//! Adds an element to the gui environment based on its type id.
		/** \param type: Type of the element to add.
		\param parent: Parent scene node of the new element. A value of 0 adds it to the root.
		\return Returns pointer to the new element or 0 if unsuccessful. */
		virtual IGUIElement* addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent = 0);

		//! Adds a GUI element to the GUI Environment based on its type name.
		/** \param typeName: Type name of the element to add. Taken from the GUIElementTypeNames c8* array.
		\param parent: Parent scene node of the new element. A value of 0 adds it to the root.
		\return Returns pointer to the new element or 0 if unsuccessful. */
		virtual IGUIElement* addGUIElement(const c8* typeName, IGUIElement* parent = 0);

		//! Returns the amount of GUI element types this factory is able to create.
		virtual SINT32 getCreatableGUIElementTypeCount() const;

		//! Returns the type of a createable GUI element type based on the index.
		/** \param idx: Index of the element type in this factory. The value must be equal or greater than 0
		and lower than getCreatableGUIElementTypeCount(). */
		virtual EGUI_ELEMENT_TYPE getCreateableGUIElementType(SINT32 idx) const;

		//! Returns the type name of a createable GUI element type based on the index.
		/** \param idx: Index of the element type in this factory. The value must be equal or greater than 0
		and lower than getCreatableGUIElementTypeCount(). */
		virtual const c8* getCreateableGUIElementTypeName(SINT32 idx) const;

		//! Returns the type name of a createable GUI element based on its type.
		/** \param type: Type of the GUI element.
		\return: Returns the name of the type if this factory can create it, otherwise it returns 0. */
		virtual const c8* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const;

	private:

		EGUI_ELEMENT_TYPE getTypeFromName(const c8* name) const;

		IGUIEnvironment* Environment;
	};
}

#endif



#endif