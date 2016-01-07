#ifndef _SAPPHIRE_I_ATTRIBUTE_
#define _SAPPHIRE_I_ATTRIBUTE_

#include "SapphireEAttributes.h"
#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"
#include "SapphireColorValue.h"
#include "SapphireVector3.h"
#include "SapphireVector2.h"
#include "SapphireMatrix4.h"
#include "SapphireRectangle.h"
#include "SapphireQuaternion.h"
#include "SapphireLine2D.h"
#include "SapphireLine3D.h"
#include "SapphireDimension2D.h"
#include "SapphireTriangle3D.h"
#include "SapphirePlane.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireITexture.h"


namespace Sapphire
{
	class IAttribute : public virtual IReferenceCounter
	{
	public:

		virtual ~IAttribute() {};

		virtual SINT32 getInt()				{ return 0; }
		virtual FLOAT32 getFloat()				{ return 0; }
		virtual ColourValue getColorf()		{ return ColourValue(1.0f, 1.0f, 1.0f, 1.0f); }
		virtual ColourValue getColor()		{ return ColourValue::getColourValue(255, 255, 255, 255); }
		virtual String getString()		{ return String(getString().c_str()); }
		virtual StringW getStringW()		{ return StringW(); }
		virtual vector<StringW> getArray()	{ return vector<StringW>(); };
		virtual bool getBool()				{ return false; }
		virtual void getBinary(void* outdata, SINT32 maxLength) {};
		virtual Vector3 getVector()		{ return Vector3(); }
		virtual Position2d getPosition()	{ return Position2d(); }
		virtual rect<SINT32> getRect()		{ return rect<SINT32>(); }
		virtual Quaternion getQuaternion(){ return Quaternion(); }
		virtual Matrix4 getMatrix()		{ return Matrix4(); }
		virtual triangle3df getTriangle()		{ return triangle3df(); }
		virtual Vector2 getVector2d()		{ return Vector2(); }
		virtual Vector2 getVector2di()		{ return Vector2(); }
		virtual line2df getLine2d()		{ return line2df(); }
		virtual line2di getLine2di()		{ return line2di(); }
		virtual line3df getLine3d()		{ return line3df(); }
		virtual line3di getLine3di()		{ return line3di(); }
		virtual dimension2du getDimension2d()	{ return dimension2du(); }
		virtual AxisAlignedBox getBBox()		{ return AxisAlignedBox(); }
		virtual Plane getPlane()		{ return Plane(); }

		virtual ITexture* getTexture()		{ return 0; }
		virtual const char* getEnum()			{ return 0; }
		virtual void* getUserPointer()			{ return 0; }

		virtual void setInt(SINT32 intValue)		{};
		virtual void setFloat(FLOAT32 floatValue)		{};
		virtual void setString(const char* text)	{};
		virtual void setString(const wchar_t* text){ setString(StringW(text).c_str()); };
		virtual void setArray(const vector<StringW>& arr)	{};
		virtual void setColor(ColourValue color)	{};
		virtual void setColor(ColourValue color)	{};
		virtual void setBool(bool boolValue)		{};
		virtual void setBinary(void* data, SINT32 maxLenght) {};
		virtual void setVector(Vector3 v)	{};
		virtual void setPosition(Position2d v)	{};
		virtual void setRect(rect<SINT32> v)		{};
		virtual void setQuaternion(Quaternion v) {};
		virtual void setMatrix(Matrix4 v) {};
		virtual void setTriangle(triangle3df v) {};
		virtual void setVector2d(Vector2 v) {};
		virtual void setVector2d(Vector2 v) {};
		virtual void setLine2d(line2df v) {};
		virtual void setLine2d(line2di v) {};
		virtual void setLine3d(line3df v) {};
		virtual void setLine3d(line3di v) {};
		virtual void setDimension2d(dimension2du v) {};
		virtual void setBBox(AxisAlignedBox v) {};
		virtual void setPlane(Plane v) {};
		virtual void setUserPointer(void* v)	{};

		virtual void setEnum(const char* enumValue, const char* const* enumerationLiterals) {};
		virtual void setTexture(ITexture*, const path& filename)	{};

		String Name;

		virtual E_ATTRIBUTE_TYPE getType() const = 0;
		virtual const wchar_t* getTypeString() const = 0;
	};
}

#endif