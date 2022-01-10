
#include "LampLight.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"

CLampLight::CLampLight() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CLampLight>();
}

CLampLight::CLampLight(const CLampLight& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("LampLightSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CLampLight::~CLampLight()
{
}

void CLampLight::Start()
{
	CGameObject::Start();
}

bool CLampLight::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("LampLightSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("MapCollider");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("LampLight.sqc"), ANIMATION_PATH, "LampLight", true, 0.6f);


	return true;
}

void CLampLight::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLampLight::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLampLight* CLampLight::Clone()
{
	return new CLampLight(*this);
}

void CLampLight::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLampLight::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("LampLightSprite");
}

