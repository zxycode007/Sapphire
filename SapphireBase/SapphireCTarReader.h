#ifndef _SAPPHIRE_C_TAR_READER_
#define _SAPPHIRE_C_TAR_READER_

#include "SapphireCompileConifg.h"

#ifdef _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"
#include "SapphireCFileList.h"

namespace Sapphire
{
	enum E_TAR_LINK_INDICATOR
	{
		ETLI_REGULAR_FILE_OLD = 0,
		ETLI_REGULAR_FILE = '0',
		ETLI_LINK_TO_ARCHIVED_FILE = '1', // Hard link
		ETLI_SYMBOLIC_LINK = '2',
		ETLI_CHAR_SPECIAL_DEVICE = '3',
		ETLI_BLOCK_SPECIAL_DEVICE = '4',
		ETLI_DIRECTORY = '5',
		ETLI_FIFO_SPECIAL_FILE = '6',
		ETLI_CONTIGUOUS_FILE = '7'
	};

	// byte-align structures
#include "SapphirePack.h"

	struct STarHeader
	{
		c8 FileName[100];
		c8 FileMode[8];
		c8 UserID[8];
		c8 GroupID[8];
		c8 Size[12];
		c8 ModifiedTime[12];
		c8 Checksum[8];
		c8 Link;
		c8 LinkName[100];
		c8 Magic[6];
		c8 USTARVersion[2];
		c8 UserName[32];
		c8 GroupName[32];
		c8 DeviceMajor[8];
		c8 DeviceMinor[8];
		c8 FileNamePrefix[155];
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

	//! Archiveloader capable of loading ZIP Archives
	class CArchiveLoaderTAR : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderTAR(IFileSystem* fs);

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".tar")
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



	class CTarReader : public virtual IFileArchive, virtual CFileList
	{
	public:

		CTarReader(IReadFile* file, bool ignoreCase, bool ignorePaths);

		virtual ~CTarReader();

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(UINT32 index);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the class Type
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_TAR; }

	private:

		UINT32 populateFileList();

		IReadFile* File;
	};
}


#endif


#endif