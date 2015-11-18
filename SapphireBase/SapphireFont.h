#ifndef _SAPPHIRE_FONT_
#define _SAPPHIRE_FONT_
#include "SapphirePrerequisites.h"
#include "SapphireRectangle.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	//! GUI�����ö������
	enum EGUI_FONT_TYPE
	{
		//! ��һ��XML�ļ���һ�������м���λͼ����
		EGFT_BITMAP = 0,

		//! ��XML�ļ���ȡ������ʸ������
		/** ��Щ����פ����ϵͳ�ڴ��в���ֱ��������ʾҲ�����Դ档���Ǳ�λͼ�����������Ǹ�������ת������*/
		EGFT_VECTOR,

		//! ʹ��һ������ϵͳ����API������
		EGFT_OS,

		//! һ���û���չ����������
		EGFT_CUSTOM
	};

	//!����ӿ�
	class IGUIFont 
	{
	public:

		//! ����һЩ�ı����Ҽ�������ָ���ľ���������
		/** \param text: Ҫ���Ƶ��ı�
		\param position: �����ı��ľ��ε�λ��
		\param color: �ı�����ɫ
		\param hcenter: ָ���������ı��Ƿ���ھ��ε�ˮƽ����
		\param vcenter: ָ���������ı��Ƿ���ھ��εĴ�ֱ����
		\param clip: ��ѡָ�룬ָ��һ�����Σ���ָ����ı��ᱻ����
		*/
		virtual void draw(const String& text, const Rectangle& position,
			ColourValue color, bool hcenter = false, bool vcenter = false,
			const Rectangle* clip = 0) = 0;

		//! ��������ı��ַ����Ŀ�Ⱥ͸߶�
		/** \return Ҫ���Ƶ��ı�����Ŀ�Ⱥ͸߶�*/
		virtual dimension2d<UINT32> getDimension(const wchar_t* text) const = 0;

		//! �����ַ��������ı��е�ָ��λ��
		/** \param text: �ı��ַ���
		\param pixel_x: ��������ַ�������������λ��
		\return �ַ��������ı��з���0�� û�з���-1*/
		virtual SINT32 getCharacterFromPos(const wchar_t* text, SINT32 pixel_x) const = 0;

		//!������������
		virtual EGUI_FONT_TYPE getType() const { return EGFT_CUSTOM; }

		//! ���������ȫ���ּ����
		virtual void setKerningWidth(SINT32 kerning) = 0;

		//! ���������ȫ���ּ��߶�
		virtual void setKerningHeight(SINT32 kerning) = 0;

		//! ��ȡ�������������ּ��ֵ����ĸ֮��ľ��룩�����û�в���,����ȫ���ּ��
		/**
		\param thisLetter: ����ṩ��������� ����ȫ�ּ��ֵ�������ĸ������ּ�ࡣ����һ���ո����ֻ��һ�����ؿ�
		��������ʾΪ��������
		\param previousLetter: ����ṩ�� �ּ����������ĸ����ӵ����ӵ�ȫ���ּ���ֵ���㡣 
		*/
		virtual SINT32 getKerningWidth(const wchar_t* thisLetter = 0, const wchar_t* previousLetter = 0) const = 0;

		//! ������ĸ֮��ľ���
		virtual SINT32 getKerningHeight() const = 0;

		//! ��������ø�������Ƶ��ַ���
		/** ���� �ڴ���������У�" "��������κεĿո񣬶�����Ϊ�հ�
		\param s ���ᷢ�͸���Ƶ�����ķ����ַ���
		*/
		virtual void setInvisibleCharacters(const wchar_t *s) = 0;
	};
}

#endif