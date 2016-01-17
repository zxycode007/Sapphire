#include "SapphireCTarReader.h"

#ifdef _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#include "SapphireCFileList.h"
#include "SapphireOS.h"
#include "SapphireCoreUtil.h"
#include "SapphireCLimitReadFile.h"

namespace Sapphire
{
	//! Constructor
	CArchiveLoaderTAR::CArchiveLoaderTAR(IFileSystem* fs)
		: FileSystem(fs)
	{
#ifdef _DEBUG
		setDebugName("CArchiveLoaderTAR");
#endif
	}


	//! returns true if the file maybe is able to be loaded by this class
	bool CArchiveLoaderTAR::isALoadableFileFormat(const path& filename) const
	{
		return hasFileExtension(filename, "tar");
	}

	//! Check to see if the loader can create archives of this type.
	bool CArchiveLoaderTAR::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
	{
		return fileType == EFAT_TAR;
	}

	//! Creates an archive from the filename
	/** \param file File handle to check.
	\return Pointer to newly created archive, or 0 upon error. */
	IFileArchive* CArchiveLoaderTAR::createArchive(const path& filename, bool ignoreCase, bool ignorePaths) const
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
	IFileArchive* CArchiveLoaderTAR::createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const
	{
		IFileArchive *archive = 0;
		if (file)
		{
			file->seek(0);
			archive = new CTarReader(file, ignoreCase, ignorePaths);
		}
		return archive;
	}

	//! Check if the file might be loaded by this class
	/** Check might look into the file.
	\param file File handle to check.
	\return True if file seems to be loadable. */
	bool CArchiveLoaderTAR::isALoadableFileFormat(IReadFile* file) const
	{
		// TAR files consist of blocks of 512 bytes
		// if it isn't a multiple of 512 then it's not a TAR file.
		if (file->getSize() % 512)
			return false;

		file->seek(0);

		// read header of first file
		STarHeader fHead;
		file->read(&fHead, sizeof(STarHeader));

		UINT32 checksum = 0;
		sscanf(fHead.Checksum, "%o", &checksum);

		// verify checksum

		// some old TAR writers assume that chars are signed, others assume unsigned
		// USTAR archives have a longer header, old TAR archives end after linkname

		UINT32 checksum1 = 0;
		SINT32 checksum2 = 0;

		// remember to blank the checksum field!
		memset(fHead.Checksum, ' ', 8);

		// old header
		for (UINT8* p = (UINT8*)(&fHead); p < (UINT8*)(&fHead.Magic[0]); ++p)
		{
			checksum1 += *p;
			checksum2 += c8(*p);
		}

		if (!strncmp(fHead.Magic, "ustar", 5))
		{
			for (UINT8* p = (UINT8*)(&fHead.Magic[0]); p < (UINT8*)(&fHead) + sizeof(fHead); ++p)
			{
				checksum1 += *p;
				checksum2 += c8(*p);
			}
		}
		return checksum1 == checksum || checksum2 == (SINT32)checksum;
	}

	/*
	TAR Archive
	*/
	CTarReader::CTarReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
		: CFileList((file ? file->getFileName() : path("")), ignoreCase, ignorePaths), File(file)
	{
#ifdef _DEBUG
		setDebugName("CTarReader");
#endif

		if (File)
		{
			File->grab();

			// fill the file list
			populateFileList();

			sort();
		}
	}


	CTarReader::~CTarReader()
	{
		if (File)
			File->drop();
	}

	const IFileList* CTarReader::getFileList() const
	{
		return this;
	}


	UINT32 CTarReader::populateFileList()
	{
		STarHeader fHead;
		Files.clear();

		UINT32 pos = 0;
		while (SINT32(pos + sizeof(STarHeader)) < File->getSize())
		{
			// seek to next file header
			File->seek(pos);

			// read the header
			File->read(&fHead, sizeof(fHead));

			// only add standard files for now
			if (fHead.Link == ETLI_REGULAR_FILE || ETLI_REGULAR_FILE_OLD)
			{
				path fullPath = "";
				fullPath.reserve(255);

				// USTAR archives have a filename prefix
				// may not be null terminated, copy carefully!
				if (!strncmp(fHead.Magic, "ustar", 5))
				{
					c8* np = fHead.FileNamePrefix;
					while (*np && (np - fHead.FileNamePrefix) < 155)
						fullPath +=*np;//fullPath.append(*np);
					np++;
				}

				// append the file name
				c8* np = fHead.FileName;
				while (*np && (np - fHead.FileName) < 100)
				{
					fullPath += *np;//fullPath.append(*np);
					np++;
				}

				// get size
				String sSize = "";
				sSize.reserve(12);
				np = fHead.Size;
				while (*np && (np - fHead.Size) < 12)
				{
					sSize+=*np;//sSize.append(*np);
					np++;
				}

				UINT32 size = strtoul(sSize.c_str(), NULL, 8);
//#if !defined(_IRR_WINDOWS_CE_PLATFORM_)
#if  SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
				if (errno == ERANGE)
					Printer::log("File too large", fullPath, LML_ERROR);
#endif

				// save start position
				UINT32 offset = pos + 512;

				// move to next file header block
				pos = offset + (size / 512) * 512 + ((size % 512) ? 512 : 0);

				// add file to list
				addItem(fullPath, offset, size, false);
			}
			else
			{
				// todo: ETLI_DIRECTORY, ETLI_LINK_TO_ARCHIVED_FILE

				// move to next block
				pos += 512;
			}

		}

		return Files.size();
	}

	//! opens a file by file name
	IReadFile* CTarReader::createAndOpenFile(const path& filename)
	{
		const SINT32 index = findFile(filename, false);

		if (index != -1)
			return createAndOpenFile(index);

		return 0;
	}

	//! opens a file by index
	IReadFile* CTarReader::createAndOpenFile(UINT32 index)
	{
		if (index >= Files.size())
			return 0;

		const SFileListEntry &entry = Files[index];
		return createLimitReadFile(entry.FullName, File, entry.Offset, entry.Size);
	}
}


#endif