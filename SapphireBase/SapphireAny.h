#ifndef _SAPPHIRE_ANY_
#define _SAPPHIRE_ANY_
#include "SapphirePrerequisites.h"
#include "SapphireException.h"
#include "SapphireString.h"
#include <algorithm>
#include <typeinfo>


namespace Sapphire
{
	/** �ܹ����������������͵Ĳ�ͬ����
	*/
	class Any
	{
	public:  

		Any()
			: mContent(0)
		{
		}
		//ģ�庯��
		template<typename ValueType>   //����ValueType��һ������
		explicit Any(const ValueType & value)
			: mContent(SAPPHIRE_NEW_T(holder<ValueType>, MEMCATEGORY_GENERAL)(value))   //mContent ��holder���ͣ������������
		{
		}

		Any(const Any & other)
			: mContent(other.mContent ? other.mContent->clone() : 0)  //�������캯��
		{
		}

		virtual ~Any()
		{
			destroy();
		}

	public: 

		//���н���
		Any& swap(Any & rhs)
		{
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType>
		Any& operator=(const ValueType & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		Any & operator=(const Any & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

	public:  

		bool isEmpty() const
		{
			return !mContent;
		}

		const std::type_info& getType() const
		{
			return mContent ? mContent->getType() : typeid(void);
		}

		inline friend std::ostream& operator <<
			(std::ostream& o, const Any& v)
		{
			if (v.mContent)
				v.mContent->writeToStream(o);
			return o;
		}

		void destroy()
		{
			SAPPHIRE_DELETE_T(mContent, placeholder, MEMCATEGORY_GENERAL);
			mContent = NULL;
		}

	protected: // ����

		class placeholder
		{
		public: // ռλ��

			virtual ~placeholder()
			{
			}

		public: // ��ѯ����

			virtual const std::type_info& getType() const = 0;

			virtual placeholder * clone() const = 0;

			virtual void writeToStream(std::ostream& o) = 0;

		};

		template<typename ValueType>
		class holder : public placeholder
		{
		public:  

			holder(const ValueType & value)
				: held(value)
			{
			}

		public: // ��ѯ����

			virtual const std::type_info & getType() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return SAPPHIRE_NEW_T(holder, MEMCATEGORY_GENERAL)(held);
			}

			virtual void writeToStream(std::ostream& o)
			{
				o << held;
			}


		public: // ֵ���ͱ�ʾ

			ValueType held;

		};



	protected: //���ݱ�ʾ
		placeholder * mContent;

		//����ֵת������
		template<typename ValueType>
		friend ValueType * any_cast(Any *);


	public:
		//�������е�����ת��ΪAny������ Any a(12.5)
		template<typename ValueType>
		ValueType operator()() const   //��������ת��
		{
			if (!mContent)
			{
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					"Bad cast from uninitialised Any",
					"Any::operator()");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(mContent)->held;
			}
			else
			{
				StringUtil::StrStreamType str;
				str << "Bad cast from type '" << getType().name() << "' "
					<< "to '" << typeid(ValueType).name() << "'";
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					str.str(),
					"Any::operator()");
			}
		}

