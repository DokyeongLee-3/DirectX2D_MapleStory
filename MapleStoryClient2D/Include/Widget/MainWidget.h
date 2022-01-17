#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Text.h"

class CMainWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CMainWidget();
	CMainWidget(const CMainWidget& window);
	virtual ~CMainWidget();

private:
	CSharedPtr<CButton>	m_Button;

	CSharedPtr<CButton>	m_Button1;
	CSharedPtr<CText>	m_Button1Text;

	CSharedPtr<CButton>	m_AnimationButton;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CMainWidget* Clone();

public:
	void StartButtonClick();
	void Button1Click();
};

