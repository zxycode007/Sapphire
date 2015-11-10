#ifndef __SAPPHIRE_EXCEPTION_
#define __SAPPHIRE_EXCEPTION_

// 预编译选项
#include "SapphirePrerequisites.h"
#include "SapphireHeaderPrefix.h"
#include "SapphireString.h"
#include <exception>

// 用就关联模式定义向后兼容性
#if SAPPHIRE_RELEASE_ASSERT == 1
#   define SAPPHIRE_ASSERT_MODE 1
#endif

// 检测SAPPHIRE断言模式

// RELEASE_EXCEPTIONS 模式
#if SAPPHIRE_ASSERT_MODE == 1
#   ifdef _DEBUG
#       define SapphireAssert( a, b ) assert( (a) && (b) )

#   else
#       if SAPPHIRE_COMP != SAPPHIRE_COMPILER_BORL
#           define SapphireAssert( a, b ) if( !(a) ) SAPPHIRE_EXCEPT( Sapphire::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#       else
#           define SapphireAssert( a, b ) if( !(a) ) SAPPHIRE_EXCEPT( Sapphire::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__ )
#       endif

#   endif

// EXCEPTIONS mode
#elif SAPPHIRE_ASSERT_MODE == 2
#   if SAPPHIRE_COMP != SAPPHIRE_COMPILER_BORL
#       define SapphireAssert( a, b ) if( !(a) ) SAPPHIRE_EXCEPT( Sapphire::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#   else
#       define SapphireAssert( a, b ) if( !(a) ) SAPPHIRE_EXCEPT( Sapphire::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__ )
#   endif

// STANDARD mode
#else
#   define SapphireAssert( a, b ) assert( (a) && (b) )

#endif

namespace Sapphire {
	
	/** 
	当抛出时，提供关于一个引擎内部发生的错误信息
	@remarks
	SAPPHIRE 绝不会用返回值取指顶错误。取代的是，如果一个错误发生，会抛出一个异常，
	并且压缩其问题的细节到一个对象中。这个应用程序用SAPPHIRE能够确定异常能够捕捉。所以SAPPHIRE，应该用try{}catch（Sapphire::Exception& e）{}块
	@par
	这些用户的应用程序绝不会创建这个对象的实例，除非希望错误句柄用同一个的对象
	*/
	class _SapphireExport Exception : public std::exception
	{
	protected:
		long line;      //行号
		int number;       //字符数
		String typeName;     //类型名
		String description;     //描述
		String source;         //源
		String file;           //文件
		mutable String fullDesc;        //文件描述
	public:
		/** 
		错误代码的静态定义
		@todo
		添加更多的异常代码，自我们想要捕捉的异常
		*/
		enum ExceptionCodes {
			ERR_CANNOT_WRITE_TO_FILE,
			ERR_INVALID_STATE,
			ERR_INVALIDPARAMS,
			ERR_RENDERINGAPI_ERROR,
			ERR_DUPLICATE_ITEM,
			ERR_ITEM_NOT_FOUND,
			ERR_FILE_NOT_FOUND,
			ERR_INTERNAL_ERROR,
			ERR_RT_ASSERTION_FAILED,
			ERR_NOT_IMPLEMENTED
		};

		/** 默认构造器
		*/
		Exception(int number, const String& description, const String& source);

		/** 
		专业构造器
		*/
		Exception(int number, const String& description, const String& source, const char* type, const char* file, long line);

		/** 
		复制构造器
		*/
		Exception(const Exception& rhs);

		/// 需要用标准库异常std：:exception
		~Exception() throw() {}

		/** 
		关联操作符
		*/
		void operator = (const Exception& rhs);

		/** 
		返回一个带错误完整描述的字符串
		@remarks
		这个描述包括错误行，信息描述, 按惯例抛出异常。并且支持额外特定平台信息。
		例如一个渲染库错误，错误描述将包括两个方面：SAPPHIRE找到的问题，和一个来自于3d渲染库的描述文本，如果有效的话
		*/
		virtual const String& getFullDescription(void) const;

		/** 
		获取错误行号
		*/
		virtual int getNumber(void) const throw();

		/**
		获取源函数
		*/
		virtual const String &getSource() const { return source; }

		/** 
		获取源文件名
		*/
		virtual const String &getFile() const { return file; }

		/**
		获取行号
		*/
		virtual long getLine() const { return line; }

		/** 
		返回一个只带有 'description'域的异常的字符串。用getFullDescriptionto取得抛出异常的一个包含行号，错误好和什么函数的完整的错误描述
		*/
		virtual const String &getDescription(void) const { return description; }

