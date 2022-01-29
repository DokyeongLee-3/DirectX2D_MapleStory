
#include "CharacterSelectInfoWidget.h"
#include "Engine.h"
#include "../Scene/LoadingScene.h"
#include "Scene/SceneManager.h"
#include "../Object/Player2D.h"
#include "../Client.h"
#include "../Scene/StartScene.h"
#include "Render/RenderManager.h"

CCharacterSelectInfoWidget::CCharacterSelectInfoWidget()
{
}

CCharacterSelectInfoWidget::CCharacterSelectInfoWidget(const CCharacterSelectInfoWidget& window) :
    CWidgetWindow(window)
{
    /*
    CSharedPtr<CImage>	m_Back;
    CSharedPtr<CImage>	m_LevelImage;
    CSharedPtr<CNumber>	m_LevelNumber;
    CSharedPtr<CText>	m_Name;
    CSharedPtr<CNumber> m_STR;
    CSharedPtr<CNumber> m_INT;
    CSharedPtr<CNumber> m_DEX;
    CSharedPtr<CNumber> m_LUK;
    CSharedPtr<CButton>	m_StartButton;
    */
}

CCharacterSelectInfoWidget::~CCharacterSelectInfoWidget()
{
}

void CCharacterSelectInfoWidget::Start()
{
}

bool CCharacterSelectInfoWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetPos(1020.f, 150.f);
    SetSize(194.f, 271.f);

    m_Back = CreateWidget<CImage>("CharacterSelectInfoBack");
    m_Back->SetTexture("CharacterSelectInfoBack", TEXT("UI/CharacterSelectScene/CharSelect.charInfo1.png"));
    m_Back->SetSize(194.f, 271.f);

    m_LevelImage = CreateWidget<CImage>("LevelImage");
    m_LevelImage->SetTexture("LevelImage", TEXT("UI/CharacterSelectScene/CharSelect.lv.lv.png"));
    m_LevelImage->SetSize(23.f, 18.f);
    m_LevelImage->SetPos(70.f, 220.f);

    m_StartButton = CreateWidget<CButton>("StartButton");

    m_StartButton->SetZOrder(1);

    m_StartButton->SetPos(27.f, 30.f);
    m_StartButton->SetSize(140.f, 52.f);
    m_StartButton->SetTexture(Button_State::Normal, "StartButtonNormal", TEXT("UI/CharacterSelectScene/GameStartNormal.png"));
    m_StartButton->SetTexture(Button_State::MouseOn, "StartButtonMouseOn", TEXT("UI/CharacterSelectScene/GameStartMouseOver.png"));
    m_StartButton->SetTexture(Button_State::Click, "StartButtonPressed", TEXT("UI/CharacterSelectScene/GameStartPressed.png"));

    m_StartButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "BtMouseOver.mp3");
    m_StartButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "BtMouseClick.mp3");

    m_StartButton->SetClickCallback<CCharacterSelectInfoWidget>(this, &CCharacterSelectInfoWidget::StartButtonClick);

    PlayerInfo Info = ((CStartScene*)(m_Viewport->GetScene()->GetSceneMode()))->GetPlayerInfo();

    std::vector<TCHAR*> vecFileName;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/CharacterSelectScene/CharSelectLV%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_LevelNumber = CreateWidget<CNumber>("CharacterSelectLevelNumber");
    m_LevelNumber->SetTexture("CharacterSelectLevelNumber", vecFileName);
    m_LevelNumber->SetPos(90.f, 220.f);
    m_LevelNumber->SetNumber(Info.Level);

    m_Name = CreateWidget<CText>("CharacterName");
    m_Name->SetText(Info.Name);
    m_Name->SetPos(60.f, 187.f);
    m_Name->SetSize(70.f, 30.f);
    m_Name->SetFontSize(11.f);
    m_Name->SetColor(1.f, 1.f, 1.f);
    m_Name->SetAlignH(TEXT_ALIGN_H::Center);
    m_Name->SetAlignV(TEXT_ALIGN_V::Middle);
    m_Name->SetAlphaEnable(true);

    m_Job = CreateWidget<CText>("CharacterJob");
    m_Job->SetText(Info.Job);
    m_Job->SetPos(75.f, 161.f);
    m_Job->SetSize(50.f, 20.f);
    m_Job->SetFontSize(9.f);
    m_Job->SetColor(1.f, 1.f, 1.f);
    m_Job->SetAlignH(TEXT_ALIGN_H::Right);
    m_Job->SetAlignV(TEXT_ALIGN_V::Middle);
    m_Job->SetAlphaEnable(true);

    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();



    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/Status/HPMPNumber%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_STR = CreateWidget<CNumber>("CharacterSelectSTRNumber");
    m_INT = CreateWidget<CNumber>("CharacterSelectINTNumber");
    m_DEX = CreateWidget<CNumber>("CharacterSelectDEXNumber");
    m_LUK = CreateWidget<CNumber>("CharacterSelectLUKNumber");

    m_STR->SetTexture("CharacterSelectSTRNumber", vecFileName);
    m_STR->SetPos(60.f, 143.f);
    m_STR->SetNumber(Info.STR);
    m_STR->SetSize(7.f, 9.f);

    m_INT->SetTexture("CharacterSelectINTNumber", vecFileName);
    m_INT->SetPos(125.f, 143.f);
    m_INT->SetNumber(Info.INT);
    m_INT->SetSize(7.f, 9.f);

    m_DEX->SetTexture("CharacterSelectDEXNumber", vecFileName);
    m_DEX->SetPos(60.f, 118.f);
    m_DEX->SetNumber(Info.DEX);
    m_DEX->SetSize(7.f, 9.f);

    m_LUK->SetTexture("CharacterSelectLUKNumber", vecFileName);
    m_LUK->SetPos(130.f, 118.f);
    m_LUK->SetNumber(Info.LUK);
    m_LUK->SetSize(7.f, 9.f);


    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();

    return true;
}

void CCharacterSelectInfoWidget::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CCharacterSelectInfoWidget::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CCharacterSelectInfoWidget::Render()
{
    CWidgetWindow::Render();
}

CCharacterSelectInfoWidget* CCharacterSelectInfoWidget::Clone()
{
    return new CCharacterSelectInfoWidget(*this);
}

void CCharacterSelectInfoWidget::StartButtonClick()
{
    CStartScene* StartScene = (CStartScene*)(m_Viewport->GetScene()->GetSceneMode());

    bool CharacterClicked = StartScene->GetStartWidget()->GetPlayer()->IsClicked();

    if (!CharacterClicked)
        return;

    CStartScene* Scene = (CStartScene*)(m_Viewport->GetScene()->GetSceneMode());

    CRenderManager::GetInst()->SetStartFadeIn(true);
    CSceneManager::GetInst()->SetFadeInEndCallback<CStartScene>(Scene, &CStartScene::CreateNextScene);

    /*CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneMode<CLoadingScene>(false);*/

    /*CScene* NextScene = CSceneManager::GetInst()->GetNextScene();

    if (NextScene)
    {
        CSceneMode* SceneMode = NextScene->GetSceneMode();

        CPlayer2D* Player = (CPlayer2D*)(m_Viewport->GetScene()->GetSceneMode()->GetPlayerObject());

        SceneMode->SetPlayerObject(Player);
    }*/
}

