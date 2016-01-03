#ifndef _SAPPHIRE_C_FPS_COUNTER_
#define _SAPPHIRE_C_FPS_COUNTER_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	class CFPSCounter
	{
	public:
		CFPSCounter();

		//! returns current fps
		SINT32 getFPS() const;

		//! returns primitive count
		UINT32 getPrimitive() const;

		//! returns average primitive count of last period
		UINT32 getPrimitiveAverage() const;

		//! returns accumulated primitive count since start
		UINT32 getPrimitiveTotal() const;

		//! to be called every frame
		void registerFrame(UINT32 now, UINT32 primitive);

	private:

		SINT32 FPS;
		UINT32 Primitive;
		UINT32 StartTime;

		UINT32 FramesCounted;
		UINT32 PrimitivesCounted;
		UINT32 PrimitiveAverage;
		UINT32 PrimitiveTotal;
	};
}

#endif