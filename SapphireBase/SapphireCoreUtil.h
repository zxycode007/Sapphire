#ifndef _SAPPHIRE_CORE_UTIL_
#define _SAPPHIRE_CORE_UTIL_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"

namespace Sapphire
{
	/*! \file coreutil.h
	\brief ����һЩ���ߺ���
	*/

	// ----------- ĳЩ�ǳ����û����ַ�������  -----------------

	//! ��Ѱһ���ļ��Ƿ�����ȷ����չ��
	inline SINT32 isFileExtension(const path& filename,
		const path& ext0,
		const path& ext1,
		const path& ext2)
	{
		//SINT32 extPos = filename.findLast('.');
		SINT32 extPos = filename.rfind('.');
		if (extPos < 0)
			return 0;

		extPos += 1;
		//if (filename.equals_substring_ignore_case(ext0, extPos)) return 1;
		//if (filename.equals_substring_ignore_case(ext1, extPos)) return 2;
		//if (filename.equals_substring_ignore_case(ext2, extPos)) return 3;
		if (StringUtil::equalsSubString(filename,ext0,extPos)) return 1;
		if (StringUtil::equalsSubString(filename, ext1, extPos)) return 2;
		if (StringUtil::equalsSubString(filename, ext2, extPos)) return 3;
		return 0;
	}

	//! �����ļ��Ƿ���һ����չ��
	inline bool hasFileExtension(const path& filename,
		const path& ext0,
		const path& ext1 = "",
		const path& ext2 = "")
	{
		return isFileExtension(filename, ext0, ext1, ext2) > 0;
	}

	//! ��һ��Դ�ļ�·�������ļ���չ�������ұ�������һ��Ŀ���ļ�·��
	inline path& cutFilenameExtension(path &dest, const path &source)
	{
		//SINT32 endPos = source.findLast('.');
		SINT32 endPos = source.rfind('.');
		dest = source.substr(0, endPos < 0 ? source.size() : endPos);
		return dest;
	}

	//! ��һ���ļ�·����ȡ�ļ���չ��
	inline path& getFileNameExtension(path &dest, const path &source)
	{
		//SINT32 endPos = source.findLast('.');
		SINT32 endPos = source.rfind('.');
		if (endPos < 0)
			dest = "";
		else
			dest = source.substr(endPos, source.size());
		return dest;
	}

	//! ���ļ���ɾ��·��
	inline path& deletePathFromFilename(path& filename)
	{
		String basefilename;
		String path;
		StringUtil::splitFilename(filename, basefilename, path);
		filename = basefilename;
		return filename;
	}


	


	//! ĳЩ��׼����(�Ƴ�����)
#undef isdigit
#undef isspace
#undef isupper
	inline SINT32 isdigit(SINT32 c) { return c >= '0' && c <= '9'; }
	inline SINT32 isspace(SINT32 c) { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }
	inline SINT32 isupper(SINT32 c) { return c >= 'A' && c <= 'Z'; }
}

#endif