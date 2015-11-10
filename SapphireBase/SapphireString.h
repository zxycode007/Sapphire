#ifndef _SAPPHIRE_String__
#define _SAPPHIRE_String__

#include "SapphirePrerequisites.h"

// ���ʹ��GCC 3.1 ��׼��
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
			/* ������ǿSTL�ķ�����������VC7.1�������Ⲣ��������ԭ��
			hash<const char*> H;
			return H(_stringBase.c_str());
			*/
			register size_t ret = 0;     //�Ĵ�������
			for (Sapphire::_StringBase::const_iterator it = _stringBase.begin(); it != _stringBase.end(); ++it)
				ret = 5 * ret + *it;

			return ret;
		}
	};
}
#   endif

#endif

namespace Sapphire {
	

	/** �ֶ������ַ�������  */
	class _SapphireExport StringUtil
	{
	public:
		typedef StringStream StrStreamType;

		/** 
		ɾ���ַ����Ŀո��TAB
		@remarks
		�û���Ҫָ��ֻ�Ƴ��ַ����Ŀ�ʼ�ͽ�β��Ĭ�����������Ƴ�
		*/
		static void trim(String& str, bool left = true, bool right = true);

		/** 
		����һ������delim��
		����һ��StringVector���������а�������ֵ��ַ�����
		@param
		delims һ�����ڲ���ַ����ķָ
		@param
		maxSplits ���ָ�����0�ǲ����ƣ�����ò�������0���ָ�����������ҷָ���ô��κ�ֹͣ
		@param
		preserveDelims Flag   �����ָ����Ƿ����Ӵ�
		*/
		static vector<String>::type split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0, bool preserveDelims = false);

		/** 
		����һ��StringVector����������ͨ��delims����doubleDelims������ֵ����в���Ӵ�
		����������һ���ַ�������"strings like this"
		@param
		delims һ��Ҫ����ֵ��ַ���
		@param
		delims һ��˫����ַ���ȥ��ֵ�token�б�
		@param
		maxSplits ���ָ�����0�ǲ����ƣ�����ò�������0���ָ�����������ҷָ���ô��κ�ֹͣ
		*/
		static vector<String>::type tokenise(const String& str, const String& delims = "\t\n ", const String& doubleDelims = "\"", unsigned int maxSplits = 0);

		/** ת���ַ�����Сд�ַ�
		*/
		static void toLowerCase(String& str);

		/**ת���ַ�������д�ַ�
		*/
		static void toUpperCase(String& str);


		/** ����ģʽƥ�䷵���Ƿ����ɴ����ַ�����ͷ
		@param pattern Ҫ����ģʽ�Ƚϵ��ַ���
		@param lowerCase ���Ϊtrue���ڱȽ�ǰ���ַ�����ͷתΪСд��ģʽƥ��Ӧ����Сдģʽ
		*/
		static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** ����ģʽƥ�䷵���Ƿ����ɴ����ַ�����β
		@param pattern Ҫ����ģʽ�Ƚϵ��ַ���
		@param lowerCase���Ϊtrue���ڱȽ�ǰ���ַ�����ͷתΪСд��ģʽƥ��Ӧ����Сдģʽ
		*/
		static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** ��׼��·���ķ�����ֻ����б�ܣ� б�ܽ�β
		*/
		static String standardisePath(const String &init);


		/** ����һ���ļ�·��������汾
		������������������ļ�·���ַ���������ϯͬ����·��
		����ת�����еĺ�б�ܵ�ǰб��
		�Ƴ��ظ���б��
		�Ƴ�·����ʼ��б��
		�Ƴ�".\" �� "..\" Ԫ·��
		���������ַ�����Сд�������Ҫ�Ļ���
		@param init Ҫ��׼�����ļ�·��
		@param makeLowerCase ���Ϊtrue��ת�������ַ�����Сд
		*/
		static String normalizeFilePath(const String& init, bool makeLowerCase = true);


		/** �ָ�һ�������޶��ļ�����һ�������ļ�����·��
		@remarks
		·���Ǳ�׼����·��
		*/
		static void splitFilename(const String& qualifiedName,
			String& outBasename, String& outPath);

		/** 
		�ָ�һ�������޶��ļ�����һ�������ļ����������ļ���������·������չ��
		*/
		static void splitFullFilename(const Sapphire::String& qualifiedName,
			Sapphire::String& outBasename, Sapphire::String& outExtention,
			Sapphire::String& outPath);

		/** 
		�ָ�һ���ļ����������ļ�����չ��
		*/
		static void splitBaseFilename(const Sapphire::String& fullName,
			Sapphire::String& outBasename, Sapphire::String& outExtention);


		/** 
		���ճ�ģʽƥ������һ��ͨ�������ƥ��
		@param str �����ַ���
		@param pattern ƥ���ַ���;������*������ͨ���
		@param caseSensitive �Ƿ�Դ�Сд����
		*/
		static bool match(const String& str, const String& pattern, bool caseSensitive = true);


		/** 
		��һ��������Ӵ��滻��ʵ�������е��Ӵ�
		@param source Դ�ַ���
		@param replaceWhat Ҫ�滻���ַ���
		@param replaceWithWhat �����滻���ַ���
		@return һ�����º���ַ���
		*/
		static const String replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat);

		/// ���ַ���
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
