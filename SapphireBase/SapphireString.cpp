
//#include "SapphireStableHeaders.h"
#include "SapphireString.h"
#include "SapphireStringVector.h"

namespace Sapphire {

	//-----------------------------------------------------------------------
	const String StringUtil::BLANK;
	//-----------------------------------------------------------------------
	void StringUtil::trim(String& str, bool left, bool right)
	{
		/*
		size_t lspaces, rspaces, len = length(), i;

		lspaces = rspaces = 0;

		if( left )
		{
		// Find spaces / tabs on the left
		for( i = 0;
		i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
		++lspaces, ++i );
		}

		if( right && lspaces < len )
		{
		// Find spaces / tabs on the right
		for( i = len - 1;
		i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
		rspaces++, i-- );
		}

		*this = substr(lspaces, len-lspaces-rspaces);
		*/
		static const String delims = " \t\r";
		if (right)
			str.erase(str.find_last_not_of(delims) + 1); // trim right
		if (left)
			str.erase(0, str.find_first_not_of(delims)); // trim left
	}

	//-----------------------------------------------------------------------
	StringVector StringUtil::split(const String& str, const String& delims, unsigned int maxSplits, bool preserveDelims)
	{
		StringVector ret;
		// Ϊ��������ǰ����һЩ�ռ�
		ret.reserve(maxSplits ? maxSplits + 1 : 10);    // ����maxSplits����ռ�������Ĭ��Ϊ10

		unsigned int numSplits = 0;

		// ʹ��STL����
		size_t start, pos;
		start = 0;
		do 
		{
			//���ҳ��ָ��ַ�����λ��
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				start = pos + 1;
			}
			else if (pos == String::npos || (maxSplits && numSplits == maxSplits))     //û�ҵ�
			{
				// ������ַ���ԭ�ⲻ��ѹ������
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				// ���ǰ�벿�����ַ���ѹ������
				ret.push_back(str.substr(start, pos - start));

				if (preserveDelims)  //�Ƿ����Ӵ�
				{
					// ��ʱ����һ�����ж���һ���ָ���
					// ѭ��ֱ���ҳ����еķָ���
					size_t delimStart = pos, delimPos;
					delimPos = str.find_first_not_of(delims, delimStart);
					if (delimPos == String::npos)
					{
						 
						ret.push_back(str.substr(delimStart));
					}
					else
					{
						ret.push_back(str.substr(delimStart, delimPos - delimStart));
					}
				}

				start = pos + 1;
			}
			 
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != String::npos);

 

