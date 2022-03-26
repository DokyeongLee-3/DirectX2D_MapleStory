#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"
#include "Widget/Image.h"

class CPlayerName :
    public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CPlayerName();
	CPlayerName(const CPlayerName& window);
	virtual ~CPlayerName();

protected:
	CSharedPtr<CImage>	m_PlayerNameBack;
	CSharedPtr<CText>	m_PlayerName;


public:
	void SetPlayerName(const std::string& Name)
	{
		wchar_t WName[256] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, WName, Length);

		m_PlayerName->SetText(WName);
		m_PlayerName->SetAlphaEnable(true);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CPlayerName* Clone();
};

