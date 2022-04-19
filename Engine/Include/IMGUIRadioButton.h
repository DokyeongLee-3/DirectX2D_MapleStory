#pragma once



#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

// System includes
#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

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
    std::vector<bool>       m_vecActive;

public:
    char* GetRadioButtonLabel(int Index)
    {
        return m_vecRadioData[Index].TextUTF8;
    }

    void AddActive(bool Active)
    {
        m_vecActive.push_back(Active);
    }

    void SetActive(int Index, bool Active)
    {
        m_vecActive[Index] = Active;
    }

    void CallRadioButtonCallback(int Index)
    {
        m_vecRadioData[Index].Callback();
    }

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

