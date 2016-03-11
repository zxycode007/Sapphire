#ifndef _SAPPHIRE_C_MESH_FILE_WRITER_
#define _SAPPHIRE_C_MESH_FILE_WRITER_

#include "SapphireIMeshWriter.h"
#include "SapphireSVertex.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIFileSystem.h"

namespace Sapphire
{
	class IXMLWriter;
	class IMeshBuffer;

	//! class to write meshes, implementing a IrrMesh (.irrmesh, .xml) writer
	/** This writer implementation has been originally developed for irrEdit and then
	merged out to the Irrlicht Engine */
	class CMeshWriter : public IMeshWriter
	{
	public:

		CMeshWriter(IVideoDriver* driver, IFileSystem* fs);
		virtual ~CMeshWriter();

		//! Returns the type of the mesh writer
		virtual EMESH_WRITER_TYPE getType() const;

		//! writes a mesh 
		virtual bool writeMesh(IWriteFile* file, IMesh* mesh, SINT32 flags = EMWF_NONE);

	protected:

		void writeBoundingBox(const AxisAlignedBox& box);

		void writeMeshBuffer(const IMeshBuffer* buffer);

		void writeMaterial(const SMaterial& material);

		StringW getVectorAsStringLine(const Vector3& v) const;

		StringW getVectorAsStringLine(const Vector2& v) const;

		// member variables:

		IFileSystem* FileSystem;
		IVideoDriver* VideoDriver;
		IXMLWriter* Writer;
	};
}

#endif