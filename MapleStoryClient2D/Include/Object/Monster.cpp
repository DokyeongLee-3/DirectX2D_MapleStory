
#include "Monster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "MonsterAnimation.h"

CMonster::CMonster()	:
	m_HP(50.f)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("MonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");

	m_Body->AddCollisionCallback<CMonster>(Collision_State::Begin, this, &CMonster::CollisionCallback);

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CMonsterAnimation>();
	CMonsterAnimation* Instance = (CMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<CMonster>("RadishHitLeft", this, &CMonster::ReturnIdle);
	Instance->SetEndFunction<CMonster>("RadishDieLeft", this, &CMonster::Die);
	


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::CollisionCallback(const CollisionResult& result)
{
	m_HP -= 10.f;
	m_Sprite->ChangeAnimation("RadishHitLeft");

	if (m_HP <= 0.f)
	{
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RadishDieLeft");
	}
}

void CMonster::Die()
{
	Destroy();
}

void CMonster::ReturnIdle()
{
	m_Sprite->ChangeAnimation("RadishIdleLeft");
}
