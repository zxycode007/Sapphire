#ifndef _SAPPHIRE_PATH_
#define _SAPPHIRE_PATH_
#include "SapphirePrerequisites.h"
#include "SapphireString.h"

namespace Sapphire
{

	//!用于所有文件系统相关的字符串类型
	/** 这种类型将透明地处理不同的文件系统编码*/
	typedef String path;

	//! 通过一个文件名使用对象的地方,但实际不用真实文件名
	/** 
	这个类不区分大小写
	此类还是一个正确的序列化支持重命名对象的第一步。
	*/
	struct SNamedPath
	{
		 
		SNamedPath() {}

		 
		SNamedPath(const path& p) : Path(p), InternalName(PathToName(p))
		{
		}

	 
		bool operator <(const SNamedPath& other) const
		{
			return InternalName < other.InternalName;
		}
		bool operator ==(const SNamedPath& other) const
		{
			return (InternalName < other.InternalName) && (Path == other.Path);
		}
	 
		void setPath(const path& p)
		{
			Path = p;
			InternalName = PathToName(p);
		}

		 
		const path& getPath() const
		{
			return Path;
		};

		//! 获取文件名
		const path& getInternalName() const
		{
			return InternalName;
		}

	

	protected:
		// 转换给定路径字符串到一个命名字符串
		path PathToName(const path& p) const
		{
			path name(p);
			name = StringUtil::replaceAll(name, "\\", "/");
			StringUtil::toLowerCase(name);
			return name;
		}

	private:
		path Path;
		path InternalName;
	};


	//! trim paths
	inline path& deletePathFromPath(path& filename, SINT32 pathCount)
	{
		// delete path from filename
		SINT32 i = filename.size();

		// search for path separator or beginning
		while (i >= 0)
		{
			if (filename[i] == '/' || filename[i] == '\\')
			{
				if (--pathCount <= 0)
					break;
			}
			--i;
		}

		if (i>0)
		{
			filename[i + 1] = 0;
			//filename.validate();
		}
		else
			filename = "";
		return filename;
	}

}


#endif