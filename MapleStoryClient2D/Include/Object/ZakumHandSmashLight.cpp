
#include "ZakumHandSmashLight.h"
#include "ZakumBody.h"

CZakumHandSmashLight::CZakumHandSmashLight()	:
	m_OwnerArmID(-1)
{
	SetTypeID<CZakumHandSmashLight>();
}

CZakumHandSmashLight::CZakumHandSmashLight(const CZakumHandSmashLight& obj)	:
	CGameObject(obj)
{
}

CZakumHandSmashLight::~CZakumHandSmashLight()
{
}

void CZakumHandSmashLight::SetSmashLightOwner(CZakumBody* Zakum)
{
	m_SmashLightOwner = Zakum;
}

void CZakumHandSmashLight::Start()
{
	CGameObject::Start();
}

bool CZakumHandSmashLight::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SmashLightSprite");
	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetOpacity(0.8f);

	return true;
}

void CZakumHandSmashLight::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float CurrentOpacity = m_Sprite->GetOpacity();

	m_Sprite->SetOpacity(CurrentOpacity - DeltaTime / 5.f);

	if (m_Active && CurrentOpacity - DeltaTime / 5.f <= 0.f)
	{
		m_SmashLightOwner->SmashReturnArm(m_OwnerArmID);
		Destroy();
	}
}

void CZakumHandSmashLight::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumHandSmashLight* CZakumHandSmashLight::Clone()
{
	return new CZakumHandSmashLight(*this);
}

void CZakumHandSmashLight::SetLightSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
