
#include "Monster.h"
#include "../Widget/DamageFont.h"

CMonster::CMonster()	:
	m_IsChanging(false)
{
	m_MonsterState = Monster_State::Idle;
}

CMonster::CMonster(const CMonster& obj) :
	CGameObject(obj)
{
}

CMonster::~CMonster()
{
}


void CMonster::Start()
{
	CGameObject::Start();
}

bool CMonster::Init()
{
	CGameObject::Init();

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

void CMonster::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CMonster::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CMonster::Die()
{
	Destroy();
}


void CMonster::SetDamage(float Damage, bool Critical)
{
}

void CMonster::PushDamageFont(float Damage, bool Critical)
{
	((CDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
}
