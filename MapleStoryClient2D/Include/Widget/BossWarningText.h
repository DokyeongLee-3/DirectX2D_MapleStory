#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"

class CBossWarningText :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CBossWarningText();
	CBossWarningText(const CBossWarningText& window);
	virtual ~CBossWarningText();

private:
	CSharedPtr<CText>	m_BossWarning;
	std::wstring		m_BossWarningText;
	float				m_AliveTime;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CBossWarningText* Clone();
};

