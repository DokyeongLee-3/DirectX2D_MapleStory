
#include "ZakumDustEffect.h"

CZakumDustEffect::CZakumDustEffect()
{
	SetTypeID<CZakumDustEffect>();
}

CZakumDustEffect::CZakumDustEffect(const CZakumDustEffect& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("DustSprite");
}

CZakumDustEffect::~CZakumDustEffect()
{
}


void CZakumDustEffect::Start()
{
	CGameObject::Start();
}

bool CZakumDustEffect::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("DustSprite");
	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	SetRootComponent(m_Sprite);

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Dust", TEXT("Monster/DustEffect.png"));

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);

	return true;
}

void CZakumDustEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float CurrentOpacity = m_Sprite->GetOpacity();

	m_Sprite->SetOpacity(CurrentOpacity - DeltaTime / 3.f);

	if (CurrentOpacity - DeltaTime / 3.f <= 0.f)
		Destroy();
}

void CZakumDustEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumDustEffect* CZakumDustEffect::Clone()
{
	return new CZakumDustEffect(*this);
}

void CZakumDustEffect::SetDestSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