		/// 覆盖std::exception::what
		const char* what() const throw() { return getFullDescription().c_str(); }

	};


	/** 创建一个区别于每个异常代码的类型模板结构
	@note
	这个很有用，有用它允许我们创建一个为返回不同异常类型重载的方法 
	*/
	template <int num>
	struct ExceptionCodeType
	{
		enum { number = num };  //异常码
	};

	// 自异常代码被使用特定异常允许每个要扑捉的异常都向下兼容
	

	class _SapphireExport UnimplementedException : public Exception
	{
	public:
		UnimplementedException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "UnimplementedException", inFile, inLine) {}
	};
	class _SapphireExport FileNotFoundException : public Exception
	{
	public:
		FileNotFoundException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "FileNotFoundException", inFile, inLine) {}
	};
	class _SapphireExport IOException : public Exception
	{
	public:
		IOException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "IOException", inFile, inLine) {}
	};
	class _SapphireExport InvalidStateException : public Exception
	{
	public:
		InvalidStateException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "InvalidStateException", inFile, inLine) {}
	};
	class _SapphireExport InvalidParametersException : public Exception
	{
	public:
		InvalidParametersException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "InvalidParametersException", inFile, inLine) {}
	};
	class _SapphireExport ItemIdentityException : public Exception
	{
	public:
		ItemIdentityException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "ItemIdentityException", inFile, inLine) {}
	};
	class _SapphireExport InternalErrorException : public Exception
	{
	public:
		InternalErrorException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "InternalErrorException", inFile, inLine) {}
	};
	class _SapphireExport RenderingAPIException : public Exception
	{
	public:
		RenderingAPIException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "RenderingAPIException", inFile, inLine) {}
	};
	class _SapphireExport RuntimeAssertionException : public Exception
	{
	public:
		RuntimeAssertionException(int inNumber, const String& inDescription, const String& inSource, const char* inFile, long inLine)
			: Exception(inNumber, inDescription, inSource, "RuntimeAssertionException", inFile, inLine) {}
	};


	/** 
	类实现转发方法为了通过构造一个基于一个异常代码的派生类型的值异常
	@remarks
	这个精确的控制通过值构造派生异常，不会遇到不明确- 每个代码都转到一个特别的类型以至于方法被重载。
	这允许SAPPHIRE_EXCEPT保存在实现中小留，同时仍然应用重量的代码到类型的映射
	*/
	class ExceptionFactory  //异常工厂
	{
	private:
		
		ExceptionFactory() {}
	public:
		static UnimplementedException create(
			ExceptionCodeType<Exception::ERR_NOT_IMPLEMENTED> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return UnimplementedException(code.number, desc, src, file, line);
		}
		static FileNotFoundException create(
			ExceptionCodeType<Exception::ERR_FILE_NOT_FOUND> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return FileNotFoundException(code.number, desc, src, file, line);
		}
		static IOException create(
			ExceptionCodeType<Exception::ERR_CANNOT_WRITE_TO_FILE> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return IOException(code.number, desc, src, file, line);
		}
		static InvalidStateException create(
			ExceptionCodeType<Exception::ERR_INVALID_STATE> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return InvalidStateException(code.number, desc, src, file, line);
		}
		static InvalidParametersException create(
			ExceptionCodeType<Exception::ERR_INVALIDPARAMS> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return InvalidParametersException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_ITEM_NOT_FOUND> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_DUPLICATE_ITEM> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static InternalErrorException create(
			ExceptionCodeType<Exception::ERR_INTERNAL_ERROR> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return InternalErrorException(code.number, desc, src, file, line);
		}
		static RenderingAPIException create(
			ExceptionCodeType<Exception::ERR_RENDERINGAPI_ERROR> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return RenderingAPIException(code.number, desc, src, file, line);
		}
		static RuntimeAssertionException create(
			ExceptionCodeType<Exception::ERR_RT_ASSERTION_FAILED> code,
			const String& desc,
			const String& src, const char* file, long line)
		{
			return RuntimeAssertionException(code.number, desc, src, file, line);
		}

	};



#ifndef SAPPHIRE_EXCEPT 
//抛出异常
#define SAPPHIRE_EXCEPT(num, desc, src) throw Sapphire::ExceptionFactory::create( \
	Sapphire::ExceptionCodeType<num>(), desc, src, __FILE__, __LINE__ );
#endif
	/** @} */
	/** @} */

}  

#include "SapphireHeaderSuffix.h"
#endif
