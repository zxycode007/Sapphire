#ifndef _SAPPHIRE_IATTRIBUTES_
#define _SAPPHIRE_IATTRIBUTES_

#include "SapphireEAttributes.h"
#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireColorValue.h"
#include "SapphireVector3.h"
#include "SapphireVector2.h"
#include "SapphireRectangle.h"
#include "SapphireMatrix4.h"
#include "SapphireQuaternion.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphirePlane.h"
#include "SapphireTriangle3D.h"
#include "SapphireLine2D.h"
#include "SapphireLine3D.h"
#include "SapphireITexture.h"
#include "SapphireIXMLReader.h"

namespace Sapphire
{
	 class ITexture;
	 class IXMLWriter;
	 //class IXMLReader;

	 //! �ṩ���Ժ����ǵ�ֵ���ҿ������л����ǵ�ͨ�ýӿ�
	 class IAttributes : public virtual IReferenceCounter
	 {
	 public:

		 //! �������Լ��ϵ���������
		 virtual UINT32 getAttributeCount() const = 0;

		 //! ����������������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual const c8* getAttributeName(SINT32 index) = 0;

		 //! �������Ե�����
		 //! \param attributeName: ������
		 virtual E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName) = 0;

		 //! ������������������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual E_ATTRIBUTE_TYPE getAttributeType(SINT32 index) = 0;

		 //! �����������͵��ַ���
		 //! \param attributeName: �������͵�����
		 virtual const StringW getAttributeTypeString(const c8* attributeName) = 0;

		 //! ͨ���������������������͵��ַ���
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual const StringW getAttributeTypeString(SINT32 index) = 0;

		 //! һ�������Ƿ����
		 virtual bool existsAttribute(const c8* attributeName) = 0;

		 //! ��������������������������-1û�ҵ�
		 virtual SINT32 findAttribute(const c8* attributeName) const = 0;

		 //! �����������
		 virtual void clear() = 0;

		 //! ��һ��XML�ļ��ж�ȡ����
		 //! \param reader Ҫ��ȡ��XML��ȡ��
		 //! \param readCurrentElementOnly �������Ϊtrue��ֻ��ȡ��ǰԪ��������'attributes'��ָ����Ԫ����
		 //! \param elementName ��Χ��Ԫ��. �������Ϊ�գ�Ĭ����'attributes' 
		 //! �������Ϊflase����һ�����ֵ������б�ᱻ��ȡ
		 virtual bool read(IXMLReader* reader, bool readCurrentElementOnly = false, const wchar_t* elementName = 0) = 0;

		 //! д����Щ���Ե�һ��XML�ļ�
		 //! \param writer: Ҫд���XMLд����
		 //! \param writeXMLHeader: д��һ��Header��XML�ļ����������ļ���ʼ��Ҫ�Ļ�
		 //! \param elementName:��Χ��Ԫ��. �������Ϊ�գ�Ĭ����'attributes' 
		 virtual bool write(IXMLWriter* writer, bool writeXMLHeader = false, const wchar_t* elementName = 0) = 0;


		 /*

		 ��������

		 */

		 //! ���һ�����ε�����
		 virtual void addInt(const c8* attributeName, SINT32 value) = 0;

		 //! ����һ�����ε�����
		 virtual void setAttribute(const c8* attributeName, SINT32 value) = 0;

		 //! ��ȡһ����������
		 //! \param attributeName: Ҫ��ȡ��������
		 //! \return ����֮ǰ���õ�����ֵ
		 virtual SINT32 getAttributeAsInt(const c8* attributeName) const = 0;

		 //!��ȡһ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual SINT32 getAttributeAsInt(SINT32 index) const = 0;

		 //! ����һ����������
		 virtual void setAttribute(SINT32 index, SINT32 value) = 0;

		 /*

		 ��������

		 */

		 //! ���һ������ֵ����
		 virtual void addFloat(const c8* attributeName, Real value) = 0;

		 //! ����һ������ֵ����
		 virtual void setAttribute(const c8* attributeName, Real value) = 0;

		 //! ��ȡһ���������Ե�ֵ
		 //! \param attributeName: Ҫ��ȡ��������
		 //! \return ����֮ǰ���õ�����ֵ
		 virtual Real getAttributeAsFloat(const c8* attributeName) = 0;

		 //!��ȡһ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Real getAttributeAsFloat(SINT32 index) = 0;

		 //! ����һ����������
		 virtual void setAttribute(SINT32 index, Real value) = 0;

		 /*

		�ַ�������

		 */

		 //! ���һ���ַ�������
		 virtual void addString(const c8* attributeName, const c8* value) = 0;

