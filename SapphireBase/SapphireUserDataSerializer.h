#ifndef _SAPPHIRE_USER_DATA_SERIALIZER
#define _SAPPHIRE_USER_DATA_SERIALIZER

class IAttributes;
class ISceneNode;

namespace Sapphire
{
	//! 从保存文件读取用户数据的接口
	/** 这个接口由用户来实现， 使可以通过ISceneManager读或写用户数据，读存数据用ISceneManager::loadScene()和ISceneManager::saveScene()
	*/
	class ISceneUserDataSerializer
	{
	public:

		virtual ~ISceneUserDataSerializer() {}

		//! 当场景管理器创建一个场景节点加载一个文件时调用
		virtual void OnCreateNode(ISceneNode* node) = 0;

		//! 当场景管理器读取一个场景节点加载一个文件时调用C
		/** 用户数据的指针包含一个用户数据的属性列表，它在读取的场景文件中关联到场景节点*/
		virtual void OnReadUserData(ISceneNode* forSceneNode,  IAttributes* userData) = 0;

		//! 当这个场景管理器在写入一个场景节点（比如到一个XML文件）时调用
		/** 实现这个方法，并且返回一个包含用户数据的你想要和场景节点一起保存的属性列表。
		 注意：这个场景管理器会调用drop来返回一个指针后，便不再需要它。 所以你不能为这个返回值创建一个对象并且返回一个长期存在的IAttribute对象， 在返回简单调用grab（）；
		 */
		virtual IAttributes* createUserData(ISceneNode* forSceneNode) = 0;
	};
}

#endif