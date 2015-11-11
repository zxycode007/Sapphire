#ifndef _SAPPHIRE_ANIMABLE_
#define _SAPPHIRE_ANIMABLE_
#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"
#include "SapphireVector3.h"
#include "SapphireVector4.h"
#include "SapphireAny.h"
#include "SapphireColorValue.h"
#include "SapphireSharedPtr.h"
#include "SapphireStringVector.h"



namespace Sapphire
{
	/** 
	����һ�������������ԣ� �����ǹؼ�֡
	@remarks
	����������һЩ����ͨ��һ��Ԥ����Ĺؼ�֡��������ʱ��ı�
	���ǿ���ֱ�ӱ����ã����������Ǵ���״̬�޸ģ�ͨ�����ϣ��ͬʱӦ�ö��������
	ʵ����������Ҫ����setValue��setCurrentStateAsBaseValue��applyDeltaValue 
	���ҳ�ʼ����Щ����
	@par
	AnimableValue ͨ���̳������������ʵ��������
	AnimableObject Ϊ��չʾ����������
	@note
	�������������ģʽ��ʵ������Ϊ���Ƿ��ʸ������Ե��ձ鷽ʽ
	ͬʱ�����Ա�ģ�廯ʹ�����Ϳ����ڱ���ʱ�����ã���ʹ������һ������Ծۺϣ����ҵ�����ͨ�������������
	��ͨ��һ�����������������е�ֵ
	*/
	class _SapphireExport AnimableValue : public AnimableAlloc
	{
	public:
		/// ������Ϊ����ֵ������
		enum ValueType
		{
			INT,
			REAL,
			VECTOR2,
			VECTOR3,
			VECTOR4,
			QUATERNION,
			COLOUR,
			RADIAN,
			DEGREE
		};
	protected:
		/// ֵ������
		ValueType mType;

		/// ����ֵ����
		union
		{
			int mBaseValueInt;
			Real mBaseValueReal[4];
		};

		/// ����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(int val) { mBaseValueInt = val; }
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(Real val) { mBaseValueReal[0] = val; }
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Vector2& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 2);
		}
		/// ����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Vector3& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 3);
		}
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Vector4& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 4);
		}
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Quaternion& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 4);
		}
		/// ����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Any& val);
		//����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const ColourValue& val)
		{
			mBaseValueReal[0] = val.r;
			mBaseValueReal[1] = val.g;
			mBaseValueReal[2] = val.b;
			mBaseValueReal[3] = val.a;
		}
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Radian& val)
		{
			mBaseValueReal[0] = val.valueRadians();
		}
		///����һ������ֵ���ڲ�����
		virtual void setAsBaseValue(const Degree& val)
		{
			mBaseValueReal[0] = val.valueRadians();
		}

	public:
		AnimableValue(ValueType t) : mType(t) {}
		virtual ~AnimableValue() {}

		/// ��ȡanimableֵ��ֵ����
		ValueType getType(void) const { return mType; }

		/// ���õ�ǰ״̬Ϊһ����ֵ��������������
		virtual void setCurrentStateAsBaseValue(void) = 0;

		 
		virtual void setValue(int) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(Real) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const Vector2&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const Vector3&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
	 
		virtual void setValue(const Vector4&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const Quaternion&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const ColourValue&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
	 
		virtual void setValue(const Radian&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const Degree&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void setValue(const Any& val);

		// �����ֵ
		virtual void resetToBaseValue(void);

		/// Ӧ��deltaֵ
		virtual void applyDeltaValue(int) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void applyDeltaValue(Real) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Vector2&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Vector3&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Vector4&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Quaternion&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// Ӧ��deltaֵ
		virtual void applyDeltaValue(const ColourValue&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Degree&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Radian&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// Ӧ��deltaֵ
		virtual void applyDeltaValue(const Any& val);

	};

	//AnimableValue������ָ��
	typedef SharedPtr<AnimableValue> AnimableValuePtr;

	/**
	����һ��Ҫ������һ�������AnimableValueʵ���Ľӿ���
	*/
	class _SapphireExport AnimableObject
	{
	protected:
		//Animable�ֵ��Map����
		typedef map<String, StringVector>::type AnimableDictionaryMap;
		/// ����Animableֵ�����б��������̬Map
		static AnimableDictionaryMap msAnimableDictionary;
		/** 
		��ȡ�����������animable�ֵ������
		@remarks
		�������������Ҫ���ǵ�ֵָ�������Ļ���������д�������
		*/
		virtual const String& getAnimableDictionaryName(void) const
		{
			return StringUtil::BLANK;
		}
		/** 
		������Ѿ������ڣ�����һ�����������animable value���ֵ��ֵ���ڲ�������
		*/
		void createAnimableDictionary(void) const
		{
			if (msAnimableDictionary.find(getAnimableDictionaryName())
				== msAnimableDictionary.end())
			{
				StringVector vec;
				initialiseAnimableDictionary(vec);
				msAnimableDictionary[getAnimableDictionaryName()] = vec;
			}

		}

		/// ��ȡһ����animable value�б�Ŀɸ��µ�����
		StringVector& _getAnimableValueNames(void)
		{
			AnimableDictionaryMap::iterator i =
				msAnimableDictionary.find(getAnimableDictionaryName());
			if (i != msAnimableDictionary.end())  //����ҵ�
			{
				return i->second;
			}
			else
			{
				SAPPHIRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
					"Animable value list not found for " + getAnimableDictionaryName(),
					"AnimableObject::getAnimableValueNames");
			}

		}

		/** 
		��ʼ���ֵ���ڲ�������Ӧ������Ҫչ�ֶ�������������ʵ��
		*/
		virtual void initialiseAnimableDictionary(StringVector&) const {}


	public:
		AnimableObject() {}
		virtual ~AnimableObject() {}

		/** 
		��ȡ�������animable value�����б�
		*/
		const StringVector& getAnimableValueNames(void) const
		{
			createAnimableDictionary();

			AnimableDictionaryMap::iterator i =
				msAnimableDictionary.find(getAnimableDictionaryName());
			if (i != msAnimableDictionary.end())
			{
				return i->second;
			}
			else
			{
				SAPPHIRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
					"Animable value list not found for " + getAnimableDictionaryName(),
					"AnimableObject::getAnimableValueNames");
			}

		}

		/** 
		����һ���������ָ����ֵ��AnimableValuePtrָ������ü�����
		@remarks
		���ܹ�ʹ��������ض�����ж�������˶����ֵ�����ڶ���׷�٣������֧�ֶ����������ǵ�ֵ�����������д�˷�����	
		*/
		virtual AnimableValuePtr createAnimableValue(const String& valueName)
		{
			SAPPHIRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"No animable value named '" + valueName + "' present.",
				"AnimableObject::createAnimableValue");
		}



	};

}



#endif 