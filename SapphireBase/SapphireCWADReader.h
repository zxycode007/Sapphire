#ifndef _SAPPHIRE_C_WAD_READER_
#define _SAPPHIRE_C_WAD_READER_

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_

#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"
#include "SapphireCFileList.h"

namespace Sapphire
{
	enum eWADFileTypes
	{
		WAD_FORMAT_UNKNOWN = 0,
		WAD_FORMAT_QUAKE2 = 1,
		WAD_FORMAT_HALFLIFE = 2,

		WAD_CMP_NONE = 0,
		WAD_CMP_LZSS = 1,

		WAD_TYP_NONE = 0,
		WAD_TYP_LABEL = 1,

		WAD_TYP_LUMPY = 64,				// 64 + grab command number
		WAD_TYP_PALETTE = 64,
		WAD_TYP_QTEX = 65,
		WAD_TYP_QPIC = 66,
		WAD_TYP_SOUND = 67,
		WAD_TYP_MIPTEX = 68,
		WAD_TYP_MIPTEX_HALFLIFE = 67,
		WAD_TYP_FONT = 70
	};

	// byte-align structures
#include "SapphirePack.h"

	struct SWADFileHeader
	{
		c8 tag[4];			// type of WAD format WAD2 = quake2, WAD3 = halflife
		UINT32 numlumps;
		UINT32 infotableofs;
	} PACK_STRUCT;

	struct SWADFileEntryOriginal
	{
		UINT32	filepos;
		UINT32 disksize;
		UINT32 size;					// uncompressed
		UINT8 type;
		UINT8 compression;
		UINT8 pad[2];
		UINT8 name[16];				// must be null terminated
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

	struct SWADFileEntry
	{
		path simpleFileName;
		bool operator < (const SWADFileEntry& other) const
		{
			return simpleFileName < other.simpleFileName;
		}

		path wadFileName;
		SWADFileEntryOriginal header;
	};

	//! Archiveloader capable of loading WAD Archives
	class CArchiveLoaderWAD : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderWAD(IFileSystem* fs);

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


	//! reads from WAD
	class CWADReader : public IFileArchive, virtual CFileList
	{
	public:

		CWADReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CWADReader();

		// file archive methods

		//! return the id of the file Archive
		virtual const path& getArchiveName() const;

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const path& filename);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(UINT32 index);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the class Type
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_WAD; }


	private:

		path Type;

		//! scans for a local header, returns false if there is no more local file header.
		bool scanLocalHeader();

		//! splits filename from zip file into useful filenames and paths
		void extractFilename(SWADFileEntry* entry);


		path Base;
		path MountPoint;

		IReadFile* File;

		eWADFileTypes WadType;
		SWADFileHeader Header;

		//core::array<SWADFileEntry> FileInfo;

		IFileSystem* FileSystem;
	};

} 

#endif



#endif