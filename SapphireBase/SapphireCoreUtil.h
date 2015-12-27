#ifndef _SAPPHIRE_CORE_UTIL_
#define _SAPPHIRE_CORE_UTIL_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"

namespace Sapphire
{
	/*! \file coreutil.h
	\brief 包含一些工具函数
	*/

	// ----------- 某些非常常用基本字符串函数  -----------------

	//! 搜寻一个文件是否有正确的扩展名
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

	//! 首先文件是否有一个扩展名
	inline bool hasFileExtension(const path& filename,
		const path& ext0,
		const path& ext1 = "",
		const path& ext2 = "")
	{
		return isFileExtension(filename, ext0, ext1, ext2) > 0;
	}

	//! 从一个源文件路径剪切文件扩展名，并且保存它到一个目的文件路径
	inline path& cutFilenameExtension(path &dest, const path &source)
	{
		//SINT32 endPos = source.findLast('.');
		SINT32 endPos = source.rfind('.');
		dest = source.substr(0, endPos < 0 ? source.size() : endPos);
		return dest;
	}

	//! 从一个文件路径获取文件扩展名
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

	//! 从文件名删除路径
	inline path& deletePathFromFilename(path& filename)
	{
		String basefilename;
		String path;
		StringUtil::splitFilename(filename, basefilename, path);
		filename = basefilename;
		return filename;
	}


	


	//! 某些标准函数(移除依赖)
#undef isdigit
#undef isspace
#undef isupper
	inline SINT32 isdigit(SINT32 c) { return c >= '0' && c <= '9'; }
	inline SINT32 isspace(SINT32 c) { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }
	inline SINT32 isupper(SINT32 c) { return c >= 'A' && c <= 'Z'; }
}

#endif