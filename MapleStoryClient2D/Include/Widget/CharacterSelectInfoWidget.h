#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Number.h"
#include "Widget/Image.h"
#include "Widget/Text.h"
#include "Widget/Button.h"

class CCharacterSelectInfoWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CCharacterSelectInfoWidget();
	CCharacterSelectInfoWidget(const CCharacterSelectInfoWidget& window);
	virtual ~CCharacterSelectInfoWidget();

private:
	CSharedPtr<CImage>	m_Back;
	CSharedPtr<CImage>	m_LevelImage;
	CSharedPtr<CNumber>	m_LevelNumber;
	CSharedPtr<CText>	m_Name;
	CSharedPtr<CText>	m_Job;
	CSharedPtr<CNumber> m_STR;
	CSharedPtr<CNumber> m_INT;
	CSharedPtr<CNumber> m_DEX;
	CSharedPtr<CNumber> m_LUK;
	CSharedPtr<CButton>	m_StartButton;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCharacterSelectInfoWidget* Clone();

public:
	void StartButtonClick();
};

