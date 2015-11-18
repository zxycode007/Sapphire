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
	//  ������Ⱦ��ͨ��ö��
	/** һ���������ݵ�������������registerNodeForRendering()�������ڽڵ�������������ڵ㣬�����Ҫ��Ⱦ�Ľڵ�
	*/
	enum E_SCENE_NODE_RENDER_PASS
	{
		//!  ��ǰ�����Ⱦͨ��
		ESNRP_NONE = 0,

		//! ���ͨ����һ����۲�㱻���õ������ǵ�һ�����ݵ�
		ESNRP_CAMERA = 1,

		//! �����ͨ������Դ���任������ռ䲢����ӵ�����
		ESNRP_LIGHT = 2,

		//! ����������պ���
		ESNRP_SKY_BOX = 4,

		//! ���е���ͨ�����ܹ�������ע��
		/**
		SceneManager::getSceneNodeRenderPass()���᷵�����ֵ��
		����������ֵ����registerNodeForRendering��ͨ��Ĭ�ϣ�,���������������ͨ�������ǹ��廹��͸�����ұ�ע�ᵽSNRT_TRANSPARENT����SNRT_SOLD����Ϣ���Զ�������
		ע������ֻע������ڵ㵽��Ϊһ�����͡������ĳ����ڵ������������͸���������ͣ�ע������(һ�ι���SNRT_SOLD��һ��͸��SNRT_TRANSPARENT)��
		������render������������getSceneNodeRenderPass�����ҳ���ǰ��Ⱦ��Ⱦͨ��������ڵ�����ϵ���Ⱦ��
		*/
		ESNRP_AUTOMATIC = 24,

		//! û�в��ʵĹ��峡���ڵ�����ر�ĳ����ڵ�
		ESNRP_SOLID = 8,

		//!͸���ĳ����ڵ㣬�ڹ̶��ڵ����ơ���������Ⱦ˳���ϴӰ��Ӻ�ǰ����
		ESNRP_TRANSPARENT = 16,

		//! ����͸��Ч���ڵ㣬������͸���ڵ�֮����������Ⱦ˳���ϴӰ��Ӻ�ǰ����
		ESNRP_TRANSPARENT_EFFECT = 32,

		//! �����ڹ���֮����͸���ڵ�֮ǰ�� ��ʱ������Ӱ�ݻ�
		ESNRP_SHADOW = 64
	};
	//�ļ�����
	class IReadFile;
	class ITextire;

	//��������ģ������
	class IAnimatedMesh;
	//���񻺳���
	class IMeshCache;
	//��ʾ����
	class IVideoDriver;
	//gui����
	class IGUIEnvironment;
	//��ȡ�ļ�ϵͳ
	class IFileSystem;
	//����ⳡ���ڵ�
	class IVolumeLightSceneNode;
	//�����ڵ�
	class ISceneNode;
	//���񳡾��ڵ�
	class IMeshSceneNode;
	//�����������񳡾��ڵ�
	class IAnimatedMeshSceneNode;
	//����
	class IMesh;
	//����������ڵ�
	class ICameraSceneNode;
	//��Դ�����ڵ�
	class ILightSceneNode;
	//BillBoard�ڵ�
	class IBillboardSceneNode;
	//����
	class ITexture;
	//��������ϵͳ�ڵ�
	class IParticleSystemSceneNode;
	//���γ����ڵ�
	class ITerrainSceneNode;



	/***
	Sapphire �ĳ���������
	*/
	class _SapphireExport IScenesScenesManager: public SceneAlloc
	{

		//��ȡһ����������ģ�� �����ָ�룬���û�м��أ��ȴ��ļ����ء�
		//���ʧ�ܣ�����NULL����������ᱻ����
		virtual IAnimatedMesh* getMesh(const String& filename) = 0;

		//! 
		//��ȡһ�����񻺳����Ľӿڣ���Ϊ���������������ṩ������Ϣ
		/** ��������ӿ��������ֶ�����¼��ص��������SceneManager::getMesh()�����ã���ȥ�Ƴ����ǻ���������Ѿ����ص����� */
		virtual IMeshCache* getMeshCache() = 0;

		//! ��ȡ��ʾ����
		/** \����һ����ʾ������ָ��
		���ָ�벻�ܱ�����*/
		virtual  IVideoDriver* getVideoDriver() = 0;

		//! ��ȡ���GUI����
		/** \����һ��GUIEnvironment��ָ��
		 */
		virtual  IGUIEnvironment* getGUIEnvironment() = 0;

		//!��ȡ�ļ�ϵͳ */
		virtual  IFileSystem* getFileSystem() = 0;

		//! ���һ������ⳡ���ڵ�
		/** ����
		scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
		32, 32, //ϸ�� U/V
		video::SColor(0, 180, 180, 180), //foot color
		video::SColor(0, 0, 0, 0) //tail color
		);
		if (n)
		{
		n->setScale(Vector3(46.0f, 45.0f, 46.0f));
		n->getMaterial(0).setTexture(0, smgr->getVideoDriver()->getTexture("lightFalloff.png"));
		}
		\return ����ɹ�����volumeLight��ָ�룬���򷵻�NULL
		*/
		virtual IVolumeLightSceneNode* addVolumeLightSceneNode(ISceneNode* parent = 0, SINT32 id = -1,
			const UINT32 subdivU = 32, const UINT32 subdivV = 32,
			const ColourValue foot = ColourValue(51, 0, 230, 180),
			const ColourValue tail = ColourValue(0, 0, 0, 0),
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! 
		// ���һ����������ӳ����ڵ�
		/** \param size: ���Ӵ�С,ÿ��ά�ȶ���ͬ
		\param parent: �����ڵ�ĸ��׽ڵ㣬���Ϊ0����û�и��ڵ�
		\param id: �ڵ��ID.
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: ��ʼ����ת
		\param scale: ��ʼ������
		\return ָ�򴴽��Ĳ��Խڵ��ָ�� */
		virtual IMeshSceneNode* addCubeSceneNode(Real size = 10.0f, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! ͷһ�켸һ�������뾶��ϸ�ڵ����峡����
		/** \param radius: ����뾶
		\param polyCount: ˮƽ�ʹ�ֱ����Ķ��������ܵĶ��������=polyCount*polyCount
		����һ�����񻺳���������������16λ���ƣ���������������256
		\param parent: �����ڵ�ĸ��ڵ㣬���Ϊ0��û�и��ڵ�
		\param id: �ڵ��ID
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: ��ʼ����ת
		\param scale: ��ʼ������
		\return Pointer ָ�򴴽��Ĳ��Խڵ��ָ�� */
		virtual IMeshSceneNode* addSphereSceneNode(Real radius = 5.0f, SINT32 polyCount = 16,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! ��ӿ�����Ⱦ��һ����������ģ�͵�һ�������ڵ� 
		/** \param mesh: ��ЩҪ��ʾ���Ѽ��صĶ�������ģ��
	    \param parent: �����ڵ�ĸ��ڵ㣬���Ϊ0��û�и��ڵ�
		\param id: �ڵ��ID
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: ��ʼ����ת
		\param scale: ��ʼ������
		\param alsoAddIfMeshPointerZero: ������ݸ���ָ�룬Ҳ��ӳ����ڵ�
		\return ָ�򴴽��Ĳ��Խڵ��ָ��.
		*/
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false) const = 0;


		//! ���������Ⱦ�ľ�̬�����һ�������ڵ㣨���ڱ���������
		/** \param mesh: Ҫ��ʾ�ľ�̬������Ѽ��ص�ָ��
		 \param parent: �����ڵ�ĸ��ڵ㣬���Ϊ0��û�и��ڵ�
		\param id: �ڵ��ID
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: ��ʼ����ת
		\param scale: ��ʼ������
		\param alsoAddIfMeshPointerZero: ������ݸ���ָ�룬Ҳ��ӳ����ڵ�
		\return ָ�򴴽��Ĳ��Խڵ��ָ��.
		*/
		virtual IMeshSceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent = 0, SINT id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f),
			bool alsoAddIfMeshPointerZero = false) = 0;


		//! ���һ��������Ⱦˮ����������ĳ����ڵ�
		/** �����������趨ΪEMT_TRANSPARENT_REFLECTION���������Ƚ���ʵ
		\param waveHeight: ˮ���ĸ߶�
		\param waveSpeed: ˮ�����ٶ�
		\param waveLength: ˮ���ĳ���
		\param mesh: Ҫ��ʾ��ˮ����̬������Ѽ��ص�ָ�� 
		 \param parent: �����ڵ�ĸ��ڵ㣬���Ϊ0��û�и��ڵ�
		\param id: �ڵ��ID
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: ��ʼ����ת
		\param scale: ��ʼ������
		\param alsoAddIfMeshPointerZero: ������ݸ���ָ�룬Ҳ��ӳ����ڵ�
		\return ָ�򴴽��Ĳ��Խڵ��ָ��. */
		virtual ISceneNode* addWaterSurfaceSceneNode(IMesh* mesh,
			Real waveHeight = 2.0f, Real waveSpeed = 300.0f, Real waveLength = 10.0f,
			ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! ���һ�����ڰ˲�������ͼ�ĳ����ڵ㣨������������
		/** ����һ�����ʺ�������Ⱦ���������巽�����˲����ǽ�������������������
		\param mesh: �����������ý���8�����İ������м�������������������������д���1֡�� ��ʹ�õ�һ֡
		\param parent: �����ڵ�ĸ��ڵ�
		\param id: �ڵ��ID
		\param minimalPolysPerNode: ָ��һ���˲����ڵ��������С������������һ���ڵ������������ֵ���������ᱻ�ָ�Ϊ��С�Ľڵ�
		\param alsoAddIfMeshPointerZero: ������ݸ���ָ�룬Ҳ��ӳ����ڵ�
		\return ָ�򴴽��Ĳ��Խڵ��ָ��. */
		virtual IMeshSceneNode* addOctreeSceneNode(IAnimatedMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 512, bool alsoAddIfMeshPointerZero = false) = 0;



		//! ���һ�����ڰ˲�������ͼ�ĳ����ڵ�
		/** ����һ�����ʺ�������Ⱦ���������巽�����˲����ǽ������������������ϱ�BSP����ܶ�
		\param mesh: �����������ý���8�����İ������м����������
		\param parent: �����ڵ�ĸ��ڵ�
		\param id: �ڵ��ID
		\param minimalPolysPerNode:ָ��һ���˲����ڵ��������С������������һ���ڵ������������ֵ���������ᱻ�ָ�Ϊ��С�Ľڵ�
		\param alsoAddIfMeshPointerZero: ������ݸ���ָ�룬Ҳ��ӳ����ڵ�
		\return ָ�򴴽��Ĳ��Խڵ��ָ��. */
		virtual IMeshSceneNode* addOctreeSceneNode(IMesh* mesh, ISceneNode* parent = 0,
			SINT32 id = -1, SINT32 minimalPolysPerNode = 256, bool alsoAddIfMeshPointerZero = false) = 0;



		//! ���һ����������ڵ㵽����ͼ�в�������Ϊ������
		/** ������������û��������з�Ӧ��Ӧ����addCameraSceneNodeFPS()����һ������������ƶ����߻������animator����
		ISceneNode::setPosition(), ICameraSceneNode::setTarget()��Щ������Ĭ�ϣ��������۲�һ��λ��(ʹ��setTarget)�������ĳ�������ת����setRotation()���ã��Ƕ����ġ�
		�������ͨ��setRotation()����������۲�ķ�����ô��������ICameraSceneNode::bindTargetAndRotation(true)
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param lookat: �������Ĺ۲��λ�ã�Ҳ��ΪĿ��
		\param parent: ����ڵ�ĸ��ڵ�. ����Ϊ��. ������ڵ��ƶ������Ҳ���ƶ�
		\param id: ���ID
		\param makeActive ���ø����Ϊ���һ����ȷ��ͬʱֻ����һ��������
		\return ����ɹ�����һ������ӿڵ�ָ��*/
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& lookat = Vector3(0, 0, 100),
			SINT32 id = -1, bool makeActive = true) = 0;


		//! ���һ��maya���͵��û����Ƶ���������ڵ㵽����ͼ
		/** 
		����һ��ʹ��animator�ṩ������MAYA3d����������Ƶ��������Ӧ��animator��setPositon������֧����������
		�������setTarget��ȥ����������Ŀ��������������������setDistanceȥ�������Ŀ��ĵ�ǰ���룬���������������뾶��
		\param parent:����ڵ�ĸ��ڵ�. ����Ϊ��.
		\param rotateSpeed: �������ת�ٶ�
		\param zoomSpeed: ����ķŴ��ٶ�
		\param translationSpeed: ������ƶ��ٶ�
		\param id: ���ID
		\param distance ��ʼ����������֮��ľ���
		\param makeActive ���ø����Ϊ���һ����ȷ��ͬʱֻ����һ��������
		\return ����ɹ�����һ������ӿڵ�ָ�� */
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			Real rotateSpeed = -1500.f, Real zoomSpeed = 200.f,
			Real translationSpeed = 1500.f, Real id = -1, Real distance = 70.f,
			bool makeActive = true) = 0;

		//! 
		//���һ����animator�ṩ�����ͼ��̲�����FPS����ڵ�
		/** 
		���FPS����ƻ��ṩһ�������FPS�����Ϸ�������Ϊ.�����ڼ򵥵�demo��ԭ�ͣ��������ṩһ��������ҵ����Ϸ�������������
		������������ڵ���ת��look-at target��@see ICameraSceneNode::bindTargetAndRotation()�������������������۲죬�������α�������ߡ�
		�������Ҫ�ı�Key���֣�����ȥָ���Լ���keymap������ʹ������Ƶ��α����Ϊwsad

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
		 
		\param parent: ����ڵ�ĸ��ڵ�. ����Ϊ��.
		\param rotateSpeed: �������ת�Ľ��ٶ�, ֻ������
		\param moveSpeed: �����λÿ���ƶ��ٶȣ��ƶ����α������
		\param id: i���ID
		\param keyMapArray: һ��keymap����Ŀ�ѡָ�롣ָ��ʲô������������ƶ������Ϊnull,��ʹ��Ĭ��KeyMap.
		������������һ�ζ�����action���󶨶������ͬһ��action
		\param keyMapSize: keymap��������Ŀ����
		\param noVerticalMovement: �������ѡ��Ϊtrue��ʹ���ֻ��ˮƽƽ���ƶ����رմ�ֱ�ƶ���Ĭ��Ϊfalse��
		'false', 
		\param jumpSpeed: ��������Ծʱ���ƶ��ٶ�
		\param invertMouse: ����Ϊtrue�Ļ�������ƶ�������ƶ�����Ĭ��Ϊfalse
		\param makeActive ���ø����Ϊ���һ����ȷ��ͬʱֻ����һ��������
		\return ����ɹ�����һ������ӿڵ�ָ��. */
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			Real rotateSpeed = 100.0f, Real moveSpeed = 0.5f, SINT32 id = -1,
			SKeyMap* keyMapArray = 0, SINT32 keyMapSize = 0, bool noVerticalMovement = false,
			Real jumpSpeed = 0.f, bool invertMouse = false,
			bool makeActive = true) = 0;




		//! ���һ����̬�Ĺ�Դ�����ڵ㵽����ͼ��
		/** 
		�����Դ��ʩ�Ӷ�̬��Դ�ڳ��������������ĳ����ڵ��ϡ����ղ��ʱ�־��MTF_LIGHTINGk������Ĭ�����õ����󲿷ֳ����ڵ��ϣ�
		\param parent: ���սڵ�ĸ��ڵ㣬���游�ڵ��ƶ�������Ϊ��
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param color: ��Դ����������ɫ����������ɫ���淴�����ɫ����ͨ��ILightSceneNode::getLightData()������
		\param radius: ���շ�Χ
		\param id: �ڵ�ID
		\return ����ɹ�����һ����Դ�ӿڵ�ָ�� */
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const Vector3& position = Vector3(0, 0, 0),
			ColourValue = ColourValue(1.0f, 1.0f, 1.0f),
			Real radius = 100.0f, SINT32 id = -1) = 0;



		//! �ڳ���ͼ�����һ��billboard �����ڵ�
		/** 
		һ��billboard��һ��3d���飺 һ��2d��Ԫ�أ����������������ͨ�����ڱ�ը�����棬���κ����ƵĶ���
		\param parent �ýڵ�ĸ��ڵ㣬���游�ڵ��ƶ�������Ϊ��
		\param size billboard�Ĵ�С�������һ��dimension����������
		\param position ����ڸ��ڵ�Ŀռ�λ��
		\param id �ڵ�ID
		\param colorTop billboard������ɫ�����ֵ��Ĭ��white��
		\param colorBottom billboard������ɫ�����ֵ��Ĭ��white)
		\return ����ɹ�����һ��billboard�ӿڵ�ָ��*/
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const dimension2d<Real>& size = dimension2d<Real>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue colorTop = ColourValue(0xFFFFFFFF), ColourValue colorBottom = ColourValue(0xFFFFFFFF)) = 0;



		//! 
		//�ڳ���ͼ�����һ����պ��ӳ����ڵ�
		/** һ����պ�����һ�����������壬������6�������һ��������λ����Χ
		\param top: ����ƽ������
		\param bottom: �ײ�ƽ������
		\param left: ���ƽ������
		\param right: �ұ�ƽ������
		\param front: ǰ��ƽ������
		\param back: ��ƽ������
		\param parent: ��պ��Ӹ��ڵ㣬һ����պ���ͨ��û�и��ڵ㣬����Ӧ��Ϊ��. ע�⣺����պ�������һ�����ڵ㣬��������ڻ���ʱ����ı�
		\param id: �ڵ�ID
		\return  ����ɹ�����һ���ӿڵ�ָ�� */
		virtual ISceneNode* addSkyBoxSceneNode( ITexture* top, ITexture* bottom,
			ITexture* left, ITexture* right, ITexture* front,
			ITexture* back, ISceneNode* parent = 0, SINT32 id = -1) = 0;



		//! �ڳ���ͼ�����һ�����ڵ�
		/** һ�������һ�����ڲ�ʹ��һ��ȫ������Ĵ��Ͱ����岢���������������Χ
		\param texture: ����������
		\param horiRes: �����һ��ˮƽ��Ķ����� 
		\param vertRes: �����һ����ֱ��Ķ����� 
		\param texturePercentage: ����߶ȵ�ʹ�ñ�����Ӧ����0��1֮�䡣
		\param spherePercentage: ����Ļ��Ʊ�����ֵӦ����0��2֮�䣬 1�Ǿ�ȷ�İ�����2����������
		\param radius ����İ뾶
		\param parent: ���ĸ��ڵ�,һ�����ͨ��û�и��ڵ㣬����Ӧ��Ϊ�ա�ע��������ڵ㱻���ã���������Ʋ��ᱻ�ı�
		\param id: �ڵ�ID
		\return ����ɹ�����һ���ӿڵ�ָ�� */
		virtual ISceneNode* addSkyDomeSceneNode( ITexture* texture,
			UINT32 horiRes = 16, UINT32 vertRes = 8,
			Real texturePercentage = 0.9, Real spherePercentage = 2.0, Real radius = 1000.f,
			ISceneNode* parent = 0, Real id = -1) = 0;



		//! ���һ������ϵͳ�ڵ㵽����ͼ��
		/** 
		\param withDefaultEmitter: ����һ��Ĭ�Ϲ��������ӷ���������Ϊtrue��action�п���һ������ϵͳ.
		�������Ϊfalse����ͨ��IParticleSystemSceneNode::setEmitter()����һ�����ӷ�����
		\param parent: ���ڵ㣬����Ϊ��
		\param id: �ڵ�ID
		\param position: ����ڸ��ڵ�Ŀռ�λ��
		\param rotation: �����ڵ��ʼ��ת
		\param scale: �����ڵ�ĳ�ʼ����
		\return  ����ɹ�����һ���ӿڵ�ָ�� */
		virtual IParticleSystemSceneNode* addParticleSystemSceneNode(
			bool withDefaultEmitter = true, ISceneNode* parent = 0, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f)) = 0;


		//! ���һ�����γ����ڵ㵽����ͼ��
		/** ����ڵ���GEO mipmapping�ļ���ʵ��һ���򵥵�����Ⱦ����
		Ϊ�˼���Զ�������ο��ϸ�ڡ�
		��������ڵ���м�ֵ���β���������ʱ���������򿪷ǳ��ǳ���ĵ�����Ұ
		����CLOD(����LOD)�㷨ͨ�������������Patch�ľ���������ÿ������һ��LOD��Patch������

		����Patch�Ĵ�С����Ϊ2^N+1 ��i.e. 8+1(9), 16+1(17), etc.
		MaxLOD ֱ����������patch�Ĵ�С
		LOD 0 ����һ��Patch���������ϸ�������е�������������
		ÿ��Lod����1������������-2^lod, ���Զ���LOD 1�� ������2������LOD 2��������4, 
		LOD 3 -8.  �������������path�Ĵ�С������LOD 8, ��һ����СΪ17��patch�� ������2^8=256����������,һ��patch��������17�Ĵ�С��
		����maxLOD 5��һ��patch��С17��2^4=16��,LOD 1��ÿ2�����㣩 LOD2 (ÿ4������)  LOD3(ÿ8������)  LOD4��ÿ16�����㣩
		\param heightMapFileName: �ڴ����ļ�������ȡ�������ݡ���Ӧ��Ϊһ���Ҷ�����λͼ
		\param parent: P ���ڵ㣬����Ϊ��
		\param id: �ڵ�ID
		\param position: �ڵ�ľ���λ��
		\param rotation: �ڵ�ľ�����תֵ. ��û��ʵ�֣�
		\param scale: �����������ӣ��������129*129�ĸ߳�ͼ������Ϸ�е���Ϊ12900*12900����Ϸ��λ����������һ��vector3( 100.0f, 100.0f, 100.0f )
	    �����Y��������Ϊ0����ô���λ��ƽ��
		\param vertexColor: ����Ĭ����ɫ�����û��ָ����������ڵ��������ô���ж��㶼���������ɫ�� Ĭ��Ϊ��ɫ��
		\param maxLOD: �ڵ�����LOD,�����֪������ʲô�ٸı���������ܻ������ֵ����
		\param patchSize: ���ε�patch��С�������֪������ʲô�ٸı���������ܻ������ֵ����
		\param smoothFactor: ����ƽ������
		\param addAlsoIfHeightmapEmpty: ����ͨ���ո߶�ͼ��ӵ��νڵ�
		\return �����ĳ����ڵ��ָ�룬�������û�ܴ���������Ϊ�ա�
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

		//!���һ�����γ����ڵ㵽����ͼ��
		/** ͬ�ϣ�ֻ������IReaderFileָ�����
		\param heightMapFile: �ڴ����ļ�ָ�룬��ȡ�������ݡ���Ӧ��Ϊһ���Ҷ�����λͼ
		\param parent: P ���ڵ㣬����Ϊ��
		\param id: �ڵ�ID
		\param position: �ڵ�ľ���λ��
		\param rotation: �ڵ�ľ�����תֵ. ��û��ʵ�֣�
		\param scale: �����������ӣ��������129*129�ĸ߳�ͼ������Ϸ�е���Ϊ12900*12900����Ϸ��λ����������һ��vector3( 100.0f, 100.0f, 100.0f )
	    �����Y��������Ϊ0����ô���λ��ƽ��
		\param vertexColor: ����Ĭ����ɫ�����û��ָ����������ڵ��������ô���ж��㶼���������ɫ�� Ĭ��Ϊ��ɫ��
		\param maxLOD: �ڵ�����LOD,�����֪������ʲô�ٸı���������ܻ������ֵ����
		\param patchSize: ���ε�patch��С�������֪������ʲô�ٸı���������ܻ������ֵ����
		\param smoothFactor: ����ƽ������
		\param addAlsoIfHeightmapEmpty: ����ͨ���ո߶�ͼ��ӵ��νڵ�
		\return �����ĳ����ڵ��ָ�룬�������û�ܴ���������Ϊ�ա� */
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