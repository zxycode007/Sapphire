#ifndef _SAPPHIRE_C_NPK_READER_
#define _SAPPHIRE_C_NPK_READER_

#include "SapphireCompileConifg.h"

#ifdef _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"
#include "SapphireCFileList.h"



namespace Sapphire
{
	//! File header containing location and size of the table of contents
	struct SNPKHeader
	{
		// Don't change the order of these fields!  They must match the order stored on disk.
		c8 Tag[4];
		UINT32 Length;
		UINT32 Offset;
	};

	//! An entry in the NPK file's table of contents.
	struct SNPKFileEntry
	{
		String Name;
		UINT32 Offset;
		UINT32 Length;
	};

	//! Archiveloader capable of loading Nebula Device 2 NPK Archives
	class CArchiveLoaderNPK : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderNPK(IFileSystem* fs);

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".zip")
		virtual bool isALoadableFileFormat(const path& filename) const;

		//! Check if the file might be loaded by this class
		/** Check might look into the file.
		\param file File handle to check.
		\return True if file seems to be loadable. */
		virtual bool isALoadableFileFormat(IReadFile* file) const;

		//! Check to see if the loader can create archives of this type.
		/** Check based on the archive type.
		\param fileType The archive type to check.
		\return True if the archile loader supports this type, false if not */
		virtual bool isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const;

		//! Creates an archive from the filename
		/** \param file File handle to check.
		\return Pointer to newly created archive, or 0 upon error. */
		virtual IFileArchive* createArchive(const path& filename, bool ignoreCase, bool ignorePaths) const;

		//! creates/loads an archive from the file.
		//! \return Pointer to the created archive. Returns 0 if loading failed.
		virtual IFileArchive* createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const;

		//! Returns the type of archive created by this loader
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_NPK; }

	private:
		IFileSystem* FileSystem;
	};


	//! reads from NPK
	class CNPKReader : public virtual IFileArchive, virtual CFileList
	{
	public:

		CNPKReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CNPKReader();

		// file archive methods

		//! return the id of the file Archive
		virtual const path& getArchiveName() const
		{
			return File->getFileName();
		}

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(UINT32 index);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the class Type
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_NPK; }

	private:

		//! scans for a local header, returns false if the header is invalid
		bool scanLocalHeader();
		void readString(String& name);

		IReadFile* File;
	};

}

#endif



#endif