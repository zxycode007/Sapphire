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
}


#endif