#ifndef _SAPPHIRE_C_FILE_SYSTEM_
#define _SAPPHIRE_C_FILE_SYSTEM_

#include "SapphireIFileSystem.h"

namespace Sapphire
{
	class CZipReader;
	class CPakReader;
	class CMountPointReader;

	/*!
	FileSystem which uses normal files and one zipfile
	*/
	class CFileSystem : public IFileSystem
	{
	public:

		//! constructor
		CFileSystem();

		//! destructor
		virtual ~CFileSystem();

		//! opens a file for read access
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! Creates an IReadFile interface for accessing memory like a file.
		virtual IReadFile* createMemoryReadFile(void* memory, SINT32 len, const path& fileName, bool deleteMemoryWhenDropped = false);

		//! Creates an IReadFile interface for accessing files inside files
		virtual IReadFile* createLimitReadFile(const path& fileName, IReadFile* alreadyOpenedFile, long pos, long areaSize);

		//! Creates an IWriteFile interface for accessing memory like a file.
		virtual IWriteFile* createMemoryWriteFile(void* memory, SINT32 len, const path& fileName, bool deleteMemoryWhenDropped = false);

		//! Opens a file for write access.
		virtual IWriteFile* createAndWriteFile(const path& filename, bool append = false);

		//! Adds an archive to the file system.
		virtual bool addFileArchive(const path& filename,
			bool ignoreCase = true, bool ignorePaths = true,
			E_FILE_ARCHIVE_TYPE archiveType = EFAT_UNKNOWN,
			const String& password = "",
			IFileArchive** retArchive = 0);

		//! Adds an archive to the file system.
		virtual bool addFileArchive(IReadFile* file, bool ignoreCase = true,
			bool ignorePaths = true,
			E_FILE_ARCHIVE_TYPE archiveType = EFAT_UNKNOWN,
			const String& password = "",
			IFileArchive** retArchive = 0);

		//! Adds an archive to the file system.
		virtual bool addFileArchive(IFileArchive* archive);

		//! move the hirarchy of the filesystem. moves sourceIndex relative up or down
		virtual bool moveFileArchive(UINT32 sourceIndex, SINT32 relative);

		//! Adds an external archive loader to the engine.
		virtual void addArchiveLoader(IArchiveLoader* loader);

		//! Returns the total number of archive loaders added.
		virtual UINT32 getArchiveLoaderCount() const;

		//! Gets the archive loader by index.
		virtual IArchiveLoader* getArchiveLoader(UINT32 index) const;

		//! gets the file archive count
		virtual UINT32 getFileArchiveCount() const;

		//! gets an archive
		virtual IFileArchive* getFileArchive(UINT32 index);

		//! removes an archive from the file system.
		virtual bool removeFileArchive(UINT32 index);

		//! removes an archive from the file system.
		virtual bool removeFileArchive(const path& filename);

		//! Removes an archive from the file system.
		virtual bool removeFileArchive(const IFileArchive* archive);

		//! Returns the string of the current working directory
		virtual const path& getWorkingDirectory();

		//! Changes the current Working Directory to the string given.
		//! The string is operating system dependent. Under Windows it will look
		//! like this: "drive:\directory\sudirectory\"
		virtual bool changeWorkingDirectoryTo(const path& newDirectory);

		//! Converts a relative path to an absolute (unique) path, resolving symbolic links
		virtual path getAbsolutePath(const path& filename) const;

		//! Returns the directory a file is located in.
		/** \param filename: The file to get the directory from */
		virtual path getFileDir(const path& filename) const;

		//! Returns the base part of a filename, i.e. the name without the directory
		//! part. If no directory is prefixed, the full name is returned.
		/** \param filename: The file to get the basename from */
		virtual path getFileBasename(const path& filename, bool keepExtension = true) const;

		//! flatten a path and file name for example: "/you/me/../." becomes "/you"
		virtual path& flattenFilename(path& directory, const path& root = "/") const;

		//! Get the relative filename, relative to the given directory
		virtual path getRelativeFilename(const path& filename, const path& directory) const;

		virtual EFileSystemType setFileListSystem(EFileSystemType listType);

		//! Creates a list of files and directories in the current working directory
		//! and returns it.
		virtual IFileList* createFileList();

		//! Creates an empty filelist
		virtual IFileList* createEmptyFileList(const path& path, bool ignoreCase, bool ignorePaths);

		//! determines if a file exists and would be able to be opened.
		virtual bool existFile(const path& filename) const;

		//! Creates a XML Reader from a file.
		virtual IXMLReader* createXMLReader(const path& filename);

		//! Creates a XML Reader from a file.
		virtual IXMLReader* createXMLReader(IReadFile* file);

		//! Creates a XML Reader from a file.
		virtual IXMLReaderUTF8* createXMLReaderUTF8(const path& filename);

		//! Creates a XML Reader from a file.
		virtual IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file);

		//! Creates a XML Writer from a file.
		virtual IXMLWriter* createXMLWriter(const path& filename);

		//! Creates a XML Writer from a file.
		virtual IXMLWriter* createXMLWriter(IWriteFile* file);

		//! Creates a new empty collection of attributes, usable for serialization and more.
		virtual IAttributes* createEmptyAttributes(IVideoDriver* driver);

	private:

		// don't expose, needs refactoring
		bool changeArchivePassword(const path& filename,
			const String& password,
			IFileArchive** archive = 0);

		//! Currently used FileSystemType
		EFileSystemType FileSystemType;
		//! WorkingDirectory for Native and Virtual filesystems
		path WorkingDirectory[2];
		//! currently attached ArchiveLoaders
		vector<IArchiveLoader*>::type ArchiveLoader;
		//! currently attached Archives
		vector<IFileArchive*>::type FileArchives;
	};
}

#endif