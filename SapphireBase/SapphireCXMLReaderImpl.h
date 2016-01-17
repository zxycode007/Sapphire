#ifndef _SAPPHIRE_C_XML_READER_IMPL_
#define _SAPPHIRE_C_XML_READER_IMPL_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireIXML.h"
#include "SapphireString.h"

namespace Sapphire
{

	
	//! implementation of the IrrXMLReade
	class CXMLReaderImpl : public IIrrXMLReader
	{
	public:

		//! Constructor
		CXMLReaderImpl(IFileReadCallBack* callback, bool deleteCallBack = true)
			: IgnoreWhitespaceText(true), TextData(0), P(0), TextBegin(0), TextSize(0), CurrentNodeType(EXN_NONE),
			SourceFormat(ETF_ASCII), TargetFormat(ETF_ASCII), IsEmptyElement(false)
		{
			if (!callback)
				return;

			storeTargetFormat();

			// read whole xml file

			readFile(callback);

			// clean up

			if (deleteCallBack)
				delete callback;

			// create list with special characters

			createSpecialCharacterList();

			// set pointer to text begin
			P = TextBegin;
		}


		//! Destructor
		virtual ~CXMLReaderImpl()
		{
			delete[] TextData;
		}


		//! Reads forward to the next xml node.
		//! \return Returns false, if there was no further node.
		virtual bool read()
		{
			// if not end reached, parse the node
			if (P && ((unsigned int)(P - TextBegin) < TextSize - 1) && (*P != 0))
			{
				return parseCurrentNode();
			}

			//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return false;
		}


		//! Returns the type of the current XML node.
		virtual EXML_NODE getNodeType() const
		{
			return CurrentNodeType;
		}


		//! Returns attribute count of the current XML node.
		virtual unsigned int getAttributeCount() const
		{
			return Attributes.size();
		}


		//! Returns name of an attribute.
		virtual const char* getAttributeName(int idx) const
		{
			if ((UINT32)idx >= Attributes.size())
				return 0;

			return Attributes[idx].Name.c_str();
		}


		//! Returns the value of an attribute.
		virtual const char* getAttributeValue(int idx) const
		{
			if ((unsigned int)idx >= Attributes.size())
				return 0;

			return Attributes[idx].Value.c_str();
		}


		//! Returns the value of an attribute.
		virtual const char* getAttributeValue(const char* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			return attr->Value.c_str();
		}


		//! Returns the value of an attribute
		virtual const char* getAttributeValueSafe(const char* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return EmptyString.c_str();

			return attr->Value.c_str();
		}



		//! Returns the value of an attribute as integer.
		int getAttributeValueAsInt(const char* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			String c(attr->Value.c_str());
			return strtol10(c.c_str());
		}


		//! Returns the value of an attribute as integer.
		int getAttributeValueAsInt(int idx) const
		{
			const char* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			String c(attrvalue);
			return strtol10(c.c_str());
		}


		//! Returns the value of an attribute as float.
		float getAttributeValueAsFloat(const char* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			String c = attr->Value.c_str();
			return fast_atof(c.c_str());
		}


		//! Returns the value of an attribute as float.
		float getAttributeValueAsFloat(int idx) const
		{
			const char* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			String c = attrvalue;
			return fast_atof(c.c_str());
		}


		//! Returns the name of the current node.
		virtual const char* getNodeName() const
		{
			return NodeName.c_str();
		}


		//! Returns data of the current node.
		virtual const char* getNodeData() const
		{
			return NodeName.c_str();
		}


		//! Returns if an element is an empty element, like <foo />
		virtual bool isEmptyElement() const
		{
			return IsEmptyElement;
		}

		//! Returns format of the source xml file.
		virtual ETEXT_FORMAT getSourceFormat() const
		{
			return SourceFormat;
		}

		//! Returns format of the strings returned by the parser.
		virtual ETEXT_FORMAT getParserFormat() const
		{
			return TargetFormat;
		}

	private:

		// Reads the current xml node
		// return false if no further node is found
		bool parseCurrentNode()
		{
			char* start = P;

			// more forward until '<' found
			while (*P != '<' && *P)
				++P;

			// not a node, so return false
			if (!*P)
				return false;

			if (P - start > 0)
			{
				// we found some text, store it
				if (setText(start, P))
					return true;
			}

			++P;

			// based on current token, parse and report next element
			switch (*P)
			{
			case '/':
				parseClosingXMLElement();
				break;
			case '?':
				ignoreDefinition();
				break;
			case '!':
				if (!parseCDATA())
					parseComment();
				break;
			default:
				parseOpeningXMLElement();
				break;
			}
			return true;
		}


