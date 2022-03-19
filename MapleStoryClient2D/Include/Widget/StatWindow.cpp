
#include "StatWindow.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"

CStatWindow::CStatWindow()
{
}

CStatWindow::CStatWindow(const CStatWindow& window)	:
	CWidgetWindow(window)
{
	m_Background = window.m_Background;
	m_Name = window.m_Name;
	m_Job = window.m_Job;
	m_HP = window.m_HP;
	m_MP = window.m_MP;
	m_HPMax = window.m_HPMax;
	m_MPMax = window.m_MPMax;
	m_HPDelimiter = window.m_HPDelimiter;
	m_MPDelimiter = window.m_MPDelimiter;
	m_STR = window.m_STR;
	m_DEX = window.m_DEX;
	m_INT = window.m_INT;
	m_LUK = window.m_LUK;
}

CStatWindow::~CStatWindow()
{
}

void CStatWindow::SetHP(int Number)
{
	m_HP->SetNumber(Number);
}

void CStatWindow::SetMP(int Number)
{
	m_MP->SetNumber(Number);
}

void CStatWindow::SetHPMax(int Number)
{
	m_HPMax->SetNumber(Number);
}

void CStatWindow::SetMPMax(int Number)
{
	m_MPMax->SetNumber(Number);
}

void CStatWindow::SetAbilityPoint(int Number)
{
	m_AbilityPoint->SetNumber(Number);
}

void CStatWindow::AddAbilityPoint(int Number)
{
	int Num = m_AbilityPoint->GetNumber();
	m_AbilityPoint->SetNumber(Num + Number);
}

void CStatWindow::Start()
{
	CWidgetWindow::Start();
}

