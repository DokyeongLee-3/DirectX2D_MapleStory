#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Number.h"
#include "Widget/Text.h"
#include "Widget/ProgressBar.h"

class CCharacterStatusWindow :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CCharacterStatusWindow();
	CCharacterStatusWindow(const CCharacterStatusWindow& window);
	virtual ~CCharacterStatusWindow();

private:
	CSharedPtr<CImage> m_HPMPBackground;
	CSharedPtr<CImage> m_Level;
	CSharedPtr<CNumber> m_LevelNumber;
	CSharedPtr<CText>	m_PlayerName;
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_MPBar;
	CSharedPtr<CNumber> m_HPMaxNumber;
	CSharedPtr<CNumber> m_MPMaxNumber;
	CSharedPtr<CNumber> m_CurrentHPNumber;
	CSharedPtr<CNumber> m_CurrentMPNumber;
	CSharedPtr<CImage> m_HPDelimiter;
	CSharedPtr<CImage> m_MPDelimiter;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCharacterStatusWindow* Clone();

public:
};

