
#include "MonsterRadish.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CMonsterRadish::CMonsterRadish()	:
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CMonsterRadish>();
}

CMonsterRadish::CMonsterRadish(const CMonsterRadish& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("MonsterRadishSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CMonsterRadish::~CMonsterRadish()
{
}

void CMonsterRadish::Start()
{
	CGameObject::Start();
}

bool CMonsterRadish::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("MonsterRadishSprite");
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
	
	Anim->AddAnimation(TEXT("RadishWalkLeft.sqc"), ANIMATION_PATH, "RadishWalkLeft", true, 1.f);
	Anim->AddAnimation(TEXT("RadishIdleLeft.sqc"), ANIMATION_PATH, "RadishIdleLeft", true, 1.f);
	Anim->AddAnimation(TEXT("RadishHitLeft.sqc"), ANIMATION_PATH, "RadishHitLeft", true, 1.f);
	Anim->AddAnimation(TEXT("RadishDieLeft.sqc"), ANIMATION_PATH, "RadishDieLeft", true, 3.f);


	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CMonsterRadish>("Flip", KeyState_Down, this, &CMonsterRadish::FlipAll);
	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CMonsterRadish>("NextAnimation", KeyState_Down, this, &CMonsterRadish::NextAnimation);

	return true;
}

void CMonsterRadish::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 Pos = m_Sprite->GetWorldPos();

	int a = 3;
}

void CMonsterRadish::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonsterRadish* CMonsterRadish::Clone()
{
	return new CMonsterRadish(*this);
}

void CMonsterRadish::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CMonsterRadish::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("MonsterRadishSprite");
}

void CMonsterRadish::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CMonsterRadish::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();
}