bool CStatWindow::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(212.f, 336.f);

	m_Background = CreateWidget<CImage>("StatWindowBackground");
	m_Background->SetTexture("StatWindowBackground", TEXT("UI/Status/StatWindow.png"));
	m_Background->SetSize(212.f, 336.f);


	m_HPDelimiter = CreateWidget<CImage>("StatHPDelimiter");
	m_HPDelimiter->SetTexture("StatHPDelimiter", TEXT("UI/Status/StatDelimiter.png"));
	m_HPDelimiter->SetPos(125.f, 187.f);
	m_HPDelimiter->SetSize(6.f, 10.f);

	m_MPDelimiter = CreateWidget<CImage>("StatMPDelimiter");
	m_MPDelimiter->SetTexture("StatMPDelimiter", TEXT("UI/Status/StatDelimiter.png"));
	m_MPDelimiter->SetPos(125.f, 167.f);
	m_MPDelimiter->SetSize(6.f, 10.f);

	
	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		PlayerInfo Info = Player->GetInfo();

		m_Name = CreateWidget<CImage>("StatName");
		m_Name->SetTexture("StatName", TEXT("UI/Status/CharacterName.png"));
		m_Name->SetPos(73.f, 295.f);
		m_Name->SetSize(63.f, 10.f);

		m_Job = CreateWidget<CImage>("StatJob");
		m_Job->SetTexture("StatJob", TEXT("UI/Status/Luminus.png"));
		m_Job->SetPos(73.f, 277.f);
		m_Job->SetSize(42.f, 10.f);


		std::vector<TCHAR*> vecFileName;

		for (int i = 0; i < 10; ++i)
		{
			TCHAR* FileName = new TCHAR[MAX_PATH];
			memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

			wsprintf(FileName, TEXT("UI/Status/StatNumber%d.png"), i);

			vecFileName.push_back(FileName);
		}

		m_HP = CreateWidget<CNumber>("StatHP");
		m_HP->SetTexture("StatHP", vecFileName);
		m_HP->SetNumber(Info.HP);
		m_HP->SetPos(m_HPDelimiter->GetWidgetPos().x - 30.f, 187.f);
		m_HP->SetSize(7.f, 9.f);

		m_HPMax = CreateWidget<CNumber>("StatHPMax");
		m_HPMax->SetTexture("StatHPMax", vecFileName);
		m_HPMax->SetNumber(Info.HPMax);
		m_HPMax->SetPos(m_HPDelimiter->GetWidgetPos().x + 15.f, 187.f);
		m_HPMax->SetSize(7.f, 9.f);

		m_MP = CreateWidget<CNumber>("StatMP");
		m_MP->SetTexture("StatMP", vecFileName);
		m_MP->SetNumber(Info.MP);
		m_MP->SetPos(m_MPDelimiter->GetWidgetPos().x - 35.f, 168.f);
		m_MP->SetSize(7.f, 9.f);

		m_MPMax = CreateWidget<CNumber>("StatMPMax");
		m_MPMax->SetTexture("StatMPMax", vecFileName);
		m_MPMax->SetNumber(Info.MPMax);
		m_MPMax->SetPos(m_MPDelimiter->GetWidgetPos().x + 15.f, 168.f);
		m_MPMax->SetSize(7.f, 9.f);

		m_STR = CreateWidget<CNumber>("StatStr");
		m_STR->SetTexture("StatStr", vecFileName);
		m_STR->SetNumber(Info.STR);
		m_STR->SetPos(80.f, 100.f);
		m_STR->SetSize(7.f, 9.f);

		m_DEX = CreateWidget<CNumber>("StatDex");
		m_DEX->SetTexture("StatDex", vecFileName);
		m_DEX->SetNumber(Info.DEX);
		m_DEX->SetPos(80.f, 82.f);
		m_DEX->SetSize(7.f, 9.f);

		m_INT = CreateWidget<CNumber>("StatInt");
		m_INT->SetTexture("StatInt", vecFileName);
		m_INT->SetNumber(Info.INT);
		m_INT->SetPos(80.f, 64.f);
		m_INT->SetSize(7.f, 9.f);

		m_LUK = CreateWidget<CNumber>("StatLuk");
		m_LUK->SetTexture("StatLuk", vecFileName);
		m_LUK->SetNumber(Info.LUK);
		m_LUK->SetPos(80.f, 46.f);
		m_LUK->SetSize(7.f, 9.f);

		m_AbilityPoint = CreateWidget<CNumber>("StatAbilityPoint");
		m_AbilityPoint->SetTexture("StatAbilityPoint", vecFileName);
		m_AbilityPoint->SetNumber(0);
		m_AbilityPoint->SetPos(80.f, 127.f);
		m_AbilityPoint->SetSize(7.f, 9.f);

		for (int i = 0; i < 10; ++i)
		{
			SAFE_DELETE_ARRAY(vecFileName[i]);
		}

		m_AutoAllocate = CreateWidget<CButton>("AutoAllocateButton");
		m_AutoAllocate->SetZOrder(1);
		m_AutoAllocate->SetSize(67.f, 34.f);
		m_AutoAllocate->SetPos(127.f, 123.f);
		m_AutoAllocate->SetTexture(Button_State::Normal, "AutoAllocateButtonNormal", TEXT("UI/Status/Stat.main.BtAuto.normal.3.png"));
		m_AutoAllocate->SetTexture(Button_State::MouseOn, "AutoAllocateButtonMouseOn", TEXT("UI/Status/Stat.main.BtAuto.mouseOver.0.png"));
		m_AutoAllocate->SetTexture(Button_State::Click, "AutoAllocateButtonClick", TEXT("UI/Status/Stat.main.BtAuto.normal.0.png"));

		m_AutoAllocate->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
		m_AutoAllocate->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

		m_AutoAllocate->SetClickCallback<CStatWindow>(this, &CStatWindow::AutoAllocateButton);


		m_STRUp = CreateWidget<CButton>("UpSTRButton");
		m_STRUp->SetZOrder(1);
		m_STRUp->SetSize(12.f, 12.f);
		m_STRUp->SetPos(137.f, 99.f);
		m_STRUp->SetTexture(Button_State::Normal, "UPNormal", TEXT("UI/Status/Stat.main.BtHpUp.normal.0.png"));
		m_STRUp->SetTexture(Button_State::MouseOn, "UPMouseOn", TEXT("UI/Status/StatUpMouseOver.png"));
		m_STRUp->SetTexture(Button_State::Click, "UPClick", TEXT("UI/Status/Stat.main.BtHpUp.pressed.0.png"));

		m_STRUp->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
		m_STRUp->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

		m_STRUp->SetClickCallback<CStatWindow>(this, &CStatWindow::UpSTRButton);

		m_DEXUp = CreateWidget<CButton>("UpDEXButton");
		m_DEXUp->SetZOrder(1);
		m_DEXUp->SetSize(12.f, 12.f);
		m_DEXUp->SetPos(137.f, 81.f);
		m_DEXUp->SetTexture(Button_State::Normal, "UPNormal", TEXT("UI/Status/Stat.main.BtHpUp.normal.0.png"));
		m_DEXUp->SetTexture(Button_State::MouseOn, "UPMouseOn", TEXT("UI/Status/StatUpMouseOver.png"));
		m_DEXUp->SetTexture(Button_State::Click, "UPClick", TEXT("UI/Status/Stat.main.BtHpUp.pressed.0.png"));

		m_DEXUp->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
		m_DEXUp->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

		m_DEXUp->SetClickCallback<CStatWindow>(this, &CStatWindow::UpDEXButton);

		m_INTUp = CreateWidget<CButton>("UpINTButton");
		m_INTUp->SetZOrder(1);
		m_INTUp->SetSize(12.f, 12.f);
		m_INTUp->SetPos(137.f, 63.f);
		m_INTUp->SetTexture(Button_State::Normal, "UPNormal", TEXT("UI/Status/Stat.main.BtHpUp.normal.0.png"));
		m_INTUp->SetTexture(Button_State::MouseOn, "UPMouseOn", TEXT("UI/Status/StatUpMouseOver.png"));
		m_INTUp->SetTexture(Button_State::Click, "UPClick", TEXT("UI/Status/Stat.main.BtHpUp.pressed.0.png"));

		m_INTUp->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
		m_INTUp->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

		m_INTUp->SetClickCallback<CStatWindow>(this, &CStatWindow::UpINTButton);

		m_LUKUp = CreateWidget<CButton>("UpLUKButton");
		m_LUKUp->SetZOrder(1);
		m_LUKUp->SetSize(12.f, 12.f);
		m_LUKUp->SetPos(137.f, 45.f);
		m_LUKUp->SetTexture(Button_State::Normal, "UPNormal", TEXT("UI/Status/Stat.main.BtHpUp.normal.0.png"));
		m_LUKUp->SetTexture(Button_State::MouseOn, "UPMouseOn", TEXT("UI/Status/StatUpMouseOver.png"));
		m_LUKUp->SetTexture(Button_State::Click, "UPClick", TEXT("UI/Status/Stat.main.BtHpUp.pressed.0.png"));

		m_LUKUp->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
		m_LUKUp->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

		m_LUKUp->SetClickCallback<CStatWindow>(this, &CStatWindow::UpLUKButton);

		vecFileName.clear();
	}

	return true;
}

void CStatWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
	
}

void CStatWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CStatWindow::Render()
{
	CWidgetWindow::Render();
}

CStatWindow* CStatWindow::Clone()
{
	return new CStatWindow(*this);
}

void CStatWindow::AutoAllocateButton()
{
	int AbilityNumber = m_AbilityPoint->GetNumber();

	if (AbilityNumber <= 0)
		return;

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		Player->UpINT(AbilityNumber);
	}

	int Number = m_INT->GetNumber();
	m_INT->SetNumber(Number + AbilityNumber);

	m_AbilityPoint->SetNumber(0);
}

void CStatWindow::UpSTRButton()
{
	int AbilityNumber = m_AbilityPoint->GetNumber();

	if (AbilityNumber <= 0)
		return;

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		Player->UpSTR(1);
	}

	int Number = m_STR->GetNumber();
	m_STR->SetNumber(Number + 1);

	m_AbilityPoint->SetNumber(AbilityNumber - 1);
}

void CStatWindow::UpDEXButton()
{
	int AbilityNumber = m_AbilityPoint->GetNumber();

	if (AbilityNumber <= 0)
		return;

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		Player->UpDEX(1);
	}

	int Number = m_DEX->GetNumber();
	m_DEX->SetNumber(Number + 1);

	m_AbilityPoint->SetNumber(AbilityNumber - 1);
}

void CStatWindow::UpINTButton()
{
	int AbilityNumber = m_AbilityPoint->GetNumber();

	if (AbilityNumber <= 0)
		return;

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		Player->UpINT(1);
	}

	int Number = m_INT->GetNumber();
	m_INT->SetNumber(Number + 1);

	m_AbilityPoint->SetNumber(AbilityNumber - 1);
}

void CStatWindow::UpLUKButton()
{
	int AbilityNumber = m_AbilityPoint->GetNumber();

	if (AbilityNumber <= 0)
		return;

	CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

	if (Player)
	{
		Player->UpLUK(1);
	}

	int Number = m_LUK->GetNumber();
	m_LUK->SetNumber(Number + 1);

	m_AbilityPoint->SetNumber(AbilityNumber - 1);
}
