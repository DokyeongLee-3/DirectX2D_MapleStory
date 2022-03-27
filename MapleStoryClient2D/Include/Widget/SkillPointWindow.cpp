
#include "SkillPointWindow.h"
#include "Scene/Scene.h"
#include "Widget/Button.h"
#include "../Object/Player2D.h"
#include "../Object/PlayerSkillInfo.h"

CSkillPointWindow::CSkillPointWindow()
{
	SetTypeID<CSkillPointWindow>();
}

CSkillPointWindow::CSkillPointWindow(const CSkillPointWindow& window)	:
	CWidgetWindow(window)
{
}

CSkillPointWindow::~CSkillPointWindow()
{
	size_t Count = m_vecFirstGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_vecFirstGradeSkillPointInfo[i]);
	}

	Count = m_vecSecondGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_vecSecondGradeSkillPointInfo[i]);
	}

	Count = m_vecThirdGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_vecThirdGradeSkillPointInfo[i]);
	}
}

void CSkillPointWindow::Start()
{
	CWidgetWindow::Start();
}

bool CSkillPointWindow::Init()
{
	SetPos(500.f, 300.f);
	SetSize(318.f, 361.f);

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("UI/Inventory/StatNumber%d.png"), i);

		vecFileName.push_back(FileName);
	}


	m_Title = CreateWidget<CText>("SkillPointWindowTitle");
	m_Title->SetSize(250.f, 40.f);
	m_Title->SetPos(50.f, 273.f);
	m_Title->SetFontSize(13.f);
	m_Title->SetZOrder(2);
	m_Title->SetColor(1.f, 1.f, 1.f);
	m_Title->SetAlignH(TEXT_ALIGN_H::Center);
	m_Title->SetFont("Thin");
	m_Title->SetText(TEXT("루미너스"));

	m_SkillPointWindowBack = CreateWidget<CImage>("SkillPointWindowBack");
	m_SkillPointWindowBack->SetTexture("SkillPointWindowBack", TEXT("UI/Skill/SkillPointWindowBack.png"));
	m_SkillPointWindowBack->SetSize(318.f, 361.f);
	m_SkillPointWindowBack->SetMouseCollisionEnable(true);

	m_ZeroGradeSkillTab = CreateWidget<CImage>("ZeroGradeSkillTab");
	m_ZeroGradeSkillTab->SetTexture("ZeroGradeSkillTab", TEXT("UI/Skill/Skill.main.Tab.disabled.0.png"));
	m_ZeroGradeSkillTab->SetSize(25.f, 18.f);
	m_ZeroGradeSkillTab->SetMouseCollisionEnable(true);
	m_ZeroGradeSkillTab->SetPos(15.f, 315.f);

	m_FirstGradeSkillTab = CreateWidget<CImage>("FirstGradeSkillTabEnable");
	m_FirstGradeSkillTab->SetTexture("FirstGradeSkillTab", TEXT("UI/Skill/Skill.main.Tab.enabled.1.png"));
	m_FirstGradeSkillTab->SetSize(25.f, 20.f);
	m_FirstGradeSkillTab->SetMouseCollisionEnable(true);
	m_FirstGradeSkillTab->SetPos(43.f, 315.f);
	m_FirstGradeSkillTab->SetClickCallback<CSkillPointWindow>(this, &CSkillPointWindow::ClickFirstGradeSkillTab);

	m_SecondGradeSkillTab = CreateWidget<CImage>("SecondGradeSkillTabDisable");
	m_SecondGradeSkillTab->SetTexture("SecondGradeSkillTab", TEXT("UI/Skill/Skill.main.Tab.disabled.2.png"));
	m_SecondGradeSkillTab->SetSize(25.f, 18.f);
	m_SecondGradeSkillTab->SetMouseCollisionEnable(true);
	m_SecondGradeSkillTab->SetPos(71.f, 315.f);
	m_SecondGradeSkillTab->SetClickCallback<CSkillPointWindow>(this, &CSkillPointWindow::ClickSecondGradeSkillTab);

	m_ThirdGradeSkillTab = CreateWidget<CImage>("ThirdGradeSkillTabDisable");
	m_ThirdGradeSkillTab->SetTexture("ThirdGradeSkillTab", TEXT("UI/Skill/Skill.main.Tab.disabled.3.png"));
	m_ThirdGradeSkillTab->SetSize(25.f, 18.f);
	m_ThirdGradeSkillTab->SetMouseCollisionEnable(true);
	m_ThirdGradeSkillTab->SetPos(99.f, 315.f);
	m_ThirdGradeSkillTab->SetClickCallback<CSkillPointWindow>(this, &CSkillPointWindow::ClickThirdGradeSkillTab);

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();
	CPlayerSkillInfo* PlayerSkillInfo = nullptr;

	if (Player)
	{
		PlayerSkillInfo = Player->GetPlayerSkillInfo();
	}

	m_EnableSkillPoint = CreateWidget<CNumber>("EnableSkillPoint");
	m_EnableSkillPoint->SetTexture("EnableSkillPoint", vecFileName);
	m_EnableSkillPoint->SetNumber(0);
	m_EnableSkillPoint->SetPos(290.f, 319.f);
	m_EnableSkillPoint->SetSize(7.f, 9.f);


	// 1차 스킬
	SkillPointInfo* Info = new SkillPointInfo;

	Info->SkillIcon = CreateWidget<CImage>("LightTransforming");
	Info->SkillIcon->SetTexture("LightTransformingIcon", TEXT("UI/Skill/27001002.icon.png"));
	Info->SkillIcon->SetSize(32.f, 32.f);
	Info->SkillIcon->SetPos(14.f, 230.f);

	Info->SkillName = CreateWidget<CText>("LightTransformingName");
	Info->SkillName->SetSize(120.f, 40.f);
	Info->SkillName->SetPos(40.f, 237.f);
	Info->SkillName->SetFontSize(11.f);
	Info->SkillName->SetZOrder(2);
	Info->SkillName->SetColor(0.f, 0.f, 0.f);
	Info->SkillName->SetAlignH(TEXT_ALIGN_H::Center);
	Info->SkillName->SetFont("Thin");
	Info->SkillName->SetText(TEXT("라이트 트랜스포밍"));

	Info->SkillLevel = CreateWidget<CNumber>("LightTransformingLevel");
	Info->SkillLevel->SetTexture("LightTransformingLevel", vecFileName);

	if (PlayerSkillInfo)
	{
		SkillInfo* SkillInfo1 = PlayerSkillInfo->FindSkillInfo("LightTransforming");
		int Lv = SkillInfo1->Level;
		Info->SkillLevel->SetNumber(Lv);
	}

	else
		Info->SkillLevel->SetNumber(-1);

	Info->SkillLevel->SetPos(55.f, 232.f);
	Info->SkillLevel->SetSize(7.f, 9.f);

	Info->SkillLevelUpButton = CreateWidget<CButton>("LightTransformingLvUpButton");

	if (Info->SkillLevel->GetNumber() == 20 || m_EnableSkillPoint->GetNumber() == 0)
		Info->SkillLevelUpButton->ButtonEnable(false);

	else
		Info->SkillLevelUpButton->ButtonEnable(true);


	Info->SkillLevelUpButton->SetTexture(Button_State::Disable, "LightTransformingLvUpButtonDisable", TEXT("UI/Skill/Skill.main.BtSpUp.disabled.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Normal, "LightTransformingLvUpButtonNormal", TEXT("UI/Skill/Skill.main.BtSpUp.normal.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::MouseOn, "LightTransformingLvUpButtonMouseOn", TEXT("UI/Skill/Skill.main.BtSpUp.mouseOver.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Click, "LightTransformingLvUpButtonClick", TEXT("UI/Skill/Skill.main.BtSpUp.pressed.0.png"));

	Info->SkillLevelUpButton->SetSize(14.f, 12.f);
	Info->SkillLevelUpButton->SetPos(135.f, 231.f);
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_vecFirstGradeSkillPointInfo.push_back(Info);

	// 2차 스킬 실피드랜서

	Info = new SkillPointInfo;

	Info->SkillIcon = CreateWidget<CImage>("SylphideLancerIcon");
	Info->SkillIcon->SetTexture("SylphideLancerIcon", TEXT("UI/Skill/27101100.icon.png"));
	Info->SkillIcon->SetSize(32.f, 32.f);
	Info->SkillIcon->SetPos(14.f, 230.f);

	Info->SkillName = CreateWidget<CText>("SylphideLancerName");
	Info->SkillName->SetSize(120.f, 40.f);
	Info->SkillName->SetPos(40.f, 237.f);
	Info->SkillName->SetFontSize(11.f);
	Info->SkillName->SetZOrder(2);
	Info->SkillName->SetColor(0.f, 0.f, 0.f);
	Info->SkillName->SetAlignH(TEXT_ALIGN_H::Center);
	Info->SkillName->SetFont("Thin");
	Info->SkillName->SetText(TEXT("실피드 랜서"));

	Info->SkillLevel = CreateWidget<CNumber>("SylphideLancerLevel");
	Info->SkillLevel->SetTexture("SylphideLancerLevel", vecFileName);

	if (PlayerSkillInfo)
	{
		SkillInfo* SkillInfo1 = PlayerSkillInfo->FindSkillInfo("SylphideLancer");
		int Lv = SkillInfo1->Level;
		Info->SkillLevel->SetNumber(Lv);
	}

	else
		Info->SkillLevel->SetNumber(-1);

	Info->SkillLevel->SetPos(55.f, 232.f);
	Info->SkillLevel->SetSize(7.f, 9.f);

	Info->SkillLevelUpButton = CreateWidget<CButton>("SylphideLancerLvUpButton");
	if (Info->SkillLevel->GetNumber() == 20 || m_EnableSkillPoint->GetNumber() == 0)
		Info->SkillLevelUpButton->ButtonEnable(false);

	else
		Info->SkillLevelUpButton->ButtonEnable(true);


	Info->SkillLevelUpButton->SetTexture(Button_State::Normal, "SylphideLancerLvUpButtonNormal", TEXT("UI/Skill/Skill.main.BtSpUp.normal.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::MouseOn, "SylphideLancerLvUpButtonMouseOn", TEXT("UI/Skill/Skill.main.BtSpUp.mouseOver.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Click, "SylphideLancerLvUpButtonClick", TEXT("UI/Skill/Skill.main.BtSpUp.pressed.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Disable, "SylphideLancerLvUpButtonDisable", TEXT("UI/Skill/Skill.main.BtSpUp.disabled.0.png"));

	Info->SkillLevelUpButton->SetSize(14.f, 12.f);
	Info->SkillLevelUpButton->SetPos(135.f, 231.f);
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_vecSecondGradeSkillPointInfo.push_back(Info);

	// 2차스킬 보이드프레셔
	Info = new SkillPointInfo;

	Info->SkillIcon = CreateWidget<CImage>("VoidPressureIcon");
	Info->SkillIcon->SetTexture("VoidPressureIcon", TEXT("UI/Skill/27101202.icon.png"));
	Info->SkillIcon->SetSize(32.f, 32.f);
	Info->SkillIcon->SetPos(160.f, 230.f);

	Info->SkillName = CreateWidget<CText>("VoidPressureName");
	Info->SkillName->SetSize(120.f, 40.f);
	Info->SkillName->SetPos(188.f, 237.f);
	Info->SkillName->SetFontSize(11.f);
	Info->SkillName->SetZOrder(2);
	Info->SkillName->SetColor(0.f, 0.f, 0.f);
	Info->SkillName->SetAlignH(TEXT_ALIGN_H::Center);
	Info->SkillName->SetFont("Thin");
	Info->SkillName->SetText(TEXT("보이드 프레셔"));

	Info->SkillLevel = CreateWidget<CNumber>("VoidPressureLevel");
	Info->SkillLevel->SetTexture("VoidPressureLevel", vecFileName);

	if (PlayerSkillInfo)
	{
		SkillInfo* SkillInfo1 = PlayerSkillInfo->FindSkillInfo("VoidPressure");
		int Lv = SkillInfo1->Level;
		Info->SkillLevel->SetNumber(Lv);
	}

	else
		Info->SkillLevel->SetNumber(-1);

	Info->SkillLevel->SetPos(203.f, 232.f);
	Info->SkillLevel->SetSize(7.f, 9.f);

	Info->SkillLevelUpButton = CreateWidget<CButton>("VoidPressureLvUpButton");
	if (Info->SkillLevel->GetNumber() == 20 || m_EnableSkillPoint->GetNumber() == 0)
		Info->SkillLevelUpButton->ButtonEnable(false);

	else
		Info->SkillLevelUpButton->ButtonEnable(true);

	Info->SkillLevelUpButton->SetTexture(Button_State::Normal, "VoidPressureLvUpButtonNormal", TEXT("UI/Skill/Skill.main.BtSpUp.normal.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::MouseOn, "VoidPressureLvUpButtonMouseOn", TEXT("UI/Skill/Skill.main.BtSpUp.mouseOver.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Click, "VoidPressureLvUpButtonClick", TEXT("UI/Skill/Skill.main.BtSpUp.pressed.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Disable, "VoidPressureLvUpButtonDisable", TEXT("UI/Skill/Skill.main.BtSpUp.disabled.0.png"));

	Info->SkillLevelUpButton->SetSize(14.f, 12.f);
	Info->SkillLevelUpButton->SetPos(282.f, 231.f);
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_vecSecondGradeSkillPointInfo.push_back(Info);

	// 3차 스킬 데스 사이드

	Info = new SkillPointInfo;

	Info->SkillIcon = CreateWidget<CImage>("DeathSideIcon");
	Info->SkillIcon->SetTexture("DeathSideIcon", TEXT("UI/Skill/27111303.icon.png"));
	Info->SkillIcon->SetSize(32.f, 32.f);
	Info->SkillIcon->SetPos(14.f, 230.f);

	Info->SkillName = CreateWidget<CText>("DeathSideName");
	Info->SkillName->SetSize(120.f, 40.f);
	Info->SkillName->SetPos(40.f, 237.f);
	Info->SkillName->SetFontSize(11.f);
	Info->SkillName->SetZOrder(2);
	Info->SkillName->SetColor(0.f, 0.f, 0.f);
	Info->SkillName->SetAlignH(TEXT_ALIGN_H::Center);
	Info->SkillName->SetFont("Thin");
	Info->SkillName->SetText(TEXT("데스 사이드"));

	Info->SkillLevel = CreateWidget<CNumber>("DeathSideLevel");
	Info->SkillLevel->SetTexture("DeathSideLevel", vecFileName);

	if (PlayerSkillInfo)
	{
		SkillInfo* SkillInfo1 = PlayerSkillInfo->FindSkillInfo("DeathSide");
		int Lv = SkillInfo1->Level;
		Info->SkillLevel->SetNumber(Lv);
	}

	else
		Info->SkillLevel->SetNumber(-1);

	Info->SkillLevel->SetPos(55.f, 232.f);
	Info->SkillLevel->SetSize(7.f, 9.f);

	Info->SkillLevelUpButton = CreateWidget<CButton>("DeathSideLvUpButton");
	if (Info->SkillLevel->GetNumber() == 20 || m_EnableSkillPoint->GetNumber() == 0)
		Info->SkillLevelUpButton->ButtonEnable(false);

	else
		Info->SkillLevelUpButton->ButtonEnable(true);

	Info->SkillLevelUpButton->SetTexture(Button_State::Disable, "DeathSideLvUpButtonDisable", TEXT("UI/Skill/Skill.main.BtSpUp.disabled.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Normal, "DeathSideLvUpButtonNormal", TEXT("UI/Skill/Skill.main.BtSpUp.normal.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::MouseOn, "DeathSideLvUpButtonMouseOn", TEXT("UI/Skill/Skill.main.BtSpUp.mouseOver.0.png"));
	Info->SkillLevelUpButton->SetTexture(Button_State::Click, "DeathSideLvUpButtonClick", TEXT("UI/Skill/Skill.main.BtSpUp.pressed.0.png"));

	Info->SkillLevelUpButton->SetSize(14.f, 12.f);
	Info->SkillLevelUpButton->SetPos(135.f, 231.f);
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	Info->SkillLevelUpButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");
	Info->SkillLevelUpButton->SetClickCallback<CSkillPointWindow>(this, &CSkillPointWindow::LevelUpDeathSideSkill);

	m_vecThirdGradeSkillPointInfo.push_back(Info);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	m_CurrentOpenTabNum = 1;

	// 지금 1차 스킬 탭 열린 상태니까 2차, 3차 스킬 관련 위젯들은 전부 Enable = false처리
	size_t Count = m_vecSecondGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecSecondGradeSkillPointInfo[i]->SkillIcon->Enable(false);
		m_vecSecondGradeSkillPointInfo[i]->SkillLevel->Enable(false);
		m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
		m_vecSecondGradeSkillPointInfo[i]->SkillName->Enable(false);
	}

	Count = m_vecThirdGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecThirdGradeSkillPointInfo[i]->SkillIcon->Enable(false);
		m_vecThirdGradeSkillPointInfo[i]->SkillLevel->Enable(false);
		m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
		m_vecThirdGradeSkillPointInfo[i]->SkillName->Enable(false);
	}

	return true;
}

void CSkillPointWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	int CurrentEnablePoint = m_EnableSkillPoint->GetNumber();
	if (CurrentEnablePoint == 0)
	{
		switch (m_CurrentOpenTabNum)
		{
		case 1:
		{
			size_t Count = m_vecFirstGradeSkillPointInfo.size();

			for (size_t i = 0; i < Count; ++i)
			{
				m_vecFirstGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(false);
			}
		}
			break;
		case 2:
		{
			size_t Count = m_vecSecondGradeSkillPointInfo.size();

			for (size_t i = 0; i < Count; ++i)
			{
				m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(false);
			}
		}
			break;
		case 3:
		{
			size_t Count = m_vecThirdGradeSkillPointInfo.size();

			for (size_t i = 0; i < Count; ++i)
			{
				m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(false);
			}
		}
			break;
		}
	}
}

void CSkillPointWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CSkillPointWindow::Render()
{
	CWidgetWindow::Render();
}

CSkillPointWindow* CSkillPointWindow::Clone()
{
	return new CSkillPointWindow(*this);
}

void CSkillPointWindow::ClickFirstGradeSkillTab()
{
	if (m_CurrentOpenTabNum == 1)
		return;

	m_FirstGradeSkillTab->SetTexture("FirstGradeSkillTabEnable", TEXT("UI/Skill/Skill.main.Tab.enabled.1.png"));
	m_FirstGradeSkillTab->SetSize(25.f, 20.f);
	m_FirstGradeSkillTab->SetClicked(false);

	m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

	switch (m_CurrentOpenTabNum)
	{
	case 2: 
	{
		m_SecondGradeSkillTab->SetTexture("SecondGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.2.png"));
		m_SecondGradeSkillTab->SetSize(25.f, 18.f);
		m_SecondGradeSkillTab->SetClicked(false);

		size_t Count = m_vecSecondGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecSecondGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillName->Enable(false);
		}
	}
		break;
	case 3:
	{
		m_ThirdGradeSkillTab->SetTexture("ThirdGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.3.png"));
		m_ThirdGradeSkillTab->SetSize(25.f, 18.f);
		m_ThirdGradeSkillTab->SetClicked(false);

		size_t Count = m_vecThirdGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecThirdGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillName->Enable(false);
		}
	}
		break;
	default:
		break;
	}

	size_t Count = m_vecFirstGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecFirstGradeSkillPointInfo[i]->SkillIcon->Enable(true);
		m_vecFirstGradeSkillPointInfo[i]->SkillLevel->Enable(true);
		m_vecFirstGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(true);
		m_vecFirstGradeSkillPointInfo[i]->SkillName->Enable(true);
	}

	m_CurrentOpenTabNum = 1;
}

void CSkillPointWindow::ClickSecondGradeSkillTab()
{
	if (m_CurrentOpenTabNum == 2)
		return;

	m_SecondGradeSkillTab->SetTexture("SecondGradeSkillTabEnable", TEXT("UI/Skill/Skill.main.Tab.enabled.2.png"));
	m_SecondGradeSkillTab->SetSize(25.f, 20.f);
	m_SecondGradeSkillTab->SetClicked(false);

	m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

	switch (m_CurrentOpenTabNum)
	{
	case 1:
	{
		m_FirstGradeSkillTab->SetTexture("FirstGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.1.png"));
		m_FirstGradeSkillTab->SetSize(25.f, 18.f);
		m_FirstGradeSkillTab->SetClicked(false);

		size_t Count = m_vecFirstGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecFirstGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillName->Enable(false);
		}
	}
		break;
	case 3:
	{
		m_ThirdGradeSkillTab->SetTexture("ThirdGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.3.png"));
		m_ThirdGradeSkillTab->SetSize(25.f, 18.f);
		m_ThirdGradeSkillTab->SetClicked(false);

		size_t Count = m_vecThirdGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecThirdGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecThirdGradeSkillPointInfo[i]->SkillName->Enable(false);
		}

	}
		break;
	default:
		break;
	}

	size_t Count = m_vecSecondGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecSecondGradeSkillPointInfo[i]->SkillIcon->Enable(true);
		m_vecSecondGradeSkillPointInfo[i]->SkillLevel->Enable(true);
		m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(true);
		m_vecSecondGradeSkillPointInfo[i]->SkillName->Enable(true);
	}

	m_CurrentOpenTabNum = 2;
}

