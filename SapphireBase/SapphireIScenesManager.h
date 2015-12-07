#ifndef _SAPPHIRE_SCENE_
#define _SAPPHIRE_SCENE_


#include "SapphirePrerequisites.h"
//#include "SapphireMemoryAllocatorConfig.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireVector3.h"
#include "SapphireFont.h"
#include "SapphirePath.h"
#include "SapphireColorValue.h"
#include "SapphireKeyMap.h"
#include "SapphireDimension2D.h"
#include "SapphireEMeshWriterEnums.h"
#include "SapphireESceneNodeTypes.h"
#include "SapphireESceneNodeAnimatorType.h"
#include "SapphireIUserDataSerializer.h"
#include "SapphireETerrianElements.h"
#include "SapphireDefines.h"






namespace Sapphire
{
	//  对于渲染的通道枚举
	/** 一个参数传递到场景管理器的registerNodeForRendering()方法来指定在绘制时这个节点中相对于其它节点的关系
	*/
	enum E_SCENE_NODE_RENDER_PASS
	{
		//!  当前活动的渲染通道
		ESNRP_NONE = 0,

		//! 相机通道，一个活动观察点被设置到。这是第一个传递的
		ESNRP_CAMERA = 1,

		//! 在这个通道，光源被变换到相机空间并且添加到驱动
		ESNRP_LIGHT = 2,

		//! 这是用于天空盒子
		ESNRP_SKY_BOX = 4,

		//! 所有的普通对象都能够在这里注册
		/**
		SceneManager::getSceneNodeRenderPass()不会返回这个值。
		如果你用这个值调用registerNodeForRendering（通常默认）,这个场景管理器会通过对象是固体还是透明并且被注册到SNRT_TRANSPARENT或者SNRT_SOLD的信息来自动决定。
		注意他会只注册这个节点到作为一个类型。如果你的场景节点有两个固体或透明材质类型，注册两次(一次固体SNRT_SOLD，一次透明SNRT_TRANSPARENT)，
		而且在render（）方法调用getSceneNodeRenderPass（）找出当前渲染渲染通道和这个节点相符合的渲染器
		*/
		ESNRP_AUTOMATIC = 24,

		//! 没有材质的固体场景节点或者特别的场景节点
		ESNRP_SOLID = 8,

		//!透明的场景节点，在固定节点后绘制。它们在渲染顺序上从按从后到前排列
		ESNRP_TRANSPARENT = 16,

		//! 场景透明效果节点，绘制在透明节点之后，它们在渲染顺序上从按从后到前排列
		ESNRP_TRANSPARENT_EFFECT = 32,

		//! 绘制在固体之后，在透明节点之前， 这时绘制阴影容积
		ESNRP_SHADOW = 64
	};
	//文件加载
	class IReadFile;
	class ITextire;
	class IImage;
	class SMaterial;
	//三角形选择器
	class ITriangleSelector;
	class IAttributes;

	//当动画的模型网格
	class IAnimatedMesh;
	//网格缓冲区
	class IMeshCache;
	//显示驱动
	class IVideoDriver;
	//gui环境
	class IGUIEnvironment;
	//获取文件系统
	class IFileSystem;
	class IWriteFile;
	class IXMLWriter;
	//体积光场景节点
	class IVolumeLightSceneNode;
	//场景节点
	class ISceneNode;
	//网格场景节点
	class IMeshSceneNode;
	//带动画的网格场景节点
	class IAnimatedMeshSceneNode;
	//网格
	class IMesh;
	//摄像机场景节点
	class ICameraSceneNode;
	//光源场景节点
	class ILightSceneNode;
	//BillBoard节点
	class IBillboardSceneNode;
	//纹理
	class ITexture;
	//场景粒子系统节点
	class IParticleSystemSceneNode;
	//地形场景节点
	class ITerrainSceneNode;
	//假的变换节点
	class IDummyTransformationSceneNode;
	//文本场景节点
	class ITextSceneNode;
	//billboard文本场景节点
	class IBillboardTextSceneNode;
	//场景节点的Animator
	class ISceneNodeAnimator;
	//碰撞反应的场景节点的Animator
	class ISceneNodeAnimatorCollisionResponse;
	// 元信息三角形选择器
	class IMetaTriangleSelector;
	//网格加载器
	class  IMeshLoader;
	//场景碰撞管理器
	class ISceneCollisionManager;
	//网格操作器
	class IMeshManipulator; 
	//事件
	class SEvent;
	//场景节点工厂
	class ISceneNodeFactory;
	//场景节点动画工厂
	class ISceneNodeAnimatorFactory;
	//网格写入器
	class IMeshWriter;
	//蒙皮网格
	class ISkinnedMesh;
	//
	class ILightManager; 
	//几何体生成器
	class IGeometryCreator;




	/***
	Sapphire 的场景管理器
	*/
	class _SapphireExport  ISceneManager : public virtual IReferenceCounter
	{
	public:

		//获取一个带动画的模型 网格的指针，如果没有加载，先从文件加载。
		//如果失败，返回NULL，另外网格会被丢弃
		virtual IAnimatedMesh* getMesh(const String& filename) = 0;

		//! 
		//获取一个网格缓冲区的接口，它为整个场景管理器提供网格信息
		/** 对于这个接口它可以手动添加新加载的网格（如果SceneManager::getMesh()不够用），去移除它们或迭代访问已经加载的网格 */
		virtual IMeshCache* getMeshCache() = 0;

		//! 获取显示驱动
		/** \返回一个显示驱动的指针
		这个指针不能被丢弃*/
		virtual  IVideoDriver* getVideoDriver() = 0;

		//! 获取活动的GUI环境
		/** \返回一个GUIEnvironment的指针
		 */
		virtual  IGUIEnvironment* getGUIEnvironment() = 0;

		//!获取文件系统 */
		virtual  IFileSystem* getFileSystem() = 0;

