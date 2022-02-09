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
	CSharedPtr<CImage>	m_QuickSlotLayer;
	CSharedPtr<CImage>	m_ExtendBar;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CSkillQuickSlotWindow* Clone();

public:
};

