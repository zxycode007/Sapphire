#ifndef _SAPPHIRE_C_MOUNT_POINT_READER_
#define _SAPPHIRE_C_MOUNT_POINT_READER_

#include "SapphireCompileConifg.h"

#ifdef _SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_

#include "SapphireIFileSystem.h"
#include "SapphireCFileList.h"

namespace Sapphire
{
	//! Archiveloader capable of loading MountPoint Archives
	class CArchiveLoaderMount : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderMount(IFileSystem* fs);

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

	private:
		IFileSystem* FileSystem;
	};

	//! A File Archive which uses a mountpoint
	class CMountPointReader : public virtual IFileArchive, virtual CFileList
	{
	public:

		//! Constructor
		CMountPointReader(IFileSystem *parent, const path& basename,
			bool ignoreCase, bool ignorePaths);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(UINT32 index);

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the class Type
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_FOLDER; }

	private:

		vector<path>::type RealFileNames;

		IFileSystem *Parent;
		void buildDirectory();
	};
}

#endif



#endif