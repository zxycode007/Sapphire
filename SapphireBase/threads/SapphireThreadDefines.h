#ifndef _SAPPHIRE_THREAD_DEFINES_
#define _SAPPHIRE_THREAD_DEFINES_

//定义线程互斥量
#define SAPPHIRE_AUTO_MUTEX_NAME mutex
//定义线程提供者
#if SAPPHIRE_THREAD_PROVIDER == 0
#include "SapphireThreadDefinesNone.h"
#elif SAPPHIRE_THREAD_PROVIDER == 1
//#include "SapphireThreadDefinesBoost.h"
#include "SapphireThreadDefinesNone.h"
#elif SAPPHIRE_THREAD_PROVIDER == 2
//#include "SapphireThreadDefinesPoco.h"
#include "SapphireThreadDefinesNone.h"
#elif SAPPHIRE_THREAD_PROVIDER == 3
#//include "SapphireThreadDefinesTBB.h"
#include "SapphireThreadDefinesNone.h"
#endif

#endif