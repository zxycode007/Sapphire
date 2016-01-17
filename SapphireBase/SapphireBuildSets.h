#ifndef __SAPPHIRE_BUILDSETS__
#define __SAPPHIRE_BUILDSETS__

#define SAPPHIRE_LITTLE_ENDIAN 0
#define SAPPHIRE_BIG_ENDIAN 1

#define SAPPHIRE_CONFIG_ENDIAN SAPPHIRE_LITTLE_ENDIAN     //0λLittle endian 1ΪBig endian
#define SAPPHIRE_STATIC 0       //�Ƿ�Ϊ��̬�⣬��֮Ϊ��̬��
#define SAPPHIRE_NOCLIENT_SET 1     //û�пͻ��ˣ���̬���ӿ⣩1�� �пͻ���0
#define SAPPHIRE_THREAD_SUPPORT 0     //��ʹ���߳�0 ��ʹ���߳�1  ʹ���߳�2


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