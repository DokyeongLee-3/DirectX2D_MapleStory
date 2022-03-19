#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"
#include "Widget/Image.h"

class CNPCName :
    public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CNPCName();
	CNPCName(const CNPCName& window);
	virtual ~CNPCName();

protected:
	CSharedPtr<CText>	m_NPCName;
	CSharedPtr<CImage>	m_DoubleClickFrame;

public:
	void SetNPCName(const std::string& Name)
	{
		wchar_t WName[256] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, WName, Length);

		m_NPCName->SetText(WName);
		m_NPCName->SetAlphaEnable(true);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CNPCName* Clone();

public:
	void OpenDialog();
};

