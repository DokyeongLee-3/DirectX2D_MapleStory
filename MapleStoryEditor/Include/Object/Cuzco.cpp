
#include "Cuzco.h"
#include "../Animation/CuzcoAnimation.h"

CCuzco::CCuzco()
{
	SetTypeID<CCuzco>();
}

CCuzco::CCuzco(const CCuzco& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("CuzcoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CCuzco::~CCuzco()
{
}

bool CCuzco::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("CuzcoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CCuzcoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(120.f, 170.f, 1.f);

	m_Sprite->SetLayerName("MovingObjFront");

	return true;

	return true;
}

void CCuzco::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CCuzco::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CCuzco* CCuzco::Clone()
{
	return new CCuzco(*this);
}

void CCuzco::Save(FILE* File)
{
	CMonster::Save(File);
}

void CCuzco::Load(FILE* File)
{
	CMonster::Load(File);
}
