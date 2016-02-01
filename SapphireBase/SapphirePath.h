#ifndef _SAPPHIRE_PATH_
#define _SAPPHIRE_PATH_
#include "SapphirePrerequisites.h"
#include "SapphireString.h"

namespace Sapphire
{

	//!���������ļ�ϵͳ��ص��ַ�������
	/** �������ͽ�͸���ش���ͬ���ļ�ϵͳ����*/
	typedef String path;

	//! ͨ��һ���ļ���ʹ�ö���ĵط�,��ʵ�ʲ�����ʵ�ļ���
	/** 
	����಻���ִ�Сд
	���໹��һ����ȷ�����л�֧������������ĵ�һ����
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

		//! ��ȡ�ļ���
		const path& getInternalName() const
		{
			return InternalName;
		}

	

	protected:
		// ת������·���ַ�����һ�������ַ���
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