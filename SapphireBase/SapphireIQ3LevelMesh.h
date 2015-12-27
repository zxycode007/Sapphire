#ifndef _SAPPHIRE_IQ3LevelMesh_
#define _SAPPHIRE_IQ3LevelMesh_

#include "SapphirePrerequisites.h"
#include "SapphireIQ3Shader.h"

namespace Sapphire
{
	namespace Quake3
	{
		//! 可以从Quake3.bsp文件直接加载网格的接口
		/** 这个网格会尝试加载索引的纹理映射*/
		class IQ3LevelMesh : public IAnimatedMesh
		{
		public:

			//! 从文件加载shader定义
			/** \param filename shader文件的名字， 如果fileNameIsValid为false，默认为/scripts
			\param fileNameIsValid 指定在当前情况下的文件名是否有效*/
			virtual const Quake3::IShader* getShader(const c8* filename, bool fileNameIsValid = true) = 0;

			//! 返回一个已经加载的shader
			virtual const Quake3::IShader* getShader(UINT32 index) const = 0;

			//! 获取一个到Entity列表的接口
			virtual Quake3::tQ3EntityList& getEntityList() = 0;

			//! 返回这个请求的画刷实体
			/** \param num 实体的模型键的数量

			如果你要解析自己的实体，用这个接口*/
			virtual IMesh* getBrushEntityMesh(SINT32 num) const = 0;

			//! 返回请求的画刷实体
			virtual IMesh* getBrushEntityMesh(Quake3::IEntity &ent) const = 0;
		};
	}
}

#endif