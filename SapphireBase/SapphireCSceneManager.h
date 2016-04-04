#ifndef _SAPPHIRE_C_SCENE_MANAGER_
#define _SAPPHIRE_C_SCENE_MANAGER_

#include "SapphireIScenesManager.h"
#include "SapphireISceneNode.h"
#include "SapphireICursorControl.h"
#include "SapphireIMeshLoader.h"
#include "SapphireCAtrributes.h"
#include "SapphireILightManager.h"
#include "SapphireIQ3Shader.h"
#include "SapphireISceneLoader.h"


namespace Sapphire
{
	class IXMLWriter;
	class IFileSystem;
	class IMeshCache; 
	class IMeshBuffer;
	class IGeometryCreator;

	/*!
	The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
    Scene Manager 管理场景节点，网格资源，相机和所有其它东西
	*/
	class CSceneManager : public ISceneManager, public ISceneNode
	{
	public:

		//! constructor
		CSceneManager(IVideoDriver* driver, IFileSystem* fs,
			ICursorControl* cursorControl, IMeshCache* cache = 0,
			IGUIEnvironment *guiEnvironment = 0);

		//! destructor
		virtual ~CSceneManager();

		//! gets an animateable mesh. loads it if needed. returned pointer must not be dropped.
		virtual IAnimatedMesh* getMesh(const String& filename);

		//! gets an animateable mesh. loads it if needed. returned pointer must not be dropped.
		virtual IAnimatedMesh* getMesh(IReadFile* file);

		//! Returns an interface to the mesh cache which is shared beween all existing scene managers.
		virtual IMeshCache* getMeshCache();

		//! returns the video driver
		virtual IVideoDriver* getVideoDriver();

		//! return the gui environment
		virtual IGUIEnvironment* getGUIEnvironment();

		//! return the filesystem
		virtual IFileSystem* getFileSystem();

		//! adds Volume Lighting Scene Node.
		//! the returned pointer must not be dropped.
		virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent = 0, SINT32 id = -1,
			const UINT32 subdivU = 32, const UINT32 subdivV = 32,
			const ColourValue foot = ColourValue::getColourValue(51, 0, 230, 180),
			const ColourValue tail = ColourValue(0, 0, 0, 0),
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! adds a cube scene node to the scene. It is a simple cube of (1,1,1) size.
		//! the returned pointer must not be dropped.
		virtual IMeshSceneNode* addCubeSceneNode(FLOAT32 size = 10.0f, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0), const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! Adds a sphere scene node to the scene.
		virtual IMeshSceneNode* addSphereSceneNode(FLOAT32 radius = 5.0f, SINT32 polyCount = 16, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! adds a scene node for rendering an animated mesh model
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false);

		//! adds a scene node for rendering a static mesh
		//! the returned pointer must not be dropped.
		virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false);

		//! Adds a scene node for rendering a animated water surface mesh.
		virtual ISceneNode* addWaterSurfaceSceneNode(IMesh* mesh, FLOAT32 waveHeight, FLOAT32 waveSpeed, FLOAT32 wlenght, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! registers a node for rendering it at a specific time.
		virtual UINT32 registerNodeForRendering(ISceneNode* node, E_SCENE_NODE_RENDER_PASS pass = ESNRP_AUTOMATIC);

		//! draws all scene nodes
		virtual void drawAll();

		//! Adds a scene node for rendering using a octree to the scene graph. This a good method for rendering
		//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
		//! faster then a bsp tree.
		virtual IMeshSceneNode* addOctreeSceneNode(IAnimatedMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 512, bool alsoAddIfMeshPointerZero = false);

		//! Adss a scene node for rendering using a octree. This a good method for rendering
		//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
		//! faster then a bsp tree.
		virtual IMeshSceneNode* addOctreeSceneNode(IMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 128, bool alsoAddIfMeshPointerZero = false);

		//! Adds a camera scene node to the tree and sets it as active camera.
		// 添加一个相机场景节点到树并且设置它为激活相机
		//! \param position: Position of the space relative to its parent where the camera will be placed.这个相机相对于父节点的空间位置
		//! \param lookat: Position where the camera will look at. Also known as target.这个朝向的位置。
		//! \param parent: Parent scene node of the camera. Can be null. If the parent moves, 这个相机节点的父节点，可以为NULL
		//! the camera will move too.
		//! \return Pointer to interface to camera  指向相机的接口
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& lookat = Vector3(0, 0, 100),
			SINT32 id = -1, bool makeActive = true);

