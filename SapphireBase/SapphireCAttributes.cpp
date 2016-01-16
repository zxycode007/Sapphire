#include "SapphirePrerequisites.h"
#include "SapphireCAtrributes.h"
#include "SapphireCAttributeImpl.h"
#include "SapphireITexture.h"
#include "SapphireIXMLWriter.h"
#include "SapphireIXMLReader.h"
#include "SapphireIVideoDriver.h"


namespace Sapphire
{
	CAttributes::CAttributes(IVideoDriver* driver)
		: Driver(driver)
	{
#ifdef _DEBUG
		setDebugName("CAttributes");
#endif

		if (Driver)
			Driver->grab();
	}

	CAttributes::~CAttributes()
	{
		clear();

		if (Driver)
			Driver->drop();
	}


	//! Removes all attributes
	void CAttributes::clear()
	{
		for (UINT32 i = 0; i<Attributes.size(); ++i)
			Attributes[i]->drop();

		Attributes.clear();
	}


	//! Sets a string attribute.
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	void CAttributes::setAttribute(const c8* attributeName, const c8* value)
	{
		for (UINT32 i = 0; i<Attributes.size(); ++i)
			if (Attributes[i]->Name == attributeName)
			{
				if (!value)
				{
					
					Attributes[i]->drop();
					vector<IAttribute*>::iterator it = Attributes.begin() + i;
					Attributes.erase(it);
				}
				else
					Attributes[i]->setString(value);

				return;
			}

		if (value)
		{
			Attributes.push_back(new CStringAttribute(attributeName, value));
		}
	}

	//! Gets a string attribute.
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setStringAttribute()
	//! or 0 if attribute is not set.
	String CAttributes::getAttributeAsString(const c8* attributeName)
	{
		String str;

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getString();
		else
			return str;
	}

