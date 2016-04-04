
#include "Sapphire.h"

namespace Sapphire
{


	class VssT
	{
	public:
		//! 默认的构造函数， 创建固体，白色明亮材质
		VssT()
			: MaterialType(EMT_SOLID), AmbientColor(ColourValue::getColourValue(255, 255, 255, 255)), DiffuseColor(ColourValue::getColourValue(255, 255, 255, 255)),
			EmissiveColor(ColourValue::getColourValue(0, 0, 0, 0)), SpecularColor(ColourValue::getColourValue(255, 255, 255, 255)),
			Shininess(0.0f), MaterialTypeParam(0.0f), MaterialTypeParam2(0.0f), Thickness(1.0f),
			ZBuffer(ECFN_LESSEQUAL), AntiAliasing(EAAM_SIMPLE), ColorMask(ECP_ALL),
			ColorMaterial(ECM_DIFFUSE), BlendOperation(EBO_NONE),
			PolygonOffsetFactor(0), PolygonOffsetDirection(EPO_FRONT),
			Wireframe(false), PointCloud(false), GouraudShading(true),
			Lighting(true), ZWriteEnable(true), BackfaceCulling(true), FrontfaceCulling(false),
			FogEnable(false), NormalizeNormals(false), UseMipMaps(true)
		{ }


		VssT(const VssT& other)
		{
			// 清空每一层材质的纹理矩阵
			for (UINT32 i = 0; i < SAPPHIRE_MAX_TEXTURE_LAYERS; ++i)
			{
				//先删除本纹理自身的纹理矩阵
				//TextureLayer[i].emptyTextureMatrix();
			}
			*this = other;
		}



		//! 本材质的纹理层数组
		SMaterialLayer TextureLayer[SAPPHIRE_MAX_TEXTURE_LAYERS];

		//! 材质类型，指定所有东西如何进行混合
		E_MATERIAL_TYPE MaterialType;

		//! 这个材质反射的环境光量
		/** 默认全白，意味着对象是完全照明的。如果你想看漫反射或高光效果，降低这个*/
		ColourValue AmbientColor;

		//! 这个材质反射的漫反射光量
		/** 默认全白. */
		ColourValue DiffuseColor;

		//! 材质发射光，默认没有发射光
		ColourValue EmissiveColor;

		//! 这个材质反射的镜面反射光量
		/** 这个材质反射白色镜面光。看SMaterial::Shininess如何开启镜面反射光源
		*/
		ColourValue SpecularColor;

		//! 镜面高光影响的大小值
		/** 值一般为20，如果设为0，没有高光。
		要激活，只需设置一个材质的shininess的值到范围[0.5,128]

		sceneNode->getMaterial(0).Shininess = 20.0f;

		你可以用下面代码，改变高光颜色
		sceneNode->getMaterial(0).SpecularColor.set(255,255,255,255);


		这个动态光源的高光颜色(SLight::SpecularColor)会很大程度影响高光颜色，
		但是当创建光源场景节点时，默认情况下设置它们到一个有效值。

		// 加载和显示网格
		scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
		smgr->getMesh("data/faerie.md2"));
		node->setMaterialTexture(0, driver->getTexture("data/Faerie2.pcx")); // 设置漫反射纹理
		node->setMaterialFlag(video::EMF_LIGHTING, true); // 打开动态光照
		node->getMaterial(0).Shininess = 20.0f; // 设置高光大小

		// 添加白色光源
		scene::ILightSceneNode* light = smgr->addLightSceneNode(0,
		core::vector3df(5,5,5), video::SColorf(1.0f, 1.0f, 1.0f));*/
		FLOAT32 Shininess;

		//! 自由参数，依赖于材质类型
		/** 多数情况无效，Mostly ignored, 用在EMT_PARALLAX_MAP_SOLID  EMT_TRANSPARENT_ALPHA_CHANNEL. */
		FLOAT32 MaterialTypeParam;

		//! 第二个自由参数，依赖材质类型
		/** 多数情况. */
		FLOAT32 MaterialTypeParam2;

		//! 非3维元素如一个点和一条线的厚度
		FLOAT32 Thickness;

		//! 是否开启Z缓冲区，默认ECFN_LESSEQUAL
		/** 值来自于 E_COMPARISON_FUNC. */
		UINT8 ZBuffer;

		//! 设置抗锯齿模式
		/** 值选择于E_ANTI_ALIASING_MODE. 默认是
		EAAM_SIMPLE|EAAM_LINE_SMOOTH, i.e. 简单msaa和线段平滑开启*/
		UINT8 AntiAliasing;

		//! 定义打开颜色平面
		/** 值定义作为E_COLOR_PLANE枚举类型可以'|'链接
		只在打开颜色平面渲染到当前渲染目标。通常用于当渲染到深度或模板缓冲区时关闭所有颜色
		，或对立体渲染用红和绿
		*/
		UINT8 ColorMask : 4;

		//! 定义在光照方程的顶点插值颜色
		/** 值选择于E_COLOR_MATERIAL.
		当光照开启，顶点颜色用材质值和光照调制替代。这使改变每个面的漫反射光的行为很简单。
		默认ECM_DIFFUSE */
		UINT8 ColorMaterial : 3;

