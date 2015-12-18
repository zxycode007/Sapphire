#ifndef _SAPPHIRE_IMESH_CACHE_
#define _SAPPHIRE_IMESH_CACHE_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IMesh;
	class IAnimatedMesh;
	class IAnimatedMeshSceneNode;
	class IMeshLoader;

	//! 这个网格缓存保存已经加载的网格，并且给它们提供一个接口
	/** 可以用ISceneManager::getMeshCache()访问它们。所有存在的scene Manager都会返回指向同一个网格缓存
	的指针，因为共享一个缓存。通过这个接口，可以手动添加新已加载的网格(如果ISceneManager::getMesh()不够的话)，
	移除它们和迭代访问已经加载的网格。
	 */
	class IMeshCache : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshCache() {}

		//! 添加一个已加载的网格到内部列表
		/** 通常ISceneManager::getMesh()是从一个文件加载网格。这个方法会搜寻已加载的网格列表，
		如果一个网格已经被加载并且如果它已经在列表和内存中存在返回一个指向它的指针。用 With IMeshCache::addMesh()，
		这个方法用于例如：网格加载器需要一次调用加载多个网格。它们可以通过这个方法添加额外的网格到scene manager。	 
		\param name 网格名，调用ISceneManager::getMesh()时，通过这个名字返回这个方法设置的网格
		\param mesh 指向这个网格的指针，该指针会被此名字所引用 */
		virtual void addMesh(const path& name, IAnimatedMesh* mesh) = 0;

		//! 从缓存中移除这个网格
		/** 在通过getMesh()加载一个网格后，这个网格可以通过这个方法从缓存中移除，释放大量内存
		\param mesh 指向要移除网格的指针 */
		virtual void removeMesh(const IMesh* const mesh) = 0;

		//! 返回在缓存中已加载网格的数量
		/** 可以使用getMesh（）和addMesh()加载新的网格到缓存中.你可以使用removeMesh(), getMeshNumber(),
		\return 缓存中的网格数 */
		virtual UINT32 getMeshCount() const = 0;

		//! 返回当前网格的索引，如果没找到返回-1
		/** \param mesh 要查询的网格指针
		\return 当前网格在缓存中的索引，-1表示没找到 */
		virtual SINT32 getMeshIndex(const IMesh* const mesh) const = 0;

		//! 基于索引号取得一个网格
		/** \param index: 网格的索引，范围是0到getMeshCount()-1.
		注意：这个只有在一个新网格加载或是移除之前这个值才有效
		\return 指向网格的指针，没有返回0.
		*/
		virtual IAnimatedMesh* getMeshByIndex(UINT32 index) = 0;


		//! 基于它的名字取得网格
		/** \param name 网格名，通常用文件名
		\return 指向网格的指针，没有返回0. */
		virtual IAnimatedMesh* getMeshByName(const  path& name) = 0;

		//! 通过索引获取一个已加载的网格名
		/** \param index: 网格的索引，范围是0到getMeshCount()-1.
		\return 如果这个名字找到，返回它，否则这个path是空的*/
		virtual const SNamedPath& getMeshName(UINT32 index) const = 0;

		//! 获取一个已加载的网格的名字
		/** \param mesh 要查询网格的指针
		\return 如果这个名字找到，返回它，否则这个path是空的*/
		virtual const SNamedPath& getMeshName(const IMesh* const mesh) const = 0;

		//! 重命名一个已加载的网格
		/** 注意重命名网格会改变网格的顺序，所以通过getMeshIndex()返回这个网格的索引
		会因此方式变化。
		\param index 在缓存中的这个网格的索引
		\param name 这个网格的新名字
		\return 重命名成功返回true. */
		virtual bool renameMesh(UINT32 index, const path& name) = 0;

		//! 重命名已加载的网格
		/** 注意重命名网格会改变网格的顺序，所以通过getMeshIndex()返回这个网格的索引
		会因此方式变化。
		\param mesh 要重命名的网格指针.
		\param name 这个网格的新名字
		\return 重命名成功返回true. */
		virtual bool renameMesh(const IMesh* const mesh, const path& name) = 0;

		//! 检测一个网格是已经被加载
		/** \param name 网格名，通常是文件名
		\return 如果已加载返回true，否则返回false*/
		virtual bool isMeshLoaded(const path& name) = 0;

		//! 清空整个网格缓存，移除所有网格
		/** 当在调用这个方法后，使用ISceneManager::getMesh()，所有网格会被重新加载
		警告：如果你有通过该ISceneManager::getMesh()获取的网格指针并且你每个grab它们，那么它们会无效
	   */
		virtual void clear() = 0;

		//! 清空在网格缓存中整个任何地方都不使用的网格
		/**当在调用这个方法后，使用ISceneManager::getMesh()，所有网格会被重新加载
		警告：如果你有通过该ISceneManager::getMesh()获取的网格指针并且你每个grab它们，那么它们会无效. */
		virtual void clearUnusedMeshes() = 0;
	};
}

#endif