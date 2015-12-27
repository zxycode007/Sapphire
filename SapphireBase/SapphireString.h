#ifndef _SAPPHIRE_String__
#define _SAPPHIRE_String__

#include "SapphirePrerequisites.h"
#include <string.h>

// 如果使用GCC 3.1 标准库
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC && SAPPHIRE_COMP_VER >= 310 && !defined(STLPORT)

#   if SAPPHIRE_COMP_VER >= 430
#       include <tr1/unordered_map> 
#   else
#       include <ext/hash_map>
namespace __gnu_cxx
{
	template <> struct hash< Sapphire::_StringBase >
	{
		size_t operator()(const Sapphire::_StringBase _stringBase) const
		{
			/* 这是增强STL的方法，但它在VC7.1存在问题并且有其他原因
			hash<const char*> H;
			return H(_stringBase.c_str());
			*/
			register size_t ret = 0;     //寄存器类型
			for (Sapphire::_StringBase::const_iterator it = _stringBase.begin(); it != _stringBase.end(); ++it)
				ret = 5 * ret + *it;

			return ret;
		}
	};
}
#   endif

#endif

namespace Sapphire {
	

	/** 手动控制字符串的类  */
	class _SapphireExport StringUtil
	{
	public:
		typedef StringStream StrStreamType;

		/** 
		删除字符串的空格或TAB
		@remarks
		用户需要指定只移除字符串的开始和结尾，默认是两个都移除
		*/
		static void trim(String& str, bool left = true, bool right = true);

		/** 
		传递一个参数delim，
		返回一个StringVector容器，其中包含被拆分的字符串。
		@param
		delims 一个用于拆分字符串的分割串
		@param
		maxSplits 最大分割数量0是不限制，如果该参数大于0，分割操作将从左到右分割这么多次后停止
		@param
		preserveDelims Flag   决定分隔符是否保留子串
		*/
		static vector<String>::type split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0, bool preserveDelims = false);

		/** 
		返回一个StringVector容器，包含通过delims或者doubleDelims参数拆分的所有拆分子串
		可以来代表一个字符串，如"strings like this"
		@param
		delims 一个要被拆分的字符串
		@param
		delims 一个双拆分字符串去拆分的token列表
		@param
		maxSplits 最大分割数量0是不限制，如果该参数大于0，分割操作将从左到右分割这么多次后停止
		*/
		static vector<String>::type tokenise(const String& str, const String& delims = "\t\n ", const String& doubleDelims = "\"", unsigned int maxSplits = 0);

		/** 转换字符串到小写字符
		*/
		static void toLowerCase(String& str);

		/**转换字符串到大写字符
		*/
		static void toUpperCase(String& str);


		/** 进行模式匹配返回是否是由传入字符串开头
		@param pattern 要进行模式比较的字符串
		@param lowerCase 如果为true，在比较前将字符串开头转为小写，模式匹配应该在小写模式
		*/
		static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** 进行模式匹配返回是否是由传入字符串结尾
		@param pattern 要进行模式比较的字符串
		@param lowerCase如果为true，在比较前将字符串开头转为小写，模式匹配应该在小写模式
		*/
		static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** 标准化路径的方法，只用于斜杠， 斜杠结尾
		*/
		static String standardisePath(const String &init);


		/** 返回一个文件路径的正规版本
		这个函数被用做构建文件路径字符串，它主席同样的路径
		它会转换所有的后斜杠到前斜杠
		移除重复的斜杠
		移除路径初始的斜杠
		移除".\" 和 "..\" 元路径
		设置所有字符串到小写（如果需要的话）
		@param init 要标准化的文件路径
		@param makeLowerCase 如果为true，转换所有字符串到小写
		*/
		static String normalizeFilePath(const String& init, bool makeLowerCase = true);


		/** 分割一个完整限定文件名到一个基本文件名和路径
		@remarks
		路径是标准化的路径
		*/
		static void splitFilename(const String& qualifiedName,
			String& outBasename, String& outPath);

