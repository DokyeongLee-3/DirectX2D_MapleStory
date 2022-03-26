
#include "SkillQuickSlotWindow.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "../Object/PlayerSkillInfo.h"
#include "Widget/Number.h"

CSkillQuickSlotWindow::CSkillQuickSlotWindow()	:
	m_Offset(5.f, 5.f),
	m_SlotSize(30.f, 30.f)
{
}

CSkillQuickSlotWindow::CSkillQuickSlotWindow(const CSkillQuickSlotWindow& window)	:
	CWidgetWindow(window)
{
	m_QuickSlotBack = window.m_QuickSlotBack;
	m_QuickSlotLayer = window.m_QuickSlotLayer;
	m_ExtendBar = window.m_ExtendBar;
}

CSkillQuickSlotWindow::~CSkillQuickSlotWindow()
{
	size_t Count = m_vecQuickSlotItem.size();

	for (int i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_vecQuickSlotItem[i]);
	}
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

	m_QuickSlotLayer = CreateWidget<CImage>("QuickSlogLayer");
	m_QuickSlotLayer->SetTexture("QuickSlogLayer", TEXT("UI/Skill/QuickSlotLayer.png"));
	m_QuickSlotLayer->SetSize(351.f, 73.f);
	m_QuickSlotLayer->SetMouseCollisionEnable(true);
	m_QuickSlotLayer->SetZOrder(5);

	m_ExtendBar = CreateWidget<CImage>("SkillQuickSlotExtendBar");
	m_ExtendBar->SetTexture("SkillQuickSlotExtendBar", TEXT("UI/Skill/QuickSlotNormal.png"));
	m_ExtendBar->SetSize(9.f, 71.f);
	m_ExtendBar->SetPos(-6.f, 0.f);
	m_ExtendBar->SetMouseCollisionEnable(true);
	m_ExtendBar->SetZOrder(5);

	m_SylphideLancerProgressBar = CreateWidget<CProgressBar>("SylphideLancerProgressBar");
	m_SylphideLancerProgressBar->SetTexture("SkillProgressBar", TEXT("Player/Skill/SkillProgressBar.png"));
	m_SylphideLancerProgressBar->SetSize(32.f, 32.f);
	m_SylphideLancerProgressBar->SetPos(142.f, 5.f);
	m_SylphideLancerProgressBar->SetPercent(0.f);
	m_SylphideLancerProgressBar->SetDir(ProgressBar_Dir::RightToLeft);
	m_SylphideLancerProgressBar->SetOpacity(0.7f);
	m_SylphideLancerProgressBar->SetZOrder(6);

	m_VoidPressureProgressBar = CreateWidget<CProgressBar>("VoidPressureProgressBar");
	m_VoidPressureProgressBar->SetTexture("SkillProgressBar", TEXT("Player/Skill/SkillProgressBar.png"));
	m_VoidPressureProgressBar->SetSize(32.f, 32.f);
	m_VoidPressureProgressBar->SetPos(177.f, 4.f);
	m_VoidPressureProgressBar->SetPercent(0.f);
	m_VoidPressureProgressBar->SetDir(ProgressBar_Dir::RightToLeft);
	m_VoidPressureProgressBar->SetOpacity(0.7f);
	m_VoidPressureProgressBar->SetZOrder(6);

	m_DeathSideProgressBar = CreateWidget<CProgressBar>("DeathSideProgressBar");
	m_DeathSideProgressBar->SetTexture("SkillProgressBar", TEXT("Player/Skill/SkillProgressBar.png"));
	m_DeathSideProgressBar->SetSize(32.f, 32.f);
	m_DeathSideProgressBar->SetPos(212.f, 4.f);
	m_DeathSideProgressBar->SetPercent(0.f);
	m_DeathSideProgressBar->SetDir(ProgressBar_Dir::RightToLeft);
	m_DeathSideProgressBar->SetOpacity(0.7f);
	m_DeathSideProgressBar->SetZOrder(6);

	RegisterItem("BroiledEels", "1", 1, 4, 999);

	return true;
}

void CSkillQuickSlotWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	size_t Count = m_vecQuickSlotItem.size();

	for (size_t i = 0; i < Count; ++i)
	{
		int ItemCount = m_vecQuickSlotItem[i]->Count;
		m_vecQuickSlotItem[i]->ItemCountWidget->SetNumber(ItemCount);
	}
}

void CSkillQuickSlotWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	CPlayerSkillInfo* PlayerSkillInfo = Player->GetPlayerSkillInfo();

	SkillInfo* Info = PlayerSkillInfo->FindSkillInfo("SylphideLancer");

	if (Info && Info->Active)
	{
		float Area = (Info->CoolTime - Info->AccTime) / Info->CoolTime;
		m_SylphideLancerProgressBar->SetPercent(Area);
	}

	Info = PlayerSkillInfo->FindSkillInfo("VoidPressure");

	if (Info && Info->Active)
	{
		float Area = (Info->CoolTime - Info->AccTime) / Info->CoolTime;
		m_VoidPressureProgressBar->SetPercent(Area);
	}

	Info = PlayerSkillInfo->FindSkillInfo("DeathSide");

	if (Info && Info->Active)
	{
		float Area = (Info->CoolTime - Info->AccTime) / Info->CoolTime;
		m_DeathSideProgressBar->SetPercent(Area);
	}
}

void CSkillQuickSlotWindow::Render()
{
	CWidgetWindow::Render();
}

CSkillQuickSlotWindow* CSkillQuickSlotWindow::Clone()
{
	return new CSkillQuickSlotWindow(*this);
}

void CSkillQuickSlotWindow::RegisterItem(const std::string& Name, const std::string& RegisterKey, int Row, int Column, int Count)
{
	QuickSlotItemState* ItState = new QuickSlotItemState;

	ItState->Count = Count;
	ItState->Name = Name;
	ItState->RegisterKey = RegisterKey;

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("UI/Inventory/ItemNo.%d.png"), i);

		vecFileName.push_back(FileName);
	}

	CNumber* QuickSlotItemNumber = CreateWidget<CNumber>("QuickSlotItemNumber");
	QuickSlotItemNumber->SetTexture("QuickSlotItemNumber", vecFileName);
	//ItemNumber->SetSize(7.f, 9.f);
	QuickSlotItemNumber->SetPos(3.f + Column * m_SlotSize.x + Column * m_Offset.x + 3.f, 3.f + Row * m_SlotSize.y + Row * m_Offset.y + 1.f);
	QuickSlotItemNumber->SetNumber(Count);
	QuickSlotItemNumber->SetZOrder(7);

	ItState->ItemCountWidget = QuickSlotItemNumber;


	m_vecQuickSlotItem.push_back(ItState);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

}

void CSkillQuickSlotWindow::ConsumeItem(const std::string& Name, int ConsumeCount)
{
	size_t Count = m_vecQuickSlotItem.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecQuickSlotItem[i]->Name == Name)
		{
			m_vecQuickSlotItem[i]->Count = m_vecQuickSlotItem[i]->Count - ConsumeCount;
			
			if (m_vecQuickSlotItem[i]->Count < 0)
				m_vecQuickSlotItem[i]->Count = 0;
		}
	}
}

QuickSlotItemState* CSkillQuickSlotWindow::FindRegisterItem(const std::string& RegisterKey)
{
	size_t Count = m_vecQuickSlotItem.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecQuickSlotItem[i]->RegisterKey == RegisterKey)
			return m_vecQuickSlotItem[i];
	}

	return nullptr;
}