		//! sets the state that text was found. Returns true if set should be set
		bool setText(char* start, char* end)
		{
			// By default xml preserves all whitespace. But Irrlicht dropped some whitespace by default 
			// in the past which did lead to OS dependent behavior. We just ignore all whitespace for now 
			// as it's the closest to fixing behavior without breaking downward compatibility too much. 
			if (IgnoreWhitespaceText)
			{
				char* p = start;
				for (; p != end; ++p)
					if (!isWhiteSpace(*p))
						break;

				if (p == end)
					return false;
			}

			// set current text to the parsed text, and replace xml special characters
			String s(start, (int)(end - start));
			NodeName = replaceSpecialCharacters(s);

			// current XML node type is text
			CurrentNodeType = EXN_TEXT;

			return true;
		}



		//! ignores an xml definition like <?xml something />
		void ignoreDefinition()
		{
			CurrentNodeType = EXN_UNKNOWN;

			// move until end marked with '>' reached
			while (*P != '>')
				++P;

			++P;
		}


		//! parses a comment
		void parseComment()
		{
			CurrentNodeType = EXN_COMMENT;
			P += 1;

			char *pCommentBegin = P;

			int count = 1;

			// move until end of comment reached
			while (count)
			{
				if (*P == '>')
					--count;
				else
					if (*P == '<')
						++count;

				++P;
			}

			P -= 3;
			NodeName = String(pCommentBegin + 2, (int)(P - pCommentBegin - 2));
			P += 3;
		}


		//! parses an opening xml element and reads attributes
		void parseOpeningXMLElement()
		{
			CurrentNodeType = EXN_ELEMENT;
			IsEmptyElement = false;
			Attributes.clear();

			// find name
			const char* startName = P;

			// find end of element
			while (*P != '>' && !isWhiteSpace(*P))
				++P;

			const char* endName = P;

			// find Attributes
			while (*P != '>')
			{
				if (isWhiteSpace(*P))
					++P;
				else
				{
					if (*P != '/')
					{
						// we've got an attribute

						// read the attribute names
						const char* attributeNameBegin = P;

						while (!isWhiteSpace(*P) && *P != '=')
							++P;

						const char* attributeNameEnd = P;
						++P;

						// read the attribute value
						// check for quotes and single quotes, thx to murphy
						while ((*P != '\"') && (*P != '\'') && *P)
							++P;

						if (!*P) // malformatted xml file
							return;

						const char attributeQuoteChar = *P;

						++P;
						const char* attributeValueBegin = P;

						while (*P != attributeQuoteChar && *P)
							++P;

						if (!*P) // malformatted xml file
							return;

						const char* attributeValueEnd = P;
						++P;

						SAttribute attr;
						attr.Name = String(attributeNameBegin,
							(int)(attributeNameEnd - attributeNameBegin));

						String s(attributeValueBegin,
							(int)(attributeValueEnd - attributeValueBegin));

						attr.Value = replaceSpecialCharacters(s);
						Attributes.push_back(attr);
					}
					else
					{
						// tag is closed directly
						++P;
						IsEmptyElement = true;
						break;
					}
				}
			}

			// check if this tag is closing directly
			if (endName > startName && *(endName - 1) == '/')
			{
				// directly closing tag
				IsEmptyElement = true;
				endName--;
			}
			NodeName = String(startName, (int)(endName - startName));

			++P;
		}


		//! parses an closing xml tag
		void parseClosingXMLElement()
		{
			CurrentNodeType = EXN_ELEMENT_END;
			IsEmptyElement = false;
			Attributes.clear();

			++P;
			const char* pBeginClose = P;

			while (*P != '>')
				++P;

			//NodeName = String(pBeginClose, (int)(P - pBeginClose));
			NodeName = String("");
			++P;
		}

		//! parses a possible CDATA section, returns false if begin was not a CDATA section
		bool parseCDATA()
		{
			if (*(P + 1) != '[')
				return false;

			CurrentNodeType = EXN_CDATA;

			// skip '<![CDATA['
			int count = 0;
			while (*P && count<8)
			{
				++P;
				++count;
			}

			if (!*P)
				return true;

			char *cDataBegin = P;
			char *cDataEnd = 0;

			// find end of CDATA
			while (*P && !cDataEnd)
			{
				if (*P == '>' &&
					(*(P - 1) == ']') &&
					(*(P - 2) == ']'))
				{
					cDataEnd = P - 2;
				}

				++P;
			}

			if (cDataEnd)
				NodeName = String(cDataBegin, (int)(cDataEnd - cDataBegin));
			else
				NodeName = "";

			return true;
		}


		// structure for storing attribute-name pairs
		struct SAttribute
		{
			String Name;
			String Value;
		};

		// finds a current attribute by name, returns 0 if not found
		const SAttribute* getAttributeByName(const char* name) const
		{
			if (!name)
				return 0;

			String n = name;

			for (int i = 0; i<(int)Attributes.size(); ++i)
				if (Attributes[i].Name == n)
					return &Attributes[i];

			return 0;
		}

