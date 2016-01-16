#ifndef _SAPPHIRE_C_ATTRIBUTES_
#define _SAPPHIRE_C_ATTRIBUTES_

#include "SapphirePrerequisites.h"
#include "SapphireString.h"  
#include "SapphireIAttributes.h"
#include "SapphireIAttribute.h"

namespace Sapphire
{

	class ITexture;
	class IVideoDriver;

	//! IAttributes�ӿڵ�ʵ��
	class CAttributes : public IAttributes
	{
	public:

		CAttributes(IVideoDriver* driver = 0);
		~CAttributes();

		//! �������Լ��������������
		virtual UINT32 getAttributeCount() const;

		//! ͨ����������������
		//! \param index: ����ֵ, ��0��getAttributeCount()-1֮��
		virtual const c8* getAttributeName(SINT32 index);

		//! �������Ե�һ������
		//! \param attributeName: ������
		virtual E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName);

		//! ������������������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual E_ATTRIBUTE_TYPE getAttributeType(SINT32 index);

		//! �����������͵��ַ���
		//! \param attributeName: �������͵�����
		virtual const StringW getAttributeTypeString(const c8* attributeName);

		//! ͨ���������������������͵��ַ���
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual const StringW getAttributeTypeString(SINT32 index);

		//! һ�������Ƿ����
		virtual bool existsAttribute(const c8* attributeName);

		//! ��������������������������-1û�ҵ�
		virtual SINT32 findAttribute(const c8* attributeName) const;

		//! �����������
		virtual void clear();

		//! ��һ��XML�ļ��ж�ȡ����
		//! \param reader Ҫ��ȡ��XML��ȡ��
		//! \param readCurrentElementOnly �������Ϊtrue��ֻ��ȡ��ǰԪ��������'attributes'��ָ����Ԫ����
		//! \param elementName ��Χ��Ԫ��. �������Ϊ�գ�Ĭ����'attributes' 
		//! �������Ϊflase����һ�����ֵ������б�ᱻ��ȡ
		virtual bool read(IXMLReader* reader, bool readCurrentElementOnly = false,
			const wchar_t* nonDefaultElementName = 0);

		//! д����Щ���Ե�һ��XML�ļ�
		//! \param writer: Ҫд���XMLд����
		//! \param writeXMLHeader: д��һ��Header��XML�ļ����������ļ���ʼ��Ҫ�Ļ�
		//! \param elementName:��Χ��Ԫ��. �������Ϊ�գ�Ĭ����'attributes' 
		virtual bool write(IXMLWriter* writer, bool writeXMLHeader = false, const wchar_t* nonDefaultElementName = 0);


		/*

		 ��������

		*/

		//! ���һ�����ε�����
		virtual void addInt(const c8* attributeName, SINT32 value);

		//! ����һ�����ε�����
		virtual void setAttribute(const c8* attributeName, SINT32 value);

		//! ��ȡһ����������
		//! \param attributeName: Ҫ��ȡ��������
		//! \return ����֮ǰ���õ�����ֵ
		virtual SINT32 getAttributeAsInt(const c8* attributeName) const;

		//!��ȡһ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual SINT32 getAttributeAsInt(SINT32 index) const;

		//! ����һ����������
		virtual void setAttribute(SINT32 index, SINT32 value);

		/*

		��������

		*/

		//! ���һ������ֵ����
		virtual void addFloat(const c8* attributeName, Real value);

		//! ����һ������ֵ����
		virtual void setAttribute(const c8* attributeName, Real value);

		//! ��ȡһ���������Ե�ֵ
		//! \param attributeName: Ҫ��ȡ��������
		//! \return ����֮ǰ���õ�����ֵ
		virtual Real getAttributeAsFloat(const c8* attributeName);

		//!��ȡһ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Real getAttributeAsFloat(SINT32 index);

		//! ����һ����������
		virtual void setAttribute(SINT32 index, Real value);

		/*

	�ַ�������

		*/

		//! ���һ���ַ�������
		virtual void addString(const c8* attributeName, const c8* value);

