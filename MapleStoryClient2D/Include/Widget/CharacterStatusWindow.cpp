
#include "CharacterStatusWindow.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "Widget/Image.h"
#include "../Object/Player2D.h"
#include "Widget/ProgressBar.h"

CCharacterStatusWindow::CCharacterStatusWindow()
{
}

CCharacterStatusWindow::CCharacterStatusWindow(const CCharacterStatusWindow& window) :
    CWidgetWindow(window)
{
    m_HPMPBackground = window.m_HPMPBackground;
    m_Level = window.m_Level;
    m_LevelNumber = window.m_LevelNumber;
    m_PlayerName = window.m_PlayerName;
    m_HPBar = window.m_HPBar;
    m_MPBar = window.m_MPBar;
    m_HPMaxNumber = window.m_HPMaxNumber;
    m_MPMaxNumber = window.m_MPMaxNumber;
    m_CurrentHPNumber = window.m_CurrentHPNumber;
    m_CurrentMPNumber = window.m_CurrentMPNumber;
    m_HPDelimiter = window.m_HPDelimiter;
    m_MPDelimiter = window.m_MPDelimiter;

}

CCharacterStatusWindow::~CCharacterStatusWindow()
{
}

void CCharacterStatusWindow::Start()
{
    CWidgetWindow::Start();
}

bool CCharacterStatusWindow::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    
    PlayerInfo Info = ((CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject())->GetInfo();

    SetSize(204.f, 70.f);

    m_HPMPBackground = CreateWidget<CImage>("HPMPBackground");
    m_HPMPBackground->SetTexture("HPMPBackground", TEXT("UI/Status/HPMPWindowBackground.png"));
    m_HPMPBackground->SetSize(204.f, 70.f);

    m_Level = CreateWidget<CImage>("Level");
    m_Level->SetTexture("Level", TEXT("UI/Status/HPMPWindowLv.png"));
    m_Level->SetPos(13.f, 53.f);
    m_Level->SetSize(15.f, 10.f);

    m_LevelNumber = CreateWidget<CNumber>("LevelNumber");
    m_LevelNumber->SetSize(7.f, 10.f);

    std::vector<TCHAR*> vecFileName;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/Status/LevelNumber%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_LevelNumber->SetTexture("LevelNumber", vecFileName);
    m_LevelNumber->SetPos(30.f, 53.f);
    m_LevelNumber->SetNumber(Info.Level);

    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();




    m_PlayerName = CreateWidget<CText>("PlayerName");
    m_PlayerName->SetText(Info.Name);
    m_PlayerName->SetPos(68.f, 53.f);
    m_PlayerName->SetSize(60.f, 13.f);
    m_PlayerName->SetFontSize(10.f);
    m_PlayerName->SetColor(1.f, 1.f, 1.f);
    m_PlayerName->SetAlignH(TEXT_ALIGN_H::Center);
    m_PlayerName->SetAlignV(TEXT_ALIGN_V::Middle);
    m_PlayerName->SetFont("Default");

    m_HPBar = CreateWidget<CProgressBar>("PlayerHPBar");
    m_HPBar->SetTexture("PlayerHPBar", TEXT("UI/Status/HPBar.png"));
    m_HPBar->SetDir(ProgressBar_Dir::RightToLeft);
    m_HPBar->SetSize(171.f, 13.f);
    m_HPBar->SetPos(25.f, 29.f);
    m_HPBar->SetPercent(((float)Info.HP / (float)Info.HPMax));

    m_MPBar = CreateWidget<CProgressBar>("PlayerMPBar");
    m_MPBar->SetTexture("PlayerMPBar", TEXT("UI/Status/MPBar.png"));
    m_MPBar->SetDir(ProgressBar_Dir::RightToLeft);
    m_MPBar->SetSize(171.f, 13.f);
    m_MPBar->SetPos(25.f, 14.f);
    m_MPBar->SetPercent(((float)Info.MP / (float)Info.MPMax));



    m_HPMaxNumber = CreateWidget<CNumber>("PlayerMaxHP");
    m_HPMaxNumber->SetSize(7.f, 9.f);
    m_HPMaxNumber->SetPos(125.f, 31.f);
    m_HPMaxNumber->SetNumber(Info.HPMax);
    
    m_MPMaxNumber = CreateWidget<CNumber>("PlayerMaxMP");
    m_MPMaxNumber->SetSize(7.f, 9.f);
    m_MPMaxNumber->SetPos(125.f, 15.f);
    m_MPMaxNumber->SetNumber(Info.MPMax);

    m_CurrentHPNumber = CreateWidget<CNumber>("PlayerHP");
    m_CurrentHPNumber->SetSize(7.f, 9.f);
    m_CurrentHPNumber->SetPos(88.f, 31.f);
    m_CurrentHPNumber->SetNumber(Info.HP);

    m_CurrentMPNumber = CreateWidget<CNumber>("PlayerMP");
    m_CurrentMPNumber->SetSize(7.f, 9.f);
    m_CurrentMPNumber->SetPos(80.f, 15.f);
    m_CurrentMPNumber->SetNumber(Info.MP);


    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/Status/HPMPNumber%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_HPMaxNumber->SetTexture("PlayerHPMPNumber", vecFileName);
    m_CurrentHPNumber->SetTexture("PlayerHPMPNumber", vecFileName);
    m_MPMaxNumber->SetTexture("PlayerHPMPNumber", vecFileName);
    m_CurrentMPNumber->SetTexture("PlayerHPMPNumber", vecFileName);


    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();

    int CurrentHPDigit = (int)m_CurrentHPNumber->GetNumberDigitCount();
    int CurrentMPDigit = (int)m_CurrentMPNumber->GetNumberDigitCount();

    m_HPDelimiter = CreateWidget<CImage>("HPDelimiter");
    m_HPDelimiter->SetTexture("HPDelimiter", TEXT("UI/Status/HPMPDelimiter.png"));
    m_HPDelimiter->SetPos((m_HPMaxNumber->GetWidgetPos().x + m_CurrentHPNumber->GetWidgetPos().x + m_CurrentHPNumber->GetWidgetSize().x * CurrentHPDigit) / 2.f - 1.f, 
        m_HPMaxNumber->GetWidgetPos().y);
    m_HPDelimiter->SetSize(6.f, 10.f);

    m_MPDelimiter = CreateWidget<CImage>("MPDelimiter");
    m_MPDelimiter->SetTexture("MPDelimiter", TEXT("UI/Status/HPMPDelimiter.png"));
    m_MPDelimiter->SetPos((m_MPMaxNumber->GetWidgetPos().x + m_CurrentMPNumber->GetWidgetPos().x + m_CurrentMPNumber->GetWidgetSize().x * CurrentMPDigit) / 2.f - 3.f,
        m_MPMaxNumber->GetWidgetPos().y);
    m_MPDelimiter->SetSize(6.f, 10.f);



    return true;
}

void CCharacterStatusWindow::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CCharacterStatusWindow::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);

    
}

void CCharacterStatusWindow::Render()
{
    CWidgetWindow::Render();

   
}

CCharacterStatusWindow* CCharacterStatusWindow::Clone()
{
    return new CCharacterStatusWindow(*this);
}