		// replaces xml special characters in a string and creates a new one
		String replaceSpecialCharacters(
			String& origstr)
		{	 
			int pos = origstr.find('&');
			int oldPos = 0;

			if (pos == -1)
				return origstr;

			String newstr;

			while (pos != -1 && pos < (int)origstr.size() - 2)
			{
				// check if it is one of the special characters

				int specialChar = -1;
				for (int i = 0; i<(int)SpecialCharacters.size(); ++i)
				{
					const char* p = &origstr.c_str()[pos] + 1;

					if (equalsn(&SpecialCharacters[i][1], p, SpecialCharacters[i].size() - 1))
					{
						specialChar = i;
						break;
					}
				}

				if (specialChar != -1)
				{
					//newstr.append(origstr.subString(oldPos, pos - oldPos));
					//newstr.append(SpecialCharacters[specialChar][0]);
					newstr += origstr.substr(oldPos, pos - oldPos);
					newstr += SpecialCharacters[specialChar][0];
					pos += SpecialCharacters[specialChar].size();
				}
				else
				{
					newstr.append(origstr.substr(oldPos, pos - oldPos + 1));
					pos += 1;
				}

				// find next &
				oldPos = pos;
				pos = origstr.find('&', pos);
			}

			if (oldPos < (int)origstr.size() - 1)
				newstr.append(origstr.substr(oldPos, origstr.size() - oldPos));

			return newstr;
		}



		//! reads the xml file and converts it into the wanted character format.
		bool readFile(IFileReadCallBack* callback)
		{
			long size = callback->getSize();
			if (size<0)
				return false;
			size += 4; // We need four terminating 0's at the end.
			// For ASCII we need 1 0's, for UTF-16 2, for UTF-32 4.

			char* data8 = new char[size];

			if (!callback->read(data8, size - 4))
			{
				delete[] data8;
				return false;
			}

			// add zeros at end

			memset(data8 + size - 4, 0, 4);

			char16* data16 = reinterpret_cast<char16*>(data8);
			char32* data32 = reinterpret_cast<char32*>(data8);

			// now we need to convert the data to the desired target format
			// based on the byte order mark.

			const unsigned char UTF8[] = { 0xEF, 0xBB, 0xBF }; // 0xEFBBBF;
			const UINT16 UTF16_BE = 0xFFFE;
			const UINT16 UTF16_LE = 0xFEFF;
			const UINT32 UTF32_BE = 0xFFFE0000;
			const UINT32 UTF32_LE = 0x0000FEFF;

			// check source for all utf versions and convert to target data format

			if (size >= 4 && data32[0]
				== static_cast<char32>(UTF32_BE))
			{
				// UTF-32, big endian
				SourceFormat = ETF_UTF32_BE;
				convertTextData(data32 + 1, data8, (size / 4) - 1); // data32+1 because we need to skip the header
			}
			else
				if (size >= 4 && data32[0] == static_cast<char32>(UTF32_LE))
				{
					// UTF-32, little endian
					SourceFormat = ETF_UTF32_LE;
					convertTextData(data32 + 1, data8, (size / 4) - 1); // data32+1 because we need to skip the header
				}
				else
					if (size >= 2 && data16[0] == UTF16_BE)
					{
						// UTF-16, big endian
						SourceFormat = ETF_UTF16_BE;
						convertTextData(data16 + 1, data8, (size / 2) - 1); // data16+1 because we need to skip the header
					}
					else
						if (size >= 2 && data16[0] == UTF16_LE)
						{
							// UTF-16, little endian
							SourceFormat = ETF_UTF16_LE;
							convertTextData(data16 + 1, data8, (size / 2) - 1); // data16+1 because we need to skip the header
						}
						else
							if (size >= 3 && memcmp(data8, UTF8, 3) == 0)
							{
								// UTF-8
								SourceFormat = ETF_UTF8;
								convertTextData(data8 + 3, data8, size - 3); // data8+3 because we need to skip the header
							}
							else
							{
								// ASCII
								SourceFormat = ETF_ASCII;
								convertTextData(data8, data8, size);
							}

			return true;
		}


