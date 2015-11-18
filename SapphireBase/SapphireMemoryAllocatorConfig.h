#ifndef _SAPPHIRE_MEOMORY_ALLOCATOR_CONFIG_
#define _SAPPHIRE_MEOMORY_ALLOCATOR_CONFIG_

#include "SapphireMemoryAllocatedObject.h"


/*

这个文件配置Sapphire所有的内存分配器，你可以为Sapphire主对象修改分配行为

当自定义内存分配器，你需要提供一个或更多的自定义分配策略类，这些类需要以下实现

@code
// 分配字节 - file/line/func 信息可选
static inline void* allocateBytes(size_t count, const char* file = 0, int line = 0, const char* func = 0);
// 释放字节
static inline void deallocateBytes(void* ptr);
// 返回单个分配器中可以分配的最大字节数
static inline size_t getMaxAllocationSize();
@endcode

策略的实现用于调用包装类和宏。 AllocatedObj对一个类覆盖了new和delete操作并且之间直接由策略来引导分配方法
STLAllocator是一个为了挂接这个容器变量的分配器到分配策略,而提供STL容器的类

SAPPHIRE_NEW
用于分配一个用于自定义的new和delete操作的对象来控制自定义的分配，通常这意味着它是由Sapphire：：AllocatedObject派生出来的
释放内存用SAPPHIRE_DELETE, 实际上你也可以用标准的new和delete，但这样不能获取行号等debug信息。
内存目录是由这些派生类自动管理，对于其他的分配器，你都可以指定它

SAPPHIRE_NEW_T
用于分配一个单独的类或结构体，他们不用自定义的new和delete操作，如一个非虚(Vector3,Quaternion)或者来自外部的库（如STL）
如果你希望调用一个析构器，你必须用SAPPHIRE_DELETE_T进行释放。 如果你不需要调用析构器，你必须用SAPPHIRE_FREE来释放内存，
这些宏确定了构造器和析构器能够被正确的调用。 另外注意：当同时分配时，你需要知道类型和内存目录，使得正确的分配器被取得。

SAPPHIRE_NEW_ARRAY_T
和 SAPPHIRE_NEW_T相比用一个额外的参数来构造多个实例到连续的内存。释放用SAPPHIRE_DELETE_ARRAY_T

SAPPHIRE_ALLOC_T
用于安全分配一个基本类型的集合，有时你需要高效的分配一块不用迭代每个元素构造/析构器的内存。
释放用SAPPHIRE_FREE

SAPPHIRE_MALLOC
这是最原始的分配的方法，如一个字节的集合，用SAPPHIRE_FREE来释放

例子
@code
/// AllocatedObject 之类, 用自定义的new和delete
AllocatedClass* obj = SAPPHIRE_NEW AllocatedClass();
SAPPHIRE_DELETE obj;
AllocatedClass* array = SAPPHIRE_NEW AllocatedClass[10];
SAPPHIRE_DELETE [] obj;
/// 非虚或者外部类 需要调用构造/析构函数
ExternalClass* obj = SAPPHIRE_NEW_T(ExternalClass, MEMCATEGORY_GENERAL)(constructorArgs);
SAPPHIRE_DELETE_T(obj, ExternalClass, MEMCATEGORY_GENERAL);
ExternalClass* obj = SAPPHIRE_NEW_ARRAY_T(ExternalClass, 10, MEMCATEGORY_GENERAL);
SAPPHIRE_DELETE_ARRAY_T(obj, NonVirtualClass, 10, MEMCATEGORY_GENERAL);
/// 基本类型
long* pLong = SAPPHIRE_ALLOC_T(long, 10, MEMCATEGORY_GENERAL);
SAPPHIRE_FREE(pLong, MEMCATEGORY_GENERAL);
/// 使用构造器的基本类型Primitive type with constructor (由于没有析构函数，你可以不匹配SAPPHIRE_NEW_T 和 SAPPHIRE_FREE)
long* pLong = SAPPHIRE_NEW_T(long, MEMCATEGORY_GENERAL)(0);
SAPPHIRE_FREE(pLong, MEMCATEGORY_GENERAL);
/// 原始内存
void* pVoid = SAPPHIRE_MALLOC(1024, MEMCATEGORY_GENERAL);
SAPPHIRE_FREE(pVoid, MEMCATEGORY_GENERAL);
@endcode
SAPPHIRE_ALLOC_T 常用于基本类型像int和float的分配方法，你可以使用SAPPHIRE_FREE来进行释放，SIMD变量和自定义对齐的变量


*/





