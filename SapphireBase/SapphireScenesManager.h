#ifndef _SAPPHIRE_SCENE_
#define _SAPPHIRE_SCENE_
#include "SapphirePrerequisites.h"
#include "SapphireMemoryAllocatorConfig.h"
#include "SapphireColorValue.h"
#include "SapphireVector3.h"
#include "SapphirePath.h"
#include "SapphireETerrianElements.h"
#include "SapphireKeyMap.h"
#include "SapphireDimension2D.h"

namespace Sapphire
{
	//  对于渲染的通道枚举
	/** 一个参数传递到场景管理器的registerNodeForRendering()方法来在节点中相对于其它节点，你打算要渲染的节点
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



	/***
	Sapphire 的场景管理器
	*/
	class _SapphireExport IScenesScenesManager: public SceneAlloc
	{

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
		virtual ISceneNode* addSkyBoxSceneNode( ITexture* top, ITexture* bottom,
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
		virtual ISceneNode* addSkyDomeSceneNode( ITexture* texture,
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

	};

	

}


#endif