void CSkillPointWindow::ClickThirdGradeSkillTab()
{
	if (m_CurrentOpenTabNum == 3)
		return;

	m_ThirdGradeSkillTab->SetTexture("ThirdGradeSkillTabEnable", TEXT("UI/Skill/Skill.main.Tab.enabled.3.png"));
	m_ThirdGradeSkillTab->SetSize(25.f, 20.f);
	m_ThirdGradeSkillTab->SetClicked(false);

	m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

	switch (m_CurrentOpenTabNum)
	{
	case 1:
	{
		m_FirstGradeSkillTab->SetTexture("FirstGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.1.png"));
		m_FirstGradeSkillTab->SetSize(25.f, 18.f);
		m_FirstGradeSkillTab->SetClicked(false);

		size_t Count = m_vecFirstGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecFirstGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecFirstGradeSkillPointInfo[i]->SkillName->Enable(false);
		}
	}
		break;
	case 2:
	{
		m_SecondGradeSkillTab->SetTexture("SecondGradeSkillTabDisable", TEXT("UI/Skill/Skill.main.Tab.disabled.2.png"));
		m_SecondGradeSkillTab->SetSize(25.f, 18.f);
		m_SecondGradeSkillTab->SetClicked(false);

		size_t Count = m_vecSecondGradeSkillPointInfo.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecSecondGradeSkillPointInfo[i]->SkillIcon->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillLevel->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(false);
			m_vecSecondGradeSkillPointInfo[i]->SkillName->Enable(false);
		}

	}
		break;
	default:
		break;
	}

	size_t Count = m_vecThirdGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecThirdGradeSkillPointInfo[i]->SkillIcon->Enable(true);
		m_vecThirdGradeSkillPointInfo[i]->SkillLevel->Enable(true);
		m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->Enable(true);
		m_vecThirdGradeSkillPointInfo[i]->SkillName->Enable(true);
	}

	m_CurrentOpenTabNum = 3;
}

