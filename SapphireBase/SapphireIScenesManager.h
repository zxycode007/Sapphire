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
	//  ������Ⱦ��ͨ��ö��
	/** һ���������ݵ�������������registerNodeForRendering()������ָ���ڻ���ʱ����ڵ�������������ڵ�Ĺ�ϵ
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
	class IImage;
	class SMaterial;
	//������ѡ����
	class ITriangleSelector;
	class IAttributes;

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
	class IWriteFile;
	class IXMLWriter;
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
	//�ٵı任�ڵ�
	class IDummyTransformationSceneNode;
	//�ı������ڵ�
	class ITextSceneNode;
	//billboard�ı������ڵ�
	class IBillboardTextSceneNode;
	//�����ڵ��Animator
	class ISceneNodeAnimator;
	//��ײ��Ӧ�ĳ����ڵ��Animator
	class ISceneNodeAnimatorCollisionResponse;
	// Ԫ��Ϣ������ѡ����
	class IMetaTriangleSelector;
	//���������
	class  IMeshLoader;
	//������ײ������
	class ISceneCollisionManager;
	//���������
	class IMeshManipulator; 
	//�¼�
	class SEvent;
	//�����ڵ㹤��
	class ISceneNodeFactory;
	//�����ڵ㶯������
	class ISceneNodeAnimatorFactory;
	//����д����
	class IMeshWriter;
	//��Ƥ����
	class ISkinnedMesh;
	//
	class ILightManager; 
	//������������
	class IGeometryCreator;




	/***
	Sapphire �ĳ���������
	*/
	class _SapphireExport  ISceneManager : public virtual IReferenceCounter
	{
	public:

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
		virtual ISceneNode* addSkyBoxSceneNode(ITexture* top, ITexture* bottom,
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
		virtual ISceneNode* addSkyDomeSceneNode(ITexture* texture,
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


		//! ���һ���յĳ����ڵ㵽����ͼ��
		/** �������ڸ߼��任���߳���ͼ�ṹ
		\return ���ؽڵ�ָ�� */
		virtual ISceneNode* addEmptySceneNode(ISceneNode* parent = 0, SINT32 id = -1) = 0;


		//! ���һ���ٱ任�����ڵ㵽����ͼ��
		/** ��������ڵ㲻�ᱻ��Ⱦ�� Ҳ���ܶ�set/getPosition��set/getRotation �� set/getScale������Ӧ������һ��������Ϊ���Ʊ任��
		\return ���ؽڵ�ָ�� */
		virtual IDummyTransformationSceneNode* addDummyTransformationSceneNode(
			ISceneNode* parent = 0, SINT32 id = -1) = 0;


		//! ���һ���ı������ڵ㣬����ʾһ��2d�ı���3ά�ռ��һ��λ��
		virtual ITextSceneNode* addTextSceneNode(IGUIFont* font, const String* text,
			ColourValue color = ColourValue(100, 255, 255, 255),
			ISceneNode* parent = 0, Vector3& position = Vector3(0, 0, 0),
			SINT32 id = -1) = 0;



		//! ���һ��ʹ��billboard���ı������ڵ㡣����ڵ�������������棬 ͨ�������������
		/**
		\param font ����billboard�ϵ����壬 ����0��GUI����Ĭ�ϵ�����
		\param text �ַ����ı�
		\param parent ���ڵ�. 0�Ƿ�root�ڵ�
		\param size billboard�Ŀ����
		\param position billboards��Ը��ڵ��λ��
		\param id: �ڵ�ID
		\param colorTop billboard������ɫ�����ֵ��Ĭ��white��
		\param colorBottom billboard������ɫ�����ֵ��Ĭ��white)
		\return ���ؽڵ�ָ�� */
		virtual IBillboardTextSceneNode* addBillboardTextSceneNode(IGUIFont* font, const String* text,
			ISceneNode* parent = 0,
			const  dimension2d<Real>& size = dimension2d<Real>(10.0f, 10.0f),
			const Vector3& position = Vector3(0, 0, 0), SINT32 id = -1,
			ColourValue colorTop = ColourValue(0xFFFFFFFF), ColourValue colorBottom = ColourValue(0xFFFFFFFF)) = 0;


		//! ���һ��ɽ��ƽ����������������
		/** .��ͨ����������Ŀ��ٲ��ԡ������ָ����ƽ�����ж���ɽ���������ж�ߡ����������Ϊ���������.
	    ,�������������ӵ�������У����ҿ���ͨ��ISceneManager::getMesh()ȡ��
		\param name: ��������֣�Ϊ�˿����� ISceneManager::getMesh(),�����뱻ָ����
		\param tileSize: �����tile��С (10.0f, 10.0f)��һ�������ֵ
		\param tileCount: ָ���ж���tile�� �����ָ��tile��С��10.0f,10.0f������tileCount(10,10)
		��ô��ȡ��һ��100��tile���ɵ�100.0f*100.0f�ľ�������
		\param material: �������Ĳ���
		\param hillHeight: ɽ��ĸ߶ȡ�
		\param countHills: ɽ����ƽ���ϵ�����
		\param textureRepeatCount: ������x��y���������ظ��Ĵ���
		return �������ʧ�ܷ���NULL�� ���ԭ��������Ч�������*/
		virtual IAnimatedMesh* addHillPlaneMesh(const  path& name,
			const dimension2d<Real>& tileSize, const  dimension2d<UINT32>& tileCount,
			SMaterial* material = 0, Real hillHeight = 0.0f,
			const dimension2d<Real>& countHills = dimension2d<Real>(0.0f, 0.0f),
			const dimension2d<Real>& textureRepeatCount = dimension2d<Real>(1.0f, 1.0f)) = 0;



		//! ���һ����̬���������������
		/** ���������һ�������ļ���һ���߶�ͼ�ļ��������д���. �����ļ������Ժܾ޴�
		(8000x8000 ����Ӧ��û����)��Ϊ�б�Ҫ�Ļ�����������ָ�����ļ�Ϊ��С������
		�����Ϊ�������ָ��һ�����֣���Ϊ������ӵ�������У�������ISceneManager::getMesh()ͨ����������Ϊ����ȡ��
		\param meshname: �����������֣�ISceneManager::getMesh()����ȡ��
		\param texture: ���ε�������ע�⣺��ͨ������һ��Ӳ��������ITexture��,ֻ��Ϊһ��IImage�����������Լ�����IVideoDriver::createImageFromFile()�������������
		\param heightmap: һ���Ҷȸ߶�ͼ�񣬾���ͬ������������IVideoDriver::createImageFromFile()�����������������ε�����ȡ�����������Ĵ�С��������С�ĸ߶�ͼ��
		�����ṩ��Ⱦ�ٶȡ�
		\param stretchSize: ���������ڸ߶�ͼ�ϵ�һ�������ж��
		\param maxHeight: ����һ����ɫ�����ڸ߶�ͼ���ж��
		\param defaultVertexBlockSize: Ĭ�϶�����С�������ĳ�ʼ�ߴ�
		\return Null �������ʧ�ܣ�������Ϊ��Ч��������������������Ѿ�����,��������û���ҵ�������ɹ�����������ָ�� */
		virtual IAnimatedMesh* addTerrainMesh(const  path& meshname,
			IImage* texture, IImage* heightmap,
			const  dimension2d<Real>& stretchSize = dimension2d<Real>(10.0f, 10.0f),
			Real maxHeight = 200.0f,
			const  dimension2d<UINT32>& defaultVertexBlockSize = dimension2d<UINT32>(64, 64)) = 0;



		//! ���һ����̬��ͷ���������
		/** \param name ������
		\param vtxColorCylinder Բ�������ɫ
		\param vtxColorCone Բ׶�����ɫ
		\param tesselationCylinder ���Բ���������ı�������Բ�������չ����һ�����Σ�
		\param tesselationCone ���Բ׶�嶥������������
		\param height ��ͷ���ܸ߶�  ����ͷ�߶�=Բ׶�߶�+Բ���߶ȣ�
		\param cylinderHeight Բ������ܸ߶ȣ�Ӧ��С���ܸ߶�
		\param widthCylinder Բ����ֱ��
		\param widthCone Բ׶��ĵ�����ֱ���� Ӧ��С��Բ׶���ֱ��
		\return ����ɹ����ؼ�ͷ�����ָ�룬�������0
	     */
		virtual IAnimatedMesh* addArrowMesh(const path& name,
			ColourValue vtxColorCylinder = ColourValue(0xFFFFFFFF),
			ColourValue vtxColorCone = ColourValue(0xFFFFFFFF),
			UINT32 tesselationCylinder = 4, UINT32 tesselationCone = 8,
			Real height = 1.f, Real cylinderHeight = 0.6f,
			Real widthCylinder = 0.05f, Real widthCone = 0.3f) = 0;



		//! ���һ����̬�������������
		/** \param name ���������
		\param radius ����İ뾶
		\param polyCountX ����ˮƽtile���ı�������
		\param polyCountY ���崹ֱtile���ı�������
		\return ����ɹ���������������ָ�룬���򷵻�0
		 */
		virtual IAnimatedMesh* addSphereMesh(const path& name,
			Real radius = 5.f, UINT32 polyCountX = 16,
			UINT32 polyCountY = 16) = 0;


		//! ���һ����������������
		/** \param name ������
		\param SubdivideU ˮƽϸ����
		\param SubdivideV ��ֱϸ����
		\param FootColor ��ĵײ���ɫ
		\param TailColor ��Ķ�����ɫ
		\return ���ʧ�ܷ���0
		*/
		virtual IAnimatedMesh* addVolumeLightMesh(const path& name,
			const UINT32 SubdivideU = 32, const UINT32 SubdivideV = 32,
			const ColourValue FootColor = ColourValue(51, 0, 230, 180),
			const ColourValue TailColor = ColourValue(0, 0, 0, 0)) = 0;



		//! ��ȡROOT�����ڵ�
		/** �����������ڵ��root�ڵ�.����������ڵ���һ������ĳ����ڵ�,���Ĵ���ֻ���ڹ������еĳ����ڵ㡣
		�����ܴӳ������Ƴ���
		\return ���ظ��ڵ�ָ��
		 */
		virtual ISceneNode* getRootSceneNode() = 0;


		//! ��ȡָ��ID�ĵ�һ�������ڵ�
		/** \param id: Ҫ������id
		\param start: ����������ڵ㿪ʼ�����е�ֻ�ڵ㶼�ᱻ���������NULL��ָ������ô��Ӹ��ڵ㿪ʼ
		\return ָ�����id���׸������ڵ㣬���û�У����ؿ�
		*/
		virtual ISceneNode* getSceneNodeFromId(SINT32 id, ISceneNode* start = 0) = 0;




		//! ��ȡָ�����ֵĵ�һ�������ڵ�
		/** \param name: Ҫ����������
		\param start: ����������ڵ㿪ʼ�����е�ֻ�ڵ㶼�ᱻ���������NULL��ָ������ô��Ӹ��ڵ㿪ʼ
		\return ָ�����id���׸������ڵ㣬���û�У����ؿ� */
		virtual ISceneNode* getSceneNodeFromName(const c8* name, ISceneNode* start = 0) = 0;



		//! ��ȡָ�����͵ĳ����ڵ�
		/** \param type: Ҫ�����ĳ����ڵ�
		\param start: ����������ڵ㿪ʼ�����е�ֻ�ڵ㶼�ᱻ���������NULL��ָ������ô��Ӹ��ڵ㿪ʼ
		\return ָ�����id���׸������ڵ㣬���û�У����ؿ� */
		virtual ISceneNode* getSceneNodeFromType(ESCENE_NODE_TYPE type, ISceneNode* start = 0) = 0;



		//! ��ȡָ�����͵ĳ����ڵ�
		/** \param type: Ҫ���ҵĳ����ڵ�����(ESNT_ANY ���������к��ӽڵ�).
		\param outNodes: Ҫ���Ľ������
		\param start: ����������ڵ㿪ʼ�����е�ֻ�ڵ㶼�ᱻ���������NULL��ָ������ô��Ӹ��ڵ㿪ʼ */
		virtual void getSceneNodesFromType(ESCENE_NODE_TYPE type,
			vector<ISceneNode*>& outNodes,
			ISceneNode* start = 0) = 0;


		//! ��ȡ��ǰ����
		/** \return ���ص�ǰ������ָ��,���ΪNULL�� ��ôû�д������*/
		virtual ICameraSceneNode* getActiveCamera() const = 0;



		//! ���õ�ǰ������
		/** ǰһ��������ʧ��
		\param camera: ����µı����������ڵ� */
		virtual void setActiveCamera(ICameraSceneNode* camera) = 0;



		//! ͨ����������������ģ�建������Ӱ����ɫ
		virtual void setShadowColor(ColourValue color = ColourValue(150, 0, 0, 0)) = 0;

		//! ��ȡ��ǰ��Ӱ����ɫ
		virtual ColourValue getShadowColor() const = 0;

		//! Ϊ��һ���ض���ʱ����Ⱦ��ע��һ���ڵ�
		/** �������Ӧ��ֻ������һ�������ڵ��ISceneNode::OnRegisterSceneNode()
		\param node: ����ƶ�ע��Ľڵ�. ͨ�������ڵ����������'this'��Ϊһ������
		\param pass:ָ�����ڵ�Ҫ������ʱ�������ڵ�Ĺ�ϵ�����磬����ڵ���һ����Ӱ����ͨ�������������нڵ����֮����ƣ�����ʹ��ESNRP_SHADOW.
		������E_SCENE_NODE_RENDER_PASS��ϸ��
		\return �����ᱻ��Ⱦ��ͨ�����޳��� */
		virtual UINT32 registerNodeForRendering(ISceneNode* node,
			E_SCENE_NODE_RENDER_PASS pass = ESNRP_AUTOMATIC) = 0;



		//! �������еĳ����ڵ�
		/** ��ֻ����IVideoDriver::beginScene()��IVideoDriver::endScene()֮��ִ��. 
		��ע�⣺�������������ʱ��������������ڻ��ƣ����һ�������д��ڵĳ����ڵ��animator��
		�޳����Գ����ڵ���޳�
		 */
		virtual void drawAll() = 0;


		//! ����һ����ת��animator�������Լ��Ĺ����ڵ������ת
		/** \param rotationSpeed ָ����������ת���ٶ� ÿ10����1��
		\return ���animator. �������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop()
		*/
		virtual ISceneNodeAnimator* createRotationAnimator(const Vector3& rotationSpeed) = 0;



		//! ����һ�����Ʒ��е�animator�� ��ʹ�ù����ڵ���һ�����Ļ��Ʒ���
		/** \param center: ���Ƶ�����
		\param radius: ���ư뾶
		\param speed: �����ٶȣ�ÿ���뻡��
		\param direction: ָ��up�����������������Ķ���
		\param startPosition: ���animator��ʼ����ʱ����ʼλ��. ֵ��һ��Բ�ı����� 0.5�ǻ���·����һ��
		\param radiusEllipsoid: ���radiusEllipsoid != 0 ��ôradius2��һ����Բ��ʼ��ֵ��һ��Բ�ı����� 0.5�ǻ���·����һ��
		\return  �������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFlyCircleAnimator(
			const Vector3& center = Vector3(0.f, 0.f, 0.f),
			Real radius = 100.f, Real speed = 0.001f,
			const Vector3& direction = Vector3(0.f, 1.f, 0.f),
			Real startPosition = 0.f,
			Real radiusEllipsoid = 0.f) = 0;
		


		//! ����һ��ֱ�߷��е�animator�� ��ʹһ�������ĳ����ڵ���һ��ֱ�ߵ����������ƶ�
		/** \param startPoint: ����ֱ�ߵĿ�ʼ��
		\param endPoint: ����ֱ�ߵĽ�����
		\param timeForWay:�������е��յ���Ҫ�ĺ�����
		\param loop: �Ƿ�ѭ�������Ϊtrue�������е�������ʱ�������´ӿ�ʼ�㿪ʼ
		\param pingpong ��׼�������animator�Ƿ�ӽ����㷵����ʼ��
		\return ���animator. �������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const Vector3& startPoint,
			const Vector3& endPoint, UINT32 timeForWay, bool loop = false, bool pingpong = false) = 0;



		//! ����һ������animator�� ��ͨ��һ�������б��л�Ŀ�곡���ڵ������
		/** \param textures: Ҫʹ�õ�������б�
		\param timePerFrame: ÿ֡�ĺ������� �б�������������ܱ���ʾ��ʱ��
		\param loop: ���Ϊfalse������ֹͣ�����һ������ᱻ���á����Ϊtrue�����������´ӵ�һ������ʼ��
		\return ���animator. �������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createTextureAnimator(const vector<ITexture*>& textures,
			SINT32 timePerFrame, bool loop = true) = 0;


		//! ����һ�������ڵ�animator������һ��ʱ����Զ�ɾ����������ڵ�
		/** \param timeMs: ����������ʱ���ڵ�ᱻɾ��
		\return ���animator. �������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop()  */
		virtual ISceneNodeAnimator* createDeleteAnimator(UINT32 timeMs) = 0;


		//! ����һ������ĳ����ڵ��animator�������Զ�������ײ���ͷ�Ӧ
		/** �� ISceneNodeAnimatorCollisionResponse ϸ��
		\param world: ������ѡ�����������������п�������������ڵ㷢����ײ�������Ρ� 
		����ͨ��ISceneManager::createTriangleSelector()����һ��������ѡ����
		\param sceneNode: ���ٿصĳ����ڵ㡣֮����������animator����������ڵ㣬��������ڵ㲻���ƶ�����ǽ��
		�����ܵ�����Ӱ�졣�������ѽ������������ڵ㵽һ���µ�λ�ö������ܵ���ײ������Ӱ�죬�����call sceneNode->setPosition()��
		animator->setTargetNode(sceneNode)
		\param ellipsoidRadius: ��ײ���ͷ�Ӧ����Բ�뾶�������ȡ��һ�������ڵ㣬���Ҳ�ȷ��Ӧ�����ö��İ뾶����Ӧ�������д���
		��⣺
		AxisAlignedBox& box = yourSceneNode->getBoundingBox();
		Vector3 radius = box.MaxEdge - box.getCenter();
		\
		\param gravityPerSecond: ���û�������������Ϊһ��ÿ��ÿ��ĵ�λ���ٶȡ�
		�����ĵ�λ=�ף���ôVector3(0,-10.0f,0)�ӽ���ʵ��������
		\param ellipsoidTranslation: Ĭ������£������ײ������Բ������������ڵ������Ϊ���ģ�
		����ζ����Բ��������������㲻�������� �����ָ��һ�����������Բ�ĵ���
		\param slidingValue: DOCUMENTATION NEEDED.
		\return ���animator,���ᵼ����ײ���ͷ�Ӧ���������������ڵ���ISceneNode::addAnimator()��Ȼ�����animator�ܹ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop()*/
		virtual ISceneNodeAnimatorCollisionResponse* createCollisionResponseAnimator(
			ITriangleSelector* world, ISceneNode* sceneNode,
			const Vector3& ellipsoidRadius = Vector3(30, 60, 30),
			const Vector3& gravityPerSecond = Vector3(0, -10.0f, 0),
			const Vector3& ellipsoidTranslation = Vector3(0, 0, 0),
			Real slidingValue = 0.0005f) = 0;



		//! ����һ�������ߵ�animator
		/** ���animator����hermit�����߸ı�����ڵ��λ�á�����Hermite�����ߵ�һ���Ӽ���Ҫô������(tightness != 0.5)Ҫôcatmull rom(tightness == 0.5)����
		���animator��1/speed���ڴ�һ�����Ƶ��ƶ�����һ����
		�������Ҫ������animator�����Ե���ISceneNodeAnimator::drop() */
		virtual ISceneNodeAnimator* createFollowSplineAnimator(SINT32 startTime,
			const vector< Vector3 >& points,
			Real speed = 1.0f, Real tightness = 0.5f, bool loop = true, bool pingpong = false) = 0;


		//! ����һ����ITriangleSelector, ����һ������
		/** ������ѡ����
		���Ա�������ײ���á���Ҫ�����ѡ�����Ժ�������������Quake3 map֮��ʹ�ã�������ISceneManager::createOctreeTriangleSelector()
		��ע�⣺����������ѡ���������Զ���������������ڵ㡣����Ҫͨ������ISceneNode::setTriangleSelector()������������
		ITriangleSelector* s = sceneManager->createTriangleSelector(yourMesh,
		yourSceneNode);
		yourSceneNode->setTriangleSelector(s);
		s->drop();
		\param mesh: ��������ε�����
		\param node: ���ڿɼ��ĺͱ任�ĳ����ڵ�
		. */
		virtual ITriangleSelector* createTriangleSelector(IMesh* mesh, ISceneNode* node) = 0;

		//! ����һ���򵥵�ITriangleSelector, ����һ���ɻ��animated���ĳ����ڵ�
		/** ������ڵ�����������ϸ�ڽ����ڲ���ȡ
		����ITriangleSelector::update() �����������ѡ����ȥ��������������񳡾��ڵ�ĵ�ǰ֡ 
		\param node �������ѡ�����Ķ��������ڵ�
		*/
		virtual ITriangleSelector* createTriangleSelector(IAnimatedMeshSceneNode* node) = 0;

		//! ����һ������һ��AABB�ļ򵥶�̬ITriangleSelector
		/** ������ѡ��������������ײ��⡣ÿ�������α���ѯ�������������ѡ������ȡ����ڵ����ײ���ӣ�һ�������µ������Ρ�
		��������ʺ϶��������ڵ㡣
		\param node: ���ڿɼ��ĺͱ任�ĳ����ڵ�
		\return ���ѡ����
		 */
		virtual ITriangleSelector* createTriangleSelectorFromBoundingBox(ISceneNode* node) = 0;


		//!  ����ͨ���˲����Ż���һ��������ѡ����
		/** Triangle selectors
		���Ա�������ײ���ã����������ѡ�������ں��������Σ� ���ɰ˲���������֯��
		��ע�⣺����������ѡ���������Զ���������������ڵ㡣����Ҫͨ������ISceneNode::setTriangleSelector()������������
		 
		ITriangleSelector* s = sceneManager->createOctreeTriangleSelector(yourMesh,
		yourSceneNode);
		yourSceneNode->setTriangleSelector(s);
		s->drop();
		������Ϣ���Ӽ�SDK�е���ײ�̳�
		\param mesh: ��������ε�����
		\param node:���ڿɼ��ĺͱ任�ĳ����ڵ�
		\param minimalPolysPerNode: ָ��һ���˲����ڵ������С�Ķ��������
		\return ���ѡ���� */
		virtual ITriangleSelector* createOctreeTriangleSelector(IMesh* mesh,
			ISceneNode* node, SINT32 minimalPolysPerNode = 32) = 0;


		//! ����һ��Ԫ��Ϣ�����ε�ѡ����
		/** һ��Ԫ��Ϣ������ѡ����ֻ������Ϊһ������������ѡ�����ļ����ṩһ��������������ѡ�����Ľӿڡ�
		�������ʽ����ײ���Կ���һ�δ���ͬ�ĵ�triangle soups��
		\return ���ѡ����
	  */
		virtual IMetaTriangleSelector* createMetaTriangleSelector() = 0;


		//! ����һ�����Դӵ��γ����ڵ�ѡ�������ε�������ѡ����
		/** \param node: �����ĵ��γ����ڵ��ָ��
		\param LOD: LOD��0�����ϸ��
		\return ���ѡ���� */
		virtual ITriangleSelector* createTerrainTriangleSelector(
			ITerrainSceneNode* node, SINT32 LOD = 0) = 0;


		//! ���һ�������µ��ļ���ʽ�Ķ�������������
		/** �������������չ���ܱ�֧�ֵ��ļ���ʽ����Ҫ����ļ�������ʵ��IMeshLoader�ӿڲ���ͨ����������������
		ʹ������������������Ѿ��ø��º�Ĳ����������±���İ汾�������ڽ��������������
		\param externalLoader: һ���µ������������ʵ�� */
		virtual void addExternalMeshLoader(IMeshLoader* externalLoader) = 0;

		//! ���ش�ʱ֧�ֵ����������������
		virtual UINT32 getMeshLoaderCount() const = 0;

		//! ���ظ��������������
		/** \param index ���ؼ����������������������һ������0���������
		\return һ��ָ����������ָ�룬�����������ȷ����0 */
		virtual IMeshLoader* getMeshLoader(UINT32 index) const = 0;


		//! ��ȡ������ײ������ָ��
		/** \return ��ײ������ָ��
		*/
		virtual ISceneCollisionManager* getSceneCollisionManager() = 0;

		//! ��ȡ�����������ָ��
		/** \return �����������ָ��
		*/
		virtual IMeshManipulator* getMeshManipulator() = 0;

		//! ���һ�������ڵ㵽ɾ������
		/** 
		����������ڵ㵽���ǰ�ȫ��ʱ������ɾ���� ����ζ�ŵ���������ڵ㲻��ִ��animator�����Ƶġ�
		�����������ͨ�����ǵĳ����ڵ�animatorɾ�������ڵ㡣�ڴ��������£����� ISceneNode::remove()�㹻�ˡ�
		�����ɾ�������ǲ���Ҫ�ġ��� ISceneManager::createDeleteAnimator()ϸ��
		\param node: Ҫɾ���Ľڵ�. */
		virtual void addToDeletionQueue(ISceneNode* node) = 0;


		//! ����һ�������¼�������
		/** ͨ���㲻��ʹ�ô˷����������ڲ�������� */
		virtual bool postEventFromUser(const SEvent& event) = 0;


		//! ������������
		/** ���еĽڵ㶼�ᱻ�Ƴ�*/
		virtual void clear() = 0;


		//! ��ȡ��������Ĳ����ӿ�
		/** �ַ������������ɲ�������������ʹ�á����磺CMS �� LMTS ��������Ҫһ�����ֽ�'CSM_TexturePath'
		�� 'LMTS_TexturePath' �Ĳ��������ù������������·������See
		CSM_TEXTURE_PATH, LMTS_TEXTURE_PATH, MY3D_TEXTURE_PATH,
		COLLADA_CREATE_SCENE_INSTANCES, DMF_TEXTURE_PATH �� DMF_USE_MATERIALS_DIRS
		*/
		virtual  IAttributes* getParameters() = 0;

		//! ��ȡ��ǰ����Ⱦͨ��
		/** ���еĳ����ڵ㶼���ض���˳���±���Ⱦ�������ǹ�Դ���������պ��ӣ� ʵ�弸���壬��͸�����塣
		����Ⱦ�����У������ڵ����Ҫ֪����ǰ������Ⱦ�ĳ�����Ⱦ����������Ϊ���磺��������Ⱦʱ��ע�����Σ� һ�ζ���͸���������һ���ǹ��塣
		��ʱҪ֪������һ����ǰ��Ⱦͨ���Ǽ��������ܹ���Ⱦ���ǵļ�������ȷ����һ����
		 */
		virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() const = 0;

		//! ��ȡĬ�ϵĳ����ڵ㹤���������Դ����������õĳ����ڵ�
		/** \return ����Ĭ�ϵĳ����ڵ㹤��ָ��*/
		virtual ISceneNodeFactory* getDefaultSceneNodeFactory() = 0;


		//! ���һ�������ڵ㹤�����������������
		/** ����ͨ���µĳ����ڵ���������չ����������Ӧ���Զ������� ���統��xml�ļ��������� */
		virtual void registerSceneNodeFactory(ISceneNodeFactory* factoryToAdd) = 0;

		//! ��ȡע��ĳ����ڵ㹤��������
		virtual UINT32 getRegisteredSceneNodeFactoryCount() const = 0;


		//! ͨ��������ȡһ�������ڵ㹤��
		/** \return ����һ�����󳡾��ڵ��ָ�� */
		virtual ISceneNodeFactory* getSceneNodeFactory(UINT32 index) = 0;



		//! ��ȡĬ�ϳ����ڵ�animator�����������������������ó����ڵ��animator
		/** \return ���ػ�ȡĬ�ϳ����ڵ�animator������ָ�� */
		virtual ISceneNodeAnimatorFactory* getDefaultSceneNodeAnimatorFactory() = 0;


		//! ��ȡ�����ڵ�animator����������
		virtual UINT32 getRegisteredSceneNodeAnimatorFactoryCount() const = 0;


		//! ͨ��������ȡ�����ڵ��animator����
		/** \return �������󳡾��ڵ��animator������ָ��*/
		virtual ISceneNodeAnimatorFactory* getSceneNodeAnimatorFactory(UINT32 index) = 0;


		//! ��ȡ�����ڵ����͵��������� ���Ϊnull����û�ҵ�
		virtual const c8* getSceneNodeTypeName(ESCENE_NODE_TYPE type) = 0;

		//! ��ȡ�����ڵ�animator���͵��������� ���Ϊnull����û�ҵ�
		virtual const c8* getAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) = 0;


		//! ͨ�����������һ�������ڵ�
		/** \return ͨ��һ������ָ����ӵĳ����ڵ��ָ�� */
		virtual ISceneNode* addSceneNode(const char* sceneNodeTypeName, ISceneNode* parent = 0) = 0;

		//! ����һ�������������ĳ����ڵ�Animator 
		/** \param typeName: Ҫ��ӵĳ����ڵ��Animator��������
		\param target: ���Animator��Ŀ�곡���ڵ�
		\return ���������Animator�ĳ����ڵ��ָ�� */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target = 0) = 0;
		


		//! ����һ���³���������
		/** ���ܹ������׵Ľ��л��Ʋ�����ͬʱ������������������
		���񻺳����������еĴ��ڵĳ�����������������ζ���������ԭ�����������ñ���getMesh()����������
		��������������еĳ�����������ͬ����Ч�������ü��ء�
		���Դ/����������������Ȼ��SapphireDevice::getSceneManager()���ʡ��������Ҫ������³����������������¼������磺FPS�����������Ҫ�ֶ�����ǰ����
		��ͨ��ʵ��IEventReceiver�����е���
		yourNewSceneManager->postEventFromUser()���ҷ���true���Ա����Դ��������������ȡ���¼��� �����������뽫�Զ�����������������.
	    */
		virtual ISceneManager* createNewSceneManager(bool cloneContent = false) = 0;

		//! ���浱ǰ������һ���ļ�
		/** ��ѡ��isDebugObject����Ϊtrue���޷����泡���ڵ��
		���������ͨ��д��һ��.irr�ļ��� һ������XML��ʽ�ļ������Ա�Irrlicht����༭���༭��
		Ҫ��������ļ�����ISceneManager::loadScene()��
		\param filename �ļ���
		\param userDataSerializer �������Ҫ����һЩÿ�������ڵ���û����ݵ��ļ�����Ҫʵ��ISceneUserDataSerializer�ӿ�
		�������������Ϊ���������򣬼�ָ��0��Ϊ���������
		\param node �����Ķ����ڵ㣬����ڵ����������������浽�����ļ��У�����0������������������г���
		\return ����ɹ�����true. */
		virtual bool saveScene(const  path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;


		//! ���浱ǰ������һ���ļ�
		/** ��ѡ��isDebugObject����Ϊtrue���޷����泡���ڵ��
		���������ͨ��д��һ��.irr�ļ��� һ������XML��ʽ�ļ������Ա�Irrlicht����༭���༭��
		Ҫ��������ļ�����ISceneManager::loadScene()��
		\param filename д���ļ�ָ��
		\param userDataSerializer �������Ҫ����һЩÿ�������ڵ���û����ݵ��ļ�����Ҫʵ��ISceneUserDataSerializer�ӿ�
		�������������Ϊ���������򣬼�ָ��0��Ϊ���������
		\param node �����Ķ����ڵ㣬����ڵ����������������浽�����ļ��У�����0������������������г���
		\return ����ɹ�����true. */
		virtual bool saveScene(IWriteFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;

		//! ���浱ǰ������һ���ļ�
		/** ��ѡ��isDebugObject����Ϊtrue���޷����泡���ڵ��
		���������ͨ��д��һ��.irr�ļ��� һ������XML��ʽ�ļ������Ա�Irrlicht����༭���༭��
		Ҫ��������ļ�����ISceneManager::loadScene()��
		\param writer Ҫ����ĳ�����XMLWriter
		\param currentPath ��ǰ·��
		\param userDataSerializer �������Ҫ����һЩÿ�������ڵ���û����ݵ��ļ�����Ҫʵ��ISceneUserDataSerializer�ӿ�
		�������������Ϊ���������򣬼�ָ��0��Ϊ���������
		\param node �����Ķ����ڵ㣬����ڵ����������������浽�����ļ��У�����0������������������г���
		\return ����ɹ�����true.*/
		virtual bool saveScene(IXMLWriter* writer, const path& currentPath, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* node = 0) = 0;


		//! ����һ��������ע�⣺��ǰ���ڵĳ��������ᱻ�����
		/** �������ͨ����.irr�ļ���ȡ�� һ������XML��ʽ�ļ������Ա�Irrlicht����༭���༭��
		���ǿ�ͨ��ISceneManager::addExternalSceneLoader��������ӱ�ĳ������͡� irr�ļ����Ա�Irricht�༭���༭
		\param filename �ļ���
		\param userDataSerializer �������Ҫ���ļ��м��ص���ؽڵ���û����ݡ���Ҫʵ��ISceneUserDataSerializer�ӿ�
		�������������Ϊ���������򣬼�ָ��0��Ϊ�������
		\param rootNode �����ĸ��ڵ㣬����0ֱ����ӳ��������������� 
		\return ����ɹ�����true.*/
		virtual bool loadScene(const path& filename, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0) = 0;

		//! ����һ��������ע�⣺��ǰ���ڵĳ��������ᱻ�����
		/** �������ͨ����.irr�ļ���ȡ�� һ������XML��ʽ�ļ������Ա�Irrlicht����༭���༭��
		���ǿ�ͨ��ISceneManager::addExternalSceneLoader��������ӱ�ĳ������͡� irr�ļ����Ա�Irricht�༭���༭
		\param file �ļ�
		\param userDataSerializer �������Ҫ���ļ��м��ص���ؽڵ���û����ݡ���Ҫʵ��ISceneUserDataSerializer�ӿ�
		�������������Ϊ���������򣬼�ָ��0��Ϊ�������
		\param rootNode �����ĸ��ڵ㣬����0ֱ����ӳ���������������
		\return ����ɹ�����true.*/
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0, ISceneNode* rootNode = 0) = 0;


		//! ��ȡһ�����õ�����д������ʵ��
		virtual IMeshWriter* createMeshWriter(EMESH_WRITER_TYPE type) = 0;

		//! ��ȡһ����Ƥ����*/
		virtual ISkinnedMesh* createSkinnedMesh() = 0;


		//! ���û�������ɫ
		virtual void setAmbientLight(const  ColourValue &ambientColor) = 0;

		//! ȡ�û�������ɫ
		virtual const ColourValue& getAmbientLight() const = 0;

		//! ע��һ���Զ���Ļص������������ڳ�����Ⱦ��ȡ�ûص�
		/** \param[in] lightManager: ����»ص�������.����Դ���0���Ƴ���ǰ�Ļص���������ָ�Ĭ�ϵ���Ϊ*/
		virtual void setLightManager(ILightManager* lightManager) = 0;

		//! ��ȡһ����������������һ��ʵ��
		/** �������������ṩһЩ����������������ͬ���͵Ļ��������塣����Զ��峡���ǳ����� */
		virtual const IGeometryCreator* getGeometryCreator(void) const = 0;

		//! ��⵱ǰ�׶��Ƿ���׶���޳���
		/** ��ע�⣺��������޳��������൱�ֲڵģ���������
		��������������true����ڵ㲻�ɼ����������ڵ㷵��false������ڵ������Ȼ���ɼ�
		\param node Ҫ�޳����Եĳ����ڵ�
		\return True ���Ϊtrue��ڵ㲻�ɼ�*/
		virtual bool isCulled(const ISceneNode* node) const = 0;

	};



}



#endif
