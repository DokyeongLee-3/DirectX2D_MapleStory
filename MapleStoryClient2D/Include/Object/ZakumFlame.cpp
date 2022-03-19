
#include "ZakumFlame.h"
#include "../Animation/ZakumFlameAnimation.h"

CZakumFlame::CZakumFlame()
{
	SetTypeID<CZakumFlame>();
}

CZakumFlame::CZakumFlame(const CZakumFlame& obj)	:
	CMonster(obj)
{
}

CZakumFlame::~CZakumFlame()
{
}

void CZakumFlame::Start()
{
	CMonster::Start();
}

bool CZakumFlame::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ZakumFlameSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_Body);


	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CZakumFlameAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CZakumFlame::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CZakumFlame::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CZakumFlame* CZakumFlame::Clone()
{
	return new CZakumFlame(*this);
}
