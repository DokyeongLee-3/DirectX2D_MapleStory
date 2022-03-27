
#include "DyingNoticeWindow.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "../Object/Player2D.h"
#include "../Scene/OnionScene.h"
#include "../Scene/Library2ndScene.h"
#include "../Scene/RadishScene.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/ZakumAltarScene.h"
#include "Input.h"

CDyingNoticeWindow::CDyingNoticeWindow()
{
}

CDyingNoticeWindow::CDyingNoticeWindow(const CDyingNoticeWindow& window)	:
	CWidgetWindow(window)
{
}

CDyingNoticeWindow::~CDyingNoticeWindow()
{
}

void CDyingNoticeWindow::Start()
{
	CWidgetWindow::Start();
}

bool CDyingNoticeWindow::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(300.f, 131.f);

	m_Background = CreateWidget<CImage>("DyingNoticeBackground");
	m_Background->SetTexture("DyingNoticeBackground", TEXT("UI/Die/Notice.0.png"));
	m_Background->SetSize(300.f, 131.f);

	m_ConfirmButton = CreateWidget<CButton>("ConfirmButton");
	m_ConfirmButton->SetZOrder(1);
	m_ConfirmButton->SetSize(40.f, 16.f);
	m_ConfirmButton->SetPos(130.f, 15.f);
	m_ConfirmButton->SetTexture(Button_State::Normal, "ConfirmButtonNormal", TEXT("UI/Die/Notice.btOK.normal.0.png"));
	m_ConfirmButton->SetTexture(Button_State::MouseOn, "ConfirmButtonMouseOn", TEXT("UI/Die/BtOK4.mouseOver.0.png"));
	m_ConfirmButton->SetTexture(Button_State::Click, "ConfirmButtonClick", TEXT("UI/Die/Notice.btOK.pressed.0.png"));
	m_ConfirmButton->SetMouseCollisionEnable(true);

	m_ConfirmButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_ConfirmButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_ConfirmButton->SetClickCallback<CDyingNoticeWindow>(this, &CDyingNoticeWindow::ConfirmButtonCallback);

	m_ConfirmButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
	m_ConfirmButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

	m_BlankCollider = CreateWidget<CImage>("BlankCollider");
	m_BlankCollider->SetTexture("BlankCollider", TEXT("UI/BlankCollider.png"));
	m_BlankCollider->SetPos(10.f, 120.f);
	m_BlankCollider->SetSize(280.f, 10.f);
	m_BlankCollider->SetMouseCollisionEnable(true);
	m_BlankCollider->SetClickCallback(this, &CDyingNoticeWindow::DragWindow);
	m_BlankCollider->SetZOrder(2);

	return true;
}

void CDyingNoticeWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CDyingNoticeWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CDyingNoticeWindow::Render()
{
	CWidgetWindow::Render();
}

CDyingNoticeWindow* CDyingNoticeWindow::Clone()
{
	return new CDyingNoticeWindow(*this);
}

void CDyingNoticeWindow::ConfirmButtonCallback()
{
	CScene* Scene = m_Viewport->GetScene();
	CPlayer2D* Player = (CPlayer2D*)Scene->GetPlayerObject();

	if (Player && Scene)
	{
		if (Scene->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
		{
			Player->ClearListCollision();

			COnionScene* SceneMode = (COnionScene*)(Scene->GetSceneMode());
			CRenderManager::GetInst()->SetStartFadeIn(true);
			CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(SceneMode, &COnionScene::CreateLobbyScene);
		}

		else if (Scene->GetSceneMode()->GetTypeID() == typeid(CRadishScene).hash_code())
		{
			Player->ClearListCollision();

			CRadishScene* SceneMode = (CRadishScene*)(Scene->GetSceneMode());
			CRenderManager::GetInst()->SetStartFadeIn(true);
			CSceneManager::GetInst()->SetFadeInEndCallback<CRadishScene>(SceneMode, &CRadishScene::CreateLobbyScene);
		}

		else if (Scene->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
		{
			Player->ClearListCollision();

			CLibrary2ndScene* SceneMode = (CLibrary2ndScene*)(Scene->GetSceneMode());
			CRenderManager::GetInst()->SetStartFadeIn(true);
			CSceneManager::GetInst()->SetFadeInEndCallback<CLibrary2ndScene>(SceneMode, &CLibrary2ndScene::CreateLobbyScene);
		}

		else if (Scene->GetSceneMode()->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		{
			Player->ClearListCollision();

			CZakumAltarScene* SceneMode = (CZakumAltarScene*)(Scene->GetSceneMode());
			CRenderManager::GetInst()->SetStartFadeIn(true);
			CSceneManager::GetInst()->SetFadeInEndCallback<CZakumAltarScene>(SceneMode, &CZakumAltarScene::CreateWayToZakumScene);
		}
	}
}

void CDyingNoticeWindow::DragWindow()
{
	int TopMost = m_Viewport->GetTopmostWindowZOrder();

	if (TopMost >= m_ZOrder)
	{
		m_Viewport->DecrementAllWindowZOrder();
		m_ZOrder = TopMost;
	}

	Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

	m_Pos += MouseMove;

	m_BlankCollider->SetClicked(false);
}