void CSkillPointWindow::AddEnableSkillPoint(int Num)
{
	m_EnableSkillPoint->AddNumber(Num);
}

void CSkillPointWindow::LevelUpDeathSideSkill()
{
	if (m_CurrentOpenTabNum != 3)
		return;

	size_t Count = m_vecThirdGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		char SkillName[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, m_vecThirdGradeSkillPointInfo[i]->SkillName->GetText(), -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, m_vecThirdGradeSkillPointInfo[i]->SkillName->GetText(), -1, SkillName, ConvertLength, 0, 0);
		std::string strSkillName = SkillName;

		if (strSkillName.find("데스 사이드") != std::string::npos)
		{
			if (m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->GetButtonState() == Button_State::Disable)
				return;

			m_vecThirdGradeSkillPointInfo[i]->SkillLevel->AddNumber(1);

			CScene* Scene = m_Viewport->GetScene();

			if (!Scene)
				return;

			CPlayer2D* Player = (CPlayer2D*)Scene->GetPlayerObject();

			int CurrentLevel = Player->GetPlayerSkillInfo()->FindSkillInfo("DeathSide")->Level;
			Player->GetPlayerSkillInfo()->FindSkillInfo("DeathSide")->Level = CurrentLevel + 1;
			m_EnableSkillPoint->AddNumber(-1);
		}
	}

}

// 레벨업해서 스킬포인트가 있고, 마스터 안한 스킬 한정으로 스킬레벨 올릴 수 있는 모든 스킬들의 버튼 Enable로 활성화 시켜주기
void CSkillPointWindow::ActivateAllLevelUpButton()
{
	size_t Count = m_vecFirstGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecFirstGradeSkillPointInfo[i]->SkillLevel->GetNumber() != 20)
		{
			m_vecFirstGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(true);
		}
	}

	Count = m_vecSecondGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecSecondGradeSkillPointInfo[i]->SkillLevel->GetNumber() != 20)
		{
			m_vecSecondGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(true);
		}
	}

	Count = m_vecThirdGradeSkillPointInfo.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecThirdGradeSkillPointInfo[i]->SkillLevel->GetNumber() != 20)
		{
			m_vecThirdGradeSkillPointInfo[i]->SkillLevelUpButton->ButtonEnable(true);
		}
	}
}
