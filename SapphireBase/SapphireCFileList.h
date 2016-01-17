#ifndef _SAPPHIRE_C_FILE_LIST_
#define _SAPPHIRE_C_FILE_LIST_

#include "SapphireIFileList.h"

namespace Sapphire
{
	//! An entry in a list of files, can be a folder or a file.
	struct SFileListEntry
	{
		//! The name of the file
		/** If this is a file or folder in the virtual filesystem and the archive
		was created with the ignoreCase flag then the file name will be lower case. */
		path Name;

		//! The name of the file including the path
		/** If this is a file or folder in the virtual filesystem and the archive was
		created with the ignoreDirs flag then it will be the same as Name. */
		path FullName;

		//! The size of the file in bytes
		UINT32 Size;

		//! The ID of the file in an archive
		/** This is used to link the FileList entry to extra info held about this
		file in an archive, which can hold things like data offset and CRC. */
		UINT32 ID;

		//! FileOffset inside an archive
		UINT32 Offset;

		//! True if this is a folder, false if not.
		bool IsDirectory;

		//! The == operator is provided so that CFileList can slowly search the list!
		bool operator ==(const struct SFileListEntry& other) const
		{
			if (IsDirectory != other.IsDirectory)
				return false;
			StringUtil::equals(FullName, other.FullName, false);
			//return FullName.equals_ignore_case(other.FullName);
			return StringUtil::equals(FullName, other.FullName, false);
		}

		//! The < operator is provided so that CFileList can sort and quickly search the list.
		bool operator <(const struct SFileListEntry& other) const
		{
			if (IsDirectory != other.IsDirectory)
				return IsDirectory;
			 
			return StringUtil::lower_ignore_case(FullName, other.FullName);
		}
	};


	//! Implementation of a file list
	class CFileList : public IFileList
	{
	public:

		// CFileList methods

		//! Constructor
		/** \param path The path of this file archive */
		CFileList(const path& path, bool ignoreCase, bool ignorePaths);

		//! Destructor
		virtual ~CFileList();

		//! Add as a file or folder to the list
		/** \param fullPath The file name including path, up to the root of the file list.
		\param isDirectory True if this is a directory rather than a file.
		\param offset The offset where the file is stored in an archive
		\param size The size of the file in bytes.
		\param id The ID of the file in the archive which owns it */
		virtual UINT32 addItem(const path& fullPath, UINT32 offset, UINT32 size, bool isDirectory, UINT32 id = 0);

		//! Sorts the file list. You should call this after adding any items to the file list
		virtual void sort();

		//! Returns the amount of files in the filelist.
		virtual UINT32 getFileCount() const;

		//! Gets the name of a file in the list, based on an index.
		virtual const path& getFileName(UINT32 index) const;

		//! Gets the full name of a file in the list, path included, based on an index.
		virtual const path& getFullFileName(UINT32 index) const;

		//! Returns the ID of a file in the file list, based on an index.
		virtual UINT32 getID(UINT32 index) const;

		//! Returns true if the file is a directory
		virtual bool isDirectory(UINT32 index) const;

		//! Returns the size of a file
		virtual UINT32 getFileSize(UINT32 index) const;

		//! Returns the offest of a file
		virtual UINT32 getFileOffset(UINT32 index) const;

		//! Searches for a file or folder within the list, returns the index
		virtual SINT32 findFile(const path& filename, bool isFolder) const;

		//! Returns the base path of the file list
		virtual const path& getPath() const;

	protected:

		//! Ignore paths when adding or searching for files
		bool IgnorePaths;

		//! Ignore case when adding or searching for files
		bool IgnoreCase;

		//! Path to the file list
		path Path;

		//! List of files
		vector<SFileListEntry>::type Files;
	};
}

#endif