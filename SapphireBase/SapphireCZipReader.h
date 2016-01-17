#ifndef _SAPPHIRE_C_ZIP_READER_
#define _SAPPHIRE_C_ZIP_READER_

#include "SapphireCompileConifg.h"

#ifdef _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_

#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"
#include "SapphireCFileList.h"


namespace Sapphire
{
	// set if the file is encrypted
	const SINT16 ZIP_FILE_ENCRYPTED =		0x0001;
	// the fields crc-32, compressed size and uncompressed size are set to
	// zero in the local header
	const SINT16 ZIP_INFO_IN_DATA_DESCRIPTOR = 0x0008;

	// byte-align structures
#include "SapphirePack.h"

	struct SZIPFileDataDescriptor
	{
		UINT32 CRC32;
		UINT32 CompressedSize;
		UINT32 UncompressedSize;
	} PACK_STRUCT;

	struct SZIPFileHeader
	{
		UINT32 Sig;				// 'PK0304' little endian (0x04034b50)
		SINT16 VersionToExtract;
		SINT16 GeneralBitFlag;
		SINT16 CompressionMethod;
		SINT16 LastModFileTime;
		SINT16 LastModFileDate;
		SZIPFileDataDescriptor DataDescriptor;
		SINT16 FilenameLength;
		SINT16 ExtraFieldLength;
		// filename (variable size)
		// extra field (variable size )
	} PACK_STRUCT;

	struct SZIPFileCentralDirFileHeader
	{
		UINT32 Sig;	// 'PK0102' (0x02014b50)
		UINT16 VersionMadeBy;
		UINT16 VersionToExtract;
		UINT16 GeneralBitFlag;
		UINT16 CompressionMethod;
		UINT16 LastModFileTime;
		UINT16 LastModFileDate;
		UINT32 CRC32;
		UINT32 CompressedSize;
		UINT32 UncompressedSize;
		UINT16 FilenameLength;
		UINT16 ExtraFieldLength;
		UINT16 FileCommentLength;
		UINT16 DiskNumberStart;
		UINT16 InternalFileAttributes;
		UINT32 ExternalFileAttributes;
		UINT32 RelativeOffsetOfLocalHeader;

		// filename (variable size)
		// extra field (variable size)
		// file comment (variable size)

	} PACK_STRUCT;

	struct SZIPFileCentralDirEnd
	{
		UINT32 Sig;			// 'PK0506' end_of central dir signature			// (0x06054b50)
		UINT16 NumberDisk;		// number of this disk
		UINT16 NumberStart;	// number of the disk with the start of the central directory
		UINT16 TotalDisk;		// total number of entries in the central dir on this disk
		UINT16 TotalEntries;	// total number of entries in the central dir
		UINT32 Size;			// size of the central directory
		UINT32 Offset;			// offset of start of centraldirectory with respect to the starting disk number
		UINT16 CommentLength;	// zipfile comment length
		// zipfile comment (variable size)
	} PACK_STRUCT;

	struct SZipFileExtraHeader
	{
		SINT16 ID;
		SINT16 Size;
	} PACK_STRUCT;

	struct SZipFileAESExtraData
	{
		SINT16 Version;
		UINT8 Vendor[2];
		UINT8 EncryptionStrength;
		SINT16 CompressionMode;
	} PACK_STRUCT;

	enum E_GZIP_FLAGS
	{
		EGZF_TEXT_DAT = 1,
		EGZF_CRC16 = 2,
		EGZF_EXTRA_FIELDS = 4,
		EGZF_FILE_NAME = 8,
		EGZF_COMMENT = 16
	};

	struct SGZIPMemberHeader
	{
		UINT16 sig; // 0x8b1f
		UINT8  compressionMethod; // 8 = deflate
		UINT8  flags;
		UINT32 time;
		UINT8  extraFlags; // slow compress = 2, fast compress = 4
		UINT8  operatingSystem;
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

	//! Contains extended info about zip files in the archive
	struct SZipFileEntry
	{
		//! Position of data in the archive file
		SINT32 Offset;

		//! The header for this file containing compression info etc
		SZIPFileHeader header;
	};

	//! Archiveloader capable of loading ZIP Archives
	class CArchiveLoaderZIP : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderZIP(IFileSystem* fs);

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

	/*!
	Zip file Reader written April 2002 by N.Gebhardt.
	*/
	class CZipReader : public virtual IFileArchive, virtual CFileList
	{
	public:

		//! constructor
		CZipReader(IReadFile* file, bool ignoreCase, bool ignorePaths, bool isGZip = false);

		//! destructor
		virtual ~CZipReader();

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(UINT32 index);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the archive type
		virtual E_FILE_ARCHIVE_TYPE getType() const;

	protected:

		//! reads the next file header from a ZIP file, returns false if there are no more headers.
		/* if ignoreGPBits is set, the item will be read despite missing
		file information. This is used when reading items from the central
		directory. */
		bool scanZipHeader(bool ignoreGPBits = false);

		//! the same but for gzip files
		bool scanGZipHeader();

		bool scanCentralDirectoryHeader();

		IReadFile* File;

		// holds extended info about files
		vector<SZipFileEntry>::type FileInfo;

		bool IsGZip;
	};
}

#endif

#endif