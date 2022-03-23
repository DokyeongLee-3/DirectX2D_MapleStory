#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "Widget/Number.h"

class CCharacterEXP :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CCharacterEXP();
	CCharacterEXP(const CCharacterEXP& window);
	virtual ~CCharacterEXP();

private:
	CSharedPtr<CImage> m_EXPBackground;
	CSharedPtr<CProgressBar> m_EXPBar;
	//CSharedPtr<CNumber> m_EXPMaxNumber;
	CSharedPtr<CNumber> m_EXPNumber;
	//CSharedPtr<CImage> m_EXPDelimiter;
	CSharedPtr<CImage> m_EXPLeftBracket;
	CSharedPtr<CImage> m_EXPRightBracket;
	CSharedPtr<CNumber>	m_EXPPercentNumber;
	CSharedPtr<CImage>	m_EXPPercent;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCharacterEXP* Clone();

public:
	void SetEXP(int EXP);

};

