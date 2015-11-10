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


	};
}



#endif 