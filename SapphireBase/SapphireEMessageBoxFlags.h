#ifndef _SAPPHIRE_E_MESSAGE_BOX_FLAGS_
#define _SAPPHIRE_E_MESSAGE_BOX_FLAGS_

namespace Sapphire
{
	//! enumeration for message box layout flags
	enum EMESSAGE_BOX_FLAG
	{
		//! Flag for the ok button
		EMBF_OK = 0x1,

		//! Flag for the cancel button
		EMBF_CANCEL = 0x2,

		//! Flag for the yes button
		EMBF_YES = 0x4,

		//! Flag for the no button
		EMBF_NO = 0x8,

		//! This value is not used. It only forces this enumeration to compile in 32 bit.
		EMBF_FORCE_32BIT = 0x7fffffff
	};
}

#endif