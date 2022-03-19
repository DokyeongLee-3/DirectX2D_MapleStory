#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CNPCDialog :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CNPCDialog();
	CNPCDialog(const CNPCDialog& window);
	virtual ~CNPCDialog();

private:
	CSharedPtr<CImage> m_Background;
	CSharedPtr<CButton>	m_ChatStopButton;
	CSharedPtr<CText>	m_ChatContents;
	CSharedPtr<CButton>	m_Option1;
	std::wstring m_Contents;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CNPCDialog* Clone();

public:
	void ChatStopCallback();
	void EnterZakumMap();
};

