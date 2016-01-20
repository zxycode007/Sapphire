#include "SapphireCompileConifg.h"
#include "SapphireCFileSystem.h"
#include "SapphireIReadFile.h"
#include "SapphireIWriteFile.h"
#include "SapphireCWriteFile.h"
#include "SapphireCZipReader.h"
#include "SapphireCMountPointReader.h"
#include "SapphireCPakReader.h"
#include "SapphireCNPKReader.h"
#include "SapphireCTarReader.h"
#include "SapphireCWADReader.h"
#include "SapphireCFileList.h"
#include "SapphireCXMLReader.h"
#include "SapphireCXMLWriter.h"
#include "SapphireOS.h"
#include "SapphireCAtrributes.h"
#include "SapphireCMemoryFile.h"
#include "SapphireCLimitReadFile.h"


#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
#if !defined ( _WIN32_WCE )
#include <direct.h> // for _chdir
#include <io.h> // for _access
#include <tchar.h>
#endif
#else
#if (defined(SAPPHIRE_POSIX_API_) || defined(SAPPHIRE_OSX_PLATFORM_))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
	
#endif



namespace Sapphire
{
	//! constructor
	CFileSystem::CFileSystem()
	{
#ifdef _DEBUG
		setDebugName("CFileSystem");
#endif

		setFileListSystem(FILESYSTEM_NATIVE);
		//! reset current working directory
		getWorkingDirectory();

#ifdef _SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderPAK(this));
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderNPK(this));
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderTAR(this));
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderWAD(this));
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderMount(this));
#endif

#ifdef _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
		ArchiveLoader.push_back(new CArchiveLoaderZIP(this));
