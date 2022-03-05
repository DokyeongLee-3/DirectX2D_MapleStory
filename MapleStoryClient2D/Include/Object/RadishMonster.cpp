
#include "RadishMonster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "../Animation/RadishMonsterAnimation.h"
#include "Engine.h"

CRadishMonster::CRadishMonster()
{
	SetTypeID<CRadishMonster>();
}

CRadishMonster::CRadishMonster(const CRadishMonster& obj) :
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("RadishMonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
}

CRadishMonster::~CRadishMonster()
{
}

bool CRadishMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("RadishMonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	m_Body->SetRadius(30.f);

	m_Sprite->AddChild(m_Body);


	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	//m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());
	//m_PaperBurn->SetFinishTime(8.f);
	//m_PaperBurn->SetFinishCallback<CRadishMonster>(this, &CRadishMonster::Die);

	m_Sprite->CreateAnimationInstance<CRadishMonsterAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CRadishMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CRadishMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CRadishMonster* CRadishMonster::Clone()
{
	return new CRadishMonster(*this);
}

void CRadishMonster::Save(FILE* File)
{
	CMonster::Save(File);
}

void CRadishMonster::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("RadishMonsterSprite");

	m_Sprite->SetTransparency(true);
}


