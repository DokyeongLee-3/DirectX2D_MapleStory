
#include "AdobisDialog.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/Viewport.h"
#include "../Scene/WayToZakumScene.h"

CAdobisDialog::CAdobisDialog()
{
	m_Contents = TEXT("어느 자쿰에게 받칠 제물이 필요하오?");
}

CAdobisDialog::CAdobisDialog(const CAdobisDialog& window) :
	CWidgetWindow(window)
{
	m_Background = FindWidget<CImage>("AdobisDialogBackground");
}

CAdobisDialog::~CAdobisDialog()
{
}

void CAdobisDialog::Start()
{
	CWidgetWindow::Start();
}

bool CAdobisDialog::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetPos(400.f, 300.f);
	SetSize(519.f, 165.f);

	m_Background = CreateWidget<CImage>("AdobisDialogBackground");
	m_Background->SetSize(519.f, 165.f);
	m_Background->SetTexture("AdobisDialogBackground", TEXT("UI/UtilDlgEx.t.png"));

	m_ChatStopButton = CreateWidget<CButton>("NPCChatStop");
	m_ChatStopButton->SetSize(85.f, 18.f);
	m_ChatStopButton->SetPos(10.f, 10.f);
	m_ChatStopButton->SetTexture(Button_State::Normal, "CharStopButtonNormal", TEXT("UI/UtilDlgEx.BtClose.normal.0.png"));
	m_ChatStopButton->SetTexture(Button_State::MouseOn, "CharStopButtonMouseOn", TEXT("UI/UtilDlgEx.BtClose.mouseOver.0.png"));
	m_ChatStopButton->SetTexture(Button_State::Click, "CharStopButtonClick", TEXT("UI/UtilDlgEx.BtClose.pressed.0.png"));
	m_ChatStopButton->SetClickCallback<CAdobisDialog>(this, &CAdobisDialog::ChatStopCallback);

	m_ChatStopButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_ChatStopButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_ChatContents = CreateWidget<CText>("NPCChat");
	m_ChatContents->SetFont("Thin");
	m_ChatContents->SetSize(200.f, 15.f);
	m_ChatContents->SetPos(165.f, 115.f);
	m_ChatContents->SetFontSize(12.f);
	m_ChatContents->SetColor(0.f, 0.f, 0.f);
	m_ChatContents->SetZOrder(1);
	m_ChatContents->SetAlignH(TEXT_ALIGN_H::Center);
	m_ChatContents->SetText(m_Contents.c_str());

	m_Option1 = CreateWidget<CButton>("Option1");
	m_Option1->SetSize(97.f, 18.f);
	m_Option1->SetPos(160.f, 65.f);
	m_Option1->SetZOrder(1);
	m_Option1->SetTexture(Button_State::Normal, "Option1Normal", TEXT("UI/EasyZakumNotSelected.png"));
	m_Option1->SetTexture(Button_State::MouseOn, "Option1MouseOn", TEXT("UI/EasyZakumSelected.png"));
	m_Option1->SetClickCallback<CAdobisDialog>(this, &CAdobisDialog::EnterZakumMap);

	return true;
}

void CAdobisDialog::Update(float DeltaTime)
{
	if (!m_Enable)
		return;

	CWidgetWindow::Update(DeltaTime);
}

void CAdobisDialog::PostUpdate(float DeltaTime)
{
	if (!m_Enable)
		return;

	CWidgetWindow::PostUpdate(DeltaTime);
}

void CAdobisDialog::Render()
{
	if (!m_Enable)
		return;

	CWidgetWindow::Render();
}

CAdobisDialog* CAdobisDialog::Clone()
{
	return new CAdobisDialog(*this);
}

void CAdobisDialog::ChatStopCallback()
{
	m_Enable = false;
}

void CAdobisDialog::EnterZakumMap()
{
	CSceneMode* Mode = m_Viewport->GetScene()->GetSceneMode();

	if (Mode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
	{
		CScene* Scene = m_Viewport->GetScene();

		if (Scene)
		{
			CPlayer2D* Player = (CPlayer2D*)Scene->GetPlayerObject();

			Player->GotoZakumAltar();
		}

	}
}
