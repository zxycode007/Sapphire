#ifndef _SAPPHIRE_C_BSP_MESH_FILE_LOADER_
#define _SAPPHIRE_C_BSP_MESH_FILE_LOADER_

#include "SapphireIMeshLoader.h"
#include "SapphireIFileSystem.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireIQ3Shader.h"

namespace Sapphire
{
	//! Meshloader capable of loading Quake 3 BSP files and shaders
	class CBSPMeshFileLoader : public IMeshLoader
	{
	public:

		//! Constructor
		CBSPMeshFileLoader(ISceneManager* smgr, IFileSystem* fs);

		//! destructor
		virtual ~CBSPMeshFileLoader();

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".bsp")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! creates/loads an animated mesh from the file.
		//! \return Pointer to the created mesh. Returns 0 if loading failed.
		//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
		//! See IReferenceCounted::drop() for more information.
		virtual IAnimatedMesh* createMesh(IReadFile* file);

	private:

		IFileSystem* FileSystem;
		ISceneManager* SceneManager;

		Quake3::Q3LevelLoadParameter LoadParam;
	};
}

#endif