/*

    这个头文件定义了获取关于CPU操作系统和编译器的相关信息

*/

#ifndef __SAPPHIRE_PLATFROM__
#define __SAPPHIRE_PLATFROM__

#include "SapphireBuildSets.h"

namespace Sapphire
{

	/*
	CPU的类型
	*/
#define SAPPHIRE_CPU_UNKNOWN    0
#define SAPPHIRE_CPU_X86        1
#define SAPPHIRE_CPU_ARM        3

	//各个平台的定义
#define SAPPHIRE_PLATFORM_WIN32 1
#define SAPPHIRE_PLATFORM_LINUX 2
#define SAPPHIRE_PLATFORM_APPLE 3
#define SAPPHIRE_PLATFORM_APPLE_IOS 4
#define SAPPHIRE_PLATFORM_ANDROID 5
#define SAPPHIRE_PLATFORM_SDL 6
#define SAPPHIRE_PLATFORM_X11 7
#define SAPPHIRE_PLATFORM_X11 7
#define SAPPHIRE_POSIX_API 8
#define SAPPHIRE_WINDOWS_CE_PLATFORM 9



	//各个编译器定义
	//msvc编译器
#define SAPPHIRE_COMPILER_MSVC 1
	//gcc编译器
#define SAPPHIRE_COMPILER_GNUC 2
	//针对ARM的GCCE嵌入式编译器
#define SAPPHIRE_COMPILER_GCCE 5
	//针对OBJECTIVE C 的clang编译器
#define SAPPHIRE_COMPILER_CLANG 6

	//大字节序和小字节序
#define SAPPHIRE_ENDIAN_LITTLE 1
#define SAPPHIRE_ENDIAN_BIG 2

	//32位和64位架构
#define SAPPHIRE_ARCHITECTURE_32 1
#define SAPPHIRE_ARCHITECTURE_64 2


	/*
	判断当前编译器的类型和版本
	*/
#if defined( __GCCE__ )
#   define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_GCCE
#   define SAPPHIRE_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_MSVC
#   define SAPPHIRE_COMP_VER _MSC_VER
#elif defined( __clang__ )
#   define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_CLANG
#   define SAPPHIRE_COMP_VER (((__clang_major__)*100) + (__clang_minor__*10) + __clang_patchlevel__)        
#elif defined( __GNUC__ )
#   define SAPPHIRE_COMPILER SAPPHIRE_COMPILER_GNUC
#   define SAPPHIRE_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10)  + __GNUC_PATCHLEVEL__)        
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

	//定义强制内联FORCEINLINE宏
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
#   if SAPPHIRE_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#define swprintf swprintf_s
#define snprintf sprintf_s
#elif !defined(__CYGWIN__)
#define swprintf _snwprintf
#define snprintf _snprintf
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif


	/* 判断当前平台 */
	//win32平台
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_WIN32
	//苹果平台
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_APPLE_IOS
#   else
#       define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_APPLE
#   endif
	// ANDROID平台
#elif defined(__ANDROID__)
#	define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_ANDROID
#else
#   define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_LINUX
#endif



	/* 判断架构32/64位 */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_64
#else
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_32
#endif


	// Windows设置
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
	//静态库无需特殊
#	if defined( SAPPHIRE_STATIC_LIB )
#	   define _SapphireExport
#      define _SapphirePrivate
#	else
	//动态库 dll
#	   if defined( SAPPHIRE_NONCLIENT_BUILD )
#       	define _SapphireExport __declspec( dllexport )
#   	else
	//mingw不支持
#           if defined( __MINGW32__ )
#               define _SapphireExport
#           else
	//MSVC才支持
#       	    define _SapphireExport __declspec( dllimport )
#           endif
#   	endif
	//windows 动态库默认就是local  需要globe的才需__declspec( dllexport )
#	define _SapphirePrivate
#endif
	// Win32用 _DEBUG
	// MinGW用  DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define SAPPHIRE_DEBUG_MODE 1
#   else
#       define SAPPHIRE_DEBUG_MODE 0
#   endif

#endif


	// Linux/Apple/iOs/Android
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_LINUX || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE_IOS || \
    SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_ANDROID

	// GCC 全局和本地标志
#   if defined( SAPPHIRE_GCC_VISIBILITY )
	//gcc 默认是globe，这点和windows不一样
#       define _SapphireExport  __attribute__ ((visibility("default")))
	//声明为local
#       define _SapphirePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _SapphireExport
#       define _SapphirePrivate
#   endif

	//windows下stricmp， linux下为strcasecmp
#   define stricmp strcasecmp
	//  定义debug标志
#   ifdef DEBUG
#       define SAPPHIRE_DEBUG_MODE 1
#   else
#       define SAPPHIRE_DEBUG_MODE 0
#   endif

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#define SAPPHIRE_PLATFORM_LIB "SapphirePlatform.bundle"
#elif SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE_IOS
#define SAPPHIRE_PLATFORM_LIB "SapphirePlatform.a"
#else //SAPPHIRE_PLATFORM_LINUX
#define SAPPHIRE_PLATFORM_LIB "libSapphirePlatform.so"
#endif
#endif




	// 字节序的设置
#ifdef SAPPHIRE_CONFIG_BIG_ENDIAN
#    define SAPPHIRE_ENDIAN SAPPHIRE_ENDIAN_BIG
#else
#    define SAPPHIRE_ENDIAN SAPPHIRE_ENDIAN_LITTLE
#endif

	//一直打开对UNICODE的编码的支持
#define SAPPHIRE_UNICODE_SUPPORT 1;

	// 整形数据统一标明数据位宽度
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef int int32;
	typedef short int16;
	typedef char int8;
	typedef float float32;
	typedef double float64;
	//定义64位整形
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#endif


}


#endif