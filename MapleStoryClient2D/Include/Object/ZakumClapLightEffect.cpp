
#include "ZakumClapLightEffect.h"
#include "ZakumClapLightEffect.h"
#include "ZakumBody.h"

CZakumClapLightEffect::CZakumClapLightEffect() :
	m_OwnerArmID(-1)
{
	SetTypeID<CZakumClapLightEffect>();
}

CZakumClapLightEffect::CZakumClapLightEffect(const CZakumClapLightEffect& obj) :
	CGameObject(obj)
{
}

CZakumClapLightEffect::~CZakumClapLightEffect()
{
}

void CZakumClapLightEffect::SetSmashLightOwner(CZakumBody* Zakum)
{
	m_SmashLightOwner = Zakum;
}

void CZakumClapLightEffect::Start()
{
	CGameObject::Start();
}

bool CZakumClapLightEffect::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ClapLightSprite");
	SetRootComponent(m_Sprite);

	SetLightSpriteTexture("ClapLight", TEXT("Monster/ClapLightEffect.png"));

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetOpacity(0.85f);

	return true;
}

void CZakumClapLightEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float CurrentOpacity = m_Sprite->GetOpacity();

	m_Sprite->SetOpacity(CurrentOpacity - DeltaTime / 5.f);

	if (m_Active && CurrentOpacity - DeltaTime / 5.f <= 0.f)
	{
		//m_SmashLightOwner->ReturnArm(m_OwnerArmID);
		Destroy();
	}
}

void CZakumClapLightEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumClapLightEffect* CZakumClapLightEffect::Clone()
{
	return new CZakumClapLightEffect(*this);
}

void CZakumClapLightEffect::SetLightSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