		//! converts the text file into the desired format.
		/** \param source: begin of the text (without byte order mark)
		\param pointerToStore: pointer to text data block which can be
		stored or deleted based on the nesessary conversion.
		\param sizeWithoutHeader: Text size in characters without header
		*/
		template<class src_char>
		void convertTextData(src_char* source, char* pointerToStore, int sizeWithoutHeader)
		{
			// convert little to big endian if necessary
			if (sizeof(src_char) > 1 &&
				isLittleEndian(TargetFormat) != isLittleEndian(SourceFormat))
				convertToLittleEndian(source);

			// check if conversion is necessary:
			if (sizeof(src_char) == sizeof(char))
			{
				// no need to convert
				TextBegin = (char*)source;
				TextData = (char*)pointerToStore;
				TextSize = sizeWithoutHeader;
			}
			else
			{
				// convert source into target data format.
				// TODO: implement a real conversion. This one just
				// copies bytes. This is a problem when there are
				// unicode symbols using more than one character.

				TextData = new char[sizeWithoutHeader];

				if (sizeof(src_char) == 1)
				{
					// we have to cast away negative numbers or results might add the sign instead of just doing a copy
					for (int i = 0; i<sizeWithoutHeader; ++i)
					{
						TextData[i] = static_cast<char>(static_cast<unsigned char>(source[i]));
					}
				}
				else
				{
					for (int i = 0; i<sizeWithoutHeader; ++i)
						TextData[i] = static_cast<char>(source[i]);
				}
				TextBegin = TextData;
				TextSize = sizeWithoutHeader;

				// delete original data because no longer needed
				delete[] pointerToStore;
			}
		}

		//! converts whole text buffer to little endian
		template<class src_char>
		void convertToLittleEndian(src_char* t)
		{
			if (sizeof(src_char) == 4)
			{
				// 32 bit

				while (*t)
				{
					*t = ((*t & 0xff000000) >> 24) |
						((*t & 0x00ff0000) >> 8) |
						((*t & 0x0000ff00) << 8) |
						((*t & 0x000000ff) << 24);
					++t;
				}
			}
			else
			{
				// 16 bit

				while (*t)
				{
					*t = (*t >> 8) | (*t << 8);
					++t;
				}
			}
		}

		//! returns if a format is little endian
		inline bool isLittleEndian(ETEXT_FORMAT f)
		{
			return f == ETF_ASCII ||
				f == ETF_UTF8 ||
				f == ETF_UTF16_LE ||
				f == ETF_UTF32_LE;
		}


		//! returns true if a character is whitespace
		inline bool isWhiteSpace(char c)
		{
			return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
		}


		//! generates a list with xml special characters
		void createSpecialCharacterList()
		{
			// list of strings containing special symbols,
			// the first character is the special character,
			// the following is the symbol string without trailing &.

			SpecialCharacters.push_back("&amp;");
			SpecialCharacters.push_back("<lt;");
			SpecialCharacters.push_back(">gt;");
			SpecialCharacters.push_back("\"quot;");
			SpecialCharacters.push_back("'apos;");

		}


		//! compares the first n characters of the strings
		bool equalsn(const char* str1, const char* str2, int len)
		{
			int i;
			for (i = 0; str1[i] && str2[i] && i < len; ++i)
				if (str1[i] != str2[i])
					return false;

			// if one (or both) of the strings was smaller then they
			// are only equal if they have the same lenght
			return (i == len) || (str1[i] == 0 && str2[i] == 0);
		}


		//! stores the target text format
		void storeTargetFormat()
		{
			// get target format. We could have done this using template specialization,
			// but VisualStudio 6 don't like it and we want to support it.

			switch (sizeof(char))
			{
			case 1:
				TargetFormat = ETF_UTF8;
				break;
			case 2:
				TargetFormat = ETF_UTF16_LE;
				break;
			case 4:
				TargetFormat = ETF_UTF32_LE;
				break;
			default:
				TargetFormat = ETF_ASCII; // should never happen.
			}
		}


		// instance variables:
		bool IgnoreWhitespaceText;	 // do not return EXN_TEXT nodes for pure whitespace
		char* TextData;         // data block of the text file
		char* P;                // current point in text to parse
		char* TextBegin;        // start of text to parse
		unsigned int TextSize;       // size of text to parse in characters, not bytes

		EXML_NODE CurrentNodeType;   // type of the currently parsed node
		ETEXT_FORMAT SourceFormat;   // source format of the xml file
		ETEXT_FORMAT TargetFormat;   // output format of this parser

		String NodeName;    // name of the node currently in - also used for text
		String EmptyString; // empty string to be returned by getSafe() methods

		bool IsEmptyElement;       // is the currently parsed node empty?

		vector<String>::type SpecialCharacters; // see createSpecialCharacterList()
		vector<SAttribute>::type Attributes; // attributes of current element

	}; // end CXMLReaderImpl

	class CXMLReaderUTF8Impl : public IIrrXMLReaderUTF8
	{
	public:

