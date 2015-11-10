#ifndef _SAPPHIRE_String__
#define _SAPPHIRE_String__

#include "SapphirePrerequisites.h"

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
		static void splitFullFilename(const Sapphire::String& qualifiedName,
			Sapphire::String& outBasename, Sapphire::String& outExtention,
			Sapphire::String& outPath);

		/** 
		分割一个文件名到基础文件和扩展名
		*/
		static void splitBaseFilename(const Sapphire::String& fullName,
			Sapphire::String& outBasename, Sapphire::String& outExtention);


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

		/// 空字符串
		static const String BLANK;
	};


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
