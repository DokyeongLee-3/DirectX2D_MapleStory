
#include "Puko.h"
#include "../Animation/PukoAnimation.h"

CPuko::CPuko()
{
	SetTypeID<CPuko>();
}

CPuko::CPuko(const CPuko& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PukoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPuko::~CPuko()
{
}

bool CPuko::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PukoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CPukoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(120.f, 170.f, 1.f);

	m_Sprite->SetLayerName("MovingObjFront");

	return true;
}

void CPuko::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CPuko::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CPuko* CPuko::Clone()
{
	return new CPuko(*this);
}

void CPuko::Save(FILE* File)
{
	CMonster::Save(File);
}

void CPuko::Load(FILE* File)
{
	CMonster::Load(File);
}
