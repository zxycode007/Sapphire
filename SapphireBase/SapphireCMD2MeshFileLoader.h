#ifndef _SAPPHIRE_C_MD2_MESH_FILE_LOADER_
#define _SAPPHIRE_C_MD2_MESH_FILE_LOADER_


#include "SapphireIMeshLoader.h"


namespace Sapphire
{
	class CAnimatedMeshMD2;

	//! Meshloader capable of loading MD2 files
	class CMD2MeshFileLoader : public IMeshLoader
	{
	public:

		//! Constructor
		CMD2MeshFileLoader();

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".bsp")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! creates/loads an animated mesh from the file.
		//! \return Pointer to the created mesh. Returns 0 if loading failed.
		//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
		//! See IReferenceCounted::drop() for more information.
		virtual IAnimatedMesh* createMesh(IReadFile* file);

	private:
		//! Loads the file data into the mesh
		bool loadFile(IReadFile* file, CAnimatedMeshMD2* mesh);

	};
}


#endif