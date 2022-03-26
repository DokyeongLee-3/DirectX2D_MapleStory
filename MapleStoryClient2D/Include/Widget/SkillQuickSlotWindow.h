#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"

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

	CSharedPtr<CProgressBar>	m_SylphideLancerProgressBar;
	CSharedPtr<CProgressBar>	m_VoidPressureProgressBar;
	CSharedPtr<CProgressBar>	m_DeathSideProgressBar;

	std::vector<QuickSlotItemState*> m_vecQuickSlotItem;

	// ÇÑÄ­¸¶´Ù »çÀÌ¿¡ ÇÁ·¹ÀÓ µÎ²²
	Vector2 m_Offset;
	// Äü½½·Ô ÇÑÄ­´ç Å©±â
	Vector2 m_SlotSize;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CSkillQuickSlotWindow* Clone();

public:
	void SetSylphideLancerProgressBarPercent(float Percent)
	{
		m_SylphideLancerProgressBar->SetPercent(Percent);
	}

	void SetVoidPressureProgressBarPercent(float Percent)
	{
		m_VoidPressureProgressBar->SetPercent(Percent);
	}

	void SetDeathSideProgressBarPercent(float Percent)
	{
		m_DeathSideProgressBar->SetPercent(Percent);
	}

public:
	void RegisterItem(const std::string& Name, const std::string& RegisterKey, int Row, int Column, int Count);
	void ConsumeItem(const std::string& Name, int ConsumeCount);
	QuickSlotItemState* FindRegisterItem(const std::string& RegisterKey);
};

