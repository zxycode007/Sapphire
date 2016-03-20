#ifndef _SAPPHIRE_IATTRIBUTE_EXCHANGING_OBJECT_
#define _SAPPHIRE_IATTRIBUTE_EXCHANGING_OBJECT_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IAttributes;

	//! 通过SAttributeReadWriteOptions传递给IAttributeExchangingObject对象的枚举标志
	enum E_ATTRIBUTE_READ_WRITE_FLAGS
	{
		//! 对XML文件序列化/反序列化
		EARWF_FOR_FILE = 0x00000001,

		//! 对一个编辑器属性盒子文件序列化/反序列化 
		EARWF_FOR_EDITOR = 0x00000002,

		//! 当写入文件名使用真实路径
		EARWF_USE_RELATIVE_PATHS = 0x00000004
	};

	//! 保存数据描述项的结果
	struct SAttributeReadWriteOptions
	{
		 
		SAttributeReadWriteOptions()
			: Flags(0), Filename(0)
		{
		}

		//! E_ATTRIBUTE_READ_WRITE_FLAGS或其它的组合标志，自定义一个
		SINT32 Flags;

		//! 可选文件名
		 String Filename;
	};

	//! 一个要序列化和反序列化它的属性到一个属性对象的对象
	class IAttributeExchangingObject : virtual public IReferenceCounter
	{
	public:

		//! 写入对象的属性
		/** 实现这个来展示你的场景节点动画器对于脚本语言，编辑器，debugger或xml的属性的序列化*/
		virtual void serializeAttributes( IAttributes* out, SAttributeReadWriteOptions* options = 0) const {}

		//! 读取对象的属性
		/** 实现这个来设置你场景节点动画器对于脚本语言，编辑器，debugger或xml的属性反序列化 */
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0) {}

	};
}

#endif
