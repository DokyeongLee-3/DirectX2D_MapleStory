
#include "NPCAdobis.h"
#include "Animation/AnimationSequence2DInstance.h"

CNPCAdobis::CNPCAdobis()
{
	SetTypeID<CNPCAdobis>();
}

CNPCAdobis::CNPCAdobis(const CNPCAdobis& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("NPCSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CNPCAdobis::~CNPCAdobis()
{
}

void CNPCAdobis::Start()
{
	CGameObject::Start();
}

bool CNPCAdobis::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("NPCSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	SetPivot(0.5f, 0.5f, 0.f);

	m_Body->SetExtent(25.f, 35.f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();

	Instance->AddAnimation(TEXT("NPCAdobisIdle.sqc"), ANIMATION_PATH, "NPCAdobisIdle", true, 0.6f);

	return true;
}

void CNPCAdobis::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNPCAdobis::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNPCAdobis* CNPCAdobis::Clone()
{
	return new CNPCAdobis(*this);
}

void CNPCAdobis::CollisionMouseBeginCallback(const CollisionResult& Result)
{
}

void CNPCAdobis::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CNPCAdobis::Load(FILE* File)
{
	CGameObject::Load(File);
}
