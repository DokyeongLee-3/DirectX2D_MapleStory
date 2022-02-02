
#include "MouseClick.h"

CMouseClick::CMouseClick()
{
}

CMouseClick::CMouseClick(const CMouseClick& window) :
    CWidgetWindow(window)
{
    m_Image = FindWidget<CImage>("Image");
}

CMouseClick::~CMouseClick()
{
}

void CMouseClick::Start()
{
}

bool CMouseClick::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(25.f, 23.f);

    m_Image = CreateWidget<CImage>("ClickImage");
    m_Image->SetTexture("MouseClick", TEXT("UI/Cursor/Cursor.12.0.png"));
    m_Image->SetSize(25.f, 23.f);
    m_Image->SetPos(-1.f, -5.f);

    return true;
}

void CMouseClick::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
}

void CMouseClick::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);
}

void CMouseClick::Render()
{
    CWidgetWindow::Render();
}

CMouseClick* CMouseClick::Clone()
{
    return new CMouseClick(*this);
}
