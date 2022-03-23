#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "Widget/Number.h"

class CBossInfoWindow :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CBossInfoWindow();
	CBossInfoWindow(const CBossInfoWindow& window);
	virtual ~CBossInfoWindow();

private:
	CSharedPtr<CImage> m_HPBarBack;
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CImage>	m_RemainTimeBack;

	CSharedPtr<CNumber>	m_RemainTimeMinuteDigit1;
	CSharedPtr<CNumber>	m_RemainTimeMinuteDigit2;
	CSharedPtr<CNumber>	m_RemainTimeSecDigit1;
	CSharedPtr<CNumber>	m_RemainTimeSecDigit2;

	float				m_AccTime;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CBossInfoWindow* Clone();

public:
	void BossHPPercent(float Percent);
};

