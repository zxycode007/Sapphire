#ifndef _SAPPHIRE_C_MESH_FILE_LOADER_
#define _SAPPHIRE_C_MESH_FILE_LOADER_

#include "SapphireIMeshLoader.h"
#include "SapphireIFileSystem.h"
#include "SapphireIVideoDriver.h"
#include "SapphireSMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireCDynamicMeshBuffer.h"
#include "SapphireIScenesManager.h"

namespace Sapphire
{
	//! Meshloader capable of loading .irrmesh meshes, the Irrlicht Engine mesh format for static meshes
	class CMeshFileLoader : public IMeshLoader
	{
	public:

		//! Constructor
		CMeshFileLoader(ISceneManager* smgr, IFileSystem* fs);

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".cob")
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! creates/loads an animated mesh from the file.
		//! \return Pointer to the created mesh. Returns 0 if loading failed.
		//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
		//! See IReferenceCounted::drop() for more information.
		virtual IAnimatedMesh* createMesh(IReadFile* file);

	private:

		//! reads a mesh sections and creates a mesh from it
		IAnimatedMesh* readMesh(IXMLReader* reader);

		//! reads a mesh sections and creates a mesh buffer from it
		IMeshBuffer* readMeshBuffer(IXMLReader* reader);

		//! skips an (unknown) section in the irrmesh file
		void skipSection(IXMLReader* reader, bool reportSkipping);

		//! reads a <material> element and stores it in the material section
		void readMaterial(IXMLReader* reader);

		//! parses a float from a char pointer and moves the pointer to
		//! the end of the parsed float
		inline FLOAT32 readFloat(const c8** p);

		//! parses an int from a char pointer and moves the pointer to
		//! the end of the parsed float
		inline SINT32 readInt(const c8** p);

		//! places pointer to next begin of a token
		void findNextNoneWhiteSpace(const c8** p);

		//! places pointer to next begin of a token
		void skipCurrentNoneWhiteSpace(const c8** p);

		//! reads floats from inside of xml element until end of xml element
		void readFloatsInsideElement(IXMLReader* reader, FLOAT32* floats, UINT32 count);

		//! read the mesh buffers
		void readMeshBuffer(IXMLReader* reader, int vertexCount, CDynamicMeshBuffer* sbuffer);

		//! read indices
		void readIndices(IXMLReader* reader, int indexCount, IIndexBuffer& indices);


		// member variables

		ISceneManager* SceneManager;
		IFileSystem* FileSystem;
	};

}

#endif