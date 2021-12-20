#pragma once

#include "GameInfo.h"

class CIMGUIWindow
{
public:
	CIMGUIWindow();
	virtual ~CIMGUIWindow();

protected:
	std::string	m_Name;
	bool	m_Open;
	int		m_WindowFlag;
	std::vector<class CIMGUIWidget*>	m_vecWidget;

protected:
	std::string	m_PopupTitle;
	// 팝업창이 열렸을때 팝업창 밖에 다른 영역을 클릭했을때
	// 클릭해도 클릭이 안되면 Modal방식, 다른 영역 클릭이 되면
	// Modaless방식
	bool		m_ModalPopup;
	std::vector<class CIMGUIWidget*>	m_vecPopupWidget;

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	void AddWindowFlag(ImGuiWindowFlags_ Flag)
	{
		m_WindowFlag |= Flag;
	}

	void EnableModalPopup()
	{
		m_ModalPopup = true;
	}

	void SetPopupWindowTitle(const std::string& Title)
	{
		m_PopupTitle = Title;
	}

public:
	void Open();
	void Close();
	void ClosePopup();
	class CIMGUIWidget* FindWidget(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	template <typename T>
	T* AddWidget(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->SetOwner(this);
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back(Widget);

		return Widget;
	}

	template <typename T>
	T* AddPopupWidget(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->SetOwner(this);
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecPopupWidget.push_back(Widget);

		return Widget;
	}
};

