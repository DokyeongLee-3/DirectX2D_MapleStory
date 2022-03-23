
#include "AmonDialog.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/Viewport.h"
#include "../Scene/WayToZakumScene.h"
#include "../Object/NPCAmon.h"

CAmonDialog::CAmonDialog()
{
	m_Contents = TEXT("이곳에서 나가겠는가? 다음번에 들어올 때는 처음부터 다시 도전해야 한다네.");
}

CAmonDialog::CAmonDialog(const CAmonDialog& window)	:
	CWidgetWindow(window)
{
}

CAmonDialog::~CAmonDialog()
{
}

void CAmonDialog::Start()
{
	CWidgetWindow::Start();
}

bool CAmonDialog::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetPos(400.f, 300.f);
	SetSize(519.f, 165.f);

	m_Background = CreateWidget<CImage>("AmonDialogBackground");
	m_Background->SetSize(519.f, 165.f);
	m_Background->SetTexture("AmonDialogBackground", TEXT("UI/AmonUtilDlgEx.png"));

	m_ChatStopButton = CreateWidget<CButton>("NPCChatStop");
	m_ChatStopButton->SetSize(85.f, 18.f);
	m_ChatStopButton->SetPos(10.f, 10.f);
	m_ChatStopButton->SetTexture(Button_State::Normal, "CharStopButtonNormal", TEXT("UI/UtilDlgEx.BtClose.normal.0.png"));
	m_ChatStopButton->SetTexture(Button_State::MouseOn, "CharStopButtonMouseOn", TEXT("UI/UtilDlgEx.BtClose.mouseOver.0.png"));
	m_ChatStopButton->SetTexture(Button_State::Click, "CharStopButtonClick", TEXT("UI/UtilDlgEx.BtClose.pressed.0.png"));
	m_ChatStopButton->SetClickCallback<CAmonDialog>(this, &CAmonDialog::ChatStopCallback);

	m_ChatStopButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_ChatStopButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_ChatContents = CreateWidget<CText>("NPCChat");
	m_ChatContents->SetFont("Thin");
	m_ChatContents->SetSize(320.f, 15.f);
	m_ChatContents->SetPos(165.f, 115.f);
	m_ChatContents->SetFontSize(12.f);
	m_ChatContents->SetColor(0.f, 0.f, 0.f);
	m_ChatContents->SetZOrder(1);
	m_ChatContents->SetAlignH(TEXT_ALIGN_H::Left);
	m_ChatContents->SetText(m_Contents.c_str());

	m_BtYes = CreateWidget<CButton>("ExitZakumSceneButton");
	m_BtYes->SetSize(57.f, 16.f);
	m_BtYes->SetPos(380.f, 10.f);
	m_BtYes->SetTexture(Button_State::Normal, "ExitZakumSceneButtonNormal", TEXT("UI/UtilDlgEx.BtYes.normal.0.png"));
	m_BtYes->SetTexture(Button_State::MouseOn, "ExitZakumSceneButtonMouseOn", TEXT("UI/UtilDlgEx.BtYes.mouseOver.0.png"));
	m_BtYes->SetTexture(Button_State::Click, "ExitZakumSceneButtonClick", TEXT("UI/UtilDlgEx.BtYes.pressed.0.png"));
	m_BtYes->SetClickCallback<CAmonDialog>(this, &CAmonDialog::GoToWayToZakumScene);

	m_BtYes->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_BtYes->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_BtNo = CreateWidget<CButton>("AmonNoButton");
	m_BtNo->SetSize(57.f, 16.f);
	m_BtNo->SetPos(440.f, 10.f);
	m_BtNo->SetTexture(Button_State::Normal, "AmonNoButtonNormal", TEXT("UI/UtilDlgEx.BtNo.normal.0.png"));
	m_BtNo->SetTexture(Button_State::MouseOn, "AmonNoButtonMouseOn", TEXT("UI/UtilDlgEx.BtNo.mouseOver.0.png"));
	m_BtNo->SetTexture(Button_State::Click, "AmonNoButtonClick", TEXT("UI/UtilDlgEx.BtNo.pressed.0.png"));
	m_BtNo->SetClickCallback<CAmonDialog>(this, &CAmonDialog::ChatStopCallback);

	m_BtNo->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_BtNo->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	return true;
}

void CAmonDialog::Update(float DeltaTime)
{
	if (!m_Enable)
		return;

	CWidgetWindow::Update(DeltaTime);
}

void CAmonDialog::PostUpdate(float DeltaTime)
{
	if (!m_Enable)
		return;

	CWidgetWindow::PostUpdate(DeltaTime);
}

void CAmonDialog::Render()
{
	if (!m_Enable)
		return;

	CWidgetWindow::Render();
}

CAmonDialog* CAmonDialog::Clone()
{
	return new CAmonDialog(*this);
}

void CAmonDialog::SetNPCAmon(CNPCAmon* Amon)
{
	m_NPCAmon = Amon;
}

void CAmonDialog::ChatStopCallback()
{
	m_Enable = false;
}

void CAmonDialog::GoToWayToZakumScene()
{
	if (m_NPCAmon)
	{
		m_NPCAmon->GoToWayToZakumScene();
	}
}
