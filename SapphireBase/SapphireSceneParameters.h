#ifndef _SAPPHIRE_SCENE_PARAMETERS_
#define _SAPPHIRE_SCENE_PARAMETERS_

#include "SapphirePrerequisites.h"

/*! \ SceneParameters.h
\���������޸�������ص����г���������ͷ�ļ�

���ļ��������в���������Щ����ISceneManager::getParameters()�����޸Ĳ�����������������Ϊ
*/

namespace Sapphire
{
	//! ������θı����͸������ϣ����ʵ�ZWrite ��־�Ŀ��ƵĲ�����
	/** Ĭ��������Ƕ���������ʵ͸���ǹر�д��z���壬��ϲ��ʡ���������ཻ����������⣬
	����Ҳ�ƻ�����Ⱦ�����͸�����ʶ���ZWriteEnableʹ��SMaterial��־�������Ĳ�������ʹ��������
	����
	SceneManager->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true);
	**/
	const c8* const ALLOW_ZWRITE_ON_TRANSPARENT = "Allow_ZWrite_On_Transparent";

	//! �ı�����csm������������·���Ĳ�����
	/**����
	SceneManager->getParameters()->setAttribute(scene::CSM_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const CSM_TEXTURE_PATH = "CSM_TexturePath";

	//! �ı�����lmts������������·��
	/** ����
	SceneManager->getParameters()->setAttribute(scene::LMTS_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const LMTS_TEXTURE_PATH = "LMTS_TexturePath";

	//! �ı�����my3d������������·�� 
	/** ����
	SceneManager->getParameters()->setAttribute(scene::MY3D_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const MY3D_TEXTURE_PATH = "MY3D_TexturePath";

	//! ָ��COLLADA �������ģʽ�Ĳ�����
	/**
	ָ��COLLADA�������ڼ���COLLADA����ʱ�Ƿ�Ӧ��ͬʱ����ģ�͵�ʵ������Դ�������
	Ĭ�ϣ�����Ϊfalse���������Ϊtrue�� ISceneManager::getMesh()������ֻ����һ��ָ��������Ҵ�����collada�ļ������������
	��Դ�������ʵ��
	����
	\code
	SceneManager->getParameters()->setAttribute(scene::COLLADA_CREATE_SCENE_INSTANCES, true);
	\endcode
	*/
	const c8* const COLLADA_CREATE_SCENE_INSTANCES = "COLLADA_CreateSceneInstances";

	//! �ı�����DMF������������·���Ĳ�����
	/** �ڵ���������ʱ�����·����Ϊ������deled�ļ����ļ���ǰ׺�����в��ҡ�
	���������һ��ָ���Ĺ��������޸���·����
	���磺
	\code
	SceneManager->getStringParameters()->setAttribute(scene::DMF_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const DMF_TEXTURE_PATH = "DMF_TexturePath";

	//! �������ڽ���DMF����������DMF����·���ṹ�Ĳ�����
	/** ������������Ϊtrue, ������deled�ļ�������·����Ч�� ����ֻ�������������Ҷ�Ӧ���ļ�.
	�����������·��Ҳ���Ա�ʹ�ã�������ʹ��һ�����õ�ý�岼��.
	���磺
	Otherwise, the
	texture path is also used, which allows to use a nicer media layout.
	Use it like this:
	\code
	//����Ĭ�ϣ�����Ҫʹ���������
	SceneManager->getParameters()->setAttribute(scene::DMF_IGNORE_MATERIALS_DIRS, false);
	\endcode
	\code
	//��Ҫʹ��ʱ
	SceneManager->getParameters()->setAttribute(scene::DMF_IGNORE_MATERIALS_DIRS, true);
	\endcode
	**/
	const c8* const DMF_IGNORE_MATERIALS_DIRS = "DMF_IgnoreMaterialsDir";

	//! ��Ӧ������͸�������е�alpha������ֵ
	/** 
	\code
	//����alpha ����ֵ �� 0.1
	SceneManager->getParameters()->setAttribute(scene::DMF_ALPHA_CHANNEL_REF, 0.1);
	\endcode
	**/
	const c8* const DMF_ALPHA_CHANNEL_REF = "DMF_AlphaRef";

	//! ��Ӧ��ת��ת�򲻷�תtga�ļ��Ĳ�����
	/** 
	\code
	//���磺��תalpha����
	SceneManager->getParameters()->setAttribute(scene::DMF_FLIP_ALPHA_TEXTURES, true);
	\endcode
	**/
	const c8* const DMF_FLIP_ALPHA_TEXTURES = "DMF_FlipAlpha";


	//! �ı�����OBJ������������·���Ĳ�����
	/** ����
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const OBJ_TEXTURE_PATH = "OBJ_TexturePath";

	//! ���������Obj�ļ��е���ṹ
	/** ����
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_LOADER_IGNORE_GROUPS, true);
	\endcode
	**/
	const c8* const OBJ_LOADER_IGNORE_GROUPS = "OBJ_IgnoreGroups";


	//! �������obj�ļ��Ĳ���.mtl�ļ�
	/** ����
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_LOADER_IGNORE_MATERIAL_FILES, true);
	\endcode
	**/
	const c8* const OBJ_LOADER_IGNORE_MATERIAL_FILES = "OBJ_IgnoreMaterialFiles";


	//! ��Ч��b3d�ļ���mipmap��־
	/** ��sapphire����������׼��������磺
	\code
	SceneManager->getParameters()->setAttribute(scene::B3D_LOADER_IGNORE_MIPMAP_FLAG, true);
	\endcode
	**/
	const c8* const B3D_LOADER_IGNORE_MIPMAP_FLAG = "B3D_IgnoreMipmapFlag";

	//! �ı��ڽ�b3d����������·���Ĳ�����
	/** ����
	\code
	SceneManager->getParameters()->setAttribute(scene::B3D_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const B3D_TEXTURE_PATH = "B3D_TexturePath";

	//! ��scene manager ��Ϊ�༭��ʹ��ʱ�����������־
	/** �����animatorϲ������ɾ��animator�ķ�ʽ,�������ֹɾ�������ڵ�.
	  */
	const c8* const SAPPHIRE_SCENE_MANAGER_IS_EDITOR = "SAPPHIRE_Editor";

	//! ����debug���߳��ȵĲ�����
	/** ����
	\code
	SceneManager->getParameters()->setAttribute(scene::DEBUG_NORMAL_LENGTH, 1.5f);
	\endcode
	**/
	const c8* const DEBUG_NORMAL_LENGTH = "DEBUG_Normal_Length";

	//! ����debug���ߵ���ɫ�Ĳ�����
	/** ����
	\code
	SceneManager->getParameters()->setAttributeAsColor(scene::DEBUG_NORMAL_COLOR, ColourValue(255, 255, 255, 255));
	\endcode
	**/
	const c8* const DEBUG_NORMAL_COLOR = "DEBUG_Normal_Color";
}

#endif