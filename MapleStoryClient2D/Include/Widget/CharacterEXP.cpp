
#include "CharacterEXP.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "Widget/Image.h"
#include "../Object/Player2D.h"
#include "Widget/ProgressBar.h"
#include "Device.h"

CCharacterEXP::CCharacterEXP()
{
}


CCharacterEXP::CCharacterEXP(const CCharacterEXP& window) :
    CWidgetWindow(window)
{
    m_EXPBackground = window.m_EXPBackground;
    m_EXPBar = window.m_EXPBar;
    //m_EXPMaxNumber = window.m_EXPMaxNumber;
    m_EXPNumber = window.m_EXPNumber;
    //m_EXPDelimiter = window.m_EXPDelimiter;
}

CCharacterEXP::~CCharacterEXP()
{
}

void CCharacterEXP::Start()
{
    CWidgetWindow::Start();
}

bool CCharacterEXP::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    PlayerInfo Info = ((CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject())->GetInfo();
    Resolution RS = CDevice::GetInst()->GetResolution();

    SetSize((float)RS.Width, 10.f);

    m_EXPBackground = CreateWidget<CImage>("EXPBackground");
    m_EXPBackground->SetTexture("EXPBackground", TEXT("UI/Status/EXPBackground.png"));
    m_EXPBackground->SetSize((float)RS.Width, 10.f);

    m_EXPBar = CreateWidget<CProgressBar>("EXPProgrssBar");
    m_EXPBar->SetTexture("EXPProgrssBar", TEXT("UI/Status/EXPBarFull.png"));
    m_EXPBar->SetSize((float)RS.Width, 5.f);
    m_EXPBar->SetPos(12.f, 2.f);
    m_EXPBar->SetDir(ProgressBar_Dir::RightToLeft);
    m_EXPBar->SetPercent((float)Info.EXP / Info.EXPMax);


    std::vector<TCHAR*> vecFileName;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/Status/HPMPNumber%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_EXPNumber = CreateWidget<CNumber>("EXPNumber");
    m_EXPNumber->SetTexture("EXPNumber", vecFileName);
    m_EXPNumber->SetPos(663.f, 0.f);
    m_EXPNumber->SetNumber(Info.EXP);

    m_EXPLeftBracket = CreateWidget<CImage>("EXPLeftBracket");
    m_EXPLeftBracket->SetTexture("EXPLeftBracket", TEXT("UI/Status/number.Lbracket.png"));
    m_EXPLeftBracket->SetSize(2.f, 9.f);
    m_EXPLeftBracket->SetPos(717.f, 0.f);
    m_EXPRightBracket = CreateWidget<CImage>("EXPRightBracket");
    m_EXPRightBracket->SetTexture("EXPRightBracket", TEXT("UI/Status/number.Rbracket.png"));
    m_EXPRightBracket->SetSize(2.f, 9.f);
    m_EXPRightBracket->SetPos(750.f, 0.f);

    m_EXPPercent = CreateWidget<CImage>("EXPPercent");
    m_EXPPercent->SetTexture("EXPPercent", TEXT("UI/Status/number.percent.png"));
    m_EXPPercent->SetSize(7.f, 7.f);
    m_EXPPercent->SetPos(738.f, 1.f);

    m_EXPPercentNumber = CreateWidget<CNumber>("EXPPercentNumber");
    m_EXPPercentNumber->SetTexture("EXPPercentNumber", vecFileName);
    m_EXPPercentNumber->SetNumber((int)(Info.EXP / Info.EXPMax));
    m_EXPPercentNumber->SetPos(725.f, 0.f);


    /*m_EXPMaxNumber->SetPos(717.f, 0.f);
    m_EXPMaxNumber->SetNumber(Info.EXPMax);*/

    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    vecFileName.clear();

    //m_EXPDelimiter = CreateWidget<CImage>("EXPDelimiter");
    //m_EXPDelimiter->SetTexture("EXPDelimiter", TEXT("UI/Status/HPMPDelimiter.png"));
    //m_EXPDelimiter->SetPos((m_EXPNumber->GetWidgetPos().x + m_EXPNumber->GetWidgetSize().x + m_EXPMaxNumber->GetWidgetPos().x) / 2.f - 10.f, 0.f);
    //m_EXPDelimiter->SetSize(6.f, 10.f);

    return true;
}

void CCharacterEXP::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);

    CPlayer2D* Player = (CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject();

    if (Player)
    {
        PlayerInfo Info = Player->GetInfo();
        int Percent = ((Info.EXP * 100) / Info.EXPMax);
        m_EXPPercentNumber->SetNumber(Percent);
    }
}

void CCharacterEXP::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CCharacterEXP::Render()
{
    CWidgetWindow::Render();
}

CCharacterEXP* CCharacterEXP::Clone()
{
    return new CCharacterEXP(*this);
}

void CCharacterEXP::SetEXP(int EXP)
{
    PlayerInfo Info = ((CPlayer2D*)m_Viewport->GetScene()->GetPlayerObject())->GetInfo();

    m_EXPNumber->SetNumber(EXP);
    m_EXPBar->SetPercent((float)EXP / Info.EXPMax);
}



