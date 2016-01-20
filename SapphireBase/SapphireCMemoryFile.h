#ifndef _SAPPHIRE_C_MEMEORY_FILE_
#define _SAPPHIRE_C_MEMEORY_FILE_

#include "SapphireIReadFile.h"
#include "SapphireIWriteFile.h"


namespace Sapphire
{
	/*!
	Class for reading and writing from memory.
	*/
	class CMemoryFile : public IReadFile, public IWriteFile
	{
	public:

		//! Constructor
		CMemoryFile(void* memory, long len, const path& fileName, bool deleteMemoryWhenDropped);

		//! Destructor
		virtual ~CMemoryFile();

		//! returns how much was read
		virtual SINT32 read(void* buffer, UINT32 sizeToRead);

		//! returns how much was written
		virtual SINT32 write(const void* buffer, UINT32 sizeToWrite);

		//! changes position in file, returns true if successful
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual long getSize() const;

		//! returns where in the file we are.
		virtual long getPos() const;

		//! returns name of file
		virtual const path& getFileName() const;

	private:

		void *Buffer;
		long Len;
		long Pos;
		path Filename;
		bool deleteMemoryWhenDropped;
	};
}

#endif