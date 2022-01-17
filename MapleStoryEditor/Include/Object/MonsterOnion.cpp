
#include "MonsterOnion.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CMonsterOnion::CMonsterOnion() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CMonsterOnion>();
}

CMonsterOnion::CMonsterOnion(const CMonsterOnion& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("MonsterOnionSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CMonsterOnion::~CMonsterOnion()
{
}

void CMonsterOnion::Start()
{
	CGameObject::Start();
}

bool CMonsterOnion::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("MonsterOnionSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("OnionIdleLeft.sqc"), ANIMATION_PATH, "OnionIdleLeft", true, 1.f);
	Anim->AddAnimation(TEXT("OnionWalkLeft.sqc"), ANIMATION_PATH, "OnionWalkLeft", true, 1.f);
	Anim->AddAnimation(TEXT("OnionHitLeft.sqc"), ANIMATION_PATH, "OnionHitLeft", true, 1.f);
	Anim->AddAnimation(TEXT("OnionDieLeft.sqc"), ANIMATION_PATH, "OnionDieLeft", true, 1.f);

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CMonsterOnion>("Flip", KeyState_Down, this, &CMonsterOnion::FlipAll);
	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CMonsterOnion>("NextAnimation", KeyState_Down, this, &CMonsterOnion::NextAnimation);
	CInput::GetInst()->CreateKey("Rotate", VK_RIGHT);
	CInput::GetInst()->SetKeyCallback<CMonsterOnion>("Rotate", KeyState_Push, this, &CMonsterOnion::RotationZ);
	CInput::GetInst()->CreateKey("RotateInv", VK_LEFT);
	CInput::GetInst()->SetKeyCallback<CMonsterOnion>("RotateInv", KeyState_Push, this, &CMonsterOnion::RotationZInv);

	return true;
}

void CMonsterOnion::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonsterOnion::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonsterOnion* CMonsterOnion::Clone()
{
	return new CMonsterOnion(*this);
}

void CMonsterOnion::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CMonsterOnion::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("MonsterOnionSprite");
}

void CMonsterOnion::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CMonsterOnion::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();
}

void CMonsterOnion::RotationZInv(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(100.f * DeltaTime);
}

void CMonsterOnion::RotationZ(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(-100.f * DeltaTime);
}