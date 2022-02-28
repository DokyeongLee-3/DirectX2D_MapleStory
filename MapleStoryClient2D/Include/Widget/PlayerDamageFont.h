#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Number.h"
#include "Widget/Image.h"

class CPlayerDamageFont	:
	public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CPlayerDamageFont();
	CPlayerDamageFont(const CPlayerDamageFont& window);
	virtual ~CPlayerDamageFont();

private:
	std::list<CSharedPtr<CNumber>>		m_ListDamage;
	std::vector<TCHAR*>					m_vecFileName;

public:
	void PushDamageNumber(int Number, bool Critical = false)
	{
		size_t Size = m_ListDamage.size();

		char Name[64] = {};

		sprintf_s(Name, "PlayerDamage%d", (int)Size);
		CNumber* Damage = CreateWidget<CNumber>(Name);
		Damage->SetPos(-24.f, 45.f + Size * 35.f);
		Damage->SetTexture(Name, m_vecFileName);
		Damage->AddFrameData(10);
		Damage->SetNumber(Number);

		m_ListDamage.push_back(Damage);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CPlayerDamageFont* Clone();
};

