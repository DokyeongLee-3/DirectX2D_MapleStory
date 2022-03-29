
#include "ZakumAttackWarningSpot.h"
#include "ZakumBody.h"

CZakumAttackWarningSpot::CZakumAttackWarningSpot()	:
	m_AccTime(0.f),
	m_ColorChangeTime(0.2f),
	m_IsRedBaseColor(false)
{
	SetTypeID<CZakumAttackWarningSpot>();
	m_LifeSpan = 4.5f;
}

CZakumAttackWarningSpot::CZakumAttackWarningSpot(const CZakumAttackWarningSpot& obj)	:
	CGameObject(obj)
{
}

CZakumAttackWarningSpot::~CZakumAttackWarningSpot()
{
}

void CZakumAttackWarningSpot::SetWarningPointOwner(CZakumBody* Zakum)
{
	m_WarningPointOwner = Zakum;
}

void CZakumAttackWarningSpot::Start()
{
	CGameObject::Start();
}

bool CZakumAttackWarningSpot::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("CZakumAttackWarningSpotSprite");

	SetRootComponent(m_Sprite);
	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "ZakumAttackWarningSpot", TEXT("Monster/ZakumAttackWarningSpot1.png"));

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);

	return true;
}

void CZakumAttackWarningSpot::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// WarningSpot이 없어지면 팔이 내려치는 동작 실행
	if (!m_Active)
	{
		if(m_WarningPointOwner && m_WarningPointOwner->IsActive())
			m_WarningPointOwner->ArmSmash(m_ArmID);
	}

	m_AccTime += DeltaTime;

	if (m_AccTime >= m_ColorChangeTime)
	{
		m_AccTime = 0.f;

		if (m_IsRedBaseColor)
		{
			m_IsRedBaseColor = false;
			m_Sprite->SetBaseColor(1.f, 1.f, 1.f, 0.8f);
		}

		else
		{
			m_IsRedBaseColor = true;
			m_Sprite->SetBaseColor(1.f, 0.4f, 0.1f, 0.7f);
		}
	}
}

void CZakumAttackWarningSpot::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumAttackWarningSpot* CZakumAttackWarningSpot::Clone()
{
	return new CZakumAttackWarningSpot(*this);
}
