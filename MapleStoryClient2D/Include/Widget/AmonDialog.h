#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CAmonDialog :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CAmonDialog();
	CAmonDialog(const CAmonDialog& window);
	virtual ~CAmonDialog();

private:
	CSharedPtr<CImage> m_Background;
	CSharedPtr<CButton>	m_ChatStopButton;
	CSharedPtr<CText>	m_ChatContents;
	CSharedPtr<CButton>	m_BtYes;
	CSharedPtr<CButton>	m_BtNo;
	std::wstring m_Contents;

	class CNPCAmon* m_NPCAmon;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CAmonDialog* Clone();

public:
	void SetNPCAmon(class CNPCAmon* Amon);
	void GoToWayToZakumScene();

public:
	void ChatStopCallback();
	void ExitZakumMap();
};

