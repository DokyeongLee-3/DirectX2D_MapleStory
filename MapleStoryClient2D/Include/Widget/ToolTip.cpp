
#include "ToolTip.h"

CToolTip::CToolTip()
{
	SetTypeID<CToolTip>();
}

CToolTip::CToolTip(const CToolTip& window)	:
	CWidgetWindow(window)
{
}

CToolTip::~CToolTip()
{
}

void CToolTip::Start()
{
	CWidgetWindow::Start();
}

bool CToolTip::Init()
{
	m_ToolTipBack = CreateWidget<CImage>("ToolTipBack");
	m_ToolTipBack->SetTexture("ToolTipBack", TEXT("UI/ToolTip/ItemToolTipBack.png"));
	m_ToolTipBack->SetSize(261.f, 120.f);
	m_ToolTipBack->SetPos(30.f, 30.f);

	m_ToolTipTitle = CreateWidget<CText>("ToolTipTitle");
	m_ToolTipTitle->SetSize(261.f, 5.f);
	m_ToolTipTitle->SetPos(150.f, 128.f);
	m_ToolTipTitle->SetFontSize(15.f);
	m_ToolTipTitle->SetFont("Thin");

	m_ToolTipContents = CreateWidget<CText>("ToolTipContents");
	m_ToolTipContents->SetSize(160.f, 90.f);
	m_ToolTipContents->SetPos(130.f, 50.f);
	m_ToolTipContents->SetFontSize(11.f);
	m_ToolTipContents->SetFont("Thin");

	m_ToolTipItemIconBack = CreateWidget<CImage>("ToolTipItemIconBack");
	m_ToolTipItemIconBack->SetTexture("ToolTipItemIconBack", TEXT("UI/ToolTip/Item.ItemIcon.base.png"));
	m_ToolTipItemIconBack->SetSize(82.f, 82.f);
	m_ToolTipItemIconBack->SetPos(40.f, 50.f);


	m_ToolTipItemIcon = CreateWidget<CImage>("ToolTipItemIcon");
	m_ToolTipItemIcon->SetPos(53.f, 77.f);


	return true;
}

void CToolTip::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CToolTip::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CToolTip::Render()
{
	CWidgetWindow::Render();
}

CToolTip* CToolTip::Clone()
{
	return new CToolTip(*this);
}
