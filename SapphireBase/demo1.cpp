
#include "Sapphire.h"

namespace Sapphire
{


	class VssT
	{
	public:
		//! Ĭ�ϵĹ��캯���� �������壬��ɫ��������
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
			// ���ÿһ����ʵ��������
			for (UINT32 i = 0; i < SAPPHIRE_MAX_TEXTURE_LAYERS; ++i)
			{
				//��ɾ��������������������
				//TextureLayer[i].emptyTextureMatrix();
			}
			*this = other;
		}



		//! �����ʵ����������
		SMaterialLayer TextureLayer[SAPPHIRE_MAX_TEXTURE_LAYERS];

		//! �������ͣ�ָ�����ж�����ν��л��
		E_MATERIAL_TYPE MaterialType;

		//! ������ʷ���Ļ�������
		/** Ĭ��ȫ�ף���ζ�Ŷ�������ȫ�����ġ�������뿴�������߹�Ч�����������*/
		ColourValue AmbientColor;

		//! ������ʷ�������������
		/** Ĭ��ȫ��. */
		ColourValue DiffuseColor;

		//! ���ʷ���⣬Ĭ��û�з����
		ColourValue EmissiveColor;

		//! ������ʷ���ľ��淴�����
		/** ������ʷ����ɫ����⡣��SMaterial::Shininess��ο������淴���Դ
		*/
		ColourValue SpecularColor;

		//! ����߹�Ӱ��Ĵ�Сֵ
		/** ֵһ��Ϊ20�������Ϊ0��û�и߹⡣
		Ҫ���ֻ������һ�����ʵ�shininess��ֵ����Χ[0.5,128]

		sceneNode->getMaterial(0).Shininess = 20.0f;

		�������������룬�ı�߹���ɫ
		sceneNode->getMaterial(0).SpecularColor.set(255,255,255,255);


		�����̬��Դ�ĸ߹���ɫ(SLight::SpecularColor)��ܴ�̶�Ӱ��߹���ɫ��
		���ǵ�������Դ�����ڵ�ʱ��Ĭ��������������ǵ�һ����Чֵ��

		// ���غ���ʾ����
		scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
		smgr->getMesh("data/faerie.md2"));
		node->setMaterialTexture(0, driver->getTexture("data/Faerie2.pcx")); // ��������������
		node->setMaterialFlag(video::EMF_LIGHTING, true); // �򿪶�̬����
		node->getMaterial(0).Shininess = 20.0f; // ���ø߹��С

		// ��Ӱ�ɫ��Դ
		scene::ILightSceneNode* light = smgr->addLightSceneNode(0,
		core::vector3df(5,5,5), video::SColorf(1.0f, 1.0f, 1.0f));*/
		FLOAT32 Shininess;

		//! ���ɲ����������ڲ�������
		/** ���������Ч��Mostly ignored, ����EMT_PARALLAX_MAP_SOLID  EMT_TRANSPARENT_ALPHA_CHANNEL. */
		FLOAT32 MaterialTypeParam;

		//! �ڶ������ɲ�����������������
		/** �������. */
		FLOAT32 MaterialTypeParam2;

		//! ��3άԪ����һ�����һ���ߵĺ��
		FLOAT32 Thickness;

		//! �Ƿ���Z��������Ĭ��ECFN_LESSEQUAL
		/** ֵ������ E_COMPARISON_FUNC. */
		UINT8 ZBuffer;

		//! ���ÿ����ģʽ
		/** ֵѡ����E_ANTI_ALIASING_MODE. Ĭ����
		EAAM_SIMPLE|EAAM_LINE_SMOOTH, i.e. ��msaa���߶�ƽ������*/
		UINT8 AntiAliasing;

		//! �������ɫƽ��
		/** ֵ������ΪE_COLOR_PLANEö�����Ϳ���'|'����
		ֻ�ڴ���ɫƽ����Ⱦ����ǰ��ȾĿ�ꡣͨ�����ڵ���Ⱦ����Ȼ�ģ�建����ʱ�ر�������ɫ
		�����������Ⱦ�ú����
		*/
		UINT8 ColorMask : 4;

		//! �����ڹ��շ��̵Ķ����ֵ��ɫ
		/** ֵѡ����E_COLOR_MATERIAL.
		�����տ�����������ɫ�ò���ֵ�͹��յ����������ʹ�ı�ÿ���������������Ϊ�ܼ򵥡�
		Ĭ��ECM_DIFFUSE */
		UINT8 ColorMaterial : 3;

		//! �����ϲ�������ת
		/** ֵѡ��������E_BLEND_OPERATION. ʹ�ô�ֵ��ʵ�ʷ����в�ȷ�������������ں��ԡ� */
		E_BLEND_OPERATION BlendOperation : 4;

		//! ָ�������ƫ�ƶ�Զ������
		/** ָ��0�رն����ƫ�ƣ���������ָ���Ƿֿ��ģ�������Ӵ�0��7
		The factor can be from 0 to 7.*/
		UINT8 PolygonOffsetFactor : 3;

		//! ��������ƫ�Ʒ���ı�־
		/** ����ǰ���ָ��ֵ������E_POLYGON_OFFSET. */
		E_POLYGON_OFFSET PolygonOffsetDirection : 1;

		//! ����һ���߿�����������Σ�Ĭ��false
		/** �û����Է���һ�����ʱ�־
		material.Wireframe=true
		��
		material.setFlag(EMF_WIREFRAME, true);
		*/
		bool Wireframe : 1;

		//! ���Ƶ��ƻ�����������  Ĭ��false
		bool PointCloud : 1;

		//! Flat��Gouraud �������ɫ? Ĭ��: true
		bool GouraudShading : 1;

		//! �����Ƿ���ܹ��ռ��� Ĭ��: true
		bool Lighting : 1;

		//! z�����Ƿ��д����ֻ�� Ĭ��true.
		/** �����������MaterialType��һ��͸�����ͻ��߳�������û����ALLOW_ZWRITE_ON_TRANSPARENT
		*/
		bool ZWriteEnable : 1;

		//! �Ƿ����޳��� Ĭ�� true
		bool BackfaceCulling : 1;

		//! �Ƿ�ǰ���޳�  Ĭ�� false
		bool FrontfaceCulling : 1;

		//! �Ƿ�����  Ĭ�� false
		bool FogEnable : 1;

		//! �Ƿ��׼������ 
		/** Ĭ��false */
		bool NormalizeNormals : 1;

		//! ���mipmap���ã��Ƿ�ʹ��mipmap
		/** ��ʱ,�ر�mipmap  Ĭ��true*/
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