#ifndef _SAPPHIRE_C_WRITE_FILE_
#define _SAPPHIRE_C_WRITE_FILE_

#include "SapphireIWriteFile.h"

namespace Sapphire
{
	/*!
	Class for writing a real file to disk.
	*/
	class CWriteFile : public IWriteFile
	{
	public:

		CWriteFile(const path& fileName, bool append);

		virtual ~CWriteFile();

		//! Reads an amount of bytes from the file.
		virtual SINT32 write(const void* buffer, UINT32 sizeToWrite);

		//! Changes position in file, returns true if successful.
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! Returns the current position in the file.
		virtual long getPos() const;

		//! Returns name of file.
		virtual const path& getFileName() const;

		//! returns if file is open
		bool isOpen() const;

	private:

		//! opens the file
		void openFile(bool append);

		path Filename;
		FILE* File;
		long FileSize;
	};
}

#endif