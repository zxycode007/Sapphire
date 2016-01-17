#include "SapphireCPakReader.h"

#ifdef _SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_

#include "SapphireOS.h"
#include "SapphireCoreUtil.h"


namespace Sapphire
{
	inline bool isHeaderValid(const SPAKFileHeader& header)
	{
		const c8* tag = header.tag;
		return tag[0] == 'P' &&
			tag[1] == 'A' &&
			tag[2] == 'C' &&
			tag[3] == 'K';
	}

	//! Constructor
	CArchiveLoaderPAK::CArchiveLoaderPAK(IFileSystem* fs)
		: FileSystem(fs)
	{
#ifdef _DEBUG
		setDebugName("CArchiveLoaderPAK");
#endif
	}


	//! returns true if the file maybe is able to be loaded by this class
	bool CArchiveLoaderPAK::isALoadableFileFormat(const path& filename) const
	{
		return hasFileExtension(filename, "pak");
	}

	//! Check to see if the loader can create archives of this type.
	bool CArchiveLoaderPAK::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
	{
		return fileType == EFAT_PAK;
	}

	//! Creates an archive from the filename
	/** \param file File handle to check.
	\return Pointer to newly created archive, or 0 upon error. */
	IFileArchive* CArchiveLoaderPAK::createArchive(const path& filename, bool ignoreCase, bool ignorePaths) const
	{
		IFileArchive *archive = 0;
		IReadFile* file = FileSystem->createAndOpenFile(filename);

		if (file)
		{
			archive = createArchive(file, ignoreCase, ignorePaths);
			file->drop();
		}

		return archive;
	}

	//! creates/loads an archive from the file.
	//! \return Pointer to the created archive. Returns 0 if loading failed.
	IFileArchive* CArchiveLoaderPAK::createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const
	{
		IFileArchive *archive = 0;
		if (file)
		{
			file->seek(0);
			archive = new CPakReader(file, ignoreCase, ignorePaths);
		}
		return archive;
	}


	//! Check if the file might be loaded by this class
	/** Check might look into the file.
	\param file File handle to check.
	\return True if file seems to be loadable. */
	bool CArchiveLoaderPAK::isALoadableFileFormat(IReadFile* file) const
	{
		SPAKFileHeader header;

		file->read(&header, sizeof(header));

		return isHeaderValid(header);
	}


	/*!
	PAK Reader
	*/
	CPakReader::CPakReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
		: CFileList((file ? file->getFileName() : path("")), ignoreCase, ignorePaths), File(file)
	{
#ifdef _DEBUG
		setDebugName("CPakReader");
#endif

		if (File)
		{
			File->grab();
			scanLocalHeader();
			sort();
		}
	}


	CPakReader::~CPakReader()
	{
		if (File)
			File->drop();
	}


	const IFileList* CPakReader::getFileList() const
	{
		return this;
	}

	bool CPakReader::scanLocalHeader()
	{
		SPAKFileHeader header;

		// Read and validate the header
		File->read(&header, sizeof(header));
		if (!isHeaderValid(header))
			return false;

		// Seek to the table of contents
#if (SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN)
		header.offset = Byteswap::byteswap(header.offset);
		header.length = Byteswap::byteswap(header.length);
#endif
		File->seek(header.offset);

		const int numberOfFiles = header.length / sizeof(SPAKFileEntry);

		// Loop through each entry in the table of contents
		for (int i = 0; i < numberOfFiles; i++)
		{
			// read an entry
			SPAKFileEntry entry;
			File->read(&entry, sizeof(entry));

#ifdef _DEBUG
			Printer::log(entry.name);
#endif

#if (SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN)
			entry.offset = Byteswap::byteswap(entry.offset);
			entry.length = Byteswap::byteswap(entry.length);
#endif

			addItem(path(entry.name), entry.offset, entry.length, false);
		}
		return true;
	}


	//! opens a file by file name
	IReadFile* CPakReader::createAndOpenFile(const path& filename)
	{
		SINT32 index = findFile(filename, false);

		if (index != -1)
			return createAndOpenFile(index);

		return 0;
	}


	//! opens a file by index
	IReadFile* CPakReader::createAndOpenFile(UINT32 index)
	{
		if (index >= Files.size())
			return 0;

		const SFileListEntry &entry = Files[index];
		return createLimitReadFile(entry.FullName, File, entry.Offset, entry.Size);
	}

}

#endif