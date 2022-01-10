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
	std::function<void()> m_CheckCallback;
	std::function<void()> m_UnCheckCallback;
	// üũ�ڽ��� üũ �ƴ���
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

	template <typename T>
	void SetCheckCallback(T* Obj, void(T::* Func)())
	{
		m_CheckCallback = std::bind(Func, Obj);
	}

	template <typename T>
	void SetUnCheckCallback(T* Obj, void(T::* Func)())
	{
		m_UnCheckCallback = std::bind(Func, Obj);
	}

public:
	template <typename T>
	void SetText(const char* Text, T* Obj, void(T::* Func)())
	{
		int TextLen = (int)strlen(Text);
		wchar_t	wText[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, wText, Length);

		// UTF8�� ��ȯ�Ѵ�.
		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, 0, 0, 0, 0);

		char* TextUTF8 = new char[Length + 1];

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, 0, 0);

		strcpy_s(m_TextUTF8, TextUTF8);

		if (Obj && Func)
			m_CheckCallback = std::bind(Func, Obj);
		else
			m_CheckCallback = nullptr;


		delete[] TextUTF8;
	}

public:
	virtual bool Init();
	virtual void Render();
};