		 //! ����һ���ַ�������
		 //! \param attributeName: Ҫ��ȡ��������
		 //! \param value: ���Ե�ֵ������Ϊ0��ɾ���������
		 virtual void setAttribute(const c8* attributeName, const c8* value) = 0;

		 //! ��ȡ����ַ�������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		 virtual String getAttributeAsString(const c8* attributeName) = 0;

		 //! ��ȡһ���ַ���������
		 //! \param attributeName Ҫ��ȡ��������
		 //! \param target Ҫ��ŵ��ַ�������
		 virtual void getAttributeAsString(const c8* attributeName, c8* target) = 0;

		 //! ����ָ���������ַ�������ֵ
		 //! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual String getAttributeAsString(SINT32 index) = 0;

		 //! ����һ��ָ���������ַ�����ֵ
		 //! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 //! \param value Ҫ���õ��ַ�����ֵ
		 virtual void setAttribute(SINT32 index, const c8* value) = 0;

		 // ���ֽ��ַ����汾

		 //!  ���һ���ַ�������
		 virtual void addString(const c8* attributeName, const wchar_t* value) = 0;

		 //! ����һ���ַ�������
		 //! \param attributeName: Ҫ��ȡ��������
		 //! \param value: ���Ե�ֵ������Ϊ0��ɾ���������
		 virtual void setAttribute(const c8* attributeName, const wchar_t* value) = 0;

		 //! ��ȡ����ַ�������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		 virtual StringW getAttributeAsStringW(const c8* attributeName) = 0;

		 //! ��ȡһ���ַ���������
		 //! \param attributeName Ҫ��ȡ��������
		 //! \param target Ҫ��ŵ��ַ�������
		 virtual void getAttributeAsStringW(const c8* attributeName, wchar_t* target) = 0;

		 //! ����ָ���������ַ�������ֵ
		 //! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual StringW getAttributeAsStringW(SINT32 index) = 0;

		 //! ����һ��ָ���������ַ�����ֵ
		 //! \param index ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 //! \param value Ҫ���õ��ַ�����ֵ
		 virtual void setAttribute(SINT32 index, const wchar_t* value) = 0;

		 /*

		 ��������������

		 */

		 //! ���һ�����������ݵ�����
		 virtual void addBinary(const c8* attributeName, void* data, SINT32 dataSizeInBytes) = 0;

		 //! ����һ�����������ݵ�����
		 virtual void setAttribute(const c8* attributeName, void* data, SINT32 dataSizeInBytes) = 0;

		 //! ��ȡһ�����������ݵ�����
		 /** \param attributeName: ������
		 \param outData ָ��Ҫ��Ŷ��������ݵĻ�����
		 \param maxSizeInBytes ������������ֽ���
		 */
		 virtual void getAttributeAsBinaryData(const c8* attributeName, void* outData, SINT32 maxSizeInBytes) = 0;

		 //! ��ȡһ�����������ݵ�����
		 /** \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 \param outData ָ��Ҫ��Ŷ��������ݵĻ�����
		 \param maxSizeInBytes ������������ֽ���
		 */
		 virtual void getAttributeAsBinaryData(SINT32 index, void* outData, SINT32 maxSizeInBytes) = 0;

		 //! ����һ����������������
		 virtual void setAttribute(SINT32 index, void* data, SINT32 dataSizeInBytes) = 0;


		 /*
		��������
		 */

		 //! ���һ�����ַ������������
		 virtual void addArray(const c8* attributeName, const vector<StringW>::type& value) = 0;

		 //! ����һ�����ַ������������
		 //! \param attributeName: ������
		 //! \param value: ���Ե�ֵ�� ����Ϊ0��ɾ���������
		 virtual void setAttribute(const c8* attributeName, const vector<StringW>::type& value) = 0;

		 //! ��ȡһ�����ַ�������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		 virtual vector<StringW>::type getAttributeAsArray(const c8* attributeName) = 0;

		 //! ͨ����������һ�����ַ�������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual vector<StringW>::type getAttributeAsArray(SINT32 index) = 0;

		 //! ͨ����������һ�����ַ���������
		 virtual void setAttribute(SINT32 index, const vector<StringW>::type& value) = 0;


		 /*

		 ��������

		 */

		 //! ���һ����������
		 virtual void addBool(const c8* attributeName, bool value) = 0;

		 //! ����һ������������
		 virtual void setAttribute(const c8* attributeName, bool value) = 0;

		 //! ��ȡһ����������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		 virtual bool getAttributeAsBool(const c8* attributeName) = 0;

