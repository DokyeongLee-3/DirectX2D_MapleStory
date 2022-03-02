
#include "Monster.h"


CMonster::CMonster()
{
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

