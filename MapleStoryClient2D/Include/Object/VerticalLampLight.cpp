
#include "VerticalLampLight.h"
#include "Animation/AnimationSequence2DInstance.h"

CVerticalLampLight::CVerticalLampLight()
{
	SetTypeID<CVerticalLampLight>();
}

CVerticalLampLight::CVerticalLampLight(const CVerticalLampLight& obj) :
	CGameObject(obj)
{
	m_RootComponent = (CSpriteComponent*)FindComponent("VerticalLampLightRoot");
	m_LeftSprite = (CSpriteComponent*)FindComponent("VerticalLampLightLeftSprite");
	m_RightSprite = (CSpriteComponent*)FindComponent("VerticalLampLightRightSprite");
	//m_Body = (CColliderBox2D*)FindComponent("Body");
}

CVerticalLampLight::~CVerticalLampLight()
{
}

void CVerticalLampLight::Start()
{
	CGameObject::Start();
}

bool CVerticalLampLight::Init()
{
	m_RootComponent = CreateComponent<CSceneComponent>("VerticalLampLightRoot");
	m_LeftSprite = CreateComponent<CSpriteComponent>("VerticalLampLightLeftSprite");
	m_RightSprite = CreateComponent<CSpriteComponent>("VerticalLampLightRightSprite");


	SetRootComponent(m_RootComponent);
	m_RootComponent->AddChild(m_LeftSprite);
	m_RootComponent->AddChild(m_RightSprite);


	SetRootComponent(m_RootComponent);

	m_LeftSprite->SetTransparency(true);

	m_LeftSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_LeftSprite->SetWorldScale(100.f, 100.f, 1.f);
	m_LeftSprite->SetRelativePos(500.f, 300.f, 0.f);
	m_LeftSprite->SetPivot(0.5f, 0.5f, 0.f);

	m_RightSprite->SetTransparency(true);

	m_RightSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_RightSprite->SetWorldScale(100.f, 100.f, 1.f);
	m_RightSprite->SetRelativePos(500.f, 300.f, 0.f);
	m_RightSprite->SetPivot(0.5f, 0.5f, 0.f);



	CAnimationSequence2DInstance* Anim = m_LeftSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("VerticalLampLight.sqc"), ANIMATION_PATH, "VerticalLampLight", true, 1.3f);

	Anim = m_RightSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("VerticalLampLight.sqc"), ANIMATION_PATH, "VerticalLampLight", true, 1.3f);
	m_RightSprite->Flip();

	return true;
}

void CVerticalLampLight::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CVerticalLampLight::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CVerticalLampLight* CVerticalLampLight::Clone()
{
	return new CVerticalLampLight(*this);
}

void CVerticalLampLight::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CVerticalLampLight::Load(FILE* File)
{
	CGameObject::Load(File);

	m_LeftSprite = (CSpriteComponent*)FindComponent("VerticalLampLightLeftSprite");

	m_LeftSprite->SetTransparency(true);

	m_LeftSprite->GetCurrentAnimation()->SetPlayTime(1.3f);

	m_RightSprite = (CSpriteComponent*)FindComponent("VerticalLampLightRightSprite");

	m_RightSprite->SetTransparency(true);

	m_RightSprite->GetCurrentAnimation()->SetPlayTime(1.3f);

	m_RightSprite->Flip();
}