		//! Constructor
		CXMLReaderUTF8Impl(IFileReadCallBack* callback, bool deleteCallBack = true)
			: IgnoreWhitespaceText(true), TextData(0), P(0), TextBegin(0), TextSize(0), CurrentNodeType(EXN_NONE),
			SourceFormat(ETF_ASCII), TargetFormat(ETF_ASCII), IsEmptyElement(false)
		{
			if (!callback)
				return;

			storeTargetFormat();

			// read whole xml file

			readFile(callback);

			// clean up

			if (deleteCallBack)
				delete callback;

			// create list with special wchar_tacters

			createSpecialCharacterList();

			// set pointer to text begin
			P = TextBegin;
		}


		//! Destructor
		virtual ~CXMLReaderUTF8Impl()
		{
			delete[] TextData;
		}


		//! Reads forward to the next xml node.
		//! \return Returns false, if there was no further node.
		virtual bool read()
		{
			// if not end reached, parse the node
			if (P && ((unsigned int)(P - TextBegin) < TextSize - 1) && (*P != 0))
			{
				return parseCurrentNode();
			}

			//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return false;
		}


		//! Returns the type of the current XML node.
		virtual EXML_NODE getNodeType() const
		{
			return CurrentNodeType;
		}


		//! Returns attribute count of the current XML node.
		virtual unsigned int getAttributeCount() const
		{
			return Attributes.size();
		}


		//! Returns name of an attribute.
		virtual const wchar_t* getAttributeName(int idx) const
		{
			if ((UINT32)idx >= Attributes.size())
				return 0;

			return Attributes[idx].Name.c_str();
		}


		//! Returns the value of an attribute.
		virtual const wchar_t* getAttributeValue(int idx) const
		{
			if ((unsigned int)idx >= Attributes.size())
				return 0;

			return Attributes[idx].Value.c_str();
		}


		//! Returns the value of an attribute.
		virtual const wchar_t* getAttributeValue(const wchar_t* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			return attr->Value.c_str();
		}


		//! Returns the value of an attribute
		virtual const wchar_t* getAttributeValueSafe(const wchar_t* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return EmptyStringW.c_str();

			return attr->Value.c_str();
		}



		//! Returns the value of an attribute as integer.
		int getAttributeValueAsInt(const wchar_t* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			StringW c(attr->Value.c_str());
			return strtol10(StringUtil::StringWToString(c).c_str());
		}


		//! Returns the value of an attribute as integer.
		int getAttributeValueAsInt(int idx) const
		{
			const wchar_t* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			StringW c(attrvalue);
			return strtol10(StringUtil::StringWToString(c).c_str());
		}


		//! Returns the value of an attribute as float.
		float getAttributeValueAsFloat(const wchar_t* name) const
		{
			const SAttribute* attr = getAttributeByName(name);
			if (!attr)
				return 0;

			StringW c = attr->Value.c_str();
			return fast_atof(StringUtil::StringWToString(c).c_str());
		}


		//! Returns the value of an attribute as float.
		float getAttributeValueAsFloat(int idx) const
		{
			const wchar_t* attrvalue = getAttributeValue(idx);
			if (!attrvalue)
				return 0;

			StringW c = attrvalue;
			return fast_atof(StringUtil::StringWToString(c).c_str());
		}


		//! Returns the name of the current node.
		virtual const wchar_t* getNodeName() const
		{
			return NodeName.c_str();
		}


		//! Returns data of the current node.
		virtual const wchar_t* getNodeData() const
		{
			return NodeName.c_str();
		}


		//! Returns if an element is an empty element, like <foo />
		virtual bool isEmptyElement() const
		{
			return IsEmptyElement;
		}

		//! Returns format of the source xml file.
		virtual ETEXT_FORMAT getSourceFormat() const
		{
			return SourceFormat;
		}

		//! Returns format of the strings returned by the parser.
		virtual ETEXT_FORMAT getParserFormat() const
		{
			return TargetFormat;
		}

	private:

		// Reads the current xml node
		// return false if no further node is found
		bool parseCurrentNode()
		{
			wchar_t* start = P;

			// more forward until '<' found
			while (*P != L'<' && *P)
				++P;

			// not a node, so return false
			if (!*P)
				return false;

			if (P - start > 0)
			{
				// we found some text, store it
				if (setText(start, P))
					return true;
			}

			++P;

			// based on current token, parse and report next element
			switch (*P)
			{
			case L'/':
				parseClosingXMLElement();
				break;
			case L'?':
				ignoreDefinition();
				break;
			case L'!':
				if (!parseCDATA())
					parseComment();
				break;
			default:
				parseOpeningXMLElement();
				break;
			}
			return true;
		}


		//! sets the state that text was found. Returns true if set should be set
		bool setText(wchar_t* start, wchar_t* end)
		{
			// By default xml preserves all whitespace. But Irrlicht dropped some whitespace by default 
			// in the past which did lead to OS dependent behavior. We just ignore all whitespace for now 
			// as it's the closest to fixing behavior without breaking downward compatibility too much. 
			if (IgnoreWhitespaceText)
			{
				wchar_t* p = start;
				for (; p != end; ++p)
					if (!isWhiteSpace(*p))
						break;

				if (p == end)
					return false;
			}

			// set current text to the parsed text, and replace xml special wchar_tacters
			StringW s(start, (int)(end - start));
			NodeName = replaceSpecialCharacters(s);

			// current XML node type is text
			CurrentNodeType = EXN_TEXT;

			return true;
		}



