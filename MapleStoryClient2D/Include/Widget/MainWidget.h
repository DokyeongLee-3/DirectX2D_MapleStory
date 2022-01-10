#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"

class CMainWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CMainWidget();
	virtual ~CMainWidget();

private:
	CSharedPtr<CButton>	m_Button;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();

public:
	void StartButtonClick();
};

