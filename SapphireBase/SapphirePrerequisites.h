/*

   ���ͷ�ļ�������sapphire��ҪԤ�����õ�������������Ϣ

   */
#ifndef __SAPPHIRE_PREREQUISITES__
#define __SAPPHIRE_PREREQUISITES__

#include <string>

//#include <string>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL ����
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

// STL �㷨 & ����
#include <algorithm>
#include <functional>
#include <limits>

// C++ ��
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

//�ⲿ�ְ�C�����룬�ⲿ����Ϊȫ�ֱ���ʹ��
extern "C" {

#   include <sys/types.h>
#   include <sys/stat.h>

}

//unordered_map��hashmap����
//#include <unordered_map>
#include "SapphireBuildSets.h"
#include "SapphireConfig.h"
#include "SapphirePlatfrom.h"

//���ݱ���������GCC,MSVC��CLANG��ѡ��ͬ������HashMap��HashSet
#if (SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC) && !defined(STLPORT)
#   if SAPPHIRE_COMP_VER >= 430
#       include <tr1/unordered_map>
#       include <tr1/unordered_set> 
#   else
#       include <ext/hash_map>
#       include <ext/hash_set>
#   endif
#elif (SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_CLANG)
#   if defined(_LIBCPP_VERSION)
#       include <unordered_map>
#       include <unordered_set>
#   else
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#   endif
#else
#   if (SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC) && !defined(STLPORT) && SAPPHIRE_COMP_VER >= 1600 // VC++ 10.0
#    	include <unordered_map>
#    	include <unordered_set>
#	else
#   	include <hash_set>
#   	include <hash_map>
#	endif
#endif 


#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
#  undef min
#  undef max
#  if defined( __MINGW32__ )
//����Unixϵͳ��ϵͳAPI���ýӿں���
#    include <unistd.h>
#  endif
#endif

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_LINUX || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_ANDROID
extern "C" {

#   include <unistd.h>
	//  ����linuxϵͳ�Ķ�̬���ӿ����ʾ����
#   include <dlfcn.h>

}
#endif

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE_IOS
extern "C" {
#   include <unistd.h>
#   include <sys/param.h>
#   include <CoreFoundation/CoreFoundation.h>
}
#endif



// �����ڴ����
#if SAPPHIRE_DEBUG_MODE 
#	if SAPPHIRE_MEMORY_TRACKER_DEBUG_MODE
#		define SAPPHIRE_MEMORY_TRACKER 1
#	else
#		define SAPPHIRE_MEMORY_TRACKER 0
#	endif
#else
#	if SAPPHIRE_MEMORY_TRACKER_RELEASE_MODE
#		define SAPPHIRE_MEMORY_TRACKER 1
#	else
#		define SAPPHIRE_MEMORY_TRACKER 0
#	endif
#endif


#define SAPPHIRE_SIMD_ALIGNMENT 16

namespace Sapphire
{
	//��Sapphire�ռ䣬Ĭ��ʹ��std��׼��
	using namespace std;

	// ����汾��Ϣ
#define SAPPHIRE_VERSION_MAJOR 0
#define SAPPHIRE_VERSION_MINOR 1
#define SAPPHIRE_VERSION_PATCH 1
#define SAPPHIRE_VERSION_SUFFIX ""
#define SAPPHIRE_VERSION_NAME "Sapphire"

#define SAPPHIRE_VERSION    ((SAPPHIRE_VERSION_MAJOR << 16) | (SAPPHIRE_VERSION_MINOR << 8) | SAPPHIRE_VERSION_PATCH)


	// ����Ҫ�õ���ʵ������
	// Ĭ��ʹ��float֪��Ԥ����ѡ��޸�
#if SAPPHIRE_DOUBLE_PRECISION == 1
	/** ��������
	@note ������ΪGPU ������/������ָ��
	*/
	typedef double Real;
#else
	/** ��������
	@note ������ΪGPU ������/������ָ��
	*/
	typedef float Real;
#endif




