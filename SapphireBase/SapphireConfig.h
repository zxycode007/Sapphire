#ifndef __SAPPHIRE_CONFIG__
#define __SAPPHIRE_CONFIG__

#include "SapphireBuildSets.h"


#define SAPPHIRE_DOUBLE_PRECISION 1     //1为双精度 0为单精度

#ifndef SAPPHIRE_WCHAR_T_STRINGS
   #define SAPPHIRE_WCHAR_T_STRINGS 0    //定义是否使用WCHAR
#endif

// 定义要使用的内存分配器类型
#define SAPPHIRE_MEMORY_ALLOCATOR_STD 1   //标准
#define SAPPHIRE_MEMORY_ALLOCATOR_NED 2    //NED
#define SAPPHIRE_MEMORY_ALLOCATOR_USER 3     //用户自定义
#define SAPPHIRE_MEMORY_ALLOCATOR_NEDPOOLING 4      //ned内存池

#ifndef SAPPHIRE_MEMORY_ALLOCATOR
#  define SAPPHIRE_MEMORY_ALLOCATOR SAPPHIRE_MEMORY_ALLOCATOR_STD      //定义内存分配器
#endif

// 定义是否在STL容器中使用自定义的内存分配器
#ifndef SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
#  define SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR 1
#endif

//如果你要使用Sapphire：：string用自定义的内存分配器，那么需要这样设置
//#define SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 1
// 这样做会让Sapphire的字符串将不会与std：：string兼容
#ifndef SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR
#	define SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 1
#endif

// 打开关闭内存跟踪模式，记录内存的分配，跟踪泄露
//默认关闭，因为会有额外开销，但如果打开须有构建每个目标
#ifndef SAPPHIRE_MEMORY_TRACKER_DEBUG_MODE
#  define SAPPHIRE_MEMORY_TRACKER_DEBUG_MODE 0
#endif

#ifndef SAPPHIRE_MEMORY_TRACKER_RELEASE_MODE
#  define SAPPHIRE_MEMORY_TRACKER_RELEASE_MODE 0
#endif

//定义多重纹理能支持的最大纹理层数
#ifndef SAPPHIRE_MAX_TEXTURE_LAYERS
#define SAPPHIRE_MAX_TEXTURE_LAYERS  16
#endif


/**
多线程支持
SAPPHIRE_THREAD_SUPPORT = 0
不支持线程
SAPPHIRE_THREAD_SUPPORT = 1
支持后台线程加载，有一个线程同时加载和构建资源。 资源管理和SharePtr 句柄是线程安全的，
资源可以再后台完成加载。线程是有效可行的， 除法你可以假定这个标志的状态不是线程安全的。
SAPPHIRE_THREAD_SUPPORT = 2
线程支持后台资源预处理，这意味着资源数据可以再后台以数据流形式输入内存，但是资源的构建（被渲染系统包含）仍然是
由主线程来完成。同步原语开销比较低主要的瓶颈还是在于后台I/O
*/
#ifndef SAPPHIRE_THREAD_SUPPORT
#define SAPPHIRE_THREAD_SUPPORT 0
#endif
#if SAPPHIRE_THREAD_SUPPORT != 0 && SAPPHIRE_THREAD_SUPPORT != 1 && SAPPHIRE_THREAD_SUPPORT != 2
#define SAPPHIRE_THREAD_SUPPORT 0
#endif

/**
线程方法提供者，四个选项
SAPPHIRE_THREAD_PROVIDER = 0
不使用线程.
SAPPHIRE_THREAD_PROVIDER = 1
Boost库线程
SAPPHIRE_THREAD_PROVIDER = 2
Poco库线程
SAPPHIRE_THREAD_PROVIDER = 3
TBB库线程
*/
#ifndef SAPPHIRE_THREAD_PROVIDER
#define SAPPHIRE_THREAD_PROVIDER 0
#endif

namespace Sapphire
{


}
#endif