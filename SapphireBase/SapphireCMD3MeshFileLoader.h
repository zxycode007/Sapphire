#ifndef _SAPPHIRE_C_MD3_MESH_FILE_LOADER_
#define _SAPPHIRE_C_MD3_MESH_FILE_LOADER_

#include "SapphireIMeshLoader.h"
#include "SapphireIFileSystem.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireIQ3Shader.h"


namespace Sapphire
{
	//! Meshloader capable of loading md3 files.
	class CMD3MeshFileLoader : public IMeshLoader
	{
	public:

		//! Constructor
		CMD3MeshFileLoader(ISceneManager* smgr);

		//! destructor
		virtual ~CMD3MeshFileLoader();

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".bsp")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! creates/loads an animated mesh from the file.
		//! \return Pointer to the created mesh. Returns 0 if loading failed.
		//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
		//! See IReferenceCounted::drop() for more information.
		virtual IAnimatedMesh* createMesh(IReadFile* file);

	private:
		ISceneManager* SceneManager;

	};
}




#endif