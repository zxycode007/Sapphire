#ifndef _SAPPHIRE_STABLE_HEADERS_
#define _SAPPHIRE_STABLE_HEADERS_

#include "SapphirePlatfrom.h"

#if (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32 && !defined(__MINGW32__)) || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE_IOS

#include "SapphireLog.h"
#include "SapphireMatrix4.h"
#include "SapphireVector2.h"
#include "SapphireVector3.h"
#include "SapphireMath.h"
#include "SapphireMatrix3.h"
#include "SapphireRay.h"
#include "SapphireSphere.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphirePlaneBoundedVolume.h"
#include "SapphirePrerequisites.h"
#include "SapphireQuaternion.h"
#include "SapphireSingleton.h"
#include "SapphireString.h"
#include "SapphirePlane.h"


#endif
#endif