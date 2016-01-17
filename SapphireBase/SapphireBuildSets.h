#ifndef __SAPPHIRE_BUILDSETS__
#define __SAPPHIRE_BUILDSETS__

#define SAPPHIRE_LITTLE_ENDIAN 0
#define SAPPHIRE_BIG_ENDIAN 1

#define SAPPHIRE_CONFIG_ENDIAN SAPPHIRE_LITTLE_ENDIAN     //0位Little endian 1为Big endian
#define SAPPHIRE_STATIC 0       //是否为静态库，反之为动态库
#define SAPPHIRE_NOCLIENT_SET 1     //没有客户端（动态链接库）1， 有客户端0
#define SAPPHIRE_THREAD_SUPPORT 0     //不使用线程0 ，使用线程1  使用线程2


namespace Sapphire
{
#if SAPPHIRE_STATIC == 1
#define SAPPHIRE_STATIC_LIB
#endif

#if SAPPHIRE_CONFIG_ENDIAN
#define SAPPHIRE_CONFIG_BIG_ENDIAN 
#endif

#if SAPPHIRE_NOCLIENT_SET == 1
#define SAPPHIRE_NONCLIENT_BUILD
#endif

}

#endif