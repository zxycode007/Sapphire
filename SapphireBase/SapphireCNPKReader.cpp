#include "SapphireCNPKReader.h"
#include "SapphireDefines.h"
#ifdef _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_

#include "SapphireOS.h"
#include "SapphireCoreUtil.h"
#include "SapphireMath.h"


#ifdef _DEBUG
#define SAPPHIRE_DEBUG_NPK_READER
#endif

namespace Sapphire
{
	bool isHeaderValid(const SNPKHeader& header)
	{
		const c8* const tag = header.Tag;
		return tag[0] == '0' &&
			tag[1] == 'K' &&
			tag[2] == 'P' &&
			tag[3] == 'N';
	}


	//! Constructor
	CArchiveLoaderNPK::CArchiveLoaderNPK(IFileSystem* fs)
		: FileSystem(fs)
	{
#ifdef _DEBUG
		setDebugName("CArchiveLoaderNPK");
#endif
	}


	//! returns true if the file maybe is able to be loaded by this class
	bool CArchiveLoaderNPK::isALoadableFileFormat(const path& filename) const
	{
		return hasFileExtension(filename, "npk");
	}

	//! Check to see if the loader can create archives of this type.
	bool CArchiveLoaderNPK::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
	{
		return fileType == EFAT_NPK;
	}

	//! Creates an archive from the filename
	/** \param file File handle to check.
	\return Pointer to newly created archive, or 0 upon error. */
	IFileArchive* CArchiveLoaderNPK::createArchive(const path& filename, bool ignoreCase, bool ignorePaths) const
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
	IFileArchive* CArchiveLoaderNPK::createArchive(IReadFile* file, bool ignoreCase, bool ignorePaths) const
	{
		IFileArchive *archive = 0;
		if (file)
		{
			file->seek(0);
			archive = new CNPKReader(file, ignoreCase, ignorePaths);
		}
		return archive;
	}


	//! Check if the file might be loaded by this class
	/** Check might look into the file.
	\param file File handle to check.
	\return True if file seems to be loadable. */
	bool CArchiveLoaderNPK::isALoadableFileFormat(IReadFile* file) const
	{
		SNPKHeader header;

		file->read(&header, sizeof(header));

		return isHeaderValid(header);
	}


	/*!
	NPK Reader
	*/
	CNPKReader::CNPKReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
		: CFileList((file ? file->getFileName() : path("")), ignoreCase, ignorePaths), File(file)
	{
#ifdef _DEBUG
		setDebugName("CNPKReader");
#endif

		if (File)
		{
			File->grab();
			if (scanLocalHeader())
				sort();
			else
				Printer::log("Failed to load NPK archive.");
		}
	}


	CNPKReader::~CNPKReader()
	{
		if (File)
			File->drop();
	}


	const IFileList* CNPKReader::getFileList() const
	{
		return this;
	}


	bool CNPKReader::scanLocalHeader()
	{
		SNPKHeader header;

		// Read and validate the header
		File->read(&header, sizeof(header));
		if (!isHeaderValid(header))
			return false;

		// Seek to the table of contents
#if   (SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN)
		header.Offset = Byteswap::byteswap(header.Offset);
		header.Length = Byteswap::byteswap(header.Length);
#endif
		header.Offset += 8;
		String dirName;
		bool inTOC = true;
		// Loop through each entry in the table of contents
		while (inTOC && (File->getPos() < File->getSize()))
		{
			// read an entry
			char tag[4] = { 0 };
			SNPKFileEntry entry;
			File->read(tag, 4);
			const int numTag = MAKE_IRR_ID(tag[3], tag[2], tag[1], tag[0]);
			int size;

			bool isDir = true;

			switch (numTag)
			{
			case MAKE_IRR_ID('D', 'I', 'R', '_'):
			{
				File->read(&size, 4);
				readString(entry.Name);
				entry.Length = 0;
				entry.Offset = 0;
#ifdef SAPPHIRE_DEBUG_NPK_READER
				Printer::log("Dir", entry.Name);
#endif
			}
			break;
			case MAKE_IRR_ID('F', 'I', 'L', 'E'):
			{
				File->read(&size, 4);
				File->read(&entry.Offset, 4);
				File->read(&entry.Length, 4);
				readString(entry.Name);
				isDir = false;
#ifdef SAPPHIRE_DEBUG_NPK_READER
				Printer::log("File", entry.Name);
#endif
#if   (SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN)
				entry.Offset = Byteswap::byteswap(entry.Offset);
				entry.Length = Byteswap::byteswap(entry.Length);
#endif
			}
			break;
			case MAKE_IRR_ID('D', 'E', 'N', 'D'):
			{
				File->read(&size, 4);
				entry.Name = "";
				entry.Length = 0;
				entry.Offset = 0;
				const SINT32 pos = dirName.find('/', dirName.size() - 2);//dirName.findLast('/', dirName.size() - 2);
				if (pos == -1)
					dirName = "";
				else
					dirName = dirName.substr(0, pos);//dirName = dirName.subString(0, pos);
#ifdef SAPPHIRE_DEBUG_NPK_READER
				Printer::log("Dirend", dirName);
#endif
			}
			break;
			default:
				inTOC = false;
			}
			// skip root dir
			if (isDir)
			{
				if (!entry.Name.size() || (entry.Name == ".") || (entry.Name == "<noname>"))
					continue;
				dirName += entry.Name;
				dirName += "/";
			}
#ifdef SAPPHIRE_DEBUG_NPK_READER
			Printer::log("Name", entry.Name);
#endif
			addItem((isDir ? dirName : dirName + entry.Name), entry.Offset + header.Offset, entry.Length, isDir);
		}
		return true;
	}


	//! opens a file by file name
	IReadFile* CNPKReader::createAndOpenFile(const path& filename)
	{
		SINT32 index = findFile(filename, false);

		if (index != -1)
			return createAndOpenFile(index);

		return 0;
	}


	//! opens a file by index
	IReadFile* CNPKReader::createAndOpenFile(UINT32 index)
	{
		if (index >= Files.size())
			return 0;

		const SFileListEntry &entry = Files[index];
		return createLimitReadFile(entry.FullName, File, entry.Offset, entry.Size);
	}

	void CNPKReader::readString(String& name)
	{
		short stringSize;
		char buf[256];
		File->read(&stringSize, 2);
#if   (SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN)
		stringSize = Byteswap::byteswap(stringSize);
#endif
		name.reserve(stringSize);
		while (stringSize)
		{
			const short next = Math::_min(stringSize, (short)255);
			File->read(buf, next);
			buf[next] = 0;
			name.append(buf);
			stringSize -= next;
		}
	}
}


#endif