
#include "BossInfoWindow.h"

CBossInfoWindow::CBossInfoWindow()	:
	m_AccTime(0.f)
{
}

CBossInfoWindow::CBossInfoWindow(const CBossInfoWindow& window)	:
	CWidgetWindow(window)
{
}

CBossInfoWindow::~CBossInfoWindow()
{
}

void CBossInfoWindow::Start()
{
	CWidgetWindow::Start();
}

bool CBossInfoWindow::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetPos(220.f, 630.f);
	SetSize(640.f, 100.f);

	m_HPBarBack = CreateWidget<CImage>("BossHPBack");
	m_HPBarBack->SetTexture("BossHPBack", TEXT("UI/BossHPBackground.png"));
	m_HPBarBack->SetSize(640.f, 20.f);
	m_HPBarBack->SetPos(100.f, 65.f);

	m_HPBar = CreateWidget<CProgressBar>("BossHPBar");
	m_HPBar->SetTexture("BossHPBar", TEXT("UI/MobGage.Gage.3.0.png"));
	m_HPBar->SetPercent(1.f);
	m_HPBar->SetSize(625.f, 10.f);
	m_HPBar->SetPos(106.f, 70.f);
	m_HPBar->SetZOrder(2);

	m_RemainTimeBack = CreateWidget<CImage>("BossRemainTime");
	m_RemainTimeBack->SetTexture("BossRemainTime", TEXT("UI/BossRemainTime.png"));
	m_RemainTimeBack->SetSize(212.f, 65.f);
	m_RemainTimeBack->SetPos(320.f, 0.f);

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("UI/bossPapulatus.alarm.num.%d.png"), i);

		vecFileName.push_back(FileName);
	}

	m_RemainTimeMinuteDigit1 = CreateWidget<CNumber>("RemainTimeMinuteDigit1");
	m_RemainTimeMinuteDigit1->SetTexture("RemainTimeMinuteDigit1", vecFileName);
	m_RemainTimeMinuteDigit1->SetSize(14.f, 20.f);
	m_RemainTimeMinuteDigit1->SetPos(410.f, 23.f);
	m_RemainTimeMinuteDigit1->SetNumber(2);
	m_RemainTimeMinuteDigit1->SetZOrder(2);

	m_RemainTimeMinuteDigit2 = CreateWidget<CNumber>("RemainTimeMinuteDigit2");
	m_RemainTimeMinuteDigit2->SetTexture("RemainTimeMinuteDigit2", vecFileName);
	m_RemainTimeMinuteDigit2->SetSize(14.f, 20.f);
	m_RemainTimeMinuteDigit2->SetPos(422.f, 23.f);
	m_RemainTimeMinuteDigit2->SetNumber(0);
	m_RemainTimeMinuteDigit2->SetZOrder(2);

	m_RemainTimeSecDigit1 = CreateWidget<CNumber>("RemainTimeSecDigit1");
	m_RemainTimeSecDigit1->SetTexture("RemainTimeSecDigit1", vecFileName);
	m_RemainTimeSecDigit1->SetSize(14.f, 20.f);
	m_RemainTimeSecDigit1->SetPos(470.f, 22.f);
	m_RemainTimeSecDigit1->SetNumber(0);
	m_RemainTimeSecDigit1->SetZOrder(2);

	m_RemainTimeSecDigit2 = CreateWidget<CNumber>("RemainTimeSecDigit2");
	m_RemainTimeSecDigit2->SetTexture("RemainTimeSecDigit2", vecFileName);
	m_RemainTimeSecDigit2->SetSize(14.f, 20.f);
	m_RemainTimeSecDigit2->SetPos(484.f, 22.f);
	m_RemainTimeSecDigit2->SetNumber(0);
	m_RemainTimeSecDigit2->SetZOrder(2);


	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	return true;
}

void CBossInfoWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	m_AccTime += DeltaTime;

	if (m_AccTime >= 1.f)
	{
		m_AccTime = 0.f;

		if (m_RemainTimeSecDigit2->GetNumber() == 0)
		{
			m_RemainTimeSecDigit2->SetNumber(9);

			if (m_RemainTimeSecDigit1->GetNumber() == 0)
			{
				m_RemainTimeSecDigit1->SetNumber(5);

				if (m_RemainTimeMinuteDigit2->GetNumber() == 0)
				{
					m_RemainTimeMinuteDigit2->SetNumber(9);

					if (m_RemainTimeMinuteDigit1->GetNumber() == 0)
					{
						int a = 3;
					}

					else
						m_RemainTimeMinuteDigit1->AddNumber(-1);

				}

				else
					m_RemainTimeMinuteDigit2->AddNumber(-1);
			}

			else
				m_RemainTimeSecDigit1->AddNumber(-1);
		}

		else
			m_RemainTimeSecDigit2->AddNumber(-1);
	}
}

void CBossInfoWindow::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CBossInfoWindow::Render()
{
	CWidgetWindow::Render();
}

CBossInfoWindow* CBossInfoWindow::Clone()
{
	return new CBossInfoWindow(*this);
}

void CBossInfoWindow::BossHPPercent(float Percent)
{
	m_HPBar->SetPercent(Percent);
}
