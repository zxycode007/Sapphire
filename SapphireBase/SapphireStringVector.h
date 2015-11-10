
#ifndef _StringVector_H__
#define _StringVector_H__

#include "SapphirePrerequisites.h"

#include "SapphireString.h"
#include "SapphireSharedPtr.h"


namespace Sapphire {

	typedef vector<String>::type StringVector;
	typedef SharedPtr<StringVector> StringVectorPtr;

	

}

#endif