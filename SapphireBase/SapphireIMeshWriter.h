#ifndef _SAPPHIRE_IMESH_WRITER_
#define _SAPPHIRE_IMESH_WRITER_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireEMeshWriterEnums.h"


namespace Sapphire
{
	class IWriteFile;
	class IMesh;

	//! 写入网格的接口
	class IMeshWriter : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshWriter() {}

		//! 获取这个mesh writer的类型
		/** 对于自己的实现，使用MAKE_IRR_ID表示EMESH_WRITER_TYPE枚举返回你唯一网格类型id
		type id.
		\return mesh writer类型ID */
		virtual EMESH_WRITER_TYPE getType() const = 0;

		//! 写一个静态网格
		/** \param file 要写入网格的文件句柄
		\param mesh 指向要写入网格的指针
		\param flags 设置writer属性的可选标志
		\return 如果成功返回true */
		virtual bool writeMesh(IWriteFile* file, IMesh* mesh,
			SINT32 flags = EMWF_NONE) = 0;

		// 写一个动画网格
		// 对于以后的使用，没有writer可以写入人当前动画网格
		/* \return 如果成功返回true */
		//virtual bool writeAnimatedMesh(io::IWriteFile* file,
		// scene::IAnimatedMesh* mesh,
		// s32 flags=EMWF_NONE) = 0;
	};
}

#endif