namespace Sapphire
{

	/**
	指向一块要进行分配内存的目的的集合种类
	这个目录将在分配时提供分配策略的不同行为。这允许你用一单个策略，而仍然可以有不同的行为。
	除了不同的策略到不同的类外，控制级别还让你为分配更高的级别
	*/
	enum MemoryCategory
	{
		/// 一般应用
		MEMCATEGORY_GENERAL = 0,
		/// 保存在主内存中的几何体
		MEMCATEGORY_GEOMETRY = 1,
		/// 用于动画数据，像跟踪，骨骼矩阵
		MEMCATEGORY_ANIMATION = 2,
		/// 节点，控制数据
		MEMCATEGORY_SCENE_CONTROL = 3,
		/// 场景对象实例
		MEMCATEGORY_SCENE_OBJECTS = 4,
		/// 其它资源
		MEMCATEGORY_RESOURCE = 5,
		/// 脚本
		MEMCATEGORY_SCRIPTING = 6,
		/// 渲染系统结构
		MEMCATEGORY_RENDERSYS = 7,


		//不用，计数值
		MEMCATEGORY_COUNT = 8
	};

}

#include "SapphireMemorySTLAllocator.h"

#if SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_NEDPOOLING       //判断内存分配器采用的类型NEDPOOLING

//如果是ned 内存池，那么包含相关的头文件
#include "SapphireMemoryNedPooling.h"
namespace Sapphire
{
	// 配置基于这个选项默认的分配器
	// 注意我们在这里使用这个内存种类，但是仍然将它输入到你分配器中，你可以选择为不同内存用途选择不同的分配策略

	// 配置种类，对于通用的分配内存
	// 注意这里种类是无效的
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public NedPoolingPolicy{};   //创建内存种类分配策略 模板类 CategorisedAllocPolicy 继承至Ned池策略NedPoolingPolicy
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public NedPoolingAlignedPolicy<align>{};  //带数据字节对齐的
}

#elif SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_NED

//#  include "SapphireMemoryNedAlloc.h"
//namespace Sapphire
//{
//	// 配置基于这个选项默认的分配器
//	// 注意我们在这里使用这个内存种类，但是仍然将它输入到你分配器中，你可以选择为不同内存用途选择不同的分配策略

	// 配置种类，对于通用的分配内存
	// 注意这里种类是无效的
//	template <MemoryCategory Cat> class CategorisedAllocPolicy : public NedAllocPolicy{};  //创建内存种类分配策略 模板类 CategorisedAllocPolicy 继承至Ned策略NedAllocPolicy
//	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public NedAlignedAllocPolicy<align>{};
//}

#  include "SapphireMemoryStdAlloc.h"
namespace Sapphire
{
	// 配置基于这个选项默认的分配器
	// 注意我们在这里使用这个内存种类，但是仍然将它输入到你分配器中，你可以选择为不同内存用途选择不同的分配策略

	// 配置种类，对于通用的分配内存
	// 注意这里种类是无效的
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public StdAllocPolicy{};
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public StdAlignedAllocPolicy < align > {};

	// 如果要为每个内存分类制定分配，这儿你做
	// template <> class CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> : public YourSceneObjectAllocPolicy{};
	// template <size_t align> class CategorisedAlignAllocPolicy<MEMCATEGORY_SCENE_OBJECTS, align> : public YourSceneObjectAllocPolicy<align>{};


}

