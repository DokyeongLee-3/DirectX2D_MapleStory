#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CStartWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CStartWidget();
	CStartWidget(const CStartWidget& window);
	virtual ~CStartWidget();

private:
	CSharedPtr<CButton>	m_ExitButton;
	CSharedPtr<CImage>	m_Back;
	CSharedPtr<CImage>	m_JobFlag;
	CSharedPtr<CImage>	m_CharacterSelectBackLight;
	CSharedPtr<CImage>	m_Player;
	CSharedPtr<CText>	m_PlayerName;
	CSharedPtr<CImage>	m_PlayerNameTagBack;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CStartWidget* Clone();

public:
	CImage* GetPlayer()	const
	{
		return m_Player;
	}

public:
	void ExitButtonClick();
	void CharacterClick();
};