	//! Gets a string attribute.
	//! \param attributeName: Name of the attribute to get.
	//! \param target: Buffer where the string is copied to.
	void CAttributes::getAttributeAsString(const c8* attributeName, char* target)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
		{
			String str = att->getString();
			strcpy(target, str.c_str());
		}
		else
			target[0] = 0;
	}

	//! Returns string attribute value by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	String CAttributes::getAttributeAsString(SINT32 index)
	{
		String str;

		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getString();

		return str;
	}


	//! Sets a string attribute.
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	void CAttributes::setAttribute(const c8* attributeName, const wchar_t* value)
	{
		for (UINT32 i = 0; i<Attributes.size(); ++i)
		{
			if (Attributes[i]->Name == attributeName)
			{
				if (!value)
				{
					Attributes[i]->drop();
					vector<IAttribute*>::iterator it = Attributes.begin() + i;
					Attributes.erase(it);
				}
				else
					Attributes[i]->setString(value);

				return;
			}
		}

		if (value)
		{
			Attributes.push_back(new CStringAttribute(attributeName, value));
		}
	}

	//! Gets a string attribute.
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setStringAttribute()
	//! or 0 if attribute is not set.
	StringW CAttributes::getAttributeAsStringW(const c8* attributeName)
	{
		StringW str;

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			str = att->getStringW();

		return str;
	}

	//! Gets a string attribute.
	//! \param attributeName: Name of the attribute to get.
	//! \param target: Buffer where the string is copied to.
	void CAttributes::getAttributeAsStringW(const c8* attributeName, wchar_t* target)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
		{
			StringW str = att->getStringW();
			wcscpy(target, str.c_str());
		}
		else
			target[0] = 0;
	}

	//! Returns string attribute value by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	StringW CAttributes::getAttributeAsStringW(SINT32 index)
	{

		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getStringW();
		else
			return StringW();
	}


	//! Adds an attribute as an array of wide strings
	void CAttributes::addArray(const c8* attributeName, const vector<StringW>::type& value)
	{
		Attributes.push_back(new CStringWArrayAttribute(attributeName, value));
	}

	//! Sets an attribute value as an array of wide strings.
	void CAttributes::setAttribute(const c8* attributeName, const vector<StringW>::type& value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setArray(value);
		else
		{
			Attributes.push_back(new CStringWArrayAttribute(attributeName, value));
		}
	}

	//! Gets an attribute as an array of wide strings.
	vector<StringW>::type CAttributes::getAttributeAsArray(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getArray();
		else
			return vector<StringW>::type();
	}

	//! Returns attribute value as an array of wide strings by index.
	vector<StringW>::type CAttributes::getAttributeAsArray(SINT32 index)
	{
		vector<StringW>::type ret;

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getArray();

		return ret;
	}

	//! Sets an attribute as an array of wide strings
	void CAttributes::setAttribute(SINT32 index, const vector<StringW>::type& value)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setArray(value);
	}




	//! Returns attribute index from name, -1 if not found
	SINT32 CAttributes::findAttribute(const c8* attributeName) const
	{
		for (UINT32 i = 0; i<Attributes.size(); ++i)
			if (Attributes[i]->Name == attributeName)
				return i;

		return -1;
	}


	IAttribute* CAttributes::getAttributeP(const c8* attributeName) const
	{
		for (UINT32 i = 0; i<Attributes.size(); ++i)
			if (Attributes[i]->Name == attributeName)
				return Attributes[i];

		return 0;
	}


	//! Sets a attribute as boolean value
	void CAttributes::setAttribute(const c8* attributeName, bool value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setBool(value);
		else
		{
			Attributes.push_back(new CBoolAttribute(attributeName, value));
		}
	}

	//! Gets a attribute as boolean value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute() as bool
	//! or 0 if attribute is not set.
	bool CAttributes::getAttributeAsBool(const c8* attributeName)
	{
		bool ret = false;

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getBool();

		//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return ret;
	}

	//! Sets a attribute as integer value
	void CAttributes::setAttribute(const c8* attributeName, SINT32 value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setInt(value);
		else
		{
			Attributes.push_back(new CIntAttribute(attributeName, value));
		}
	}

	//! Gets a attribute as integer value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute() as integer
	//! or 0 if attribute is not set.
	SINT32 CAttributes::getAttributeAsInt(const c8* attributeName) const
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getInt();
		else
			return 0;
	}

	//! Sets a attribute as float value
	void CAttributes::setAttribute(const c8* attributeName, Real value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setFloat(value);
		else
			Attributes.push_back(new CFloatAttribute(attributeName, value));
	}

	//! Gets a attribute as integer value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute() as float value
	//! or 0 if attribute is not set.
	Real CAttributes::getAttributeAsFloat(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getFloat();

		return 0.f;
	}

	//! Sets a attribute as color
	void CAttributes::setAttribute(const c8* attributeName, ColourValue value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setColor(value);
		else
			Attributes.push_back(new CColorAttribute(attributeName, value));
	}

	//! Gets an attribute as color
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	ColourValue CAttributes::getAttributeAsColor(const c8* attributeName)
	{
		ColourValue ret(0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getColor();

		return ret;
	}

	//! Sets a attribute as floating point color
	//void CAttributes::setAttribute(const c8* attributeName, ColourValue value)
	//{
	//	IAttribute* att = getAttributeP(attributeName);
	//	if (att)
	//		att->setColor(value);
	//	else
	//		Attributes.push_back(new CColorfAttribute(attributeName, value));
	//}

	//! Gets an attribute as floating point color
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	//ColourValue CAttributes::getAttributeAsColorf(const c8* attributeName)
	//{
	//	IAttribute* att = getAttributeP(attributeName);
	//	if (att)
	//		return att->getColorf();
	//	else
	//		return ColourValue();
	//}

	//! Sets a attribute as 2d position
	void CAttributes::setAttribute(const c8* attributeName, Position2d value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setPosition(value);
		else
			Attributes.push_back(new CPosition2DAttribute(attributeName, value));
	}

	//! Gets an attribute as 2d position
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	//Position2d CAttributes::getAttributeAsPosition2d(const c8* attributeName)
	//{
	//	IAttribute* att = getAttributeP(attributeName);
	//	if (att)
	//		return att->getPosition();
	//	else
	//		return Position2d();
	//}

	//! Sets a attribute as rectangle
	void CAttributes::setAttribute(const c8* attributeName, rect<SINT32> value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setRect(value);
		else
			Attributes.push_back(new CRectAttribute(attributeName, value));
	}

	//! Gets an attribute as rectangle
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	rect<SINT32> CAttributes::getAttributeAsRect(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getRect();
		else
			return rect<SINT32>();
	}

	//! Sets a attribute as dimension2d
	void CAttributes::setAttribute(const c8* attributeName, dimension2d<UINT32> value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setDimension2d(value);
		else
			Attributes.push_back(new CDimension2dAttribute(attributeName, value));
	}

	//! Gets an attribute as dimension2d
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	dimension2d<UINT32> CAttributes::getAttributeAsDimension2d(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getDimension2d();
		else
			return dimension2d<UINT32>();
	}

	//! Sets a attribute as vector
	void CAttributes::setAttribute(const c8* attributeName, Vector3 value)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setVector(value);
		else
			Attributes.push_back(new CVector3DAttribute(attributeName, value));
	}

	//! Sets a attribute as vector
	//void CAttributes::setAttribute(const c8* attributeName, Vector2 value)
	//{
	//	IAttribute* att = getAttributeP(attributeName);
	//	if (att)