#elif SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_STD

#  include "SapphireMemoryStdAlloc.h"
namespace Sapphire
{
	// 配置基于这个选项默认的分配器
	// 注意我们在这里使用这个内存种类，但是仍然将它输入到你分配器中，你可以选择为不同内存用途选择不同的分配策略

	// 配置种类，对于通用的分配内存
	// 注意这里种类是无效的
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public StdAllocPolicy{};  //标准分配策略,若要对不同类型对象，进行不同分配内存分配策略定义如下
	//template <> class CategorisedAllocPolicy<MEMCATEGORY_TYPE> : public YouCustomAllocPolicy{};
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public StdAlignedAllocPolicy < align > {};

	// 如果要为每个内存分类制定分配，这儿你做
	// template <> class CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> : public YourSceneObjectAllocPolicy{};
	// template <size_t align> class CategorisedAlignAllocPolicy<MEMCATEGORY_SCENE_OBJECTS, align> : public YourSceneObjectAllocPolicy<align>{};


}

#else

// 定义你自建的分配器

#endif

namespace Sapphire
{
	// 相应的分配类型策略快捷方式
	typedef CategorisedAllocPolicy<MEMCATEGORY_GENERAL> GeneralAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_GEOMETRY> GeometryAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_ANIMATION> AnimationAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCENE_CONTROL> SceneCtlAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> SceneObjAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_RESOURCE> ResourceAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCRIPTING> ScriptingAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_RENDERSYS> RenderSysAllocPolicy;

	// 现在为每个分配工作定义所有的基类
	typedef AllocatedObj<GeneralAllocPolicy> GeneralAllocatedObject;
	typedef AllocatedObj<GeometryAllocPolicy> GeometryAllocatedObject;
	typedef AllocatedObj<AnimationAllocPolicy> AnimationAllocatedObject;
	typedef AllocatedObj<SceneCtlAllocPolicy> SceneCtlAllocatedObject;
	typedef AllocatedObj<SceneObjAllocPolicy> SceneObjAllocatedObject;
	typedef AllocatedObj<ResourceAllocPolicy> ResourceAllocatedObject;
	typedef AllocatedObj<ScriptingAllocPolicy> ScriptingAllocatedObject;
	typedef AllocatedObj<RenderSysAllocPolicy> RenderSysAllocatedObject;


