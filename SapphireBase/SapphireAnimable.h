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
	定义一个动画对象属性， 可以是关键帧
	@remarks
	动画属性是一些可以通过一个预定义的关键帧序列随着时间改变
	它们可以直接被设置，或者由它们存在状态修改（通常如果希望同时应用多个动画）
	实现这个借口需要重载setValue，setCurrentStateAsBaseValue和applyDeltaValue 
	并且初始化这些类型
	@par
	AnimableValue 通过继承他的任意类的实例来访问
	AnimableObject 为了展示它动画属性
	@note
	这个类是适配器模式的实例，因为它是访问个别属性的普遍方式
	同时他可以被模板化使得类型可以在编译时被引用，着使得它比一般更难以聚合，并且当动画通常包含多个属性
	它通过一个单独类来处理所有的值
	*/
	class _SapphireExport AnimableValue : public AnimableAlloc
	{
	public:
		/// 可以作为动画值的类型
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
		/// 值的类型
		ValueType mType;

		/// 基础值数据
		union
		{
			int mBaseValueInt;
			Real mBaseValueReal[4];
		};

		/// 设置一个基础值的内部方法
		virtual void setAsBaseValue(int val) { mBaseValueInt = val; }
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(Real val) { mBaseValueReal[0] = val; }
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(const Vector2& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 2);
		}
		/// 设置一个基础值的内部方法
		virtual void setAsBaseValue(const Vector3& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 3);
		}
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(const Vector4& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 4);
		}
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(const Quaternion& val)
		{
			memcpy(mBaseValueReal, val.ptr(), sizeof(Real) * 4);
		}
		/// 设置一个基础值的内部方法
		virtual void setAsBaseValue(const Any& val);
		//设置一个基础值的内部方法
		virtual void setAsBaseValue(const ColourValue& val)
		{
			mBaseValueReal[0] = val.r;
			mBaseValueReal[1] = val.g;
			mBaseValueReal[2] = val.b;
			mBaseValueReal[3] = val.a;
		}
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(const Radian& val)
		{
			mBaseValueReal[0] = val.valueRadians();
		}
		///设置一个基础值的内部方法
		virtual void setAsBaseValue(const Degree& val)
		{
			mBaseValueReal[0] = val.valueRadians();
		}

	public:
		AnimableValue(ValueType t) : mType(t) {}
		virtual ~AnimableValue() {}

		/// 获取animable值的值类型
		ValueType getType(void) const { return mType; }

		/// 设置当前状态为一个基值，用于增量动画
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

		// 重设基值
		virtual void resetToBaseValue(void);

		/// 应用delta值
		virtual void applyDeltaValue(int) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		 
		virtual void applyDeltaValue(Real) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// 应用delta值
		virtual void applyDeltaValue(const Vector2&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// 应用delta值
		virtual void applyDeltaValue(const Vector3&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// 应用delta值
		virtual void applyDeltaValue(const Vector4&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// 应用delta值
		virtual void applyDeltaValue(const Quaternion&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		/// 应用delta值
		virtual void applyDeltaValue(const ColourValue&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// 应用delta值
		virtual void applyDeltaValue(const Degree&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// 应用delta值
		virtual void applyDeltaValue(const Radian&) {
			SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
		}
		// 应用delta值
		virtual void applyDeltaValue(const Any& val);

	};

	//AnimableValue的智能指针
	typedef SharedPtr<AnimableValue> AnimableValuePtr;

	/**
	定义一个要公开有一个或更多AnimableValue实例的接口类
	*/
	class _SapphireExport AnimableObject
	{
	protected:
		//Animable字典的Map类型
		typedef map<String, StringVector>::type AnimableDictionaryMap;
		/// 关于Animable值名字列表的类名静态Map
		static AnimableDictionaryMap msAnimableDictionary;
		/** 
		获取对于这个类在animable字典的名字
		@remarks
		子类如果他们需要他们的值指出动画的话，必须重写这个方法
		*/
		virtual const String& getAnimableDictionaryName(void) const
		{
			return StringUtil::BLANK;
		}
		/** 
		如果它已经不存在，创建一个对于这个类animable value名字的字典的内部方法。
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

		/// 获取一个到animable value列表的可更新的引用
		StringVector& _getAnimableValueNames(void)
		{
			AnimableDictionaryMap::iterator i =
				msAnimableDictionary.find(getAnimableDictionaryName());
			if (i != msAnimableDictionary.end())  //如果找到
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
		初始化字典的内部方法，应该由想要展现动画参数的子类实现
		*/
		virtual void initialiseAnimableDictionary(StringVector&) const {}


	public:
		AnimableObject() {}
		virtual ~AnimableObject() {}

		/** 
		获取这个对象animable value名的列表
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
		创建一个对于这个指定的值的AnimableValuePtr指针的引用计数器
		@remarks
		你能够使用这个返回对象进行动画处理此对象的值，用于动画追踪，如果想支持动画处理它们的值，子类必须重写此方法。	
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