//			att->setVector2d(value);
//		else
//			Attributes.push_back(new CVector2DAttribute(attributeName, value));
//	}

	//! Gets an attribute as vector
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	Vector3 CAttributes::getAttributeAsVector3d(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getVector();
		else
			return Vector3();
	}

	//! Gets an attribute as vector
	Vector2 CAttributes::getAttributeAsVector2d(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getVector2d();
		else
			return Vector2();
	}

	//! Sets an attribute as binary data
	void CAttributes::setAttribute(const c8* attributeName, void* data, SINT32 dataSizeInBytes)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setBinary(data, dataSizeInBytes);
		else
			Attributes.push_back(new CBinaryAttribute(attributeName, data, dataSizeInBytes));
	}

	//! Gets an attribute as binary data
	//! \param attributeName: Name of the attribute to get.
	void CAttributes::getAttributeAsBinaryData(const c8* attributeName, void* outData, SINT32 maxSizeInBytes)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->getBinary(outData, maxSizeInBytes);
	}

	//! Sets an attribute as enumeration
	void CAttributes::setAttribute(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setEnum(enumValue, enumerationLiterals);
		else
			Attributes.push_back(new CEnumAttribute(attributeName, enumValue, enumerationLiterals));
	}

	//! Gets an attribute as enumeration
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	const char* CAttributes::getAttributeAsEnumeration(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getEnum();
		else
			return 0;
	}

	//! Gets an attribute as enumeration
	SINT32 CAttributes::getAttributeAsEnumeration(const c8* attributeName, const char* const* enumerationLiteralsToUse)
	{
		IAttribute* att = getAttributeP(attributeName);

		if (enumerationLiteralsToUse && att)
		{
			const char* value = att->getEnum();
			if (value)
			{
				for (SINT32 i = 0; enumerationLiteralsToUse[i]; ++i)
					if (!strcmp(value, enumerationLiteralsToUse[i]))
						return i;
			}
		}

		return -1;
	}

	//! Gets the list of enumeration literals of an enumeration attribute
	//! \param attributeName: Name of the attribute to get.
	void CAttributes::getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, vector<String>::type& outLiterals)
	{
		IAttribute* att = getAttributeP(attributeName);

		if (att && att->getType() == EAT_ENUM)
			outLiterals = ((CEnumAttribute*)att)->EnumLiterals;
	}

	//! Sets an attribute as texture reference
	void CAttributes::setAttribute(const c8* attributeName, ITexture* value, const path& filename)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setTexture(value, filename);
		else
			Attributes.push_back(new CTextureAttribute(attributeName, value, Driver, filename));
	}


	//! Gets an attribute as texture reference
	//! \param attributeName: Name of the attribute to get.
	ITexture* CAttributes::getAttributeAsTexture(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getTexture();
		else
			return 0;
	}

	//! Gets an attribute as texture reference
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	ITexture* CAttributes::getAttributeAsTexture(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getTexture();
		else
			return 0;
	}


	//! Returns amount of string attributes set in this scene manager.
	UINT32 CAttributes::getAttributeCount() const
	{
		return Attributes.size();
	}

	//! Returns string attribute name by index.
	//! \param index: Index value, must be between 0 and getStringAttributeCount()-1.
	const c8* CAttributes::getAttributeName(SINT32 index)
	{
		if ((UINT32)index >= Attributes.size())
			return 0;

		return Attributes[index]->Name.c_str();
	}

	//! Returns the type of an attribute
	E_ATTRIBUTE_TYPE CAttributes::getAttributeType(const c8* attributeName)
	{
		E_ATTRIBUTE_TYPE ret = EAT_UNKNOWN;

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getType();

		return ret;
	}

	//! Returns attribute type by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	E_ATTRIBUTE_TYPE CAttributes::getAttributeType(SINT32 index)
	{
		if ((UINT32)index >= Attributes.size())
			return EAT_UNKNOWN;

		return Attributes[index]->getType();
	}

	//! Returns the type of an attribute
	const StringW CAttributes::getAttributeTypeString(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getTypeString();
		else
			return L"unknown";
	}

	//! Returns attribute type string by index.
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	const StringW CAttributes::getAttributeTypeString(SINT32 index)
	{
		if ((UINT32)index >= Attributes.size())
			return L"unknown";

		return Attributes[index]->getTypeString();
	}

	//! Gets an attribute as boolean value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	bool CAttributes::getAttributeAsBool(SINT32 index)
	{
		bool ret = false;

		if ((UINT32)index < Attributes.size())
			ret = Attributes[index]->getBool();

		//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return ret;
	}

	//! Gets an attribute as integer value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	SINT32 CAttributes::getAttributeAsInt(SINT32 index) const
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getInt();
		else
			return 0;
	}

	//! Gets an attribute as float value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	Real CAttributes::getAttributeAsFloat(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getFloat();
		else
			return 0.f;
	}

	//! Gets an attribute as color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	ColourValue CAttributes::getAttributeAsColor(SINT32 index)
	{
		ColourValue ret(0);

		if ((UINT32)index < Attributes.size())
			ret = Attributes[index]->getColor();

		return ret;
	}

	//! Gets an attribute as floating point color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	//ColourValue CAttributes::getAttributeAsColor(SINT32 index)
