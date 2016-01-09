#ifndef _SAPPHIRE_I_XML_READER_
#define _SAPPHIRE_I_XML_READER_

#include "SapphireIXML.h"

namespace Sapphire
{
	//! An xml reader for wide characters, derived from IReferenceCounted.
	/** This XML Parser can read any type of text files from any source
	Irrlicht can read. Just call IFileSystem::createXMLReader(). For more
	informations on how to use the parser, see IIrrXMLReader */
	//typedef IIrrXMLReader<wchar_t, IReferenceCounter> IXMLReader;

	//! An xml reader for ASCII or UTF-8 characters, derived from IReferenceCounted.
	/** This XML Parser can read any type of text files from any source
	Irrlicht can read. Just call IFileSystem::createXMLReaderUTF8(). For
	more informations on how to use the parser, see IIrrXMLReader */
	typedef IIrrXMLReader<c8, IReferenceCounter> IXMLReaderUTF8;
}
#endif