#ifndef _SAPPHIRE_CULLING_TYPES_
#define _SAPPHIRE_CULLING_TYPES_

#include "SapphirePrerequisites.h"

namespace Sapphire
{

	//! ���ó����ڵ��Զ��޳����͵�ö��
	enum E_CULLING_TYPE
	{
		EAC_OFF = 0,
		EAC_BOX = 1,
		EAC_FRUSTUM_BOX = 2,
		EAC_FRUSTUM_SPHERE = 4,
		EAC_OCC_QUERY = 8
	};

	//! �޳�������
	const c8* const AutomaticCullingNames[] =
	{
		"false",
		"box",			// camera box against node box
		"frustum_box",		// camera frustum against node box
		"frustum_sphere",	// camera frustum against node sphere
		"occ_query",	// occlusion query
		0
	};
}

#endif