#ifndef _SAPPHIRE_C_XML_READER_
#define _SAPPHIRE_C_XML_READER_

#include "SapphireIXMLReader.h"

namespace Sapphire
{
	class IReadFile;

	//! creates an IXMLReader
	IXMLReader* createIXMLReader(IReadFile* file);

	//! creates an IXMLReader
	IXMLReaderUTF8* createIXMLReaderUTF8(IReadFile* file);
}

#endif