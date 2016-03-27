#include "SapphireCDefaultSceneNodeAnimatorFactory.h"
#include "SapphireCSceneNodeAnimatorCameraFPS.h"
#include "SapphireCSceneNodeAnimatorCameraMaya.h"
#include "SapphireICursorControl.h"
#include "SapphireISceneNodeAnimatorCollisionResponse.h"
#include "SapphireIScenesManager.h"


namespace Sapphire
{
	//! Names for scene node types
	const c8* const SceneNodeAnimatorTypeNames[] =
	{
		"flyCircle",
		"flyStraight",
		"followSpline",
		"rotation",
		"texture",
		"deletion",
		"collisionResponse",
		"cameraFPS",
		"cameraMaya",
		0
	};


	CDefaultSceneNodeAnimatorFactory::CDefaultSceneNodeAnimatorFactory(ISceneManager* mgr, ICursorControl* crs)
		: Manager(mgr), CursorControl(crs)
	{
#ifdef _DEBUG
		setDebugName("CDefaultSceneNodeAnimatorFactory");
#endif

		// don't grab the scene manager here to prevent cyclic references
		if (CursorControl)
			CursorControl->grab();
	}


	CDefaultSceneNodeAnimatorFactory::~CDefaultSceneNodeAnimatorFactory()
	{
		if (CursorControl)
			CursorControl->drop();
	}


	//! creates a scene node animator based on its type id
	ISceneNodeAnimator* CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target)
	{
		ISceneNodeAnimator* anim = 0;

		switch (type)
		{
		case ESNAT_FLY_CIRCLE:
			anim = Manager->createFlyCircleAnimator(Vector3(0, 0, 0), 10);
			break;
		case ESNAT_FLY_STRAIGHT:
			anim = Manager->createFlyStraightAnimator(Vector3(0, 0, 0), Vector3(100, 100, 100), 10000, true);
			break;
		case ESNAT_FOLLOW_SPLINE:
		{
			vector<Vector3>::type points;
			points.push_back(Vector3(0, 0, 0));
			points.push_back(Vector3(10, 5, 10));
			anim = Manager->createFollowSplineAnimator(0, points);
		}
		break;
		case ESNAT_ROTATION:
			anim = Manager->createRotationAnimator(Vector3(0.3f, 0, 0));
			break;
		case ESNAT_TEXTURE:
		{
			vector<ITexture*>::type textures;
			anim = Manager->createTextureAnimator(textures, 250);
		}
		break;
		case ESNAT_DELETION:
			anim = Manager->createDeleteAnimator(5000);
			break;
		case ESNAT_COLLISION_RESPONSE:
			anim = Manager->createCollisionResponseAnimator(0, target);
			break;
		case ESNAT_CAMERA_FPS:
			anim = new CSceneNodeAnimatorCameraFPS(CursorControl);
			break;
		case ESNAT_CAMERA_MAYA:
			anim = new CSceneNodeAnimatorCameraMaya(CursorControl);
			break;
		default:
			break;
		}

		if (anim && target)
			target->addAnimator(anim);

		return anim;
	}


	//! creates a scene node animator based on its type name
	ISceneNodeAnimator* CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(const c8* typeName, ISceneNode* target)
	{
		return createSceneNodeAnimator(getTypeFromName(typeName), target);
	}


	//! returns amount of scene node animator types this factory is able to create
	UINT32 CDefaultSceneNodeAnimatorFactory::getCreatableSceneNodeAnimatorTypeCount() const
	{
		return ESNAT_COUNT;
	}


	//! returns type of a createable scene node animator type
	ESCENE_NODE_ANIMATOR_TYPE CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorType(UINT32 idx) const
	{
		if (idx<ESNAT_COUNT)
			return (ESCENE_NODE_ANIMATOR_TYPE)idx;
		else
			return ESNAT_UNKNOWN;
	}


	//! returns type name of a createable scene node animator type 
	const c8* CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorTypeName(UINT32 idx) const
	{
		if (idx<ESNAT_COUNT)
			return SceneNodeAnimatorTypeNames[idx];
		else
			return 0;
	}

	//! returns type name of a createable scene node animator type 
	const c8* CDefaultSceneNodeAnimatorFactory::getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const
	{
		// for this factory: index == type

		if (type<ESNAT_COUNT)
			return SceneNodeAnimatorTypeNames[type];
		else
			return 0;
	}

	ESCENE_NODE_ANIMATOR_TYPE CDefaultSceneNodeAnimatorFactory::getTypeFromName(const c8* name) const
	{
		for (UINT32 i = 0; SceneNodeAnimatorTypeNames[i]; ++i)
			if (!strcmp(name, SceneNodeAnimatorTypeNames[i]))
				return (ESCENE_NODE_ANIMATOR_TYPE)i;

		return ESNAT_UNKNOWN;
	}
}