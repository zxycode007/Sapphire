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

	//! IAttributes接口的实现
	class CAttributes : public IAttributes
	{
	public:

		CAttributes(IVideoDriver* driver = 0);
		~CAttributes();

		//! 返回属性集合里的属性数量
		virtual UINT32 getAttributeCount() const;

		//! 通过索引来返回属性
		//! \param index: 索引值, 在0到getAttributeCount()-1之间
		virtual const c8* getAttributeName(SINT32 index);

		//! 返回属性的一个类型
		//! \param attributeName: 属性名
		virtual E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName);

		//! 返回索引的属性类型
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual E_ATTRIBUTE_TYPE getAttributeType(SINT32 index);

		//! 返回属性类型的字符串
		//! \param attributeName: 属性类型的名字
		virtual const StringW getAttributeTypeString(const c8* attributeName);

		//! 通过属性索引返回属性类型的字符串
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual const StringW getAttributeTypeString(SINT32 index);

		//! 一个属性是否存在
		virtual bool existsAttribute(const c8* attributeName);

		//! 查找属性名，返回索引，返回-1没找到
		virtual SINT32 findAttribute(const c8* attributeName) const;

		//! 清空所有属性
		virtual void clear();

		//! 从一个XML文件中读取属性
		//! \param reader 要读取的XML读取器
		//! \param readCurrentElementOnly 如果设置为true，只读取当前元素名字是'attributes'或指定的元素名
		//! \param elementName 周围的元素. 如果设置为空，默认是'attributes' 
		//! 如果设置为flase，第一个出现的属性列表会被读取
		virtual bool read(IXMLReader* reader, bool readCurrentElementOnly = false,
			const wchar_t* nonDefaultElementName = 0);

		//! 写入这些属性到一个XML文件
		//! \param writer: 要写入的XML写入器
		//! \param writeXMLHeader: 写入一个Header到XML文件，如果这个文件开始需要的话
		//! \param elementName:周围的元素. 如果设置为空，默认是'attributes' 
		virtual bool write(IXMLWriter* writer, bool writeXMLHeader = false, const wchar_t* nonDefaultElementName = 0);


		/*

		 整数属性

		*/

		//! 添加一个整形的属性
		virtual void addInt(const c8* attributeName, SINT32 value);

		//! 设置一个整形的属性
		virtual void setAttribute(const c8* attributeName, SINT32 value);

		//! 获取一个整形属性
		//! \param attributeName: 要获取的属性名
		//! \return 返回之前设置的属性值
		virtual SINT32 getAttributeAsInt(const c8* attributeName) const;

		//!获取一个整形属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual SINT32 getAttributeAsInt(SINT32 index) const;

		//! 设置一个整形属性
		virtual void setAttribute(SINT32 index, SINT32 value);

		/*

		浮点属性

		*/

		//! 添加一个浮点值属性
		virtual void addFloat(const c8* attributeName, Real value);

		//! 设置一个浮点值属性
		virtual void setAttribute(const c8* attributeName, Real value);

		//! 获取一个浮点属性的值
		//! \param attributeName: 要获取的属性名
		//! \return 返回之前设置的属性值
		virtual Real getAttributeAsFloat(const c8* attributeName);

		//!获取一个浮点属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Real getAttributeAsFloat(SINT32 index);

		//! 设置一个浮点属性
		virtual void setAttribute(SINT32 index, Real value);

		/*

	字符串属性

		*/

		//! 添加一个字符串属性
		virtual void addString(const c8* attributeName, const c8* value);

		//! 设置一个字符串属性
		//! \param attributeName: 要获取的属性名
		//! \param value: 属性的值，设置为0会删除这个属性
		virtual void setAttribute(const c8* attributeName, const c8* value);

		//! 获取这个字符串属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值，如果没设置返回0
		virtual String getAttributeAsString(const c8* attributeName);

		//! 获取一个字符串属性名
		//! \param attributeName 要获取的属性名
		//! \param target 要存放的字符串缓冲
		virtual void getAttributeAsString(const c8* attributeName, c8* target);

		//! 返回指定索引的字符串属性值
		//! \param index 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual String getAttributeAsString(SINT32 index);

		//! 设置一个指定锁定的字符串的值
		//! \param index 索引值, 可以到0 和 getAttributeCount()-1之间.
		//! \param value 要设置的字符串的值
		virtual void setAttribute(SINT32 index, const c8* value);

		// 宽字节字符串版本

		//!  添加一个字符串属性
		virtual void addString(const c8* attributeName, const wchar_t* value);

		//! 设置一个字符串属性
		//! \param attributeName: 要获取的属性名
		//! \param value: 属性的值，设置为0会删除这个属性
		virtual void setAttribute(const c8* attributeName, const wchar_t* value);

		//! 获取这个字符串属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值，如果没设置返回0
		virtual StringW getAttributeAsStringW(const c8* attributeName);

		//! 获取一个字符串属性名
		//! \param attributeName 要获取的属性名
		//! \param target 要存放的字符串缓冲
		virtual void getAttributeAsStringW(const c8* attributeName, wchar_t* target);

		//! 返回指定索引的字符串属性值
		//! \param index 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual StringW getAttributeAsStringW(SINT32 index);

		//! 设置一个指定锁定的字符串的值
		//! \param index 索引值, 可以到0 和 getAttributeCount()-1之间.
		//! \param value 要设置的字符串的值
		virtual void setAttribute(SINT32 index, const wchar_t* value);

		/*

		 二进制数据属性

		*/

		//! 添加一个二进制数据的属性
		virtual void addBinary(const c8* attributeName, void* data, SINT32 dataSizeInBytes);

		//! 设置一个二进制数据的属性
		virtual void setAttribute(const c8* attributeName, void* data, SINT32 dataSizeInBytes);

		//! 获取一个二进制数据的属性
		/** \param attributeName: 属性名
		\param outData 指向要存放二进制数据的缓冲区
		\param maxSizeInBytes 缓冲区的最大字节数
		*/
		virtual void getAttributeAsBinaryData(const c8* attributeName, void* outData, SINT32 maxSizeInBytes);

		//! 获取一个二进制数据的属性
		/** \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		\param outData 指向要存放二进制数据的缓冲区
		\param maxSizeInBytes 缓冲区的最大字节数
		*/
		virtual void getAttributeAsBinaryData(SINT32 index, void* outData, SINT32 maxSizeInBytes);

		//! 设置一个二进制数据属性
		virtual void setAttribute(SINT32 index, void* data, SINT32 dataSizeInBytes);


		/*
		数组属性
		*/

		//! 添加一个宽字符串数组的属性
		virtual void addArray(const c8* attributeName, const vector<StringW>::type& value);

		//! 设置一个宽字符串数组的属性
		//! \param attributeName: 属性名
		//! \param value: 属性的值， 设置为0会删除这个属性
		virtual void setAttribute(const c8* attributeName, const vector<StringW>::type& value);

		//! 获取一个宽字符串属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值，如果没设置返回0
		virtual vector<StringW>::type getAttributeAsArray(const c8* attributeName);

		//! 通过索引返回一个宽字符串属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual vector<StringW>::type getAttributeAsArray(SINT32 index);

		//! 通过索引设置一个宽字符串的属性
		virtual void setAttribute(SINT32 index, const vector<StringW>::type& value);

		/*

		布尔属性

		*/

		//! 添加一个布尔属性
		virtual void addBool(const c8* attributeName, bool value);

		//! 设置一个布尔的属性
		virtual void setAttribute(const c8* attributeName, bool value);

		//! 获取一个布尔属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值，如果没设置返回0
		virtual bool getAttributeAsBool(const c8* attributeName);

		//! 通过索引返回一个布尔属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual bool getAttributeAsBool(SINT32 index);

		//! 通过索引设置一个布尔的属性
		virtual void setAttribute(SINT32 index, bool value);

		/*

		枚举属性

		*/

		//! 添加一个枚举的属性
		virtual void addEnum(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);

		//! 添加一个枚举属性
		virtual void addEnum(const c8* attributeName, SINT32 enumValue, const c8* const* enumerationLiterals);

		//! 设置一个枚举属性
		virtual void setAttribute(const c8* attributeName, const c8* enumValue, const c8* const* enumerationLiterals);

		//! 获取一个枚举属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值，如果没设置返回0
		virtual const c8* getAttributeAsEnumeration(const c8* attributeName);

		//! 获取一个枚举属性 
		/** \param attributeName: 属性名
		\param enumerationLiteralsToUse: 用于这些枚举文本来获取索引。当这个属性可能由XML文件读取时有用
		，并且只包含这个枚举字符串，但是没有它的索引信息
		\return  返回之前设置的属性值
		*/
		virtual SINT32 getAttributeAsEnumeration(const c8* attributeName, const c8* const* enumerationLiteralsToUse);

		//!  获取一个枚举属性 
		/** \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		\param enumerationLiteralsToUse: 用于这些枚举文本来获取索引。当这个属性可能由XML文件读取时有用
		，并且只包含这个枚举字符串，但是没有它的索引信息
		\return 返回之前设置的属性值
		*/
		virtual SINT32 getAttributeAsEnumeration(SINT32 index, const c8* const* enumerationLiteralsToUse);

		//! 获取一个枚举属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual const c8* getAttributeAsEnumeration(SINT32 index);

		//! 获取一个枚举属性的枚举文本列表
		//! \param attributeName 属性名
		//! \param outLiterals 枚举名的字符串集合
		virtual void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, vector<String>::type& outLiterals);

		//! 获取一个枚举属性的枚举文本列表
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		//! \param outLiterals 枚举名的字符串集合
		virtual void getAttributeEnumerationLiteralsOfEnumeration(SINT32 index, vector<String>::type& outLiterals);

		//! 设置一个枚举属性
		virtual void setAttribute(SINT32 index, const c8* enumValue, const c8* const* enumerationLiterals);


		/*

		ColourValue属性

		*/

		//! 添加一个颜色属性
		virtual void addColor(const c8* attributeName, ColourValue value);

		//! 设置一个颜色属性 
		virtual void setAttribute(const c8* attributeName, ColourValue color);

		//! 获取一个颜色属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性值
		virtual ColourValue getAttributeAsColor(const c8* attributeName);

		//! 获取一个颜色属性
		//! \param index:  索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual ColourValue getAttributeAsColor(SINT32 index);

		//! 设置一个颜色属性
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

		Vector3 属性

		*/

		//!  添加一个Vector3属性
		virtual void addVector3d(const c8* attributeName, Vector3 value);

		//! 设置一个Vector3属性
		virtual void setAttribute(const c8* attributeName, Vector3 v);

		//! 获取一个Vector3属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual Vector3 getAttributeAsVector3d(const c8* attributeName);

		//! 获取一个Vector3属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Vector3 getAttributeAsVector3d(SINT32 index);

		//! 设置一个Vector2属性
		virtual void setAttribute(SINT32 index, Vector3 v);


		/*

		Vector2d Attribute

		*/

		//!  添加一个Vector2属性
		virtual void addVector2d(const c8* attributeName, Vector2 value);

		//! 设置一个Vector2属性
		virtual void setAttribute(const c8* attributeName, Vector2 v);

		//! 获取一个Vector2属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual Vector2 getAttributeAsVector2d(const c8* attributeName);

		//! 获取一个Vector2属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Vector2 getAttributeAsVector2d(SINT32 index);

		//! 设置一个Vector2属性
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

		 矩形属性

		*/

		//! 添加一个矩形属性
		virtual void addRect(const c8* attributeName, rect<SINT32> value);

		//! 设置一个矩形属性
		virtual void setAttribute(const c8* attributeName, rect<SINT32> v);

		//! 获取一个矩形属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual rect<SINT32> getAttributeAsRect(const c8* attributeName);

		//! 获取一个矩形属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual rect<SINT32> getAttributeAsRect(SINT32 index);

		//! 设置一个矩形属性
		virtual void setAttribute(SINT32 index, rect<SINT32> v);


		/*

		Dimension2d 属性

		*/

		//! 添加一个dimension2d属性
		virtual void addDimension2d(const c8* attributeName, dimension2d<UINT32> value);

		//! 设置一个dimension2d属性
		virtual void setAttribute(const c8* attributeName, dimension2d<UINT32> v);

		//! 获取一个dimension2d属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual dimension2d<UINT32> getAttributeAsDimension2d(const c8* attributeName);

		//! 获取一个dimension2d属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual dimension2d<UINT32> getAttributeAsDimension2d(SINT32 index);

		//! 设置一个dimension2d属性
		virtual void setAttribute(SINT32 index, dimension2d<UINT32> v);


		/*
		矩阵属性
		*/

		//! 添加一个矩阵属性
		virtual void addMatrix(const c8* attributeName, const Matrix4& v);

		//! 设置一个矩阵属性
		virtual void setAttribute(const c8* attributeName, const Matrix4& v);

		//! 获取一个矩阵属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual Matrix4 getAttributeAsMatrix(const c8* attributeName);

		//! 获取一个矩阵属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Matrix4 getAttributeAsMatrix(SINT32 index);

		//! 设置一个矩阵属性
		virtual void setAttribute(SINT32 index, const Matrix4& v);

		/*
		  四元数属性
		*/

		//! 添加一个四元数属性
		virtual void addQuaternion(const c8* attributeName, Quaternion v);

		//! 设置一个四元数属性
		virtual void setAttribute(const c8* attributeName, Quaternion v);

		//! 获取一个四元数属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual Quaternion getAttributeAsQuaternion(const c8* attributeName);

		//! 获取一个四元数属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Quaternion getAttributeAsQuaternion(SINT32 index);

		//! 设置一个四元数属性
		virtual void setAttribute(SINT32 index, Quaternion v);

		/*

		3d AABB碰撞盒子

		*/

		//! 添加一个AABB属性
		virtual void addBox3d(const c8* attributeName, AxisAlignedBox v);

		//! 设置一个AABB属性
		virtual void setAttribute(const c8* attributeName, AxisAlignedBox v);

		//! 获取一个AABB属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual AxisAlignedBox getAttributeAsBox3d(const c8* attributeName);

		//! 获取一个AABB属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual AxisAlignedBox getAttributeAsBox3d(SINT32 index);

		//! 设置一个AABB属性
		virtual void setAttribute(SINT32 index, AxisAlignedBox v);

		/*

		平面属性

		*/

		//! 添加一个平面属性
		virtual void addPlane3d(const c8* attributeName, Plane v);

		//! 设置一个平面属性
		virtual void setAttribute(const c8* attributeName, Plane v);

		//! 获取一个平面属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual Plane getAttributeAsPlane3d(const c8* attributeName);

		//! 获取一个平面属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual Plane getAttributeAsPlane3d(SINT32 index);

		//! 设置一个平面属性
		virtual void setAttribute(SINT32 index, Plane v);


		/*

		三角形

		*/

		//! 添加一个 三角形属性
		virtual void addTriangle3d(const c8* attributeName, triangle3df v);

		//! 设置一个 三角形属性
		virtual void setAttribute(const c8* attributeName, triangle3df v);

		//! 获取一个 三角形属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual triangle3df getAttributeAsTriangle3d(const c8* attributeName);

		//! 获取一个 三角形属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual triangle3df getAttributeAsTriangle3d(SINT32 index);

		//! 设置一个 三角形属性
		virtual void setAttribute(SINT32 index, triangle3df v);



		/*

		2d线段

		*/

		//! 添加一个 2d线段属性
		virtual void addLine2d(const c8* attributeName, line2df v);

		//! 设置一个 2d线段属性
		virtual void setAttribute(const c8* attributeName, line2df v);

		//! 获取一个 2d线段属性
		//! \param attributeName: 属性名
		//! \return 返回之前设置的属性
		virtual line2df getAttributeAsLine2d(const c8* attributeName);

		//! 获取一个 2d线段属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual line2df getAttributeAsLine2d(SINT32 index);

		//! 设置一个 2d线段属性
		virtual void setAttribute(SINT32 index, line2df v);


		/*

		3d 线段

		*/

		//! 添加一个 3d线段属性
		virtual void addLine3d(const c8* attributeName, line3df v);

		//! 设置一个用户指针的属性
		virtual void setAttribute(const c8* attributeName, line3df v);

		//! 获取一个用户指针的属性
		//! \param attributeName: 属性名
		virtual line3df getAttributeAsLine3d(const c8* attributeName);

		//! 获取一个  用户指针属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual line3df getAttributeAsLine3d(SINT32 index);

		//! 设置一个 用户指针属性
		virtual void setAttribute(SINT32 index, line3df v);


		/*

		纹理属性
		*/

		//! 添加一个纹理引用的属性
		virtual void addTexture(const c8* attributeName, ITexture* texture, const path& filename = "");

		//! 设置一个纹理引用的属性
		virtual void setAttribute(const c8* attributeName, ITexture* texture, const path& filename = "");

		//! 获取一个纹理引用的属性
		//! \param attributeName: 属性名
		virtual ITexture* getAttributeAsTexture(const c8* attributeName);

		//! 获取一个  纹理引用属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual ITexture* getAttributeAsTexture(SINT32 index);

		//! 设置一个 纹理引用属性
		virtual void setAttribute(SINT32 index, ITexture* texture, const path& filename = "");



		/*

		用户指针属性

		*/

		//! 添加一个用户指针的属性
		virtual void addUserPointer(const c8* attributeName, void* userPointer);

		//! 设置一个用户指针的属性
		virtual void setAttribute(const c8* attributeName, void* userPointer);

		//! 获取一个用户指针的属性
		//! \param attributeName: 属性名
		virtual void* getAttributeAsUserPointer(const c8* attributeName);

		//! 获取一个  用户指针属性
		//! \param index: 索引值, 可以到0 和 getAttributeCount()-1之间.
		virtual void* getAttributeAsUserPointer(SINT32 index);

		//! 设置一个 用户指针属性
		virtual void setAttribute(SINT32 index, void* userPointer);

	protected:

		void readAttributeFromXML(IXMLReader* reader);

		vector<IAttribute*>::type Attributes;

		IAttribute* getAttributeP(const c8* attributeName) const;

		IVideoDriver* Driver;
	};

}

#endif
