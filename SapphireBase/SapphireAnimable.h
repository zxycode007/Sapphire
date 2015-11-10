#ifndef _SAPPHIRE_ANIMABLE_
#define _SAPPHIRE_ANIMABLE_
#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"
#include "SapphireVector3.h"
#include "SapphireVector4.h"
#include "SapphireAny.h"
#include "SapphireColorValue.h"



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


	};
}



#endif 