
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
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("MonsterRadishSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CMonsterRadish::~CMonsterRadish()
{
}

void CMonsterRadish::Start()
{
	CMonster::Start();
}

bool CMonsterRadish::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("MonsterRadishSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);



	//m_Sprite->CreateAnimationInstance<CLowerClassBookAnimation>();
	//COnionMonsterAnimation* Instance = (COnionMonsterAnimation*)m_Sprite->GetAnimationInstance();

	return true;
}

void CMonsterRadish::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CMonsterRadish::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CMonsterRadish* CMonsterRadish::Clone()
{
	return new CMonsterRadish(*this);
}

void CMonsterRadish::Save(FILE* File)
{
	CMonster::Save(File);
}

void CMonsterRadish::Load(FILE* File)
{
	CMonster::Load(File);

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

void CMonsterRadish::RotationZInv(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(100.f * DeltaTime);
}

void CMonsterRadish::RotationZ(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(-100.f * DeltaTime);
}