		//! ����һ���ַ�������
		//! \param attributeName: Ҫ��ȡ��������
		//! \param value: ���Ե�ֵ������Ϊ0��ɾ���������
		virtual void setAttribute(const c8* attributeName, const c8* value);

		//! ��ȡ����ַ�������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		virtual String getAttributeAsString(const c8* attributeName);

		//! ��ȡһ���ַ���������
		//! \param attributeName Ҫ��ȡ��������
		//! \param target Ҫ��ŵ��ַ�������
		virtual void getAttributeAsString(const c8* attributeName, c8* target);

		//! ����ָ���������ַ�������ֵ
		//! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual String getAttributeAsString(SINT32 index);

		//! ����һ��ָ���������ַ�����ֵ
		//! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		//! \param value Ҫ���õ��ַ�����ֵ
		virtual void setAttribute(SINT32 index, const c8* value);

		// ���ֽ��ַ����汾

		//!  ���һ���ַ�������
		virtual void addString(const c8* attributeName, const wchar_t* value);

		//! ����һ���ַ�������
		//! \param attributeName: Ҫ��ȡ��������
		//! \param value: ���Ե�ֵ������Ϊ0��ɾ���������
		virtual void setAttribute(const c8* attributeName, const wchar_t* value);

		//! ��ȡ����ַ�������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		virtual StringW getAttributeAsStringW(const c8* attributeName);

		//! ��ȡһ���ַ���������
		//! \param attributeName Ҫ��ȡ��������
		//! \param target Ҫ��ŵ��ַ�������
		virtual void getAttributeAsStringW(const c8* attributeName, wchar_t* target);

		//! ����ָ���������ַ�������ֵ
		//! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual StringW getAttributeAsStringW(SINT32 index);

		//! ����һ��ָ���������ַ�����ֵ
		//! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		//! \param value Ҫ���õ��ַ�����ֵ
		virtual void setAttribute(SINT32 index, const wchar_t* value);

		/*

		 ��������������

		*/

		//! ���һ�����������ݵ�����
		virtual void addBinary(const c8* attributeName, void* data, SINT32 dataSizeInBytes);

		//! ����һ�����������ݵ�����
		virtual void setAttribute(const c8* attributeName, void* data, SINT32 dataSizeInBytes);

		//! ��ȡһ�����������ݵ�����
		/** \param attributeName: ������
		\param outData ָ��Ҫ��Ŷ��������ݵĻ�����
		\param maxSizeInBytes ������������ֽ���
		*/
		virtual void getAttributeAsBinaryData(const c8* attributeName, void* outData, SINT32 maxSizeInBytes);

		//! ��ȡһ�����������ݵ�����
		/** \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		\param outData ָ��Ҫ��Ŷ��������ݵĻ�����
		\param maxSizeInBytes ������������ֽ���
		*/
		virtual void getAttributeAsBinaryData(SINT32 index, void* outData, SINT32 maxSizeInBytes);

		//! ����һ����������������
		virtual void setAttribute(SINT32 index, void* data, SINT32 dataSizeInBytes);


		/*
		��������
		*/

		//! ���һ�����ַ������������
		virtual void addArray(const c8* attributeName, const vector<StringW>::type& value);

		//! ����һ�����ַ������������
		//! \param attributeName: ������
		//! \param value: ���Ե�ֵ�� ����Ϊ0��ɾ���������
		virtual void setAttribute(const c8* attributeName, const vector<StringW>::type& value);

		//! ��ȡһ�����ַ�������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		virtual vector<StringW>::type getAttributeAsArray(const c8* attributeName);

		//! ͨ����������һ�����ַ�������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual vector<StringW>::type getAttributeAsArray(SINT32 index);

		//! ͨ����������һ�����ַ���������
		virtual void setAttribute(SINT32 index, const vector<StringW>::type& value);

		/*

		��������

		*/

		//! ���һ����������
		virtual void addBool(const c8* attributeName, bool value);

		//! ����һ������������
		virtual void setAttribute(const c8* attributeName, bool value);

		//! ��ȡһ����������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		virtual bool getAttributeAsBool(const c8* attributeName);

		//! ͨ����������һ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual bool getAttributeAsBool(SINT32 index);