		//ģ�庯��������ָ���������͵�ֵ
		template <typename ValueType>
		ValueType get(void) const
		{
			if (!mContent)
			{
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					"Bad cast from uninitialised Any",
					"Any::operator()");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(mContent)->held;
			}
			else
			{
				StringUtil::StrStreamType str;
				str << "Bad cast from type '" << getType().name() << "' "
					<< "to '" << typeid(ValueType).name() << "'";
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					str.str(),
					"Any::operator()");
			}
		}

	};


	/** 
	ָ��һ���ܹ�������ѧ�����������࣬����ֻ��֧�ּӼ��˳��������� 
	*/
	class AnyNumeric : public Any
	{
	public:
		AnyNumeric()
			: Any()
		{
		}

		template<typename ValueType>
		AnyNumeric(const ValueType & value)

		{
			mContent = SAPPHIRE_NEW_T(numholder<ValueType>, MEMCATEGORY_GENERAL)(value);
		}

		AnyNumeric(const AnyNumeric & other)
			: Any()
		{
			mContent = other.mContent ? other.mContent->clone() : 0;
		}

	protected:
		class numplaceholder : public Any::placeholder
		{
		public: 

			~numplaceholder()
			{
			}
			virtual placeholder* add(placeholder* rhs) = 0;
			virtual placeholder* subtract(placeholder* rhs) = 0;
			virtual placeholder* multiply(placeholder* rhs) = 0;
			virtual placeholder* multiply(Real factor) = 0;
			virtual placeholder* divide(placeholder* rhs) = 0;
		};

		template<typename ValueType>
		class numholder : public numplaceholder
		{
		public:  

			numholder(const ValueType & value)
				: held(value)
			{
			}

		public:  

			virtual const std::type_info & getType() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held);
			}

			virtual placeholder* add(placeholder* rhs)
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held + static_cast<numholder*>(rhs)->held);
			}
			virtual placeholder* subtract(placeholder* rhs)
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held - static_cast<numholder*>(rhs)->held);
			}
			virtual placeholder* multiply(placeholder* rhs)
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held * static_cast<numholder*>(rhs)->held);
			}
			virtual placeholder* multiply(Real factor)
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held * factor);
			}
			virtual placeholder* divide(placeholder* rhs)
			{
				return SAPPHIRE_NEW_T(numholder, MEMCATEGORY_GENERAL)(held / static_cast<numholder*>(rhs)->held);
			}
			virtual void writeToStream(std::ostream& o)
			{
				o << held;
			}

		public:  

			ValueType held;

		};

		
		AnyNumeric(placeholder* pholder)
		{
			mContent = pholder;
		}

	public:
		AnyNumeric & operator=(const AnyNumeric & rhs)
		{
			AnyNumeric(rhs).swap(*this);
			return *this;
		}
		AnyNumeric operator+(const AnyNumeric& rhs) const
		{
			return AnyNumeric(
				static_cast<numplaceholder*>(mContent)->add(rhs.mContent));
		}
		AnyNumeric operator-(const AnyNumeric& rhs) const
		{
			return AnyNumeric(
				static_cast<numplaceholder*>(mContent)->subtract(rhs.mContent));
		}
		AnyNumeric operator*(const AnyNumeric& rhs) const
		{
			return AnyNumeric(
				static_cast<numplaceholder*>(mContent)->multiply(rhs.mContent));
		}
		AnyNumeric operator*(Real factor) const
		{
			return AnyNumeric(
				static_cast<numplaceholder*>(mContent)->multiply(factor));
		}
		AnyNumeric operator/(const AnyNumeric& rhs) const
		{
			return AnyNumeric(
				static_cast<numplaceholder*>(mContent)->divide(rhs.mContent));
		}
		AnyNumeric& operator+=(const AnyNumeric& rhs)
		{
			*this = AnyNumeric(
				static_cast<numplaceholder*>(mContent)->add(rhs.mContent));
			return *this;
		}
		AnyNumeric& operator-=(const AnyNumeric& rhs)
		{
			*this = AnyNumeric(
				static_cast<numplaceholder*>(mContent)->subtract(rhs.mContent));
			return *this;
		}
		AnyNumeric& operator*=(const AnyNumeric& rhs)
		{
			*this = AnyNumeric(
				static_cast<numplaceholder*>(mContent)->multiply(rhs.mContent));
			return *this;
		}
		AnyNumeric& operator/=(const AnyNumeric& rhs)
		{
			*this = AnyNumeric(
				static_cast<numplaceholder*>(mContent)->divide(rhs.mContent));
			return *this;
		}




	};


	template<typename ValueType>
	ValueType * any_cast(Any * operand)
	{
		return operand && (std::strcmp(operand->getType().name(), typeid(ValueType).name()) == 0)
			? &static_cast<Any::holder<ValueType> *>(operand->mContent)->held
			: 0;
	}

	template<typename ValueType>
	const ValueType * any_cast(const Any * operand)
	{
		return any_cast<ValueType>(const_cast<Any *>(operand));
	}

	template<typename ValueType>
	ValueType any_cast(const Any & operand)
	{
		const ValueType * result = any_cast<ValueType>(&operand);
		if (!result)
		{
			StringUtil::StrStreamType str;
			str << "Bad cast from type '" << operand.getType().name() << "' "
				<< "to '" << typeid(ValueType).name() << "'";
			SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				str.str(),
				"Sapphire::any_cast");
		}
		return *result;
	}
	/** @} */
	/** @} */
}

#endif