		//! ignores an xml definition like <?xml something />
		void ignoreDefinition()
		{
			CurrentNodeType = EXN_UNKNOWN;

			// move until end marked with '>' reached
			while (*P != L'>')
				++P;

			++P;
		}


		//! parses a comment
		void parseComment()
		{
			CurrentNodeType = EXN_COMMENT;
			P += 1;

			wchar_t *pCommentBegin = P;

			int count = 1;

			// move until end of comment reached
			while (count)
			{
				if (*P == L'>')
					--count;
				else
					if (*P == L'<')
						++count;

				++P;
			}

			P -= 3;
			NodeName = StringW(pCommentBegin + 2, (int)(P - pCommentBegin - 2));
			P += 3;
		}


		//! parses an opening xml element and reads attributes
		void parseOpeningXMLElement()
		{
			CurrentNodeType = EXN_ELEMENT;
			IsEmptyElement = false;
			Attributes.clear();

			// find name
			const wchar_t* startName = P;

			// find end of element
			while (*P != L'>' && !isWhiteSpace(*P))
				++P;

			const wchar_t* endName = P;

			// find Attributes
			while (*P != L'>')
			{
				if (isWhiteSpace(*P))
					++P;
				else
				{
					if (*P != L'/')
					{
						// we've got an attribute

						// read the attribute names
						const wchar_t* attributeNameBegin = P;

						while (!isWhiteSpace(*P) && *P != L'=')
							++P;

						const wchar_t* attributeNameEnd = P;
						++P;

						// read the attribute value
						// check for quotes and single quotes, thx to murphy
						while ((*P != L'\"') && (*P != L'\'') && *P)
							++P;

						if (!*P) // malformatted xml file
							return;

						const wchar_t attributeQuoteChar = *P;

						++P;
						const wchar_t* attributeValueBegin = P;

						while (*P != attributeQuoteChar && *P)
							++P;

						if (!*P) // malformatted xml file
							return;

						const wchar_t* attributeValueEnd = P;
						++P;

						SAttribute attr;
						attr.Name = StringW(attributeNameBegin,
							(int)(attributeNameEnd - attributeNameBegin));

						StringW s(attributeValueBegin,
							(int)(attributeValueEnd - attributeValueBegin));

						attr.Value = replaceSpecialCharacters(s);
						Attributes.push_back(attr);
					}
					else
					{
						// tag is closed directly
						++P;
						IsEmptyElement = true;
						break;
					}
				}
			}

			// check if this tag is closing directly
			if (endName > startName && *(endName - 1) == L'/')
			{
				// directly closing tag
				IsEmptyElement = true;
				endName--;
			}
			NodeName = StringW(startName, (int)(endName - startName));

			++P;
		}


		//! parses an closing xml tag
		void parseClosingXMLElement()
		{
			CurrentNodeType = EXN_ELEMENT_END;
			IsEmptyElement = false;
			Attributes.clear();

			++P;
			const wchar_t* pBeginClose = P;

			while (*P != L'>')
				++P;

			//NodeName = StringW(pBeginClose, (int)(P - pBeginClose));
			NodeName = StringW(L"");
			++P;
		}

		//! parses a possible CDATA section, returns false if begin was not a CDATA section
		bool parseCDATA()
		{
			if (*(P + 1) != L'[')
				return false;

			CurrentNodeType = EXN_CDATA;

			// skip '<![CDATA['
			int count = 0;
			while (*P && count<8)
			{
				++P;
				++count;
			}

			if (!*P)
				return true;

			wchar_t *cDataBegin = P;
			wchar_t *cDataEnd = 0;

			// find end of CDATA
			while (*P && !cDataEnd)
			{
				if (*P == L'>' &&
					(*(P - 1) == L']') &&
					(*(P - 2) == L']'))
				{
					cDataEnd = P - 2;
				}

				++P;
			}

			if (cDataEnd)
				NodeName = StringW(cDataBegin, (int)(cDataEnd - cDataBegin));
			else
				NodeName = L"";

			return true;
		}


		// structure for storing attribute-name pairs
		struct SAttribute
		{
			StringW Name;
			StringW Value;
		};

		// finds a current attribute by name, returns 0 if not found
		const SAttribute* getAttributeByName(const wchar_t* name) const
		{
			if (!name)
				return 0;

			StringW n = name;

			for (int i = 0; i<(int)Attributes.size(); ++i)
				if (Attributes[i].Name == n)
					return &Attributes[i];

			return 0;
		}

