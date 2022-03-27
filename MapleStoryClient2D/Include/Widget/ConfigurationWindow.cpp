
#include "ConfigurationWindow.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "Widget/Image.h"

CConfigurationWindow::CConfigurationWindow()
{
}

// 부모인 CWidgetWindow안에서 Clone한 Widget들을 m_WidgetList에 넣었으니
// 그걸 m_WidgetWindow에서 찾아서 그 포인터들을 멤버(m_Button, m_Button1, m_Button1Text)에
// 할당만 해주면됨
CConfigurationWindow::CConfigurationWindow(const CConfigurationWindow& window) :
    CWidgetWindow(window)
{
    m_BGMSoundSlider = window.m_BGMSoundSlider;
    m_Configuration = window.m_Configuration;
    m_BGMSoundSliderLeftMostXPos = window.m_BGMSoundSliderLeftMostXPos;
    m_BGMSoundSliderRightMostXPos = window.m_BGMSoundSliderRightMostXPos;
}

CConfigurationWindow::~CConfigurationWindow()
{
}

void CConfigurationWindow::Start()
{
    CWidgetWindow::Start();
}

bool CConfigurationWindow::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetPos(700.f, 100.f);
    SetSize(265.f, 472.f);

    m_Configuration = CreateWidget<CImage>("ConfigurationWindowBackground");
    m_Configuration->SetTexture("ConfigurationWindow", TEXT("UI/Configuration/ConfigurationBackground.png"));
    m_Configuration->SetSize(265.f, 472.f);
    m_Configuration->SetMouseCollisionEnable(false);

    m_BGMSoundSlider = CreateWidget<CImage>("BGMSoundSlider");
    m_BGMSoundSlider->SetTexture("BGMSoundSlider", TEXT("UI/Configuration/OptionScroll.png"));
    m_BGMSoundSlider->SetSize(28.f, 11.f);
    m_BGMSoundSlider->SetPos(130.f, 333.f);
    m_BGMSoundSliderLeftMostXPos = 90.f;
    m_BGMSoundSliderRightMostXPos = 147.f;
    m_BGMSoundSlider->SetMoveXAllowed(true);

    m_EffectSoundSlider = CreateWidget<CImage>("EffectSoundSlider");
    m_EffectSoundSlider->SetTexture("EffectSoundSlider", TEXT("UI/Configuration/OptionScroll.png"));
    m_EffectSoundSlider->SetSize(28.f, 11.f);
    m_EffectSoundSlider->SetPos(130.f, 303.f);
    m_EffectSoundSliderLeftMostXPos = 90.f;
    m_EffectSoundSliderRightMostXPos = 147.f;
    m_EffectSoundSlider->SetMoveXAllowed(true);


    float SliderXPos = m_BGMSoundSlider->GetWidgetPos().x;
    m_Viewport->GetScene()->GetResource()->SetVolume("BGM", (int)((SliderXPos - m_BGMSoundSliderLeftMostXPos) / (m_BGMSoundSliderRightMostXPos - m_BGMSoundSliderLeftMostXPos) * 100));

    SliderXPos = m_EffectSoundSlider->GetWidgetPos().x;
    m_Viewport->GetScene()->GetResource()->SetVolume("Effect", (int)((SliderXPos - m_EffectSoundSliderLeftMostXPos) / (m_EffectSoundSliderRightMostXPos - m_EffectSoundSliderLeftMostXPos) * 100));
    
    //CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
    //CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
    //CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Push, this, &CConfigurationWindow::MoveSlider);

    m_BlankCollider = CreateWidget<CImage>("BlankCollider");
    m_BlankCollider->SetTexture("BlankCollider", TEXT("UI/BlankCollider.png"));
    m_BlankCollider->SetPos(5.f, 430.f);
    m_BlankCollider->SetSize(260.f, 40.f);
    m_BlankCollider->SetMouseCollisionEnable(true);
    m_BlankCollider->SetClickCallback(this, &CConfigurationWindow::DragWindow);
    m_BlankCollider->SetZOrder(3);

    return true;
}

void CConfigurationWindow::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CConfigurationWindow::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);

    float SliderXPos = m_BGMSoundSlider->GetWidgetPos().x;
    m_Viewport->GetScene()->GetResource()->SetVolume("BGM", (int)((SliderXPos - m_BGMSoundSliderLeftMostXPos) / (m_BGMSoundSliderRightMostXPos - m_BGMSoundSliderLeftMostXPos) * 100));

    SliderXPos = m_EffectSoundSlider->GetWidgetPos().x;
    m_Viewport->GetScene()->GetResource()->SetVolume("Effect", (int)((SliderXPos - m_EffectSoundSliderLeftMostXPos) / (m_EffectSoundSliderRightMostXPos - m_EffectSoundSliderLeftMostXPos) * 100));
}

void CConfigurationWindow::Render()
{
    CWidgetWindow::Render();

    if (m_BGMSoundSlider->GetWidgetPos().x > m_BGMSoundSliderRightMostXPos)
        m_BGMSoundSlider->SetPos(m_BGMSoundSliderRightMostXPos, m_BGMSoundSlider->GetWidgetPos().y);

    if (m_BGMSoundSlider->GetWidgetPos().x < m_BGMSoundSliderLeftMostXPos)
        m_BGMSoundSlider->SetPos(m_BGMSoundSliderLeftMostXPos, m_BGMSoundSlider->GetWidgetPos().y);

    if (m_EffectSoundSlider->GetWidgetPos().x > m_EffectSoundSliderRightMostXPos)
        m_EffectSoundSlider->SetPos(m_EffectSoundSliderRightMostXPos, m_EffectSoundSlider->GetWidgetPos().y);

    if (m_EffectSoundSlider->GetWidgetPos().x < m_EffectSoundSliderLeftMostXPos)
        m_EffectSoundSlider->SetPos(m_EffectSoundSliderLeftMostXPos, m_EffectSoundSlider->GetWidgetPos().y);
}

CConfigurationWindow* CConfigurationWindow::Clone()
{
    return new CConfigurationWindow(*this);
}

void CConfigurationWindow::DragWindow()
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