		/** 
		分割一个完整限定文件名到一个基本文件名到基本文件名，还有路径，扩展名
		*/
		static void splitFullFilename(const String& qualifiedName,
			String& outBasename, String& outExtention,
			String& outPath);

		/** 
		分割一个文件名到基础文件和扩展名
		*/
		static void splitBaseFilename(const String& fullName,
			String& outBasename, String& outExtention);


		/** 
		简单日常模式匹配允许一个通配符进行匹配
		@param str 测试字符串
		@param pattern 匹配字符串;包括‘*’这类通配符
		@param caseSensitive 是否对大小写敏感
		*/
		static bool match(const String& str, const String& pattern, bool caseSensitive = true);


		/** 
		用一个另外的子串替换掉实例中所有的子串
		@param source 源字符串
		@param replaceWhat 要替换的字符串
		@param replaceWithWhat 用于替换的字符串
		@return 一个更新后的字符串
		*/
		static const String replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat);

		/**
		比较两个字符串是否相同
		@param  源字符串
		@param  目的字符串
		@param  是否区分大小写（默认区分）
		*/
		static const bool equals(const String& source, const String& other, bool caseSensitive = true);
		

		//! 比较源字符串的子串忽略大小写
		/** \param other: 另外一个要比较的字符串
		\param sourcePos: 源字符串要比较起始地址
		\param caseSensitive: 是否区分大小 (默认区分)
		\return 如果字符串比较成功返回TRUE */
		static const bool equalsSubString(const String&source, const String&other, const SINT32 sourceStartPos = 0, bool caseSensitive = true);
		

		/// 空字符串
		static const String BLANK;
		
	};


	

	//! 用来在fast_atof统计小数点的字符的选择

	_SapphireExport extern String LOCALE_DECIMAL_POINTS;

	const float fast_atof_table[17] = {
		0.f,
		0.1f,
		0.01f,
		0.001f,
		0.0001f,
		0.00001f,
		0.000001f,
		0.0000001f,
		0.00000001f,
		0.000000001f,
		0.0000000001f,
		0.00000000001f,
		0.000000000001f,
		0.0000000000001f,
		0.00000000000001f,
		0.000000000000001f,
		0.0000000000000001f
	};

	//! 转换一个简单的基于10进制的字符串到一个32位unsigned int
	/** \param[in] in: 要转换的字符串。不允许前导字符， 只有数位0到9。 解析到第一个非数字时停止
	\param[out] out: 可选， 如果提供，它会设置指针指向第一个字符，而不能用于计算
	\return 这个unsigned int值。如果字符串在一个UINT32指定太多的数位，它将返回INT_MAX
	*/
	inline UINT32 strtoul10(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0;
		}

		bool overflow = false;
		UINT32 unsignedValue = 0;
		while ((*in >= '0') && (*in <= '9'))
		{
			const UINT32 tmp = (unsignedValue * 10) + (*in - '0');
			if (tmp<unsignedValue)
			{
				unsignedValue = (UINT32)0xffffffff;
				overflow = true;
			}
			if (!overflow)
				unsignedValue = tmp;
			++in;
		}

		if (out)
			*out = in;

		return unsignedValue;
	}

	//!转换一个简单的基于10进制的字符串到一个32位 int
	/** \param[in] in: 要转换的字符串。只能以-,+开头，然后接着数字0到9，解析到第一个非数字时停止
	\param[out] out: 可选， 如果提供，它会设置指针指向第一个字符，而不能用于计算
	\return 这个SINT的数位值，如果字符串在一个SINT32指定太多的数位，它将返回+INT_MAX或-INT_MAX
	*/
	inline SINT32 strtol10(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0;
		}

		const bool negative = ('-' == *in);
		if (negative || ('+' == *in))
			++in;

		const UINT32 unsignedValue = strtoul10(in, out);
		if (unsignedValue > (UINT32)INT_MAX)
		{
			if (negative)
				return (SINT32)INT_MIN;
			else
				return (SINT32)INT_MAX;
		}
		else
		{
			if (negative)
				return -((SINT32)unsignedValue);
			else
				return (SINT32)unsignedValue;
		}
	}

	//! 转换一个16进制编码的字符串到一个unsigned int
	/** \param[in] in 要转换的数字。 只能是0到9和字符A到F和a到f
	\return 这个unsigned int的数字。 如果输入不是16进制，返回0xffffffff
	*/
	inline UINT32 ctoul16(char in)
	{
		if (in >= '0' && in <= '9')
			return in - '0';
		else if (in >= 'a' && in <= 'f')
			return 10u + in - 'a';
		else if (in >= 'A' && in <= 'F')
			return 10u + in - 'A';
		else
			return 0xffffffff;
	}

	//! 转换一个基于16进制的字符串到一个32位unsigned int
	/** \param[in] in: 要转换的字符串 只允许0到9和A到F(a到f)。解析到第一个无效字符停止
	\param[out] out: 可选， 如果提供，它会设置指针指向第一个字符，而不能用于计算
	\return 这个unsigned int数字。 如果字符串指定太多位，返回INT_MAX
	*/
	inline UINT32 strtoul16(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0;
		}

		bool overflow = false;
		UINT32 unsignedValue = 0;
		while (true)
		{
			UINT32 tmp = 0;
			if ((*in >= '0') && (*in <= '9'))
				tmp = (unsignedValue << 4u) + (*in - '0');
			else if ((*in >= 'A') && (*in <= 'F'))
				tmp = (unsignedValue << 4u) + (*in - 'A') + 10;
			else if ((*in >= 'a') && (*in <= 'f'))
				tmp = (unsignedValue << 4u) + (*in - 'a') + 10;
			else
				break;
			if (tmp<unsignedValue)
			{
				unsignedValue = (UINT32)INT_MAX;
				overflow = true;
			}
			if (!overflow)
				unsignedValue = tmp;
			++in;
		}

		if (out)
			*out = in;

		return unsignedValue;
	}

	//! 转换一个简单8位数字的字符串到一个32位unsigned int
	/** \param[in] 要转换的字符串 只允许0到7。解析到第一个无效字符停止
	\param[out] out 可选， 如果提供，它会设置指针指向第一个字符，而不能用于计算
	\return 这个unsigned int数字。 如果字符串指定太多位，返回INT_MAX
	*/
	inline UINT32 strtoul8(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0;
		}

		bool overflow = false;
		UINT32 unsignedValue = 0;
		while (true)
		{
			UINT32 tmp = 0;
			if ((*in >= '0') && (*in <= '7'))
				tmp = (unsignedValue << 3u) + (*in - '0');
			else
				break;
			if (tmp<unsignedValue)
			{
				unsignedValue = (UINT32)INT_MAX;
				overflow = true;
			}
			if (!overflow)
				unsignedValue = tmp;
			++in;
		}

		if (out)
			*out = in;

		return unsignedValue;
	}

	//! 转换C类型字符串数值（16，8，10位整数）到一个32位unsigned int 
	/** \param[in]  要转换的字符串,如果字符串以0x开始，使用16进制解析器。
	如果0开始，使用8进制解析器，其它用10进制解析器，解析到第一个无效字符停止。
	\param[out] 可选， 如果提供，它会设置指针指向第一个字符，而不能用于计算
	\return 这个unsigned int数字。 如果字符串指定太多位，返回INT_MAX
	*/
	inline UINT32 strtoul_prefix(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0;
		}
		if ('0' == in[0])
			return ('x' == in[1] ? strtoul16(in + 2, out) : strtoul8(in + 1, out));
		return strtoul10(in, out);
	}

	//! 转换一个数字序列到一个正浮点数
	/** 只能够解析数字0到9，在解析到任何其它数字停止，包括符合字符或小数点
	\param in: 要转换的数字序列
	\param out: (可选) 将设置指向第一个没转换字符的指针
	\return 这个数字序列表示的整个正浮点数
	*/
	inline Real strtof10(const char* in, const char** out = 0)
	{
		if (!in)
		{
			if (out)
				*out = in;
			return 0.f;
		}

		const UINT32 MAX_SAFE_U32_VALUE = UINT_MAX / 10 - 10;
		UINT32 intValue = 0;


		while ((*in >= '0') && (*in <= '9'))
		{
			// 注意溢出
			if (intValue >= MAX_SAFE_U32_VALUE)
				break;

			intValue = (intValue * 10) + (*in - '0');
			++in;
		}

		Real floatValue = (Real)intValue;


		while ((*in >= '0') && (*in <= '9'))
		{
			floatValue = (floatValue * 10.f) + (Real)(*in - '0');
			++in;
			if (floatValue > FLT_MAX)
				break;
		}

		if (out)
			*out = in;

		return floatValue;
	}

	//! 提供一个快速转换一个字符串到一个浮点型的函数
	/** 这不能保证比atof()精确，但是快6到8倍
	\param[in] in 要转换的字符串.
	\param[out] 结果的浮点写入到这里
	\return 指向字符串首个字符的指针，不是用来创建浮点值的
	*/
	inline const char* fast_atof_move(const char* in, Real& result)
	{

		//当这个函数进行任何修改时，请运行回归测试。

		result = 0.f;
		if (!in)
			return 0;

		const bool negative = ('-' == *in);
		if (negative || ('+' == *in))
			++in;

		Real value = strtof10(in, &in);

		if (LOCALE_DECIMAL_POINTS.find(*in) >= 0)
		{
			const char* afterDecimal = ++in;
			const Real decimal = strtof10(in, &afterDecimal);
			value += decimal * fast_atof_table[afterDecimal - in];
			in = afterDecimal;
		}

		if ('e' == *in || 'E' == *in)
		{
			++in;
			// 假设指数是一个完整的数
			// strtol10()会处理+和-号
			// 但是计算float 要防止FLT_MAX溢出
			value *= powf(10.f, (Real)strtol10(in, &in));
		}

		result = negative ? -value : value;
		return in;
	}

	//! 转换一个字符串到一个浮点数
	/** \param floatAsString 这个要转换的字符串
	\param out(可选) 将设置指向第一个没转换字符的指针
	\result 输入字符串的浮点值
	*/
	inline float fast_atof(const char* floatAsString, const char** out = 0)
	{
		Real ret;
		if (out)
			*out = fast_atof_move(floatAsString, ret);
		else
			fast_atof_move(floatAsString, ret);
		return ret;
	}

#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC && SAPPHIRE_COMP_VER >= 310 && !defined(STLPORT)
#   if SAPPHIRE_COMP_VER < 430
	typedef ::__gnu_cxx::hash< _StringBase > _StringHash;
#   else
	typedef ::std::tr1::hash< _StringBase > _StringHash;
#   endif
#elif SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_CLANG
#   if defined(_LIBCPP_VERSION)
	typedef ::std::hash< _StringBase > _StringHash;
#   else
	typedef ::std::tr1::hash< _StringBase > _StringHash;
#   endif
#elif SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC && SAPPHIRE_COMP_VER >= 1600 && !defined(STLPORT) // VC++ 10.0
	typedef ::std::tr1::hash< _StringBase > _StringHash;
#elif !defined( _STLP_HASH_FUN_H )
	typedef stdext::hash_compare< _StringBase, std::less< _StringBase > > _StringHash;
#else
	typedef std::hash< _StringBase > _StringHash;
#endif 
	/** @} */
	/** @} */

}  

#endif  
