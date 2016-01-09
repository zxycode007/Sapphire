#include "SapphirePrerequisites.h"
#include "SapphireString.h"
#include "SapphireCAtrributes.h"
#include "SapphireString.h"
#include "SapphireITexture.h"
#include "SapphireIVideoDriver.h"



namespace Sapphire
{
	/*
	基础类型
	*/

	// 对于Bool型IAttribute 的实现
	class CBoolAttribute : public IAttribute
	{
	public:

		CBoolAttribute(const char* name, bool value)
		{
			Name = name;
			setBool(value);
		}

		virtual SINT32 getInt()
		{
			return BoolValue ? 1 : 0;
		}

		virtual FLOAT32 getFloat()
		{
			return BoolValue ? 1.0f : 0.0f;
		}

		virtual bool getBool()
		{
			return BoolValue;
		}

		virtual StringW getStringW()
		{
			return StringW(BoolValue ? L"true" : L"false");
		}

		virtual void setInt(SINT32 intValue)
		{
			BoolValue = (intValue != 0);
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			BoolValue = (floatValue != 0);
		}

		virtual void setBool(bool boolValue)
		{
			BoolValue = boolValue;
		}

		virtual void setString(const char* string)
		{
			BoolValue = strcmp(string, "true") == 0;
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_BOOL;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"bool";
		}

		bool BoolValue;
	};

	// 对于Integer型IAttribute 的实现
	class CIntAttribute : public IAttribute
	{
	public:

		CIntAttribute(const char* name, SINT32 value)
		{
			Name = name;
			setInt(value);
		}

		virtual SINT32 getInt()
		{
			return Value;
		}

		virtual FLOAT32 getFloat()
		{
			return (FLOAT32)Value;
		}

		virtual bool getBool()
		{
			return (Value != 0);
		}

		virtual StringW getStringW()
		{
			StringWStream ws;
			ws << Value;
			return ws.str();
		}

		virtual void setInt(SINT32 intValue)
		{
			Value = intValue;
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			Value = (SINT32)floatValue;
		};

		virtual void setString(const char* text)
		{
			Value = atoi(text);
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_INT;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"int";
		}

		SINT32 Value;
	};

	//  对于floats型IAttribute 的实现 
	class CFloatAttribute : public IAttribute
	{
	public:

		CFloatAttribute(const char* name, FLOAT32 value)
		{
			Name = name;
			setFloat(value);
		}

		virtual SINT32 getInt()
		{
			return (SINT32)Value;
		}

		virtual FLOAT32 getFloat()
		{
			return Value;
		}

		virtual bool getBool()
		{
			return (Value != 0);
		}

		virtual StringW getStringW()
		{
			StringWStream ws;
			ws << Value;
			return ws.str();
		}

		virtual void setInt(SINT32 intValue)
		{
			Value = (FLOAT32)intValue;
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			Value = floatValue;
		}

		virtual void setString(const char* text)
		{
			Value = fast_atof(text);
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_FLOAT;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"float";
		}

		FLOAT32 Value;
	};



	/*
	可以被表述为一个数字列表的属性类型
	如向量，颜色，位置，三角形等...
	*/

	// 对与所有一个是数字列表的属性的基础类
	class CNumbersAttribute : public IAttribute
	{
	public:

		CNumbersAttribute(const char* name, ColourValue value) :
			ValueI(), ValueF(), Count(4), IsFloat(true)
		{
			Name = name;

			ValueF.push_back(value.r);
			ValueF.push_back(value.g);
			ValueF.push_back(value.b);
			ValueF.push_back(value.a);

		}


