#ifndef _SAPPHIRE_IREFERENCE_COUNTER_
#define _SAPPHIRE_IREFERENCE_COUNTER_

#include "SapphirePrerequisites.h"

namespace Sapphire
{
	class IReferenceCounter
	{
	public:
		IReferenceCounter() :ReferenceCounter(1)
		{

		}
		virtual ~IReferenceCounter()
		{

		}
		void grab() const { ++ReferenceCounter; }

		bool drop() const
		{			
				--ReferenceCounter;
			if (!ReferenceCounter)
			{
				SAPPHIRE_DELETE this;
				return true;
			}

			return false;
		}

		SINT32 getReferenceCount() const
		{
			return ReferenceCounter;
		}
	private:
		mutable int ReferenceCounter;
	};
}

#endif
