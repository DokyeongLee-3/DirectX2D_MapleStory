#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Number.h"
#include "Widget/Image.h"

class CDamageFont :
    public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CDamageFont();
	CDamageFont(const CDamageFont& window);
	virtual ~CDamageFont();

private:
	std::list<CSharedPtr<CNumber>>		m_ListDamage;
	std::list<CSharedPtr<CImage>>		m_ListCriticalEffect;
	std::vector<TCHAR*>					m_vecFileName;
	std::vector<TCHAR*>					m_vecCriticalFileName;

public:
	void PushDamageNumber(int Number, bool Critical = false)
	{
		size_t Size = m_ListDamage.size();

		char Name[64] = {};

		if (Critical)
		{
			sprintf_s(Name, "CriDamage%d", (int)Size);
			CNumber* Damage = CreateWidget<CNumber>(Name);
			Damage->SetPos(30.f, 10.f + Size * 35.f);
			Damage->SetTexture(Name, m_vecCriticalFileName);
			Damage->AddFrameData(10);
			Damage->SetNumber(Number);
			Damage->SetRenderStart(false);
			m_ListDamage.push_back(Damage);

			CImage* CriticalEffect = CreateWidget<CImage>("CriticalEffect");
			CriticalEffect->SetTexture("CriticalEffect", TEXT("UI/DamageFont/NoCri1.effect.png"));
			CriticalEffect->SetPos(-10.f, 20.f + Size * 35.f);
			CriticalEffect->SetSize(62.f, 57.f);
			CriticalEffect->SetRenderStart(false);
			m_ListCriticalEffect.push_back(CriticalEffect);

		}

		else
		{
			sprintf_s(Name, "Damage%d", (int)Size);
			CNumber* Damage = CreateWidget<CNumber>(Name);
			Damage->SetPos(30.f, 10.f + Size * 35.f);
			Damage->SetTexture(Name, m_vecFileName);
			Damage->AddFrameData(10);
			Damage->SetNumber(Number);
			Damage->SetRenderStart(false);

			m_ListDamage.push_back(Damage);
		}
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CDamageFont* Clone();
};

