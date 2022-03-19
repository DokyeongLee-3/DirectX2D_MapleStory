
#include "ZakumFogEffect.h"

CZakumFogEffect::CZakumFogEffect()
{
	SetTypeID<CZakumFogEffect>();
}

CZakumFogEffect::CZakumFogEffect(const CZakumFogEffect& obj)	:
	CGameObject(obj)
{
}

CZakumFogEffect::~CZakumFogEffect()
{
}

void CZakumFogEffect::Start()
{
	CGameObject::Start();
}

bool CZakumFogEffect::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("DustSprite");
	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	SetRootComponent(m_Sprite);

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Fog", TEXT("Monster/FogEffect.png"));

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);

	return true;
}

void CZakumFogEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(0.f, 20.f * DeltaTime, 0.f);

	float CurrentOpacity = m_Sprite->GetOpacity();

	m_Sprite->SetOpacity(CurrentOpacity - DeltaTime / 3.f);

	if (CurrentOpacity - DeltaTime / 3.f <= 0.f)
		Destroy();
}

void CZakumFogEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumFogEffect* CZakumFogEffect::Clone()
{
	return new CZakumFogEffect(*this);
}