		//! 添加一个体积光场景节点
		/** 例如
		scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
		32, 32, //细分 U/V
		video::SColor(0, 180, 180, 180), //foot color
		video::SColor(0, 0, 0, 0) //tail color
		);
		if (n)
		{
		n->setScale(Vector3(46.0f, 45.0f, 46.0f));
		n->getMaterial(0).setTexture(0, smgr->getVideoDriver()->getTexture("lightFalloff.png"));
		}
		\return 如果成功返回volumeLight的指针，否则返回NULL
		*/
		virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent = 0, SINT32 id = -1,
			const UINT32 subdivU = 32, const UINT32 subdivV = 32,
			const ColourValue foot = ColourValue(51, 0, 230, 180),
			const ColourValue tail = ColourValue(0, 0, 0, 0),
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 
		// 添加一个立方体盒子场景节点
		/** \param size: 盒子大小,每个维度都相同
		\param parent: 场景节点的父亲节点，如果为0，则没有父节点
		\param id: 节点的ID.
		\param position: 相对于父节点的空间位置
		\param rotation: 初始的旋转
		\param scale: 初始的缩放
		\return 指向创建的测试节点的指针 */
		virtual IMeshSceneNode* addCubeSceneNode(Real size = 10.0f, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 头一天几一个给定半径和细节的球体场景节
		/** \param radius: 球体半径
		\param polyCount: 水平和垂直方向的顶点数。总的多边形数是=polyCount*polyCount
		对于一个网格缓冲区的索引数有着16位限制，这个参数必须低于256
		\param parent: 场景节点的父节点，如果为0则没有父节点
		\param id: 节点的ID
		\param position: 相对于父节点的空间位置
		\param rotation: 初始的旋转
		\param scale: 初始的缩放
		\return Pointer 指向创建的测试节点的指针 */
		virtual IMeshSceneNode* addSphereSceneNode(Real radius = 5.0f, SINT32 polyCount = 16,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 添加可以渲染的一个动画网格模型的一个场景节点 
		/** \param mesh: 这些要显示和已加载的动画网格模型
		\param parent: 场景节点的父节点，如果为0则没有父节点
		\param id: 节点的ID
		\param position: 相对于父节点的空间位置
		\param rotation: 初始的旋转
		\param scale: 初始的缩放
		\param alsoAddIfMeshPointerZero: 如果传递个空指针，也添加场景节点
		\return 指向创建的测试节点的指针.
		*/
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false) const = 0;


		//! 添加用于渲染的静态网格的一个场景节点（长期保留的网格）
		/** \param mesh: 要显示的静态网格的已加载的指针
		 \param parent: 场景节点的父节点，如果为0则没有父节点
		 \param id: 节点的ID
		 \param position: 相对于父节点的空间位置
		 \param rotation: 初始的旋转
		 \param scale: 初始的缩放
		 \param alsoAddIfMeshPointerZero: 如果传递个空指针，也添加场景节点
		 \return 指向创建的测试节点的指针.
		 */
		virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent = 0, SINT id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false) = 0;


		//! 添加一个用于渲染水体表面的网格的场景节点
		/** 当材质类型设定为EMT_TRANSPARENT_REFLECTION，看起来比较真实
		\param waveHeight: 水波的高度
		\param waveSpeed: 水波的速度
		\param waveLength: 水波的长度
		\param mesh: 要显示的水波静态网格的已加载的指针
		\param parent: 场景节点的父节点，如果为0则没有父节点
		\param id: 节点的ID
		\param position: 相对于父节点的空间位置
		\param rotation: 初始的旋转
		\param scale: 初始的缩放
		\param alsoAddIfMeshPointerZero: 如果传递个空指针，也添加场景节点
		\return 指向创建的测试节点的指针. */
		virtual ISceneNode* addWaterSurfaceSceneNode(IMesh* mesh,
			Real waveHeight = 2.0f, Real waveSpeed = 300.0f, Real waveLength = 10.0f,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 添加一个用于八叉树场景图的场景节点（带动画的网格）
		/** 这是一个很适合用于渲染大量几何体方法。八叉树是建立在游离的网格基础上
		\param mesh: 这个网格包含用建立8叉树的包含所有几何体的网格，如果这个动画网格有大于1帧， 则使用第一帧
		\param parent: 场景节点的父节点
		\param id: 节点的ID
		\param minimalPolysPerNode: 指定一个八叉树节点包含的最小多边形数，如果一个节点多边形少于这个值，它将不会被分割为更小的节点
		\param alsoAddIfMeshPointerZero: 如果传递个空指针，也添加场景节点
		\return 指向创建的测试节点的指针. */
		virtual IMeshSceneNode* addOctreeSceneNode(IAnimatedMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 512, bool alsoAddIfMeshPointerZero = false) = 0;



		//! 添加一个用于八叉树场景图的场景节点
		/** 这是一个很适合用于渲染大量几何体方法。八叉树是建立在游离的网格基础上比BSP树块很多
		\param mesh: 这个网格包含用建立8叉树的包含所有几何体的网格
		\param parent: 场景节点的父节点
		\param id: 节点的ID
		\param minimalPolysPerNode:指定一个八叉树节点包含的最小多边形数，如果一个节点多边形少于这个值，它将不会被分割为更小的节点
		\param alsoAddIfMeshPointerZero: 如果传递个空指针，也添加场景节点
		\return 指向创建的测试节点的指针. */
		virtual IMeshSceneNode* addOctreeSceneNode(IMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 256, bool alsoAddIfMeshPointerZero = false) = 0;



		//! 添加一个相机场景节点到场景图中并设置它为活动的相机
		/** 这个相机不会对用户的输入有反应，应该用addCameraSceneNodeFPS()创建一个。如果你想移动或者活动它。用animator或者
		ISceneNode::setPosition(), ICameraSceneNode::setTarget()这些方法。默认，这个相机观察一个位置(使用setTarget)并且它的场景的旋转（用setRotation()设置）是独立的。
		如果你想通过setRotation()来控制相机观察的方向，那么对它调用ICameraSceneNode::bindTargetAndRotation(true)
		\param position: 相对于父节点的空间位置
		\param lookat: 这个相机的观察的位置，也称为目标
		\param parent: 相机节点的父节点. 可以为空. 如果父节点移动，相机也会移动
		\param id: 相机ID
		\param makeActive 设置该相机为活动的一个，确定同时只能有一个活动相机、
		\return 如果成功返回一个相机接口的指针*/
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& lookat = Vector3(0, 0, 100),
			SINT32 id = -1, bool makeActive = true) = 0;


		//! 添加一个maya类型的用户控制的相机场景节点到场景图
		/**
		这是一个使用animator提供类似在MAYA3d软件的鼠标控制的相机。在应用animator后setPositon不能再支持这个相机。
		代替的是setTarget，去修正相机这个目标这个相机的盘旋。并且setDistance去设置这个目标的当前距离，（相机的盘旋轨道半径）
		\param parent:相机节点的父节点. 可以为空.
		\param rotateSpeed: 相机的旋转速度
		\param zoomSpeed: 相机的放大速度
		\param translationSpeed: 相机的移动速度
		\param id: 相机ID
		\param distance 初始的相机与对象之间的距离
		\param makeActive 设置该相机为活动的一个，确定同时只能有一个活动相机、
		\return 如果成功返回一个相机接口的指针 */
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			Real rotateSpeed = -1500.f, Real zoomSpeed = 200.f,
			Real translationSpeed = 1500.f, Real id = -1, Real distance = 70.f,
			bool makeActive = true) = 0;

