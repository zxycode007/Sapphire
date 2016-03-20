#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_MD2_LOADER_

#include "SapphireCMD3MeshFileLoader.h"
#include "SapphireCAnimatedMeshMD3.h"


namespace Sapphire
{

	//! Constructor
	CMD3MeshFileLoader::CMD3MeshFileLoader(ISceneManager* smgr)
		: SceneManager(smgr)
	{
	}


	//! destructor
	CMD3MeshFileLoader::~CMD3MeshFileLoader()
	{
	}


	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	bool CMD3MeshFileLoader::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "md3");
	}


	IAnimatedMesh* CMD3MeshFileLoader::createMesh(IReadFile* file)
	{
		CAnimatedMeshMD3 * mesh = new CAnimatedMeshMD3();

		if (mesh->loadModelFile(0, file, SceneManager->getFileSystem(), SceneManager->getVideoDriver()))
			return mesh;

		mesh->drop();
		return 0;
	}
}



#endif