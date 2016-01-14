#ifndef _SAPPHIRE_E_GUI_ALIGNMENT_
#define _SAPPHIRE_E_GUI_ALIGNMENT_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	enum EGUI_ALIGNMENT
	{
		//! Aligned to parent's top or left side (default)
		EGUIA_UPPERLEFT = 0,
		//! Aligned to parent's bottom or right side
		EGUIA_LOWERRIGHT,
		//! Aligned to the center of parent
		EGUIA_CENTER,
		//! Stretched to fit parent
		EGUIA_SCALE
	};

	//! Names for alignments
	const c8* const GUIAlignmentNames[] =
	{
		"upperLeft",
		"lowerRight",
		"center",
		"scale",
		0
	};
}

#endif