		// replaces xml special wchar_tacters in a string and creates a new one
		StringW replaceSpecialCharacters(
			StringW& origstr)
		{
			int pos = origstr.find('&');
			int oldPos = 0;

			if (pos == -1)
				return origstr;

			StringW newstr;

			while (pos != -1 && pos < (int)origstr.size() - 2)
			{
				// check if it is one of the special wchar_tacters

				int specialChar = -1;
				for (int i = 0; i<(int)SpecialCharacters.size(); ++i)
				{
					const wchar_t* p = &origstr.c_str()[pos] + 1;

					if (equalsn(&SpecialCharacters[i][1], p, SpecialCharacters[i].size() - 1))
					{
						specialChar = i;
						break;
					}
				}

				if (specialChar != -1)
				{
					//newstr.append(origstr.subStringW(oldPos, pos - oldPos));
					//newstr.append(SpecialCharacters[specialChar][0]);
					newstr += origstr.substr(oldPos, pos - oldPos);
					newstr += SpecialCharacters[specialChar][0];
					pos += SpecialCharacters[specialChar].size();
				}
				else
				{
					newstr.append(origstr.substr(oldPos, pos - oldPos + 1));
					pos += 1;
				}

				// find next &
				oldPos = pos;
				pos = origstr.find('&', pos);
			}

			if (oldPos < (int)origstr.size() - 1)
				newstr.append(origstr.substr(oldPos, origstr.size() - oldPos));

			return newstr;
		}



		//! reads the xml file and converts it into the wanted wchar_tacter format.
		bool readFile(IFileReadCallBack* callback)
		{
			long size = callback->getSize();
			if (size<0)
				return false;
			size += 4; // We need four terminating 0's at the end.
			// For ASCII we need 1 0's, for UTF-16 2, for UTF-32 4.

			wchar_t* data8 = new wchar_t[size];

			if (!callback->read(data8, size - 4))
			{
				delete[] data8;
				return false;
			}

			// add zeros at end

			memset(data8 + size - 4, 0, 4);

			char16* data16 = reinterpret_cast<char16*>(data8);
			char32* data32 = reinterpret_cast<char32*>(data8);

			// now we need to convert the data to the desired target format
			// based on the byte order mark.

			const unsigned char UTF8[] = { 0xEF, 0xBB, 0xBF }; // 0xEFBBBF;
			const UINT16 UTF16_BE = 0xFFFE;
			const UINT16 UTF16_LE = 0xFEFF;
			const UINT32 UTF32_BE = 0xFFFE0000;
			const UINT32 UTF32_LE = 0x0000FEFF;

			// check source for all utf versions and convert to target data format

			if (size >= 4 && data32[0]
				== static_cast<char32>(UTF32_BE))
			{
				// UTF-32, big endian
				SourceFormat = ETF_UTF32_BE;
				convertTextData(data32 + 1, data8, (size / 4) - 1); // data32+1 because we need to skip the header
			}
			else
				if (size >= 4 && data32[0] == static_cast<char32>(UTF32_LE))
				{
					// UTF-32, little endian
					SourceFormat = ETF_UTF32_LE;
					convertTextData(data32 + 1, data8, (size / 4) - 1); // data32+1 because we need to skip the header
				}
				else
					if (size >= 2 && data16[0] == UTF16_BE)
					{
						// UTF-16, big endian
						SourceFormat = ETF_UTF16_BE;
						convertTextData(data16 + 1, data8, (size / 2) - 1); // data16+1 because we need to skip the header
					}
					else
						if (size >= 2 && data16[0] == UTF16_LE)
						{
							// UTF-16, little endian
							SourceFormat = ETF_UTF16_LE;
							convertTextData(data16 + 1, data8, (size / 2) - 1); // data16+1 because we need to skip the header
						}
						else
							if (size >= 3 && memcmp(data8, UTF8, 3) == 0)
							{
								// UTF-8
								SourceFormat = ETF_UTF8;
								convertTextData(data8 + 3, data8, size - 3); // data8+3 because we need to skip the header
							}
							else
							{
								// ASCII
								SourceFormat = ETF_ASCII;
								convertTextData(data8, data8, size);
							}

			return true;
		}


