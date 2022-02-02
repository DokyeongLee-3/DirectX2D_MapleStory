#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CSkillQuickSlotWindow :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CSkillQuickSlotWindow();
	CSkillQuickSlotWindow(const CSkillQuickSlotWindow& window);
	virtual ~CSkillQuickSlotWindow();

private:
	CSharedPtr<CImage>	m_QuickSlotBack;
	CSharedPtr<CImage>	m_ExtendBar;
	CSharedPtr<CImage>	m_Shift;
	CSharedPtr<CImage>	m_Insert;
	CSharedPtr<CImage>	m_Home;
	CSharedPtr<CImage>	m_PageUp;
	CSharedPtr<CImage>	m_PageDown;
	CSharedPtr<CImage>	m_End;
	CSharedPtr<CImage>	m_Ctrl;
	CSharedPtr<CImage>	m_Delete;
	CSharedPtr<CImage>	m_KeyQ;
	CSharedPtr<CImage>	m_KeyW;
	CSharedPtr<CImage>	m_KeyE;
	CSharedPtr<CImage>	m_KeyR;
	CSharedPtr<CImage>	m_KeyT;
	CSharedPtr<CImage>	m_KeyA;
	CSharedPtr<CImage>	m_Key1;
	CSharedPtr<CImage>	m_Key2;
	CSharedPtr<CImage>	m_Key3;
	CSharedPtr<CImage>	m_Key4;
	CSharedPtr<CImage>	m_Key5;
	CSharedPtr<CImage>	m_Key6;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CSkillQuickSlotWindow* Clone();

public:
};

