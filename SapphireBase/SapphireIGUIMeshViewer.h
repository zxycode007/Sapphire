#ifndef _SAPPHIRE_I_GUI_MESH_VIEWER_
#define _SAPPHIRE_I_GUI_MESH_VIEWER_

#include "SapphireIGUIElement.h"

namespace Sapphire
{
	class SMaterial;
	class IAnimatedMesh;

	//! 3d mesh viewing GUI element.
	class IGUIMeshViewer : public IGUIElement
	{
	public:

		//! constructor
		IGUIMeshViewer(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle)
			: IGUIElement(EGUIET_MESH_VIEWER, environment, parent, id, rectangle) {}

		//! Sets the mesh to be shown
		virtual void setMesh(IAnimatedMesh* mesh) = 0;

		//! Gets the displayed mesh
		virtual IAnimatedMesh* getMesh() const = 0;

		//! Sets the material
		virtual void setMaterial(const SMaterial& material) = 0;

		//! Gets the material
		virtual const SMaterial& getMaterial() const = 0;
	};
}

#endif