#include "MouseNormal.h"
#include "Input.h"

CMouseNormal::CMouseNormal()
{
}

CMouseNormal::CMouseNormal(const CMouseNormal& window) :
    CWidgetWindow(window)
{
    m_Image = FindWidget<CImage>("Image");
}

CMouseNormal::~CMouseNormal()
{
}

void CMouseNormal::Start()
{
}

bool CMouseNormal::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(24.f, 28.f);

    m_Image = CreateWidget<CImage>("Image");

    m_Image->SetTexture("MouseNormal", TEXT("UI/Cursor/Cursor.0.0.png"));
    m_Image->SetSize(24.f, 28.f);

    return true;
}

void CMouseNormal::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CMouseNormal::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CMouseNormal::Render()
{
    CWidgetWindow::Render();
}

CMouseNormal* CMouseNormal::Clone()
{
    return new CMouseNormal(*this);
}