		return ret;
	}
	//-----------------------------------------------------------------------
	StringVector StringUtil::tokenise(const String& str, const String& singleDelims, const String& doubleDelims, unsigned int maxSplits)
	{
		StringVector ret;
		   
		ret.reserve(maxSplits ? maxSplits + 1 : 10);   

		unsigned int numSplits = 0;
		String delims = singleDelims + doubleDelims;

		 
		size_t start, pos;
		char curDoubleDelim = 0;
		start = 0;
		do
		{
			if (curDoubleDelim != 0)
			{
				pos = str.find(curDoubleDelim, start);
			}
			else
			{
				pos = str.find_first_of(delims, start);
			}

			if (pos == start)
			{
				char curDelim = str.at(pos);
				if (doubleDelims.find_first_of(curDelim) != String::npos)
				{
					curDoubleDelim = curDelim;
				}
				 
				start = pos + 1;
			}
			else if (pos == String::npos || (maxSplits && numSplits == maxSplits))  //��ĩβ
			{
				if (curDoubleDelim != 0)
				{
					 
				}
				 
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				if (curDoubleDelim != 0)
				{
					curDoubleDelim = 0;
				}

				 
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			if (curDoubleDelim == 0)
			{
				 
				start = str.find_first_not_of(singleDelims, start);
			}

			++numSplits;

		} while (start != String::npos);

		return ret;
	}
	//-----------------------------------------------------------------------
	void StringUtil::toLowerCase(String& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			tolower);
	}

	//-----------------------------------------------------------------------
	void StringUtil::toUpperCase(String& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			toupper);
	}
	//-----------------------------------------------------------------------
	bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		String startOfThis = str.substr(0, patternLen);
		if (lowerCase)
			StringUtil::toLowerCase(startOfThis);

		return (startOfThis == pattern);
	}
	//-----------------------------------------------------------------------
	bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		String endOfThis = str.substr(thisLen - patternLen, patternLen);
		if (lowerCase)
			StringUtil::toLowerCase(endOfThis);

		return (endOfThis == pattern);
	}
	//-----------------------------------------------------------------------
	String StringUtil::standardisePath(const String& init)
	{
		String path = init;

		std::replace(path.begin(), path.end(), '\\', '/');
		if (path[path.length() - 1] != '/')
			path += '/';

		return path;
	}
	//-----------------------------------------------------------------------
	String StringUtil::normalizeFilePath(const String& init, bool makeLowerCase)
	{
		const char* bufferSrc = init.c_str();
		int pathLen = (int)init.size();
		int indexSrc = 0;
		int indexDst = 0;
		int metaPathArea = 0;

		char reservedBuf[1024];
		char* bufferDst = reservedBuf;
		bool isDestAllocated = false;
		if (pathLen > 1023)
		{
			//���Դ·��̫��̫����ȷ�����ᳬ������Ļ�����
			isDestAllocated = true;
			bufferDst = new char[pathLen + 1];
		}

		
		while (indexSrc < pathLen)
		{
			if ((bufferSrc[indexSrc] == '\\') || (bufferSrc[indexSrc] == '/'))
			{
				//�����һ��Ŀ¼�ָ�����������
				++indexSrc;
				continue;
			}
			else
			{
				//�������һ�� ".\"
				if ((bufferSrc[indexSrc] == '.') &&
					((bufferSrc[indexSrc + 1] == '\\') || (bufferSrc[indexSrc + 1] == '/')))
				{
					indexSrc += 2;
					continue;
				}

				// ����������һ��"..\"
				else if ((bufferSrc[indexSrc] == '.') && (bufferSrc[indexSrc + 1] == '.') &&
					((bufferSrc[indexSrc + 2] == '\\') || (bufferSrc[indexSrc + 2] == '/')))
				{
					if (indexDst > metaPathArea)
					{
						
						do {
							--indexDst;
						} while ((indexDst > metaPathArea) && (bufferDst[indexDst - 1] != '/'));
						indexSrc += 3;
						continue;
					}
					else
					{
						 
						metaPathArea += 3;
					}
				}
			}

			//��Դ��Ŀ��ת����ǰĿ¼��
			while (indexSrc < pathLen)
			{
				char curChar = bufferSrc[indexSrc];
				if (makeLowerCase) curChar = tolower(curChar);
				if ((curChar == '\\') || (curChar == '/')) curChar = '/';
				bufferDst[indexDst] = curChar;
				++indexDst;
				++indexSrc;
				if (curChar == '/') break;
			}
		}
		bufferDst[indexDst] = 0;

		String normalized(bufferDst);
		if (isDestAllocated)
		{
			delete[] bufferDst;
		}

		return normalized;
	}
	//-----------------------------------------------------------------------
	void StringUtil::splitFilename(const String& qualifiedName,
		String& outBasename, String& outPath)
	{
		String path = qualifiedName;
		// ���滻\�滻/
		std::replace(path.begin(), path.end(), '\\', '/');
		// ��/�ָ�
		size_t i = path.find_last_of('/');

		if (i == String::npos)
		{
			outPath.clear();
			outBasename = qualifiedName;
		}
		else
		{
			outBasename = path.substr(i + 1, path.size() - i - 1);
			outPath = path.substr(0, i + 1);
		}

	}
	//-----------------------------------------------------------------------
	void StringUtil::splitBaseFilename(const Sapphire::String& fullName,
		Sapphire::String& outBasename, Sapphire::String& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == Sapphire::String::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i + 1);
			outBasename = fullName.substr(0, i);
		}
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	void StringUtil::splitFullFilename(const Sapphire::String& qualifiedName,
		Sapphire::String& outBasename, Sapphire::String& outExtention, Sapphire::String& outPath)
	{
		Sapphire::String fullName;
		splitFilename(qualifiedName, fullName, outPath);
		splitBaseFilename(fullName, outBasename, outExtention);
	}
	//-----------------------------------------------------------------------
	bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
	{
		String tmpStr = str;
		String tmpPattern = pattern;
		if (!caseSensitive)
		{
			StringUtil::toLowerCase(tmpStr);
			StringUtil::toLowerCase(tmpPattern);
		}

		String::const_iterator strIt = tmpStr.begin();
		String::const_iterator patIt = tmpPattern.begin();
		String::const_iterator lastWildCardIt = tmpPattern.end();
		while (strIt != tmpStr.end() && patIt != tmpPattern.end())
		{
			if (*patIt == '*')
			{
				lastWildCardIt = patIt;
				++patIt;
				if (patIt == tmpPattern.end())
				{
					strIt = tmpStr.end();
				}
				else
				{
					// ɨ��֪���ҵ���һ��ƥ���ַ���
					while (strIt != tmpStr.end() && *strIt != *patIt)
						++strIt;
				}
			}
			else
			{
				if (*patIt != *strIt)
				{
					if (lastWildCardIt != tmpPattern.end())
					{
						// ͨ����ܹ�ƥ�������ȷ������
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						 
						return false;
					}
				}
				else
				{
					++patIt;
					++strIt;
				}
			}

		}
		// �����������ƥ���ַ�����ĩβ
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	//-----------------------------------------------------------------------
	const String StringUtil::replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat)
	{
		String result = source;
		String::size_type pos = 0;
		while (1)
		{
			pos = result.find(replaceWhat, pos);
			if (pos == String::npos) break;
			result.replace(pos, replaceWhat.size(), replaceWithWhat);
			pos += replaceWithWhat.size();
		}
		return result;
	}


	const bool StringUtil::equalsSubString(const String&source, const String&other, const SINT32 sourceStartPos, bool caseSensitive) 
	{
		//��ʼλ�ó���Դ�ַ�����󳤶ȣ��Ƚ�ʧ�ܷ���
		if ((UINT32)sourceStartPos >= source.size())
			return false;

		UINT32 i;
		String _s1 = source;
		String _s2 = other;
		//toLowerCase(_s1);
		//toLowerCase(_s2);


		return equals(_s1, _s2, caseSensitive);
	}

	const bool StringUtil::equals(const String& source, const String& other, bool caseSensitive)
	{
		if (caseSensitive)
		{
			return source == other;
		}
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
		return stricmp(source.c_str(), other.c_str()) == 0;
#else
#include <strings.h>
		return strncasecmp(source.c_str(), other.c_str()) == 0;
#endif
		return false;
	}

}