		//! 保存混合操作的旋转
		/** 值选择来自于E_BLEND_OPERATION. 使用此值的实际方法尚不确定，所以它现在忽略。 */
		E_BLEND_OPERATION BlendOperation : 4;

		//! 指定多边形偏移多远的因子
		/** 指定0关闭多边形偏移，这个方向的指定是分开的，这个因子从0到7
		The factor can be from 0 to 7.*/
		UINT8 PolygonOffsetFactor : 3;

		//! 定义多边形偏移方向的标志
		/** 可以前或后，指定值可以用E_POLYGON_OFFSET. */
		E_POLYGON_OFFSET PolygonOffsetDirection : 1;

		//! 绘制一个线框或填充的三角形，默认false
		/** 用户可以访问一个材质标志
		material.Wireframe=true
		或
		material.setFlag(EMF_WIREFRAME, true);
		*/
		bool Wireframe : 1;

		//! 绘制点云或填充的三角形  默认false
		bool PointCloud : 1;

		//! Flat或Gouraud 高络德着色? 默认: true
		bool GouraudShading : 1;

		//! 材质是否接受光照计算 默认: true
		bool Lighting : 1;

		//! z缓冲是否可写或者只读 默认true.
		/** 如果材质类型MaterialType是一个透明类型或者场景参数没设置ALLOW_ZWRITE_ON_TRANSPARENT
		*/
		bool ZWriteEnable : 1;

		//! 是否背面剔除打开 默认 true
		bool BackfaceCulling : 1;

		//! 是否前面剔除  默认 false
		bool FrontfaceCulling : 1;

		//! 是否开启雾化  默认 false
		bool FogEnable : 1;

		//! 是否标准化法线 
		/** 默认false */
		bool NormalizeNormals : 1;

		//! 如果mipmap可用，是否使用mipmap
		/** 有时,关闭mipmap  默认true*/
		bool UseMipMaps : 1;

		
	};

}

using namespace Sapphire;


#ifdef _SAPPHIRE_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main()
{
	int size = sizeof(SMaterial);
	Sapphire::vector<SMaterial>::type v;
	SMaterial m1 = SMaterial();
	SMaterial m2 = SMaterial();
	SMaterial m3 = SMaterial();
	SMaterial m4 = SMaterial();
	SMaterial m5 = SMaterial();
	v.push_back(m1);
	v.push_back(m2);
	v.push_back(m3);
	v.push_back(m4);
	v.push_back(m5);
	String s = "sdklasdfk'";
	
	/*
	The most important function of the engine is the createDevice()
	function. The IrrlichtDevice is created by it, which is the root
	object for doing anything with the engine. createDevice() has 7
	parameters:

	- deviceType: Type of the device. This can currently be the Null-device,
	one of the two software renderers, D3D8, D3D9, or OpenGL. In this
	example we use EDT_SOFTWARE, but to try out, you might want to
	change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
	EDT_DIRECT3D9, or EDT_OPENGL.

	- windowSize: Size of the Window or screen in FullScreenMode to be
	created. In this example we use 640x480.

	- bits: Amount of color bits per pixel. This should be 16 or 32. The
	parameter is often ignored when running in windowed mode.

	- fullscreen: Specifies if we want the device to run in fullscreen mode
	or not.

	- stencilbuffer: Specifies if we want to use the stencil buffer (for
	drawing shadows).

	- vsync: Specifies if we want to have vsync enabled, this is only useful
	in fullscreen mode.

	- eventReceiver: An object to receive events. We do not want to use this
	parameter here, and set it to 0.

	Always check the return value to cope with unsupported drivers,
	dimensions, etc.
	*/
	Device *device =
		createDevice(EDT_OPENGL, dimension2d<UINT32>(1024, 768), 32,
		false, false, false, 0);

	if (!device)
		return 1;
	
	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	/*
	Get a pointer to the VideoDriver, the SceneManager and the graphical
	user interface environment, so that we do not always have to write
	device->getVideoDriver(), device->getSceneManager(), or
	device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<SINT32>(10, 10, 260, 22), true);

	/*
	To show something interesting, we load a Quake 2 model and display it.
	We only have to get the Mesh from the Scene Manager with getMesh() and add
	a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
	check the return value of getMesh() to become aware of loading problems
	and other errors.

	Instead of writing the filename sydney.md2, it would also be possible
	to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
	other supported file format. By the way, that cool Quake 2 model
	called sydney was modelled by Brian Collins.
	*/
	//IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	
	/*
	To let the mesh look a little bit nicer, we change its material. We
	disable lighting because we do not have a dynamic light in here, and
	the mesh would be totally black otherwise. Then we set the frame loop,
	such that the predefined STAND animation is used. And last, we apply a
	texture to the mesh. Without it the mesh would be drawn using only a
	color.
	*/
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(EMAT_STAND);
		//node->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));
		
		node->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
		
	}

	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0), which is
	approximately the place where our md2 model is.
	*/
	smgr->addCameraSceneNode(0, Vector3(0, 30, -40), Vector3(0, 5, 0));

	/*
	Ok, now we have set up the scene, lets draw everything: We run the
	device in a while() loop, until the device does not want to run any
	more. This would be when the user closes the window or presses ALT+F4
	(or whatever keycode closes a window).
	*/
	while (device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, ColourValue::getColourValue(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht 
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	device->drop();

	return 0;
	return 0;
}