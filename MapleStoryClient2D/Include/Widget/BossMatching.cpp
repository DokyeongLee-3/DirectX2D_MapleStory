
#include "BossMatching.h"
#include "Widget/Image.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/OnionScene.h"
#include "../Scene/RadishScene.h"
#include "../Scene/Library2ndScene.h"
#include "Render/RenderManager.h"

CBossMatching::CBossMatching()
{
}


CBossMatching::CBossMatching(const CBossMatching& window) :
    CWidgetWindow(window)
{
    m_Background = window.m_Background;
    m_Button = window.m_Button;
}

CBossMatching::~CBossMatching()
{
}

void CBossMatching::Start()
{
    CWidgetWindow::Start();
}

bool CBossMatching::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(264.f, 382.f);

    m_Background = CreateWidget<CImage>("BossMatchingBack");
    m_Background->SetTexture("BossMatchingBack", TEXT("UI/BossMatching/BossMatchingBack.png"));
    m_Background->SetSize(264.f, 382.f);

    m_Button = CreateWidget<CButton>("BossMatchingStart");
    m_Button->SetTexture(Button_State::Normal, "BossMatchingStartNormal", TEXT("UI/BossMatching/UserList.Main.Boss.BtEntry.normal.0.png"));
    m_Button->SetTexture(Button_State::MouseOn, "BossMatchingStartMouseOn", TEXT("UI/BossMatching/UserList.Main.Boss.BtEntry.mouseOver.0.png"));
    m_Button->SetTexture(Button_State::Click, "BossMatchingStartClick", TEXT("UI/BossMatching/UserList.Main.Boss.BtEntry.pressed.0.png"));
    m_Button->SetClickCallback<CBossMatching>(this, &CBossMatching::CreateWayToZakumScene);
    m_Button->SetPos(178.f, 13.f);
    m_Button->SetSize(74.f, 48.f);

    return true;
}

void CBossMatching::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CBossMatching::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CBossMatching::Render()
{
    CWidgetWindow::Render();
}

CBossMatching* CBossMatching::Clone()
{
    return new CBossMatching(*this);
}


void CBossMatching::CreateWayToZakumScene()
{
    CSceneMode* SceneMode = m_Viewport->GetScene()->GetSceneMode();

    if (SceneMode)
    {
        if (m_Viewport->GetScene()->GetSceneMode()->GetTypeID() == typeid(CWayToZakumScene).hash_code())
            return;


        else if (m_Viewport->GetScene()->GetSceneMode()->GetTypeID() == typeid(CLobbyScene).hash_code())
        {
            CLobbyScene* Scene = (CLobbyScene*)SceneMode;
            CRenderManager::GetInst()->SetStartFadeIn(true);
            CSceneManager::GetInst()->SetFadeInEndCallback<CLobbyScene>(Scene, &CLobbyScene::CreateWayToZakumScene);
        }          

        else if (m_Viewport->GetScene()->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
        {
            COnionScene* Scene = (COnionScene*)SceneMode;
            CRenderManager::GetInst()->SetStartFadeIn(true);
            CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(Scene, &COnionScene::CreateWayToZakumScene);
        }

        else if (m_Viewport->GetScene()->GetSceneMode()->GetTypeID() == typeid(CRadishScene).hash_code())
        {
            CRadishScene* Scene = (CRadishScene*)SceneMode;
            CRenderManager::GetInst()->SetStartFadeIn(true);
            CSceneManager::GetInst()->SetFadeInEndCallback<CRadishScene>(Scene, &CRadishScene::CreateWayToZakumScene);
        }

        else if (m_Viewport->GetScene()->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
        {
            CLibrary2ndScene* Scene = (CLibrary2ndScene*)SceneMode;
            CRenderManager::GetInst()->SetStartFadeIn(true);
            CSceneManager::GetInst()->SetFadeInEndCallback<CLibrary2ndScene>(Scene, &CLibrary2ndScene::CreateWayToZakumScene);
        }
    }
}