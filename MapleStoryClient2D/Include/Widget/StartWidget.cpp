
#include "StartWidget.h"
#include "Engine.h"
#include "../Scene/LoadingScene.h"
#include "Scene/SceneManager.h"
#include "Resource/Sound/SoundManager.h"
#include "../Scene/StartScene.h"

CStartWidget::CStartWidget()
{
}

CStartWidget::CStartWidget(const CStartWidget& window) :
    CWidgetWindow(window)
{
    m_ExitButton = FindWidget<CButton>("ExitButton");
    m_Back = FindWidget<CImage>("Back");
}

CStartWidget::~CStartWidget()
{
}

void CStartWidget::Start()
{
}

bool CStartWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    m_Viewport->GetScene()->GetResource()->LoadSound("BGM", true, "CharacterSelectBGM", "CharacterSelect.mp3");
    m_Viewport->GetScene()->GetResource()->SoundPlay("CharacterSelectBGM");

    SetSize(1280.f, 720.f);

    m_Back = CreateWidget<CImage>("Back");

    m_Back->SetTexture("CharacterSelectBackground", TEXT("UI/CharacterSelectScene/CharacterSelectBackground.png"));
    m_Back->SetSize(1280.f, 960.f);
    m_Back->SetPos(0.f, -120.f);

    m_ExitButton = CreateWidget<CButton>("ExitButton");
    m_ExitButton->SetZOrder(1);

    m_ExitButton->SetPos(0.f, 50.f);
    m_ExitButton->SetSize(81.f, 34.f);
    m_ExitButton->SetTexture(Button_State::Normal, "EndButtonNormal", TEXT("UI/CharacterSelectScene/ExitNormal.png"));
    m_ExitButton->SetTexture(Button_State::MouseOn, "EndButtonMouseOn", TEXT("UI/CharacterSelectScene/ExitMouseOver.png"));
    m_ExitButton->SetTexture(Button_State::Click, "EndButtonPressed", TEXT("UI/CharacterSelectScene/ExitPressed.png"));

    m_ExitButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
    m_ExitButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

    m_ExitButton->SetClickCallback<CStartWidget>(this, &CStartWidget::ExitButtonClick);


    m_JobFlag = CreateWidget<CImage>("JobFlag");
    m_JobFlag->SetTexture("JobFlag", TEXT("UI/CharacterSelectScene/CharSelectLuminousFlag.png"));
    m_JobFlag->SetSize(77.f, 162.f);
    m_JobFlag->SetPos(150.f, 410.f);
    m_JobFlag->SetZOrder(1);

    m_CharacterSelectBackLight = CreateWidget<CImage>("CharacterSelectBackLight");
    m_CharacterSelectBackLight->SetPos(200.f, 410.f);
    m_CharacterSelectBackLight->LoadSequence2D("CharacterSelectBackLight.sqc");

    m_Player = CreateWidget<CImage>("Player");
    m_Player->SetPos(210.f, 401.f);
    m_Player->LoadSequence2D("CharacterSelectPlayerStandRight.sqc");
    m_Player->SetClickCallback(this, &CStartWidget::CharacterClick);
    //m_Player->LoadSequence2D("CharacterSelectPlayerWalkRight.sqc");
    m_Player->SetZOrder(1);

    m_PlayerNameTagBack = CreateWidget<CImage>("PlayerNameTagBack");
    m_PlayerNameTagBack->SetSize(100.f, 26.f);
    m_PlayerNameTagBack->SetPos(225.f, 384.f);
    m_PlayerNameTagBack->SetTexture("PlayerNameTagBack", TEXT("UI/CharacterSelectScene/CharacterSelectNameTag.png"));
    m_PlayerNameTagBack->SetZOrder(1);

    PlayerInfo Info = ((CStartScene*)(m_Viewport->GetScene()->GetSceneMode()))->GetPlayerInfo();

    m_PlayerName = CreateWidget<CText>("PlayerName");
    m_PlayerName->SetText(Info.Name);
    m_PlayerName->SetAlphaEnable(true);
    m_PlayerName->SetPos(230.f, 383.f);
    m_PlayerName->SetSize(90.f, 30.f);
    m_PlayerName->SetFontSize(10.f);
    m_PlayerName->SetColor(1.f, 1.f, 1.f);
    m_PlayerName->SetOpacity(0.8f);
    m_PlayerName->SetAlignH(TEXT_ALIGN_H::Center);
    m_PlayerName->SetAlignV(TEXT_ALIGN_V::Middle);
    m_PlayerName->SetZOrder(2);

    return true;
}

void CStartWidget::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CStartWidget::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);

    if (m_Player->IsClicked())
        m_PlayerName->SetOpacity(1.f);
}

void CStartWidget::Render()
{
    CWidgetWindow::Render();
}

CStartWidget* CStartWidget::Clone()
{
    return new CStartWidget(*this);
}

void CStartWidget::ExitButtonClick()
{
    CEngine::GetInst()->Exit();
}

void CStartWidget::CharacterClick()
{
    m_Player->ClearWidgetFrameData();
    m_Player->LoadSequence2D("CharacterSelectPlayerWalkRight.sqc", ANIMATION_PATH, 1.f);

}
