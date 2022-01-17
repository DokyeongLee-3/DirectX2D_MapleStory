#include "MainWidget.h"

CMainWidget::CMainWidget()
{
}

// 부모인 CWidgetWindow안에서 Clone한 Widget들을 m_WidgetList에 넣었으니
// 그걸 m_WidgetWindow에서 찾아서 그 포인터들을 멤버(m_Button, m_Button1, m_Button1Text)에
// 할당만 해주면됨
CMainWidget::CMainWidget(const CMainWidget& window) :
    CWidgetWindow(window)
{
    m_Button = FindWidget<CButton>("Button");
    m_Button1 = FindWidget<CButton>("Button1");
    m_Button1Text = FindWidget<CText>("Text");
}

CMainWidget::~CMainWidget()
{
}

void CMainWidget::Start()
{
    CWidgetWindow::Start();
}

bool CMainWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(1280.f, 720.f);

    m_Button = CreateWidget<CButton>("Button");

    m_Button->SetPos(400.f, 300.f);
    m_Button->SetSize(200.f, 50.f);
    m_Button->SetTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
    m_Button->SetTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
    m_Button->SetTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

    m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
    m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
    m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

    m_Button->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
    m_Button->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

    m_Button->SetClickCallback<CMainWidget>(this, &CMainWidget::StartButtonClick);

    m_Button1 = CreateWidget<CButton>("Button1");

    m_Button1->SetPos(50.f, 200.f);
    m_Button1->SetSize(100.f, 40.f);

    m_Button1->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
    m_Button1->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
    m_Button1->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

    m_Button1->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
    m_Button1->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

    m_Button1->SetClickCallback<CMainWidget>(this, &CMainWidget::Button1Click);

    m_Button1Text = CreateWidget<CText>("Text");

    m_Button1Text->SetText(TEXT("Button1"));
    m_Button1Text->SetPos(50.f, 200.f);
    m_Button1Text->SetSize(100.f, 40.f);
    m_Button1Text->SetZOrder(1);
    m_Button1Text->SetColor(1.f, 0.f, 0.f);
    //m_Button1Text->SetOpacity(0.5f);
    m_Button1Text->SetAlignH(TEXT_ALIGN_H::Center);
    m_Button1Text->SetShadowEnable(true);
    m_Button1Text->SetShadowOffset(2.f, 2.f);


    //m_AnimationButton = CreateWidget<CButton>("AnimationButton");
    //m_AnimationButton->LoadSequence2D(Button_State::Normal, "GameStartBlink.sqc", ANIMATION_PATH, 0.5f);
    //m_AnimationButton->SetPos(20.f, 20.f);
    //m_AnimationButton->SetTextureTint(Button_State::Normal, 0, 0, 220, 255);
    
    return true;
}

void CMainWidget::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CMainWidget::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CMainWidget::Render()
{
    CWidgetWindow::Render();
}

CMainWidget* CMainWidget::Clone()
{
    return new CMainWidget(*this);
}

void CMainWidget::StartButtonClick()
{
}

void CMainWidget::Button1Click()
{
}
