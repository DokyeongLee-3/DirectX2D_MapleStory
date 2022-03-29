
#include "ClapWarning.h"
#include "ZakumBody.h"

CClapWarning::CClapWarning()	:
	m_OnBright(false)
{
	SetTypeID<CClapWarning>();

	m_LifeSpan = 4.f;
}

CClapWarning::CClapWarning(const CClapWarning& obj)	:
	CGameObject(obj)
{
}

CClapWarning::~CClapWarning()
{
}

void CClapWarning::Start()
{
	CGameObject::Start();
}

bool CClapWarning::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SmashLightSprite");
	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	SetClapWarningSpriteTexture("ClapWarning", TEXT("Monster/ClapWarning.png"));

	return true;
}

void CClapWarning::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!m_Active)
	{
		if (m_WarningPointOwner && m_WarningPointOwner->IsActive())
			m_WarningPointOwner->ZakumClap();

		return;
	}

	float CurrentOpacity = m_Sprite->GetOpacity();

	if (m_OnBright)
	{
		float Result = CurrentOpacity + DeltaTime;
		m_Sprite->SetOpacity(CurrentOpacity + DeltaTime * 2.5f);

		if (Result >= 1.f)
			m_OnBright = false;
	}

	else
	{
		float Result = CurrentOpacity - DeltaTime;
		m_Sprite->SetOpacity(CurrentOpacity - DeltaTime * 2.5f);

		if (Result < 0.1f)
			m_OnBright = true;
	}
}

void CClapWarning::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CClapWarning* CClapWarning::Clone()
{
	return new CClapWarning(*this);
}

void CClapWarning::SetWarningPointOwner(CZakumBody* Zakum)
{
	m_WarningPointOwner = Zakum;
}

void CClapWarning::SetClapWarningSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
