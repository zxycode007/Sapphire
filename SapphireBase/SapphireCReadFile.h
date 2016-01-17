#ifndef _SAPPHIRE_C_READ_FILE_
#define _SAPPHIRE_C_READ_FILE_

#include "SapphireIReadFile.h"

namespace Sapphire
{
	/*!
	Class for reading a real file from disk.
	*/
	class CReadFile : public IReadFile
	{
	public:

		CReadFile(const path& fileName);

		virtual ~CReadFile();

		//! returns how much was read
		virtual SINT32 read(void* buffer, UINT32 sizeToRead);

		//! changes position in file, returns true if successful
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual long getSize() const;

		//! returns if file is open
		virtual bool isOpen() const
		{
			return File != 0;
		}

		//! returns where in the file we are.
		virtual long getPos() const;

		//! returns name of file
		virtual const path& getFileName() const;

	private:

		//! opens the file
		void openFile();

		FILE* File;
		long FileSize;
		path Filename;
	};
}

#endif