
#include "MouseCatch.h"

CMouseCatch::CMouseCatch()
{
}

CMouseCatch::CMouseCatch(const CMouseCatch& window) :
    CWidgetWindow(window)
{
    m_Image = FindWidget<CImage>("Image");
}

CMouseCatch::~CMouseCatch()
{
}

void CMouseCatch::Start()
{
}

bool CMouseCatch::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(32.f, 31.f);

    m_Image = CreateWidget<CImage>("Image");

    return true;
}

void CMouseCatch::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CMouseCatch::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CMouseCatch::Render()
{
    CWidgetWindow::Render();
}

CMouseCatch* CMouseCatch::Clone()
{
    return new CMouseCatch(*this);
}
