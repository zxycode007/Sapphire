#ifndef _SAPPHIRE_I_SCENE_LOADER_
#define _SAPPHIRE_I_SCENE_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class ISceneNode;
	class ISceneUserDataSerializer;

	//! 这个类能够加载一个场景到场景管理器中
	/** 如果你想要引擎加载当前不支持的场景文件格式，那么实现这个接口，并且用ISceneManager::addExternalSceneLoader()添加你的新SceneLoader到引擎  */
	class ISceneLoader : public virtual IReferenceCounter
	{
	public:

		//! 如果这个类可以加载这个文件，返回true
		/** 这是通过文件扩展名来决定的
		\param filename 要测试的文件名
		\return 如果文件扩展名是支持的类型返回true */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! 如果这个类可以加载这个文件，返回true
		/**  
		\param file 要测试的文件
		\return 如果文件扩展名是支持的类型返回true */
		virtual bool isALoadableFileFormat(IReadFile* file) const = 0;

		//! 加载场景到场景管理器中
		/** \param file 包含这个场景的文件
		\param userDataSerializer: 如果要加载文件中的用户数据，关联到某些场景节点，实现ISceneUserDataSerializer
		接口并且提供它作为参数。否则指定这个参数为0
		\param rootNode 要加载场景的根节点，这个没有提供，那么这个场景将被加载到根节点
		\return 如果成功返回true，失败返回false*/
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0,
			ISceneNode* rootNode = 0) = 0;

	};
}

#endif