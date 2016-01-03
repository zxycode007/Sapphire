#ifndef __SAPPHIRE_CONFIG__
#define __SAPPHIRE_CONFIG__

#include "SapphireBuildSets.h"


#define SAPPHIRE_DOUBLE_PRECISION 1     //1Ϊ˫���� 0Ϊ������

#ifndef SAPPHIRE_WCHAR_T_STRINGS
   #define SAPPHIRE_WCHAR_T_STRINGS 0    //�����Ƿ�ʹ��WCHAR
#endif

#ifndef SAPPHIRE_FAST_MATH
#define SAPPHIRE_FAST_MATH 0  //�Ƿ�ʹ�ÿ��ټ���
#endif 

#define SAPPHIRE_SINT64_SUPPORT    //����SINT64֧��
#ifndef SAPPHIRE_SINT64_SUPPORT  
#define SAPPHIRE_SINT64_NO_SUPPORT
#endif

// ����Ҫʹ�õ��ڴ����������
#define SAPPHIRE_MEMORY_ALLOCATOR_STD 1   //��׼
#define SAPPHIRE_MEMORY_ALLOCATOR_NED 2    //NED
#define SAPPHIRE_MEMORY_ALLOCATOR_USER 3     //�û��Զ���
#define SAPPHIRE_MEMORY_ALLOCATOR_NEDPOOLING 4      //ned�ڴ��

#ifndef SAPPHIRE_MEMORY_ALLOCATOR
#  define SAPPHIRE_MEMORY_ALLOCATOR SAPPHIRE_MEMORY_ALLOCATOR_STD      //�����ڴ������
#endif

// �����Ƿ���STL������ʹ���Զ�����ڴ������
#ifndef SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
#  define SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR 1
#endif

//�����Ҫʹ��Sapphire����string���Զ�����ڴ����������ô��Ҫ��������
//#define SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 1
// ����������Sapphire���ַ�����������std����string����
#ifndef SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR
#	define SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 1
#endif

// �򿪹ر��ڴ����ģʽ����¼�ڴ�ķ��䣬����й¶
//Ĭ�Ϲرգ���Ϊ���ж��⿪��������������й���ÿ��Ŀ��
#ifndef SAPPHIRE_MEMORY_TRACKER_DEBUG_MODE
#  define SAPPHIRE_MEMORY_TRACKER_DEBUG_MODE 0
#endif

#ifndef SAPPHIRE_MEMORY_TRACKER_RELEASE_MODE
#  define SAPPHIRE_MEMORY_TRACKER_RELEASE_MODE 0
#endif

//�������������֧�ֵ�����������
#ifndef SAPPHIRE_MAX_TEXTURE_LAYERS
#define SAPPHIRE_MAX_TEXTURE_LAYERS  8
#endif


/**
���߳�֧��
SAPPHIRE_THREAD_SUPPORT = 0
��֧���߳�
SAPPHIRE_THREAD_SUPPORT = 1
֧�ֺ�̨�̼߳��أ���һ���߳�ͬʱ���غ͹�����Դ�� ��Դ�����SharePtr ������̰߳�ȫ�ģ�
��Դ�����ٺ�̨��ɼ��ء��߳�����Ч���еģ� ��������Լٶ������־��״̬�����̰߳�ȫ�ġ�
SAPPHIRE_THREAD_SUPPORT = 2
�߳�֧�ֺ�̨��ԴԤ��������ζ����Դ���ݿ����ٺ�̨����������ʽ�����ڴ棬������Դ�Ĺ���������Ⱦϵͳ��������Ȼ��
�����߳�����ɡ�ͬ��ԭ�￪���Ƚϵ���Ҫ��ƿ���������ں�̨I/O
*/
#ifndef SAPPHIRE_THREAD_SUPPORT
#define SAPPHIRE_THREAD_SUPPORT 0
#endif
#if SAPPHIRE_THREAD_SUPPORT != 0 && SAPPHIRE_THREAD_SUPPORT != 1 && SAPPHIRE_THREAD_SUPPORT != 2
#define SAPPHIRE_THREAD_SUPPORT 0
#endif

/**
�̷߳����ṩ�ߣ��ĸ�ѡ��
SAPPHIRE_THREAD_PROVIDER = 0
��ʹ���߳�.
SAPPHIRE_THREAD_PROVIDER = 1
Boost���߳�
SAPPHIRE_THREAD_PROVIDER = 2
Poco���߳�
SAPPHIRE_THREAD_PROVIDER = 3
TBB���߳�
*/
#ifndef SAPPHIRE_THREAD_PROVIDER
#define SAPPHIRE_THREAD_PROVIDER 0
#endif

#define SAPPHIRE_WITH_SOFTWARE 0
#define SAPPHIRE_WITH_BURNINGSVIDEO 1
#define SAPPHIRE_WITH_DIRECT3D_8 2
#define SAPPHIRE_DIRECT3D_9 3
#define SAPPHIRE_WITH_OPENGL 4

#ifndef SAPPHIRE_3D_API
#define SAPPHIRE_3D_API  SAPPHIRE_WITH_OPENGL	
#endif

//! ����SAPPHIRE_OPENGL_USE_EXTPOINTER  �����ҪOpenGL ��Ⱦ����ͨ������ָ��ʹ��OpenGL ��չ
/** ĳЩϵͳ��֧��ͨ������ָ�붯̬��չOpenGL */
#if !defined(SAPPHIRE_PLATFORM_APPLE) && !defined(SAPPHIRE_PLATFORM_SOLARIS)
#define SAPPHIRE_OPENGL_USE_EXTPOINTER
#endif



// ��������ϰ��
#if defined(_STDCALL_SUPPORTED)
#define SAPPHIRE_CALLCONV __stdcall
#else
#define SAPPHIRE_CALLCONV __cdecl
#endif // STDCALL_SUPPORTED

namespace Sapphire
{


}
#endif