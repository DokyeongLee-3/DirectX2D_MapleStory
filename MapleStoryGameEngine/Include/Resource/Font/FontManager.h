#pragma once

#include "../../GameInfo.h"

class CFontManager
{
	friend class CResourceManager;

private:
	CFontManager();
	~CFontManager();

private:
	IDWriteFactory5* m_WriteFactory;
	// DWrite의 폰트객체에 해당하는 IDWriteTextFormat객체를 생성할때 사용하는 객체
	// IDWriteFontCollection1사용해서 IDWriteTextFormat객체를 만들어주면
	// IDWriteFontCollection1에 등록된 폰트를 참조하게 된다.
	std::unordered_map<std::string, IDWriteFontCollection1*>	m_mapFontFile;
	// Dwrite의 폰트 객체
	std::unordered_map<std::string, IDWriteTextFormat*>			m_mapFont;
	// ARGB순으로 8비트씩 unsigned int에 들어가있다
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>		m_mapFontColor;
	TCHAR			m_FontFamilyName[256];
	char			m_FontFamilyNameMultibyte[256];

public:
	bool Init();
	bool CreateFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName, int Weight, float FontSize,
		const TCHAR* LocalName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	const TCHAR* GetFontFaceName(const std::string& Name);
	const char* GetFontFaceNameMultibyte(const std::string& Name);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& Color);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, IDWriteTextFormat* Font, float Width,
		float Height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, const std::string& FontName, float Width,
		float Height);

	IDWriteFontCollection1* FindFontFile(const std::string& Name);
	IDWriteTextFormat* FindFont(const std::string& Name);
};