		CNumbersAttribute(const char* name, Vector3 value) :
			ValueI(), ValueF(), Count(3), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.x);
			ValueF.push_back(value.y);
			ValueF.push_back(value.z);
		}

		CNumbersAttribute(const char* name, rect<SINT32> value) :
			ValueI(), ValueF(), Count(4), IsFloat(false)
		{
			Name = name;
			ValueI.push_back(value.UpperLeftCorner.x);
			ValueI.push_back(value.UpperLeftCorner.y);
			ValueI.push_back(value.LowerRightCorner.x);
			ValueI.push_back(value.LowerRightCorner.y);
		}

		CNumbersAttribute(const char* name, rect<FLOAT32> value) :
			ValueI(), ValueF(), Count(4), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.UpperLeftCorner.x);
			ValueF.push_back(value.UpperLeftCorner.y);
			ValueF.push_back(value.LowerRightCorner.x);
			ValueF.push_back(value.LowerRightCorner.y);
		}

		CNumbersAttribute(const char* name, Matrix4 value) :
			ValueI(), ValueF(), Count(16), IsFloat(true)
		{
			Name = name;
			for (SINT32 r = 0; r < 4; ++r)
				for (SINT32 c = 0; c < 4; ++c)
					ValueF.push_back(value[r][c]);
		}

		CNumbersAttribute(const char* name, Quaternion value) :
			ValueI(), ValueF(), Count(4), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.x);
			ValueF.push_back(value.y);
			ValueF.push_back(value.z);
			ValueF.push_back(value.w);
		}

		CNumbersAttribute(const char* name, AxisAlignedBox value) :
			ValueI(), ValueF(), Count(6), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.getMinimum().x);
			ValueF.push_back(value.getMinimum().y);
			ValueF.push_back(value.getMinimum().z);
			ValueF.push_back(value.getMaximum().x);
			ValueF.push_back(value.getMaximum().y);
			ValueF.push_back(value.getMaximum().z);
		}

		CNumbersAttribute(const char* name, Plane value) :
			ValueI(), ValueF(), Count(4), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.normal.x);
			ValueF.push_back(value.normal.y);
			ValueF.push_back(value.normal.z);
			ValueF.push_back(value.d);
		}

		CNumbersAttribute(const char* name, triangle3df value) :
			ValueI(), ValueF(), Count(9), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.pointA.x);
			ValueF.push_back(value.pointA.y);
			ValueF.push_back(value.pointA.z);
			ValueF.push_back(value.pointB.x);
			ValueF.push_back(value.pointB.y);
			ValueF.push_back(value.pointB.z);
			ValueF.push_back(value.pointC.x);
			ValueF.push_back(value.pointC.y);
			ValueF.push_back(value.pointC.z);
		}

		CNumbersAttribute(const char* name, Vector2 value) :
			ValueI(), ValueF(), Count(2), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.x);
			ValueF.push_back(value.y);
		}
		/*
		CNumbersAttribute(const char* name, Vector2 value) :
		ValueI(), ValueF(), Count(2), IsFloat(false)
		{
		Name = name;
		ValueI.push_back(value.x);
		ValueI.push_back(value.y);
		}
		*/
		CNumbersAttribute(const char* name, line2di value) :
			ValueI(), ValueF(), Count(4), IsFloat(false)
		{
			Name = name;
			ValueI.push_back(value.start.x);
			ValueI.push_back(value.start.y);
			ValueI.push_back(value.end.x);
			ValueI.push_back(value.end.y);
		}

		CNumbersAttribute(const char* name, line2df value) :
			ValueI(), ValueF(), Count(4), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.start.x);
			ValueF.push_back(value.start.y);
			ValueF.push_back(value.end.x);
			ValueF.push_back(value.end.y);
		}

		CNumbersAttribute(const char* name, line3df value) :
			ValueI(), ValueF(), Count(6), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.start.x);
			ValueF.push_back(value.start.y);
			ValueF.push_back(value.start.z);
			ValueF.push_back(value.end.x);
			ValueF.push_back(value.end.y);
			ValueF.push_back(value.end.z);
		}

		CNumbersAttribute(const char* name, dimension2du value) :
			ValueI(), ValueF(), Count(2), IsFloat(false)
		{
			Name = name;
			ValueI.push_back(value.Width);
			ValueI.push_back(value.Height);
		}


		CNumbersAttribute(const char* name, dimension2df value) :
			ValueI(), ValueF(), Count(2), IsFloat(true)
		{
			Name = name;
			ValueF.push_back(value.Width);
			ValueF.push_back(value.Height);
		}



		// getting values
		virtual SINT32 getInt()
		{
			if (Count == 0)
				return 0;

			if (IsFloat)
				return (SINT32)ValueF[0];
			else
				return ValueI[0];
		}

		virtual FLOAT32 getFloat()
		{
			if (Count == 0)
				return 0.0f;

			if (IsFloat)
				return ValueF[0];
			else
				return (FLOAT32)ValueI[0];
		}

		virtual bool getBool()
		{
			// return true if any number is nonzero
			bool ret = false;

			for (UINT32 i = 0; i < Count; ++i)
				if (IsFloat ? (ValueF[i] != 0) : (ValueI[i] != 0))
				{
					ret = true;
					break;
				}

			return ret;

		}


		virtual String getString()
		{
			String outstr;

			for (UINT32 i = 0; i < Count; ++i)
			{
				if (IsFloat)
					outstr += ValueF[i];
				else
					outstr += ValueI[i];

				if (i < Count - 1)
					outstr += ", ";
			}
			return outstr;
		}
		virtual StringW getStringW()
		{
			StringW outstr;

			for (UINT32 i = 0; i < Count; ++i)
			{
				if (IsFloat)
					outstr += ValueF[i];
				else
					outstr += ValueI[i];

				if (i < Count - 1)
					outstr += L", ";
			}
			return outstr;
		}

		virtual Position2d getPosition()
		{
			Position2d p;

			if (IsFloat)
			{
				p.x = (SINT32)(Count > 0 ? ValueF[0] : 0);
				p.y = (SINT32)(Count > 1 ? ValueF[1] : 0);
			}
			else
			{
				p.x = Count > 0 ? ValueI[0] : 0;
				p.y = Count > 1 ? ValueI[1] : 0;
			}

			return p;
		}

		virtual Vector3 getVector()
		{
			Vector3 v;

			if (IsFloat)
			{
				v.x = Count > 0 ? ValueF[0] : 0;
				v.y = Count > 1 ? ValueF[1] : 0;
				v.z = Count > 2 ? ValueF[2] : 0;
			}
			else
			{
				v.x = (FLOAT32)(Count > 0 ? ValueI[0] : 0);
				v.y = (FLOAT32)(Count > 1 ? ValueI[1] : 0);
				v.z = (FLOAT32)(Count > 2 ? ValueI[2] : 0);
			}

			return v;
		}

		virtual Vector2 getVector2d()
		{
			Vector2 v;

			if (IsFloat)
			{
				v.x = Count > 0 ? ValueF[0] : 0;
				v.y = Count > 1 ? ValueF[1] : 0;
			}
			else
			{
				v.x = (FLOAT32)(Count > 0 ? ValueI[0] : 0);
				v.y = (FLOAT32)(Count > 1 ? ValueI[1] : 0);
			}

			return v;
		}



		virtual ColourValue getColor()
		{
			ColourValue c;
			c.r = (0, Count > 0 ? ValueF[0] : 0);
			c.g = (1, Count > 1 ? ValueF[1] : 0);
			c.b = (2, Count > 2 ? ValueF[2] : 0);
			c.a = (3, Count > 3 ? ValueF[3] : 0);

			return c;
		}


		virtual rect<SINT32> getRect()
		{
			rect<SINT32> r;

			if (IsFloat)
			{
				r.UpperLeftCorner.x = (SINT32)(Count > 0 ? ValueF[0] : 0);
				r.UpperLeftCorner.y = (SINT32)(Count > 1 ? ValueF[1] : 0);
				r.LowerRightCorner.x = (SINT32)(Count > 2 ? ValueF[2] : r.UpperLeftCorner.x);
				r.LowerRightCorner.y = (SINT32)(Count > 3 ? ValueF[3] : r.UpperLeftCorner.y);
			}
			else
			{
				r.UpperLeftCorner.x = Count > 0 ? ValueI[0] : 0;
				r.UpperLeftCorner.y = Count > 1 ? ValueI[1] : 0;
				r.LowerRightCorner.x = Count > 2 ? ValueI[2] : r.UpperLeftCorner.x;
				r.LowerRightCorner.y = Count > 3 ? ValueI[3] : r.UpperLeftCorner.y;
			}
			return r;
		}

		virtual dimension2du getDimension2d()
		{
			dimension2d<UINT32> dim;

			if (IsFloat)
			{
				dim.Width = (UINT32)(Count > 0 ? ValueF[0] : 0);
				dim.Height = (UINT32)(Count > 1 ? ValueF[1] : 0);
			}
			else
			{
				dim.Width = (UINT32)(Count > 0 ? ValueI[0] : 0);
				dim.Height = (UINT32)(Count > 1 ? ValueI[1] : 0);
			}
			return dim;
		}

		virtual Matrix4 getMatrix()
		{
			Matrix4 ret;
			if (IsFloat)
			{
				for (UINT32 r = 0; r < 4; ++r)
					for (UINT32 c = 0; c<4; ++c)
						if (Count > c + r * 4)
							ret[r][c] = ValueF[c + r * 4];
			}
			else
			{
				for (UINT32 r = 0; r < 4; ++r)
					for (UINT32 c = 0; c<4; ++c)
						if (Count > c + r * 4)
							ret[r][c] = (FLOAT32)ValueI[c + r * 4];
			}
			return ret;
		}

		virtual Quaternion getQuaternion()
		{
			Quaternion ret;
			if (IsFloat)
			{
				ret.x = Count > 0 ? ValueF[0] : 0.0f;
				ret.y = Count > 1 ? ValueF[1] : 0.0f;
				ret.z = Count > 2 ? ValueF[2] : 0.0f;
				ret.w = Count > 3 ? ValueF[3] : 0.0f;
			}
			else
			{
				ret.x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.z = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.w = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
			}
			return ret;
		}

		virtual triangle3df getTriangle()
		{
			triangle3df ret;

			if (IsFloat)
			{
				ret.pointA.x = Count > 0 ? ValueF[0] : 0.0f;
				ret.pointA.y = Count > 1 ? ValueF[1] : 0.0f;
				ret.pointA.z = Count > 2 ? ValueF[2] : 0.0f;
				ret.pointB.x = Count > 3 ? ValueF[3] : 0.0f;
				ret.pointB.y = Count > 4 ? ValueF[4] : 0.0f;
				ret.pointB.z = Count > 5 ? ValueF[5] : 0.0f;
				ret.pointC.x = Count > 6 ? ValueF[6] : 0.0f;
				ret.pointC.y = Count > 7 ? ValueF[7] : 0.0f;
				ret.pointC.z = Count > 8 ? ValueF[8] : 0.0f;
			}
			else
			{
				ret.pointA.x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.pointA.y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.pointA.z = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.pointB.x = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
				ret.pointB.y = Count > 4 ? (FLOAT32)ValueI[4] : 0.0f;
				ret.pointB.z = Count > 5 ? (FLOAT32)ValueI[5] : 0.0f;
				ret.pointC.x = Count > 6 ? (FLOAT32)ValueI[6] : 0.0f;
				ret.pointC.y = Count > 7 ? (FLOAT32)ValueI[7] : 0.0f;
				ret.pointC.z = Count > 8 ? (FLOAT32)ValueI[8] : 0.0f;
			}

			return ret;
		}

		virtual Plane getPlane()
		{
			Plane ret;

			if (IsFloat)
			{
				ret.normal.x = Count > 0 ? ValueF[0] : 0.0f;
				ret.normal.y = Count > 1 ? ValueF[1] : 0.0f;
				ret.normal.z = Count > 2 ? ValueF[2] : 0.0f;
				ret.d = Count > 3 ? ValueF[3] : 0.0f;
			}
			else
			{
				ret.normal.x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.normal.y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.normal.z = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.d = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
			}

			return ret;
		}

		virtual AxisAlignedBox getBBox()
		{
			AxisAlignedBox ret;
			if (IsFloat)
			{
				ret.getMinimum().x = Count > 0 ? ValueF[0] : 0.0f;
				ret.getMinimum().y = Count > 1 ? ValueF[1] : 0.0f;
				ret.getMinimum().z = Count > 2 ? ValueF[2] : 0.0f;
				ret.getMaximum().x = Count > 3 ? ValueF[3] : 0.0f;
				ret.getMaximum().y = Count > 4 ? ValueF[4] : 0.0f;
				ret.getMaximum().z = Count > 5 ? ValueF[5] : 0.0f;
			}
			else
			{
				ret.getMinimum().x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.getMinimum().y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.getMinimum().z = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.getMaximum().x = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
				ret.getMaximum().y = Count > 4 ? (FLOAT32)ValueI[4] : 0.0f;
				ret.getMaximum().z = Count > 5 ? (FLOAT32)ValueI[5] : 0.0f;
			}
			return ret;

		}

		virtual line2df getLine2d()
		{
			line2df ret;
			if (IsFloat)
			{
				ret.start.x = Count > 0 ? ValueF[0] : 0.0f;
				ret.start.y = Count > 1 ? ValueF[1] : 0.0f;
				ret.end.x = Count > 2 ? ValueF[2] : 0.0f;
				ret.end.y = Count > 3 ? ValueF[3] : 0.0f;
			}
			else
			{
				ret.start.x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.start.y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.end.x = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.end.y = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
			}
			return ret;
		}

		virtual line3df getLine3d()
		{
			line3df ret;
			if (IsFloat)
			{
				ret.start.x = Count > 0 ? ValueF[0] : 0.0f;
				ret.start.y = Count > 1 ? ValueF[1] : 0.0f;
				ret.start.z = Count > 2 ? ValueF[2] : 0.0f;
				ret.end.x = Count > 3 ? ValueF[3] : 0.0f;
				ret.end.y = Count > 4 ? ValueF[4] : 0.0f;
				ret.end.z = Count > 5 ? ValueF[5] : 0.0f;
			}
			else
			{
				ret.start.x = Count > 0 ? (FLOAT32)ValueI[0] : 0.0f;
				ret.start.y = Count > 1 ? (FLOAT32)ValueI[1] : 0.0f;
				ret.start.z = Count > 2 ? (FLOAT32)ValueI[2] : 0.0f;
				ret.end.x = Count > 3 ? (FLOAT32)ValueI[3] : 0.0f;
				ret.end.y = Count > 4 ? (FLOAT32)ValueI[4] : 0.0f;
				ret.end.z = Count > 5 ? (FLOAT32)ValueI[5] : 0.0f;
			}
			return ret;
		}

		//! get float vector
		virtual vector<FLOAT32>::type getFloatArray()
		{
			if (!IsFloat)
			{
				ValueF.clear();
				for (UINT32 i = 0; i < Count; ++i)
					ValueF.push_back((FLOAT32)ValueI[i]);
			}
			return ValueF;
		}

		//! get int vector
		virtual vector<SINT32>::type getIntArray()
		{
			if (IsFloat)
			{
				ValueI.clear();
				for (UINT32 i = 0; i < Count; ++i)
					ValueI.push_back((SINT32)ValueF[i]);
			}
			return ValueI;
		}


		// setting values
		virtual void setInt(SINT32 intValue)
		{
			// set all values
			for (UINT32 i = 0; i < Count; ++i)
				if (IsFloat)
					ValueF[i] = (FLOAT32)intValue;
				else
					ValueI[i] = intValue;
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			// set all values
			for (UINT32 i = 0; i < Count; ++i)
				if (IsFloat)
					ValueF[i] = floatValue;
				else
					ValueI[i] = (SINT32)floatValue;
		}

		virtual void setBool(bool boolValue)
		{
			setInt(boolValue ? 1 : 0);
		}

		virtual void setString(const char* text)
		{
			// parse text

			const char* P = (const char*)text;

			reset();

			UINT32 i = 0;

			for (i = 0; i < Count && *P; ++i)
			{
				while (*P && P[0] != '-' && (P[0] == ' ' || (P[0] < '0' || P[0] > '9')))
					++P;

				// set value
				if (*P)
				{
					if (IsFloat)
					{
						FLOAT32 c = 0;
						P = fast_atof_move(P, c);
						ValueF[i] = c;
					}
					else
					{
						// todo: fix this to read ints properly
						FLOAT32 c = 0;
						P = fast_atof_move(P, c);
						ValueI[i] = (SINT32)c;

					}
				}
			}
			// todo: warning message
			//if (i < Count-1)
			//{
			//
			//}
		}

		virtual void setPosition(Position2d v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = (FLOAT32)v.x;
				if (Count > 1) ValueF[1] = (FLOAT32)v.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = v.x;
				if (Count > 1) ValueI[1] = v.y;
			}
		}

		virtual void setVector(Vector3 v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = v.x;
				if (Count > 1) ValueF[1] = v.y;
				if (Count > 2) ValueF[2] = v.z;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)v.x;
				if (Count > 1) ValueI[1] = (SINT32)v.y;
				if (Count > 2) ValueI[2] = (SINT32)v.z;
			}
		}

		virtual void setColor(ColourValue color)
		{
			reset();
			if (Count > 0) ValueF[0] = color.r;
			if (Count > 1) ValueF[1] = color.g;
			if (Count > 2) ValueF[2] = color.b;
			if (Count > 3) ValueF[3] = color.a;

		}



		virtual void setRect(rect<SINT32> value)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = (FLOAT32)value.UpperLeftCorner.x;
				if (Count > 1) ValueF[1] = (FLOAT32)value.UpperLeftCorner.y;
				if (Count > 2) ValueF[2] = (FLOAT32)value.LowerRightCorner.x;
				if (Count > 3) ValueF[3] = (FLOAT32)value.LowerRightCorner.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = value.UpperLeftCorner.x;
				if (Count > 1) ValueI[1] = value.UpperLeftCorner.y;
				if (Count > 2) ValueI[2] = value.LowerRightCorner.x;
				if (Count > 3) ValueI[3] = value.LowerRightCorner.y;
			}
		}

		virtual void setMatrix(Matrix4 value)
		{
			reset();
			if (IsFloat)
			{
				for (UINT32 r = 0; r < 4; ++r)
					for (UINT32 c = 0; c<4; ++c)
						if (Count > c + r * 4)
							ValueF[c + r * 4] = value[r][c];
			}
			else
			{
				for (UINT32 r = 0; r < 4; ++r)
					for (UINT32 c = 0; c<4; ++c)
						if (Count > c + r * 4)
							ValueI[c + r * 4] = (SINT32)value[r][c];
			}
		}

		virtual void setQuaternion(Quaternion value)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = value.x;
				if (Count > 1) ValueF[1] = value.y;
				if (Count > 2) ValueF[2] = value.z;
				if (Count > 3) ValueF[3] = value.w;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)value.x;
				if (Count > 1) ValueI[1] = (SINT32)value.y;
				if (Count > 2) ValueI[2] = (SINT32)value.z;
				if (Count > 3) ValueI[3] = (SINT32)value.w;
			}
		}

		virtual void setBoundingBox(AxisAlignedBox value)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = value.getMinimum().x;
				if (Count > 1) ValueF[1] = value.getMinimum().y;
				if (Count > 2) ValueF[2] = value.getMinimum().z;
				if (Count > 3) ValueF[3] = value.getMaximum().x;
				if (Count > 4) ValueF[4] = value.getMaximum().y;
				if (Count > 5) ValueF[5] = value.getMaximum().z;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)value.getMinimum().x;
				if (Count > 1) ValueI[1] = (SINT32)value.getMinimum().y;
				if (Count > 2) ValueI[2] = (SINT32)value.getMinimum().z;
				if (Count > 3) ValueI[3] = (SINT32)value.getMaximum().x;
				if (Count > 4) ValueI[4] = (SINT32)value.getMaximum().y;
				if (Count > 5) ValueI[5] = (SINT32)value.getMaximum().z;
			}
		}

		virtual void setPlane(Plane value)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = value.normal.x;
				if (Count > 1) ValueF[1] = value.normal.y;
				if (Count > 2) ValueF[2] = value.normal.z;
				if (Count > 3) ValueF[3] = value.d;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)value.normal.x;
				if (Count > 1) ValueI[1] = (SINT32)value.normal.y;
				if (Count > 2) ValueI[2] = (SINT32)value.normal.z;
				if (Count > 3) ValueI[3] = (SINT32)value.d;
			}
		}

		virtual void setTriangle3d(triangle3df value)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = value.pointA.x;
				if (Count > 1) ValueF[1] = value.pointA.y;
				if (Count > 2) ValueF[2] = value.pointA.z;
				if (Count > 3) ValueF[3] = value.pointB.x;
				if (Count > 4) ValueF[4] = value.pointB.y;
				if (Count > 5) ValueF[5] = value.pointB.z;
				if (Count > 6) ValueF[6] = value.pointC.x;
				if (Count > 7) ValueF[7] = value.pointC.y;
				if (Count > 8) ValueF[8] = value.pointC.z;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)value.pointA.x;
				if (Count > 1) ValueI[1] = (SINT32)value.pointA.y;
				if (Count > 2) ValueI[2] = (SINT32)value.pointA.z;
				if (Count > 3) ValueI[3] = (SINT32)value.pointB.x;
				if (Count > 4) ValueI[4] = (SINT32)value.pointB.y;
				if (Count > 5) ValueI[5] = (SINT32)value.pointB.z;
				if (Count > 6) ValueI[6] = (SINT32)value.pointC.x;
				if (Count > 7) ValueI[7] = (SINT32)value.pointC.y;
				if (Count > 8) ValueI[8] = (SINT32)value.pointC.z;
			}
		}

		virtual void setVector2d(Vector2 v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = v.x;
				if (Count > 1) ValueF[1] = v.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)v.x;
				if (Count > 1) ValueI[1] = (SINT32)v.y;
			}
		}
		/*
		virtual void setVector2d(Vector2 v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = (FLOAT32)v.x;
				if (Count > 1) ValueF[1] = (FLOAT32)v.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = v.x;
				if (Count > 1) ValueI[1] = v.y;
			}
		}
		*/
		virtual void setLine2d(line2di v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = (FLOAT32)v.start.x;
				if (Count > 1) ValueF[1] = (FLOAT32)v.start.y;
				if (Count > 2) ValueF[2] = (FLOAT32)v.end.x;
				if (Count > 3) ValueF[3] = (FLOAT32)v.end.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = v.start.x;
				if (Count > 1) ValueI[1] = v.start.y;
				if (Count > 2) ValueI[2] = v.end.x;
				if (Count > 3) ValueI[3] = v.end.y;
			}
		}

		virtual void setLine2d(line2df v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = v.start.x;
				if (Count > 1) ValueF[1] = v.start.y;
				if (Count > 2) ValueF[2] = v.end.x;
				if (Count > 3) ValueF[3] = v.end.y;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)v.start.x;
				if (Count > 1) ValueI[1] = (SINT32)v.start.y;
				if (Count > 2) ValueI[2] = (SINT32)v.end.x;
				if (Count > 3) ValueI[3] = (SINT32)v.end.y;
			}
		}

		virtual void setDimension2d(dimension2du v)
		{
			reset();
			if (IsFloat)
			{
				if (Count > 0) ValueF[0] = (FLOAT32)v.Width;
				if (Count > 1) ValueF[1] = (FLOAT32)v.Height;
			}
			else
			{
				if (Count > 0) ValueI[0] = (SINT32)v.Width;
				if (Count > 1) ValueI[1] = (SINT32)v.Height;
			}
		}

		//! set float vector
		virtual void setFloatArray(vector<FLOAT32>::type &vals)
		{
			reset();

			for (UINT32 i = 0; i < vals.size() && i < Count; ++i)
			{
				if (IsFloat)
					ValueF[i] = vals[i];
				else
					ValueI[i] = (SINT32)vals[i];
			}
		}

		//! set int vector
		virtual void setIntArray(vector<SINT32>::type &vals)
		{
			reset();

			for (UINT32 i = 0; i < vals.size() && i < Count; ++i)
			{
				if (IsFloat)
					ValueF[i] = (FLOAT32)vals[i];
				else
					ValueI[i] = vals[i];
			}
		}


		//! is it a number list?
		virtual bool isNumberList()
		{
			return true;
		}

		//! is it a float list?
		virtual bool isFloat()
		{
			return IsFloat;
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			if (IsFloat)
				return EAT_FLOATARRAY;
			else
				return EAT_INTARRAY;
		}

		virtual const wchar_t* getTypeString() const
		{
			if (IsFloat)
				return L"floatlist";
			else
				return L"intlist";
		}

	protected:

		//! clear all values
		void reset()
		{
			if (IsFloat)
				for (UINT32 i = 0; i < Count; ++i)
					ValueF[i] = 0.0f;
			else
				for (UINT32 i = 0; i < Count; ++i)
					ValueI[i] = 0;
		}

		vector<SINT32>::type ValueI;
		vector<FLOAT32>::type ValueF;
		UINT32 Count;
		bool IsFloat;
	};


	// 浮点颜色属性的实现类
	class CColorfAttribute : public CNumbersAttribute
	{
	public:

		CColorfAttribute(const char* name, ColourValue value) : CNumbersAttribute(name, value) {}

		virtual SINT32 getInt()
		{
			return getColor().getAsARGB();
		}



		virtual void setInt(SINT32 intValue)
		{
			ColourValue c = ColourValue(intValue);
			ValueF[0] = c.getRed();
			ValueF[1] = c.getGreen();
			ValueF[2] = c.getBlue();
			ValueF[3] = c.getAlpha();
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			setInt((SINT32)floatValue);
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_COLORF;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"colorf";
		}
	};



	// 颜色属性的实现类
	class CColorAttribute : public CNumbersAttribute
	{
	public:

		CColorAttribute(const char* name, const ColourValue& value) : CNumbersAttribute(name, value) {}

//		CColorAttribute(const char* name, const ColourValue& value) : CNumbersAttribute(name, value) {}

		virtual SINT32 getInt()
		{
			return getColor().getAsARGB();
		}


		virtual void setInt(SINT32 intValue)
		{
			ColourValue c = ColourValue(intValue);
			ValueF[0] = c.getRed();
			ValueF[1] = c.getGreen();
			ValueF[2] = c.getBlue();
			ValueF[3] = c.getAlpha();
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			setInt((SINT32)floatValue);
		}

		virtual String getString()
		{

			ColourValue c = getColor();
			StringStream ws;
			ws << c.getAlpha() << c.getRed() << c.getGreen() << c.getBlue();

			return ws.str();
		}

		virtual void setString(const char* text)
		{
			UINT32 c;
			if (sscanf(text, "%08x", &c) != 1)
			{
				CNumbersAttribute::setString(text);
			}
			else
				setColor(ColourValue(c));
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_COLOR;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"color";
		}

	};


	// Attribute implemented for 3d vectors
	class CVector3DAttribute : public CNumbersAttribute
	{
	public:

		CVector3DAttribute(const char* name, Vector3 value) : CNumbersAttribute(name, value) {}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_VECTOR3D;
		}

		virtual Matrix4 getMatrix()
		{
			Matrix4 ret = Matrix4::IDENTITY;
			ret.setTrans(Vector3(ValueF[0], ValueF[1], ValueF[2]));
			return ret;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"vector3d";
		}
	};

	// 2d向量属性的实现类
	class CVector2DAttribute : public CNumbersAttribute
	{
	public:

		CVector2DAttribute(const char* name, Vector2 value) : CNumbersAttribute(name, value) {}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_VECTOR2D;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"vector2d";
		}
	};

	//  2d位置属性的实现类
	class CPosition2DAttribute : public CNumbersAttribute
	{
	public:

		CPosition2DAttribute(const char* name, Position2d value) : CNumbersAttribute(name, value) {}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_POSITION2D;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"position";
		}
	};



	// 矩形属性实现类
	class CRectAttribute : public CNumbersAttribute
	{
	public:

		CRectAttribute(const char* name, rect<SINT32> value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_RECT;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"rect";
		}
	};


	// Dismesion属性实现类
	class CDimension2dAttribute : public CNumbersAttribute
	{
	public:

		CDimension2dAttribute(const char* name, dimension2d<UINT32> value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_DIMENSION2D;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"dimension2d";
		}
	};

	// 矩阵属性实现类
	class CMatrixAttribute : public CNumbersAttribute
	{
	public:

		CMatrixAttribute(const char* name, Matrix4 value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_MATRIX;
		}

		virtual Quaternion getQuaternion()
		{

			return getMatrix().extractQuaternion();
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"matrix";
		}
	};

	// 四元数属性实现类
	class CQuaternionAttribute : public CNumbersAttribute
	{
	public:

		CQuaternionAttribute(const char* name, Quaternion value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_QUATERNION;
		}

		virtual Matrix4 getMatrix()
		{
			Matrix3 mat3;
			getQuaternion().ToRotationMatrix(mat3);
			return mat3;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"Quaternion";
		}
	};


	// AABB属性实现类
	class CBBoxAttribute : public CNumbersAttribute
	{
	public:

		CBBoxAttribute(const char* name, AxisAlignedBox value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_BBOX;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"box3d";
		}
	};

	// 平面属性实现类
	class CPlaneAttribute : public CNumbersAttribute
	{
	public:

		CPlaneAttribute(const char* name, Plane value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_PLANE;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"plane";
		}
	};

	// 三角形属性实现类
	class CTriangleAttribute : public CNumbersAttribute
	{
	public:

		CTriangleAttribute(const char* name, triangle3df value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_TRIANGLE3D;
		}

		virtual Plane getPlane()
		{
			return getTriangle().getPlane();
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"triangle";
		}
	};


	// 2d线段属性实现类
	class CLine2dAttribute : public CNumbersAttribute
	{
	public:

		CLine2dAttribute(const char* name, line2df value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_LINE2D;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"line2d";
		}
	};

	// 3d线段属性实现类
	class CLine3dAttribute : public CNumbersAttribute
	{
	public:

		CLine3dAttribute(const char* name, line3df value) : CNumbersAttribute(name, value) { }

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_LINE3D;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"line3d";
		}
	};


	// 2d向量
	// dimension2du

	/*
	特殊的属性
	*/

	// 枚举文本属性的实现
	class CEnumAttribute : public IAttribute
	{
	public:

		CEnumAttribute(const char* name, const char* value, const char* const* literals)
		{
			Name = name;
			setEnum(value, literals);
		}

		virtual void setEnum(const char* enumValue, const char* const* enumerationLiterals)
		{
			int literalCount = 0;

			if (enumerationLiterals)
			{
				SINT32 i;
				for (i = 0; enumerationLiterals[i]; ++i)
					++literalCount;
				//EnumLiterals.reallocate(literalCount);
				for (i = 0; enumerationLiterals[i]; ++i)
					EnumLiterals.push_back(enumerationLiterals[i]);
			}

			setString(enumValue);
		}

		virtual SINT32 getInt()
		{
			for (SINT32 i = 0; EnumLiterals.size(); ++i)
				//if (Value.equals_ignore_case(EnumLiterals[i]))
				if (StringUtil::equals(Value, EnumLiterals[i]))
				{
					return i;
				}

			return -1;
		}

		virtual FLOAT32 getFloat()
		{
			return (FLOAT32)getInt();
		}

		virtual bool getBool()
		{
			return (getInt() != 0); 
		}

		virtual String getString()
		{
			return Value;
		}

		virtual StringW getStringW()
		{
			return StringUtil::StringToStringW(Value);//StringW(Value.c_str());
		}

		virtual void setInt(SINT32 intValue)
		{
			if (intValue >= 0 && intValue < (SINT32)EnumLiterals.size())
				Value = EnumLiterals[intValue];
			else
				Value = "";
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			setInt((SINT32)floatValue);
		};

		virtual void setString(const char* text)
		{
			Value = text;
		}

		virtual const char* getEnum()
		{
			return Value.c_str();
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_ENUM;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"enum";
		}

		String Value;
		vector<String>::type EnumLiterals;
	};





	// 字符串属性的实现
	class CStringAttribute : public IAttribute
	{
	public:

		CStringAttribute(const char* name, const char* value)
		{
			IsStringW = false;
			Name = name;
			setString(value);
		}

		CStringAttribute(const char* name, const wchar_t* value)
		{
			IsStringW = true;
			Name = name;
			setString(value);
		}

		CStringAttribute(const char* name, void* binaryData, SINT32 lenghtInBytes)
		{
			IsStringW = false;
			Name = name;
			setBinary(binaryData, lenghtInBytes);
		}

		virtual SINT32 getInt()
		{
			if (IsStringW)
				return _wtoi(ValueW.c_str());
			else
				return atoi(Value.c_str());
		}

		virtual FLOAT32 getFloat()
		{
			if (IsStringW)
				return fast_atof(StringUtil::StringWToString(ValueW).c_str());//return fast_atof(String(ValueW.c_str()).c_str());
			else
				return fast_atof(Value.c_str());
		}

		virtual bool getBool()
		{
			if (IsStringW)
				return StringUtil::equals(StringUtil::StringWToString(ValueW), String("true"), false);//return ValueW.equals_ignore_case(L"true");
			else
				return StringUtil::equals(Value, String("true"), false);
		}

		virtual String getString()
		{
			if (IsStringW)
				return StringUtil::StringWToString(ValueW);
			else
				return Value;
		}
		virtual StringW getStringW()
		{
			if (IsStringW)
				return ValueW;
			else
				return StringUtil::StringToStringW(Value);
		}

		virtual void setInt(SINT32 intValue)
		{
			if (IsStringW)
			{
				StringWStream ws;
				ws << intValue;
				ws >> ValueW;
			}
			else
			{
				StringStream ss;
				ss << intValue;
				ss >> Value;
			}
				 
		}

		virtual void setFloat(FLOAT32 floatValue)
		{
			if (IsStringW)
			{
				StringWStream ws;
				ws << floatValue;
				ws >> ValueW;
			}
			else
			{
				StringStream ss;
				ss << floatValue;
				ss >> Value;
			}
		};

		virtual void setString(const char* text)
		{
			if (IsStringW)
				ValueW = StringUtil::StringToStringW(text);
			else
				Value = text;
		}

		virtual void setString(const wchar_t* text)
		{
			if (IsStringW)
				ValueW = text;
			else
				Value = StringUtil::StringWToString(text);
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_STRING;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"string";
		}

		virtual void getBinary(void* outdata, SINT32 maxLength)
		{
			SINT32 dataSize = maxLength;
			c8* datac8 = (c8*)(outdata);
			SINT32 p = 0;
			const c8* dataString = Value.c_str();

			for (SINT32 i = 0; i < dataSize; ++i)
				datac8[i] = 0;

			while (dataString[p] && p < dataSize)
			{
				SINT32 v = getByteFromHex((c8)dataString[p * 2]) * 16;

				if (dataString[(p * 2) + 1])
					v += getByteFromHex((c8)dataString[(p * 2) + 1]);

				datac8[p] = v;
				++p;
			}
		};

		virtual void setBinary(void* data, SINT32 maxLength)
		{
			SINT32 dataSize = maxLength;
			c8* datac8 = (c8*)(data);
			char tmp[3];
			tmp[2] = 0;
			Value = "";

			for (SINT32 b = 0; b < dataSize; ++b)
			{
				getHexStrFromByte(datac8[b], tmp);
				Value.append(tmp);
			}
		};

		bool IsStringW;
		String Value;
		StringW ValueW;

	protected:

		static inline SINT32 getByteFromHex(c8 h)
		{
			if (h >= '0' && h <= '9')
				return h - '0';

			if (h >= 'a' && h <= 'f')
				return h - 'a' + 10;

			return 0;
		}

		static inline void getHexStrFromByte(c8 byte, c8* out)
		{
			SINT32 b = (byte & 0xf0) >> 4;

			for (SINT32 i = 0; i < 2; ++i)
			{
				if (b >= 0 && b <= 9)
					out[i] = b + '0';
				if (b >= 10 && b <= 15)
					out[i] = (b - 10) + 'a';

				b = byte & 0x0f;
			}
		}
	};

	// 二进制数据属性的实现
	class CBinaryAttribute : public CStringAttribute
	{
	public:

		CBinaryAttribute(const char* name, void* binaryData, SINT32 lenghtInBytes)
			: CStringAttribute(name, binaryData, lenghtInBytes)
		{

		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_BINARY;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"binary";
		}
	};



	// 纹理引用的属性的实现
	class CTextureAttribute : public IAttribute
	{
	public:

		CTextureAttribute(const char* name, ITexture* value, IVideoDriver* driver, const path& filename)
			: Value(0), Driver(driver), OverrideName(filename)
		{
			if (Driver)
				Driver->grab();

			Name = name;
			setTexture(value);
		}

		~CTextureAttribute()
		{
			if (Driver)
				Driver->drop();

			if (Value)
				Value->drop();
		}

		virtual ITexture* getTexture()
		{
			return Value;
		}

		virtual bool getBool()
		{
			return (Value != 0);
		}

		virtual StringW getStringW()
		{
			 
			if (OverrideName.size())
				return StringUtil::StringToStringW(OverrideName);//StringW(OverrideName);

			if (Value)
				return StringUtil::StringToStringW(Value->getName().getPath()); //StringW(Value->getName().getPath().c_str());

			return StringW(0);
		}

		virtual String getString()
		{
			 
			if (OverrideName.size())
				return String(OverrideName);

			if (Value)
				return String(Value->getName().getPath().c_str());

			return String(0);
		}

		virtual void setString(const char* text)
		{
			if (Driver)
			{
				if (text && *text)
				{
					setTexture(Driver->getTexture(text));
					OverrideName = text;
				}
				else
					setTexture(0);
			}
		}

		virtual void setTexture(ITexture* value)
		{
			if (value == Value)
				return;

			if (Value)
				Value->drop();

			Value = value;

			if (Value)
				Value->grab();
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_TEXTURE;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"texture";
		}

		ITexture* Value;
		IVideoDriver* Driver;
		path OverrideName;
	};



	// StringW数组的属性实现
	class CStringWArrayAttribute : public IAttribute
	{
	public:

		CStringWArrayAttribute(const char* name, const vector<StringW>& value)
		{
			Name = name;
			setArray(value);
		}

		virtual vector<StringW> getArray()
		{
			return Value;
		}

		virtual void setArray(const vector<StringW>& value)
		{
			Value = value;
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_STRINGWARRAY;
		}

		virtual const wchar_t* getTypeString() const
		{
			return L"stringwvector";
		}

		vector<StringW> Value;
	};


	// 用户指针属性的实现
	class CUserPointerAttribute : public IAttribute
	{
	public:

		CUserPointerAttribute(const char* name, void* value)
		{
			Name = name;
			Value = value;
		}

		virtual SINT32 getInt()
		{
			return *static_cast<SINT32*>(Value);
		}

		virtual bool getBool()
		{
			return (Value != 0);
		}

		virtual StringW getStringW()
		{
			wchar_t buf[32];
			swprintf(buf, 32, L"%p", Value);

			return StringW(buf);
		}

		virtual void setString(const char* text)
		{
			UINT32 tmp;
			sscanf(text, "0x%x", &tmp);
			Value = (void *)tmp;
		}

		virtual E_ATTRIBUTE_TYPE getType() const
		{
			return EAT_USER_POINTER;
		}

		virtual void setUserPointer(void* v)
		{
			Value = v;
		}

		virtual void* getUserPointer()
		{
			return Value;
		}


		virtual const wchar_t* getTypeString() const
		{
			return L"userPointer";
		}

		void* Value;
	};
}