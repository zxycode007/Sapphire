/*

    ���ͷ�ļ������˻�ȡ����CPU����ϵͳ�ͱ������������Ϣ

*/

#ifndef __SAPPHIRE_PLATFROM__
#define __SAPPHIRE_PLATFROM__

#include "SapphireBuildSets.h"

namespace Sapphire
{

	/*
	CPU������
	*/
#define SAPPHIRE_CPU_UNKNOWN    0
#define SAPPHIRE_CPU_X86        1
#define SAPPHIRE_CPU_ARM        3

	//����ƽ̨�Ķ���
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



	//��������������
	//msvc������
#define SAPPHIRE_COMPILER_MSVC 1
	//gcc������
#define SAPPHIRE_COMPILER_GNUC 2
	//���ARM��GCCEǶ��ʽ������
#define SAPPHIRE_COMPILER_GCCE 5
	//���OBJECTIVE C ��clang������
#define SAPPHIRE_COMPILER_CLANG 6

	//���ֽ����С�ֽ���
#define SAPPHIRE_ENDIAN_LITTLE 1
#define SAPPHIRE_ENDIAN_BIG 2

	//32λ��64λ�ܹ�
#define SAPPHIRE_ARCHITECTURE_32 1
#define SAPPHIRE_ARCHITECTURE_64 2


	/*
	�жϵ�ǰ�����������ͺͰ汾
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

	//����ǿ������FORCEINLINE��
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


	/* �жϵ�ǰƽ̨ */
	//win32ƽ̨
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_WIN32
	//ƻ��ƽ̨
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_APPLE_IOS
#   else
#       define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_APPLE
#   endif
	// ANDROIDƽ̨
#elif defined(__ANDROID__)
#	define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_ANDROID
#else
#   define SAPPHIRE_PLATFORM SAPPHIRE_PLATFORM_LINUX
#endif



	/* �жϼܹ�32/64λ */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_64
#else
#   define SAPPHIRE_ARCH_TYPE SAPPHIRE_ARCHITECTURE_32
#endif


	// Windows����
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
	//��̬����������
#	if defined( SAPPHIRE_STATIC_LIB )
#	   define _SapphireExport
#      define _SapphirePrivate
#	else
	//��̬�� dll
#	   if defined( SAPPHIRE_NONCLIENT_BUILD )
#       	define _SapphireExport __declspec( dllexport )
#   	else
	//mingw��֧��
#           if defined( __MINGW32__ )
#               define _SapphireExport
#           else
	//MSVC��֧��
#       	    define _SapphireExport __declspec( dllimport )
#           endif
#   	endif
	//windows ��̬��Ĭ�Ͼ���local  ��Ҫglobe�Ĳ���__declspec( dllexport )
#	define _SapphirePrivate
#endif
	// Win32�� _DEBUG
	// MinGW��  DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define SAPPHIRE_DEBUG_MODE 1
#   else
#       define SAPPHIRE_DEBUG_MODE 0
#   endif

#endif


	// Linux/Apple/iOs/Android
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_LINUX || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE_IOS || \
    SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_ANDROID

	// GCC ȫ�ֺͱ��ر�־
#   if defined( SAPPHIRE_GCC_VISIBILITY )
	//gcc Ĭ����globe������windows��һ��
#       define _SapphireExport  __attribute__ ((visibility("default")))
	//����Ϊlocal
#       define _SapphirePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _SapphireExport
#       define _SapphirePrivate
#   endif

	//windows��stricmp�� linux��Ϊstrcasecmp
#   define stricmp strcasecmp
	//  ����debug��־
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




	// �ֽ��������
#ifdef SAPPHIRE_CONFIG_BIG_ENDIAN
#    define SAPPHIRE_ENDIAN SAPPHIRE_ENDIAN_BIG
#else
#    define SAPPHIRE_ENDIAN SAPPHIRE_ENDIAN_LITTLE
#endif

	//һֱ�򿪶�UNICODE�ı����֧��
#define SAPPHIRE_UNICODE_SUPPORT 1;

	// ��������ͳһ��������λ���
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef int int32;
	typedef short int16;
	typedef char int8;
	typedef float float32;
	typedef double float64;
	//����64λ����
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#endif


}


#endif