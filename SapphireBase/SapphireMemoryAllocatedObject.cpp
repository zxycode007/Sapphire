#include "SapphireStableHeader.h"
#include "SapphirePrerequisites.h"


namespace Sapphire
{
	template class AllocatedObj<GeneralAllocPolicy>;
	template class AllocatedObj<GeometryAllocPolicy>;
	template class AllocatedObj<AnimationAllocPolicy>;
	template class AllocatedObj<SceneCtlAllocPolicy>;
	template class AllocatedObj<SceneObjAllocPolicy>;
	template class AllocatedObj<ResourceAllocPolicy>;
	template class AllocatedObj<ScriptingAllocPolicy>;
	template class AllocatedObj<RenderSysAllocPolicy>;
}