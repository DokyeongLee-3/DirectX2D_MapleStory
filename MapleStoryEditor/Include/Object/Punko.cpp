
#include "Punko.h"
#include "../Animation/PunkoAnimation.h"

CPunko::CPunko()
{
	SetTypeID<CPunko>();
}

CPunko::CPunko(const CPunko& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PunkoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPunko::~CPunko()
{
}

bool CPunko::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PunkoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CPunkoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(120.f, 170.f, 1.f);

	m_Sprite->SetLayerName("MovingObjFront");

	return true;
}

void CPunko::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CPunko::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CPunko* CPunko::Clone()
{
	return new CPunko(*this);
}

void CPunko::Save(FILE* File)
{
	CMonster::Save(File);
}

void CPunko::Load(FILE* File)
{
	CMonster::Load(File);
}
