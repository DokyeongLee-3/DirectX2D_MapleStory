
#include "BossWarningText.h"

CBossWarningText::CBossWarningText()	:
	m_AliveTime(3.5f)
{
	m_BossWarningText = TEXT("약화된 자쿰이 가막못의오리님을 가장 위협적인 적으로 간주하고 있습니다.");
}

CBossWarningText::CBossWarningText(const CBossWarningText& window)	:
	CWidgetWindow(window)
{
}

CBossWarningText::~CBossWarningText()
{
}

void CBossWarningText::Start()
{
	CWidgetWindow::Start();
}

bool CBossWarningText::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetPos(220.f, 500.f);
	SetSize(900.f, 200.f);

	m_BossWarning = CreateWidget<CText>("BossWarningText");
	m_BossWarning->SetFont("Thin");
	m_BossWarning->SetSize(900.f, 200.f);
	m_BossWarning->SetPos(0.f, 0.f);
	m_BossWarning->SetFontSize(16.f);
	m_BossWarning->SetColor(0.8313f, 0.55f, 0.1059f);
	m_BossWarning->SetZOrder(1);
	m_BossWarning->SetAlignH(TEXT_ALIGN_H::Center);
	m_BossWarning->SetText(m_BossWarningText.c_str());

	return true;
}

void CBossWarningText::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	if(m_AliveTime > 0.f)
		m_AliveTime -= DeltaTime;

	else if (m_AliveTime <= 0.f)
	{
		float CurrentOpacity = m_BossWarning->GetOpacity();

		if (CurrentOpacity <= 0.f)
			m_Enable = false;
		else
			m_BossWarning->SetOpacity(CurrentOpacity - DeltaTime / 4.f);
	}
}

void CBossWarningText::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CBossWarningText::Render()
{
	CWidgetWindow::Render();
}

CBossWarningText* CBossWarningText::Clone()
{
	return new CBossWarningText(*this);
}
