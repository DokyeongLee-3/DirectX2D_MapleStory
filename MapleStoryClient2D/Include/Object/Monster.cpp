
#include "Monster.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "Bill.h"

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

void CMonster::SetTrackState()
{
	m_MonsterState = Monster_State::Track;
}

void CMonster::GiveEXP()
{
	CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

	if (Player)
	{
		int EXP = m_MonsterInfo.Level * 10;

		Player->GetEXP(EXP);
	}
}

void CMonster::Die()
{
	GiveEXP();

	Destroy();
}

void CMonster::DropItem()
{
	CBill* Bill = m_Scene->CreateGameObject<CBill>("Bill");

	Vector3 Pos = GetWorldPos();
	Vector3 Scale = GetWorldScale();

	Bill->SetWorldPos(Pos.x, Pos.y + 10.f, Pos.z);
	
}

void CMonster::SetDamage(float Damage, bool Critical)
{
}

void CMonster::PushDamageFont(float Damage, bool Critical)
{
	((CDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
}