		//! Adds a camera scene node which is able to be controlle with the mouse similar
		//! like in the 3D Software Maya by Alias Wavefront.
		//! The returned pointer must not be dropped.
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			FLOAT32 rotateSpeed = -1500.f, FLOAT32 zoomSpeed = 200.f,
			FLOAT32 translationSpeed = 1500.f, SINT32 id = -1, FLOAT32 distance = 70.f,
			bool makeActive = true);

		//! Adds a camera scene node which is able to be controled with the mouse and keys
		//! like in most first person shooters (FPS):
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			FLOAT32 rotateSpeed = 100.0f, FLOAT32 moveSpeed = .5f, SINT32 id = -1,
			SKeyMap* keyMapArray = 0, SINT32 keyMapSize = 0,
			bool noVerticalMovement = false, FLOAT32 jumpSpeed = 0.f,
			bool invertMouseY = false, bool makeActive = true);

		//! Adds a dynamic light scene node. The light will cast dynamic light on all
		//! other scene nodes in the scene, which have the material flag MTF_LIGHTING
		//! turned on. (This is the default setting in most scene nodes).
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			ColourValue color = ColourValue(1.0f, 1.0f, 1.0f),
			FLOAT32 range = 100.0f, SINT32 id = -1);

		//! Adds a billboard scene node to the scene. A billboard is like a 3d sprite: A 2d element,
		//! which always looks to the camera. It is usually used for things like explosions, fire,
		//! lensflares and things like that.
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const dimension2d<FLOAT32>& size = dimension2d<FLOAT32>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue shadeTop = 0xFFFFFFFF, ColourValue shadeBottom = 0xFFFFFFFF);

		//! Adds a skybox scene node. A skybox is a big cube with 6 textures on it and
		//! is drawn around the camera position.
		virtual ISceneNode* addSkyBoxSceneNode(ITexture* top, ITexture* bottom,
			ITexture* left, ITexture* right, ITexture* front,
			ITexture* back, ISceneNode* parent = 0, SINT32 id = -1);

		//! Adds a skydome scene node. A skydome is a large (half-) sphere with a
		//! panoramic texture on it and is drawn around the camera position.
		virtual ISceneNode* addSkyDomeSceneNode(ITexture* texture,
			UINT32 horiRes = 16, UINT32 vertRes = 8,
			FLOAT32 texturePercentage = 0.9, FLOAT32 spherePercentage = 2.0, FLOAT32 radius = 1000.f,
			ISceneNode* parent = 0, SINT32 id = -1);

		//! Adds a text scene node, which is able to display
		//! 2d text at a position in three dimensional space
		virtual ITextSceneNode* addTextSceneNode(IGUIFont* font, const wchar_t* text,
			ColourValue color = ColourValue::getColourValue(100, 255, 255, 255),
			ISceneNode* parent = 0, Vector3& position = Vector3(0, 0, 0),
			SINT32 id = -1);

		//! Adds a text scene node, which uses billboards
		virtual IBillboardTextSceneNode* addBillboardTextSceneNode(IGUIFont* font, const wchar_t* text,
			ISceneNode* parent = 0,
			const dimension2d<FLOAT32>& size = dimension2d<FLOAT32>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue colorTop = 0xFFFFFFFF, ColourValue colorBottom = 0xFFFFFFFF);

		//! Adds a scene node, which can render a quake3 shader
		virtual IMeshSceneNode* addQuake3SceneNode(const IMeshBuffer* meshBuffer, const Quake3::IShader * shader,
			ISceneNode* parent = 0, SINT32 id = -1
			);


		//! Adds a Hill Plane mesh to the mesh pool. The mesh is
		//! generated on the fly and looks like a plane with some hills
		//! on it. You can specify how many hills should be on the plane
		//! and how high they should be. Also you must specify a name
		//! for the mesh because the mesh is added to the mesh pool and
		//! can be retrieved back using ISceneManager::getMesh with the
		//! name as parameter.
		virtual IAnimatedMesh* addHillPlaneMesh(const path& name,
			const dimension2d<FLOAT32>& tileSize, const dimension2d<UINT32>& tileCount,
			SMaterial* material = 0, FLOAT32 hillHeight = 0.0f,
			const dimension2d<FLOAT32>& countHills = dimension2d<FLOAT32>(1.0f, 1.0f),
			const dimension2d<FLOAT32>& textureRepeatCount = dimension2d<FLOAT32>(1.0f, 1.0f));

		//! Adds a terrain mesh to the mesh pool.
		virtual IAnimatedMesh* addTerrainMesh(const path& meshname, IImage* texture, IImage* heightmap,
			const dimension2d<FLOAT32>& stretchSize = dimension2d<FLOAT32>(10.0f, 10.0f),
			FLOAT32 maxHeight = 200.0f,
			const dimension2d<UINT32>& defaultVertexBlockSize = dimension2d<UINT32>(64, 64));

		//! Add a arrow mesh to the mesh pool
		virtual IAnimatedMesh* addArrowMesh(const path& name,
			ColourValue vtxColor0, ColourValue vtxColor1,
			UINT32 tesselationCylinder, UINT32 tesselationCone,
			FLOAT32 height, FLOAT32 cylinderHeight, FLOAT32 width0,
			FLOAT32 width1);

		//! Adds a static sphere mesh to the mesh pool.
		IAnimatedMesh* addSphereMesh(const path& name,
			FLOAT32 radius = 5.f, UINT32 polyCountX = 16, UINT32 polyCountY = 16);

		//! Adds a static volume light mesh to the mesh pool.
		IAnimatedMesh* addVolumeLightMesh(const path& name,
			const UINT32 SubdivideU = 32, const UINT32 SubdivideV = 32,
			const ColourValue FootColor = ColourValue(51, 0, 230, 180),
			const ColourValue TailColor = ColourValue(0, 0, 0, 0));

		//! Adds a particle system scene node.
		virtual IParticleSystemSceneNode* addParticleSystemSceneNode(
			bool withDefaultEmitter = true, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		//! Adds a terrain scene node to the scene graph.
		virtual ITerrainSceneNode* addTerrainSceneNode(
			const path& heightMapFileName,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			ColourValue vertexColor = ColourValue(255, 255, 255, 255),
			SINT32 maxLOD = 4, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17, SINT32 smoothFactor = 0,
			bool addAlsoIfHeightmapEmpty = false);

		//! Adds a terrain scene node to the scene graph.
		virtual ITerrainSceneNode* addTerrainSceneNode(
			IReadFile* heightMap,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			ColourValue vertexColor = ColourValue(255, 255, 255, 255),
			SINT32 maxLOD = 4, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17, SINT32 smoothFactor = 0,
			bool addAlsoIfHeightmapEmpty = false);

		//! Adds a dummy transformation scene node to the scene graph.
		virtual IDummyTransformationSceneNode* addDummyTransformationSceneNode(
			ISceneNode* parent = 0, SINT32 id = -1);

		//! Adds an empty scene node.
		virtual ISceneNode* addEmptySceneNode(ISceneNode* parent, SINT32 id = -1);

		//! Returns the root scene node. This is the scene node wich is parent
		//! of all scene nodes. The root scene node is a special scene node which
		//! only exists to manage all scene nodes. It is not rendered and cannot
		//! be removed from the scene.
		//! \return Pointer to the root scene node.
		virtual ISceneNode* getRootSceneNode();

		//! Returns the current active camera.
		//! \return The active camera is returned. Note that this can be NULL, if there
		//! was no camera created yet.
		virtual ICameraSceneNode* getActiveCamera() const;

		//! Sets the active camera. The previous active camera will be deactivated.
		//! \param camera: The new camera which should be active.
		virtual void setActiveCamera(ICameraSceneNode* camera);

		//! creates a rotation animator, which rotates the attached scene node around itself.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return The animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		virtual ISceneNodeAnimator* createRotationAnimator(const Vector3& rotationPerSecond);

		//! creates a fly circle animator
		/** Lets the attached scene node fly around a center.
		\param center Center relative to node origin
		\param speed: The orbital speed, in radians per millisecond.
		\param direction: Specifies the upvector used for alignment of the mesh.
		\param startPosition: The position on the circle where the animator will
		begin. Value is in multiples  of a circle, i.e. 0.5 is half way around.
		\return The animator. Attach it to a scene node with ISceneNode::addAnimator()
		*/
		virtual ISceneNodeAnimator* createFlyCircleAnimator(
			const Vector3& center = Vector3(0.f, 0.f, 0.f),
			FLOAT32 radius = 100.f, FLOAT32 speed = 0.001f,
			const Vector3& direction = Vector3(0.f, 1.f, 0.f),
			FLOAT32 startPosition = 0.f,
			FLOAT32 radiusEllipsoid = 0.f);

		//! Creates a fly straight animator, which lets the attached scene node
		//! fly or move along a line between two points.
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const Vector3& startPoint,
			const Vector3& endPoint, UINT32 timeForWay, bool loop = false, bool pingpong = false);

		//! Creates a texture animator, which switches the textures of the target scene
		//! node based on a list of textures.
		virtual ISceneNodeAnimator* createTextureAnimator(const vector<ITexture*>::type& textures,
			SINT32 timePerFrame, bool loop);

		//! Creates a scene node animator, which deletes the scene node after
		//! some time automaticly.
		virtual ISceneNodeAnimator* createDeleteAnimator(UINT32 timeMS);


		//! Creates a special scene node animator for doing automatic collision detection
		//! and response.
		virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
			ITriangleSelector* world, ISceneNode* sceneNode,
			const Vector3& ellipsoidRadius = Vector3(30, 60, 30),
			const Vector3& gravityPerSecond = Vector3(0, -1.0f, 0),
			const Vector3& ellipsoidTranslation = Vector3(0, 0, 0),
			FLOAT32 slidingValue = 0.0005f);

		//! Creates a follow spline animator.
		virtual ISceneNodeAnimator* createFollowSplineAnimator(SINT32 startTime,
			const vector< Vector3 >::type& points,
			FLOAT32 speed, FLOAT32 tightness, bool loop, bool pingpong);


		//! Creates a simple ITriangleSelector, based on a mesh.
		virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node);

		//! Creates a simple ITriangleSelector, based on an animated mesh scene node.
		//! Details of the mesh associated with the node will be extracted internally.
		//! Call ITriangleSelector::update() to have the triangle selector updated based
		//! on the current frame of the animated mesh scene node.
		//! \param: The animated mesh scene node from which to build the selector
		virtual ITriangleSelector* createTriangleSelector(IAnimatedMeshSceneNode* node);

		//! Creates a simple ITriangleSelector, based on a mesh.
		virtual ITriangleSelector* createOctreeTriangleSelector(IMesh* mesh,
			ISceneNode* node, SINT32 minimalPolysPerNode);

		//! Creates a simple dynamic ITriangleSelector, based on a axis aligned bounding box.
		virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(
			ISceneNode* node);

		//! Creates a meta triangle selector.
		virtual IMetaTriangleSelector* createMetaTriangleSelector();

		//! Creates a triangle selector which can select triangles from a terrain scene node
		//! \param: Pointer to the created terrain scene node
		//! \param: Level of detail, 0 for highest detail.
		virtual ITriangleSelector* createTerrainTriangleSelector(
			ITerrainSceneNode* node, SINT32 LOD = 0);

		//! Adds an external mesh loader.
		virtual void addExternalMeshLoader(IMeshLoader* externalLoader);

		//! Returns the number of mesh loaders supported by Irrlicht at this time
		virtual UINT32 getMeshLoaderCount() const;

		//! Retrieve the given mesh loader
		virtual IMeshLoader* getMeshLoader(UINT32 index) const;

		//! Adds an external scene loader.
		virtual void addExternalSceneLoader(ISceneLoader* externalLoader);

		//! Returns the number of scene loaders supported by Irrlicht at this time
		virtual UINT32 getSceneLoaderCount() const;

		//! Retrieve the given scene loader
		virtual ISceneLoader* getSceneLoader(UINT32 index) const;

		//! Returns a pointer to the scene collision manager.
		virtual ISceneCollisionManager* getSceneCollisionManager();

		//! Returns a pointer to the mesh manipulator.
		virtual IMeshManipulator* getMeshManipulator();

		//! Sets the color of stencil buffers shadows drawn by the scene manager.
		virtual void setShadowColor(ColourValue color);

		//! Returns the current color of shadows.
		virtual ColourValue getShadowColor() const;

		//! Adds a scene node to the deletion queue.
		virtual void addToDeletionQueue(ISceneNode* node);

		//! Returns the first scene node with the specified id.
		virtual ISceneNode* getSceneNodeFromId(SINT32 id, ISceneNode* start = 0);

		//! Returns the first scene node with the specified name.
		virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start = 0);

		//! Returns the first scene node with the specified type.
		virtual ISceneNode* getSceneNodeFromType(ESCENE_NODE_TYPE type, ISceneNode* start = 0);

		//! returns scene nodes by type.
		virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type, vector<ISceneNode*>::type& outNodes, ISceneNode* start = 0);

		//! Posts an input event to the environment. Usually you do not have to
		//! use this method, it is used by the internal engine.
		virtual bool postEventFromUser(const SEvent& event);

		//! Clears the whole scene. All scene nodes are removed.
		virtual void clear();

		//! Removes all children of this scene node
		virtual void removeAll();

		//! Returns interface to the parameters set in this scene.
		virtual IAttributes* getParameters();

		//! Returns current render pass.
		virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const;

		//! Creates a new scene manager.
		virtual ISceneManager* createNewSceneManager(bool cloneContent);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SCENE_MANAGER; }

		//! Returns the default scene node factory which can create all built in scene nodes
		virtual ISceneNodeFactory* getDefaultSceneNodeFactory();

		//! Adds a scene node factory to the scene manager.
		/** Use this to extend the scene manager with new scene node types which it should be
		able to create automaticly, for example when loading data from xml files. */
		virtual void registerSceneNodeFactory(ISceneNodeFactory* factoryToAdd);

		//! Returns amount of registered scene node factories.
		virtual UINT32 getRegisteredSceneNodeFactoryCount() const;

		//! Returns a scene node factory by index
		virtual ISceneNodeFactory* getSceneNodeFactory(UINT32 index);

		//! Returns a typename from a scene node type or null if not found
		virtual const c8* getSceneNodeTypeName(ESCENE_NODE_TYPE type);

		//! Returns a typename from a scene node animator type or null if not found
		virtual const c8* getAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type);

		//! Adds a scene node to the scene by name
		virtual ISceneNode* addSceneNode(const char* sceneNodeTypeName, ISceneNode* parent = 0);

		//! creates a scene node animator based on its type name
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target = 0);

		//! Returns the default scene node animator factory which can create all built-in scene node animators
		virtual ISceneNodeAnimatorFactory* getDefaultSceneNodeAnimatorFactory();

		//! Adds a scene node animator factory to the scene manager.
		virtual void registerSceneNodeAnimatorFactory(ISceneNodeAnimatorFactory* factoryToAdd);

		//! Returns amount of registered scene node animator factories.
		virtual UINT32 getRegisteredSceneNodeAnimatorFactoryCount() const;

		//! Returns a scene node animator factory by index
		virtual ISceneNodeAnimatorFactory* getSceneNodeAnimatorFactory(UINT32 index);

		//! Saves the current scene into a file.
		virtual bool saveScene(const path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0);

		//! Saves the current scene into a file.
		virtual bool saveScene(IWriteFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0);

		//! Saves the current scene into a file.
		virtual bool saveScene(IXMLWriter* writer, const path& currentPath, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0);

		//! Loads a scene. Note that the current scene is not cleared before.
		virtual bool loadScene(const path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0);

		//! Loads a scene. Note that the current scene is not cleared before.
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0);

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns a mesh writer implementation if available
		virtual IMeshWriter* createMeshWriter(EMESH_WRITER_TYPE type);

		//! Get a skinned mesh, which is not available as header-only code
		virtual ISkinnedMesh* createSkinnedMesh();

		//! Sets ambient color of the scene
		virtual void setAmbientLight(const ColourValue &ambientColor);

		//! Returns ambient color of the scene
		virtual const ColourValue& getAmbientLight() const;

		//! Register a custom callbacks manager which gets callbacks during scene rendering.
		virtual void setLightManager(ILightManager* lightManager);

		//! Get current render time.
		virtual E_SCENE_NODE_RENDER_PASS getCurrentRendertime() const { return CurrentRendertime; }

		//! Set current render time.
		virtual void setCurrentRendertime(E_SCENE_NODE_RENDER_PASS currentRendertime) { CurrentRendertime = currentRendertime; }

		//! Get an instance of a geometry creator.
		virtual const IGeometryCreator* getGeometryCreator(void) const { return GeometryCreator; }

		//! returns if node is culled
		virtual bool isCulled(const ISceneNode* node) const;

	private:

		//! clears the deletion list
		void clearDeletionList();

		//! writes a scene node
		void writeSceneNode(IXMLWriter* writer, ISceneNode* node, ISceneUserDataSerializer* userDataSerializer, const fschar_t* currentPath = 0, bool init = false);

		struct DefaultNodeEntry
		{
			DefaultNodeEntry(ISceneNode* n) :
				Node(n), TextureValue(0)
			{
				if (n->getMaterialCount())
					TextureValue = (n->getMaterial(0).getTexture(0));
			}

			bool operator < (const DefaultNodeEntry& other) const
			{
				return (TextureValue < other.TextureValue);
			}

			ISceneNode* Node;
		private:
			void* TextureValue;
		};

		//! sort on distance (center) to camera
		struct TransparentNodeEntry
		{
			TransparentNodeEntry(ISceneNode* n, const Vector3& camera)
				: Node(n)
			{
				Distance = Node->getAbsoluteTransformation().getTrans().squaredDistance(camera);
			}

			bool operator < (const TransparentNodeEntry& other) const
			{
				return Distance > other.Distance;
			}

			ISceneNode* Node;
		private:
			FLOAT64 Distance;
		};

		//! sort on distance (sphere) to camera
		struct DistanceNodeEntry
		{
			DistanceNodeEntry(ISceneNode* n, const Vector3& cameraPos)
				: Node(n)
			{
				setNodeAndDistanceFromPosition(n, cameraPos);
			}

			bool operator < (const DistanceNodeEntry& other) const
			{
				return Distance < other.Distance;
			}

			void setNodeAndDistanceFromPosition(ISceneNode* n, const Vector3 & fromPosition)
			{
				Node = n;
				Distance = Node->getAbsoluteTransformation().getTrans().squaredDistance(fromPosition);
				Distance -= Node->getBoundingBox().getExtent().squaredLength() * 0.5;
			}

			ISceneNode* Node;
		private:
			FLOAT64 Distance;
		};

		//! video driver
		IVideoDriver* Driver;

		//! file system
		IFileSystem* FileSystem;

		//! GUI Enviroment ( Debug Purpose )
		IGUIEnvironment* GUIEnvironment;

		//! cursor control
		ICursorControl* CursorControl;

		//! collision manager
		ISceneCollisionManager* CollisionManager;

		//! render pass lists
		vector<ISceneNode*>::type CameraList;
		vector<ISceneNode*>::type LightList;
		vector<ISceneNode*>::type ShadowNodeList;
		vector<ISceneNode*>::type SkyBoxList;
		vector<DefaultNodeEntry>::type SolidNodeList;
		vector<TransparentNodeEntry>::type TransparentNodeList;
		vector<TransparentNodeEntry>::type TransparentEffectNodeList;

		vector<IMeshLoader*>::type MeshLoaderList;
		vector<ISceneLoader*>::type SceneLoaderList;
		vector<ISceneNode*>::type DeletionList;
		vector<ISceneNodeFactory*>::type SceneNodeFactoryList;
		vector<ISceneNodeAnimatorFactory*>::type SceneNodeAnimatorFactoryList;

		//! current active camera
		ICameraSceneNode* ActiveCamera;
		Vector3 camWorldPos; // Position of camera for transparent nodes.

		ColourValue ShadowColor;
		ColourValue AmbientLight;

		//! String parameters
		// NODE: Attributes are slow and should only be used for debug-info and not in release
		CAttributes Parameters;

		//! Mesh cache
		IMeshCache* MeshCache;

		E_SCENE_NODE_RENDER_PASS CurrentRendertime;

		//! An optional callbacks manager to allow the user app finer control
		//! over the scene lighting and rendering.
		// 一个可选的回调管理器，它允许用户应用程序更好的控制场景光源和渲染
		ILightManager* LightManager;

		//! constants for reading and writing XML.
		//! Not made static due to portability problems.
		const StringW IRR_XML_FORMAT_SCENE;
		const StringW IRR_XML_FORMAT_NODE;
		const StringW IRR_XML_FORMAT_NODE_ATTR_TYPE;

		IGeometryCreator* GeometryCreator;
	};

}


#endif