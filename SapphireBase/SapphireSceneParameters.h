#ifndef _SAPPHIRE_SCENE_PARAMETERS_
#define _SAPPHIRE_SCENE_PARAMETERS_

#include "SapphirePrerequisites.h"

/*! \ SceneParameters.h
\包括对于修改网格加载的所有场景参数的头文件

这文件包括所有参数名，这些可用ISceneManager::getParameters()设置修改插件和网格加载器的行为
*/

namespace Sapphire
{
	//! 对于如何改变对于透明（混合）材质的ZWrite 标志的控制的参数名
	/** 默认情况下是对于所有真实透明是关闭写入z缓冲，混合材质。这避免了相交面带来的问题，
	但是也破坏了渲染。如果透明材质对于ZWriteEnable使用SMaterial标志，正如别的材质类型使用这属性
	例如
	SceneManager->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true);
	**/
	const c8* const ALLOW_ZWRITE_ON_TRANSPARENT = "Allow_ZWrite_On_Transparent";

	//! 改变内置csm加载器的纹理路径的参数名
	/**例如
	SceneManager->getParameters()->setAttribute(scene::CSM_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const CSM_TEXTURE_PATH = "CSM_TexturePath";

	//! 改变内置lmts加载器的纹理路径
	/** 例如
	SceneManager->getParameters()->setAttribute(scene::LMTS_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const LMTS_TEXTURE_PATH = "LMTS_TexturePath";

	//! 改变内置my3d加载器的纹理路径 
	/** 例如
	SceneManager->getParameters()->setAttribute(scene::MY3D_TEXTURE_PATH, "path/to/your/textures");
	**/
	const c8* const MY3D_TEXTURE_PATH = "MY3D_TexturePath";

	//! 指定COLLADA 网格加载模式的参数名
	/**
	指定COLLADA加载器在加载COLLADA网格时是否应该同时创建模型的实例，光源和相机。
	默认，设置为false。如果设置为true， ISceneManager::getMesh()方法将只返回一个指向假网格并且创建在collada文件中所有网格和
	光源和相机的实例
	例如
	\code
	SceneManager->getParameters()->setAttribute(scene::COLLADA_CREATE_SCENE_INSTANCES, true);
	\endcode
	*/
	const c8* const COLLADA_CREATE_SCENE_INSTANCES = "COLLADA_CreateSceneInstances";

	//! 改变内置DMF加载器的纹理路径的参数名
	/** 在当加载纹理时，这个路径作为定义在deled文件的文件名前缀，进行查找。
	这允许对于一个指定的工程设置修改器路径。
	例如：
	\code
	SceneManager->getStringParameters()->setAttribute(scene::DMF_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const DMF_TEXTURE_PATH = "DMF_TexturePath";

	//! 对于在内建的DMF加载器保留DMF纹理路径结构的参数名
	/** 如果这个参数设为true, 定义在deled文件的纹理路径无效， 并且只用纹理名来查找对应的文件.
	否则，这个纹理路径也可以被使用，它允许使用一个更好的媒体布局.
	例如：
	Otherwise, the
	texture path is also used, which allows to use a nicer media layout.
	Use it like this:
	\code
	//这是默认，不需要使用这个设置
	SceneManager->getParameters()->setAttribute(scene::DMF_IGNORE_MATERIALS_DIRS, false);
	\endcode
	\code
	//需要使用时
	SceneManager->getParameters()->setAttribute(scene::DMF_IGNORE_MATERIALS_DIRS, true);
	\endcode
	**/
	const c8* const DMF_IGNORE_MATERIALS_DIRS = "DMF_IgnoreMaterialsDir";

	//! 对应设置在透明材质中的alpha的引用值
	/** 
	\code
	//设置alpha 引用值 到 0.1
	SceneManager->getParameters()->setAttribute(scene::DMF_ALPHA_CHANNEL_REF, 0.1);
	\endcode
	**/
	const c8* const DMF_ALPHA_CHANNEL_REF = "DMF_AlphaRef";

	//! 对应旋转翻转或不翻转tga文件的参数名
	/** 
	\code
	//例如：翻转alpha纹理
	SceneManager->getParameters()->setAttribute(scene::DMF_FLIP_ALPHA_TEXTURES, true);
	\endcode
	**/
	const c8* const DMF_FLIP_ALPHA_TEXTURES = "DMF_FlipAlpha";


	//! 改变内置OBJ加载器的纹理路径的参数名
	/** 例如
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const OBJ_TEXTURE_PATH = "OBJ_TexturePath";

	//! 避免加载在Obj文件中的组结构
	/** 例如
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_LOADER_IGNORE_GROUPS, true);
	\endcode
	**/
	const c8* const OBJ_LOADER_IGNORE_GROUPS = "OBJ_IgnoreGroups";


	//! 避免加载obj文件的材质.mtl文件
	/** 例如
	\code
	SceneManager->getParameters()->setAttribute(scene::OBJ_LOADER_IGNORE_MATERIAL_FILES, true);
	\endcode
	**/
	const c8* const OBJ_LOADER_IGNORE_MATERIAL_FILES = "OBJ_IgnoreMaterialFiles";


	//! 无效化b3d文件的mipmap标志
	/** 用sapphire的纹理创建标准替代，例如：
	\code
	SceneManager->getParameters()->setAttribute(scene::B3D_LOADER_IGNORE_MIPMAP_FLAG, true);
	\endcode
	**/
	const c8* const B3D_LOADER_IGNORE_MIPMAP_FLAG = "B3D_IgnoreMipmapFlag";

	//! 改变内建b3d加载器纹理路径的参数名
	/** 例如
	\code
	SceneManager->getParameters()->setAttribute(scene::B3D_TEXTURE_PATH, "path/to/your/textures");
	\endcode
	**/
	const c8* const B3D_TEXTURE_PATH = "B3D_TexturePath";

	//! 当scene manager 作为编辑器使用时，设置这个标志
	/** 特殊的animator喜欢这种删除animator的方式,例如会阻止删除场景节点.
	  */
	const c8* const SAPPHIRE_SCENE_MANAGER_IS_EDITOR = "SAPPHIRE_Editor";

	//! 设置debug法线长度的参数名
	/** 例如
	\code
	SceneManager->getParameters()->setAttribute(scene::DEBUG_NORMAL_LENGTH, 1.5f);
	\endcode
	**/
	const c8* const DEBUG_NORMAL_LENGTH = "DEBUG_Normal_Length";

	//! 设置debug法线的颜色的参数名
	/** 例如
	\code
	SceneManager->getParameters()->setAttributeAsColor(scene::DEBUG_NORMAL_COLOR, ColourValue(255, 255, 255, 255));
	\endcode
	**/
	const c8* const DEBUG_NORMAL_COLOR = "DEBUG_Normal_Color";
}

#endif