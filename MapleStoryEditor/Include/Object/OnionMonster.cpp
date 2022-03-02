
#include "OnionMonster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "../Animation/OnionMonsterAnimation.h"
#include "Engine.h"

COnionMonster::COnionMonster() 
{
	SetTypeID<COnionMonster>();
}

COnionMonster::COnionMonster(const COnionMonster& obj) :
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
}

COnionMonster::~COnionMonster()
{
}

bool COnionMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("OnionMonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	m_Body->SetRadius(30.f);

	m_Sprite->AddChild(m_Body);


	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	//m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());
	//m_PaperBurn->SetFinishTime(8.f);
	//m_PaperBurn->SetFinishCallback<COnionMonster>(this, &COnionMonster::Die);

	m_Sprite->CreateAnimationInstance<COnionMonsterAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void COnionMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void COnionMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

COnionMonster* COnionMonster::Clone()
{
	return new COnionMonster(*this);
}

void COnionMonster::Save(FILE* File)
{
	CMonster::Save(File);
}

void COnionMonster::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");

	m_Sprite->SetTransparency(true);
}