		//! converts the text file into the desired format.
		/** \param source: begin of the text (without byte order mark)
		\param pointerToStore: pointer to text data block which can be
		stored or deleted based on the nesessary conversion.
		\param sizeWithoutHeader: Text size in wchar_tacters without header
		*/
		template<class src_wchar_t>
		void convertTextData(src_wchar_t* source, wchar_t* pointerToStore, int sizeWithoutHeader)
		{
			// convert little to big endian if necessary
			if (sizeof(src_wchar_t) > 1 &&
				isLittleEndian(TargetFormat) != isLittleEndian(SourceFormat))
				convertToLittleEndian(source);

			// check if conversion is necessary:
			if (sizeof(src_wchar_t) == sizeof(wchar_t))
			{
				// no need to convert
				TextBegin = (wchar_t*)source;
				TextData = (wchar_t*)pointerToStore;
				TextSize = sizeWithoutHeader;
			}
			else
			{
				// convert source into target data format.
				// TODO: implement a real conversion. This one just
				// copies bytes. This is a problem when there are
				// unicode symbols using more than one wchar_tacter.

				TextData = new wchar_t[sizeWithoutHeader];

				if (sizeof(src_wchar_t) == 1)
				{
					// we have to cast away negative numbers or results might add the sign instead of just doing a copy
					for (int i = 0; i<sizeWithoutHeader; ++i)
					{
						TextData[i] = static_cast<wchar_t>(static_cast<unsigned wchar_t>(source[i]));
					}
				}
				else
				{
					for (int i = 0; i<sizeWithoutHeader; ++i)
						TextData[i] = static_cast<wchar_t>(source[i]);
				}
				TextBegin = TextData;
				TextSize = sizeWithoutHeader;

				// delete original data because no longer needed
				delete[] pointerToStore;
			}
		}

		//! converts whole text buffer to little endian
		template<class src_wchar_t>
		void convertToLittleEndian(src_wchar_t* t)
		{
			if (sizeof(src_wchar_t) == 4)
			{
				// 32 bit

				while (*t)
				{
					*t = ((*t & 0xff000000) >> 24) |
						((*t & 0x00ff0000) >> 8) |
						((*t & 0x0000ff00) << 8) |
						((*t & 0x000000ff) << 24);
					++t;
				}
			}
			else
			{
				// 16 bit

				while (*t)
				{
					*t = (*t >> 8) | (*t << 8);
					++t;
				}
			}
		}

		//! returns if a format is little endian
		inline bool isLittleEndian(ETEXT_FORMAT f)
		{
			return f == ETF_ASCII ||
				f == ETF_UTF8 ||
				f == ETF_UTF16_LE ||
				f == ETF_UTF32_LE;
		}


		//! returns true if a wchar_tacter is whitespace
		inline bool isWhiteSpace(wchar_t c)
		{
			return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
		}


		//! generates a list with xml special wchar_tacters
		void createSpecialCharacterList()
		{
			// list of strings containing special symbols,
			// the first wchar_tacter is the special wchar_tacter,
			// the following is the symbol string without trailing &.

			SpecialCharacters.push_back(L"&amp;");
			SpecialCharacters.push_back(L"<lt;");
			SpecialCharacters.push_back(L">gt;");
			SpecialCharacters.push_back(L"\"quot;");
			SpecialCharacters.push_back(L"'apos;");

		}


		//! compares the first n wchar_tacters of the strings
		bool equalsn(const wchar_t* str1, const wchar_t* str2, int len)
		{
			int i;
			for (i = 0; str1[i] && str2[i] && i < len; ++i)
				if (str1[i] != str2[i])
					return false;

			// if one (or both) of the strings was smaller then they
			// are only equal if they have the same lenght
			return (i == len) || (str1[i] == 0 && str2[i] == 0);
		}


		//! stores the target text format
		void storeTargetFormat()
		{
			// get target format. We could have done this using template specialization,
			// but VisualStudio 6 don't like it and we want to support it.

			switch (sizeof(wchar_t))
			{
			case 1:
				TargetFormat = ETF_UTF8;
				break;
			case 2:
				TargetFormat = ETF_UTF16_LE;
				break;
			case 4:
				TargetFormat = ETF_UTF32_LE;
				break;
			default:
				TargetFormat = ETF_ASCII; // should never happen.
			}
		}


		// instance variables:
		bool IgnoreWhitespaceText;	 // do not return EXN_TEXT nodes for pure whitespace
		wchar_t* TextData;         // data block of the text file
		wchar_t* P;                // current point in text to parse
		wchar_t* TextBegin;        // start of text to parse
		unsigned int TextSize;       // size of text to parse in wchar_tacters, not bytes

		EXML_NODE CurrentNodeType;   // type of the currently parsed node
		ETEXT_FORMAT SourceFormat;   // source format of the xml file
		ETEXT_FORMAT TargetFormat;   // output format of this parser

		StringW NodeName;    // name of the node currently in - also used for text
		StringW EmptyStringW; // empty string to be returned by getSafe() methods

		bool IsEmptyElement;       // is the currently parsed node empty?

		vector<StringW>::type SpecialCharacters; // see createSpecialCharacterList()
		vector<SAttribute>::type Attributes; // attributes of current element

	}; // end CXMLReaderImpl
}

#endif