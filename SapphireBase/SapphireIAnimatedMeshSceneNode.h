#ifndef _SAPPHIRE_I_ANIMATED_MESH_SCENE_NODE_
#define _SAPPHIRE_I_ANIMATED_MESH_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireVector3.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireISceneNode.h"
#include "SapphireIBoneSceneNode.h"
#include "SapphireIAnimatedMeshMD2.h"



namespace Sapphire
{
	class IShadowVolumeSceneNode;

	enum E_JOINT_UPDATE_ON_RENDER
	{
		//!什么都不做
		EJUOR_NONE = 0,

		//! 获取这个网格的joint positions （对于附加节点）
		EJUOR_READ,

		//! 控制这个网格里的joint positions (eg. 布娃娃,或是从animateJoint设置动画animation )
		EJUOR_CONTROL
	};


	class IAnimatedMeshSceneNode;

	//! 对于终止的animation动画的事件捕捉的回调接口
	/** 实现这个接口并用IAnimatedMeshSceneNode::setAnimationEndCallback可以提醒一个动画是否播放完毕
	**/
	class IAnimationEndCallBack : public virtual IReferenceCounter
	{
	public:

		//!当动画播放完毕的时候会被调用
		/** 见 IAnimatedMeshSceneNode::setAnimationEndCallback 更多信息
		\param node: 这个动画播放完毕的节点 */
		virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node) = 0;
	};

	//! 能够显示一个活动网格和它的阴影的场景节点
	/** 阴影是可选的：如果阴影可以显示，就指向IAnimatedMeshSceneNode::createShadowVolumeSceneNode().*/
	class IAnimatedMeshSceneNode : public ISceneNode
	{
	public:

		IAnimatedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		virtual ~IAnimatedMeshSceneNode() {}

		//! 设置当前帧的帧数
		/** 从现在开始，动画从这一帧开始播放
		\param frame: 让动画从这一帧开始播放的帧数号
		这个帧号必须是对于这个场景节点的IMesh有效的帧号。关于细节见IAnimatedMesh::getMesh() */
		virtual void setCurrentFrame(Real frame) = 0;

		//! 设置循环动画的之间的帧数号
		/** 默认网格的0 - MaximalFrameCount
		\param begin: 循环起始帧
		\param end: 循环结束帧
		\return 如果成功返回true*/
		virtual bool setFrameLoop(SINT32 begin, SINT32 end) = 0;

		//! 设置动画播放速度
		/** \param framesPerSecond: 每秒播放帧数 */
		virtual void setAnimationSpeed(Real framesPerSecond) = 0;

		//! 获取这个动画播放的速度
		/** \return 每秒播放帧数 */
		virtual Real getAnimationSpeed() const = 0;

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
		\return 创建的阴影节点的指针。 
		 */
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
			SINT32 id = -1, bool zfailmethod = true, Real infinity = 1000.0f) = 0;


		//! 获取一个指向这个网格里的一个joint(如果这个网格是一个基于骨骼的网格).
		/** 
		通过这个方法，他可以附加场景节点到joint 例如：可以添加一个武器到一个动画模型的左手。
		例如
		\code
		ISceneNode* hand =
		yourAnimatedMeshSceneNode->getJointNode("LeftHand");
		hand->addChild(weaponSceneNode);
		\endcode
		请注意：这个由这个方法返回的joint在这次调用之前可能会不存在，并且
		在这个节点里的joint会通过它创建
		\param jointName: joint的名字
		\return 指向通过指定名字表示这个joint的场景节点,如果这个包含的网格
		没有一个蒙皮网格或是这个名字的joint并没被找到
		 */
		virtual IBoneSceneNode* getJointNode(const c8* jointName) = 0;

		//! 和getJointNode(const c8* jointName)一样, 但是基于ID
		virtual IBoneSceneNode* getJointNode(UINT32 jointID) = 0;

		//! 获取joint的数量
		/** \return 这个网格joint的数量*/
		virtual UINT32 getJointCount() const = 0;

		//! 开始一个默认的MD2动画
		/** 如果在这个场景节点的网格是一个MD2网格的话，通过这个方法，它可以容易的开始一个Run，Attack，Die或其它的动画。
		否则，什么都不会发生。
		\param anim: 一个MD2 动画类型，它可以被播放，例如站立动画EMAT_STAND
		\return 如果成功返回true，否则返回false */
		virtual bool setMD2Animation(EMD2_ANIMATION_TYPE anim) = 0;

		//! 开始一个特别的MD2动画
		/** 
		如果在这个场景节点的网格是一个MD2网格的话，通过这个方法，它可以容易的开始一个Run，Attack，Die或其它的动画。
		否则，什么都不会发生。
		这个方法用一个字符串来定义这个动画,如果这个动画是一个标准的MD2动画，你可以用EMD2_ANIMATION_TYPE枚举带来
		\param animationName: 要播放的动画名
		\return 如果成功返回true，否则返回false */
		virtual bool setMD2Animation(const c8* animationName) = 0;

		//! 返回当前显示的帧号
		virtual Real getFrameNr() const = 0;
		//! 返回当前起始帧号
		virtual SINT32 getStartFrame() const = 0;
		//! 返回当前结束帧号
		virtual SINT32 getEndFrame() const = 0;

		//! 设置默认的循环模式
		/** 如果设置为false，动画将不会循环播放*/
		virtual void setLoopMode(bool playAnimationLooped) = 0;

		//! 返回当前的循环模式
		/** 当动画循环播放返回true */
		virtual bool getLoopMode() const = 0;

		//! 设置在动画回放完毕时调用的回调接口
		/** 设置为0关闭之后回调
		请注意:这只有在非循环模式下才会被调用
		见IAnimatedMeshSceneNode::setLoopMode(). */
		virtual void setAnimationEndCallback(IAnimationEndCallBack* callback = 0) = 0;

		//! 设置这个场景节点不拷贝这个网格的材质是否在只读模式使用它们
		/** 这个方法可能会改变一个网格的材质，导致引用这个网格的所有网格节点发生改变。
		\param readonly 是在这个材质是否应该只读 */
		virtual void setReadOnlyMaterials(bool readonly) = 0;

		//! 检测场景节点不拷贝网格材质，而是否用只读模式使用它们。
		/** 这个标志可以通过setReadOnlyMaterials()设置
		\return 这个材质是否只读 */
		virtual bool isReadOnlyMaterials() const = 0;

		//! 设置一个新网格
		virtual void setMesh(IAnimatedMesh* mesh) = 0;

		//! 返回当前网格
		virtual IAnimatedMesh* getMesh(void) = 0;

		//! 如果这个网格是一个MD3网格，获一个特别MD3绝对变换的标签，或者它一个正常场景节点，返回绝对变换
		virtual const SMD3QuaternionTag* getMD3TagTransformation(const String & tagname) = 0;

		//! 设置在渲染中joint应该怎样更新  （Joint更新模式）
		virtual void setJointMode(E_JOINT_UPDATE_ON_RENDER mode) = 0;

		//! 设置以秒的转换时间
		/** 
		这需要开启joint，并且设置Joint模式到EJUOR_CONTROL。你必须调用animateJoints(),
		否则这个网格不会活动
		 */
		virtual void setTransitionTime(Real Time) = 0;

		//! 基于当前帧来活动在这个网格里的joint
		/** 还需要说明变换 */
		virtual void animateJoints(bool CalculateAbsolutePositions = true) = 0;

		//! 无视变换渲染网格 
		/** 剔除不受影响 */
		virtual void setRenderFromIdentity(bool On) = 0;

		//! 创建一个场景节点和它孩子的克隆
		/** \param newParent 一个可选的父节点
		\param newManager 一个可选的场景管理器
		\return 这个节点的新克隆 */
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0) = 0;

	};
}

#endif