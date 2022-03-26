
#include "PlayerName.h"
#include "../Object/Player2D.h"

CPlayerName::CPlayerName()
{
}

CPlayerName::CPlayerName(const CPlayerName& window)	:
	CWidgetWindow(window)
{
}

CPlayerName::~CPlayerName()
{
}

void CPlayerName::Start()
{
	CWidgetWindow::Start();
}

bool CPlayerName::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	CPlayer2D* Player = nullptr;
	TCHAR PlayerName[256] = {};

	if (m_OwnerComponent)
	{
		Player = (CPlayer2D*)m_OwnerComponent->GetGameObject();
	}

	else
	{
		wcscpy_s(PlayerName, TEXT("NoName"));
	}

	m_PlayerNameBack = CreateWidget<CImage>("BlackFrame");
	m_PlayerNameBack->SetPos(4.f, 17.f);
	m_PlayerNameBack->SetSize(72.f, 15.f);
	m_PlayerNameBack->SetTexture("BlackFrame", TEXT("UI/BlackBar.png"));
	m_PlayerNameBack->SetMouseCollisionEnable(true);



	m_PlayerName = CreateWidget<CText>("PlayerNameWidget");
	m_PlayerName->SetSize(80.f, 50.f);
	m_PlayerName->SetFontSize(50.f);
	m_PlayerName->SetText(Player->GetInfo().Name);
	m_PlayerName->SetPos(0.f, 0.f);
	m_PlayerName->SetFontSize(10.f);
	m_PlayerName->SetColor(1.f, 1.f, 1.f);
	m_PlayerName->SetOpacity(1.f);
	m_PlayerName->SetZOrder(2);
	m_PlayerName->SetAlignH(TEXT_ALIGN_H::Center);
	m_PlayerName->SetAlphaEnable(true);
	m_PlayerName->SetFont("Thin");

	return true;
}

void CPlayerName::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CPlayerName::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CPlayerName::Render()
{
	CWidgetWindow::Render();
}

CPlayerName* CPlayerName::Clone()
{
	return new CPlayerName(*this);
}
