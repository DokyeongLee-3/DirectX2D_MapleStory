
#include "SkillQuickSlotWindow.h"

CSkillQuickSlotWindow::CSkillQuickSlotWindow()
{
}

CSkillQuickSlotWindow::CSkillQuickSlotWindow(const CSkillQuickSlotWindow& window)	:
	CWidgetWindow(window)
{
}

CSkillQuickSlotWindow::~CSkillQuickSlotWindow()
{
}

void CSkillQuickSlotWindow::Start()
{
	CWidgetWindow::Start();
}

bool CSkillQuickSlotWindow::Init()
{

	SetPos(929.f, 11.f);
	SetSize(351.f, 73.f);

	m_QuickSlotBack = CreateWidget<CImage>("SkillQuickSlotBack");
	m_QuickSlotBack->SetTexture("SkillQuickSlotBack", TEXT("UI/Skill/QuickSlotBackground.png"));
	m_QuickSlotBack->SetSize(347.f, 67.f);
	m_QuickSlotBack->SetMouseCollisionEnable(true);

	CImage* QuickSlogLayer = CreateWidget<CImage>("QuickSlogLayer");
	QuickSlogLayer->SetTexture("QuickSlogLayer", TEXT("UI/Skill/QuickSlotLayer.png"));
	QuickSlogLayer->SetSize(351.f, 73.f);
	QuickSlogLayer->SetMouseCollisionEnable(true);
	QuickSlogLayer->SetZOrder(5);

	m_ExtendBar = CreateWidget<CImage>("SkillQuickSlotExtendBar");
	m_ExtendBar->SetTexture("SkillQuickSlotExtendBar", TEXT("UI/Skill/QuickSlotNormal.png"));
	m_ExtendBar->SetSize(9.f, 71.f);
	m_ExtendBar->SetPos(-6.f, 0.f);
	m_ExtendBar->SetMouseCollisionEnable(true);
	m_ExtendBar->SetZOrder(5);



	return true;
}

void CSkillQuickSlotWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CSkillQuickSlotWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CSkillQuickSlotWindow::Render()
{
	CWidgetWindow::Render();
}

CSkillQuickSlotWindow* CSkillQuickSlotWindow::Clone()
{
	return new CSkillQuickSlotWindow(*this);
}
