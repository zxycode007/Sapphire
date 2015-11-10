#include "SapphirePrerequisites.h"
#include "SapphireCommon.h"
#include "SapphireLogManager.h"

namespace Sapphire
{
	int findCommandLineOpts(int numargs, char** argv, UnaryOptionList& unaryOptList,
		BinaryOptionList& binOptList)
	{
		int startIndex = 1;
		for (int i = 1; i < numargs; ++i)
		{
			String tmp(argv[i]);
			if (StringUtil::startsWith(tmp, "-"))
			{
				UnaryOptionList::iterator ui = unaryOptList.find(argv[i]);
				if (ui != unaryOptList.end())
				{
					ui->second = true;
					++startIndex;
					continue;
				}
				BinaryOptionList::iterator bi = binOptList.find(argv[i]);
				if (bi != binOptList.end())
				{
					bi->second = argv[i + 1];
					startIndex += 2;
					++i;
					continue;
				}

				// Invalid option
				LogManager::getSingleton().logMessage("Invalid option " + tmp);

			}
		}
		return startIndex;
	}


/**
通用的hash函数
来源
http://www.azillionmonkeys.com/qed/hash.html
*/
#if SAPPHIRE_ENDIAN == SAPPHIRE_ENDIAN_LITTLE
#  define SAPPHIRE_GET16BITS(d) (*((const uint16 *) (d)))
#else
	// 转换uint16到小字节序，意味这首个自己是按最低有效位
#  define SAPPHIRE_GET16BITS(d) (*((const uint8 *) (d)) + (*((const uint8 *) (d+1))<<8))
#endif
	uint32 _SapphireExport FastHash(const char * data, int len, uint32 hashSoFar)
	{
		uint32 hash;
		uint32 tmp;
		int rem;
		//以前的哈希值
		if (hashSoFar)
			hash = hashSoFar;
		else
			hash = len;

		if (len <= 0 || data == NULL) return 0;

		rem = len & 3;
		len >>= 2;

		/* 主循环 */
		for (; len > 0; len--) {
			hash += SAPPHIRE_GET16BITS(data);
			tmp = (SAPPHIRE_GET16BITS(data + 2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			data += 2 * sizeof(uint16);
			hash += hash >> 11;
		}

		/* 处理结束 */
		switch (rem) {
		case 3: hash += SAPPHIRE_GET16BITS(data);
			hash ^= hash << 16;
			hash ^= data[sizeof(uint16)] << 18;
			hash += hash >> 11;
			break;
		case 2: hash += SAPPHIRE_GET16BITS(data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1: hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
		}

		/* 强制最后127位的崩溃*/
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}

}