	//���ݱ�������������hashMap��hashSet
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_GNUC && SAPPHIRE_COMP_VER >= 310 && !defined(STLPORT)
#   if SAPPHIRE_COMP_VER >= 430
#       define HashMap ::std::tr1::unordered_map
#		define HashSet ::std::tr1::unordered_set
#    else
#       define HashMap ::__gnu_cxx::hash_map
#       define HashSet ::__gnu_cxx::hash_set
#    endif
#elif SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_CLANG
#    if defined(_LIBCPP_VERSION)
#       define HashMap ::std::unordered_map
#       define HashSet ::std::unordered_set
#    else
#       define HashMap ::std::tr1::unordered_map
#       define HashSet ::std::tr1::unordered_set
#    endif
#else
#   if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
#       if SAPPHIRE_COMP_VER >= 1600 // VC++ 10.0
#			define HashMap ::std::tr1::unordered_map
#           define HashSet ::std::tr1::unordered_set
#		elif SAPPHIRE_COMP_VER > 1300 && !defined(_STLP_MSVC)
#           define HashMap ::stdext::hash_map
#           define HashSet ::stdext::hash_set
#       else
#           define HashMap ::std::hash_map
#           define HashSet ::std::hash_set
#       endif
#   else
#       define HashMap ::std::hash_map
#       define HashSet ::std::hash_set
#   endif
#endif

	//��̬�Ѷ������
#define SAPPHIRE_DEFINE_STATIC_LOCAL(type, name, arguments) static type& name = *new type arguments
	//�ض�����������
	typedef unsigned char UCHAR;
	typedef unsigned short USHORT;
	typedef unsigned int UINT;
	typedef unsigned long ULONG;
	typedef int SINT;
	typedef __int32 SINT32;
	typedef __int64 SINT64;
	typedef unsigned __int64 UINT64;
	typedef unsigned __int32 UINT32;
	typedef unsigned __int8 UINT8;
	typedef char c8;   //8λchar
	typedef char s8;
	


#if SAPPHIRE_WCHAR_T_STRINGS == 0
	typedef char S_CHAR;
#else
	typedef wchar_t S_CHAR;
#endif

		//���̶߳���֧��
#include "threads\SapphireThreadDefines.h"

////////////////////////Ԥ������////////////////////////////////
	
	class Math;
	class Vector2;
	class Vector3;
	class Vector4;
	class Radian;
	class Degree;
	class Angle;
	class Plane;
	class Sphere;
	class PlaneBoundedVolume;
	class Ray;
	class Quaternion;
	class Matrix3;
	class Matrix4;
	class AxisAlignedBox;
	class Rectangle;
	
	
	
}

//�������������ú��Ժ������׼���ļ�
#include "SapphireStdHeader.h"
#include "SapphireKeyMap.h"
#include "SapphireDimension2D.h"
#include "SapphireDefines.h";
#include "SapphireSceneNodeTypes.h"
#include "SapphireMemoryAllocatorConfig.h"


namespace Sapphire
{

#if SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR        //�Ƿ�ʹ���Զ�����ڴ������
#if SAPPHIRE_WCHAR_T_STRINGS                 //�Ƿ�ʹ��WCHAR
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy > >	_StringBase;   //�����ַ��������
#else
	typedef std::basic_string<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > >	_StringBase;
#endif

#if SAPPHIRE_WCHAR_T_STRINGS
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy >> _StringStreamBase;   //�����ַ������Ļ���
#else
	typedef std::basic_stringstream<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > > _StringStreamBase;
#endif

	//ʹ���Զ�����ڴ���������ԭSTL����String���ټ���, ��StdStringT��T��  ����ʾstd::basic_string<T, std::char_traits<T>, std::allocator<T> >   T����Ϊchar  wchar_t
#define StdStringT(T) std::basic_string<T, std::char_traits<T>, std::allocator<T> >	
#define CustomMemoryStringT(T) std::basic_string<T, std::char_traits<T>, STLAllocator<T,GeneralAllocPolicy> >	      //ʹ���Զ���STL�ڴ��������STL String  �����Ӳ��Ժ�DEBUG��֧�֣�


