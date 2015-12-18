#ifndef _SAPPHIRE_IMESH_SCENE_NODE_
#define _SAPPHIRE_IMESH_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireISceneNode.h"

namespace Sapphire
{
	class IShadowVolumeSceneNode;
	class IMesh;

	//! 显示为一个静态网格的场景节点
	class IMeshSceneNode : public ISceneNode
	{
	public:

		//! 
		/** 使用setMesh（）来设置要显示的网格
		*/
		IMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1, 1, 1))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! 设置一个要显示的新网格
		/** \param mesh 要显示的网格 */
		virtual void setMesh(IMesh* mesh) = 0;

		//! 显示获取当前显示网格的定义
		/** \return 反这个节点显示网格的指针*/
		virtual IMesh* getMesh(void) = 0;

		//! 创建阴影容积场景节点作为这个节点的子节点
		/** 
		这个阴影可以用ZPass或Zfail方法渲染。ZPass要稍快一点，由于创建阴影容积相对容易点，
		但是这个方法在当相机在阴影容积里面会出现看起来丑陋的东西。这些缩放用ZFail方法不会
		发生。
		\param shadowMesh: 可选的  对于阴影容积自定义网格
		\param id: 阴影场景节点的ID。  这个id用于以后标识这个节点
		\param zfailmethod: 如果设置为true，这个阴影将使用zfail方法，否则使用zpass
		\param infinity: 阴影容积算法缩放这个阴影容积的值(对于zfail 阴影容积，我们只支持finite 阴影
		，所以相机zfar必须大于这个阴影的back cap，它依赖于infinity参数)
		\return 创建的阴影节点的指针。*/
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
			SINT32 id = -1, bool zfailmethod = true, Real infinity = 1000.0f) = 0;

		//! 设置这个场景节点不拷贝这个网格的材质是否在只读模式使用它们
		/** 这个方法可能会改变一个网格的材质，导致引用这个网格的所有网格节点发生改变。
		\param readonly 是在这个材质是否应该只读 */
		virtual void setReadOnlyMaterials(bool readonly) = 0;

		//! 检测场景节点不拷贝网格材质，而是否用只读模式使用它们。
		/** 这个标志可以通过setReadOnlyMaterials()设置
		\return 这个材质是否只读 */
		virtual bool isReadOnlyMaterials() const = 0;
	};
}

#endif