//	{
//		if ((UINT32)index < Attributes.size())
//			return Attributes[index]->getColorf();
//
//		return ColourValue();
//	}

	//! Gets an attribute as 3d vector
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	Vector3 CAttributes::getAttributeAsVector3d(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getVector();
		else
			return Vector3();
	}

	//! Gets an attribute as 2d vector
	Vector2 CAttributes::getAttributeAsVector2d(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getVector2d();
		else
			return Vector2();
	}

	//! Gets an attribute as position2d
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
//	Position2d CAttributes::getAttributeAsPosition2d(SINT32 index)
//	{
	//	if ((UINT32)index < Attributes.size())
//			return Attributes[index]->getPosition();
//		else
	//		return Position2d();
//	}

	//! Gets an attribute as rectangle
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	rect<SINT32>  CAttributes::getAttributeAsRect(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getRect();
		else
			return rect<SINT32>();
	}

	//! Gets an attribute as dimension2d
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	dimension2d<UINT32>  CAttributes::getAttributeAsDimension2d(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getDimension2d();
		else
			return dimension2d<UINT32>();
	}


	//! Gets an attribute as binary data
	///! \param index: Index value, must be between 0 and getAttributeCount()-1.
	void CAttributes::getAttributeAsBinaryData(SINT32 index, void* outData, SINT32 maxSizeInBytes)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->getBinary(outData, maxSizeInBytes);
	}


	//! Gets an attribute as enumeration
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	const char* CAttributes::getAttributeAsEnumeration(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getEnum();
		else
			return 0;
	}


	//! Gets an attribute as enumeration
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	SINT32 CAttributes::getAttributeAsEnumeration(SINT32 index, const char* const* enumerationLiteralsToUse)
	{
		if ((UINT32)index < Attributes.size())
		{
			IAttribute* att = Attributes[index];

			if (enumerationLiteralsToUse && att)
			{
				const char* value = att->getEnum();
				if (value)
				{
					for (SINT32 i = 0; enumerationLiteralsToUse[i]; ++i)
						if (!strcmp(value, enumerationLiteralsToUse[i]))
							return i;
				}
			}
		}

		return -1;
	}

	//! Gets the list of enumeration literals of an enumeration attribute
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	void CAttributes::getAttributeEnumerationLiteralsOfEnumeration(SINT32 index, vector<String>::type& outLiterals)
	{
		if ((UINT32)index < Attributes.size() &&
			Attributes[index]->getType() == EAT_ENUM)
			outLiterals = ((CEnumAttribute*)Attributes[index])->EnumLiterals;
	}


	//! Adds an attribute as integer
	void CAttributes::addInt(const c8* attributeName, SINT32 value)
	{
		Attributes.push_back(new CIntAttribute(attributeName, value));
	}

	//! Adds an attribute as float
	void CAttributes::addFloat(const c8* attributeName, Real value)
	{
		Attributes.push_back(new CFloatAttribute(attributeName, value));
	}

	//! Adds an attribute as string
	void CAttributes::addString(const c8* attributeName, const char* value)
	{
		Attributes.push_back(new CStringAttribute(attributeName, value));
	}

	//! Adds an attribute as wchar string
	void CAttributes::addString(const c8* attributeName, const wchar_t* value)
	{
		Attributes.push_back(new CStringAttribute(attributeName, value));
	}

	//! Adds an attribute as bool
	void CAttributes::addBool(const c8* attributeName, bool value)
	{
		Attributes.push_back(new CBoolAttribute(attributeName, value));
	}

	//! Adds an attribute as enum
	void CAttributes::addEnum(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals)
	{
		Attributes.push_back(new CEnumAttribute(attributeName, enumValue, enumerationLiterals));
	}

	//! Adds an attribute as enum
	void CAttributes::addEnum(const c8* attributeName, SINT32 enumValue, const char* const* enumerationLiterals)
	{
		addEnum(attributeName, "", enumerationLiterals);
		//Attributes[Attributes.size()-1]->setInt(enumValue);
		vector<IAttribute*>::type::iterator it = (Attributes.end()-1);
		(*it)->setInt(enumValue);
		
	}

	//! Adds an attribute as color
	void CAttributes::addColor(const c8* attributeName, ColourValue value)
	{
		Attributes.push_back(new CColorAttribute(attributeName, value));
	}

	//! Adds an attribute as floating point color
	//void CAttributes::addColor(const c8* attributeName, ColourValue value)
	//{
	//	Attributes.push_back(new CColorfAttribute(attributeName, value));
	//}

	//! Adds an attribute as 3d vector
	void CAttributes::addVector3d(const c8* attributeName, Vector3 value)
	{
		Attributes.push_back(new CVector3DAttribute(attributeName, value));
	}

	//! Adds an attribute as 2d vector
	void CAttributes::addVector2d(const c8* attributeName, Vector2 value)
	{
		Attributes.push_back(new CVector2DAttribute(attributeName, value));
	}


	//! Adds an attribute as 2d position
	//void CAttributes::addPosition2d(const c8* attributeName, Position2d value)
	//{
	//	Attributes.push_back(new CPosition2DAttribute(attributeName, value));
	//}

	//! Adds an attribute as rectangle
	void CAttributes::addRect(const c8* attributeName, rect<SINT32> value)
	{
		Attributes.push_back(new CRectAttribute(attributeName, value));
	}

	//! Adds an attribute as dimension2d
	void CAttributes::addDimension2d(const c8* attributeName, dimension2d<UINT32> value)
	{
		Attributes.push_back(new CDimension2dAttribute(attributeName, value));
	}

	//! Adds an attribute as binary data
	void CAttributes::addBinary(const c8* attributeName, void* data, SINT32 dataSizeInBytes)
	{
		Attributes.push_back(new CBinaryAttribute(attributeName, data, dataSizeInBytes));
	}

	//! Adds an attribute as texture reference
	void CAttributes::addTexture(const c8* attributeName, ITexture* texture, const path& filename)
	{
		Attributes.push_back(new CTextureAttribute(attributeName, texture, Driver, filename));
	}

	//! Returns if an attribute with a name exists
	bool CAttributes::existsAttribute(const c8* attributeName)
	{
		return getAttributeP(attributeName) != 0;
	}

	//! Sets an attribute value as string.
	//! \param attributeName: Name for the attribute
	void CAttributes::setAttribute(SINT32 index, const c8* value)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setString(value);
	}

	//! Sets an attribute value as string.
	//! \param attributeName: Name for the attribute
	void CAttributes::setAttribute(SINT32 index, const wchar_t* value)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setString(value);
	}

	//! Sets an attribute as boolean value
	void CAttributes::setAttribute(SINT32 index, bool value)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setBool(value);
	}

	//! Sets an attribute as integer value
	void CAttributes::setAttribute(SINT32 index, SINT32 value)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setInt(value);
	}

	//! Sets a attribute as float value
	void CAttributes::setAttribute(SINT32 index, Real value)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setFloat(value);
	}

	//! Sets a attribute as color
	void CAttributes::setAttribute(SINT32 index, ColourValue color)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setColor(color);
	}

	//! Sets a attribute as floating point color
	//void CAttributes::setAttribute(SINT32 index, ColourValue color)
	//{
	//	if ((UINT32)index < Attributes.size())
	//		Attributes[index]->setColor(color);
	//}

	//! Sets a attribute as vector
	void CAttributes::setAttribute(SINT32 index, Vector3 v)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setVector(v);
	}

	//! Sets a attribute as vector
	void CAttributes::setAttribute(SINT32 index, Vector2 v)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setVector2d(v);
	}

	//! Sets a attribute as position
	//void CAttributes::setAttribute(SINT32 index, Position2d v)
	//{
	//	if ((UINT32)index < Attributes.size())
	//		Attributes[index]->setPosition(v);
	//}

	//! Sets a attribute as rectangle
	void CAttributes::setAttribute(SINT32 index, rect<SINT32> v)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setRect(v);
	}

	//! Sets a attribute as dimension2d
	void CAttributes::setAttribute(SINT32 index, dimension2d<UINT32> v)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setDimension2d(v);
	}

	//! Sets an attribute as binary data
	void CAttributes::setAttribute(SINT32 index, void* data, SINT32 dataSizeInBytes)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setBinary(data, dataSizeInBytes);
	}


	//! Sets an attribute as enumeration
	void CAttributes::setAttribute(SINT32 index, const char* enumValue, const char* const* enumerationLiterals)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setEnum(enumValue, enumerationLiterals);
	}


	//! Sets an attribute as texture reference
	void CAttributes::setAttribute(SINT32 index, ITexture* texture, const path& filename)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setTexture(texture, filename);
	}


	//! Adds an attribute as matrix
	void CAttributes::addMatrix(const c8* attributeName, const Matrix4& v)
	{
		Attributes.push_back(new CMatrixAttribute(attributeName, v));
	}


	//! Sets an attribute as matrix
	void CAttributes::setAttribute(const c8* attributeName, const Matrix4& v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setMatrix(v);
		else
			Attributes.push_back(new CMatrixAttribute(attributeName, v));
	}

	//! Gets an attribute as a Matrix4
	Matrix4 CAttributes::getAttributeAsMatrix(const c8* attributeName)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			return att->getMatrix();
		else
			return Matrix4();

	}

	//! Gets an attribute as matrix
	Matrix4 CAttributes::getAttributeAsMatrix(SINT32 index)
	{
		if ((UINT32)index < Attributes.size())
			return Attributes[index]->getMatrix();
		else
			return Matrix4();
	}

	//! Sets an attribute as matrix
	void CAttributes::setAttribute(SINT32 index, const Matrix4& v)
	{
		if ((UINT32)index < Attributes.size())
			Attributes[index]->setMatrix(v);
	}


	//! Adds an attribute as Quaternion
	void CAttributes::addQuaternion(const c8* attributeName, Quaternion v)
	{
		Attributes.push_back(new CQuaternionAttribute(attributeName, v));
	}


	//! Sets an attribute as Quaternion
	void CAttributes::setAttribute(const c8* attributeName, Quaternion v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setQuaternion(v);
		else
		{
			Attributes.push_back(new CQuaternionAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a Quaternion
	Quaternion CAttributes::getAttributeAsQuaternion(const c8* attributeName)
	{
		Quaternion ret(0, 1, 0, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getQuaternion();

		return ret;
	}

	//! Gets an attribute as Quaternion
	Quaternion CAttributes::getAttributeAsQuaternion(SINT32 index)
	{
		Quaternion ret(0, 1, 0, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getQuaternion();

		return ret;
	}

	//! Sets an attribute as Quaternion
	void CAttributes::setAttribute(SINT32 index, Quaternion v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setQuaternion(v);
	}

	//! Adds an attribute as axis aligned bounding box
	void CAttributes::addBox3d(const c8* attributeName, AxisAlignedBox v)
	{
		Attributes.push_back(new CBBoxAttribute(attributeName, v));
	}

	//! Sets an attribute as axis aligned bounding box
	void CAttributes::setAttribute(const c8* attributeName, AxisAlignedBox v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setBBox(v);
		else
		{
			Attributes.push_back(new CBBoxAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a axis aligned bounding box
	AxisAlignedBox CAttributes::getAttributeAsBox3d(const c8* attributeName)
	{
		AxisAlignedBox ret(0, 0, 0, 0, 0, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getBBox();

		return ret;
	}

	//! Gets an attribute as axis aligned bounding box
	AxisAlignedBox CAttributes::getAttributeAsBox3d(SINT32 index)
	{
		AxisAlignedBox ret(0, 0, 0, 0, 0, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getBBox();

		return ret;
	}

	//! Sets an attribute as axis aligned bounding box
	void CAttributes::setAttribute(SINT32 index, AxisAlignedBox v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setBBox(v);
	}

	//! Adds an attribute as 3d plane
	void CAttributes::addPlane3d(const c8* attributeName, Plane v)
	{
		Attributes.push_back(new CPlaneAttribute(attributeName, v));
	}

	//! Sets an attribute as 3d plane
	void CAttributes::setAttribute(const c8* attributeName, Plane v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setPlane(v);
		else
		{
			Attributes.push_back(new CPlaneAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a 3d plane
	Plane CAttributes::getAttributeAsPlane3d(const c8* attributeName)
	{
		Plane ret(0, 0, 0, 0, 1, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getPlane();

		return ret;
	}

	//! Gets an attribute as 3d plane
	Plane CAttributes::getAttributeAsPlane3d(SINT32 index)
	{
		Plane ret(0, 0, 0, 0, 1, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getPlane();

		return ret;
	}

	//! Sets an attribute as 3d plane
	void CAttributes::setAttribute(SINT32 index, Plane v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setPlane(v);
	}

	//! Adds an attribute as 3d triangle
	void CAttributes::addTriangle3d(const c8* attributeName, triangle3df v)
	{
		Attributes.push_back(new CTriangleAttribute(attributeName, v));
	}

	//! Sets an attribute as 3d triangle
	void CAttributes::setAttribute(const c8* attributeName, triangle3df v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setTriangle(v);
		else
		{
			Attributes.push_back(new CTriangleAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a 3d triangle
	triangle3df CAttributes::getAttributeAsTriangle3d(const c8* attributeName)
	{
		triangle3df ret;
		ret.pointA = ret.pointB = ret.pointC = Vector3(0, 0, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getTriangle();

		return ret;
	}

	//! Gets an attribute as 3d triangle
	triangle3df CAttributes::getAttributeAsTriangle3d(SINT32 index)
	{
		triangle3df ret;
		ret.pointA = ret.pointB = ret.pointC = Vector3(0, 0, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getTriangle();

		return ret;
	}

	//! Sets an attribute as 3d triangle
	void CAttributes::setAttribute(SINT32 index, triangle3df v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setTriangle(v);
	}

	//! Adds an attribute as a 2d line
	void CAttributes::addLine2d(const c8* attributeName, line2df v)
	{
		Attributes.push_back(new CLine2dAttribute(attributeName, v));
	}

	//! Sets an attribute as a 2d line
	void CAttributes::setAttribute(const c8* attributeName, line2df v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setLine2d(v);
		else
		{
			Attributes.push_back(new CLine2dAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a 2d line
	line2df CAttributes::getAttributeAsLine2d(const c8* attributeName)
	{
		line2df ret(0, 0, 0, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getLine2d();

		return ret;
	}

	//! Gets an attribute as a 2d line
	line2df CAttributes::getAttributeAsLine2d(SINT32 index)
	{
		line2df ret(0, 0, 0, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getLine2d();

		return ret;
	}

	//! Sets an attribute as a 2d line
	void CAttributes::setAttribute(SINT32 index, line2df v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setLine2d(v);
	}

	//! Adds an attribute as a 3d line
	void CAttributes::addLine3d(const c8* attributeName, line3df v)
	{
		Attributes.push_back(new CLine3dAttribute(attributeName, v));
	}

	//! Sets an attribute as a 3d line
	void CAttributes::setAttribute(const c8* attributeName, line3df v)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setLine3d(v);
		else
		{
			Attributes.push_back(new CLine3dAttribute(attributeName, v));
		}
	}

	//! Gets an attribute as a 3d line
	line3df CAttributes::getAttributeAsLine3d(const c8* attributeName)
	{
		line3df ret(0, 0, 0, 0, 0, 0);

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			ret = att->getLine3d();

		return ret;
	}

	//! Gets an attribute as a 3d line
	line3df CAttributes::getAttributeAsLine3d(SINT32 index)
	{
		line3df ret(0, 0, 0, 0, 0, 0);

		if (index >= 0 && index < (SINT32)Attributes.size())
			ret = Attributes[index]->getLine3d();

		return ret;
	}

	//! Sets an attribute as a 3d line
	void CAttributes::setAttribute(SINT32 index, line3df v)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setLine3d(v);

	}


	//! Adds an attribute as user pointner
	void CAttributes::addUserPointer(const c8* attributeName, void* userPointer)
	{
		Attributes.push_back(new CUserPointerAttribute(attributeName, userPointer));
	}

	//! Sets an attribute as user pointer
	void CAttributes::setAttribute(const c8* attributeName, void* userPointer)
	{
		IAttribute* att = getAttributeP(attributeName);
		if (att)
			att->setUserPointer(userPointer);
		else
		{
			Attributes.push_back(new CUserPointerAttribute(attributeName, userPointer));
		}
	}

	//! Gets an attribute as user pointer
	//! \param attributeName: Name of the attribute to get.
	void* CAttributes::getAttributeAsUserPointer(const c8* attributeName)
	{
		void* value = 0;

		IAttribute* att = getAttributeP(attributeName);
		if (att)
			value = att->getUserPointer();

		return value;
	}

	//! Gets an attribute as user pointer
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	void* CAttributes::getAttributeAsUserPointer(SINT32 index)
	{
		void* value = 0;

		if (index >= 0 && index < (SINT32)Attributes.size())
			value = Attributes[index]->getUserPointer();

		return value;
	}

	//! Sets an attribute as user pointer
	void CAttributes::setAttribute(SINT32 index, void* userPointer)
	{
		if (index >= 0 && index < (SINT32)Attributes.size())
			Attributes[index]->setUserPointer(userPointer);
	}


	//! Reads attributes from a xml file.
	//! \param readCurrentElementOnly: If set to true, reading only works if current element has the name 'attributes'.
	//! IF set to false, the first appearing list attributes are read.
	bool CAttributes::read(IXMLReader* reader, bool readCurrentElementOnly,
		const wchar_t* nonDefaultElementName)
	{
		if (!reader)
			return false;

		clear();

		StringW elementName = L"attributes";
		if (nonDefaultElementName)
			elementName = nonDefaultElementName;

		if (readCurrentElementOnly)
		{
			
			if (elementName != reader->getNodeName())
				return false;
		}

		while (reader->read())
		{
			switch (reader->getNodeType())
			{
			case EXN_ELEMENT:
				readAttributeFromXML(reader);
				break;
			case EXN_ELEMENT_END:
				if (elementName == reader->getNodeName())
					return true;
				break;
			default:
				break;
			}
		}

		return true;
	}


	void CAttributes::readAttributeFromXML(IXMLReader* reader)
	{
		StringW element = reader->getNodeName();
		String name = StringUtil::StringWToString(reader->getAttributeValue(L"name"));

		if (element == L"enum")
		{
			addEnum(name.c_str(), 0, 0);
			Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
		}
		else
			if (element == L"binary")
			{
				addBinary(name.c_str(), 0, 0);
				Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
			}
			else
				if (element == L"color")
				{
					addColor(name.c_str(), ColourValue());
					Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
				}
				// else
					//if (element == L"colorf")
					//{
					//	addColorf(name.c_str(), ColourValue());
					//	Attributes[Attributes.size()-1]->setString(reader->getAttributeValue(L"value"));
					//}
					else
						if (element == L"float")
						{
							addFloat(name.c_str(), 0);
							Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
						}
						else
							if (element == L"int")
							{
								addInt(name.c_str(), 0);
								Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
							}
							else
								if (element == L"bool")
								{
									addBool(name.c_str(), 0);
									Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
								}
								else
									if (element == L"string")
									{
										addString(name.c_str(), L"");
										Attributes[Attributes.size()-1]->setString(reader->getAttributeValue(L"value"));
									}
									else
										if (element == L"texture")
										{
											addTexture(name.c_str(), 0);
											Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
										}
										else
											if (element == L"vector3d")
											{
												addVector3d(name.c_str(), Vector3());
												Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
											}
											else
												if (element == L"vector2d")
												{
													addVector2d(name.c_str(), Vector2());
													Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
												}
											//	else
												//	if (element == L"position")
												//	{
												//		addPosition2d(name.c_str(), Position2d());
												//		Attributes[Attributes.size()-1]->setString(reader->getAttributeValue(L"value"));
												//	}
													else
														if (element == L"rect")
														{
															addRect(name.c_str(), rect<SINT32>());
															Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
														}
														else
															if (element == L"matrix")
															{
																addMatrix(name.c_str(), Matrix4());
																Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
															}
															else
																if (element == L"Quaternion")
																{
																	addQuaternion(name.c_str(), Quaternion());
																	Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																}
																else
																	if (element == L"box3d")
																	{
																		addBox3d(name.c_str(), AxisAlignedBox());
																		Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																	}
																	else
																		if (element == L"plane")
																		{
																			addPlane3d(name.c_str(), Plane());
																			Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																		}
																		else
																			if (element == L"triangle")
																			{
																				addTriangle3d(name.c_str(), triangle3df());
																				Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																			}
																			else
																				if (element == L"line2d")
																				{
																					addLine2d(name.c_str(), line2df());
																					Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																				}
																				else
																					if (element == L"line3d")
																					{
																						addLine3d(name.c_str(), line3df());
																						Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																					}
																					else
																						if (element == L"stringwarray")
																						{
																							vector<StringW>::type tmpArray;

																							SINT32 count = reader->getAttributeValueAsInt(L"count");
																							SINT32 n = 0;
																							const StringW tmpName(L"value");
																							for (; n<count; ++n)
																							{
																								
																								tmpArray.push_back(reader->getAttributeValue((tmpName + StringUtil::StringToStringW((StringUtil::int32ToString(n)))).c_str()));
																							}
																							addArray(name.c_str(), tmpArray);
																						}
																						else
																							if (element == L"userPointer")
																							{
																								// It's debatable if a pointer should be set or not, but it's more likely that adding it now would wreck user-applications.
																								// Also it probably doesn't makes sense setting this to a value when it comes from file.
																							}
																							else
																								if (element == L"dimension2d")
																								{
																									addDimension2d(name.c_str(), dimension2d<UINT32>());
																									Attributes[Attributes.size() - 1]->setString(reader->getAttributeValue(L"value"));
																								}
	}

	//! Write these attributes into a xml file
	bool CAttributes::write(IXMLWriter* writer, bool writeXMLHeader,
		const wchar_t* nonDefaultElementName)
	{
		if (!writer)
			return false;

		if (writeXMLHeader)
			writer->writeXMLHeader();

		StringW elementName = L"attributes";
		if (nonDefaultElementName)
			elementName = nonDefaultElementName;

		writer->writeElement(elementName.c_str(), false);
		writer->writeLineBreak();

		SINT32 i = 0;
		for (; i<(SINT32)Attributes.size(); ++i)
		{
			if (Attributes[i]->getType() == EAT_STRINGWARRAY)
			{
				vector<StringW>::type arraynames, arrayvalues;
				vector<StringW>::type arrayinput = Attributes[i]->getArray();

				// build arrays

				// name
				arraynames.push_back(StringW(L"name"));
				arrayvalues.push_back(StringW(StringUtil::StringToStringW(Attributes[i]->Name)));

				// count
				arraynames.push_back(StringW(L"count"));
				arrayvalues.push_back(StringW(StringUtil::StringToStringW(StringUtil::int32ToString(arrayinput.size()))));

				// array...
				UINT32 n = 0;
				const StringW tmpName(L"value");
				for (; n < arrayinput.size(); ++n)
				{
					arraynames.push_back((tmpName + StringW(StringUtil::StringToStringW(StringUtil::int32ToString(n)))));
					arrayvalues.push_back(arrayinput[n]);
				}

				// write them
				writer->writeElement(Attributes[i]->getTypeString(), true, arraynames, arrayvalues);
			}
			else
			{
				writer->writeElement(
					Attributes[i]->getTypeString(), true,
					L"name", StringW(StringUtil::StringToStringW(Attributes[i]->Name.c_str())).c_str(),
					L"value", Attributes[i]->getStringW().c_str());
			}

			writer->writeLineBreak();
		}

		writer->writeClosingTag(elementName.c_str());
		writer->writeLineBreak();

		return true;
	}
}