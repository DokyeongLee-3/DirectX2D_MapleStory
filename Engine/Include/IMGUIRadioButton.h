#pragma once

#include "IMGUIWidget.h"

struct RadioData
{
	char TextUTF8[1024];
	std::function<void()> Callback;
};

class CIMGUIRadioButton :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUITree;

protected:
	CIMGUIRadioButton();
	virtual ~CIMGUIRadioButton();

protected:
	std::vector<RadioData> m_vecRadioData;

public:
	template <typename T>
	void AddText(const char* Text, T* Obj, void(T::* Func)())
	{
		int TextLen = (int)strlen(Text);
		wchar_t	wText[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, wText, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, 0, 0, 0, 0);

		char* TextUTF8 = new char[Length + 1];

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, 0, 0);

		RadioData Data;

		strcpy_s(Data.TextUTF8, TextUTF8);

		if (Obj && Func)
			Data.Callback = std::bind(Func, Obj);
		else
			Data.Callback = nullptr;


		m_vecRadioData.push_back(Data);

		delete[] TextUTF8;
	}

public:
	virtual bool Init();
	virtual void Render();

};

