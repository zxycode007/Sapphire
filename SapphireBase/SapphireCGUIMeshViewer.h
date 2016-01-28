#ifndef _SAPPHIRE_C_GUI_MESH_VIEWER_
#define _SAPPHIRE_C_GUI_MESH_VIEWER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUIMeshViewer.h"
#include "SapphireSMaterial.h"


namespace Sapphire
{
	class CGUIMeshViewer : public IGUIMeshViewer
	{
	public:

		//! constructor
		CGUIMeshViewer(IGUIEnvironment* environment, IGUIElement* parent, SINT32 id, rect<SINT32> rectangle);

		//! destructor
		virtual ~CGUIMeshViewer();

		//! sets the mesh to be shown
		virtual void setMesh(IAnimatedMesh* mesh);

		//! Gets the displayed mesh
		virtual IAnimatedMesh* getMesh() const;

		//! sets the material
		virtual void setMaterial(const SMaterial& material);

		//! gets the material
		virtual const SMaterial& getMaterial() const;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

	private:

		SMaterial Material;
		IAnimatedMesh* Mesh;
	};
}

#endif

#endif