		 //! ͨ����������һ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual bool getAttributeAsBool(SINT32 index) = 0;

		 //! ͨ����������һ������������
		 virtual void setAttribute(SINT32 index, bool value) = 0;

		 /*

		 ö������

		 */

		 //! ���һ��ö�ٵ�����
		 virtual void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;

		 //! ���һ��ö������
		 virtual void addEnum(const c8* attributeName, SINT32 enumValue, const c8* const* enumerationLiterals) = 0;

		 //! ����һ��ö������
		 virtual void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals) = 0;

		 //! ��ȡһ��ö������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ�����û���÷���0
		 virtual const c8* getAttributeAsEnumeration(const c8* attributeName) = 0;

		 //! ��ȡһ��ö������ 
		 /** \param attributeName: ������
		 \param enumerationLiteralsToUse: ������Щö���ı�����ȡ��������������Կ�����XML�ļ���ȡʱ����
		 ������ֻ�������ö���ַ���������û������������Ϣ
		 \return  ����֮ǰ���õ�����ֵ
		 */
		 virtual SINT32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse) = 0;

		 //!  ��ȡһ��ö������ 
		 /** \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 \param enumerationLiteralsToUse: ������Щö���ı�����ȡ��������������Կ�����XML�ļ���ȡʱ����
		 ������ֻ�������ö���ַ���������û������������Ϣ
		 \return ����֮ǰ���õ�����ֵ
		 */
		 virtual SINT32 getAttributeAsEnumeration(SINT32 index, const c8* const* enumerationLiteralsToUse) = 0;

		 //! ��ȡһ��ö������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual const c8* getAttributeAsEnumeration(SINT32 index) = 0;

		 //! ��ȡһ��ö�����Ե�ö���ı��б�
		 //! \param attributeName ������
		 //! \param outLiterals ö�������ַ�������
		 virtual void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName,  vector<String>::type& outLiterals) = 0;

		 //! ��ȡһ��ö�����Ե�ö���ı��б�
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 //! \param outLiterals ö�������ַ�������
		 virtual void getAttributeEnumerationLiteralsOfEnumeration(SINT32 index, vector<String>::type& outLiterals) = 0;

		 //! ����һ��ö������
		 virtual void setAttribute(SINT32 index, const c8* enumValue, const c8* const* enumerationLiterals) = 0;


		 /*

		 ColourValue����

		 */

		 //! ���һ����ɫ����
		 virtual void addColor(const c8* attributeName,  ColourValue value) = 0;


		 //! ����һ����ɫ���� 
		 virtual void setAttribute(const c8* attributeName, ColourValue color) = 0;

		 //! ��ȡһ����ɫ����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����ֵ
		 virtual ColourValue getAttributeAsColor(const c8* attributeName) = 0;

		 //! ��ȡһ����ɫ����
		 //! \param index:  ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual ColourValue getAttributeAsColor(SINT32 index) = 0;

		 //! ����һ����ɫ����
		 virtual void setAttribute(SINT32 index, ColourValue color) = 0;

		 /*

		 SColorf Attribute

		 

		 //! Adds an attribute as floating point color
		 virtual void addColorf(const c8* attributeName, video::SColorf value) = 0;

		 //! Sets a attribute as floating point color
		 virtual void setAttribute(const c8* attributeName, video::SColorf color) = 0;

		 //! Gets an attribute as floating point color
		 //! \param attributeName: Name of the attribute to get.
		 //! \return Returns value of the attribute previously set by setAttribute()
		 virtual video::SColorf getAttributeAsColorf(const c8* attributeName) = 0;

		 //! Gets an attribute as floating point color
		 //! \param index: Index value, must be between 0 and getAttributeCount()-1.
		 virtual video::SColorf getAttributeAsColorf(SINT32 index) = 0;

		 //! Sets an attribute as floating point color
		 virtual void setAttribute(SINT32 index, video::SColorf color) = 0;
		 */

		 /*

		 Vector3 ����

		 */

		 //!  ���һ��Vector3����
		 virtual void addVector3d(const c8* attributeName, Vector3 value) = 0;

		 //! ����һ��Vector3����
		 virtual void setAttribute(const c8* attributeName, Vector3 v) = 0;

		 //! ��ȡһ��Vector3����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual Vector3 getAttributeAsVector3d(const c8* attributeName) = 0;

		 //! ��ȡһ��Vector3����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Vector3 getAttributeAsVector3d(SINT32 index) = 0;

		 //! ����һ��Vector2����
		 virtual void setAttribute(SINT32 index, Vector3 v) = 0;

		 /*

		 Vector2 Attribute

		 */

		 //!  ���һ��Vector2����
		 virtual void addVector2d(const c8* attributeName,Vector2 value) = 0;

		 //! ����һ��Vector2����
		 virtual void setAttribute(const c8* attributeName, Vector2 v) = 0;

		 //! ��ȡһ��Vector2����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual Vector2 getAttributeAsVector2d(const c8* attributeName) = 0;

		 //! ��ȡһ��Vector2����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Vector2 getAttributeAsVector2d(SINT32 index) = 0;

		 //! ����һ��Vector2����
		 virtual void setAttribute(SINT32 index, Vector2 v) = 0;

		 /*

		 2dPosition2d Attribute

		

		 //! Adds an attribute as 2d position
		 virtual void addPosition2d(const c8* attributeName, core::position2di value) = 0;

		 //! Sets a attribute as 2d position
		 virtual void setAttribute(const c8* attributeName, core::position2di v) = 0;

		 //! Gets an attribute as position
		 //! \param attributeName: Name of the attribute to get.
		 //! \return Returns value of the attribute previously set by setAttribute()
		 virtual core::position2di getAttributeAsPosition2d(const c8* attributeName) = 0;

		 //! Gets an attribute as position
		 //! \param index: Index value, must be between 0 and getAttributeCount()-1.
		 virtual core::position2di getAttributeAsPosition2d(SINT32 index) = 0;

		 //! Sets an attribute as 2d position
		 virtual void setAttribute(SINT32 index, core::position2di v) = 0;
		 */

		 /*

		 ��������

		 */

		 //! ���һ����������
		 virtual void addRect(const c8* attributeName,  rect<SINT32> value) = 0;

		 //! ����һ����������
		 virtual void setAttribute(const c8* attributeName, rect<SINT32> v) = 0;

		 //! ��ȡһ����������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual rect<SINT32> getAttributeAsRect(const c8* attributeName) = 0;

		 //! ��ȡһ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual rect<SINT32> getAttributeAsRect(SINT32 index) = 0;

		 //! ����һ����������
		 virtual void setAttribute(SINT32 index, rect<SINT32> v) = 0;


		 /*

		 Dimension2d ����

		 */

		 //! ���һ��dimension2d����
		 virtual void addDimension2d(const c8* attributeName,  dimension2d<UINT32> value) = 0;

		 //! ����һ��dimension2d����
		 virtual void setAttribute(const c8* attributeName, dimension2d<UINT32> v) = 0;

		 //! ��ȡһ��dimension2d����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual  dimension2d<UINT32> getAttributeAsDimension2d(const c8* attributeName) = 0;


		 //! ��ȡһ��dimension2d����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual  dimension2d<UINT32> getAttributeAsDimension2d(SINT32 index) = 0;

		 //! ����һ��dimension2d����
		 virtual void setAttribute(SINT32 index, dimension2d<UINT32> v) = 0;


		 /*
		 ��������
		 */

		 //! ���һ����������
		 virtual void addMatrix(const c8* attributeName, const Matrix4& v) = 0;

		 //! ����һ����������
		 virtual void setAttribute(const c8* attributeName, const Matrix4& v) = 0;

		 //! ��ȡһ����������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual Matrix4 getAttributeAsMatrix(const c8* attributeName) = 0;

		 //! ��ȡһ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Matrix4 getAttributeAsMatrix(SINT32 index) = 0;

		 //! ����һ����������
		 virtual void setAttribute(SINT32 index, const Matrix4& v) = 0;

		 /*
		   ��Ԫ������

		 */

		 //! ���һ����Ԫ������
		 virtual void addQuaternion(const c8* attributeName, Quaternion v) = 0;

		 //! ����һ����Ԫ������
		 virtual void setAttribute(const c8* attributeName, Quaternion v) = 0;

		 //! ��ȡһ����Ԫ������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual Quaternion getAttributeAsQuaternion(const c8* attributeName) = 0;

		 //! ��ȡһ����Ԫ������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Quaternion getAttributeAsQuaternion(SINT32 index) = 0;

		 //! ����һ����Ԫ������
		 virtual void setAttribute(SINT32 index, Quaternion v) = 0;

		 /*

		 3d AABB��ײ����

		 */

		 //! ���һ��AABB����
		 virtual void addBox3d(const c8* attributeName, AxisAlignedBox v) = 0;

		 //! ����һ��AABB����
		 virtual void setAttribute(const c8* attributeName, AxisAlignedBox v) = 0;

		 //! ��ȡһ��AABB����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual AxisAlignedBox getAttributeAsBox3d(const c8* attributeName) = 0;

		 //! ��ȡһ��AABB����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual AxisAlignedBox getAttributeAsBox3d(SINT32 index) = 0;

		 //! ����һ��AABB����
		 virtual void setAttribute(SINT32 index, AxisAlignedBox v) = 0;

		 /*

		ƽ������

		 */

		 //! ���һ��ƽ������
		 virtual void addPlane3d(const c8* attributeName, Plane v) = 0;

		 //! ����һ��ƽ������
		 virtual void setAttribute(const c8* attributeName, Plane v) = 0;

		 //! ��ȡһ��ƽ������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual Plane getAttributeAsPlane3d(const c8* attributeName) = 0;

		 //! ��ȡһ��ƽ������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual Plane getAttributeAsPlane3d(SINT32 index) = 0;

		 //! ����һ��ƽ������
		 virtual void setAttribute(SINT32 index, Plane v) = 0;


		 /*

		 ������

		 */

		 //! ���һ�� ����������
		 virtual void addTriangle3d(const c8* attributeName, triangle3df v) = 0;

		 //! ����һ�� ����������
		 virtual void setAttribute(const c8* attributeName, triangle3df v) = 0;

		 //! ��ȡһ�� ����������
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual triangle3df getAttributeAsTriangle3d(const c8* attributeName) = 0;

		 //! ��ȡһ�� ����������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual triangle3df getAttributeAsTriangle3d(SINT32 index) = 0;

		 //! ����һ�� ����������
		 virtual void setAttribute(SINT32 index, triangle3df v) = 0;


		 /*

		 2d�߶�

		 */

		 //! ���һ�� 2d�߶�����
		 virtual void addLine2d(const c8* attributeName,  line2df v) = 0;

		 //! ����һ�� 2d�߶�����
		 virtual void setAttribute(const c8* attributeName, line2df v) = 0;

		 //! ��ȡһ�� 2d�߶�����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual line2df getAttributeAsLine2d(const c8* attributeName) = 0;

		 //! ��ȡһ�� 2d�߶�����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual  line2df getAttributeAsLine2d(SINT32 index) = 0;

		 //! ����һ�� 2d�߶�����
		 virtual void setAttribute(SINT32 index,  line2df v) = 0;


		 /*

		 3d �߶�

		 */

		 //! ���һ�� 3d�߶�����
		 virtual void addLine3d(const c8* attributeName,  line3df v) = 0;

		 //! ����һ�� 3d�߶�����
		 virtual void setAttribute(const c8* attributeName,  line3df v) = 0;

		 //! ��ȡһ�� 3d�߶�����
		 //! \param attributeName: ������
		 //! \return ����֮ǰ���õ�����
		 virtual  line3df getAttributeAsLine3d(const c8* attributeName) = 0;

		 //! ��ȡһ�� 3d�߶�����
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual  line3df getAttributeAsLine3d(SINT32 index) = 0;

		 //! ����һ�� 3d�߶�����
		 virtual void setAttribute(SINT32 index, line3df v) = 0;


		 /*

		 ��������
		 */

		 //! ���һ���������õ�����
		 virtual void addTexture(const c8* attributeName,  ITexture* texture, const path& filename = "") = 0;

		 //! ����һ���������õ�����
		 virtual void setAttribute(const c8* attributeName, ITexture* texture, const  path& filename = "") = 0;

		 //! ��ȡһ���������õ�����
		 //! \param attributeName: ������
		 virtual  ITexture* getAttributeAsTexture(const c8* attributeName) = 0;

		 //! ��ȡһ��  ������������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual  ITexture* getAttributeAsTexture(SINT32 index) = 0;

		 //! ����һ�� ������������
		 virtual void setAttribute(SINT32 index,  ITexture* texture, const  path& filename = "") = 0;


		 /*

		 �û�ָ������

		 */

		 //! ���һ���û�ָ�������
		 virtual void addUserPointer(const c8* attributeName, void* userPointer) = 0;

		 //! ����һ���û�ָ�������
		 virtual void setAttribute(const c8* attributeName, void* userPointer) = 0;

		 //! ��ȡһ���û�ָ�������
		 //! \param attributeName: ������
		 virtual void* getAttributeAsUserPointer(const c8* attributeName) = 0;

		 //! ��ȡһ��  �û�ָ������
		 //! \param index: ����ֵ, ���Ե�0 �� getAttributeCount()-1֮��.
		 virtual void* getAttributeAsUserPointer(SINT32 index) = 0;

		 //! ����һ�� �û�ָ������
		 virtual void setAttribute(SINT32 index, void* userPointer) = 0;

	 };
}

#endif