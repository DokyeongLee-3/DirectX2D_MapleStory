#pragma once

#include "IMGUIWidget.h"

class CIMGUICheckBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUICheckBox();
	virtual ~CIMGUICheckBox();

private:
	char m_TextUTF8[1024];
	std::function<void()> m_Callback;
	// 체크박스에 체크 됐는지
	bool m_Check;

public:
	bool IsCheck()	const
	{
		return m_Check;
	}

	void SetCheck()
	{
		m_Check = true;
	}

	void SetNotCheck()
	{
		m_Check = false;
	}

public:
	template <typename T>
	void SetText(const char* Text, T* Obj, void(T::* Func)())
	{
		int TextLen = (int)strlen(Text);
		wchar_t	wText[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, wText, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, 0, 0, 0, 0);

		char* TextUTF8 = new char[Length + 1];

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, 0, 0);

		strcpy_s(m_TextUTF8, TextUTF8);

		if (Obj && Func)
			m_Callback = std::bind(Func, Obj);
		else
			m_Callback = nullptr;


		delete[] TextUTF8;
	}

public:
	virtual bool Init();
	virtual void Render();
};