#endif

	}


	//! destructor
	CFileSystem::~CFileSystem()
	{
		UINT32 i;

		for (i = 0; i < FileArchives.size(); ++i)
		{
			FileArchives[i]->drop();
		}

		for (i = 0; i < ArchiveLoader.size(); ++i)
		{
			ArchiveLoader[i]->drop();
		}
	}


	//! opens a file for read access
	IReadFile* CFileSystem::createAndOpenFile(const path& filename)
	{
		IReadFile* file = 0;
		UINT32 i;

		for (i = 0; i< FileArchives.size(); ++i)
		{
			file = FileArchives[i]->createAndOpenFile(filename);
			if (file)
				return file;
		}

		// Create the file using an absolute path so that it matches
		// the scheme used by CNullDriver::getTexture().
		return createReadFile(getAbsolutePath(filename));
	}


	//! Creates an IReadFile interface for treating memory like a file.
	IReadFile* CFileSystem::createMemoryReadFile(void* memory, SINT32 len,
		const path& fileName, bool deleteMemoryWhenDropped)
	{
		if (!memory)
			return 0;
		else
			return new CMemoryFile(memory, len, fileName, deleteMemoryWhenDropped);
	}


	//! Creates an IReadFile interface for reading files inside files
	IReadFile* CFileSystem::createLimitReadFile(const path& fileName,
		IReadFile* alreadyOpenedFile, long pos, long areaSize)
	{
		if (!alreadyOpenedFile)
			return 0;
		else
			return new CLimitReadFile(alreadyOpenedFile, pos, areaSize, fileName);
	}


	//! Creates an IReadFile interface for treating memory like a file.
	IWriteFile* CFileSystem::createMemoryWriteFile(void* memory, SINT32 len,
		const path& fileName, bool deleteMemoryWhenDropped)
	{
		if (!memory)
			return 0;
		else
			return new CMemoryFile(memory, len, fileName, deleteMemoryWhenDropped);
	}


	//! Opens a file for write access.
	IWriteFile* CFileSystem::createAndWriteFile(const path& filename, bool append)
	{
		return (IWriteFile*)createWriteFile(filename, append);
	}


	//! Adds an external archive loader to the engine.
	void CFileSystem::addArchiveLoader(IArchiveLoader* loader)
	{
		if (!loader)
			return;

		loader->grab();
		ArchiveLoader.push_back(loader);
	}

	//! Returns the total number of archive loaders added.
	UINT32 CFileSystem::getArchiveLoaderCount() const
	{
		return ArchiveLoader.size();
	}

	//! Gets the archive loader by index.
	IArchiveLoader* CFileSystem::getArchiveLoader(UINT32 index) const
	{
		if (index < ArchiveLoader.size())
			return ArchiveLoader[index];
		else
			return 0;
	}

	//! move the hirarchy of the filesystem. moves sourceIndex relative up or down
	bool CFileSystem::moveFileArchive(UINT32 sourceIndex, SINT32 relative)
	{
		bool r = false;
		const SINT32 dest = (SINT32)sourceIndex + relative;
		const SINT32 dir = relative < 0 ? -1 : 1;
		const SINT32 sourceEnd = ((SINT32)FileArchives.size()) - 1;
		IFileArchive *t;

		for (SINT32 s = (SINT32)sourceIndex; s != dest; s += dir)
		{
			if (s < 0 || s > sourceEnd || s + dir < 0 || s + dir > sourceEnd)
				continue;

			t = FileArchives[s + dir];
			FileArchives[s + dir] = FileArchives[s];
			FileArchives[s] = t;
			r = true;
		}
		return r;
	}


	//! Adds an archive to the file system.
	bool CFileSystem::addFileArchive(const path& filename, bool ignoreCase,
		bool ignorePaths, E_FILE_ARCHIVE_TYPE archiveType,
		const String& password,
		IFileArchive** retArchive)
	{
		IFileArchive* archive = 0;
		bool ret = false;

		// see if archive is already added
		if (changeArchivePassword(filename, password, retArchive))
			return true;

		SINT32 i;

		// do we know what type it should be?
		if (archiveType == EFAT_UNKNOWN || archiveType == EFAT_FOLDER)
		{
			// try to load archive based on file name
			for (i = ArchiveLoader.size() - 1; i >= 0; --i)
			{
				if (ArchiveLoader[i]->isALoadableFileFormat(filename))
				{
					archive = ArchiveLoader[i]->createArchive(filename, ignoreCase, ignorePaths);
					if (archive)
						break;
				}
			}

			// try to load archive based on content
			if (!archive)
			{
				IReadFile* file = createAndOpenFile(filename);
				if (file)
				{
					for (i = ArchiveLoader.size() - 1; i >= 0; --i)
					{
						file->seek(0);
						if (ArchiveLoader[i]->isALoadableFileFormat(file))
						{
							file->seek(0);
							archive = ArchiveLoader[i]->createArchive(file, ignoreCase, ignorePaths);
							if (archive)
								break;
						}
					}
					file->drop();
				}
			}
		}
		else
		{
			// try to open archive based on archive loader type

			IReadFile* file = 0;

			for (i = ArchiveLoader.size() - 1; i >= 0; --i)
			{
				if (ArchiveLoader[i]->isALoadableFileFormat(archiveType))
				{
					// attempt to open file
					if (!file)
						file = createAndOpenFile(filename);

					// is the file open?
					if (file)
					{
						// attempt to open archive
						file->seek(0);
						if (ArchiveLoader[i]->isALoadableFileFormat(file))
						{
							file->seek(0);
							archive = ArchiveLoader[i]->createArchive(file, ignoreCase, ignorePaths);
							if (archive)
								break;
						}
					}
					else
					{
						// couldn't open file
						break;
					}
				}
			}

			// if open, close the file
			if (file)
				file->drop();
		}

		if (archive)
		{
			FileArchives.push_back(archive);
			if (password.size())
				archive->Password = password;
			if (retArchive)
				*retArchive = archive;
			ret = true;
		}
		else
		{
			Printer::log("Could not create archive for", filename, LML_ERROR);
		}

		//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return ret;
	}

	// don't expose!
	bool CFileSystem::changeArchivePassword(const path& filename,
		const String& password,
		IFileArchive** archive)
	{
		for (SINT32 idx = 0; idx < (SINT32)FileArchives.size(); ++idx)
		{
			// TODO: This should go into a path normalization method
			// We need to check for directory names with trailing slash and without
			const path absPath = getAbsolutePath(filename);
			const path arcPath = FileArchives[idx]->getFileList()->getPath();
			if ((absPath == arcPath) || ((absPath + SAPPHIRE_TEXT("/")) == arcPath))
			{
				if (password.size())
					FileArchives[idx]->Password = password;
				if (archive)
					*archive = FileArchives[idx];
				return true;
			}
		}

		return false;
	}

	bool CFileSystem::addFileArchive(IReadFile* file, bool ignoreCase,
		bool ignorePaths, E_FILE_ARCHIVE_TYPE archiveType,
		const String& password, IFileArchive** retArchive)
	{
		if (!file || archiveType == EFAT_FOLDER)
			return false;

		if (file)
		{
			if (changeArchivePassword(file->getFileName(), password, retArchive))
				return true;

			IFileArchive* archive = 0;
			SINT32 i;

			if (archiveType == EFAT_UNKNOWN)
			{
				// try to load archive based on file name
				for (i = ArchiveLoader.size() - 1; i >= 0; --i)
				{
					if (ArchiveLoader[i]->isALoadableFileFormat(file->getFileName()))
					{
						archive = ArchiveLoader[i]->createArchive(file, ignoreCase, ignorePaths);
						if (archive)
							break;
					}
				}

				// try to load archive based on content
				if (!archive)
				{
					for (i = ArchiveLoader.size() - 1; i >= 0; --i)
					{
						file->seek(0);
						if (ArchiveLoader[i]->isALoadableFileFormat(file))
						{
							file->seek(0);
							archive = ArchiveLoader[i]->createArchive(file, ignoreCase, ignorePaths);
							if (archive)
								break;
						}
					}
				}
			}
			else
			{
				// try to open archive based on archive loader type
				for (i = ArchiveLoader.size() - 1; i >= 0; --i)
				{
					if (ArchiveLoader[i]->isALoadableFileFormat(archiveType))
					{
						// attempt to open archive
						file->seek(0);
						if (ArchiveLoader[i]->isALoadableFileFormat(file))
						{
							file->seek(0);
							archive = ArchiveLoader[i]->createArchive(file, ignoreCase, ignorePaths);
							if (archive)
								break;
						}
					}
				}
			}

			if (archive)
			{
				FileArchives.push_back(archive);
				if (password.size())
					archive->Password = password;
				if (retArchive)
					*retArchive = archive;
				return true;
			}
			else
			{
				 Printer::log("Could not create archive for", file->getFileName(), LML_ERROR);
			}
		}

		return false;
	}


	//! Adds an archive to the file system.
	bool CFileSystem::addFileArchive(IFileArchive* archive)
	{
		for (UINT32 i = 0; i < FileArchives.size(); ++i)
		{
			if (archive == FileArchives[i])
			{
				//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
				return false;
			}
		}
		FileArchives.push_back(archive);
		return true;
	}


	//! removes an archive from the file system.
	bool CFileSystem::removeFileArchive(UINT32 index)
	{
		bool ret = false;
		if (index < FileArchives.size())
		{
			FileArchives[index]->drop();
			FileArchives.erase(FileArchives.begin()+index);
			ret = true;
		}
		//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return ret;
	}


	//! removes an archive from the file system.
	bool CFileSystem::removeFileArchive(const path& filename)
	{
		const path absPath = getAbsolutePath(filename);
		for (UINT32 i = 0; i < FileArchives.size(); ++i)
		{
			if (absPath == FileArchives[i]->getFileList()->getPath())
				return removeFileArchive(i);
		}
		//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return false;
	}


	//! Removes an archive from the file system.
	bool CFileSystem::removeFileArchive(const IFileArchive* archive)
	{
		for (UINT32 i = 0; i < FileArchives.size(); ++i)
		{
			if (archive == FileArchives[i])
			{
				//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
				return removeFileArchive(i);
			}
		}
		//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return false;
	}


	//! gets an archive
	UINT32 CFileSystem::getFileArchiveCount() const
	{
		return FileArchives.size();
	}


	IFileArchive* CFileSystem::getFileArchive(UINT32 index)
	{
		return index < getFileArchiveCount() ? FileArchives[index] : 0;
	}


	//! Returns the string of the current working directory
	const path& CFileSystem::getWorkingDirectory()
	{
		EFileSystemType type = FileSystemType;

		if (type != FILESYSTEM_NATIVE)
		{
			type = FILESYSTEM_VIRTUAL;
		}
		else
		{
#if (SAPPHIRE_PLATFORM == SAPPHIRE_WINDOWS_CE_PLATFORM)
			// does not need this
#elif ( SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
			fschar_t tmp[_MAX_PATH];
#if SAPPHIRE_WCHAR_T_STRINGS
			_wgetcwd(tmp, _MAX_PATH);
			WorkingDirectory[FILESYSTEM_NATIVE] = tmp;
			WorkingDirectory[FILESYSTEM_NATIVE].replace(L'\\', L'/');
#else
			_getcwd(tmp, _MAX_PATH);
			WorkingDirectory[FILESYSTEM_NATIVE] = tmp;
			//
			//WorkingDirectory[FILESYSTEM_NATIVE].replace('\\', '/');
			WorkingDirectory[FILESYSTEM_NATIVE] = StringUtil::replaceAll(WorkingDirectory[FILESYSTEM_NATIVE],"\\", "/");
#endif
#endif

#if (SAPPHIRE_PLATFORM == SAPPHIRE_POSIX_API || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE)

			// getting the CWD is rather complex as we do not know the size
			// so try it until the call was successful
			// Note that neither the first nor the second parameter may be 0 according to POSIX

#if SAPPHIRE_WCHAR_T_STRINGS
			UINT32 pathSize = 256;
			wchar_t *tmpPath = new wchar_t[pathSize];
			while ((pathSize < (1 << 16)) && !(wgetcwd(tmpPath, pathSize)))
			{
				delete[] tmpPath;
				pathSize *= 2;
				tmpPath = new char[pathSize];
			}
			if (tmpPath)
			{
				WorkingDirectory[FILESYSTEM_NATIVE] = tmpPath;
				delete[] tmpPath;
			}
#else
			UINT32 pathSize = 256;
			char *tmpPath = new char[pathSize];
			while ((pathSize < (1 << 16)) && !(getcwd(tmpPath, pathSize)))
			{
				delete[] tmpPath;
				pathSize *= 2;
				tmpPath = new char[pathSize];
			}
			if (tmpPath)
			{
				WorkingDirectory[FILESYSTEM_NATIVE] = tmpPath;
				delete[] tmpPath;
			}
#endif
#endif

			//WorkingDirectory[type].validate();
		}

		return WorkingDirectory[type];
	}


	//! Changes the current Working Directory to the given string.
	bool CFileSystem::changeWorkingDirectoryTo(const path& newDirectory)
	{
		bool success = false;

		if (FileSystemType != FILESYSTEM_NATIVE)
		{
			WorkingDirectory[FILESYSTEM_VIRTUAL] = newDirectory;
			// is this empty string constant really intended?
			flattenFilename(WorkingDirectory[FILESYSTEM_VIRTUAL], SAPPHIRE_TEXT(""));
			success = true;
		}
		else
		{
			WorkingDirectory[FILESYSTEM_NATIVE] = newDirectory;

#if (SAPPHIRE_PLATFORM == SAPPHIRE_WINDOWS_CE_PLATFORM)
			success = true;
#elif defined(_MSC_VER)
#if SAPPHIRE_WCHAR_T_STRINGS
			success = (_wchdir(newDirectory.c_str()) == 0);
#else
			success = (_chdir(newDirectory.c_str()) == 0);
#endif
#else
#if SAPPHIRE_WCHAR_T_STRINGS
			success = (_wchdir(newDirectory.c_str()) == 0);
#else
			success = (chdir(newDirectory.c_str()) == 0);
#endif
#endif
		}

		return success;
	}


	path CFileSystem::getAbsolutePath(const path& filename) const
	{
#if (SAPPHIRE_PLATFORM == SAPPHIRE_WINDOWS_CE_PLATFORM)
		return filename;
#elif (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
		fschar_t *p = 0;
		fschar_t fpath[_MAX_PATH];
#if SAPPHIRE_WCHAR_T_STRINGS
		p = _wfullpath(fpath, filename.c_str(), _MAX_PATH);
		StringW tmp(p);
		tmp.replace(L'\\', L'/');
#else
		p = _fullpath(fpath, filename.c_str(), _MAX_PATH);
		String tmp(p);
		tmp = StringUtil::replaceAll(tmp,"\\","/");
		//tmp.replace('\\', '/');
#endif
		return tmp;
#elif (SAPPHIRE_PLATFORM  == SAPPHIRE_POSIX_API || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE)
		c8* p = 0;
		c8 fpath[4096];
		fpath[0] = 0;
		p = realpath(filename.c_str(), fpath);
		if (!p)
		{
			// content in fpath is unclear at this point
			if (!fpath[0]) // seems like fpath wasn't altered, use our best guess
			{
				path tmp(filename);
				return flattenFilename(tmp);
			}
			else
				return path(fpath);
		}
		if (filename[filename.size() - 1] == '/')
			return path(p) + SAPPHIRE_TEXT("/");
		else
			return path(p);
#else
		return path(filename);
#endif
	}


	//! returns the directory part of a filename, i.e. all until the first
	//! slash or backslash, excluding it. If no directory path is prefixed, a '.'
	//! is returned.
	path CFileSystem::getFileDir(const path& filename) const
	{
		// find last forward or backslash
		SINT32 lastSlash = filename.find_last_of('/');
		const SINT32 lastBackSlash = filename.find_last_of('\\');
		lastSlash = lastSlash > lastBackSlash ? lastSlash : lastBackSlash;

		if ((UINT32)lastSlash < filename.size())
			return filename.substr(0, lastSlash);
		else
			return SAPPHIRE_TEXT(".");
	}


	//! returns the base part of a filename, i.e. all except for the directory
	//! part. If no directory path is prefixed, the full name is returned.
	path CFileSystem::getFileBasename(const path& filename, bool keepExtension) const
	{
		// find last forward or backslash
		SINT32 lastSlash = filename.find_last_of('/');
		const SINT32 lastBackSlash = filename.find_last_of('\\');
		lastSlash = Math::_max(lastSlash, lastBackSlash);

		// get number of chars after last dot
		SINT32 end = 0;
		if (!keepExtension)
		{
			// take care to search only after last slash to check only for
			// dots in the filename
			end = filename.find_last_of('.');
			if (end == -1 || end < lastSlash)
				end = 0;
			else
				end = filename.size() - end;
		}

		if ((UINT32)lastSlash < filename.size())
			return filename.substr(lastSlash + 1, filename.size() - lastSlash - 1 - end);
		else if (end != 0)
			return filename.substr(0, filename.size() - end);
		else
			return filename;
	}


	//! flatten a path and file name for example: "/you/me/../." becomes "/you"
	path& CFileSystem::flattenFilename(path& directory, const path& root) const
	{
		//directory.replace('\\', '/');
		directory = StringUtil::replaceAll(directory, "\\", "/");
		if (directory[directory.length()-1] != '/')
			directory+='/';

		path dir;
		path subdir;

		SINT32 lastpos = 0;
		SINT32 pos = 0;
		bool lastWasRealDir = false;

		while ((pos = directory.find('/', lastpos)) >= 0)//while ((pos = directory.findNext('/', lastpos)) >= 0)
		{
			subdir = directory.substr(lastpos, pos - lastpos + 1);

			if (subdir == SAPPHIRE_TEXT("../"))
			{
				if (lastWasRealDir)
				{
					deletePathFromPath(dir, 2);
					lastWasRealDir = (dir.size() != 0);
				}
				else
				{
					dir.append(subdir);
					lastWasRealDir = false;
				}
			}
			else if (subdir == SAPPHIRE_TEXT("/"))
			{
				dir = root;
			}
			else if (subdir != SAPPHIRE_TEXT("./"))
			{
				dir.append(subdir);
				lastWasRealDir = true;
			}

			lastpos = pos + 1;
		}
		directory = dir;
		return directory;
	}


	//! Get the relative filename, relative to the given directory
	path CFileSystem::getRelativeFilename(const path& filename, const path& directory) const
	{
		if (filename.empty() || directory.empty())
			return filename;

		path path1, file, ext;
		//splitFilename(getAbsolutePath(filename), &path1, &file, &ext);
		StringUtil::splitFullFilename(getAbsolutePath(filename),file, ext, path1);
		path path2(getAbsolutePath(directory));
		vector<path>::type list1, list2;
		list1 = StringUtil::split(path1, SAPPHIRE_TEXT("/\\"), 2);
		list2 = StringUtil::split(path2, SAPPHIRE_TEXT("/\\"), 2);
		//path1.split(list1, SAPPHIRE_TEXT("/\\"), 2);
		//path2.split(list2, SAPPHIRE_TEXT("/\\"), 2);
		UINT32 i = 0;
		vector<path>::const_iterator it1, it2;
		it1 = list1.begin();
		it2 = list2.begin();

#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
		fschar_t partition1 = 0, partition2 = 0;
		path prefix1, prefix2;
		if (it1 != list1.end())
			prefix1 = *it1;
		if (it2 != list2.end())
			prefix2 = *it2;
		if (prefix1.size() > 1 && prefix1[1] == SAPPHIRE_TEXT(':'))
			partition1 = CONVERT_TO_LOWER(prefix1[0]);
		if (prefix2.size() > 1 && prefix2[1] == SAPPHIRE_TEXT(':'))
			partition2 = CONVERT_TO_LOWER(prefix2[0]);

		// must have the same prefix or we can't resolve it to a relative filename
		if (partition1 != partition2)
		{
			return filename;
		}
#endif


		for (; i<list1.size() && i<list2.size()
#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
			&& (StringUtil::toLowerCase2(String(*it1)) == StringUtil::toLowerCase2(String(*it1)))//&& (path(*it1).make_lower() == path(*it2).make_lower())
#else
			&& (*it1 == *it2)
#endif
			; ++i)
		{
			++it1;
			++it2;
		}
		path1 = SAPPHIRE_TEXT("");
		for (; i<list2.size(); ++i)
			path1 += SAPPHIRE_TEXT("../");
		while (it1 != list1.end())
		{
			path1 += *it1++;
			path1 += SAPPHIRE_TEXT('/');
		}
		path1 += file;
		if (ext.size())
		{
			path1 += SAPPHIRE_TEXT('.');
			path1 += ext;
		}
		return path1;
	}


	//! Sets the current file systen type
	EFileSystemType CFileSystem::setFileListSystem(EFileSystemType listType)
	{
		EFileSystemType current = FileSystemType;
		FileSystemType = listType;
		return current;
	}


	//! Creates a list of files and directories in the current working directory
	IFileList* CFileSystem::createFileList()
	{
		CFileList* r = 0;
		path Path = getWorkingDirectory();
		//Path.replace('\\', '/');
		Path = StringUtil::replaceAll(Path, "\\", "/");
		if (Path[Path.length()-1] != '/')
			Path += '/';

		//! Construct from native filesystem
		if (FileSystemType == FILESYSTEM_NATIVE)
		{
			// --------------------------------------------
			//! Windows version
#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32)
#if !defined ( _WIN32_WCE )

			r = new CFileList(Path, true, false);

			// TODO: Should be unified once mingw adapts the proper types
#if defined(__GNUC__)
			long hFile; //mingw return type declaration
#else
			intptr_t hFile;
#endif

			struct _tfinddata_t c_file;
			if ((hFile = _tfindfirst(_T("*"), &c_file)) != -1L)
			{
				do
				{
					r->addItem(Path + c_file.name, 0, c_file.size, (_A_SUBDIR & c_file.attrib) != 0, 0);
				} while (_tfindnext(hFile, &c_file) == 0);

				_findclose(hFile);
			}
#endif

			//TODO add drives
			//entry.Name = "E:\\";
			//entry.isDirectory = true;
			//Files.push_back(entry);
#endif

			// --------------------------------------------
			//! Linux version
#if (SAPPHIRE_PLATFORM == SAPPHIRE_POSIX_API || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE)


			r = new CFileList(Path, false, false);

			r->addItem(Path + SAPPHIRE_TEXT(".."), 0, 0, true, 0);

			//! We use the POSIX compliant methods instead of scandir
			DIR* dirHandle = opendir(Path.c_str());
			if (dirHandle)
			{
				struct dirent *dirEntry;
				while ((dirEntry = readdir(dirHandle)))
				{
					UINT32 size = 0;
					bool isDirectory = false;

					if ((strcmp(dirEntry->d_name, ".") == 0) ||
						(strcmp(dirEntry->d_name, "..") == 0))
					{
						continue;
					}
					struct stat buf;
					if (stat(dirEntry->d_name, &buf) == 0)
					{
						size = buf.st_size;
						isDirectory = S_ISDIR(buf.st_mode);
					}
#if !defined(SAPPHIRE_SOLARIS_PLATFORM_) && !defined(__CYGWIN__)
					// only available on some systems
					else
					{
						isDirectory = dirEntry->d_type == DT_DIR;
					}
#endif

					r->addItem(Path + dirEntry->d_name, 0, size, isDirectory, 0);
				}
				closedir(dirHandle);
			}
#endif
		}
		else
		{
			//! create file list for the virtual filesystem
			r = new CFileList(Path, false, false);

			//! add relative navigation
			SFileListEntry e2;
			SFileListEntry e3;

			//! PWD
			r->addItem(Path + SAPPHIRE_TEXT("."), 0, 0, true, 0);

			//! parent
			r->addItem(Path + SAPPHIRE_TEXT(".."), 0, 0, true, 0);

			//! merge archives
			for (UINT32 i = 0; i < FileArchives.size(); ++i)
			{
				const IFileList *merge = FileArchives[i]->getFileList();

				for (UINT32 j = 0; j < merge->getFileCount(); ++j)
				{
					if (isInSameDirectory(Path, merge->getFullFileName(j)) == 0)
					{
						r->addItem(merge->getFullFileName(j), merge->getFileOffset(j), merge->getFileSize(j), merge->isDirectory(j), 0);
					}
				}
			}
		}

		if (r)
			r->sort();
		return r;
	}

	//! Creates an empty filelist
	IFileList* CFileSystem::createEmptyFileList(const path& path, bool ignoreCase, bool ignorePaths)
	{
		return new CFileList(path, ignoreCase, ignorePaths);
	}


	//! determines if a file exists and would be able to be opened.
	bool CFileSystem::existFile(const path& filename) const
	{
		for (UINT32 i = 0; i < FileArchives.size(); ++i)
			if (FileArchives[i]->getFileList()->findFile(filename) != -1)
				return true;

#if (SAPPHIRE_PLATFORM == SAPPHIRE_WINDOWS_CE_PLATFORM)
#if (SAPPHIRE_WCHAR_T_STRINGS)
		HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
#else
		HANDLE hFile = CreateFileW(core::stringw(filename).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
#endif
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
		else
		{
			CloseHandle(hFile);
			return true;
		}
#else
		//SAPPHIRE_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
#if defined(_MSC_VER)
#if (SAPPHIRE_WCHAR_T_STRINGS)
		return (_waccess(filename.c_str(), 0) != -1);
#else
		return (_access(filename.c_str(), 0) != -1);
#endif
#elif defined(F_OK)
#if (SAPPHIRE_WCHAR_T_STRINGS)
		return (_waccess(filename.c_str(), F_OK) != -1);
#else
		return (access(filename.c_str(), F_OK) != -1);
#endif
#else
		return (access(filename.c_str(), 0) != -1);
#endif
#endif
	}


	//! Creates a XML Reader from a file.
	IXMLReader* CFileSystem::createXMLReader(const path& filename)
	{
		IReadFile* file = createAndOpenFile(filename);
		if (!file)
			return 0;

		IXMLReader* reader = createXMLReader(file);
		file->drop();
		return reader;
	}


	//! Creates a XML Reader from a file.
	IXMLReader* CFileSystem::createXMLReader(IReadFile* file)
	{
		if (!file)
			return 0;

		return createIXMLReader(file);
	}


	//! Creates a XML Reader from a file.
	IXMLReaderUTF8* CFileSystem::createXMLReaderUTF8(const path& filename)
	{
		IReadFile* file = createAndOpenFile(filename);
		if (!file)
			return 0;

		IXMLReaderUTF8* reader = createIXMLReaderUTF8(file);
		file->drop();
		return reader;
	}


	//! Creates a XML Reader from a file.
	IXMLReaderUTF8* CFileSystem::createXMLReaderUTF8(IReadFile* file)
	{
		if (!file)
			return 0;

		return createIXMLReaderUTF8(file);
	}


	//! Creates a XML Writer from a file.
	IXMLWriter* CFileSystem::createXMLWriter(const path& filename)
	{
		IWriteFile* file = createAndWriteFile(filename);
		IXMLWriter* writer = 0;
		if (file)
		{
			writer = createXMLWriter(file);
			file->drop();
		}
		return writer;
	}


	//! Creates a XML Writer from a file.
	IXMLWriter* CFileSystem::createXMLWriter(IWriteFile* file)
	{
		return new CXMLWriter(file);
	}


	//! creates a filesystem which is able to open files from the ordinary file system,
	//! and out of zipfiles, which are able to be added to the filesystem.
	IFileSystem* createFileSystem()
	{
		return new CFileSystem();
	}


	//! Creates a new empty collection of attributes, usable for serialization and more.
	IAttributes* CFileSystem::createEmptyAttributes(IVideoDriver* driver)
	{
		return new CAttributes(driver);
	}

}