	// 定义每个分配器类
	// NOTE: 小，非虚类 不能作为分配器的子类
	// 使用原或者STL分配器/反分配器
	typedef ScriptingAllocatedObject	AbstractNodeAlloc;
	typedef AnimationAllocatedObject	AnimableAlloc;
	typedef AnimationAllocatedObject	AnimationAlloc;
	typedef GeneralAllocatedObject		ArchiveAlloc;
	typedef GeometryAllocatedObject		BatchedGeometryAlloc;
	typedef RenderSysAllocatedObject	BufferAlloc;
	typedef GeneralAllocatedObject		CodecAlloc;
	typedef ResourceAllocatedObject		CompositorInstAlloc;
	typedef GeneralAllocatedObject		ConfigAlloc;
	typedef GeneralAllocatedObject		ControllerAlloc;
	typedef GeometryAllocatedObject		DebugGeomAlloc;
	typedef GeneralAllocatedObject		DynLibAlloc;
	typedef GeometryAllocatedObject		EdgeDataAlloc;
	typedef GeneralAllocatedObject		FactoryAlloc;
	typedef SceneObjAllocatedObject		FXAlloc;
	typedef GeneralAllocatedObject		ImageAlloc;
	typedef GeometryAllocatedObject		IndexDataAlloc;
	typedef GeneralAllocatedObject		LogAlloc;
	typedef SceneObjAllocatedObject		MovableAlloc;
	typedef SceneCtlAllocatedObject		NodeAlloc;
	typedef SceneObjAllocatedObject		OverlayAlloc;
	typedef RenderSysAllocatedObject	GpuParamsAlloc;
	typedef ResourceAllocatedObject		PassAlloc;
	typedef GeometryAllocatedObject		PatchAlloc;
	typedef GeneralAllocatedObject		PluginAlloc;
	typedef GeneralAllocatedObject		ProfilerAlloc;
	typedef GeometryAllocatedObject		ProgMeshAlloc;
	typedef SceneCtlAllocatedObject		RenderQueueAlloc;
	typedef RenderSysAllocatedObject	RenderSysAlloc;
	typedef GeneralAllocatedObject		RootAlloc;
	typedef ResourceAllocatedObject		ResourceAlloc;
	typedef GeneralAllocatedObject		SerializerAlloc;
	typedef SceneCtlAllocatedObject		SceneMgtAlloc;
	typedef ScriptingAllocatedObject    ScriptCompilerAlloc;
	typedef ScriptingAllocatedObject    ScriptTranslatorAlloc;
	typedef SceneCtlAllocatedObject		ShadowDataAlloc;
	typedef GeneralAllocatedObject		StreamAlloc;
	typedef SceneObjAllocatedObject		SubEntityAlloc;
	typedef ResourceAllocatedObject		SubMeshAlloc;
	typedef ResourceAllocatedObject		TechniqueAlloc;
	typedef GeneralAllocatedObject		TimerAlloc;
	typedef ResourceAllocatedObject		TextureUnitStateAlloc;
	typedef GeneralAllocatedObject		UtilityAlloc;
	typedef GeometryAllocatedObject		VertexDataAlloc;
	typedef RenderSysAllocatedObject	ViewportAlloc;
	typedef SceneCtlAllocatedObject		LodAlloc;
	typedef GeneralAllocatedObject		SceneAlloc;
	typedef GeneralAllocatedObject		VertexAlloc;

	// 容器
	// 由typedef STLAllocator<T, DefaultAllocPolicy, Category> TAlloc;定义
	// 在 vector<T, TAlloc>::type 中使用



}

//工具函数
namespace Sapphire
{


	/** 构造一个placement new 的对象数组的函数,实际是单个对象而并非使用new[]创建真实数组，这个扩展分配一个非法的额外连续内存，所以它不能用于自定义的分配器
	*/
	template<typename T>
	T* constructN(T* basePtr, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			new ((void*)(basePtr + i)) T();
		}
		return basePtr;
	}


}


#if SAPPHIRE_DEBUG_MODE      //DEBUG模式开启

/// 直接分配一个raw内存块，并且指示用途种类
#	define SAPPHIRE_MALLOC(bytes, category) ::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// 直接分配个基本类型的内存块，并指示其用途
#	define SAPPHIRE_ALLOC_T(T, count, category) static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// 释放由SAPPHIRE_MALLOC 或 SAPPHIRE_ALLOC_T 分配的内存，需要确认其用途对应的策略
#	define SAPPHIRE_FREE(ptr, category) ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)

/// 用placement new分配一个基本类型，扩展类型或者构造器参数用非虚的类型
#	define SAPPHIRE_NEW_T(T, category) new (::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// 分配一个count大小的基本类型的内存块， Sapphire::constructN不能用以继承至AllocatedObj的类
#	define SAPPHIRE_NEW_ARRAY_T(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// 释放由SAPPHIRE_NEW_T分配的内存
#	define SAPPHIRE_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
/// 释放SAPPHIRE_NEW_ARRAY_T分配的内存
#	define SAPPHIRE_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