		//! 
		//添加一个用animator提供了鼠标和键盘操作的FPS相机节点
		/**
		这个FPS相机计划提供一个像典型FPS射击游戏的相机行为.它用于简单的demo和原型，但不能提供一个对于商业级游戏的完整解决方案
		它绑定相机场景节点旋转到look-at target：@see ICameraSceneNode::bindTargetAndRotation()。用这个相机，你用鼠标观察，并且用游标键来行走、
		如果你想要改变Key布局，可以去指定自己的keymap。例如使相机控制的游标键改为wsad

		SKeyMap keyMap[8];
		keyMap[0].Action = EKA_MOVE_FORWARD;
		keyMap[0].KeyCode = KEY_UP;
		keyMap[1].Action = EKA_MOVE_FORWARD;
		keyMap[1].KeyCode = KEY_KEY_W;

		keyMap[2].Action = EKA_MOVE_BACKWARD;
		keyMap[2].KeyCode = KEY_DOWN;
		keyMap[3].Action = EKA_MOVE_BACKWARD;
		keyMap[3].KeyCode = KEY_KEY_S;

		keyMap[4].Action = EKA_STRAFE_LEFT;
		keyMap[4].KeyCode = KEY_LEFT;
		keyMap[5].Action = EKA_STRAFE_LEFT;
		keyMap[5].KeyCode = KEY_KEY_A;

		keyMap[6].Action = EKA_STRAFE_RIGHT;
		keyMap[6].KeyCode = KEY_RIGHT;
		keyMap[7].Action = EKA_STRAFE_RIGHT;
		keyMap[7].KeyCode = KEY_KEY_D;

		camera = sceneManager->addCameraSceneNodeFPS(0, 100, 500, -1, keyMap, 8);

		\param parent: 相机节点的父节点. 可以为空.
		\param rotateSpeed: 摄像机旋转的角速度, 只针对鼠标
		\param moveSpeed: 相机单位每秒移动速度，移动有游标键控制
		\param id: i相机ID
		\param keyMapArray: 一个keymap数组的可选指针。指定什么键用于相机的移动。如果为null,则使用默认KeyMap.
		可以在数组中一次定义多个action来绑定多个键到同一个action
		\param keyMapSize: keymap数组中条目数量
		\param noVerticalMovement: 设置这个选项为true，使相机只在水平平面移动，关闭垂直移动。默认为false，
		'false',
		\param jumpSpeed: 这个相机跳跃时的移动速度
		\param invertMouse: 设置为true的话，鼠标移动和相机移动反向，默认为false
		\param makeActive 设置该相机为活动的一个，确定同时只能有一个活动相机、
		\return 如果成功返回一个相机接口的指针. */
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			Real rotateSpeed = 100.0f, Real moveSpeed = 0.5f, SINT32 id = -1,
			SKeyMap* keyMapArray = 0, SINT32 keyMapSize = 0, bool noVerticalMovement = false,
			Real jumpSpeed = 0.f, bool invertMouse = false,
			bool makeActive = true) = 0;




		//! 添加一个动态的光源场景节点到场景图中
		/**
		这个光源将施加动态光源在场景中所有其它的场景节点上。光照材质标志：MTF_LIGHTINGk开启（默认设置到绝大部分场景节点上）
		\param parent: 光照节点的父节点，可随父节点移动，可以为空
		\param position: 相对于父节点的空间位置
		\param color: 光源的漫反射颜色。环境光颜色或镜面反射光颜色可以通过ILightSceneNode::getLightData()来设置
		\param radius: 光照范围
		\param id: 节点ID
		\return 如果成功返回一个光源接口的指针 */
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			ColourValue = ColourValue(1.0f, 1.0f, 1.0f),
			Real radius = 100.0f, SINT32 id = -1) = 0;



		//! 在场景图中添加一个billboard 场景节点
		/**
		一个billboard是一个3d精灵： 一个2d的元素，它经常对着相机。通常用于爆炸，火焰，光晕和类似的东西
		\param parent 该节点的父节点，可随父节点移动，可以为空
		\param size billboard的大小，这个是一个dimension，包含长宽
		\param position 相对于父节点的空间位置
		\param id 节点ID
		\param colorTop billboard顶点颜色的最高值（默认white）
		\param colorBottom billboard顶点颜色的最低值（默认white)
		\return 如果成功返回一个billboard接口的指针*/
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const dimension2d<Real>& size = dimension2d<Real>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue colorTop = ColourValue(0xFFFFFFFF), ColourValue colorBottom = ColourValue(0xFFFFFFFF)) = 0;



		//! 
		//在场景图中添加一个天空盒子场景节点
		/** 一个天空盒子是一个大型立方体，它带有6个纹理并且绘制在相机位置周围
		\param top: 顶部平面纹理
		\param bottom: 底部平面纹理
		\param left: 左边平面纹理
		\param right: 右边平面纹理
		\param front: 前边平面纹理
		\param back: 右平面纹理
		\param parent: 天空盒子父节点，一个天空盒子通常没有父节点，所以应该为空. 注意：给天空盒子设置一个父节点，这个盒子在绘制时不会改变
		\param id: 节点ID
		\return  如果成功返回一个接口的指针 */
		virtual ISceneNode* addSkyBoxSceneNode(ITexture* top, ITexture* bottom,
			ITexture* left, ITexture* right, ITexture* front,
			ITexture* back, ISceneNode* parent = 0, SINT32 id = -1) = 0;



		//! 在场景图中添加一个天穹节点
		/** 一个天穹是一个在内部使用一个全景纹理的大型半球体并绘制在摄像机的周围
		\param texture: 天穹体的纹理
		\param horiRes: 球体的一个水平层的顶点数
		\param vertRes: 球体的一个垂直层的顶点数
		\param texturePercentage: 纹理高度的使用比例，应该在0和1之间。
		\param spherePercentage: 球体的绘制比例，值应该在0到2之间， 1是精确的半球并且2是完整球体
		\param radius 球体的半径
		\param parent: 天穹的父节点,一个天穹通常没有父节点，所以应该为空。注意如果父节点被设置，这个天穹绘制不会被改变
		\param id: 节点ID
		\return 如果成功返回一个接口的指针 */
		virtual ISceneNode* addSkyDomeSceneNode(ITexture* texture,
			UINT32 horiRes = 16, UINT32 vertRes = 8,
			Real texturePercentage = 0.9, Real spherePercentage = 2.0, Real radius = 1000.f,
			ISceneNode* parent = 0, Real id = -1) = 0;



		//! 添加一个粒子系统节点到场景图中
		/**
		\param withDefaultEmitter: 创建一个默认工作的粒子发射器。设为true在action中看到一个粒子系统.
		如果设置为false，你通过IParticleSystemSceneNode::setEmitter()设置一个粒子发射器
		\param parent: 父节点，可以为空
		\param id: 节点ID
		\param position: 相对于父节点的空间位置
		\param rotation: 场景节点初始旋转
		\param scale: 场景节点的初始缩放
		\return  如果成功返回一个接口的指针 */
		virtual IParticleSystemSceneNode* addParticleSystemSceneNode(
			bool withDefaultEmitter = true, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 添加一个地形场景节点到场景图中
		/** 这个节点用GEO mipmapping的技术实现一个简单地形渲染器。
		为了减少远处三角形块的细节。
		这个场景节点科研价值地形并且在运行时更新索引打开非常非常大的地形视野
		它用CLOD(连续LOD)算法通过检测相机到这个Patch的距离来更新每个基于一个LOD的Patch的索引

		地形Patch的大小必须为2^N+1 如i.e. 8+1(9), 16+1(17), etc.
		MaxLOD 直接依赖地形patch的大小
		LOD 0 对于一个Patch包含在最大细节下所有的三角形索引。
		每个Lod上升1，创建的索引-2^lod, 所以对于LOD 1， 步进是2，对于LOD 2，步进是4,
		LOD 3 -8.  这个步进不大于path的大小。所以LOD 8, 和一个大小为17的patch， 会请求2^8=256个顶点索引,一个patch不可能有17的大小。
		所以maxLOD 5，一个patch大小17（2^4=16）,LOD 1（每2个顶点） LOD2 (每4个顶点)  LOD3(每8个顶点)  LOD4（每16个顶点）
		\param heightMapFileName: 在磁盘文件名，读取顶点数据。这应该为一个灰度缩放位图
		\param parent: P 父节点，可以为空
		\param id: 节点ID
		\param position: 节点的绝对位置
		\param rotation: 节点的绝对旋转值. （没有实现）
		\param scale: 地形缩放因子，如果你用129*129的高程图，在游戏中地形为12900*12900个游戏单位，它用缩放一种vector3( 100.0f, 100.0f, 100.0f )
		如果你Y缩放因子为0，那么地形会扁平化
		\param vertexColor: 顶点默认颜色，如果没有指定这个场景节点的纹理，那么所有顶点都会是这个颜色。 默认为白色。
		\param maxLOD: 节点的最大LOD,如果你知道在做什么再改变它，这可能会出现奇怪的情况
		\param patchSize: 地形的patch大小，如果你知道在做什么再改变它，这可能会出现奇怪的情况
		\param smoothFactor: 顶点平滑因子
		\param addAlsoIfHeightmapEmpty: 可以通过空高度图添加地形节点
		\return 创建的场景节点的指针，如果地形没能创建，可以为空。
		*/
		virtual ITerrainSceneNode* addTerrainSceneNode(
			const path& heightMapFileName,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			ColourValue vertexColor = ColourValue(255, 255, 255, 255),
			SINT32 maxLOD = 5, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17, SINT32 smoothFactor = 0,
			bool addAlsoIfHeightmapEmpty = false) = 0;

		//!添加一个地形场景节点到场景图中
		/** 同上，只不过由IReaderFile指针加载
		\param heightMapFile: 在磁盘文件指针，读取顶点数据。这应该为一个灰度缩放位图
		\param parent: P 父节点，可以为空
		\param id: 节点ID
		\param position: 节点的绝对位置
		\param rotation: 节点的绝对旋转值. （没有实现）
		\param scale: 地形缩放因子，如果你用129*129的高程图，在游戏中地形为12900*12900个游戏单位，它用缩放一种vector3( 100.0f, 100.0f, 100.0f )
		如果你Y缩放因子为0，那么地形会扁平化
		\param vertexColor: 顶点默认颜色，如果没有指定这个场景节点的纹理，那么所有顶点都会是这个颜色。 默认为白色。
		\param maxLOD: 节点的最大LOD,如果你知道在做什么再改变它，这可能会出现奇怪的情况
		\param patchSize: 地形的patch大小，如果你知道在做什么再改变它，这可能会出现奇怪的情况
		\param smoothFactor: 顶点平滑因子
		\param addAlsoIfHeightmapEmpty: 可以通过空高度图添加地形节点
		\return 创建的场景节点的指针，如果地形没能创建，可以为空。 */
		virtual ITerrainSceneNode* addTerrainSceneNode(
			IReadFile* heightMapFile,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			ColourValue vertexColor = ColourValue(255, 255, 255, 255),
			SINT32 maxLOD = 5, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17, SINT32 smoothFactor = 0,
			bool addAlsoIfHeightmapEmpty = false) = 0;


		//! 添加一个空的场景节点到场景图中
		/** 可以用在高级变换或者场景图结构
		\return 返回节点指针 */
		virtual ISceneNode* addEmptySceneNode(ISceneNode* parent = 0, SINT32 id = -1) = 0;


		//! 添加一个假变换场景节点到场景图中
		/** 这个场景节点不会被渲染， 也不能对set/getPosition，set/getRotation 和 set/getScale作出反应。它以一个矩阵作为类似变换。
		\return 返回节点指针 */
		virtual IDummyTransformationSceneNode* addDummyTransformationSceneNode(
			ISceneNode* parent = 0, SINT32 id = -1) = 0;


		//! 添加一个文本场景节点，它显示一个2d文本到3维空间的一个位置
		virtual ITextSceneNode* addTextSceneNode(IGUIFont* font, const String* text,
			ColourValue color = ColourValue(100, 255, 255, 255),
			ISceneNode* parent = 0, Vector3& position = Vector3(0, 0, 0),
			SINT32 id = -1) = 0;



		//! 添加一个使用billboard的文本场景节点。这个节点会有文字在上面， 通过距离进行缩放
		/**
		\param font 用在billboard上的字体， 传递0用GUI环境默认的字体
		\param text 字符串文本
		\param parent 父节点. 0是否root节点
		\param size billboard的宽与高
		\param position billboards相对父节点的位置
		\param id: 节点ID
		\param colorTop billboard顶点颜色的最高值（默认white）
		\param colorBottom billboard顶点颜色的最低值（默认white)
		\return 返回节点指针 */
		virtual IBillboardTextSceneNode* addBillboardTextSceneNode(IGUIFont* font, const String* text,
			ISceneNode* parent = 0,
			const  dimension2d<Real>& size = dimension2d<Real>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue colorTop = ColourValue(0xFFFFFFFF), ColourValue colorBottom = ColourValue(0xFFFFFFFF)) = 0;


		//! 添加一个山丘平面网格到这个网格池中
		/** .它通常用于引擎的快速测试。你可以指定在平面上有多少山丘并且它们有多高。尽管你必须为它起个名字.
	    ,由于这个网格添加到网格池中，并且可以通过ISceneManager::getMesh()取得
		\param name: 网格的名字，为了可以由 ISceneManager::getMesh(),它必须被指定。
		\param tileSize: 网格的tile大小 (10.0f, 10.0f)是一个不错的值
		\param tileCount: 指定有多少tile。 如果你指定tile大小（10.0f,10.0f）并且tileCount(10,10)
		那么你取得一个100个tile构成的100.0f*100.0f的矩形区域
		\param material: 这个网格的材质
		\param hillHeight: 山丘的高度。
		\param countHills: 山丘在平面上的数量
		\param textureRepeatCount: 定义在x和y方向纹理重复的次数
		return 如果创建失败返回NULL。 这个原因由于无效参数造成*/
		virtual IAnimatedMesh* addHillPlaneMesh(const  path& name,
			const dimension2d<Real>& tileSize, const  dimension2d<UINT32>& tileCount,
			SMaterial* material = 0, Real hillHeight = 0.0f,
			const dimension2d<Real>& countHills = dimension2d<Real>(0.0f, 0.0f),
			const dimension2d<Real>& textureRepeatCount = dimension2d<Real>(1.0f, 1.0f)) = 0;



		//! 添加一个静态地形网格到网格池中
		/** 这个网格由一个纹理文件和一个高度图文件在运行中创建. 两个文件都可以很巨大
		(8000x8000 像素应该没问题)因为有必要的话，生成器会分割这个文件为更小的纹理。
		你必须为这个网格指定一个名字，因为网格被添加的网格池中，可以由ISceneManager::getMesh()通过其名字做为参数取得
		\param meshname: 这个网格的名字，ISceneManager::getMesh()可以取得
		\param texture: 地形的纹理，请注意：这通常不是一个硬件级纹理（ITexture）,只作为一个IImage软件纹理，你可以加载由IVideoDriver::createImageFromFile()来加载这个纹理
		\param heightmap: 一个灰度高度图像，就如同纹理，它可以由IVideoDriver::createImageFromFile()创建。创建的三角形的数量取决于这个纹理的大小，所以用小的高度图，
		可以提供渲染速度。
		\param stretchSize: 参数定义在高度图上的一个像素有多大
		\param maxHeight: 定义一个白色像素在高度图上有多高
		\param defaultVertexBlockSize: 默认顶点块大小，顶点间的初始尺寸
		\return Null 如果创建失败，可能因为无效参数，或者这个网格名已经存在,或者纹理没有找到；如果成功，返回网格指针 */
		virtual IAnimatedMesh* addTerrainMesh(const  path& meshname,
			IImage* texture, IImage* heightmap,
			const  dimension2d<Real>& stretchSize = dimension2d<Real>(10.0f, 10.0f),
			Real maxHeight = 200.0f,
			const  dimension2d<UINT32>& defaultVertexBlockSize = dimension2d<UINT32>(64, 64)) = 0;



		//! 添加一个静态箭头网格到网格池
		/** \param name 网格名
		\param vtxColorCylinder 圆柱体的颜色
		\param vtxColorCone 圆锥体的颜色
		\param tesselationCylinder 组成圆柱体侧面的四边形数（圆柱体侧面展开是一个矩形）
		\param tesselationCone 组成圆锥体顶部的三角形数
		\param height 箭头的总高度  （箭头高度=圆锥高度+圆柱高度）
		\param cylinderHeight 圆柱体的总高度，应该小于总高度
		\param widthCylinder 圆柱体直径
		\param widthCone 圆锥体的底座的直径， 应该小于圆锥体的直径
		\return 如果成功返回箭头网格的指针，否则就是0
	     */
		virtual IAnimatedMesh* addArrowMesh(const path& name,
			ColourValue vtxColorCylinder = ColourValue(0xFFFFFFFF),
			ColourValue vtxColorCone = ColourValue(0xFFFFFFFF),
			UINT32 tesselationCylinder = 4, UINT32 tesselationCone = 8,
			Real height = 1.f, Real cylinderHeight = 0.6f,
			Real widthCylinder = 0.05f, Real widthCone = 0.3f) = 0;



		//! 添加一个静态球体网格到网格池
		/** \param name 网格的名字
		\param radius 球体的半径
		\param polyCountX 球体水平tile的四边形数量
		\param polyCountY 球体垂直tile的四边形数量
		\return 如果成功，返回球体网格指针，否则返回0
		 */
		virtual IAnimatedMesh* addSphereMesh(const path& name,
			Real radius = 5.f, UINT32 polyCountX = 16,
			UINT32 polyCountY = 16) = 0;


		//! 添加一个体积光网格到网格池
		/** \param name 网格名
		\param SubdivideU 水平细分数
		\param SubdivideV 垂直细分数
		\param FootColor 光的底部颜色
		\param TailColor 光的顶部颜色
		\return 如果失败返回0
		*/
		virtual IAnimatedMesh* addVolumeLightMesh(const path& name,
			const UINT32 SubdivideU = 32, const UINT32 SubdivideV = 32,
			const ColourValue FootColor = ColourValue(51, 0, 230, 180),
			const ColourValue TailColor = ColourValue(0, 0, 0, 0)) = 0;



		//! 获取ROOT场景节点
		/** 这索引场景节点的root节点.这个场景根节点是一个特殊的场景节点,它的存在只用于管理所有的场景节点。
		它不能从场景中移除。
		\return 返回根节点指针
		 */
		virtual ISceneNode* getRootSceneNode() = 0;


		//! 获取指定ID的第一个场景节点
		/** \param id: 要搜索的id
		\param start: 从这个场景节点开始，所有的只节点都会被搜索。如果NULL被指定，那么会从根节点开始
		\return 指向这个id的首个场景节点，如果没有，返回空
		*/
		virtual ISceneNode* getSceneNodeFromId(SINT32 id, ISceneNode* start = 0) = 0;




		//! 获取指定名字的第一个场景节点
		/** \param name: 要搜索的名字
		\param start: 从这个场景节点开始，所有的只节点都会被搜索。如果NULL被指定，那么会从根节点开始
		\return 指向这个id的首个场景节点，如果没有，返回空 */
		virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start = 0) = 0;



		//! 获取指定类型的场景节点
		/** \param type: 要搜索的场景节点
		\param start: 从这个场景节点开始，所有的只节点都会被搜索。如果NULL被指定，那么会从根节点开始
		\return 指向这个id的首个场景节点，如果没有，返回空 */
		virtual ISceneNode* getSceneNodeFromType(ESCENE_NODE_TYPE type, ISceneNode* start = 0) = 0;



		//! 获取指定类型的场景节点
		/** \param type: 要查找的场景节点类型(ESNT_ANY 将返回所有孩子节点).
		\param outNodes: 要填充的结果数组
		\param start: 从这个场景节点开始，所有的只节点都会被搜索。如果NULL被指定，那么会从根节点开始 */
		virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type,
			vector<ISceneNode*>& outNodes,
			ISceneNode* start = 0) = 0;


		//! 获取当前活动相机
		/** \return 返回当前活动相机的指针,如果为NULL， 那么没有创建相机*/
		virtual ICameraSceneNode* getActiveCamera() const = 0;



		//! 设置当前活动的相机
		/** 前一个活动相机会失活
		\param camera: 这个新的被激活的相机节点 */
		virtual void setActiveCamera(ICameraSceneNode* camera) = 0;



		//! 通过场景管理器设置模板缓冲区阴影的颜色
		virtual void setShadowColor(ColourValue color = ColourValue(150, 0, 0, 0)) = 0;

		//! 获取当前阴影的颜色
		virtual ColourValue getShadowColor() const = 0;

		//! 为在一个特定的时候渲染它注册一个节点
		/** 这个方法应该只被用在一个场景节点的ISceneNode::OnRegisterSceneNode()
		\param node: 会绘制而注册的节点. 通常场景节点可以用设置'this'作为一个参数
		\param pass:指定当节点要被绘制时与其它节点的关系。例如，如果节点是一个阴影，它通常是在其它所有节点绘制之后绘制，并且使用ESNRP_SHADOW.
		看场景E_SCENE_NODE_RENDER_PASS的细节
		\return 场景会被渲染（通道被剔除） */
		virtual UINT32 registerNodeForRendering(ISceneNode* node,
			E_SCENE_NODE_RENDER_PASS pass = ESNRP_AUTOMATIC) = 0;



		//! 绘制所有的场景节点
		/** 这只能在IVideoDriver::beginScene()和IVideoDriver::endScene()之间执行. 
		请注意：当调用这个函数时，这个场景不仅在绘制，而且还激活场景中存在的场景节点的animator，
		剔除，对场景节点的剔除
		 */
		virtual void drawAll() = 0;


		//! 创建一个旋转的animator，它对自己的关联节点进行旋转
		/** \param rotationSpeed 指定动画的旋转的速度 每10毫秒1度
		\return 这个animator. 关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop()
		*/
		virtual ISceneNodeAnimator* createRotationAnimator(const Vector3& rotationSpeed) = 0;



		//! 创建一个环绕飞行的animator， 它使得关联节点绕一个中心环绕飞行
		/** \param center: 环绕的中心
		\param radius: 环绕半径
		\param speed: 环绕速度，每毫秒弧度
		\param direction: 指定up向量，用于这个网格的对齐
		\param startPosition: 这个animator开始环绕时的起始位置. 值是一个圆的倍数， 0.5是环绕路径的一半
		\param radiusEllipsoid: 如果radiusEllipsoid != 0 那么radius2从一个椭圆开始。值是一个圆的倍数， 0.5是环绕路径的一半
		\return  关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFlyCircleAnimator(
			const Vector3& center = Vector3(0.f, 0.f, 0.f),
			Real radius = 100.f, Real speed = 0.001f,
			const Vector3& direction = Vector3(0.f, 1.f, 0.f),
			Real startPosition = 0.f,
			Real radiusEllipsoid = 0.f) = 0;
		


		//! 创建一个直线飞行的animator， 它使一个关联的场景节点沿一条直线的两点间飞行移动
		/** \param startPoint: 这条直线的开始点
		\param endPoint: 这条直线的结束点
		\param timeForWay:从起点飞行到终点需要的毫秒数
		\param loop: 是否循环，如果为true，当飞行到结束点时，将重新从开始点开始
		\param pingpong 标准设置这个animator是否从结束点返回起始点
		\return 这个animator. 关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const Vector3& startPoint,
			const Vector3& endPoint, UINT32 timeForWay, bool loop = false, bool pingpong = false) = 0;



		//! 创建一个纹理animator， 它通过一个纹理列表切换目标场景节点的纹理
		/** \param textures: 要使用的纹理的列表
		\param timePerFrame: 每帧的毫秒数， 列表里的任意纹理能被显示的时间
		\param loop: 如果为false，动画停止后，最后一个纹理会被设置。如果为true，动画会重新从第一个纹理开始。
		\return 这个animator. 关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createTextureAnimator(const vector<ITexture*>& textures,
			SINT32 timePerFrame, bool loop = true) = 0;


		//! 创建一个场景节点animator，它会一定时间后自动删除这个场景节点
		/** \param timeMs: 毫秒数，到时间后节点会被删除
		\return 这个animator. 关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop()  */
		virtual ISceneNodeAnimator* createDeleteAnimator(UINT32 timeMs) = 0;


		//! 创建一个特殊的场景节点的animator，用于自动出来碰撞检测和反应
		/** 见 ISceneNodeAnimatorCollisionResponse 细节
		\param world: 三角形选择器保存世界中所有可能与这个场景节点发生碰撞的三角形。 
		可以通过ISceneManager::createTriangleSelector()创建一个三角形选择器
		\param sceneNode: 被操控的场景节点。之后你添加这个animator到这个场景节点，这个场景节点不会移动穿过墙面
		并且受到重力影响。如果你需呀传输这个场景节点到一个新的位置而不用受到碰撞几何体影响，则调用call sceneNode->setPosition()；
		animator->setTargetNode(sceneNode)
		\param ellipsoidRadius: 碰撞检测和反应的椭圆半径。如果你取得一个场景节点，并且不确定应该设置多大的半径，你应该用下列代码
		检测：
		AxisAlignedBox& box = yourSceneNode->getBoundingBox();
		Vector3 radius = box.MaxEdge - box.getCenter();
		\
		\param gravityPerSecond: 设置环境的重力，作为一个每秒每秒的单位加速度。
		如果你的单位=米，那么Vector3(0,-10.0f,0)接近真实的重力。
		\param ellipsoidTranslation: 默认情况下，这个碰撞检测的椭圆是以这个场景节点的中心为中心，
		它意味着椭圆环绕着它。如果你不想这样， 你可以指定一个对于这个椭圆的调整
		\param slidingValue: DOCUMENTATION NEEDED.
		\return 这个animator,他会导致碰撞检测和反应。关联它到场景节点用ISceneNode::addAnimator()，然后这个animator能够活动它
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop()*/
		virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
			ITriangleSelector* world, ISceneNode* sceneNode,
			const Vector3& ellipsoidRadius = Vector3(30, 60, 30),
			const Vector3& gravityPerSecond = Vector3(0, -10.0f, 0),
			const Vector3& ellipsoidTranslation = Vector3(0, 0, 0),
			Real slidingValue = 0.0005f) = 0;



		//! 创建一个样条线的animator
		/** 这个animator根据hermit样条线改变关联节点的位置。它用Hermite样条线的一个子集：要么基样条(tightness != 0.5)要么catmull rom(tightness == 0.5)样条
		这个animator在1/speed秒内从一个控制点移动到下一个。
		如果不需要长期用animator，可以调用ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFollowSplineAnimator(SINT32 startTime,
			const vector< Vector3 >& points,
			Real speed = 1.0f, Real tightness = 0.5f, bool loop = true, bool pingpong = false) = 0;


		//! 创建一个简单ITriangleSelector, 基于一个网格
		/** 三角形选择器
		可以被用做碰撞剪裁。不要用这个选择器对海量三角形类似Quake3 map之类使用，而是用ISceneManager::createOctreeTriangleSelector()
		请注意：创建三角形选择器不是自动关联到这个场景节点。你需要通过调用ISceneNode::setTriangleSelector()来关联。如下
		ITriangleSelector* s = sceneManager->createTriangleSelector(yourMesh,
		yourSceneNode);
		yourSceneNode->setTriangleSelector(s);
		s->drop();
		\param mesh: 这个三角形的网格
		\param node: 用于可见的和变换的场景节点
		. */
		virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node) = 0;

		//! 创建一个简单的ITriangleSelector, 基于一个可活动（animated）的场景节点
		/** 与这个节点关联的网格的细节将由内部提取
		调用ITriangleSelector::update() 让这个三角形选择器去更新这个动画网格场景节点的当前帧 
		\param node 构建这个选择器的动画场景节点
		*/
		virtual ITriangleSelector* createTriangleSelector(IAnimatedMeshSceneNode* node) = 0;

		//! 创建一个基于一个AABB的简单动态ITriangleSelector
		/** 三角形选择器可以用在碰撞检测。每当三角形被查询到，这个三角形选择器获取这个节点的碰撞盒子，一个创建新的三角形。
		这个方法适合动画场景节点。
		\param node: 用于可见的和变换的场景节点
		\return 这个选择器
		 */
		virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(ISceneNode* node) = 0;


		//!  创建通过八叉树优化的一个三角形选择器
		/** Triangle selectors
		可以被用做碰撞剪裁，这个三角形选择器用于海量三角形， 它由八叉树进行组织。
		请注意：创建三角形选择器不是自动关联到这个场景节点。你需要通过调用ISceneNode::setTriangleSelector()来关联。如下
		 
		ITriangleSelector* s = sceneManager->createOctreeTriangleSelector(yourMesh,
		yourSceneNode);
		yourSceneNode->setTriangleSelector(s);
		s->drop();
		更多信息例子见SDK中的碰撞教程
		\param mesh: 这个三角形的网格
		\param node:用于可见的和变换的场景节点
		\param minimalPolysPerNode: 指定一个八叉树节点包含最小的多边形数。
		\return 这个选择器 */
		virtual ITriangleSelector* createOctreeTriangleSelector(IMesh* mesh,
			ISceneNode* node, SINT32 minimalPolysPerNode = 32) = 0;


		//! 创建一个元信息三角形的选择器
		/** 一个元信息三角形选择器只不过是为一个或多个三角形选择器的集合提供一个公共的三角形选择器的接口。
		在这个方式，碰撞测试可以一次处理不同的的triangle soups。
		\return 这个选择器
	  */
		virtual IMetaTriangleSelector* createMetaTriangleSelector() = 0;


		//! 创建一个可以从地形场景节点选择三角形的三角形选择器
		/** \param node: 创建的地形场景节点的指针
		\param LOD: LOD，0是最高细节
		\return 这个选择器 */
		virtual ITriangleSelector* createTerrainTriangleSelector(
			ITerrainSceneNode* node, SINT32 LOD = 0) = 0;


		//! 添加一个对于新的文件格式的额外的网格加载器
		/** 如果你想引擎扩展不能被支持的文件格式。需要在你的加载类中实现IMeshLoader接口并且通过这个方法添加它。
		使用这个方法，他可能已经用更新后的不用引擎重新编译的版本重载了内建的网格加载器。
		\param externalLoader: 一个新的网格加载器的实现 */
		virtual void addExternalMeshLoader(IMeshLoader* externalLoader) = 0;

		//! 返回此时支持的网格加载器的数量
		virtual UINT32 getMeshLoaderCount() const = 0;

		//! 返回给定的网格加载器
		/** \param index 返回加载器的索引。这个参数是一个基于0数组的索引
		\return 一个指定加载器的指针，如果索引不正确返回0 */
		virtual IMeshLoader* getMeshLoader(UINT32 index) const = 0;


		//! 获取场景碰撞管理器指针
		/** \return 碰撞管理器指针
		*/
		virtual ISceneCollisionManager* getSceneCollisionManager() = 0;

		//! 获取网格操作器的指针
		/** \return 网格操作器的指针
		*/
		virtual IMeshManipulator* getMeshManipulator() = 0;

		//! 添加一个场景节点到删除队列
		/** 
		当这个场景节点到了是安全的时候立即删除。 它意味着当这个场景节点不能执行animator和类似的。
		这个方法用于通过它们的场景节点animator删除场景节点。在大多数情况下，调用 ISceneNode::remove()足够了。
		用这个删除队列是不必要的。见 ISceneManager::createDeleteAnimator()细节
		\param node: 要删除的节点. */
		virtual void addToDeletionQueue(ISceneNode* node) = 0;


		//! 发送一个输入事件到环境
		/** 通常你不能使用此方法，它由内部引擎调用 */
		virtual bool postEventFromUser(const SEvent& event) = 0;


		//! 清理整个场景
		/** 所有的节点都会被移除*/
		virtual void clear() = 0;


		//! 获取这个场景的参数接口
		/** 字符串参数可用由插件和网格加载器使用。例如：CMS 和 LMTS 加载器需要一个名字叫'CSM_TexturePath'
		和 'LMTS_TexturePath' 的参数来设置关联查找纹理的路径。见See
		CSM_TEXTURE_PATH, LMTS_TEXTURE_PATH, MY3D_TEXTURE_PATH,
		COLLADA_CREATE_SCENE_INSTANCES, DMF_TEXTURE_PATH 和 DMF_USE_MATERIALS_DIRS
		*/
		virtual  IAttributes* getParameters() = 0;

		//! 获取当前的渲染通道
		/** 所有的场景节点都在特定的顺序下被渲染。首先是光源，相机，天空盒子， 实体几何体，和透明物体。
		在渲染过程中，场景节点可能要知道当前正在渲染的场景渲染管理器，因为例如：他们在渲染时会注册两次， 一次对于透明几何体和一次是固体。
		当时要知道是哪一个当前渲染通道是激活他们能够渲染它们的几何体正确的那一部分
		 */
		virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const = 0;

		//! 获取默认的场景节点工厂，它可以创建所有内置的场景节点
		/** \return 返回默认的场景节点工厂指针*/
		virtual ISceneNodeFactory* getDefaultSceneNodeFactory() = 0;


		//! 添加一个场景节点工厂到这个场景管理器
		/** 用以通过新的场景节点类型来扩展场景管理，它应该自动创建， 例如当从xml文件加载数据 */
		virtual void registerSceneNodeFactory(ISceneNodeFactory* factoryToAdd) = 0;

		//! 获取注册的场景节点工厂的数量
		virtual UINT32 getRegisteredSceneNodeFactoryCount() const = 0;


		//! 通过索引获取一个场景节点工厂
		/** \return 返回一个请求场景节点的指针 */
		virtual ISceneNodeFactory* getSceneNodeFactory(UINT32 index) = 0;



		//! 获取默认场景节点animator工厂，它用来创建所有内置场景节点的animator
		/** \return 返回获取默认场景节点animator工厂的指针 */
		virtual ISceneNodeAnimatorFactory* getDefaultSceneNodeAnimatorFactory() = 0;


		//! 获取场景节点animator工厂的数量
		virtual UINT32 getRegisteredSceneNodeAnimatorFactoryCount() const = 0;


		//! 通过索引获取场景节点的animator工厂
		/** \return 返回请求场景节点的animator工厂的指针*/
		virtual ISceneNodeAnimatorFactory* getSceneNodeAnimatorFactory(UINT32 index) = 0;


		//! 获取场景节点类型的类型名， 如果为null，则没找到
		virtual const c8* getSceneNodeTypeName(ESCENE_NODE_TYPE type) = 0;

		//! 获取场景节点animator类型的类型名， 如果为null，则没找到
		virtual const c8* getAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) = 0;


		//! 通过名字来添加一个场景节点
		/** \return 通过一个工厂指向添加的场景节点的指针 */
		virtual ISceneNode* addSceneNode(const char* sceneNodeTypeName, ISceneNode* parent = 0) = 0;

		//! 创建一个基于类型名的场景节点Animator 
		/** \param typeName: 要添加的场景节点的Animator的类型名
		\param target: 这个Animator的目标场景节点
		\return 返回这个新Animator的场景节点的指针 */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target = 0) = 0;
		


		//! 创建一个新场景管理器
		/** 它能够更容易的进行绘制并且在同时保存两个独立场景。
		网格缓冲区会由所有的存在的场景管理器共享，这意味着如果你在原场景管理器用比如getMesh()来加载网格。
		这个网格将其它所有的场景管理器中同样有效，而不用加载。
		这个源/主场景管理器将仍然由SapphireDevice::getSceneManager()访问。如果你需要在这个新场景管理器中输入事件，例如：FPS摄像机，你需要手动输入前进；
		就通过实行IEventReceiver来进行调用
		yourNewSceneManager->postEventFromUser()并且返回true，以便这个源场景管理器不能取得事件。 否则，所有输入将自动传到主场景管理器.
	    */
		virtual ISceneManager* createNewSceneManager(bool cloneContent = false) = 0;

		//! 保存当前场景到一个文件
		/** 用选项isDebugObject设置为true是无法保存场景节点的
		。这个场景通常写入一个.irr文件， 一个基于XML格式文件，可以被Irrlicht引擎编辑器编辑。
		要加载这个文件，见ISceneManager::loadScene()。
		\param filename 文件名
		\param userDataSerializer 如果你想要保存一些每个场景节点的用户数据到文件。需要实现ISceneUserDataSerializer接口
		并在这儿将它作为参数。否则，简单指定0作为这个参数、
		\param node 场景的顶部节点，这个节点和所有它后代都保存到场景文件中，传递0或这个管理器保存所有场景
		\return 如果成功返回true. */
		virtual bool saveScene(const  path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;


		//! 保存当前场景到一个文件
		/** 用选项isDebugObject设置为true是无法保存场景节点的
		。这个场景通常写入一个.irr文件， 一个基于XML格式文件，可以被Irrlicht引擎编辑器编辑。
		要加载这个文件，见ISceneManager::loadScene()。
		\param filename 写入文件指针
		\param userDataSerializer 如果你想要保存一些每个场景节点的用户数据到文件。需要实现ISceneUserDataSerializer接口
		并在这儿将它作为参数。否则，简单指定0作为这个参数、
		\param node 场景的顶部节点，这个节点和所有它后代都保存到场景文件中，传递0或这个管理器保存所有场景
		\return 如果成功返回true. */
		virtual bool saveScene(IWriteFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;

		//! 保存当前场景到一个文件
		/** 用选项isDebugObject设置为true是无法保存场景节点的
		。这个场景通常写入一个.irr文件， 一个基于XML格式文件，可以被Irrlicht引擎编辑器编辑。
		要加载这个文件，见ISceneManager::loadScene()。
		\param writer 要保存的场景的XMLWriter
		\param currentPath 当前路径
		\param userDataSerializer 如果你想要保存一些每个场景节点的用户数据到文件。需要实现ISceneUserDataSerializer接口
		并在这儿将它作为参数。否则，简单指定0作为这个参数、
		\param node 场景的顶部节点，这个节点和所有它后代都保存到场景文件中，传递0或这个管理器保存所有场景
		\return 如果成功返回true.*/
		virtual bool saveScene(IXMLWriter* writer, const path& currentPath, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;


		//! 加载一个场景。注意：当前存在的场景并不会被清除、
		/** 这个场景通常由.irr文件读取， 一个基于XML格式文件，可以被Irrlicht引擎编辑器编辑。
		但是可通过ISceneManager::addExternalSceneLoader给引擎添加别的场景类型。 irr文件可以被Irricht编辑器编辑
		\param filename 文件名
		\param userDataSerializer 如果你想要从文件中加载的相关节点的用户数据。需要实现ISceneUserDataSerializer接口
		并在这儿将它作为参数。否则，简单指定0作为这个参数
		\param rootNode 场景的根节点，传递0直接添加场景到场景管理器 
		\return 如果成功返回true.*/
		virtual bool loadScene(const path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0) = 0;

		//! 加载一个场景。注意：当前存在的场景并不会被清除、
		/** 这个场景通常由.irr文件读取， 一个基于XML格式文件，可以被Irrlicht引擎编辑器编辑。
		但是可通过ISceneManager::addExternalSceneLoader给引擎添加别的场景类型。 irr文件可以被Irricht编辑器编辑
		\param file 文件
		\param userDataSerializer 如果你想要从文件中加载的相关节点的用户数据。需要实现ISceneUserDataSerializer接口
		并在这儿将它作为参数。否则，简单指定0作为这个参数
		\param rootNode 场景的根节点，传递0直接添加场景到场景管理器
		\return 如果成功返回true.*/
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0) = 0;


		//! 获取一个可用的网格写入器的实现
		virtual IMeshWriter* createMeshWriter(EMESH_WRITER_TYPE type) = 0;

		//! 获取一个蒙皮网格，*/
		virtual ISkinnedMesh* createSkinnedMesh() = 0;


		//! 设置环境光颜色
		virtual void setAmbientLight(const  ColourValue &ambientColor) = 0;

		//! 取得环境光颜色
		virtual const ColourValue& getAmbientLight() const = 0;

		//! 注册一个自定义的回调管理器，它在场景渲染是取得回调
		/** \param[in] lightManager: 这个新回调管理器.你可以传递0，移除当前的回调管理器解恢复默认的行为*/
		virtual void setLightManager(ILightManager* lightManager) = 0;

		//! 获取一个几何体生成器的一个实例
		/** 几何体生成器提供一些辅助方法来创建不同类型的基本几何体。这对自定义场景非常有用 */
		virtual const IGeometryCreator* getGeometryCreator(void) const = 0;

		//! 检测当前阶段是否被视锥体剔除掉
		/** 请注意：依赖这个剔除方法是相当粗糙的，或者慢。
		如果这个方法返回true，则节点不可见，如果这个节点返回false，这个节点可能仍然不可见
		\param node 要剔除测试的场景节点
		\return True 如果为true则节点不可见*/
		virtual bool isCulled(const ISceneNode* node) const = 0;

	};



}



#endif
