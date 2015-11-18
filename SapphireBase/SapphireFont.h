#ifndef _SAPPHIRE_FONT_
#define _SAPPHIRE_FONT_
#include "SapphirePrerequisites.h"
#include "SapphireRectangle.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	//! GUI字体的枚举类型
	enum EGUI_FONT_TYPE
	{
		//! 从一个XML文件或一个纹理中加载位图字体
		EGFT_BITMAP = 0,

		//! 由XML文件读取可缩放矢量字体
		/** 这些字体驻留在系统内存中并且直到他们显示也不用显存。它们比位图字体慢，但是更容易旋转和缩放*/
		EGFT_VECTOR,

		//! 使用一个操作系统中立API的字体
		EGFT_OS,

		//! 一个用户扩展的字体类型
		EGFT_CUSTOM
	};

	//!字体接口
	class IGUIFont 
	{
	public:

		//! 绘制一些文本并且剪切它到指定的矩形区域类
		/** \param text: 要绘制的文本
		\param position: 绘制文本的矩形的位置
		\param color: 文本的颜色
		\param hcenter: 指定如果这个文本是否该在矩形的水平中心
		\param vcenter: 指定如果这个文本是否该在矩形的垂直中心
		\param clip: 可选指针，指向一个矩形，它指向的文本会被剪切
		*/
		virtual void draw(const String& text, const Rectangle& position,
			ColourValue color, bool hcenter = false, bool vcenter = false,
			const Rectangle* clip = 0) = 0;

		//! 计算给定文本字符串的宽度和高度
		/** \return 要绘制的文本区域的宽度和高度*/
		virtual dimension2d<UINT32> getDimension(const wchar_t* text) const = 0;

		//! 计算字符索引在文本中的指定位置
		/** \param text: 文本字符串
		\param pixel_x: 返回这个字符索引处的像素位置
		\return 字符索引在文本中返回0， 没有返回-1*/
		virtual SINT32 getCharacterFromPos(const wchar_t* text, SINT32 pixel_x) const = 0;

		//!返回字体类型
		virtual EGUI_FONT_TYPE getType() const { return EGFT_CUSTOM; }

		//! 设置字体的全局字间距宽度
		virtual void setKerningWidth(SINT32 kerning) = 0;

		//! 设置字体的全局字间距高度
		virtual void setKerningHeight(SINT32 kerning) = 0;

		//! 获取对于这个字体的字间距值（字母之间的距离）。如果没有参数,返回全局字间距
		/**
		\param thisLetter: 如果提供这个参数， 增加全局间距值到这个字母的左侧字间距。例如一个空格可能只有一个像素宽，
		但可以显示为几个像素
		\param previousLetter: 如果提供， 字间距由两个字母和添加到增加的全局字间距的值计算。 
		*/
		virtual SINT32 getKerningWidth(const wchar_t* thisLetter = 0, const wchar_t* previousLetter = 0) const = 0;

		//! 返回字母之间的距离
		virtual SINT32 getKerningHeight() const = 0;

		//! 定义个不用该字体绘制的字符串
		/** 例如 在大多数字体中，" "不会绘制任何的空格，都是作为空白
		\param s 不会发送给视频驱动的符号字符串
		*/
		virtual void setInvisibleCharacters(const wchar_t *s) = 0;
	};
}

#endif