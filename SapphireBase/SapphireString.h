#ifndef _SAPPHIRE_String__
#define _SAPPHIRE_String__

#include "SapphirePrerequisites.h"
#include <string.h>

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
		static void splitFullFilename(const String& qualifiedName,
			String& outBasename, String& outExtention,
			String& outPath);

		/** 
		�ָ�һ���ļ����������ļ�����չ��
		*/
		static void splitBaseFilename(const String& fullName,
			String& outBasename, String& outExtention);


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

		/**
		�Ƚ������ַ����Ƿ���ͬ
		@param  Դ�ַ���
		@param  Ŀ���ַ���
		@param  �Ƿ����ִ�Сд��Ĭ�����֣�
		*/
		static const bool equals(const String& source, const String& other, bool caseSensitive = true);
		

		//! �Ƚ�Դ�ַ������Ӵ����Դ�Сд
		/** \param other: ����һ��Ҫ�Ƚϵ��ַ���
		\param sourcePos: Դ�ַ���Ҫ�Ƚ���ʼ��ַ
		\param caseSensitive: �Ƿ����ִ�С (Ĭ������)
		\return ����ַ����Ƚϳɹ�����TRUE */
		static const bool equalsSubString(const String&source, const String&other, const SINT32 sourceStartPos = 0, bool caseSensitive = true);
		

		/// ���ַ���
		static const String BLANK;
		
	};


	

	//! ������fast_atofͳ��С������ַ���ѡ��

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

	//! ת��һ���򵥵Ļ���10���Ƶ��ַ�����һ��32λunsigned int
	/** \param[in] in: Ҫת�����ַ�����������ǰ���ַ��� ֻ����λ0��9�� ��������һ��������ʱֹͣ
	\param[out] out: ��ѡ�� ����ṩ����������ָ��ָ���һ���ַ������������ڼ���
	\return ���unsigned intֵ������ַ�����һ��UINT32ָ��̫�����λ����������INT_MAX
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

	//!ת��һ���򵥵Ļ���10���Ƶ��ַ�����һ��32λ int
	/** \param[in] in: Ҫת�����ַ�����ֻ����-,+��ͷ��Ȼ���������0��9����������һ��������ʱֹͣ
	\param[out] out: ��ѡ�� ����ṩ����������ָ��ָ���һ���ַ������������ڼ���
	\return ���SINT����λֵ������ַ�����һ��SINT32ָ��̫�����λ����������+INT_MAX��-INT_MAX
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

	//! ת��һ��16���Ʊ�����ַ�����һ��unsigned int
	/** \param[in] in Ҫת�������֡� ֻ����0��9���ַ�A��F��a��f
	\return ���unsigned int�����֡� ������벻��16���ƣ�����0xffffffff
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

	//! ת��һ������16���Ƶ��ַ�����һ��32λunsigned int
	/** \param[in] in: Ҫת�����ַ��� ֻ����0��9��A��F(a��f)����������һ����Ч�ַ�ֹͣ
	\param[out] out: ��ѡ�� ����ṩ����������ָ��ָ���һ���ַ������������ڼ���
	\return ���unsigned int���֡� ����ַ���ָ��̫��λ������INT_MAX
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

	//! ת��һ����8λ���ֵ��ַ�����һ��32λunsigned int
	/** \param[in] Ҫת�����ַ��� ֻ����0��7����������һ����Ч�ַ�ֹͣ
	\param[out] out ��ѡ�� ����ṩ����������ָ��ָ���һ���ַ������������ڼ���
	\return ���unsigned int���֡� ����ַ���ָ��̫��λ������INT_MAX
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

	//! ת��C�����ַ�����ֵ��16��8��10λ��������һ��32λunsigned int 
	/** \param[in]  Ҫת�����ַ���,����ַ�����0x��ʼ��ʹ��16���ƽ�������
	���0��ʼ��ʹ��8���ƽ�������������10���ƽ���������������һ����Ч�ַ�ֹͣ��
	\param[out] ��ѡ�� ����ṩ����������ָ��ָ���һ���ַ������������ڼ���
	\return ���unsigned int���֡� ����ַ���ָ��̫��λ������INT_MAX
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

	//! ת��һ���������е�һ����������
	/** ֻ�ܹ���������0��9���ڽ������κ���������ֹͣ�����������ַ���С����
	\param in: Ҫת������������
	\param out: (��ѡ) ������ָ���һ��ûת���ַ���ָ��
	\return ����������б�ʾ��������������
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
			// ע�����
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

	//! �ṩһ������ת��һ���ַ�����һ�������͵ĺ���
	/** �ⲻ�ܱ�֤��atof()��ȷ�����ǿ�6��8��
	\param[in] in Ҫת�����ַ���.
	\param[out] ����ĸ���д�뵽����
	\return ָ���ַ����׸��ַ���ָ�룬����������������ֵ��
	*/
	inline const char* fast_atof_move(const char* in, Real& result)
	{

		//��������������κ��޸�ʱ�������лع���ԡ�

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
			// ����ָ����һ����������
			// strtol10()�ᴦ��+��-��
			// ���Ǽ���float Ҫ��ֹFLT_MAX���
			value *= powf(10.f, (Real)strtol10(in, &in));
		}

		result = negative ? -value : value;
		return in;
	}

	//! ת��һ���ַ�����һ��������
	/** \param floatAsString ���Ҫת�����ַ���
	\param out(��ѡ) ������ָ���һ��ûת���ַ���ָ��
	\result �����ַ����ĸ���ֵ
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