	//����ģ�廯�Ĳ�����
	template<typename T>
	bool operator <(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //�Ƚ�����StdString���ַ�������
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<0;
	}
	template<typename T>
	bool operator <(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)       //�Ƚ�StdString ��CustomMemoryString����
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<0;
	}
	template<typename T>
	bool operator <=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)    
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) <= 0;
	}
	template<typename T>
	bool operator <=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) <= 0;
	}
	template<typename T>
	bool operator >(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>0;
	}
	template<typename T>
	bool operator >(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>0;
	}
	template<typename T>
	bool operator >=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) >= 0;
	}
	template<typename T>
	bool operator >=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) >= 0;
	}

	template<typename T>
	bool operator ==(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) == 0;
	}
	template<typename T>
	bool operator ==(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) == 0;
	}

	template<typename T>
	bool operator !=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) != 0;
	}
	template<typename T>
	bool operator !=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length()) != 0;
	}

	template<typename T>
	CustomMemoryStringT(T) operator +=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //�ַ���������
	{
		return CustomMemoryStringT(T)(l) += o.c_str();
	}
	template<typename T>
	CustomMemoryStringT(T) operator +=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)   //�ַ���������
	{
		return CustomMemoryStringT(T)(l.c_str()) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //�ַ���������
	{
		return CustomMemoryStringT(T)(l) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)     //�ַ���������
	{
		return CustomMemoryStringT(T)(l.c_str()) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const T* l, const CustomMemoryStringT(T)& o)        //�ַ���������
	{
		return CustomMemoryStringT(T)(l) += o;
	}


	// ȡ�����壬ֻ����StdStringT�� CustomerMemoryStringTֻ��������������������滻
#undef StdStringT
#undef CustomMemoryStringT

#else
#if SAPPHIRE_WCHAR_T_STRINGS             //�ж��Ƿ���wchar_t
	typedef std::wstring _StringBase;     //�����ַ�������
#else 
	typedef std::string _StringBase;
#endif

#if SAPPHIRE_WCHAR_T_STRINGS
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > _StringStreamBase;        //�����ַ���������
#else
	typedef std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > _StringStreamBase;
#endif

#endif

	typedef _StringBase String;
	typedef _StringStreamBase StringStream;
	typedef StringStream stringstream;

}





//������STL����������STL��׼���Allocator�滻ΪSTLAllocator<T, GeneralAllocPolicy>
namespace Sapphire
{
	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct deque
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::deque<T, A> type;
		typedef typename std::deque<T, A>::iterator iterator;
		typedef typename std::deque<T, A>::const_iterator const_iterator;
#else
		typedef typename std::deque<T> type;
		typedef typename std::deque<T>::iterator iterator;
		typedef typename std::deque<T>::const_iterator const_iterator;
#endif
	};

	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct vector
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::vector<T, A> type;
		typedef typename std::vector<T, A>::iterator iterator;
		typedef typename std::vector<T, A>::const_iterator const_iterator;
#else
		typedef typename std::vector<T> type;
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
#endif
	};

	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct list
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::list<T, A> type;
		typedef typename std::list<T, A>::iterator iterator;
		typedef typename std::list<T, A>::const_iterator const_iterator;
#else
		typedef typename std::list<T> type;
		typedef typename std::list<T>::iterator iterator;
		typedef typename std::list<T>::const_iterator const_iterator;
#endif
	};

	template <typename T, typename P = std::less<T>, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct set
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::set<T, P, A> type;
		typedef typename std::set<T, P, A>::iterator iterator;
		typedef typename std::set<T, P, A>::const_iterator const_iterator;
#else
		typedef typename std::set<T, P> type;
		typedef typename std::set<T, P>::iterator iterator;
		typedef typename std::set<T, P>::const_iterator const_iterator;
#endif
	};

	template <typename K, typename V, typename P = std::less<K>, typename A = STLAllocator<std::pair<const K, V>, GeneralAllocPolicy> >
	struct map
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::map<K, V, P, A> type;
		typedef typename std::map<K, V, P, A>::iterator iterator;
		typedef typename std::map<K, V, P, A>::const_iterator const_iterator;
#else
		typedef typename std::map<K, V, P> type;
		typedef typename std::map<K, V, P>::iterator iterator;
		typedef typename std::map<K, V, P>::const_iterator const_iterator;
#endif
	};

	template <typename K, typename V, typename P = std::less<K>, typename A = STLAllocator<std::pair<const K, V>, GeneralAllocPolicy> >
	struct multimap
	{
#if SAPPHIRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
		typedef typename std::multimap<K, V, P, A> type;
		typedef typename std::multimap<K, V, P, A>::iterator iterator;
		typedef typename std::multimap<K, V, P, A>::const_iterator const_iterator;
#else
		typedef typename std::multimap<K, V, P> type;
		typedef typename std::multimap<K, V, P>::iterator iterator;
		typedef typename std::multimap<K, V, P>::const_iterator const_iterator;
#endif
	};




}




#endif
