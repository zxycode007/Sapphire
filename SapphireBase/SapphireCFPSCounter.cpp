#include "SapphireCFPSCounter.h"
#include "SapphireQMath.h"

namespace Sapphire
{
	CFPSCounter::CFPSCounter()
		: FPS(60), Primitive(0), StartTime(0), FramesCounted(0),
		PrimitivesCounted(0), PrimitiveAverage(0), PrimitiveTotal(0)
	{

	}


	//! returns current fps
	SINT32 CFPSCounter::getFPS() const
	{
		return FPS;
	}


	//! returns current primitive count
	UINT32 CFPSCounter::getPrimitive() const
	{
		return Primitive;
	}


	//! returns average primitive count of last period
	UINT32 CFPSCounter::getPrimitiveAverage() const
	{
		return PrimitiveAverage;
	}


	//! returns accumulated primitive count since start
	UINT32 CFPSCounter::getPrimitiveTotal() const
	{
		return PrimitiveTotal;
	}


	//! to be called every frame
	void CFPSCounter::registerFrame(UINT32 now, UINT32 primitivesDrawn)
	{
		++FramesCounted;
		PrimitiveTotal += primitivesDrawn;
		PrimitivesCounted += primitivesDrawn;
		Primitive = primitivesDrawn;

		const UINT32 milliseconds = now - StartTime;

		if (milliseconds >= 1500)
		{
			const float32 invMilli = reciprocal((float32)milliseconds);

			FPS = ceil32((1000 * FramesCounted) * invMilli);
			PrimitiveAverage = ceil32((1000 * PrimitivesCounted) * invMilli);

			FramesCounted = 0;
			PrimitivesCounted = 0;
			StartTime = now;
		}
	}
}