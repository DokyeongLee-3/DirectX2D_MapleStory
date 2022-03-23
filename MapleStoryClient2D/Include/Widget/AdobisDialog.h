#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CAdobisDialog :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CAdobisDialog();
	CAdobisDialog(const CAdobisDialog& window);
	virtual ~CAdobisDialog();

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
	virtual CAdobisDialog* Clone();

public:
	void ChatStopCallback();
	void EnterZakumMap();
};

