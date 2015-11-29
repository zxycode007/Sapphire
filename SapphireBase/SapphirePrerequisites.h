/*

   这个头文件整合了sapphire需要预先配置的组件相关所有信息

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

// STL 容器
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

// STL 算法 & 函数
#include <algorithm>
#include <functional>
#include <limits>

// C++ 流
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

//这部分按C来编译，这部分作为全局变量使用
extern "C" {

#   include <sys/types.h>
#   include <sys/stat.h>

}

//unordered_map比hashmap更快
//#include <unordered_map>
#include "SapphireBuildSets.h"
#include "SapphireConfig.h"
#include "SapphirePlatfrom.h"

//根据编译器类型GCC,MSVC，CLANG来选择不同的类型HashMap和HashSet
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
//对类Unix系统的系统API调用接口函数
#    include <unistd.h>
#  endif
#endif

#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_LINUX || SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_ANDROID
extern "C" {

#   include <unistd.h>
	//  对于linux系统的动态链接库的显示调用
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



// 定义内存跟踪
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
	//在Sapphire空间，默认使用std标准库
	using namespace std;

	// 定义版本信息
#define SAPPHIRE_VERSION_MAJOR 0
#define SAPPHIRE_VERSION_MINOR 1
#define SAPPHIRE_VERSION_PATCH 1
#define SAPPHIRE_VERSION_SUFFIX ""
#define SAPPHIRE_VERSION_NAME "Sapphire"

#define SAPPHIRE_VERSION    ((SAPPHIRE_VERSION_MAJOR << 16) | (SAPPHIRE_VERSION_MINOR << 8) | SAPPHIRE_VERSION_PATCH)


	// 定义要用到的实型数据
	// 默认使用float知道预编译选项被修改
#if SAPPHIRE_DOUBLE_PRECISION == 1
	/** 浮点类型
	@note 不能作为GPU 缓冲区/参数的指针
	*/
	typedef double Real;
#else
	/** 浮点类型
	@note 不能作为GPU 缓冲区/参数的指针
	*/
	typedef float Real;
#endif




	//根据编译器类型设置hashMap和hashSet
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

	//静态堆对象分配
#define SAPPHIRE_DEFINE_STATIC_LOCAL(type, name, arguments) static type& name = *new type arguments
	//重定义数据类型
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
	typedef char c8;   //8位char
	typedef char s8;
	


#if SAPPHIRE_WCHAR_T_STRINGS == 0
	typedef char S_CHAR;
#else
	typedef wchar_t S_CHAR;
#endif

		//多线程定义支持
#include "threads\SapphireThreadDefines.h"

////////////////////////预定义类////////////////////////////////
	
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

//在所有配置设置号以后包含标准库文件
#include "SapphireStdHeader.h"
#include "SapphireKeyMap.h"
#include "SapphireDimension2D.h"
#include "SapphireDefines.h";
#include "SapphireSceneNodeTypes.h"
#include "SapphireMemoryAllocatorConfig.h"


namespace Sapphire
{

#if SAPPHIRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR        //是否使用自定义的内存分配器
#if SAPPHIRE_WCHAR_T_STRINGS                 //是否使用WCHAR
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy > >	_StringBase;   //定义字符串类基类
#else
	typedef std::basic_string<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > >	_StringBase;
#endif

#if SAPPHIRE_WCHAR_T_STRINGS
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy >> _StringStreamBase;   //定义字符串流的基类
#else
	typedef std::basic_stringstream<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > > _StringStreamBase;
#endif

	//使用自定义的内存分配器后和原STL：：String不再兼容, 用StdStringT（T）  来表示std::basic_string<T, std::char_traits<T>, std::allocator<T> >   T可以为char  wchar_t
#define StdStringT(T) std::basic_string<T, std::char_traits<T>, std::allocator<T> >	
#define CustomMemoryStringT(T) std::basic_string<T, std::char_traits<T>, STLAllocator<T,GeneralAllocPolicy> >	      //使用自定义STL内存分配器的STL String  （增加策略和DEBUG的支持）


	//重载模板化的操作符
	template<typename T>
	bool operator <(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //比较两个StdString的字符串长度
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<0;
	}
	template<typename T>
	bool operator <(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)       //比较StdString 与CustomMemoryString长度
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
	CustomMemoryStringT(T) operator +=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //字符串的连接
	{
		return CustomMemoryStringT(T)(l) += o.c_str();
	}
	template<typename T>
	CustomMemoryStringT(T) operator +=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)   //字符串的连接
	{
		return CustomMemoryStringT(T)(l.c_str()) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)   //字符串的连接
	{
		return CustomMemoryStringT(T)(l) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)     //字符串的连接
	{
		return CustomMemoryStringT(T)(l.c_str()) += o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const T* l, const CustomMemoryStringT(T)& o)        //字符串的连接
	{
		return CustomMemoryStringT(T)(l) += o;
	}


	// 取消定义，只用于StdStringT与 CustomerMemoryStringT只用于上面运算符的重置替换
#undef StdStringT
#undef CustomMemoryStringT

#else
#if SAPPHIRE_WCHAR_T_STRINGS             //判断是否是wchar_t
	typedef std::wstring _StringBase;     //定义字符串基类
#else 
	typedef std::string _StringBase;
#endif

#if SAPPHIRE_WCHAR_T_STRINGS
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > _StringStreamBase;        //定义字符串流基类
#else
	typedef std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > _StringStreamBase;
#endif

#endif

	typedef _StringBase String;
	typedef _StringStreamBase StringStream;
	typedef StringStream stringstream;

}





//处理后的STL的容器，将STL标准库的Allocator替换为STLAllocator<T, GeneralAllocPolicy>
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