		//! ͨ����������һ������������
		virtual void setAttribute(SINT32 index, bool value);

		/*

		ö������

		*/

		//! ���һ��ö�ٵ�����
		virtual void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);

		//! ���һ��ö������
		virtual void addEnum(const c8* attributeName, SINT32 enumValue, const c8* const* enumerationLiterals);

		//! ����һ��ö������
		virtual void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);

		//! ��ȡһ��ö������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		virtual const c8* getAttributeAsEnumeration(const c8* attributeName);

		//! ��ȡһ��ö������ 
		/** \param attributeName: ������
		\param enumerationLiteralsToUse: ������Щö���ı�����ȡ��������������Կ�����XML�ļ���ȡʱ����
		������ֻ�������ö���ַ���������û������������Ϣ
		\return  ����֮ǰ���õ�����ֵ
		*/
		virtual SINT32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse);

		//!  ��ȡһ��ö������ 
		/** \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		\param enumerationLiteralsToUse: ������Щö���ı�����ȡ��������������Կ�����XML�ļ���ȡʱ����
		������ֻ�������ö���ַ���������û������������Ϣ
		\return ����֮ǰ���õ�����ֵ
		*/
		virtual SINT32 getAttributeAsEnumeration(SINT32 index, const c8* const* enumerationLiteralsToUse);

		//! ��ȡһ��ö������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual const c8* getAttributeAsEnumeration(SINT32 index);

		//! ��ȡһ��ö�����Ե�ö���ı��б�
		//! \param attributeName ������
		//! \param outLiterals ö�������ַ�������
		virtual void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, vector<String>::type& outLiterals);

		//! ��ȡһ��ö�����Ե�ö���ı��б�
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		//! \param outLiterals ö�������ַ�������
		virtual void getAttributeEnumerationLiteralsOfEnumeration(SINT32 index, vector<String>::type& outLiterals);

		//! ����һ��ö������
		virtual void setAttribute(SINT32 index, const c8* enumValue, const c8* const* enumerationLiterals);


		/*

		ColourValue����

		*/

		//! ���һ����ɫ����
		virtual void addColor(const c8* attributeName, ColourValue value);

		//! ����һ����ɫ���� 
		virtual void setAttribute(const c8* attributeName, ColourValue color);

		//! ��ȡһ����ɫ����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����ֵ
		virtual ColourValue getAttributeAsColor(const c8* attributeName);

		//! ��ȡһ����ɫ����
		//! \param index:  ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual ColourValue getAttributeAsColor(SINT32 index);

		//! ����һ����ɫ����
		virtual void setAttribute(SINT32 index, ColourValue color);

		/*

		ColourValuef Attribute

		

		//! Adds an attribute as floating point color
		virtual void addColorf(const c8* attributeName, ColourValue value);

		//! Sets a attribute as floating point color
		virtual void setAttribute(const c8* attributeName, ColourValue color);

		//! Gets an attribute as floating point color
		//! \param attributeName: Name of the attribute to get.
		//! \return Returns value of the attribute previously set by setAttribute()
		virtual ColourValue getAttributeAsColorf(const c8* attributeName);

		//! Gets an attribute as floating point color
		//! \param index: Index value, must be between 0 and getAttributeCount()-1.
		virtual ColourValue getAttributeAsColorf(SINT32 index);

		//! Sets an attribute as floating point color
		virtual void setAttribute(SINT32 index, ColourValue color);
		*/

		/*

		Vector3 ����

		*/

		//!  ���һ��Vector3����
		virtual void addVector3d(const c8* attributeName, Vector3 value);

		//! ����һ��Vector3����
		virtual void setAttribute(const c8* attributeName, Vector3 v);

		//! ��ȡһ��Vector3����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual Vector3 getAttributeAsVector3d(const c8* attributeName);

		//! ��ȡһ��Vector3����
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Vector3 getAttributeAsVector3d(SINT32 index);

		//! ����һ��Vector2����
		virtual void setAttribute(SINT32 index, Vector3 v);


		/*

		Vector2d Attribute

		*/

		//!  ���һ��Vector2����
		virtual void addVector2d(const c8* attributeName, Vector2 value);

		//! ����һ��Vector2����
		virtual void setAttribute(const c8* attributeName, Vector2 v);

		//! ��ȡһ��Vector2����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual Vector2 getAttributeAsVector2d(const c8* attributeName);

		//! ��ȡһ��Vector2����
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Vector2 getAttributeAsVector2d(SINT32 index);

		//! ����һ��Vector2����
		virtual void setAttribute(SINT32 index, Vector2 v);


		

		//Position2d Attribute

		

		//! Adds an attribute as 2d position
	//	virtual void addPosition2d(const c8* attributeName, Position2d value);

		//! Sets a attribute as 2d position
		//virtual void setAttribute(const c8* attributeName, Position2d v);

		//! Gets an attribute as position
		//! \param attributeName: Name of the attribute to get.
		//! \return Returns value of the attribute previously set by setAttribute()
	//	virtual Position2d getAttributeAsPosition2d(const c8* attributeName);

		//! Gets an attribute as position
		//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	//	virtual Position2d getAttributeAsPosition2d(SINT32 index);

		//! Sets an attribute as 2d position
	//	virtual void setAttribute(SINT32 index, Position2d v);
		


		/*

		 ��������

		*/

		//! ���һ����������
		virtual void addRect(const c8* attributeName, rect<SINT32> value);

		//! ����һ����������
		virtual void setAttribute(const c8* attributeName, rect<SINT32> v);

		//! ��ȡһ����������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual rect<SINT32> getAttributeAsRect(const c8* attributeName);

		//! ��ȡһ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual rect<SINT32> getAttributeAsRect(SINT32 index);

		//! ����һ����������
		virtual void setAttribute(SINT32 index, rect<SINT32> v);


		/*

		Dimension2d ����

		*/

		//! ���һ��dimension2d����
		virtual void addDimension2d(const c8* attributeName, dimension2d<UINT32> value);

		//! ����һ��dimension2d����
		virtual void setAttribute(const c8* attributeName, dimension2d<UINT32> v);

		//! ��ȡһ��dimension2d����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual dimension2d<UINT32> getAttributeAsDimension2d(const c8* attributeName);

		//! ��ȡһ��dimension2d����
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual dimension2d<UINT32> getAttributeAsDimension2d(SINT32 index);

		//! ����һ��dimension2d����
		virtual void setAttribute(SINT32 index, dimension2d<UINT32> v);


		/*
		��������
		*/

		//! ���һ����������
		virtual void addMatrix(const c8* attributeName, const Matrix4& v);

		//! ����һ����������
		virtual void setAttribute(const c8* attributeName, const Matrix4& v);

		//! ��ȡһ����������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual Matrix4 getAttributeAsMatrix(const c8* attributeName);

		//! ��ȡһ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Matrix4 getAttributeAsMatrix(SINT32 index);

		//! ����һ����������
		virtual void setAttribute(SINT32 index, const Matrix4& v);

		/*
		  ��Ԫ������
		*/

		//! ���һ����Ԫ������
		virtual void addQuaternion(const c8* attributeName, Quaternion v);

		//! ����һ����Ԫ������
		virtual void setAttribute(const c8* attributeName, Quaternion v);

		//! ��ȡһ����Ԫ������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual Quaternion getAttributeAsQuaternion(const c8* attributeName);

		//! ��ȡһ����Ԫ������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Quaternion getAttributeAsQuaternion(SINT32 index);

		//! ����һ����Ԫ������
		virtual void setAttribute(SINT32 index, Quaternion v);

		/*

		3d AABB��ײ����

		*/

		//! ���һ��AABB����
		virtual void addBox3d(const c8* attributeName, AxisAlignedBox v);

		//! ����һ��AABB����
		virtual void setAttribute(const c8* attributeName, AxisAlignedBox v);

		//! ��ȡһ��AABB����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual AxisAlignedBox getAttributeAsBox3d(const c8* attributeName);

		//! ��ȡһ��AABB����
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual AxisAlignedBox getAttributeAsBox3d(SINT32 index);

		//! ����һ��AABB����
		virtual void setAttribute(SINT32 index, AxisAlignedBox v);

		/*

		ƽ������

		*/

		//! ���һ��ƽ������
		virtual void addPlane3d(const c8* attributeName, Plane v);

		//! ����һ��ƽ������
		virtual void setAttribute(const c8* attributeName, Plane v);

		//! ��ȡһ��ƽ������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual Plane getAttributeAsPlane3d(const c8* attributeName);

		//! ��ȡһ��ƽ������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual Plane getAttributeAsPlane3d(SINT32 index);

		//! ����һ��ƽ������
		virtual void setAttribute(SINT32 index, Plane v);


		/*

		������

		*/

		//! ���һ�� ����������
		virtual void addTriangle3d(const c8* attributeName, triangle3df v);

		//! ����һ�� ����������
		virtual void setAttribute(const c8* attributeName, triangle3df v);

		//! ��ȡһ�� ����������
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual triangle3df getAttributeAsTriangle3d(const c8* attributeName);

		//! ��ȡһ�� ����������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual triangle3df getAttributeAsTriangle3d(SINT32 index);

		//! ����һ�� ����������
		virtual void setAttribute(SINT32 index, triangle3df v);



		/*

		2d�߶�

		*/

		//! ���һ�� 2d�߶�����
		virtual void addLine2d(const c8* attributeName, line2df v);

		//! ����һ�� 2d�߶�����
		virtual void setAttribute(const c8* attributeName, line2df v);

		//! ��ȡһ�� 2d�߶�����
		//! \param attributeName: ������
		//! \return ����֮ǰ���õ�����
		virtual line2df getAttributeAsLine2d(const c8* attributeName);

		//! ��ȡһ�� 2d�߶�����
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual line2df getAttributeAsLine2d(SINT32 index);

		//! ����һ�� 2d�߶�����
		virtual void setAttribute(SINT32 index, line2df v);


		/*

		3d �߶�

		*/

		//! ���һ�� 3d�߶�����
		virtual void addLine3d(const c8* attributeName, line3df v);

		//! ����һ���û�ָ�������
		virtual void setAttribute(const c8* attributeName, line3df v);

		//! ��ȡһ���û�ָ�������
		//! \param attributeName: ������
		virtual line3df getAttributeAsLine3d(const c8* attributeName);

		//! ��ȡһ��  �û�ָ������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual line3df getAttributeAsLine3d(SINT32 index);

		//! ����һ�� �û�ָ������
		virtual void setAttribute(SINT32 index, line3df v);


		/*

		��������
		*/

		//! ���һ���������õ�����
		virtual void addTexture(const c8* attributeName, ITexture* texture, const path& filename = "");

		//! ����һ���������õ�����
		virtual void setAttribute(const c8* attributeName, ITexture* texture, const path& filename = "");

		//! ��ȡһ���������õ�����
		//! \param attributeName: ������
		virtual ITexture* getAttributeAsTexture(const c8* attributeName);

		//! ��ȡһ��  ������������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual ITexture* getAttributeAsTexture(SINT32 index);

		//! ����һ�� ������������
		virtual void setAttribute(SINT32 index, ITexture* texture, const path& filename = "");



		/*

		�û�ָ������

		*/

		//! ���һ���û�ָ�������
		virtual void addUserPointer(const c8* attributeName, void* userPointer);

		//! ����һ���û�ָ�������
		virtual void setAttribute(const c8* attributeName, void* userPointer);

		//! ��ȡһ���û�ָ�������
		//! \param attributeName: ������
		virtual void* getAttributeAsUserPointer(const c8* attributeName);

		//! ��ȡһ��  �û�ָ������
		//! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		virtual void* getAttributeAsUserPointer(SINT32 index);

		//! ����һ�� �û�ָ������
		virtual void setAttribute(SINT32 index, void* userPointer);

	protected:

		void readAttributeFromXML(IXMLReader* reader);

		vector<IAttribute*>::type Attributes;

		IAttribute* getAttributeP(const c8* attributeName) const;

		IVideoDriver* Driver;
	};

}

#endif
