#include "SapphireCWriteFile.h"


namespace Sapphire
{
	CWriteFile::CWriteFile(const path& fileName, bool append)
		: FileSize(0)
	{
#ifdef _DEBUG
		setDebugName("CWriteFile");
#endif

		Filename = fileName;
		openFile(append);
	}



	CWriteFile::~CWriteFile()
	{
		if (File)
			fclose(File);
	}



	//! returns if file is open
	inline bool CWriteFile::isOpen() const
	{
		return File != 0;
	}



	//! returns how much was read
	SINT32 CWriteFile::write(const void* buffer, UINT32 sizeToWrite)
	{
		if (!isOpen())
			return 0;

		return (SINT32)fwrite(buffer, 1, sizeToWrite, File);
	}



	//! changes position in file, returns true if successful
	//! if relativeMovement==true, the pos is changed relative to current pos,
	//! otherwise from begin of file
	bool CWriteFile::seek(long finalPos, bool relativeMovement)
	{
		if (!isOpen())
			return false;

		return fseek(File, finalPos, relativeMovement ? SEEK_CUR : SEEK_SET) == 0;
	}



	//! returns where in the file we are.
	long CWriteFile::getPos() const
	{
		return ftell(File);
	}



	//! opens the file
	void CWriteFile::openFile(bool append)
	{
		if (Filename.size() == 0)
		{
			File = 0;
			return;
		}

#if (SAPPHIRE_WCHAR_T_STRINGS)
		File = _wfopen(Filename.c_str(), append ? L"ab" : L"wb");
#else
		File = fopen(Filename.c_str(), append ? "ab" : "wb");
#endif

		if (File)
		{
			// get FileSize

			fseek(File, 0, SEEK_END);
			FileSize = ftell(File);
			fseek(File, 0, SEEK_SET);
		}
	}



	//! returns name of file
	const path& CWriteFile::getFileName() const
	{
		return Filename;
	}



	IWriteFile* createWriteFile(const path& fileName, bool append)
	{
		CWriteFile* file = new CWriteFile(fileName, append);
		if (file->isOpen())
			return file;

		file->drop();
		return 0;
	}
}