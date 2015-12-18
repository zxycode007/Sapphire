#ifndef _SAPPHIRE_IMESH_LOADER_
#define _SAPPHIRE_IMESH_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class IAnimatedMesh;


	//! 这个类可以从文件中加载一个动画网格
	/** 如果你需要加载引擎不支持的格式，需要实现你的新MeshLoader，然后通过ISceneManager::addExternalMeshLoader()
	添加到引擎。
    */
	class IMeshLoader : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshLoader() {}

		//! 如果这个文件可以被这个类加载返回true
		/** 这是通过基于文件扩展名决定
		\param filename 要测试的文件名
		\return 如果这个文件可以被这个类加载返回true */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! 从一个文件创建/加载一个动画网格
		/** \param file 要加载的文件句柄
		\return 指向创建网格的指针，如果创建失败返回0*/
		virtual IAnimatedMesh* createMesh(IReadFile* file) = 0;
	};

}


#endif