#include "SapphireCFileList.h"
#include "SapphireCoreUtil.h"

namespace Sapphire
{
	static const path emptyFileListEntry;

	CFileList::CFileList(const path& path, bool ignoreCase, bool ignorePaths)
		: IgnorePaths(ignorePaths), IgnoreCase(ignoreCase), Path(path)
	{
#ifdef _DEBUG
		setDebugName("CFileList");
#endif
		StringUtil::replaceAll(Path, "\\", "/");
		//Path.replace('\\', '/');
	}

	CFileList::~CFileList()
	{
		Files.clear();
	}

	UINT32 CFileList::getFileCount() const
	{
		return Files.size();
	}

	void CFileList::sort()
	{
		std::stable_sort(Files.begin(), Files.end());
		//Files.sort();
	}

	const path& CFileList::getFileName(UINT32 index) const
	{
		if (index >= Files.size())
			return emptyFileListEntry;

		return Files[index].Name;
	}


	//! Gets the full name of a file in the list, path included, based on an index.
	const path& CFileList::getFullFileName(UINT32 index) const
	{
		if (index >= Files.size())
			return emptyFileListEntry;

		return Files[index].FullName;
	}

	//! adds a file or folder
	UINT32 CFileList::addItem(const path& fullPath, UINT32 offset, UINT32 size, bool isDirectory, UINT32 id)
	{
		SFileListEntry entry;
		entry.ID = id ? id : Files.size();
		entry.Offset = offset;
		entry.Size = size;
		entry.Name = fullPath;
		StringUtil::replaceAll(entry.Name, "\\", "/");
		//entry.Name.replace('\\', '/');
		entry.IsDirectory = isDirectory;

		// remove trailing slash
		if (entry.Name[entry.Name.length()-1] == '/')
		//entry.Name.lastChar() == '/'
		{
			entry.IsDirectory = true;
			entry.Name[entry.Name.size() - 1] = 0;
			//entry.Name.validate();
		}

		if (IgnoreCase)
			StringUtil::toLowerCase(entry.Name);
			//entry.Name.make_lower();

		entry.FullName = entry.Name;

		deletePathFromFilename(entry.Name);

		if (IgnorePaths)
			entry.FullName = entry.Name;

		//os::Printer::log(Path.c_str(), entry.FullName);

		Files.push_back(entry);

		return Files.size() - 1;
	}

	//! Returns the ID of a file in the file list, based on an index.
	UINT32 CFileList::getID(UINT32 index) const
	{
		return index < Files.size() ? Files[index].ID : 0;
	}

	bool CFileList::isDirectory(UINT32 index) const
	{
		bool ret = false;
		if (index < Files.size())
			ret = Files[index].IsDirectory;

		//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return ret;
	}

	//! Returns the size of a file
	UINT32 CFileList::getFileSize(UINT32 index) const
	{
		return index < Files.size() ? Files[index].Size : 0;
	}

	//! Returns the size of a file
	UINT32 CFileList::getFileOffset(UINT32 index) const
	{
		return index < Files.size() ? Files[index].Offset : 0;
	}


	//! Searches for a file or folder within the list, returns the index
	SINT32 CFileList::findFile(const path& filename, bool isDirectory = false) const
	{
		SFileListEntry entry;
		// we only need FullName to be set for the search
		entry.FullName = filename;
		entry.IsDirectory = isDirectory;

		// exchange
		//entry.FullName.replace('\\', '/');
		StringUtil::replaceAll(entry.FullName, "\\", "/");

		// remove trailing slash
		if (entry.Name[entry.Name.length() - 1] == '/') //(entry.FullName.lastChar() == '/')
		{
			entry.IsDirectory = true;
			entry.FullName[entry.FullName.size() - 1] = 0;
			//entry.FullName.validate();
		}

		if (IgnoreCase)
			StringUtil::toLowerCase(entry.FullName);//entry.FullName.make_lower();

		if (IgnorePaths)
			deletePathFromFilename(entry.FullName);
		 
		int index = std::distance(Files.begin(), std::find(Files.begin(), Files.end(), entry));
		return index;//Files.binary_search(entry);
	}


	//! Returns the base path of the file list
	const path& CFileList::getPath() const
	{
		return Path;
	}
}