//对齐分配
/// 分配一个对齐到SIMD边界的RAW内存块,并且指出它的使用类型
#	define SAPPHIRE_MALLOC_SIMD(bytes, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)  //align 默认=0
/// 分配一个raw内存块对齐用户定义的边界，并且指示出它使用的类型
#	define SAPPHIRE_MALLOC_ALIGN(bytes, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// 分配一个对齐到SIMD边界的基本类型的内存块，并且指示它的使用类型
#	define SAPPHIRE_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)) //align 默认=0
///分配一个对齐到用户定义边界的基本类型的内存块，并且指示它的使用类型
#	define SAPPHIRE_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// 释放SAPPHIRE_MALLOC_SIMD SAPPHIRE_ALLOC_T_SIMD分配的内存，使用种类是要说明策略必须的
#	define SAPPHIRE_FREE_SIMD(ptr, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr)
/// 释放SAPPHIRE_MALLOC_ALIGN SAPPHIRE_ALLOC_T_ALIGN  分配的内存，使用种类是要说明策略必须的
#	define SAPPHIRE_FREE_ALIGN(ptr, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr)

/// 用placement new 分配一个基本类型， 额外的类型或者非虚拟类型对齐到SIMD边界
#	define SAPPHIRE_NEW_T_SIMD(T, category) new (::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// 分配指定数量的对齐到SIMD边界基本类型的一个内存块， Sapphire::constructN不能使用于继承AllocatedObj的类
#	define SAPPHIRE_NEW_ARRAY_T_SIMD(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// 释放有SAPPHIRE_NEW_T_SIMD分配的内存
#	define SAPPHIRE_DELETE_T_SIMD(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}
/// 释放由SAPPHIRE_NEW_ARRAY_T_SIMD 分配的一系列内存， 依次调用析构函数
#	define SAPPHIRE_DELETE_ARRAY_T_SIMD(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}
/// 用placement new分配一个基本类型，额外类型或非虚类型并对齐用户的定义边界
#	define SAPPHIRE_NEW_T_ALIGN(T, category, align) new (::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// 分配一定数量对齐到用户定义边界的一块内存块， Sapphire::constructN不能用于基础至AllocatedObj的类
#	define SAPPHIRE_NEW_ARRAY_T_ALIGN(T, count, category, align) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// 释放由SAPPHIRE_NEW_T_ALIGN 分配的内存
#	define SAPPHIRE_DELETE_T_ALIGN(ptr, T, category, align) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}
///释放由SAPPHIRE_NEW_ARRAY_T_ALIGN分配的内存，并依次调用析构函数
#	define SAPPHIRE_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align) if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}

// new / delete 对于派生与AllocatedObj的类 （对齐由每个类策略决定）可以取得文件/行号/函数名的参数，派生与AllocatedObj的类只能使用自定义的new/delete
//用于AllocatedObj重载了new 和 delete
#	define SAPPHIRE_NEW new (__FILE__, __LINE__, __FUNCTION__)
#	define SAPPHIRE_DELETE delete


#else // Release模式


#	define SAPPHIRE_MALLOC(bytes, category) ::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(bytes)

#	define SAPPHIRE_ALLOC_T(T, count, category) static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_FREE(ptr, category) ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)


#	define SAPPHIRE_NEW_T(T, category) new (::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}


#	define SAPPHIRE_MALLOC_SIMD(bytes, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes)

#	define SAPPHIRE_MALLOC_ALIGN(bytes, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes)

#	define SAPPHIRE_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_FREE_SIMD(ptr, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr)

#	define SAPPHIRE_FREE_ALIGN(ptr, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr)


#	define SAPPHIRE_NEW_T_SIMD(T, category) new (::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T_SIMD(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T_SIMD(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T_SIMD(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_NEW_T_ALIGN(T, category, align) new (::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T_ALIGN(T, count, category, align) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T_ALIGN(ptr, T, category, align) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align) if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}


#	define SAPPHIRE_NEW new 
#	define SAPPHIRE_DELETE delete

#endif  


namespace Sapphire
{
	/**
	这个函数对一个给定的指针请求SAPPHIRE_DELETE
	*/
	template<typename T>
	void deletePtr(T* ptr)
	{
		SAPPHIRE_DELETE ptr;
	}
}






#endif