#ifndef _SAPPHIRE_C_LIMIT_READ_FILE_
#define _SAPPHIRE_C_LIMIT_READ_FILE_

#include "SapphireIReadFile.h"

namespace Sapphire
{
	class CUnicodeConverter;

	/*! this is a read file, which is limited to some boundaries,
	so that it may only start from a certain file position
	and may only read until a certain file position.
	This can be useful, for example for reading uncompressed files
	in an archive (zip, tar).
	!*/
	class CLimitReadFile : public IReadFile
	{
	public:

		CLimitReadFile(IReadFile* alreadyOpenedFile, long pos, long areaSize, const  path& name);

		virtual ~CLimitReadFile();

		//! returns how much was read
		virtual SINT32 read(void* buffer, UINT32 sizeToRead);

		//! changes position in file, returns true if successful
		//! if relativeMovement==true, the pos is changed relative to current pos,
		//! otherwise from begin of file
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual long getSize() const;

		//! returns where in the file we are.
		virtual long getPos() const;

		//! returns name of file
		virtual const path& getFileName() const;

	private:

		path Filename;
		long AreaStart;
		long AreaEnd;
		long Pos;
		IReadFile* File;
	};

}



#endif