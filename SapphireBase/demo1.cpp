
#include "Sapphire.h"


using namespace Sapphire;


#ifdef _SAPPHIRE_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main()
{
	
	order_map<S3DVertex, int> map;
	S3DVertex s = S3DVertex(Vector3(23, 12, 14), Vector3(231, 33, 22), ColourValue::getColourValue(255,255,255,255), Vector2(12, 34));
	map.insert(s, 1);
	order_map<S3DVertex, int>::Node* nofe = map.find(s);
	int i = nofe->getValue();
	

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
		createDevice(EDT_OPENGL, dimension2d<Sapphire::UINT32>(1920, 1080), 32,
		false, false, false, 0);

	if (!device)
		return 1;
	
	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"HELLO WORLD!");

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
	guienv->addStaticText(L"Hello World!",
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
	//IAnimatedMesh* mesh = smgr->getMesh("media/q2mdl-wham/tris.md2");
	//IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	IAnimatedMesh* mesh = smgr->getMesh("media/kokoro/kokoro.obj");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	//node->setDebugDataVisible(EDS_BBOX );
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
	//	node->setMD2Animation(EMAT_STAND);
		//node->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));
		
		//node->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
	    // node->setMaterialTexture(0, driver->getTexture("media/q2mdl-wham/tundra.bmp"));
		//node->setMaterialTexture(0, driver->getTexture("media/kokoro/kok_face_d.tga"));
		//node->setMaterialTexture(0, driver->getTexture("media/kokoro/kok_acc1_d.tga"));
		//node->setMaterialTexture(0, driver->getTexture("media/kokoro/kok_body_d.tga"));
		//node->setMaterialTexture(0, driver->getTexture("media/kokoro/kok_body_d.tga"));
		//node->setMaterialTexture(0, driver->getTexture("media/kokoro/kok_hairback_s.tga"));
		node->setPosition(Vector3(1, 29, -39));
	    node->setRotation(Vector3(Math::DegreesToRadians(45), 0, 0));
		node->setRotation(Vector3(0, Math::DegreesToRadians(180), 0));
		
	}

	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0), which is
	approximately the place where our md2 model is.
	*/
	//smgr->addCameraSceneNode(0, Vector3(0, 30, -40), Vector3(0, 5, 0));
	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 50.0f, 0.01f);
	camera->setTarget(Vector3(5, 10, 0));
	camera->setPosition(Vector3(0, 30, -40));

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