#ifndef __SAPPHIRE_EXCEPTION_
#define __SAPPHIRE_EXCEPTION_

// Ԥ����ѡ��
#include "SapphirePrerequisites.h"
#include "SapphireHeaderPrefix.h"
#include "SapphireString.h"
#include <exception>

// �þ͹���ģʽ������������
#if SAPPHIRE_RELEASE_ASSERT == 1
#   define SAPPHIRE_ASSERT_MODE 1
#endif

// ���SAPPHIRE����ģʽ

// RELEASE_EXCEPTIONS ģʽ
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
	���׳�ʱ���ṩ����һ�������ڲ������Ĵ�����Ϣ
	@remarks
	SAPPHIRE �������÷���ֵȡָ������ȡ�����ǣ����һ�������������׳�һ���쳣��
	����ѹ���������ϸ�ڵ�һ�������С����Ӧ�ó�����SAPPHIRE�ܹ�ȷ���쳣�ܹ���׽������SAPPHIRE��Ӧ����try{}catch��Sapphire::Exception& e��{}��
	@par
	��Щ�û���Ӧ�ó�������ᴴ����������ʵ��������ϣ����������ͬһ���Ķ���
	*/
	class _SapphireExport Exception : public std::exception
	{
	protected:
		long line;      //�к�
		int number;       //�ַ���
		String typeName;     //������
		String description;     //����
		String source;         //Դ
		String file;           //�ļ�
		mutable String fullDesc;        //�ļ�����
	public:
		/** 
		�������ľ�̬����
		@todo
		��Ӹ�����쳣���룬��������Ҫ��׽���쳣
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

		/** Ĭ�Ϲ�����
		*/
		Exception(int number, const String& description, const String& source);

		/** 
		רҵ������
		*/
		Exception(int number, const String& description, const String& source, const char* type, const char* file, long line);

		/** 
		���ƹ�����
		*/
		Exception(const Exception& rhs);

		/// ��Ҫ�ñ�׼���쳣std��:exception
		~Exception() throw() {}

		/** 
		����������
		*/
		void operator = (const Exception& rhs);

		/** 
		����һ�������������������ַ���
		@remarks
		����������������У���Ϣ����, �������׳��쳣������֧�ֶ����ض�ƽ̨��Ϣ��
		����һ����Ⱦ����󣬴��������������������棺SAPPHIRE�ҵ������⣬��һ��������3d��Ⱦ��������ı��������Ч�Ļ�
		*/
		virtual const String& getFullDescription(void) const;

		/** 
		��ȡ�����к�
		*/
		virtual int getNumber(void) const throw();

		/**
		��ȡԴ����
		*/
		virtual const String &getSource() const { return source; }

		/** 
		��ȡԴ�ļ���
		*/
		virtual const String &getFile() const { return file; }

		/**
		��ȡ�к�
		*/
		virtual long getLine() const { return line; }

		/** 
		����һ��ֻ���� 'description'����쳣���ַ�������getFullDescriptiontoȡ���׳��쳣��һ�������кţ�����ú�ʲô�����������Ĵ�������
		*/
		virtual const String &getDescription(void) const { return description; }

		/// ����std::exception::what
		const char* what() const throw() { return getFullDescription().c_str(); }

	};


	/** ����һ��������ÿ���쳣���������ģ��ṹ
	@note
	��������ã��������������Ǵ���һ��Ϊ���ز�ͬ�쳣�������صķ��� 
	*/
	template <int num>
	struct ExceptionCodeType
	{
		enum { number = num };  //�쳣��
	};

	// ���쳣���뱻ʹ���ض��쳣����ÿ��Ҫ��׽���쳣�����¼���
	

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
	��ʵ��ת������Ϊ��ͨ������һ������һ���쳣������������͵�ֵ�쳣
	@remarks
	�����ȷ�Ŀ���ͨ��ֵ���������쳣��������������ȷ- ÿ�����붼ת��һ���ر�����������ڷ��������ء�
	������SAPPHIRE_EXCEPT������ʵ����С����ͬʱ��ȻӦ�������Ĵ��뵽���͵�ӳ��
	*/
	class ExceptionFactory  //�쳣����
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
//�׳��쳣
#define SAPPHIRE_EXCEPT(num, desc, src) throw Sapphire::ExceptionFactory::create( \
	Sapphire::ExceptionCodeType<num>(), desc, src, __FILE__, __LINE__ );
#endif
	/** @} */
	/** @} */

}  

#include "SapphireHeaderSuffix.h"
#endif
