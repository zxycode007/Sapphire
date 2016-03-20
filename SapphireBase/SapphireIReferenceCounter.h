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

		//! Returns the debug name of the object.
		/** The Debugname may only be set and changed by the object
		itself. This method should only be used in Debug mode.
		\return Returns a string, previously set by setDebugName(); */
		const c8* getDebugName() const
		{
			return DebugName;
		}
	protected:
		 
		void setDebugName(const c8* newName)
		{
			DebugName = newName;
		}
	private:
		mutable int ReferenceCounter;
		 
		const c8* DebugName;


	};

}

#endif
